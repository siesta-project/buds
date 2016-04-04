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
  !    case ( 0 )
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

  !> @bud variable
  !!
  !! Handler for the pointer type stored
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER
    
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER
    
#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    
#endif
  end type BUD_TYPE_NAME

  
  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains the sparsity pattern for a CSR matrix.
  !! This sparse matrix *must* contain the diagonal
  !! elements.
  type BUD_TYPE_NAME_
    
    !> Number of rows in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nr = 0
    !> Number of columns in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nc = 0
    !> Number of non-zero elements in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nz = 0

    !> Index of the equivalent row (size `nr+1`)
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: rptr(:) => null()

#if BUD_SM_CSR == 1
    !> Number of non-zero elements per row (`nrow(i)=rptr(i+1)-rptr(i)`)
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: nrow(:) => null()
#endif

    !> The column index of the equivalent sparse matrix (size `nz`)
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: col(:) => null()


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER
  
  
  !> Create new @bud instances
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: new
  
  !> Create new @bud instances
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

  !> Number of non-zero elements in the stored array (same as regular `size`)
  interface nonzeros
    module procedure nonzeros_
  end interface
  public :: nonzeros

  !> Number of rows in matrix
  interface mat_rows
    module procedure mat_rows_
  end interface 
  public :: mat_rows
  
  !> Number of columns in matrix
  interface mat_cols
    module procedure mat_cols_
  end interface 
  public :: mat_cols

  !> Wrap-around column indices to form a smaller matrix
  interface get_wrapped
    module procedure get_wrapped_
  end interface 
  public :: get_wrapped
  
  !> Query internal data structure elements via simple routine
  interface attach
    module procedure attach_
  end interface 
  public :: attach

  !> Finalize and sort the column indices per row
  interface finalize
    module procedure finalize_
  end interface
  public :: finalize

  !> Retrieve a pointer to the internal row pointers
  interface get_row_ptrp
    module procedure get_row_ptrp_
  end interface
  public :: get_row_ptrp
  
  !> Retrieve a pointer to the internal column indices
  interface get_columnp
    module procedure get_columnp_
  end interface
  public :: get_columnp

  !> Query sparse index of a given row and column
  interface index
    module procedure index_
  end interface
  public :: index

  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print

  ! Include the common elements of a sparsity method
# include "SM_common.inc"
  
  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "bud_common.inc"


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
    
  end subroutine delete_data
  
  !> @endcond BUD_DEVELOPER

  
  !> Create a new @bud allocating room or specific size
  !!
  !! @param[inout] this @bud to be allocated
  !! @param[in] nr number of rows of this @bud
  !! @param[in] nc number of columns of this @bud
  !! @param[in] nz number of non-zero elements this @bud
  subroutine new_dim_(this, nr, nc, nz)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
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

  !> Create a new @bud by copying in from an outside list
  !!
  !! @param[inout] this @bud to be allocated
  !! @param[in] nr number of rows of this @bud
  !! @param[in] nc number of columns of this @bud
  !! @param[in] nz number of non-zero elements this @bud
  !! @param[in] rptr row pointers (can be of size `nr` or `nr+1`)
  !! @param[in] col column indices for the sparse elements
  subroutine new_copy_(this, nr, nc, nz, rptr, col)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: rptr(nr), col(nz)

    integer(BUD_TYPE_VAR_PREC) :: ir

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

  !> Query number of non-zero elements
  elemental function nonzeros_(this) result(nnzs)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nnzs
    nnzs = this%D%nz
  end function nonzeros_

  !> Query number of rows of the stored sparse matrix
  elemental function mat_rows_(this) result(rows)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: rows
    rows = this%D%nr
  end function mat_rows_

  !> Query number of columns of the stored sparse matrix
  elemental function mat_cols_(this) result(cols)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: cols
    cols = this%D%nc
  end function mat_cols_

  !> Retrieve access to sparse pattern via pointers.
  !!
  !! To manually access the sparsity pattern one should
  !! retrive the pointers and interact externally with the
  !! sparsity pattern.
  !!
  !! Interacting externally with sparse pattern will yield faster performance.
  !!
  !! All arguments are optional and *must* be accessed by keyword
  !! usage.
  !!
  !! @todo
  !! Add error checking for `D` parameter.
  !!
  !! @param[in] this the sparsity @bud
  !! @param[out] nr @opt number of rows
  !! @param[out] nc @opt number of columns
  !! @param[out] nz @opt number of non-zero elements
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
  !! @param[out] rptr @opt row pointer (`rptr(2)` is starting index of `ir=2`)
# else
  !! @param[out] rptr @opt row pointer (`rptr(2)+1` is starting index of `ir=2`)
# endif
  !! @param[out] col @opt column index
