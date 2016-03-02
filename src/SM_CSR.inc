  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"
  
#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
  
  !> BUD_MOD_NAME documentation
  !!
  !! @details
  !! The detailed implementation of the sparsity BUD_TYPE_NAME type.
  !!
  !! This contains the implementation of a CSR sparsity pattern.
  !!
  !! The CSR sparsity pattern stored *must* be sorted in each
  !! row such that the columns are consecutively aligned.
  !! This will help ensure a fast access pattern in the matrix
  !! with a row-based access pattern.
  !!
  !! \code{.f90}
  !!   integer :: nr, nz, ir, idx
  !!   integer, pointer, contiguous :: rptr, col
  !!   call attach(this, nr=nr, nz=nz, rptr, col)
  !!
  !!   do ir = 1 , nr
  !!     do idx = rptr(ir) , rptr(ir+1) - 1
  !!       ! access M(ir,col(idx))
  !!     end do
  !!   end do
  !! \endcode
  !!
  !! There are no data-consistency checkings performed (for performance
  !! reasons) hence you *can* end up with multiple entries for the
  !! same matrix element.
  !! In such cases one must define the actions taken.
  !!
  !! @note
  !! This sparsity pattern is constructed to conform with the
  !! MKL Sparse BLAS library.
  !! The sparsity pattern is 1-based and is the 3-array variant
  !! of the CSR format. The 3-array variant can be used in the
  !! 4-array input without changing any array elements and/or
  !! extra memory allocation.
  !! The sparsity pattern *must* contain the diagonal elements.
  !!
  !! @author Nick R. Papior (2016, February)
  
  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"
  
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
    !> The column index of the equivalent sparse matrix (size `nz`)
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: col(:) => null()

    ! Consistent data in the reference counted object
