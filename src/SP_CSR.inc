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
  !  - BUD_SP_CSR
  !    case ( 0 ) ! default
  !      standard CSR format (MKL-SpBLAS)
  !    case ( 1 )
  !      zero based pointer CSR format
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

#ifndef BUD_SP_CSR
# define BUD_SP_CSR 0
#endif

#ifndef BUD_SP_INTEROP_C
# define BUD_SP_INTEROP_C 0
#endif

#if BUD_SP_CSR == 0
#elif BUD_SP_CSR == 1
# if BUD_SP_INTEROP_C == 1
#  error "SP_INTEROP_C may not be defined when using SP_CSR == 1"
# endif
#else
# error "SP_CSR *MUST* be either 0 or 1"
#endif


#if BUD_SP_CSR == 0
# if BUD_SP_INTEROP_C == 0
#  define BUD_SP_MOD modp
   ! This is the initial pointer value for the
   ! first pointer element
#  define BUD_SP_PTR 1
   ! this is the addition to the pointer arrays
#  define BUD_SP_PTR_A
#  define BUD_SP_PTR_B -1
# else
#  define BUD_SP_MOD mod
#  define BUD_SP_PTR 0
#  define BUD_SP_PTR_A +1
#  define BUD_SP_PTR_B
# endif
#else
# define BUD_SP_MOD modp
# define BUD_SP_PTR 0
# define BUD_SP_PTR_A +1
# define BUD_SP_PTR_B
#endif


#if BUD_SP_INTEROP_C == 0
# define BUD_SP_IDX(a) (a)
# define BUD_SP_IDXF(a) (a)
#elif BUD_SP_INTEROP_C == 1
# define BUD_SP_IDX(a) (a-1)
# define BUD_SP_IDXF(a) (a+1)
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

  !> Sparse matrix type in the CSR format
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

    procedure, private :: new_dim_
    procedure, private :: new_copy_

    procedure, private :: columnp_
    procedure, private :: column_rp_

    procedure, private :: remove_row_el_
    procedure, private :: remove_row_list_

    procedure, private :: remove_col_el_
    procedure, private :: remove_col_list_

    procedure, private :: translate_row_el_
    procedure, private :: translate_row_list_

    procedure, private :: translate_col_el_
    procedure, private :: translate_col_list_

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
    !> @iSee #nrow_p
    procedure, public :: nrow_p => nrowp_
#endif

    !> @iSee #column_p
    generic, public :: column_p => columnp_, column_rp_


    !> @iSee #remove_row
    generic, public :: remove_row => remove_row_el_, &
      remove_row_list_

    !> @iSee #remove_column
    generic, public :: remove_column => remove_col_el_, &
      remove_col_list_

    !> @iSee #translate_row
    generic, public :: translate_row => translate_row_el_, &
      translate_row_list_

    !> @iSee #translate_column
    generic, public :: translate_column => translate_col_el_, &
      translate_col_list_


    !> @iSee #sort
    procedure, public :: sort => sort_

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

    !> Whether the sparse pattern has been sorted, see #sort_sp
    logical :: sorted = .false.

    !> Index of the equivalent row (size `nr+1`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)

