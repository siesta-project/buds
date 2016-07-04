  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This module may be controlled via these
  ! preprocessor variables:
  !  - BUD_MOD_NAME name of the module
  !  - BUD_TYPE_NAME name of the public type
  !  - BUD_TYPE_NAME_ name of the internal data pointer
  !  - BUD_TYPE_NAME_STR the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW the routine name for creating a new type
  !
  ! Additionally the CSR matrix format may be
  ! controlled via these flags:
  !  - BUD_SM_CSR
  !    case ( 0 ) ! default
  !      standard CSR format (MKL-SpBLAS)
  !    case ( 1 )
  !      zero based pointer CSR format
  !      The row-pointer is zero based
  !      This will have an additional array with
  !      the number of non-zero elements per row
  !        (this is equivalent to the SIESTA sparsity)
  !      This does not allow C-interoperability (do NOT set
  !      BUD_SM_INTEROP_C in this case)
  !      
  !  - BUD_SM_INTEROP_C=0|1
  !    Use C-indexing with BUD_SM_INTEROP_C == 1
  !    All subsequent operations correspond to a
  !    C-index method.
  !    Cannot be used together with BUD_SM_CSR == 1

#ifndef BUD_SM_CSR
# define BUD_SM_CSR 0
#endif

#if BUD_SM_CSR == 0
#elif BUD_SM_CSR == 1
#ifdef BUD_SM_INTEROP_C
# error "SM_INTEROP_C may not be defined when using SM_CSR == 1"
#endif
#else
# error "SM_CSR *MUST* be either 0 or 1"
#endif

#ifndef BUD_SM_INTEROP_C
# define BUD_SM_INTEROP_C 0
#endif

#if BUD_SM_INTEROP_C == 0
# define BUD_SM_MOD modp
#elif BUD_SM_INTEROP_C == 1
# define BUD_SM_MOD mod
#else
# error "SM_INTEROP_C *MUST* be either 0 or 1"
#endif

#include "bud_utils.inc"
  
#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
  
  
  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Sparse matrix type in the CSR format
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER
    
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER
    
#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    procedure, private :: new_dim_ => new_dim_
    procedure, private :: new_copy_ => new_copy_
    !> @iSee new
    generic, public :: new => new_dim_, new_copy_

#ifdef DO_NOT_USE
    !> @iSee #index
    procedure, public :: index => index_
#endif

    !> @iSee #nonzeros
    procedure, public :: nonzeros => nonzeros_

    !> @iSee #mat_rows
    procedure, public :: rows => mat_rows_
    !> @iSee #mat_rows
    procedure, public :: mat_rows => mat_rows_

    !> @iSee #mat_cols
    procedure, public :: cols => mat_cols_
    !> @iSee #mat_cols
    procedure, public :: mat_cols => mat_cols_

    !> @iSee #mat_fold
    procedure, public :: fold => fold_
    !> @iSee #mat_fold
    procedure, public :: mat_fold => fold_

    !> @iSee #mat_sort
    procedure, public :: sort => sort_
    !> @iSee #mat_sort
    procedure, public :: mat_sort => sort_

    !> @iSee #mat_pointerp
    procedure, public :: pointerp => get_row_offsetp_
    !> @iSee #mat_pointerp
    procedure, public :: mat_pointerp => get_row_offsetp_

    !> @iSee #mat_pointerp
    procedure, public :: offsetp => get_row_offsetp_
    !> @iSee #mat_pointerp
    procedure, public :: mat_offsetp => get_row_offsetp_

    !> @iSee #mat_columnp
    procedure, public :: columnp => get_columnp_
    !> @iSee #mat_columnp
    procedure, public :: mat_columnp => get_columnp_

    !> @iSee #attach
    procedure, public :: attach => attach_

#endif
  end type BUD_TYPE_NAME

  
  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains the sparsity pattern for a CSR matrix.
  type BUD_TYPE_NAME_
    
    !> Number of rows in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nr = 0
    !> Number of columns in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nc = 0
    !> Number of non-zero elements in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nz = 0

    !> Index of the equivalent row (size `nr+1`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:) => null()

