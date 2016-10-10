  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This module may be controlled via these
  ! preprocessor variables:
  !  - BUD_MOD_NAME name of the module
  !  - BUD_TYPE_NAME name of the public type
  !  - BUD_TYPE_NAME_ name of the internal data pointer
  !  - BUD_TYPE_NAME_STR the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW the routine name for creating a new type
  !
  ! Additionally the CSC pattern format may be
  ! controlled via these flags:
  !  - BUD_SP_CSC
  !    case ( 0 ) ! default
  !      standard CSC format (MKL-SpBLAS)
  !    case ( 1 )
  !      zero based pointer CSC format
  !      The row-pointer is zero based
  !      This will have an additional array with
  !      the number of non-zero elements per row
  !        (this is equivalent to the SIESTA sparsity)
  !      This does not allow C-interoperability (do NOT set
  !      BUD_SP_INTEROP_C in this case)
  !
  !  - BUD_SP_INTEROP_C=0|1
  !    Use C-indexing with BUD_SP_INTEROP_C == 1
  !    All subsequent operations correspond to a
  !    C-index method.
  !    Cannot be used together with BUD_SP_CSR == 1
  
#ifndef BUD_SP_CSC
# define BUD_SP_CSC 0
#endif

#ifndef BUD_SP_INTEROP_C
# define BUD_SP_INTEROP_C 0
#endif

#if BUD_SP_CSC == 0
#elif BUD_SP_CSC == 1
# if BUD_SP_INTEROP_C == 1
#  error "SP_INTEROP_C may not be defined when using SP_CSC == 1"
# endif
#else
# error "SP_CSC *MUST* be either 0 or 1"
#endif

  
#if BUD_SP_INTEROP_C == 0
# define BUD_SP_MOD modp
#elif BUD_SP_INTEROP_C == 1
# define BUD_SP_MOD mod
#else
# error "SP_INTEROP_C *MUST* be either 0 or 1"
#endif

  
#include "bud_utils.inc"
  
#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
  
  use BUD_CC3(BUD_MOD,_,SP_generic)
  
  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Sparse pattern type in the CSR format
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER
    
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER
    
#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @name Private procedures
    !> @{
    ! Doxygen needed line

    procedure, private :: new_dim_ => new_dim_
    procedure, private :: new_copy_ => new_copy_

    procedure, private :: rowp_
    procedure, private :: row_cp_

    !> @}
    
    !> @iSee BUD_TYPE_NEW
    generic, public :: BUD_TYPE_NEW => new_dim_, new_copy_
    !> @iSee BUD_TYPE_NEW
    generic, public :: new => new_dim_, new_copy_

    !> @iSee #nonzeros
    procedure, public :: nonzeros => nonzeros_

    !> @iSee #rows
    procedure, public :: rows => rows_

    !> @iSee #columns
    procedure, public :: columns => columns_

    !> @iSee #size
    procedure, public :: size => size_

    !> @iSee #index
    procedure, public :: index => index_
    
    !> @iSee #offset_p
    procedure, public :: offset_p => offsetp_

#if BUD_SP_CSR == 1
    !> @iSee #ncolumn_p
    procedure, public :: ncolumn_p => ncolumnp_
#endif

    !> @iSee #row_p
    generic, public :: row_p => rowp_, row_cp_

    
    !> @iSee #fold
    procedure, public :: fold => fold_

    !> @iSee #sort
    procedure, public :: sort => sort_

    !> @iSee #attach
    procedure, public :: attach => attach_