#elif BUD_SM_CSR == 1
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
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer, contiguous, optional :: rptr(:), col(:)
#if BUD_SM_CSR == 1
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer, contiguous, optional :: nrow(:)
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


  !> Get sparse index from double index
  !!
  !! This should only be used if one does scattered
  !! access to the matrix.
  !!
  !! Manual interaction with the sparsity pattern is preferred.
  !!
  !! @param[in] this the sparsity pattern @bud
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
  !! @param[in] ir the requested row (0-based)
#else
  !! @param[in] ir the requested row (1-based)
#endif
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
  !! @param[in] ic the requested column (0-based)
#else
  !! @param[in] ic the requested column (1-based)
#endif
  !! @param[out] idx the index of `(ir,ic)`, <0 if non-existing
  pure subroutine index_(this, ir, ic, idx)
    use BUD_CC2(BUD_MOD, _utils), only: find_bin
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in), value :: ir
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ic
    integer(BUD_TYPE_VAR_PREC), intent(out) :: idx

#if BUD_SM_CSR == 0
#if BUD_SM_INTEROP_C == 0
    call find_bin(this%D%rptr(ir+1)-this%D%rptr(ir), &
      this%D%col(this%D%rptr(ir):), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir) - 1 + idx
    
#else
    call find_bin(this%D%rptr(ir+2)-this%D%rptr(ir+1), &
      this%D%col(this%D%rptr(ir+1)+1:), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir+1) + idx - 1
    
#endif

#elif BUD_SM_CSR == 1
    call find_bin(this%D%nrow(ir), &
      this%D%col(this%D%rptr(ir)+1:), ic, idx)

    if ( idx > 0 ) idx = this%D%rptr(ir) + idx
    
#endif
    
  end subroutine index_


  !> Wrap-around sparse matrix to form a smaller sparse matrix
  !!
  !! This reduces the column indices in a sparse pattern to
  !! an equivalent column reduced matrix by converting column
  !!  `c` to column `mod(c-1, nc) + 1`. Which in a 0-counted
  !! array would simply be: `mod(c,nc)`.
  !!
  !! This can be useful for transforming a rectangular matrix
  !! `M(nr, 2*nr)` to a square matrix `M(nr,nr)`.
  !!
  !! @param[in] from the originating sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] unite @opt=.false. unites equivalent sparse
  !!                  elements to not have dublicate elements.
  subroutine get_wrapped_(from, nc, to, unite)
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick, modp

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nc
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: unite

    ! Local unite variable
    logical :: lunite

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnz, fi
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: fptr(:), fcol(:)
    integer(BUD_TYPE_VAR_PREC) :: tnr, ti
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: tptr(:), tcol(:)
    
    integer(BUD_TYPE_VAR_PREC) :: ir, i, ptr
    integer(BUD_TYPE_VAR_PREC), pointer :: ind

    integer(BUD_TYPE_VAR_PREC), allocatable :: tmpcol(:)

    lunite = .false.
    if ( present(unite) ) lunite = unite

    ! Initialize
    call attach(from, nr=fnr, nz=fnz, rptr=fptr, col=fcol)
    
    ! Do simple case
    if ( .not. lunite ) then
      
      call new(to, fnr, nc, fnz)

      call attach(to, rptr=tptr, col=tcol)
      
      tptr = fptr
      do i = 1 , fnz
        tcol(i) = BUD_SM_MOD(fcol(i), nc)
      end do
      
    else

      ! First calculate the size of the matrix
      nullify(tptr,tcol)
      allocate(tptr(fnr+1),tcol(fnz))
      allocate(tmpcol(fptr(2)-fptr(1)))

#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
      tptr(1) = 1
# else
      tptr(1) = 0
# endif
#elif BUD_SM_CSR == 1
      tptr(1) = 0
#endif

      do ir = 1 , fnr
        
        ! ensure pivoting array long enough
        call realloc_tmpcol(ir)

        ! Create pivoting array so that we
        ! may easily find the reduced sparsity pattern
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fcol(fptr(ir):), tmpcol(1:) )
# else
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fcol(fptr(ir)+1:), tmpcol(1:) )
# endif
#elif BUD_SM_CSR == 1
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fcol(fptr(ir)+1:), tmpcol(1:) )
#endif

#if BUD_SM_CSR == 0
       
        ! point to the following rows pointer
        ! this enables to count the number of entries
        ! in the current row as:
        !   number entries = tptr(ir+1) - tptr(ir)
        ind => tptr(ir+1)
        
        ! initialize counter for the following row
        ! (i.e. count number of entries)
        ind = tptr(ir)
        
        ! initialize first element
# if BUD_SM_INTEROP_C == 0
        ptr = tptr(ir) - 1
# else
        ptr = tptr(ir)
# endif

        tcol(ind) = BUD_SM_MOD(fcol(ptr+tmpcol(1)), nc)
        do i = 2 , fptr(ir+1) - fptr(ir)
          if ( fcol(ptr+tmpcol(i-1)) /= fcol(ptr+tmpcol(i)) ) then