#if BUD_SM_CSR == 1
    !> Number of non-zero elements per row (`nrow(i)=rptr(i+1)-rptr(i)`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: nrow(:) => null()
#endif

    !> The column index of the equivalent sparse matrix (size `nz`)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:) => null()


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER
  
  
  !> Create a new sparse matrix
  !!
  !! You may either create an empty sparse matrix with
  !! a fixed size, or create a sparse matrix from
  !! existing data.
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: new
  
  !> @iSee new
  interface BUD_TYPE_NEW
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: BUD_TYPE_NEW

!!$ Sadly, intel compilers cannot figure out to
!!$ deallocate from other pointers... :(
!!$
!!$  !> Associate @bud instances to already existing arrays
!!$  interface assoc
!!$    module procedure assoc_
!!$  end interface assoc
!!$  !> Associate @bud instances to already existing arrays
!!$  interface BUD_TYPE_ASSOC
!!$    module procedure assoc_
!!$  end interface BUD_TYPE_ASSOC


  !> Fold a sparse matrix to form a smaller sparse matrix
  !!
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
  !! Reduce the size of a sparse matrix by folding `ir -> mod(ir-1, nnr) + 1`
# else
  !! Reduce the size of a sparse matrix by folding `ir -> mod(ir, nnr)`
# endif
#elif BUD_SM_CSR == 1
  !! Reduce the size of a sparse matrix by folding `ir -> mod(ir-1, nnr) + 1`
#endif
  !! with `nnr` being the new number of rows.
  !!
  !! If the folding reduces the matrix dimensions by half
  !! (for a matrix with even number of rows and columns)
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
  !! Both the number of rows and/or columns may changed
  !! individually.
  interface mat_fold
    module procedure fold_
  end interface 
  public :: mat_fold
  
  
  !> Sorts columns in the sparse pattern
  !!
  !! Sorts the sparse matrix such that the column
  !! index is always increasing for each row.
  !! This will generally allow faster access patterns.
  !!
  !! @note
  !! An integer error is return which should be zero
  !! if no errors occurs.
  !!
  !! If the `err` value is non-zero the following table
  !! may be used:
  !!
  !!  - `err == SM_INPUT + 3`:
  !!    the 3rd input is erroneously supplied.
  interface mat_sort
    module procedure sort_
  end interface
  public :: mat_sort


  !> Retrieve a pointer to the row offsets/pointers
  !!
  !! A pointer with the row offsets.
  !!
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
  !! `M(ir,col(pointer(ir)))` is the first sparse
  !! element in row `ir`.
# else
  !! `M(ir,col(pointer(ir)+1))` is the first sparse
  !! element in row `ir`.
# endif
#elif BUD_SM_CSR == 1
  !! `M(ir,col(pointer(ir)+1))` is the first sparse
  !! element in row `ir`.
#endif
  interface mat_pointerp
    module procedure get_row_offsetp_
  end interface
  public :: mat_pointerp

  !> @iSee #mat_pointerp
  interface mat_offsetp
    module procedure get_row_offsetp_
  end interface
  public :: mat_offsetp

  
  !> Retrieve a pointer to the column indices
  interface mat_columnp
    module procedure get_columnp_
  end interface
  public :: mat_columnp

  
  ! Include the common elements of a sparsity method
  ! This includes "bud_common.inc"
# include "SM_common.inc"
  

  !> @cond BUD_DEVELOPER
  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  pure subroutine delete_data(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this

    this%nr = 0
    this%nc = 0
    this%nz = 0

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. associated(this%col) ) return
    deallocate(this%rptr)
    nullify(this%rptr)
    deallocate(this%col)
    nullify(this%col)
#if BUD_SM_CSR == 1
    deallocate(this%nrow)
    nullify(this%nrow)
#endif

    
  end subroutine delete_data
  
  !> @endcond BUD_DEVELOPER

  
  !> @param[inout] this the sparse matrix
  !! @param[in] nr number of rows of the matrix
  !! @param[in] nc number of columns of the matrix
  !! @param[in] nz number of non-zero elements the matrix
  subroutine new_dim_(this, nr, nc, nz)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz

    call initialize(this)

    this%D%nr = nr
    this%D%nc = nc
    this%D%nz = nz
    
    allocate(this%D%rptr(nr+1))
#if BUD_SM_CSR == 1
    allocate(this%D%nrow(nr))
#endif

    allocate(this%D%col(nz))

  end subroutine new_dim_

  
  !> @param[inout] this the new sparse matrix
  !! @param[in] nr number of rows of the matrix
  !! @param[in] nc number of columns of the matrix
  !! @param[in] nz number of non-zero elements of the matrix
  !! @param[in] rptr row pointers (at least size `nr`)
  !! @param[in] col column indices for the sparse elements
  subroutine new_copy_(this, nr, nc, nz, rptr, col)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: rptr(nr), col(nz)

#if BUD_SM_CSR == 1
    integer(BUD_TYPE_VAR_PREC) :: ir
#endif

    ! Copy over information
#if BUD_SM_CSR == 0
    ! It must be based on the starting index
# if BUD_SM_INTEROP_C == 0
    if ( rptr(1) /= 1 ) then
      call delete(this)
      return
    end if
# else
    if ( rptr(1) /= 0 ) then
      call delete(this)
      return
    end if
# endif
    
#elif BUD_SM_CSR == 1
    ! It must be 0-based
    if ( rptr(1) /= 0 ) then
      call delete(this)
      return
    end if
#endif

    ! pre-allocate
    call new(this, nr, nc, nz)

    ! Copy pointers
    this%D%rptr(1:this%D%nr) = rptr

    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
    this%D%rptr(this%D%nr+1) = this%D%nz + 1
# else
    this%D%rptr(this%D%nr+1) = this%D%nz
# endif
    
#elif BUD_SM_CSR == 1
    this%D%rptr(this%D%nr+1) = this%D%nz

    do ir = 1 , nr
      this%D%nrow(ir) = this%D%rptr(ir+1) - this%D%rptr(ir)
    end do
    
#endif

    this%D%col = col

  end subroutine new_copy_

  
  !> @param[in] this sparse matrix @bud
  !! @param[out] nr @opt number of rows in SM
  !! @param[out] nc @opt number of columns in SM
  !! @param[out] nz @opt number of non-zero elements in SM
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
  !! @param[out] rptr @opt row pointer (`rptr(2)` is starting index of `ir=2`)
# else
  !! @param[out] rptr @opt row pointer (`rptr(2)+1` is starting C-index of `ir=2`)
# endif
#elif BUD_SM_CSR == 1
  !! @param[out] rptr @opt row pointer (`rptr(2)+1` is starting index of `ir=2`)
#endif
  !! @param[out] col @opt column index
#if BUD_SM_CSR == 1
  !! @param[out] nrow @opt number of non-zero elements per row
#endif
  subroutine attach_(this, D, nr, nc, nz, rptr, col &
#if BUD_SM_CSR == 1
    , nrow &
#endif
    )
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: rptr(:), col(:)
#if BUD_SM_CSR == 1
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: nrow(:)
#endif

    if ( present(nr) ) nr = this%D%nr
    if ( present(nc) ) nc = this%D%nc
    if ( present(nz) ) nz = this%D%nz
    if ( present(rptr) ) rptr => this%D%rptr
    if ( present(col) ) col => this%D%col
#if BUD_SM_CSR == 1
    if ( present(nrow) ) nrow => this%D%nrow
#endif
    
  end subroutine attach_



  !> @param[in] from the originating sparse pattern
  !! @param[in] nr new number of rows in the sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] single @opt=.false. if .true.: will compress equivalent entries
  !!                  to a single entry, no double entries will be found subsequently.
  subroutine fold_(from, nr, nc, to, single)
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick, sort_ps
    use BUD_CC2(BUD_MOD, _utils), only: find_bin
    use BUD_CC2(BUD_MOD, _utils), only: modp

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: single

    type(BUD_TYPE_NAME) :: tmp

    if ( nr == mat_rows(from) ) then
      
      ! Fast-skip to the easier code if input
      ! is easy.
      call fold_col_only(from, nc, to, single = single)
      
    else

      call delete(to)
      call fold_row_only(from, nr, tmp)
      call fold_col_only(tmp, nc, to, single = single)
      call delete(tmp)
      
    end if

  end subroutine fold_


  !> @param[in] from the originating sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] single @opt=.false. if .true.: will compress equivalent entries
  !!                  to a single entry, no double entries will be found subsequently.
  subroutine fold_col_only(from, nc, to, single)
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick, modp

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nc
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: single

    ! Local single variable
    logical :: lsingle

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnc, fnz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), fcol(:), fsol(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:), tcol(:)
    
    integer(BUD_TYPE_VAR_PREC) :: ir, i, ptr
    integer(BUD_TYPE_VAR_PREC), pointer :: ind

    integer(BUD_TYPE_VAR_PREC), allocatable :: tmpcol(:)

    lsingle = .false.
    if ( present(single) ) lsingle = single

    
    ! Initialize the arrays from the originating
    ! sparse pattern
    call attach(from, nr=fnr, nc=fnc, nz=fnz, &
      rptr=fptr, col=fcol)

    if ( .not. lsingle ) then
      ! Here we do allow dublicate entries
      ! Sorting out these values will be the
      ! responsibility of the programmer.
      
      ! Create new folded matrix
      call new(to, fnr, nc, fnz)

      call attach(to, rptr=tptr, col=tcol)
      
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0

      tptr(1) = 1
      ptr = 0
      
      ! loop on the folded rows
      do ir = 1 , fnr
        ! loop on entries in the current (old) row
        do i = fptr(ir) , fptr(ir+1) - 1
          ptr = ptr + 1
          tcol(ptr) = BUD_SM_MOD(fcol(i), nc)
        end do
        ! update the following pointer
        tptr(ir+1) = ptr + 1
        
      end do