#endif
  end type BUD_TYPE_NAME

  
  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains the sparsity pattern for a CSR pattern.
  type BUD_TYPE_NAME_
    
    !> Number of rows in the pattern
    integer(BUD_TYPE_VAR_PREC) :: nr = 0
    !> Number of columns in the pattern
    integer(BUD_TYPE_VAR_PREC) :: nc = 0
    !> Number of non-zero elements in the sparse pattern
    integer(BUD_TYPE_VAR_PREC) :: nz = 0
    
    !> Whether the sparse pattern has been sorted, see #sort_sp
    logical :: sorted = .false.
    
    !> Index of the equivalent column (size `nr+1`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: cptr(:) => null()

#if BUD_SP_CSC == 1
    !> Number of non-zero elements per column (`ncol(i)=cptr(i+1)-cptr(i)`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ncol(:) => null()
#endif

    !> The row index of the equivalent sparse pattern (size `nz`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: row(:) => null()


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER
  
  
  !> Create a new sparse pattern
  !!
  !! You may either create an empty sparse pattern with
  !! a fixed size, or create a sparse pattern from
  !! existing data.
  interface BUD_TYPE_NEW
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: BUD_TYPE_NEW
  
  
  !> @iSee BUD_TYPE_NEW
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: new

  

  !> Retrieve a pointer to the column offsets/pointers
  !!
  !! A pointer with the column offsets.
  !!
#if BUD_SP_CSC == 0
# if BUD_SP_INTEROP_C == 1
  !! `M(row(pointer(ic)+1),ic)` is the first sparse
  !! element in column `ic`.
# else
  !! `M(row(pointer(ic)),ic)` is the first sparse
  !! element in column `ic`.
# endif
#elif BUD_SP_CSC == 1
  !! `M(row(pointer(ic)+1),ic)` is the first sparse
  !! element in column `ic`.
#endif
  interface offset_p
    module procedure offsetp_
  end interface
  public :: offset_p

  
  !> Retrieve a pointer to the row indices in the sparse pattern
  interface row_p
    module procedure rowp_
    module procedure row_cp_
  end interface
  public :: row_p

#if BUD_SP_CSR == 1
  !> Retrieve a pointer to the number of entries per column
  interface ncolumn_p
    module procedure ncolumnp_
  end interface
  public :: ncolumn_p
#endif

  
  !> Fold a sparse pattern to form a smaller sparse pattern
  !!
#if BUD_SP_CSC == 0
  !! Reduce the size of a sparse pattern by folding `ir -> mod(ir-1, nnr) + 1`
#elif BUD_SP_CSC == 1
  !! Reduce the size of a sparse pattern by folding `ir -> mod(ir-1, nnr) + 1`
#endif
  !! with `nnr` being the new number of rows.
  !!
  !! If the folding reduces the pattern dimensions by half
  !! (for a pattern with even number of rows and columns)
  !! it will correspond to the following operation:
  !! \f{align*}{
  !!  \mathbf M_{\mathrm{old}} &=
  !!   \begin{bmatrix}
  !!     \mathbf A & \mathbf B \\{}
  !!     \mathbf C & \mathbf D
  !!   \end{bmatrix} \\{}
  !!  \mathbf M_{\mathrm{new}} &=
  !!   \mathbf A + \mathbf B + \mathbf C + \mathbf D
  !! \f}
  !!
  !! Both the number of rows and/or columns may be changed
  !! individually.
  interface fold
    module procedure fold_
  end interface 
  public :: fold

  
  !> Sorts rows in the sparse pattern
  !!
  !! Sorts the sparse pattern such that the row
  !! index is always increasing for each column.
  !! This will generally allow faster access patterns.
  !!
  !! @note
  !! An integer error is return which should be zero
  !! if no errors occurs.
  !!
  !! If the `err` value is non-zero the following table
  !! may be used:
  !!
  !!  - `err == SP_INPUT + 3`:
  !!    the 3rd input is erroneously supplied.
  interface sort
    module procedure sort_
  end interface
  public :: sort

  
  ! Include the common elements of a sparsity method
# include "SP_common.inc"

  
  !> @cond BUD_DEVELOPER
  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  pure subroutine delete_(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this

    this%nr = 0
    this%nc = 0
    this%nz = 0
    this%sorted = .false.

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. associated(this%row) ) return
    deallocate(this%cptr)
    nullify(this%cptr)
    deallocate(this%row)
    nullify(this%row)
#if BUD_SP_CSR == 1
    deallocate(this%ncol)
    nullify(this%ncol)
#endif
    
  end subroutine delete_
  
  !> @endcond BUD_DEVELOPER

  
  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full copied data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    
    call delete(to)
    if ( .not. is_initd(from) ) return
    
    call new(to, from%D%nr, from%D%nc, from%D%nz, &
      from%D%cptr, from%D%row)
    to%D%sorted = from%D%sorted
    
    ! Copy the common data-content
    call common_copy_(from, to)

  end subroutine copy_

  
  !> @param[inout] this the sparse pattern
  !! @param[in] nr number of rows of the pattern
  !! @param[in] nc number of columns of the pattern
  !! @param[in] nz number of non-zero elements the pattern
  subroutine new_dim_(this, nr, nc, nz)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz

    call initialize(this)

    this%D%nr = nr
    this%D%nc = nc
    this%D%nz = nz
    this%D%sorted = .false.
    
    allocate(this%D%cptr(nc+1))
#if BUD_SP_CSC == 1
    allocate(this%D%ncol(nc))
#endif

    allocate(this%D%row(nz))

  end subroutine new_dim_

  
  !> @param[inout] this the new sparse pattern
  !! @param[in] nr number of rows of the pattern
  !! @param[in] nc number of columns of the pattern
  !! @param[in] nz number of non-zero elements of the pattern
  !! @param[in] cptr column pointers (at least size `nc`)
  !! @param[in] row row-indices for the sparse elements
  subroutine new_copy_(this, nr, nc, nz, cptr, row)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: cptr(nc), row(nz)

#if BUD_SP_CSC == 1
    integer(BUD_TYPE_VAR_PREC) :: ic
#endif

    ! Copy over information
#if BUD_SP_CSC == 0
    ! It must be based on the starting index
# if BUD_SP_INTEROP_C == 0
    if ( cptr(1) /= 1 ) then
      call delete(this)
      return
    end if
# else
    if ( cptr(1) /= 0 ) then
      call delete(this)
      return
    end if
# endif
    
#elif BUD_SP_CSC == 1
    ! It must be 0-based
    if ( cptr(1) /= 0 ) then
      call delete(this)
      return
    end if
#endif

    ! pre-allocate
    call new(this, nr, nc, nz)

    ! Copy pointers
    this%D%cptr(1:this%D%nc) = cptr

    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
#if BUD_SP_CSC == 0

# if BUD_SP_INTEROP_C == 0
    this%D%cptr(this%D%nc+1) = this%D%nz + 1
# else
    this%D%cptr(this%D%nc+1) = this%D%nz
# endif
    
#elif BUD_SP_CSC == 1
    this%D%cptr(this%D%nc+1) = this%D%nz

    do ic = 1 , nc
      this%D%ncol(ic) = this%D%cptr(ic+1) - this%D%cptr(ic)
    end do
    
#endif

    this%D%row = row

  end subroutine new_copy_


  !> @param[in] this sparse pattern
  !! @return a pointer to the column offsets for the sparse pattern (contiguous)
  function offsetp_(this) result(cptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: cptr(:)
    
    cptr => this%D%cptr
    
  end function offsetp_

  
  !> @param[in] this sparse pattern
  !! @return a pointer to the row indices for the sparse pattern (contiguous)
  function rowp_(this) result(row)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: row(:)

    row => this%D%row
    
  end function rowp_

  !> @param[in] this sparse pattern
  !> @param[in] c only retrieve the rows that reside in column `c`
  !! @return a pointer to the row indices for the sparse pattern (contiguous)
  function row_cp_(this,c) result(row)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: c
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: row(:)

#if BUD_SP_CSR == 0
# if BUD_SP_INTEROP_C == 0
    row => this%D%row(this%D%cptr(c):this%D%cptr(c+1)-1)
# else
    row => this%D%row(this%D%cptr(c)+1:this%D%cptr(c+1))
# endif
#elif BUD_SP_CSR == 1
    row => this%D%row(this%D%cptr(c)+1:this%D%cptr(c+1))
#endif
    
  end function row_cp_
  
#if BUD_SP_CSR == 1
  !> @param[in] this sparse matrix
  !! @return a pointer to the array that holds the number of entries per column
  function ncolumnp_(this) result(ncol)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ncol(:)
    
    ncol => this%D%ncol
    
  end function ncolumnp_
#endif
  
  !> @param[in] this sparse pattern @bud
  !! @param[out] nr @opt number of rows in SP
  !! @param[out] nc @opt number of columns in SP
  !! @param[out] nz @opt number of non-zero elements in SP
  !! @param[out] cptr @opt column pointer (`cptr(2)` is starting index of `ic=2`)
  !! @param[out] row @opt row index
#if BUD_SP_CSC == 1
  !! @param[out] ncol @opt number of non-zero elements per column
#endif
  subroutine attach_(this, D, nr, nc, nz, cptr, row &
#if BUD_SP_CSC == 1
    , ncol &
#endif
    )
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: cptr(:), row(:)
#if BUD_SP_CSC == 1
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: ncol(:)
#endif

    if ( present(nr) ) nr = this%D%nr
    if ( present(nc) ) nc = this%D%nc
    if ( present(nz) ) nz = this%D%nz
    if ( present(cptr) ) cptr => this%D%cptr
    if ( present(row) ) row => this%D%row
#if BUD_SP_CSC == 1
    if ( present(ncol) ) ncol => this%D%ncol
#endif
    
  end subroutine attach_


  !> @param[in] from the originating sparse pattern
  !! @param[in] nr new number of rows in the sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] single @opt=.false. if .true.: will compress equivalent entries
  !!                  to a single entry, no double entries will be found subsequently. If .false. the order of elements will be the same
  subroutine fold_(from, nr, nc, to, single)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: single

    type(BUD_TYPE_NAME) :: tmp

    if ( nc == columns(from) ) then
      
      ! Fast-skip to the easier code if input
      ! is easy.
      call fold_row_only(from, nr, to, single = single)
      
    else

      call delete(to)
      call fold_col_only(from, nc, tmp)
      call fold_row_only(tmp, nr, to, single = single)
      call delete(tmp)
      
    end if

  end subroutine fold_

  
  !> @cond BUD_DEVELOPER

  !> @param[in] from the originating sparse pattern
  !! @param[in] nr new number of rows in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] single @opt=.false. if .true.: will compress equivalent entries
  !!                  to a single entry, no double entries will be found subsequently.
  subroutine fold_row_only(from, nr, to, single)
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick, modp

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: single

    ! Local single variable
    logical :: lsingle

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnc, fnz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), frow(:), fsow(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:), trow(:)
#if BUD_SP_CSC == 1
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tcol(:)
#endif
    
    integer(BUD_TYPE_VAR_PREC) :: ic, i, ptr
    integer(BUD_TYPE_VAR_PREC), pointer :: ind

    integer(BUD_TYPE_VAR_PREC), allocatable :: tmprow(:)

    lsingle = .false.
    if ( present(single) ) lsingle = single

    
    ! Initialize the arrays from the originating
    ! sparse pattern
    call attach(from, nr=fnr, nc=fnc, nz=fnz, &
      cptr=fptr, row=frow)

    if ( .not. lsingle ) then
      ! Here we do allow dublicate entries
      ! Sorting out these values will be the
      ! responsibility of the programmer.
      
      ! Create new folded pattern
      call new(to, nr, fnc, fnz)

#if BUD_SP_CSC == 0
      call attach(to, cptr=tptr, row=trow)

# if BUD_SP_INTEROP_C == 0
      tptr(1) = 1
# else
      tptr(1) = 0
# endif
      ptr = 0

      ! loop on the folded columns
      do ic = 1 , fnc
        ! loop on entries in the current (old) column
        do i = fptr(ic) , fptr(ic+1) - 1
          ptr = ptr + 1
          trow(ptr) = BUD_SP_MOD(frow(i), nr)
        end do
# if BUD_SP_INTEROP_C == 0
        ! update the following pointer
        tptr(ic+1) = ptr + 1
# endif        
      end do

#elif BUD_SP_CSC == 1
      call attach(to, cptr=tptr, row=trow, ncol=tcol)

      tptr(1) = 0
      ptr = 0
      
      ! loop on the folded columns
      do ic = 1 , fnc
        ! loop on entries in the current column
        do i = fptr(ic) + 1 , fptr(ic+1)
          ptr = ptr + 1
          trow(ptr) = BUD_SP_MOD(frow(i), nr)
        end do
        ! update the following pointer
        tptr(ic+1) = ptr
        tcol(ic) = tptr(ic+1) - tptr(ic)
      end do

#endif
      
    else
      ! We will limit the number of entries
      ! by removing all dublicated entries

      ! First calculate the size of the pattern
      nullify(tptr,trow)
      allocate(tptr(fnc+1),trow(fnz))
      allocate(tmprow(fptr(2)-fptr(1)))

#if BUD_SP_CSC == 0

# if BUD_SP_INTEROP_C == 0
      tptr(1) = 1
# else
      tptr(1) = 0
# endif

      ! loop on the new columns
      do ic = 1 , fnc
        ! point to the following columns pointer
        ! this enables to count the number of entries
        ! in the current column as:
        !   number entries = tptr(ic+1) - tptr(ic)
        ind => tptr(ic+1)
        
        ! initialize counter for the following column
        ! (i.e. count number of entries)
        ind = tptr(ic)
        if ( fptr(ic+1) - fptr(ic) == 0 ) cycle

        ! ensure pivoting array long enough
        call realloc_tmprow(ic)

        ! Point to the current row indices
# if BUD_SP_INTEROP_C == 0
        fsow => frow(fptr(ic):)
# else
        fsow => frow(fptr(ic)+1:)
# endif
                
        ! Create pivoting array so that we
        ! may easily find the reduced sparsity pattern
        call sort_quick(fptr(ic+1)-fptr(ic), &
          fsow, tmprow )

        ! initialize the first element
# if BUD_SP_INTEROP_C == 1
        ind = ind + 1
# endif
        trow(ind) = BUD_SP_MOD(fsow(tmprow(1)), nr)
# if BUD_SP_INTEROP_C == 0
        ind = ind + 1
# endif
        do i = 2 , fptr(ic+1) - fptr(ic)
          ! loop on entries in the current (old) row
          ! check that the variable does not exist
          if ( fsow(tmprow(i-1)) /= fsow(tmprow(i)) ) then
# if BUD_SP_INTEROP_C == 1
            ind = ind + 1
# endif
            trow(ind) = BUD_SP_MOD(fsow(tmprow(i)), nr)
# if BUD_SP_INTEROP_C == 0
            ind = ind + 1
# endif
          end if
        end do
        
      end do

      ! ensure de-allocation
      deallocate(tmprow)
      
      ! create the new sparsity pattern
      i = tptr(fnc+1)
      ! Correct number of non-zero elements in case there
      ! are elements in the last column
      ! I.e. if there are no elements in the last column, then
      !   tptr(fnc) == tptr(fnc+1)
      if ( tptr(fnc) /= i ) i = i - 1
      call new(to, nr, fnc, i, tptr, trow)

#elif BUD_SP_CSC == 1

      tptr(1) = 0
      
      do ic = 1 , fnc
        ind => tptr(ic+1)
        ind = tptr(ic)
        if ( fptr(ic+1) - fptr(ic) == 0 ) cycle

        call realloc_tmprow(ic)

        fsow => frow(fptr(ic)+1:)
          
        call sort_quick(fptr(ic+1)-fptr(ic), &
          fsow, tmprow )

        ind = ind + 1
        trow(ind) = BUD_SP_MOD(fsow(tmprow(1)), nr)
        do i = 2 , fptr(ic+1) - fptr(ic)
          if ( fsow(tmprow(i-1)) /= fsow(tmprow(i)) ) then
            ind = ind + 1
            trow(ind) = BUD_SP_MOD(fsow(tmprow(i)), nr)
          end if
        end do

      end do

      ! ensure de-allocation
      deallocate(tmprow)
      
      ! create the new sparsity pattern
      call new(to, nr, fnc, tptr(fnc+1), tptr, trow)

#endif
      
    end if

  contains

    ! Reallocate the temporary row indices in
    ! a sorted array
    subroutine realloc_tmprow(ic)
      integer(BUD_TYPE_VAR_PREC), intent(in) :: ic
      integer(BUD_TYPE_VAR_PREC) :: n
      n = fptr(ic+1) - fptr(ic)
      if ( n > size(tmprow) ) then
        deallocate(tmprow)
        allocate(tmprow(n))
      end if
    end subroutine realloc_tmprow
    
  end subroutine fold_row_only


  !> @param[in] from the originating sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  subroutine fold_col_only(from, nc, to)
    use BUD_CC2(BUD_MOD, _utils), only: modp
    
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nc
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnc, fnz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), frow(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:), trow(:)
    integer(BUD_TYPE_VAR_PREC) :: c, ic, i
    integer(BUD_TYPE_VAR_PREC), pointer :: ind

    ! Initialize the arrays from the originating
    ! sparse pattern
    call attach(from, nr=fnr, nc=fnc, nz=fnz, &
      cptr=fptr, row=frow)

    ! Here we do allow dublicate entries
    ! Sorting out these values will be the
    ! responsibility of the programmer.
    
    ! Create new folded pattern
    call new(to, fnr, nc, fnz)

    ! Note that we do not need to update ncol (for BUD_SP_CSC == 1)
    ! this is because fold_row_only will ALWAYS be called after.
    ! And `ncol` will be created upon the `new` call.
    call attach(to, cptr=tptr, row=trow)
      