#if BUD_SP_CSR == 1
    !> Number of non-zero elements per row (`nrow(i)=rptr(i+1)-rptr(i)`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: nrow(:)
#endif

    !> The column index of the equivalent sparse matrix (size `nz`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: col(:)


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER


  !> Create a new sparse matrix
  !!
  !! You may either create an empty sparse matrix with
  !! a fixed size, or create a sparse matrix from
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




  !> Retrieve a pointer to the row offsets/pointers
  !!
  !! A pointer with the row offsets.
  !!
#if BUD_SP_CSR == 0
# if BUD_SP_INTEROP_C == 0
  !! `M(ir,col(pointer(ir)))`
# else
  !! `M(ir,col(pointer(ir)+1))`
# endif
  !! is the first sparse element in row `ir`.
#elif BUD_SP_CSR == 1
  !! `M(ir,col(pointer(ir)+1))` is the first sparse
  !! element in row `ir`.
#endif
  interface offset_p
    module procedure offsetp_
  end interface
  public :: offset_p


  !> Retrieve a pointer to the column indices
  interface column_p
    module procedure columnp_
    module procedure column_rp_
  end interface
  public :: column_p

#if BUD_SP_CSR == 1
  !> Retrieve a pointer to the number of entries per row
  interface nrow_p
    module procedure nrowp_
  end interface
  public :: nrow_p
#endif


  !> Remove rows from the sparse pattern
  interface remove_row
    module procedure remove_row_el_
    module procedure remove_row_list_
  end interface
  public :: remove_row

  !> Remove columns from the sparse pattern
  interface remove_column
    module procedure remove_col_el_
    module procedure remove_col_list_
  end interface
  public :: remove_column


  !> Translate rows in the sparse pattern to new rows
  !!
  !! This will essentially change all entries for given
  !! rows into a new set of rows.
  !! Furthermore, by denoting the new row with a negative
  !! index the element will be deleted.
  interface translate_row
    module procedure translate_row_el_
    module procedure translate_row_list_
  end interface
  public :: translate_row


  !> Translate columns in the sparse pattern to new columns
  !!
  !! This will essentially change all entries for given
  !! columns into a new set of columns.
  !! Furthermore, by denoting the new column with a negative
  !! index the column will be deleted.
  interface translate_column
    module procedure translate_col_el_
    module procedure translate_col_list_
  end interface
  public :: translate_column


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
  !!  - `err == SP_INPUT + 3`:
  !!    the 3rd input is erroneously supplied.
  interface sort
    module procedure sort_
  end interface
  public :: sort

  ! Include the common elements of a sparsity method
  ! This includes "bud_common.inc"
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
    if ( .not. allocated(this%col) ) return
    deallocate(this%rptr)
    deallocate(this%col)
#if BUD_SP_CSR == 1
    deallocate(this%nrow)
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
      from%D%rptr, from%D%col)
    to%D%sorted = from%D%sorted

    ! Copy the common data-content
    call common_copy_(from, to)

  end subroutine copy_

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
    this%D%sorted = .false.

    allocate(this%D%rptr(nr+1))
#if BUD_SP_CSR == 1
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

#if BUD_SP_CSR == 1
    integer(BUD_TYPE_VAR_PREC) :: ir
#endif

    ! Copy over information
    ! It must be based on the starting index
    if ( rptr(1) /= BUD_SP_PTR ) then
      call delete(this)
      return
    end if

    ! pre-allocate
    call new(this, nr, nc, nz)

    ! Copy pointers
    this%D%rptr(1:this%D%nr) = rptr

    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
    this%D%rptr(this%D%nr+1) = this%D%nz + BUD_SP_PTR

#if BUD_SP_CSR == 1
    do ir = 1 , nr
      this%D%nrow(ir) = this%D%rptr(ir+1) - this%D%rptr(ir)
    end do
#endif

    this%D%col = col

  end subroutine new_copy_


  !> @param[in] this sparse matrix
  !! @return a pointer to the row offsets for the sparse matrix (contiguous)
  function offsetp_(this) result(rptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:)

    rptr => this%D%rptr

  end function offsetp_

  !> @param[in] this sparse matrix
  !! @return a pointer to the column indices for the sparse matrix (contiguous)
  function columnp_(this) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    col => this%D%col

  end function columnp_

  !> @param[in] this sparse matrix
  !> @param[in] r only retrieve the columns that reside in row `r`
  !! @return a pointer to the column indices for the sparse matrix of row `r` (contiguous)
  function column_rp_(this, r) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: r
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    col => this%D%col(this%D%rptr(r)BUD_SP_PTR_A:this%D%rptr(r+1)BUD_SP_PTR_B)

  end function column_rp_

#if BUD_SP_CSR == 1
  !> @param[in] this sparse matrix
  !! @return a pointer to the array that holds the number of entries per row
  function nrowp_(this) result(nrow)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: nrow(:)

    nrow => this%D%nrow

  end function nrowp_
#endif


  !> @param[in] this sparse matrix @bud
  !! @param[out] nr @opt number of rows in SP
  !! @param[out] nc @opt number of columns in SP
  !! @param[out] nz @opt number of non-zero elements in SP
  !! @param[out] rptr @opt row pointer (`rptr(2)BUD_SP_PTR_A` is starting index of `ir=2`)
#if BUD_SP_CSR == 1
  !! @param[out] nrow @opt number of non-zero elements per row
#endif
  subroutine attach_(this, D, nr, nc, nz, rptr, col &
#if BUD_SP_CSR == 1
    , nrow &
#endif
    )
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: rptr(:), col(:)
#if BUD_SP_CSR == 1
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: nrow(:)
#endif

    if ( present(nr) ) nr = this%D%nr
    if ( present(nc) ) nc = this%D%nc
    if ( present(nz) ) nz = this%D%nz
    if ( present(rptr) ) rptr => this%D%rptr
    if ( present(col) ) col => this%D%col
#if BUD_SP_CSR == 1
    if ( present(nrow) ) nrow => this%D%nrow
#endif

  end subroutine attach_


  !> @param[in] this the sparse matrix (sorted, @isee #sp_sort)
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

      do idx = this%D%rptr(ir) BUD_SP_PTR_A , this%D%rptr(ir+1) BUD_SP_PTR_B
        if ( this%D%col(idx) == ic ) return
      end do

      idx = -1

      return

    end if

    call find_bin(this%D%rptr(ir+1)-this%D%rptr(ir), &
      this%D%col(this%D%rptr(ir) BUD_SP_PTR_A:), ic, idx)
    if ( idx > 0 ) idx = this%D%rptr(ir) BUD_SP_PTR_B + idx

  end function index_



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
    integer(BUD_TYPE_VAR_PREC) :: ir, nr, nz, ptr, i
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:), tvt(:)

    ! Initialize error
    err = 0

    ! Get sparse matrix array
    call attach(this, nr=nr, nz=nz, rptr=rptr, col=col)

    if ( present(pvt) ) then

      if ( size(pvt) < nz ) then
        err = SP_INPUT + 3
        pvt = -1
        return
      end if

      do ir = 1 , nr

        nz = rptr(ir+1) - rptr(ir)
        ptr = rptr(ir) - BUD_SP_PTR

        ! get pivoting array
        tvt => pvt(ptr+1:)

        call sort_quick(nz, col(ptr+1:), tvt)
        do i = 1 , nz
          tvt(i) = ptr + tvt(i)
        end do
        do i = 1 , nz
          col(ptr + i) = col(tvt(i))
        end do

      end do

    else

      do ir = 1 , nr

        col => column_p(this, ir)
        nz = size(col)

        call sort_quick(nz, col)

      end do

    end if

    this%D%sorted = .true.

  end subroutine sort_


  !> @param[in] sp1 the first sparse pattern
  !> @param[in] sp2 the second sparse pattern
  !> @param[inout] sp the union of `sp1` and `sp2`
  subroutine union_(sp1, sp2, sp)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: sp1, sp2
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: sp

    type(BUD_LIST_NAME) :: L1, L2, L
    integer(BUD_TYPE_VAR_PREC) :: ir, n, nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    ! A union removes dublicate entries...
    ! This may be undesired.. :(
    call attach(sp1, nr=nr, nc=nc)

    ! Create a list with the pointers
    allocate(rptr(nr+1))
    rptr(1) = BUD_SP_PTR
    do ir = 1 , nr

      ! Create union...
      col => column_p(sp1, ir)
      n = size(col)
      call new(L1, n, col)
      col => column_p(sp2, ir)
      n = size(col)
      call new(L2, n, col)

      ! Sort both lists (makes unions faster)
      call sort(L1)
      call sort(L2)

      ! Create union...
      call union(L1, L2, L2)
      ! We also ensure it is sorted.
      call sort(L2)

      ! push the rows to the new list
      call push(L, L2)

      rptr(ir+1) = rptr(ir) + size(L2)

    end do

    col => list_p(L)
    call new(sp, nr, nc, nz, rptr, col)

    call delete(L1)
    call delete(L2)
    call delete(L)

  end subroutine union_


  !> @param[in] from the originating sparse pattern
  !! @param[in] n number of elements in `remove`
  !! @param[in] remove columns that are removed
  !! @param[inout] to sparse pattern after deleting the columns
  subroutine remove_col_el_(from, n, remove, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n, remove(n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lrem

    ! Create a new list
    call new(lrem, n, remove)
    call sort(lrem)

    call remove_col_list_(from, lrem, to)

    call delete(lrem)

  end subroutine remove_col_el_

  !> @param[in] from the originating sparse pattern
  !! @param[in] remove list with columns that are removed
  !! @param[inout] to sparse pattern after deleting the columns
  subroutine remove_col_list_(from, remove, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: remove
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: ll
    integer(BUD_TYPE_VAR_PREC) :: i, n
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: lst(:)

    ! Create a new list
    n = size(remove)
    call new(ll, n)
    lst => list_p(ll)
    do i = 1 , n
      lst(i) = -1
    end do

    call translate_col_list_(from, remove, ll, to)

    call delete(ll)

  end subroutine remove_col_list_


  !> @param[in] from the originating sparse pattern
  !! @param[in] nin number of elements in `in_col`
  !! @param[in] in_col the set of columns that will be translated into `out_col` (preferentially this should be sorted)
  !! @param[in] nout number of elements in `out_col`
  !! @param[in] out_col the set of translation columns
  !! @param[inout] to the resulting sparse pattern after translating the columns
  subroutine translate_col_el_(from, nin, in_col, nout, out_col, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nin, in_col(nin), nout, out_col(nout)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lin, lout

    call new(lin, nin, in_col)
    call new(lout, nout, out_col)

    call translate_col_list_(from, lin, lout, to)

    call delete(lin)
    call delete(lout)

  end subroutine translate_col_el_


  !> @param[in] from the originating sparse pattern
  !! @param[in] in_col a list set of columns that will be translated into `out_col` (preferentially this should be sorted)
  !! @param[in] out_col a list set of translation columns
  !! @param[inout] to the resulting sparse pattern after translating the columns
  subroutine translate_col_list_(from, in_col, out_col, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME) :: in_col, out_col
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lcol
    type(BUD_TYPE_NAME) :: tmp
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nz, ir, i, idx
    integer(BUD_TYPE_VAR_PREC), allocatable :: trptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: oc(:)

    call attach(from, nr=nr, nc=nc, nz=nz, rptr=rptr, col=col)

    ! Retrieve list
    oc => list_p(out_col)

    ! Create a list with the columns
    call new(lcol, nz)
    allocate(trptr(nr+1))

    ! Now start create the sparse pattern
    trptr(1) = rptr(1)
    do ir = 1, nr

      ! figure out if they should be translated
      do i = rptr(ir) BUD_SP_PTR_A, rptr(ir+1) BUD_SP_PTR_B
        idx = index(in_col, col(i))
        if ( idx <= 0 ) then
          call push(lcol, col(i))
        else if ( oc(idx) > 0 ) then
          ! we have a translation
          ! check if the resulting translated column is
          ! negative, if so, it means deletion

          ! we retain the item
          call push(lcol, oc(idx))
        end if
      end do

      ! update the following pointer
      trptr(ir+1) = size(lcol) + BUD_SP_PTR

    end do

    col => list_p(lcol)
    call new(to, nr, nc, size(lcol), trptr, col)

    call delete(lcol)

  end subroutine translate_col_list_


  !> @param[in] from the originating sparse pattern
  !! @param[in] n number of elements in `remove`
  !! @param[in] remove rows that are removed
  !! @param[inout] to sparse pattern after deleting the rows
  subroutine remove_row_el_(from, n, remove, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n, remove(n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lrem

    ! Create a new list
    call new(lrem, n, remove)
    call sort(lrem)

    call remove_row_list_(from, lrem, to)

    call delete(lrem)

  end subroutine remove_row_el_

  !> @param[in] from the originating sparse pattern
  !! @param[in] remove list with rows that are removed
  !! @param[inout] to sparse pattern after deleting the rows
  subroutine remove_row_list_(from, remove, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: remove
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: ll
    integer(BUD_TYPE_VAR_PREC) :: i, n
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: lst(:)

    ! Create a new list
    n = size(remove)
    call new(ll, n)
    lst => list_p(ll)
    do i = 1 , n
      lst(i) = -1
    end do

    call translate_row_list_(from, remove, ll, to)

    call delete(ll)

  end subroutine remove_row_list_



  !> @param[in] from the originating sparse pattern
  !! @param[in] nin number of elements in `in_row`
  !! @param[in] in_row the set of rows that will be translated into `out_row` (preferentially this should be sorted)
  !! @param[in] nout number of elements in `out_row`
  !! @param[in] out_row the set of translation rows
  !! @param[inout] to the resulting sparse pattern after translating the rows
  subroutine translate_row_el_(from, nin, in_row, nout, out_row, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nin, in_row(nin), nout, out_row(nout)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lin, lout

    call new(lin, nin, in_row)
    call new(lout, nout, out_row)

    call translate_row_list_(from, lin, lout, to)

    call delete(lin)
    call delete(lout)

  end subroutine translate_row_el_

  !> @param[in] from the originating sparse pattern
  !! @param[in] in_row a list set of rows that will be translated into `out_row`
  !! @param[in] out_row a list set of translation rows (preferentially this should be sorted)
  !! @param[inout] to the resulting sparse pattern after translating the rows
  subroutine translate_row_list_(from, in_row, out_row, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME) :: in_row, out_row
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME), allocatable :: lrow(:)
    type(BUD_LIST_NAME) :: tmp
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nnr, nz, r, ir, i, idx, del_nc
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: row(:), col(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: or(:)

    call attach(from, nr=nr, nc=nc)

    ! This algorithm moves in_row to out_row, and possibly deletes
    ! rows.
    allocate(lrow(nc))

    ! Initialize the sparse lists
    do ir = 1 , nr
      call new(lrow(ir), BUD_CC2(1_,BUD_TYPE_VAR_PREC))
    end do

    ! List of out rows
    or => list_p(out_row)

    ! Now actually populate the sparse rows
    do ir = 1 , nr

      ! Get rows in current column
      col => column_p(from, ir)
      i = int(size(col), BUD_TYPE_VAR_PREC)

      ! Figure out if this is in in_row (i.e. should it be translated)
      idx = index(in_row, ir)
      if ( idx < 1 ) then

        ! The column is not translated
        call push(lrow(ir), i, col)

      else if ( or(idx) >= BUD_SP_IDX(1) ) then

        ! Find where the new column has moved
        r = BUD_SP_IDXF(or(idx))
        call push(lrow(r), i, col)

      end if

    end do

    ! Now re-create the new sparsity pattern
    ! First figure out the new number of rows...
    row => list_p(in_row)
    nnr = nr
    do i = 1 , size(in_row)
      idx = index(out_row, row(i))
      if ( idx <= 0 ) then
        ! We also delete the row, so we may recognize it
        call delete(lrow(row(i)))
        nnr = nnr - 1
      end if
    end do

    ! Now re-create the pointers
    allocate(rptr(nnr+1))
    rptr(1) = BUD_SP_PTR
    ! Loop on old rows
    ir = 0
    do r = 1 , nr

      ! if the list has been deleted, simply
      ! skip
      if ( .not. is_initd(lrow(r)) ) cycle
      ir = ir + 1

      ! Create full list of columns
      call push(tmp, lrow(r))

      ! Create the pointer
      rptr(ir+1) = rptr(ir) + size(lrow(r))

      call delete(lrow(r))

    end do

    ! Now we have, rptr, and column
    col => list_p(tmp)
    ! finally we also know that all elements of lrow
    ! has been deleted

    call new(to, nnr, nc, size(tmp), rptr, col)

    call delete(tmp)
    deallocate(lrow)

  end subroutine translate_row_list_



! Local pre-processor variables that
! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_LIST_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

  ! Control variables
#undef BUD_SP_CSR
#undef BUD_SP_INTEROP_C
#undef BUD_SP_MOD
#undef BUD_SP_IDX
#undef BUD_SP_IDXF

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