# else 

      tptr(1) = 0
      ptr = 0
      
      ! loop on the folded rows
      do ir = 1 , fnr
        ! loop on entries in the current (old) row
        do i = fptr(ir) + 1 , fptr(ir+1)
          ptr = ptr + 1
          tcol(ptr) = BUD_SM_MOD(fcol(i), nc)
        end do
        ! update the following pointer
        tptr(ir+1) = ptr
        
      end do

# endif
      
#elif BUD_SM_CSR == 1

      tptr(1) = 0
      ptr = 0
      
      ! loop on the folded rows
      do ir = 1 , fnr
        ! loop on entries in the current (old) row
        do i = fptr(ir) + 1 , fptr(ir+1)
          ptr = ptr + 1
          tcol(ptr) = BUD_SM_MOD(fcol(i), nc)
        end do
        ! update the following pointer
        tptr(ir+1) = ptr
        
      end do

#endif
      
    else
      ! We will limit the number of entries
      ! by removing all dublicated entries

      ! First calculate the size of the matrix
      nullify(tptr,tcol)
      allocate(tptr(fnr+1),tcol(fnz))
      allocate(tmpcol(fptr(2)-fptr(1)))

#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
      
      tptr(1) = 1
      
      ! loop on the new rows
      do ir = 1 , fnr
        ! point to the following rows pointer
        ! this enables to count the number of entries
        ! in the current row as:
        !   number entries = tptr(ir+1) - tptr(ir)
        ind => tptr(ir+1)
        
        ! initialize counter for the following row
        ! (i.e. count number of entries)
        ind = tptr(ir)

        ! ensure pivoting array long enough
        call realloc_tmpcol(ir)

        ! Point to the current column indices
        fsol => fcol(fptr(ir):)
          
        ! Create pivoting array so that we
        ! may easily find the reduced sparsity pattern
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fsol, tmpcol )

        ! initialize the first element
        tcol(ind) = BUD_SM_MOD(fsol(tmpcol(1)), nc)
        ind = ind + 1
        do i = 2 , fptr(ir+1) - fptr(ir)
          ! loop on entries in the current (old) row
          ! check that the variable does not exist
          if ( fsol(tmpcol(i-1)) /= fsol(tmpcol(i)) ) then
            tcol(ind) = BUD_SM_MOD(fsol(tmpcol(i)), nc)
            ind = ind + 1
          end if
        end do
        
      end do

      ! ensure de-allocation
      deallocate(tmpcol)
      
      ! create the new sparsity pattern
      !> TODO check that the last row has > 0 elements
      !! this will justify the `-1` below
      call new(to, fnr, nc, tptr(fnr+1)-1, tptr, tcol)