#if BUD_SP_CSC == 0

# if BUD_SP_INTEROP_C == 0
    tptr(1) = 1
# else
    tptr(1) = 0
# endif

    ! loop on folded columns
    do c = 1 , nc
      ind => tptr(c+1)
      ind = tptr(c)
      ! loop on old columns
      do ic = c , fnc , nc
        ! loop on entries in the current (old) column
# if BUD_SP_INTEROP_C == 0
        do i = fptr(ic) , fptr(ic+1) - 1
# else
        do i = fptr(ic) + 1 , fptr(ic+1)
          ind = ind + 1
# endif
          trow(ind) = BUD_SP_MOD(frow(i), fnr)
# if BUD_SP_INTEROP_C == 0
          ind = ind + 1
# endif
        end do
      end do
    end do
    
#elif BUD_SP_CSC == 1

    tptr(1) = 0
    
    ! loop on folded columns
    do c = 1 , nc
      ind => tptr(c+1)
      ind = tptr(c)
      ! loop on old columns
      do ic = c , fnc , nc
        ! loop on entries in the current (old) column
        do i = fptr(ic) + 1 , fptr(ic+1)
          ind = ind + 1
          trow(ind) = BUD_SP_MOD(frow(i), fnr)
        end do
      end do
    end do
    
#endif
    
  end subroutine fold_col_only

  !> @endcond BUD_DEVELOPER

  
  
  !> @param[in] this sparse pattern to sort (in-place)
  !! @param[out] err associated error message of the sparsity pattern
  !! @param[out] pvt @opt=@none if requested the pivoting array for the sorted sparsity pattern
  subroutine sort_(this, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD,_utils), only: sort_quick

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out), target, optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: i, ic, nc, nz, ptr
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: cptr(:), row(:), tvt(:)

    ! Initialize error
    this%D%error_ = 0
    
    ! Get sparse pattern array
    call attach(this, nc=nc, nz=nz, cptr=cptr, row=row)

    if ( present(pvt) ) then
      
      if ( size(pvt) < nz ) then
        this%D%error_ = SP_INPUT + 3
        pvt = -1
        return
      end if

      do ic = 1 , nc
        
        nz = cptr(ic+1) - cptr(ic)