#   include "bud_common_type.inc"

  end type BUD_TYPE_NAME_
  
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

  
  !> Create a new @bud allocating room or specific size
  !!
  !! @param[inout] this @bud to be allocated
  !! @param[in] nr number of rows of this @bud
  !! @param[in] nc number of columns of this @bud
  !! @param[in] nz number of non-zero elements this @bud
  subroutine new_dim_(this, nr, nc, nz)
    type(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz

    call init(this)

    this%data%nr = nr
    this%data%nc = nc
    this%data%nz = nz
    
    allocate(this%data%rptr(nr+1))
    allocate(this%data%col(nz))

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
    type(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: rptr(nr), col(nz)

    ! Copy over information
    ! It must be 1-based
    if ( rptr(1) /= 1 ) then
      call delete(this)
      return
    end if

    ! pre-allocate
    call new(this, nr, nc, nz)

    ! Copy pointers
    this%data%rptr(1:this%data%nr) = rptr
    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
    this%data%rptr(this%data%nr+1) = this%data%nz + 1
    
    this%data%col = col

  end subroutine new_copy_

  !> Query number of non-zero elements
  elemental function nonzeros_(this) result(nnzs)
    type(BUD_TYPE_NAME), intent(in) :: this
    integer :: nnzs
    nnzs = this%data%nz
  end function nonzeros_

  !> Query number of rows of the stored sparse matrix
  elemental function mat_rows_(this) result(rows)
    type(BUD_TYPE_NAME), intent(in) :: this
    integer :: rows
    rows = this%data%nr
  end function mat_rows_

  !> Query number of columns of the stored sparse matrix
  elemental function mat_cols_(this) result(cols)
    type(BUD_TYPE_NAME), intent(in) :: this
    integer :: cols
    cols = this%data%nc
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
  !! @param[out] rptr @opt row pointer (`rptr(2)` is starting index of `ir=2`)
  !! @param[out] col @opt column index
  subroutine attach_(this, D, nr, nc, nz, rptr, col)
    type(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer, contiguous, optional :: rptr(:), col(:)

    if ( present(nr) ) nr = this%data%nr
    if ( present(nc) ) nc = this%data%nc
    if ( present(nz) ) nz = this%data%nz
    if ( present(rptr) ) rptr => this%data%rptr
    if ( present(col) ) col => this%data%col

  end subroutine attach_


  !> Get sparse index from double index
  !!
  !! This should only be used if one does scattered
  !! access to the matrix.
  !!
  !! Manual interaction with the sparsity pattern is preferred.
  !!
  !! @param[in] this the sparsity pattern @bud
  !! @param[in] ir the requested row
  !! @param[in] ic the requested column
  !! @param[out] idx the index of `(ir,ic)`, <0 if non-existing
  pure subroutine index_(this, ir, ic, idx)
    use BUD_CC2(BUD_MOD, _utils), only: find_bin
    type(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir, ic
    integer(BUD_TYPE_VAR_PREC), intent(out) :: idx
    
    call find_bin(this%data%rptr(ir+1)-this%data%rptr(ir), &
      this%data%col(this%data%rptr(ir):), ic, idx)
    
    if ( idx > 0 ) idx = this%data%rptr(ir) + idx

  end subroutine index_


  !> Wrap-around sparse matrix to form a smaller sparse matrix
  !!
  !! This reduces the column indices in a sparse pattern to
  !! an equivalent column reduced matrix by converting column
  !!  `c` to column `mod(c-1, nc) + 1`. Which in a 0-counted
  !! array would simply be: `mod(c,nc)`.
  !!
  !! This can be usefull for transforming an `M(nr, 2*nr)`
  !! matrix to `M(nr,nr)` square matrix.
  !!
  !! @param[in] from the originating sparse pattern
  !! @param[in] nc new number of columns in the sparse pattern
  !! @param[inout] to the resulting sparse pattern
  !! @param[in] unite @opt=.false. unites equivalent sparse
  !!                  elements to not have dublicate elements.
  !! @author Nick R. Papior (2016, February)
  subroutine get_wrapped_(from, nc, to, unite)
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick, modp

    type(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nc
    type(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: unite

    ! Local unite variable
    logical :: lunite

    ! from array elements
    integer(BUD_TYPE_VAR_PREC) :: fnr, fnz, fi
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: fptr(:), fcol(:)
    integer(BUD_TYPE_VAR_PREC) :: tnr, ti
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: tptr(:), tcol(:)
    
    integer(BUD_TYPE_VAR_PREC) :: ir, i
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
        tcol(i) = modp(fcol(i), nc)
      end do
      
    else

      ! First calculate the size of the matrix
      nullify(tptr,tcol)
      allocate(tptr(fnr+1),tcol(fnz))
      allocate(tmpcol(fptr(2)-fptr(1)))

      ! we correct the index at the end of
      ! each loop as it eases the
      ! readability of the algorithm
      tptr(1) = 0
      do ir = 1 , fnr
        
        ! ensure pivoting array long enough
        call realloc_tmpcol(ir)

        ! Create pivoting array so that we
        ! may easily find the reduced sparsity pattern
        call sort_quick(fptr(ir+1)-fptr(ir), &
          fcol(fptr(ir):), tmpcol(1:) )

        ! point to the following rows pointer
        ! this enables to count the number of entries
        ! in the current row as:
        !   number entries = tptr(ir+1) - tptr(ir)
        ind => tptr(ir+1)
        
        ! initialize counter for the following row
        ! (i.e. count number of entries)
        ind = tptr(ir)
        ! initialize first element
        tcol(ind) = modp(fcol(tptr(ir)+tmpcol(1)), nc)
        do i = 2 , fptr(ir+1) - fptr(ir)
          if ( fcol(tptr(ir)+tmpcol(i-1)) /= fcol(tptr(ir)+tmpcol(i)) ) then
            ind = ind + 1

            tcol(ind) = modp(fcol(tptr(ir)+tmpcol(i-1)), nc)
          end if
        end do

        tptr(ir) = tptr(ir) + 1
        
      end do
      ! update number of entries
      tptr(fnr+1) = tptr(fnr+1) + 1

      deallocate(tmpcol)

      ! create the new sparsity pattern
      call new(to, fnr, nc, tptr(fnr+1), tptr, tcol)
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
    type(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: rptr(:)
    
    rptr => this%data%rptr
    
  end function get_row_ptrp_

  !> Retrieve a pointer to the column indices
  !!
  !! @param[in] this type to retrieve the pointer from
  !! @return the pointer to the contained column indices segment
  function get_columnp_(this) result(col)
    type(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer, contiguous :: col(:)

    col => this%data%col
    
  end function get_columnp_

  
  !> Sorts the elements in the sparse pattern
  !!
  !! Sorts the sparse matrix such that the column
  !! index is always increasing for each row.
  !!
  !! @param[in] this @bud to finalize
  !! @param[out] err associated error message of the sparsity pattern
  !! @param[out] pvt @opt=@none if requested the pivoting array for the finalized sparsity pattern
  subroutine finalize_(this, err, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick

    type(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out) :: err
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: ir, i, nr, nc, nz
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
        call sort_quick(nz, col(rptr(ir):), pvt(rptr(ir):))
        do i = rptr(ir) , rptr(ir) + nz - 1
          pvt(i) = pvt(i) + rptr(ir)
          if ( col(i) > nc ) then
            err = - i
          end if
          if ( i > rptr(ir) ) then
            if ( col(i-1) == col(i) ) then
              err = SM_ENTRY_MULTIPLE
            end if
          end if
        end do
      end do

    else
      
      do ir = 1 , nr
        nz = rptr(ir+1) - rptr(ir)
        call sort_quick(nz, col(rptr(ir):))
        do i = rptr(ir) , rptr(ir) + nz - 1
          if ( col(i) > nc ) then
            err = - i
            return
          end if
          if ( i > rptr(ir) ) then
            if ( col(i-1) == col(i) ) then
              err = SM_ENTRY_MULTIPLE
              return
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
  !!
  !! @author Nick R. Papior (2016, February)
  subroutine print_(this, info, indent)
    type(BUD_TYPE_NAME), intent(in) :: this
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

    write(fmt, '(a,i0,a)') '(t',lindent,',2a)'

    if ( .not. is_initd(this) ) then
      write(*,fmt) trim(name), " not initialized."
      return
    end if

    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,4(i0,a))'
    
    write(*,fmt) "<", trim(name), " rows=", this%data%nr, &
      ", cols=", this%data%nc, ", nnzs=",this%data%nz, &
      ", refs: ", get_refs(this), ">"
    
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