# else

      tptr(1) = 0
      
      ! loop on the new rows
      do ir = 1 , fnr
        ind => tptr(ir+1)
        
        ind = tptr(ir)

        call realloc_tmpcol(ir)

        fsol => fcol(fptr(ir)+1:)
          
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fsol, tmpcol )

        ind = ind + 1
        tcol(ind) = BUD_SM_MOD(fsol(tmpcol(1)), nc)
        do i = 2 , fptr(ir+1) - fptr(ir)
          if ( fsol(tmpcol(i-1)) /= fsol(tmpcol(i)) ) then
            ind = ind + 1
            tcol(ind) = BUD_SM_MOD(fsol(tmpcol(i)), nc)
          end if
        end do

      end do

      ! ensure de-allocation
      deallocate(tmpcol)
      
      ! create the new sparsity pattern
      call new(to, fnr, nc, tptr(fnr+1), tptr, tcol)

# endif
      
#elif BUD_SM_CSR == 1

      tptr(1) = 0
      
      do ir = 1 , fnr
        ind => tptr(ir+1)
        ind = tptr(ir)

        call realloc_tmpcol(ir)

        fsol => fcol(fptr(ir)+1:)
          
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fsol, tmpcol )

        ind = ind + 1
        tcol(ind) = BUD_SM_MOD(fsol(tmpcol(1)), nc)
        do i = 2 , fptr(ir+1) - fptr(ir)
          if ( fsol(tmpcol(i-1)) /= fsol(tmpcol(i)) ) then
            ind = ind + 1
            tcol(ind) = BUD_SM_MOD(fsol(tmpcol(i)), nc)
          end if
        end do

      end do

      ! ensure de-allocation
      deallocate(tmpcol)
      
      ! create the new sparsity pattern
      call new(to, fnr, nc, tptr(fnr+1), tptr, tcol)

#endif
      
    end if

    !> @cond BUD_DEVELOPER
  contains

    ! Reallocate the temporary column indices in
    ! a sorted array
    subroutine realloc_tmpcol(ir)
      integer(BUD_TYPE_VAR_PREC), intent(in) :: ir
      integer(BUD_TYPE_VAR_PREC) :: n
      n = fptr(ir+1) - fptr(ir)
      if ( n > size(tmpcol) ) then
        deallocate(tmpcol)
        allocate(tmpcol(n))
      end if
    end subroutine realloc_tmpcol

    !> @endcond BUD_DEVELOPER
    
  end subroutine fold_col_only


  !> @param[in] from the originating sparse pattern
  !! @param[in] nr new number of rows in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  subroutine fold_row_only(from, nr, to)
    use BUD_CC2(BUD_MOD, _utils), only: modp
    
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnc, fnz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), fcol(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:), tcol(:)
    
    integer(BUD_TYPE_VAR_PREC) :: r, ir, i
    integer(BUD_TYPE_VAR_PREC), pointer :: ind

    ! Initialize the arrays from the originating
    ! sparse pattern
    call attach(from, nr=fnr, nc=fnc, nz=fnz, &
      rptr=fptr, col=fcol)

    ! Here we do allow dublicate entries
    ! Sorting out these values will be the
    ! responsibility of the programmer.
    
    ! Create new folded matrix
    call new(to, nr, fnc, fnz)
    
    call attach(to, rptr=tptr, col=tcol)
      
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
    
    tptr(1) = 1

    ! loop on folded rows
    do r = 1 , nr
      ind => tptr(r+1)
      ind = tptr(r)
      ! loop on old rows
      do ir = r , fnr , nr
        ! loop on entries in the current (old) row
        do i = fptr(ir) , fptr(ir+1) - 1
          tcol(ind) = BUD_SM_MOD(fcol(i), fnc)
          ind = ind + 1
        end do
      end do
    end do
    
# else 
    
    tptr(1) = 0

    ! loop on folded rows
    do r = 1 , nr
      ind => tptr(r+1)
      ind = tptr(r)
      ! loop on old rows
      do ir = r , fnr , nr
        ! loop on entries in the current (old) row
        do i = fptr(ir) + 1 , fptr(ir+1)
          ind = ind + 1
          tcol(ind) = BUD_SM_MOD(fcol(i), fnc)
        end do
      end do
    end do
# endif
    
#elif BUD_SM_CSR == 1

    tptr(1) = 0

    ! loop on folded rows
    do r = 1 , nr
      ind => tptr(r+1)
      ind = tptr(r)
      ! loop on old rows
      do ir = r , fnr , nr
        ! loop on entries in the current (old) row
        do i = fptr(ir) + 1 , fptr(ir+1)
          ind = ind + 1
          tcol(ind) = BUD_SM_MOD(fcol(i), fnc)
        end do
      end do
    end do

#endif
    
  end subroutine fold_row_only


  !> @param[in] this sparse matrix
  !! @return a pointer to the row offsets for the sparse matrix (contiguous)
  function get_row_offsetp_(this) result(rptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:)
    
    rptr => this%D%rptr
    
  end function get_row_offsetp_

  
  !> @param[in] this sparse matrix
  !! @return a pointer to the column indices for the sparse matrix (contiguous)
  function get_columnp_(this) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    col => this%D%col
    
  end function get_columnp_

  
  !> @param[in] this sparse pattern to sort (in-place)
  !! @param[out] err associated error message of the sparsity pattern
  !! @param[out] pvt @opt=@none if requested the pivoting array for the sorted sparsity pattern
  subroutine sort_(this, err, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out) :: err
    integer(BUD_TYPE_VAR_PREC), intent(out), target, optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: ir, nr, nz, ptr
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:), tvt(:)

    ! Initialize error
    err = 0
    
    ! Get sparse matrix array
    call attach(this, nr=nr, nz=nz, rptr=rptr, col=col)

    if ( present(pvt) ) then
      
      if ( size(pvt) < nz ) then
        err = SM_INPUT + 3
        pvt = -1
        return
      end if

      do ir = 1 , nr
        
        nz = rptr(ir+1) - rptr(ir)
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
        ptr = rptr(ir) - 1
# else
        ptr = rptr(ir)
# endif
#elif BUD_SM_CSR == 1
        ptr = rptr(ir)
#endif

        ! get pivoting array
        tvt => pvt(ptr+1:)

        call sort_quick(nz, col(ptr+1:), tvt)
        col(ptr+1:ptr+nz) = col(ptr+tvt(1:nz))

      end do

    else

      do ir = 1 , nr
        
        nz = rptr(ir+1) - rptr(ir)
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
        ptr = rptr(ir) - 1
# else
        ptr = rptr(ir)
# endif
#elif BUD_SM_CSR == 1
        ptr = rptr(ir)
#endif
        
        call sort_quick(nz, col(ptr+1:ptr+nz))

      end do
      
    end if
    
  end subroutine sort_


#ifdef DO_NOT_USE
  
  !> @param[in] this the sparse matrix (sorted, @isee #sort)
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
  !! @param[in] ir row index (0-based)
#else
  !! @param[in] ir row index (1-based)
#endif
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
  !! @param[in] ic column index (0-based)
#else
  !! @param[in] ic column index (1-based)
#endif
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
  !! @return the sparse index of `M(ir,ic)`, `<0` if `M(ir,ic) = 0` (0-based)
#else
  !! @return the sparse index of `M(ir,ic)`, `<0` if `M(ir,ic) = 0` (1-based)
#endif
  pure function index_(this, ir, ic) result(idx)
    use BUD_CC2(BUD_MOD, _utils), only: find_bin
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ic
    integer(BUD_TYPE_VAR_PREC) :: idx

#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
    call find_bin(this%D%rptr(ir+1)-this%D%rptr(ir), &
      this%D%col(this%D%rptr(ir):), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir) - 1 + idx
    
# else
    call find_bin(this%D%rptr(ir+2)-this%D%rptr(ir+1), &
      this%D%col(this%D%rptr(ir+1)+1:), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir+1) + idx - 1
    
# endif

#elif BUD_SM_CSR == 1
    call find_bin(this%D%nrow(ir), &
      this%D%col(this%D%rptr(ir)+1:), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir) + idx
    
#endif
    
  end function index_

#endif
  
  
  !> @param[in] this sparse matrix
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent

    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = BUD_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    
    if ( .not. is_initd(this) ) then
      write(*,fmt) "<", trim(name), " not initialized>"
      return
    end if

    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,4(i0,a))'
    
    write(*,fmt) "<", trim(name), " rows=", this%D%nr, &
      ", cols=", this%D%nc, ", nnzs=",this%D%nz, &
      ", refs: ", references(this), ">"
    
  end subroutine print_


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
#undef BUD_SM_CSR
#undef BUD_SM_INTEROP_C
#undef BUD_SM_MOD

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

