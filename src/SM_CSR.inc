  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This module may be controlled via these
  ! preprocessor variables:
  !  - BUD_MOD_NAME name of the module
  !  - BUD_TYPE_NAME name of the public type
  !  - BUD_TYPE_NAME_ name of the internal data pointer
  !  - BUD_TYPE_NAME_STR the routine name in "string" format (for IO)
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

  ! Retrieve the settings for the sparse matrix
# include "SM.inc"

#if BUD_SM_CSR == 0
#elif BUD_SM_CSR == 1
# if BUD_SM_INTEROP_C == 1
#  error "SM_INTEROP_C may not be defined when using SM_CSR == 1"
# endif
#else
# error "SM_CSR *MUST* be either 0 or 1"
#endif


#include "bud_utils.inc"

  use BUD_CC3(BUD_MOD,_,SM_common)

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Sparse matrix type in the CSR format
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    !> @name Private procedures
    !> @{
    ! Doxygen needed line

    procedure, private :: new_dim_
    procedure, private :: new_copy_

    procedure, private :: column_p_
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

    !> @iSee #new
    generic, public :: new => new_dim_, new_copy_

    !> @iSee #nonzeros
    procedure, public :: nonzeros => nonzeros_

    !> @iSee #max_nonzeros
    procedure, public :: max_nonzeros => max_nonzeros_

    !> @iSee #rows
    procedure, public :: rows => rows_

    !> @iSee #columns
    procedure, public :: columns => columns_

    !> @iSee #size
    procedure, public :: size => size_

    !> @iSee #index
    procedure, public :: index => index_

    !> @iSee #offset_p
    procedure, public :: offset_p => offset_p_

    !> @iSee #nrow_p
    procedure, public :: nrow_p => nrow_p_

    !> @iSee #column_p
    generic, public :: column_p => column_p_, column_rp_


    !> @iSee #add_element
    procedure, public :: add_element => add_element_


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

    !> @iSee #equivalent
    procedure, public :: equivalent => equivalent_

    !> @iSee #finalize
    procedure, public :: finalize => finalize_

    !> @iSee #attach
    procedure, public :: attach => attach_

#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DEVELOPER

  ! These fields are used in the sparse matrix stuff.
  integer(BUD_TYPE_VAR_PREC), parameter :: ONE = BUD_CC2(1_,BUD_TYPE_VAR_PREC)
  integer(BUD_TYPE_VAR_PREC), parameter :: ZERO = BUD_CC2(0_,BUD_TYPE_VAR_PREC)

  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains the sparsity pattern for a CSR matrix.
  type BUD_TYPE_NAME_

    !> Number of rows in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nr = ZERO
    !> Number of columns in the matrix
    integer(BUD_TYPE_VAR_PREC) :: nc = ZERO
    !> Number of non-zero elements in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nz = ZERO
    !> Total number of elements possible in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nt = ZERO

    !> Whether the sparse matrix has been sorted, see #sort
    logical :: sorted = .false.

    !> Whether the sparse matrix has been finalized, see #finalize
    logical :: finalized = .false.

    !> Index of the equivalent row (size `nr+1`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)

    !> Number of non-zero elements per row
    integer(BUD_TYPE_VAR_PREC), allocatable :: nrow(:)

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
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: new


  !> Retrieve a pointer to the row offsets/pointers
  !!
  !! A pointer with the row offsets.
  !!
#if BUD_SM_CSR == 0
# if BUD_SM_INTEROP_C == 0
  !! `M(ir,col(pointer(ir)))`
# else
  !! `M(ir,col(pointer(ir)+1))`
# endif
  !! is the first sparse element in row `ir`.
#elif BUD_SM_CSR == 1
  !! `M(ir,col(pointer(ir)+1))` is the first sparse
  !! element in row `ir`.
#endif
  interface offset_p
    module procedure offset_p_
  end interface
  public :: offset_p


  !> Retrieve a pointer to the column indices
  interface column_p
    module procedure column_p_
    module procedure column_rp_
  end interface
  public :: column_p

  !> Retrieve a pointer to the number of entries per row
  interface nrow_p
    module procedure nrow_p_
  end interface
  public :: nrow_p

  !> Retrieve the sparse index for the row and column (-1 if non existing)
  interface index
    module procedure index_
  end interface
  public :: index


  !> Remove rows from the sparse matrix
  interface remove_row
    module procedure remove_row_el_
    module procedure remove_row_list_
  end interface
  public :: remove_row

  !> Remove columns from the sparse matrix
  interface remove_column
    module procedure remove_col_el_
    module procedure remove_col_list_
  end interface
  public :: remove_column


  !> Translate rows in the sparse matrix to new rows
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


  !> Translate columns in the sparse matrix to new columns
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


  !> Sorts columns in the sparse matrix
  !!
  !! Sorts the sparse matrix such that the column
  !! index is always increasing for each row.
  !! This will generally allow faster access patterns.
  !!
  !! One may query the error of the object to check
  !! whether the input is correct.
  interface sort
    module procedure sort_
  end interface
  public :: sort


  !> Checks whether two sparse matrices have the same elements
  !!
  !! This routine returns `.true.` if the two
  !! sparse matrices have all the same elements in the
  !! same order.
  !!
  !! In case it is the same object it returns immediately
  !! with `.true.`.
  interface equivalent
    module procedure equivalent_
  end interface
  public :: equivalent


  !> Finalize the sparsity pattern by removing non-used elements
  !!
  !! After finalization the number of non-zero elements
  !! and the total number of possible elements are the same.
  !! I.e. this routine removes all non-used elements in the
  !! sparse matrix.
  !!
  !! @note this routine re-creates the sparse matrix,
  !! so any pointers to the internal data-structure
  !! must be re-instantiated before use.
  interface finalize
    module procedure finalize_
  end interface
  public :: finalize


  !> Adds a non-zero element to the sparse matrix
  !!
  !! Add a non-zero element to the sparse matrix.
  !! In case there is not enough space the sparse matrix
  !! will be re-allocated and copied to it-self.
  !! Adding an element will thus not necessarily preserve
  !! the allocated elements and any pointers to the
  !! data contained needs to be updated.
  !!
  !! If the error is `0` there was no need to
  !! extend the sparse matrix data.
  !! If the error is `-1` the element has not been added
  !! If the error is above `0` the sparse matrix
  !! has been re-allocated and the element is contained.
  !! The re-allocation ensures that all rows can
  !! contain `maxval(nrow)+8` and can thus result in
  !! a large memory increase.
  interface add_element
    module procedure add_element_
  end interface
  public :: add_element


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
  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this
    integer :: stat, istat

    this%D%nr = 0
    this%D%nc = 0
    this%D%nz = 0
    this%D%nt = 0
    this%D%sorted = .false.
    this%D%finalized = .false.

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. allocated(this%D%col) ) return
    stat = 0
    deallocate(this%D%rptr, stat=istat)
    if ( 0 /= istat ) stat = istat
    deallocate(this%D%col, stat=stat)
    if ( 0 /= istat ) stat = istat
    deallocate(this%D%nrow, stat=stat)
    if ( 0 /= istat ) stat = istat

  end subroutine delete_

  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full copied data
  !! @param[in] dealloc @opt=.true. whether `to` should be de-allocated before
  subroutine copy_(from, to, dealloc)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    logical, intent(in), optional :: dealloc
    logical :: ldealloc
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), nrow(:), fcol(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:)
    integer(BUD_TYPE_VAR_PREC) :: i, ir, nr

    call set_error(to, 0)

    ! Check options
    ldealloc = .true.
    if ( present(dealloc) ) ldealloc = dealloc
    if ( ldealloc ) call delete(to)

    ! quick return if there is nothing to copy
    if ( .not. is_initd(from) ) return

    if ( is_initd(to) ) then

      ! Check if to has room
      if ( rows(from) /= rows(to) .or. &
        columns(from) /= columns(to) ) then

        ! We do not have the same shape
        call set_error(to, -1)
        return

      end if

      ! They have the same dimensions
      ! Check there is room
      call attach(from, nr=nr, rptr=fptr, nrow=nrow, col=fcol)
      call attach(to, rptr=tptr)

      do ir = 1 , nr

        if ( nrow(ir) > tptr(ir+ONE) - tptr(ir) ) then
          call set_error(to, ir)
          exit
        end if

      end do

      ! quick return if there is not room
      ! The error message will return the column
      ! where there is not enough room
      if ( error(to) /= 0 ) return

      ! Now reset all elements in `to`
      call attach(to, nrow=nrow)

      ! reset everything
      nrow(:) = 0

      ! Loop all elements in from and copy them to
      to%D%sorted = .true.
      do ir = 1, nr

        fcol => column_p(from, ir)

        do i = 1, size(fcol)
          call add_element(to, ir, fcol(i))
        end do

      end do

    else

      ! We make a copy by creating it
      call new(to, from%D%nr, from%D%nc, from%D%nz, &
        from%D%rptr, from%D%col, from%D%nrow)
      to%D%sorted = from%D%sorted

    end if

  end subroutine copy_

  !> @param[inout] this the sparse matrix
  !! @param[in] nr number of rows of the matrix
  !! @param[in] nc number of columns of the matrix
  !! @param[in] nt @opt total number of possible non-zero elements
  !! @param[in] npr @opt number of non-zero elements per row (defaults to 10 if `nt` is not specified)
  subroutine new_dim_(this, nr, nc, nt, npr)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc
    integer(BUD_TYPE_VAR_PREC), intent(in), optional :: nt, npr
    integer(BUD_TYPE_VAR_PREC) :: r

    call initialize(this)

    this%D%nr = nr
    this%D%nc = nc
    if ( present(nt) ) then
      this%D%nt = nt
    else if ( present(npr) ) then
      this%D%nt = npr * nr
    else
      this%D%nt = nr * BUD_CC2(10_,BUD_TYPE_VAR_PREC)
    end if
    this%D%nz = 0
    this%D%sorted = .false.
    this%D%finalized = .false.

    allocate(this%D%rptr(nr+1))
    allocate(this%D%nrow(nr))
    allocate(this%D%col(this%D%nt))

    ! Initialize sparse matrix
    if ( present(npr) ) then
      this%D%rptr(1) = BUD_SM_PTR
      do r = 1 , nr
        this%D%rptr(r) = this%D%rptr(r-ONE) + npr
      end do
      this%D%rptr(nr+1) = this%D%nt + BUD_SM_PTR
    else
      this%D%rptr(1) = BUD_SM_PTR
    end if
    do r = 1 , nr
      this%D%nrow(r) = ZERO
    end do

  end subroutine new_dim_


  !> @param[inout] this the new sparse matrix
  !! @param[in] nr number of rows of the matrix
  !! @param[in] nc number of columns of the matrix
  !! @param[in] nz number of non-zero elements of the matrix
  !! @param[in] rptr row pointers (at least size `nr`)
  !! @param[in] col column indices for the sparse elements
  !! @param[in] nrow @opt number of column elements per row
  subroutine new_copy_(this, nr, nc, nz, rptr, col, nrow)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: rptr(nr), col(nz)
    integer(BUD_TYPE_VAR_PREC), intent(in), optional :: nrow(nr)

    integer(BUD_TYPE_VAR_PREC) :: r

    ! Copy over information
    ! It must be based on the starting index
    if ( rptr(1) /= BUD_SM_PTR ) then
      call delete(this)
      return
    end if

    ! pre-allocate
    call new(this, nr, nc, nz)
    this%D%nz = nz

    ! Copy pointers
    do r = 1 , nr
      this%D%rptr(r) = rptr(r)
    end do

    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
    this%D%rptr(this%D%nr+1) = this%D%nz + BUD_SM_PTR

    if ( present(nrow) ) then
      do r = 1 , nr
        this%D%nrow(r) = nrow(r)
      end do
    else
      do r = 1 , nr
        this%D%nrow(r) = this%D%rptr(r+1) - this%D%rptr(r)
      end do
    end if

    do r = 1 , nz
      this%D%col(r) = col(r)
    end do

  end subroutine new_copy_


  !> @param[in] this sparse matrix
  !! @return a pointer to the row offsets for the sparse matrix (contiguous)
  function offset_p_(this) result(rptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:)

    rptr => this%D%rptr

  end function offset_p_

  !> @param[in] this sparse matrix
  !! @return a pointer to the column indices for the sparse matrix (contiguous)
  function column_p_(this) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    col => this%D%col

  end function column_p_

  !> @param[in] this sparse matrix
  !> @param[in] r only retrieve the columns that reside in row `r`
  !! @return a pointer to the column indices for the sparse matrix of row `r` (contiguous)
  function column_rp_(this, r) result(col)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: r
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    col => this%D%col(this%D%rptr(r)BUD_SM_PTR_A:this%D%rptr(r)+this%D%nrow(r)BUD_SM_PTR_B)

  end function column_rp_

  !> @param[in] this sparse matrix
  !! @return a pointer to the array that holds the number of entries per row
  function nrow_p_(this) result(nrow)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: nrow(:)

    nrow => this%D%nrow

  end function nrow_p_


  !> @param[in] this sparse matrix @bud
  !! @param[out] nr @opt number of rows in SM
  !! @param[out] nc @opt number of columns in SM
  !! @param[out] nz @opt number of non-zero elements in SM
  !! @param[out] nt @opt total number of possible non-zero elements in SM
  !! @param[out] rptr @opt row pointer (`rptr(2)BUD_SM_PTR_A` is starting index of `ir=2`)
  !! @param[out] nrow @opt number of non-zero elements per row
  subroutine attach_(this, D, nr, nc, nz, nt, rptr, col, nrow)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz, nt
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: rptr(:), col(:)
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: nrow(:)

    if ( is_initd(this) ) then
      if ( present(nr) ) nr = this%D%nr
      if ( present(nc) ) nc = this%D%nc
      if ( present(nz) ) nz = this%D%nz
      if ( present(nt) ) nt = this%D%nt
      if ( present(rptr) ) rptr => this%D%rptr
      if ( present(col) ) col => this%D%col
      if ( present(nrow) ) nrow => this%D%nrow
    else
      if ( present(nr) ) nr = ZERO
      if ( present(nc) ) nc = ZERO
      if ( present(nz) ) nz = ZERO
      if ( present(nt) ) nt = ZERO
      if ( present(rptr) ) nullify(rptr)
      if ( present(col) ) nullify(col)
      if ( present(nrow) ) nullify(nrow)
    end if

  end subroutine attach_


  !> @param[in] this the sparse matrix (sorted, @isee #sp_sort)
#if BUD_SM_CSR == 0 && BUD_SM_INTEROP_C == 1
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

      do idx = this%D%rptr(ir) BUD_SM_PTR_A , this%D%rptr(ir) + this%D%nrow(ir) BUD_SM_PTR_B
        if ( this%D%col(idx) == ic ) return
      end do

      idx = - ONE

      return

    end if

    call find_bin(this%D%nrow(ir), &
      this%D%col(this%D%rptr(ir) BUD_SM_PTR_A:), ic, idx)
    if ( idx > ZERO ) idx = this%D%rptr(ir) BUD_SM_PTR_B + idx

  end function index_



  !> @param[inout] this sparse matrix to sort (in-place)
  !! @param[out] pvt @opt=@none if requested the pivoting array for the sorted sparsity pattern
  subroutine sort_(this, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD, _utils), only: sort_quick

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out), target, optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: ir, nr, nz, ptr, i
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), nrow(:), col(:), tvt(:)

    ! Initialize error
    call set_error(this, 0)

    ! Get sparse matrix array
    call attach(this, nr=nr, nz=nz, rptr=rptr, col=col, nrow=nrow)

    if ( present(pvt) ) then

      if ( size(pvt) < nz ) then
        call set_error(this, SM_INPUT + 3)
        pvt = -ONE
        return
      end if

      do ir = 1 , nr

        nz = nrow(ir)
        ptr = rptr(ir) - BUD_SM_PTR

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

        call sort_quick(nz, col(1:))

      end do

    end if

    this%D%sorted = .true.

  end subroutine sort_


  !> @param[inout] a first sparse matrix
  !! @param[inout] b second sparse matrix
  !! @return whether they have the same entries
  function equivalent_(a, b) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: a, b
    logical :: is

    integer(BUD_TYPE_VAR_PREC) :: i, nr

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: arow(:), acol(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: brow(:), bcol(:)

    is = .true.
    if ( same(a, b) ) return

    is = rows(a) == rows(b) .and. &
      columns(a) == columns(b)
    if ( .not. is ) return

    ! get information
    call attach(a, nr=nr, nrow=arow)
    call attach(b, nrow=brow)

    do i = 1, nr

      ! Check number of elements per row
      is = arow(i) == brow(i)
      if ( .not. is ) return

      ! check that there are elements
      if ( arow(i) == 0 ) cycle

      acol => column_p(a, i)
      bcol => column_p(b, i)

      is = all(acol == bcol)
      if ( .not. is ) return

    end do

  end function equivalent_


  !> @param[inout] this sparse matrix to finalize (in-place)
  subroutine finalize_(this)
    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: ir, nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), col(:)

    type(BUD_LIST_NAME) :: lptr, lcol
    integer(BUD_TYPE_VAR_PREC) :: p
    logical :: sorted

    ! Initialize error
    call set_error(this, 0)

    ! Get sparse matrix array
    call attach(this, nr=nr, nc=nc, nz=nz, col=col)

    ! initialize lists
    call new(lptr, nr)
    call new(lcol, nz)

    ! Initialize the pointer
    p = BUD_SM_PTR
    call push(lptr, p)

    do ir = 1 , nr

      col => column_p(this, ir)
      nz = size(col)

      ! Push the new pointer index
      p = p + nz
      call push(lptr, p)

      ! Push the column-indices to the list
      call push(lcol, nz, col)

    end do

    ! Determine whether we should sort it after-wards
    ! I.e. maintain state of object
    sorted = is_sorted(this)

    p = size(lcol)
    ! Get pointers
    rptr => list_p(lptr)
    col => list_p(lcol)
    call new(this, nr, nc, p, rptr, col)
    call delete(lptr)
    call delete(lcol)

    if ( sorted ) then
      call sort(this)
    end if
    this%D%finalized = .true.

  end subroutine finalize_


  !> @param[inout] this the sparse matrix to add an element to
  !! @param[in] ir the row index of the element
  !! @param[in] ic the column index of the element
  !! @param[in] dry_run @opt=.false., if `.true.` will do nothing but issue error messages as though it had runned.
  recursive subroutine add_element_(this, ir, ic, dry_run)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir, ic
    logical, intent(in), optional :: dry_run

    logical :: ldry
    type(BUD_TYPE_NAME) :: nthis
    integer(BUD_TYPE_VAR_PREC) :: i, r, ix, nr, nc, npr

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nrow(:), col(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr_(:), nrow_(:), col_(:)

    ldry = .false.
    if ( present(dry_run) ) ldry = dry_run

    call attach(this, nr=nr, nc=nc, &
      rptr=ptr, nrow=nrow, col=col)

    ! check if there is place in the sparse matrix
    ! for an entry without re-allocation
    if ( ptr(ir+ONE) - ptr(ir) > nrow(ir) ) then

      ix = ptr(ir) BUD_SM_PTR_A +nrow(ir)
      if ( nrow(ir) == 0 ) then
        i = 0
      else
        i = col(ix - ONE)
      end if
      if ( .not. ldry ) then
        col(ix) = ic
        nrow(ir) = nrow(ir) + ONE
        this%D%nz = this%D%nz + ONE

        if ( this%D%sorted .and. ic < i ) then

          ! denote it non-sorted
          this%D%sorted = .false.

        end if
      end if

      call set_error(this, 0)

      return

    end if

    ! Re-allocate the sparse matrix and copy it
    ! We add 8 elements per row.
    ! Perhaps this should be defined by the user.
    npr = maxval(nrow) + BUD_CC2(8_,BUD_TYPE_VAR_PREC)
    ! Quick return if user request dry-run
    if ( ldry ) then
      call set_error(this, npr)
      return
    end if

    call new(nthis, nr, nc, npr=npr)
    call attach(nthis, rptr=ptr_, nrow=nrow_, col=col_)

    ! Copy all elements correctly
    ix = ONE
    do r = 1 , nr
      nrow_(r) = nrow(r)

      ix = ptr_(r) BUD_SM_PTR_A
      ! copy this row
      do i = ptr(r) BUD_SM_PTR_A , ptr(r) + nrow(r) BUD_SM_PTR_B
        col_(ix) = col(i)
        ix = ix + ONE
      end do

    end do

    ! add the element to the new sparse pattern
    call add_element(nthis, ir, ic)
    ! copy the sparse pattern to this one
    this = nthis
    ! clean-up
    call delete(nthis)
    ! specify the error message which then
    ! is the new size per row
    call set_error(this, npr)

  end subroutine add_element_


  !> @param[in] sp1 the first sparse matrix
  !! @param[in] sp2 the second sparse matrix
  !! @param[inout] sp the union of `sp1` and `sp2`
  subroutine union_(sp1, sp2, sp)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: sp1, sp2
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: sp

    type(BUD_LIST_NAME) :: L1, L2, L
    integer(BUD_TYPE_VAR_PREC) :: ir, n, nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:)

    ! A union removes duplicate entries...
    ! This may be undesired.. :(
    call attach(sp1, nr=nr, nc=nc)

    ! Create a list with the pointers
    allocate(rptr(nr+1))
    rptr(1) = BUD_SM_PTR
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


  !> @param[in] from the originating sparse matrix
  !! @param[in] n number of elements in `remove`
  !! @param[in] remove columns that are removed
  !! @param[inout] to sparse matrix after deleting the columns
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

  !> @param[in] from the originating sparse matrix
  !! @param[in] remove list with columns that are removed
  !! @param[inout] to sparse matrix after deleting the columns
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


  !> @param[in] from the originating sparse matrix
  !! @param[in] nin number of elements in `in_col`
  !! @param[in] in_col the set of columns that will be translated into `out_col` (preferentially this should be sorted)
  !! @param[in] nout number of elements in `out_col`
  !! @param[in] out_col the set of translation columns
  !! @param[inout] to the resulting sparse matrix after translating the columns
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


  !> @param[in] from the originating sparse matrix
  !! @param[in] in_col a list set of columns that will be translated into `out_col` (preferentially this should be sorted)
  !! @param[in] out_col a list set of translation columns
  !! @param[inout] to the resulting sparse matrix after translating the columns
  subroutine translate_col_list_(from, in_col, out_col, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME) :: in_col, out_col
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lcol
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nz, ir, i, idx
    integer(BUD_TYPE_VAR_PREC), allocatable :: trptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: rptr(:), nrow(:), col(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: oc(:)

    call attach(from, nr=nr, nc=nc, nz=nz, rptr=rptr, col=col, nrow=nrow)

    ! Retrieve list
    oc => list_p(out_col)

    ! Create a list with the columns
    call new(lcol, nz)
    allocate(trptr(nr+1))

    ! Now start create the sparse matrix
    trptr(1) = rptr(1)
    do ir = 1, nr

      ! figure out if they should be translated
      do i = rptr(ir) BUD_SM_PTR_A, rptr(ir) + nrow(ir) BUD_SM_PTR_B
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
      trptr(ir+1) = size(lcol) + BUD_SM_PTR

    end do

    col => list_p(lcol)
    call new(to, nr, nc, size(lcol), trptr, col)

    call delete(lcol)

  end subroutine translate_col_list_


  !> @param[in] from the originating sparse matrix
  !! @param[in] n number of elements in `remove`
  !! @param[in] remove rows that are removed
  !! @param[inout] to sparse matrix after deleting the rows
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

  !> @param[in] from the originating sparse matrix
  !! @param[in] remove list with rows that are removed
  !! @param[inout] to sparse matrix after deleting the rows
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



  !> @param[in] from the originating sparse matrix
  !! @param[in] nin number of elements in `in_row`
  !! @param[in] in_row the set of rows that will be translated into `out_row` (preferentially this should be sorted)
  !! @param[in] nout number of elements in `out_row`
  !! @param[in] out_row the set of translation rows
  !! @param[inout] to the resulting sparse matrix after translating the rows
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

  !> @param[in] from the originating sparse matrix
  !! @param[in] in_row a list set of rows that will be translated into `out_row`
  !! @param[in] out_row a list set of translation rows (preferentially this should be sorted)
  !! @param[inout] to the resulting sparse matrix after translating the rows
  subroutine translate_row_list_(from, in_row, out_row, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME) :: in_row, out_row
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME), allocatable :: lrow(:)
    type(BUD_LIST_NAME) :: tmp
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nnr, r, ir, i, idx
    integer(BUD_TYPE_VAR_PREC), allocatable :: rptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: row(:), col(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: or(:)

    call attach(from, nr=nr, nc=nc)

    ! This algorithm moves in_row to out_row, and possibly deletes
    ! rows.
    allocate(lrow(nc))

    ! Initialize the sparse lists
    do ir = 1 , nr
      call new(lrow(ir), ONE)
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

      else if ( or(idx) >= BUD_SM_IDX(1) ) then

        ! Find where the new column has moved
        r = BUD_SM_IDXF(or(idx))
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
    rptr(1) = BUD_SM_PTR
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


  !> @param[inout] f `File` bud
  !! @param[in] this the sparse matrix bud
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: col(:), nrow(:)

    logical :: formatted
    integer :: iu

    integer(BUD_TYPE_VAR_PREC) :: nz
    integer(BUD_TYPE_VAR_PREC) :: ir
    integer(BUD_TYPE_VAR_PREC) :: nr, nc

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    ! First figure out if the file is an unformatted file
    formatted = is_formatted(f)
    iu = unit(f)

    call attach(this, nr=nr, nc=nc, nz=nz, nrow=nrow)

    ! First we write the size of the sparse matrix
    if ( formatted ) then
      write(iu, '(i16)') nr, nc, nz
      write(iu, '(l16)') this%D%sorted
      write(iu, '(i16)') nrow(:)
    else
      write(iu) nr, nc, nz
      write(iu) this%D%sorted
      write(iu) nrow(:)
    end if

    ! Write the sparse columns
    do ir = 1 , nr
      col => column_p(this, ir)
      if ( formatted ) then
        write(iu, '(i16)') col(:)
      else
        write(iu) col(:)
      end if
    end do

  end subroutine write_

  !> @param[inout] f `File` bud
  !! @param[inout] this the sparse matrix bud
  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), col(:), nrow(:)

    logical :: formatted
    integer :: iu

    logical :: sorted
    integer(BUD_TYPE_VAR_PREC) :: nz, i
    integer(BUD_TYPE_VAR_PREC) :: ir
    integer(BUD_TYPE_VAR_PREC) :: nr, nc

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    ! First figure out if the file is an unformatted file
    formatted = is_formatted(f)
    iu = unit(f)

    ! First we need to read the array dimensions...
    if ( formatted ) then
      read(iu, '(i16)') nr, nc, nz
      read(iu, '(l16)') sorted
    else
      read(iu) nr, nc, nz
      read(iu) sorted
    end if

    call new(this, nr, nc, nz)
    call attach(this, rptr=ptr, nrow=nrow)

    ! Read nrow
    if ( formatted ) then
      read(iu, '(i16)') nrow
    else
      read(iu) nrow
    end if

    ! Create the pointer array
    ptr(1) = BUD_SM_PTR
    do i = 2 , nr + 1
      ptr(i) = ptr(i-1) + nrow(i-1)
    end do

    ! Read the sparse columns
    do ir = 1 , nr
      col => column_p(this, ir)
      if ( formatted ) then
        read(iu, '(i16)') col(:)
      else
        read(iu) col(:)
      end if
    end do

    this%D%sorted = sorted
    this%D%finalized = .true.

  end subroutine read_


! Local pre-processor variables that
! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_LIST_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

  ! Control variables
#undef BUD_SM_CSR
#undef BUD_SM_INTEROP_C
#undef BUD_SM_MOD
#undef BUD_SM_IDX
#undef BUD_SM_IDXF

#include "bud_cleanup.inc"


! project-buds -- local file settings
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