# if BUD_SM_INTEROP_C == 1
            ind = ind + 1
# endif
            tcol(ind) = BUD_SM_MOD(fcol(ptr+tmpcol(i-1)), nc)
# if BUD_SM_INTEROP_C == 0
            ind = ind + 1
# endif
          end if
        end do

        
#elif BUD_SM_CSR == 1
        ! point to the following rows pointer
        ! this enables to count the number of entries
        ! in the current row as:
        !   number entries = tptr(ir+1) - tptr(ir)
        ind => tptr(ir+1)
        
        ! initialize counter for the following row
        ! (i.e. count number of entries)
        ind = tptr(ir)
        ! initialize first element
        tcol(ind) = BUD_SM_MOD(fcol(tptr(ir)+tmpcol(1)), nc)
        do i = 2 , fptr(ir+1) - fptr(ir)
          if ( fcol(tptr(ir)+tmpcol(i-1)) /= fcol(tptr(ir)+tmpcol(i)) ) then
            tcol(ind) = BUD_SM_MOD(fcol(tptr(ir)+tmpcol(i-1)), nc)
            ind = ind + 1
          end if
          
        end do
        
#endif
        
      end do

      deallocate(tmpcol)

      ! create the new sparsity pattern
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
      call new(to, fnr, nc, tptr(fnr+1), tptr, tcol)
# else
      call new(to, fnr, nc, tptr(fnr+1)+1, tptr, tcol)
# endif
#elif BUD_SM_CSR == 1
      call new(to, fnr, nc, tptr(fnr+1), tptr, tcol)
#endif

      deallocate(tptr, tcol)

    end if

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

  end subroutine get_wrapped_



  !> Retrieve a pointer to the row pointers
  !!
  !! @param[in] this type to retrieve the pointer from
  !! @return the pointer to the contained row-pointer segment
  function get_row_ptrp_(this) result(rptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: rptr(:)
    
    rptr => this%D%rptr
    
  end function get_row_ptrp_

  !> Retrieve a pointer to the column indices
  !!
  !! @param[in] this type to retrieve the pointer from
  !! @return the pointer to the contained column indices segment
  function get_columnp_(this) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: col(:)

    col => this%D%col
    
  end function get_columnp_

  
  !> Sorts the elements in the sparse pattern
  !!
  !! Sorts the sparse matrix such that the column
  !! index is always increasing for each row.
  !!
  !! Double entries are not allowed and an error will
  !! be returned.
  !!
  !! @param[in] this @bud to finalize
  !! @param[out] err associated error message of the sparsity pattern
  !! @param[out] pvt @opt=@none if requested the pivoting array for the finalized sparsity pattern
  subroutine finalize_(this, err, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out) :: err
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: ir, i, nr, nc, nz, ptr
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: rptr(:), col(:)

    ! Initialize error
    err = 0
    
    ! Get arrays
    call attach(this, nr=nr, nc=nc, nz=nz, rptr=rptr, col=col)

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
        call sort_quick(nz, col(ptr+1:), pvt(ptr+1:))
        do i = ptr + 1 , ptr + nz
          pvt(i) = ptr + pvt(i)
# if BUD_SM_INTEROP_C == 0
          if ( col(i) >  nc ) err = - i
# else
          if ( col(i) >= nc ) err = - i
# endif
          if ( err == 0 ) then
            if ( i > ptr + 1 ) then
              if ( col(i-1) == col(i) ) then
                err = SM_ENTRY_MULTIPLE
              end if
            end if
          end if
        end do
        
#elif BUD_SM_CSR == 1
        call sort_quick(nz, col(rptr(ir)+1:), pvt(rptr(ir)+1:))
        do i = rptr(ir) + 1 , rptr(ir) + nz
          pvt(i) = rptr(ir) + pvt(i)
          if ( col(i) > nc ) then
            err = - i
          end if
          if ( err == 0 ) then
            if ( i > rptr(ir) + 1 ) then
              if ( col(i-1) == col(i) ) then
                err = SM_ENTRY_MULTIPLE
              end if
            end if
          end if
        end do
#endif
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
        call sort_quick(nz, col(ptr+1:))
        do i = ptr + 1 , ptr + nz
# if BUD_SM_INTEROP_C == 0
          if ( col(i) >  nc ) err = - i
# else
          if ( col(i) >= nc ) err = - i
# endif
          if ( err == 0 ) then
            if ( i > ptr + 1 ) then
              if ( col(i-1) == col(i) ) then
                err = SM_ENTRY_MULTIPLE
                return
              end if
            end if
          end if
        end do
        
      end do
      
    end if
    
  end subroutine
  
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer(BUD_TYPE_VAR_PREC) :: nr, nz, nnzs

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