#if BUD_SP_CSC == 0
# if BUD_SP_INTEROP_C == 0
        ptr = cptr(ic) - 1
# else
        ptr = cptr(ic)
# endif
#elif BUD_SP_CSC == 1
        ptr = cptr(ic)
#endif

        ! get pivoting array
        tvt => pvt(ptr+1:)
        
        call sort_quick(nz, row(ptr+1:), tvt)
        do i = 1 , nz
          tvt(i) = ptr + tvt(i)
        end do
        do i = 1 , nz
          row(ptr+i) = row(tvt(i))
        end do

      end do

    else

      do ic = 1 , nc
        
        nz = cptr(ic+1) - cptr(ic)
#if BUD_SP_CSC == 0
# if BUD_SP_INTEROP_C == 0
        ptr = cptr(ic) - 1
# else
        ptr = cptr(ic)
# endif
#elif BUD_SP_CSC == 1
        ptr = cptr(ic)
#endif
        
        call sort_quick(nz, row(ptr+1:ptr+nz))

      end do
      
    end if

    this%D%sorted = .true.
    
  end subroutine sort_



  !> @param[in] this the sparse matrix (sorted, @isee #sp_sort
#if BUD_SP_CSR == 0 && BUD_SP_INTEROP_C == 1
  !! @param[in] ir row index (0-based)
  !! @param[in] ic column index (0-based)
  !! @return the sparse index of `M(ir,ic)`, `<0` if `M(ir,ic) = 0` (0-based)
#else
  !! @param[in] ir row index (1-based)
  !! @param[in] ic column index (1-based)
  !! @return the sparse index of `M(ir,ic)`, `<0` if `M(ir,ic) = 0` (1-based)
#endif
  pure function index_(this, ir, ic) result(idx)
    use BUD_CC2(BUD_MOD, _utils), only: find_bin
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ic
    integer(BUD_TYPE_VAR_PREC) :: idx

    if ( .not. this%D%sorted ) then
      
#if BUD_SP_CSC == 0
# if BUD_SP_INTEROP_C == 0
      do idx = this%D%cptr(ic) , this%D%cptr(ic+1) - 1
        if ( this%D%row(idx) == ir ) return
      end do
# else
      do idx = this%D%cptr(ic) + 1 , this%D%cptr(ic+1)
        if ( this%D%row(idx) == ir ) return
      end do
# endif
#elif BUD_SP_CSC == 1
      do idx = this%D%cptr(ic) + 1 , this%D%cptr(ic+1)
        if ( this%D%row(idx) == ir ) return
      end do
#endif
      
      idx = -1
      
      return
      
    end if

#if BUD_SP_CSC == 0
# if BUD_SP_INTEROP_C == 0
    call find_bin(this%D%cptr(ic+1)-this%D%cptr(ic), &
      this%D%row(this%D%cptr(ic):), ir, idx)
    if ( idx > 0 ) idx = this%D%cptr(ic) - 1 + idx
# else
    call find_bin(this%D%cptr(ic+1)-this%D%cptr(ic), &
      this%D%row(this%D%cptr(ic)+1:), ir, idx)
    if ( idx > 0 ) idx = this%D%cptr(ic) + idx
# endif
#elif BUD_SP_CSC == 1
    call find_bin(this%D%ncol(ic), &
      this%D%row(this%D%cptr(ic)+1:), ir, idx)
    if ( idx > 0 ) idx = this%D%cptr(ic) + idx
#endif
    
  end function index_


! Local pre-processor variables that
! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

  ! Control variables
#undef BUD_SP_CSC
#undef BUD_SP_INTEROP_C
#undef BUD_SP_MOD

#include "bud_cleanup.inc"
  
  
! project-bud -- local file settings
!     Anything below this line may be overwritten by scripts
!     Below are non-editable settings

! Local Variables:
!  mode: f90
!  f90-if-indent: 2
!  f90-type-indent: 2
!  f90-associate-indent: 2
!  f90-continuation-indent: 2
!  f90-structure-indent: 2
!  f90-critical-indent: 2
!  f90-program-indent: 2
!  f90-do-indent: 2
! End:

