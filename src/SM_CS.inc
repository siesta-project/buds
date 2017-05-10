  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This module may be controlled via these
  ! preprocessor variables:
  !  - BUD_MOD_NAME name of the module
  !  - BUD_TYPE_NAME name of the public type
  !  - BUD_TYPE_NAME_ name of the internal data pointer
  !  - BUD_TYPE_NAME_STR the routine name in "string" format (for IO)
  !
  ! Additionally the CS[CR] pattern format may be
  ! controlled via these flags:
  !  - BUD_SM_CS[CR]
  !    case ( 0 ) ! default
  !      standard CS[CR] format (MKL-SpBLAS)
  !    case ( 1 )
  !      zero based pointer CS[CR] format
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
  !    Cannot be used together with BUD_SM_CS[CR] == 1

  ! Retrieve the settings for the sparse matrix
# include "SM.inc"

#if BUD_SM_CS == 0
#elif BUD_SM_CS == 1
# if BUD_SM_INTEROP_C == 1
#  error "SM_INTEROP_C may not be defined when using SM_CS == 1"
# endif
#else
# error "SM_CS *MUST* be either 0 or 1"
#endif


#include "bud_utils.inc"

  use BUD_CC3(BUD_MOD,_,SM_common)

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Sparse matrix type in the CSC format
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

    procedure, private :: indices_p_
    procedure, private :: indices_cp_

    procedure, private :: remove_row_el_
    procedure, private :: remove_row_list_

    procedure, private :: remove_column_el_
    procedure, private :: remove_column_list_

    procedure, private :: translate_row_el_
    procedure, private :: translate_row_list_

    procedure, private :: translate_column_el_
    procedure, private :: translate_column_list_

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

#ifdef BUD_SM_CSR
    !> @iSee #nzrow_p
    procedure, public :: nzrow_p => nentries_p_

    !> @iSee #row_p
    generic, public :: row_p => indices_p_, indices_cp_
#else
    !> @iSee #nzcolumn_p
    procedure, public :: nzcolumn_p => nentries_p_

    !> @iSee #column_p
    generic, public :: column_p => indices_p_, indices_cp_
#endif

    !> @iSee #add_element
    procedure, public :: add_element => add_element_


    !> @iSee #remove_row
    generic, public :: remove_row => remove_row_el_, &
      remove_row_list_

    !> @iSee #remove_column
    generic, public :: remove_column => remove_column_el_, &
      remove_column_list_

    !> @iSee #translate_row
    generic, public :: translate_row => translate_row_el_, &
      translate_row_list_

    !> @iSee #translate_column
    generic, public :: translate_column => translate_column_el_, &
      translate_column_list_


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
  !! Contains the sparsity pattern for a CSC pattern.
  type BUD_TYPE_NAME_

    !> Number of rows in the pattern
    integer(BUD_TYPE_VAR_PREC) :: nr = ZERO
    !> Number of columns in the pattern
    integer(BUD_TYPE_VAR_PREC) :: nc = ZERO
    !> Number of non-zero elements in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nz = ZERO
    !> Total number of elements possible in the sparse matrix
    integer(BUD_TYPE_VAR_PREC) :: nt = ZERO

    !> Whether the sparse matrix has been sorted, see #sort
    logical :: sorted = .false.

    !> Whether the sparse matrix has been finalized, see #finalize
    logical :: finalized = .false.

    !> Pointer to the index of the equivalent column/row (size `_CS_PTR_S+1`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: _CS_PTR(:)

    !> Number of non-zero entries per _CS_PTR_S
    integer(BUD_TYPE_VAR_PREC), allocatable :: _CS_NUM(:)

    !> The index of the equivalent sparse matrix (size `nt`)
    integer(BUD_TYPE_VAR_PREC), allocatable :: _CS_IDX(:)

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


  !> Retrieve a pointer to the column offsets/pointers
  !!
  !! A pointer with the column offsets.
  !!
#ifdef BUD_SM_CSR
# if BUD_SM_INTEROP_C == 1
  !! `M(ir, column(cptr(ir)+1))` is the first sparse
  !! element in row `ir`.
# else
  !! `M(ir, column(cptr(ir)))` is the first sparse
  !! element in row `ir`.
# endif
#else
# if BUD_SM_INTEROP_C == 1
  !! `M(row(rptr(ic)+1),ic)` is the first sparse
  !! element in column `ic`.
# else
  !! `M(row(rptr(ic)),ic)` is the first sparse
  !! element in column `ic`.
# endif
#endif
  interface offset_p
    module procedure offset_p_
  end interface
  public :: offset_p

#ifdef BUD_SM_CSR
  !> Retrieve a pointer to the number of entries per column
  interface nzrow_p
    module procedure nentries_p_
  end interface
  public :: nzrow_p

  !> Retrieve a pointer to the row indices in the sparse matrix
  interface row_p
    module procedure indices_p_
    module procedure indices_cp_
  end interface
  public :: row_p

#else
  !> Retrieve a pointer to the number of entries per row
  interface nzcolumn_p
    module procedure nentries_p_
  end interface
  public :: nzcolumn_p

  !> Retrieve a pointer to the column indices in the sparse matrix
  interface column_p
    module procedure indices_p_
    module procedure indices_cp_
  end interface
  public :: column_p

#endif

  !> Retrieve a pointer to the indices in the sparse matrix
  interface indices_p
    module procedure indices_p_
    module procedure indices_cp_
  end interface
  public :: indices_p

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
    module procedure remove_column_el_
    module procedure remove_column_list_
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
    module procedure translate_column_el_
    module procedure translate_column_list_
  end interface
  public :: translate_column


  !> Sorts rows in the sparse matrix
  !!
  !! Sorts the sparse matrix such that the row
  !! index is always increasing for each column.
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

    call set_error(this, 0)

    this%D%nr = 0
    this%D%nc = 0
    this%D%nz = 0
    this%D%nt = 0
    this%D%sorted = .false.
    this%D%finalized = .false.

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. allocated(this%D% _CS_IDX) ) return
    stat = 0
    deallocate(this%D% _CS_PTR, stat=istat)
    if ( 0 /= istat ) stat = istat
    deallocate(this%D% _CS_IDX, stat=stat)
    if ( 0 /= istat ) stat = istat
    deallocate(this%D% _CS_NUM, stat=stat)
    if ( 0 /= istat ) stat = istat

    call set_error(this, stat)

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
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: fptr(:), nent(:), fidx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: tptr(:)
    integer(BUD_TYPE_VAR_PREC) :: i, j, nc, nr

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
      call attach(from, nr=nr, nc=nc, ptr=fptr, nentries=nent, indices=fidx)
      call attach(to, ptr=tptr)

      do i = 1 , _CS_N

        if ( nent(i) > tptr(i+ONE) - tptr(i) ) then
          call set_error(to, i)
          exit
        end if

      end do

      ! quick return if there is not room
      ! The error message will return the column
      ! where there is not enough room
      if ( error(to) /= 0 ) return

      ! Now reset all elements in `to`
      call attach(to, nentries=nent)

      ! reset everything
      nent(:) = 0

      ! Loop all elements in from and copy them to
      to%D%sorted = .true.
      do i = 1, _CS_N

        fidx => indices_p(from, i)

        do j = 1, size(fidx)
#ifdef BUD_SM_CSR
          call add_element(to, fidx(j), i)
#else
          call add_element(to, i, fidx(j))
#endif
        end do

      end do

    else

      ! We make a copy by creating it
      call new(to, from%D%nr, from%D%nc, from%D%nz, &
        from%D% _CS_PTR, from%D% _CS_IDX, from%D% _CS_NUM)
      to%D%sorted = from%D%sorted

    end if

  end subroutine copy_


  !> @param[inout] this the sparse matrix
  !! @param[in] nr number of rows of the pattern
  !! @param[in] nc number of columns of the pattern
  !! @param[in] nt @opt number of possible non-zero elements
  !! @param[in] np @opt number of non-zero elements per _CS_PTR_N
  subroutine new_dim_(this, nr, nc, nt, np)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc
    integer(BUD_TYPE_VAR_PREC), intent(in), optional :: nt, np
    integer(BUD_TYPE_VAR_PREC) :: i

    call initialize(this)

    this%D%nr = nr
    this%D%nc = nc
    if ( present(nt) ) then
      this%D%nt = nt
    else if ( present(np) ) then
      this%D%nt = np * _CS_N
    else
      this%D%nt = _CS_N * BUD_CC2(10_,BUD_TYPE_VAR_PREC)
    end if
    this%D%nz = 0
    this%D%sorted = .false.
    this%D%finalized = .false.

    allocate(this%D% _CS_PTR(_CS_N+1))
    allocate(this%D% _CS_NUM(_CS_N))
    allocate(this%D% _CS_IDX(this%D%nt))

    ! Initialize sparse matrix
    if ( present(np) ) then
      this%D% _CS_PTR(1) = BUD_SM_PTR
      do i = 1 , _CS_N
        this%D% _CS_PTR(i) = this%D% _CS_PTR(i-ONE) + np
      end do
      this%D% _CS_PTR(_CS_N+ONE) = this%D% _CS_PTR(_CS_N) + BUD_SM_PTR
    else
      this%D% _CS_PTR(1) = BUD_SM_PTR
    end if
    do i = 1 , _CS_N
      this%D% _CS_NUM(i) = ZERO
    end do

  end subroutine new_dim_


  !> @param[inout] this the new sparse matrix
  !! @param[in] nr number of rows of the pattern
  !! @param[in] nc number of columns of the pattern
  !! @param[in] nz number of non-zero elements of the pattern
  !! @param[in] cptr column pointers (at least size `nc`)
  !! @param[in] row row-indices for the sparse elements
  !! @param[in] ncol @opt number of elements per column
  subroutine new_copy_(this, nr, nc, nz, _CS_PTR, _CS_IDX, _CS_NUM)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    ! sizes
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nr, nc, nz
    ! limiting ptr
    integer(BUD_TYPE_VAR_PREC), intent(in) :: _CS_PTR(_CS_N), _CS_IDX(nz)
    integer(BUD_TYPE_VAR_PREC), intent(in), optional :: _CS_NUM(_CS_N)

    integer(BUD_TYPE_VAR_PREC) :: i

    ! Copy over information
    ! It must be based on the starting index
    if ( _CS_PTR(1) /= BUD_SM_PTR ) then
      call delete(this)
      return
    end if

    ! pre-allocate
    call new(this, nr, nc, nz)
    this%D%nz = nz

    ! Copy pointers
    do i = 1 , _CS_N
      this%D% _CS_PTR(i) = _CS_PTR(i)
    end do

    ! create last pointer (to one plus number of elements)
    ! This ensures simple loops without taking care of
    ! the last index
    this%D% _CS_PTR(_CS_N+1) = this%D%nz + BUD_SM_PTR

    if ( present(_CS_NUM) ) then
      do i = 1 , _CS_N
        this%D% _CS_NUM(i) = _CS_NUM(i)
      end do
    else
      do i = 1 , _CS_N
        this%D% _CS_NUM(i) = this%D% _CS_PTR(i+1) - this%D% _CS_PTR(i)
      end do
    end if

    do i = 1 , nz
      this%D% _CS_IDX(i) = _CS_IDX(i)
    end do

  end subroutine new_copy_


  !> @param[in] this sparse matrix
  !! @return a pointer to the column offsets for the sparse matrix (contiguous)
  function offset_p_(this) result(ptr)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:)

    ptr => this%D% _CS_PTR

  end function offset_p_


  !> @param[in] this sparse matrix
  !! @return a pointer to the row indices for the sparse matrix (contiguous)
  function indices_p_(this) result(idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx(:)

    idx => this%D% _CS_IDX

  end function indices_p_

  !> @param[in] this sparse matrix
  !> @param[in] i only retrieve the rows that reside in column `c`
  !! @return a pointer to the row indices for the sparse matrix (contiguous)
  function indices_cp_(this,i) result(idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: i
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx(:)

    idx => this%D% _CS_IDX(this%D% _CS_PTR(i)BUD_SM_PTR_A:this%D% _CS_PTR(i)+this%D% _CS_NUM(i)BUD_SM_PTR_B)

  end function indices_cp_

  !> @param[in] this sparse matrix
  !! @return a pointer to the array that holds the number of entries per column
  function nentries_p_(this) result(nent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: nent(:)

    nent => this%D% _CS_NUM

  end function nentries_p_

  !> @param[in] this sparse matrix @bud
  !! @param[out] nr @opt number of rows in SM
  !! @param[out] nc @opt number of columns in SM
  !! @param[out] nz @opt number of non-zero elements in SM
  !! @param[out] nt @opt total number of possible non-zero elements in SM
  !! @param[out] _CS_PTR @opt _CS_PTR_N pointer (`_CS_PTR(2)BUD_SM_PTR_A` is starting index of `2`)
  !! @param[out] _CS_IDX @opt _CS_IDX_N index
  !! @param[out] _CS_NUM @opt number of non-zero elements per _CS_NUM_N
  subroutine attach_(this, D, nr, nc, nz, nt, _CS_PTR, _CS_IDX, _CS_NUM, &
    ptr, nentries, indices)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    ! Having this forces the user to explicitly specify the
    ! wanted information.
    logical, intent(in), optional :: D
    integer(BUD_TYPE_VAR_PREC), intent(out), optional :: nr, nc, nz, nt
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: _CS_PTR(:)
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: _CS_IDX(:)
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: _CS_NUM(:)
    ! Common names
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: ptr(:)
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: nentries(:)
    integer(BUD_TYPE_VAR_PREC), intent(out), pointer BUD_FORTRAN_CONTIGUOUS, optional :: indices(:)

    if ( is_initd(this) ) then
      if ( present(nr) ) nr = this%D%nr
      if ( present(nc) ) nc = this%D%nc
      if ( present(nz) ) nz = this%D%nz
      if ( present(nt) ) nt = this%D%nt
      if ( present(ptr) ) ptr => this%D% _CS_PTR
      if ( present(_CS_PTR) ) _CS_PTR => this%D% _CS_PTR
      if ( present(indices) ) indices => this%D% _CS_IDX
      if ( present(_CS_IDX) ) _CS_IDX => this%D% _CS_IDX
      if ( present(nentries) ) nentries => this%D% _CS_NUM
      if ( present(_CS_NUM) ) _CS_NUM => this%D% _CS_NUM

    else
      if ( present(nr) ) nr = ZERO
      if ( present(nc) ) nc = ZERO
      if ( present(nz) ) nz = ZERO
      if ( present(nt) ) nt = ZERO

      if ( present(ptr) ) nullify(ptr)
      if ( present(_CS_PTR) ) nullify(_CS_PTR)
      if ( present(indices) ) nullify(indices)
      if ( present(_CS_IDX) ) nullify(_CS_IDX)
      if ( present(nentries) ) nullify(nentries)
      if ( present(_CS_NUM) ) nullify(_CS_NUM)
    end if

  end subroutine attach_


    !> @param[in] this the sparse matrix (sorted, @isee #sp_sort
#if BUD_SM_INTEROP_C == 1
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

      do idx = this%D% _CS_PTR(_CS_I) BUD_SM_PTR_A , this%D% _CS_PTR(_CS_I) + this%D% _CS_NUM(_CS_I) BUD_SM_PTR_B
        if ( this%D% _CS_IDX(idx) == _CS_J ) return
      end do

      idx = -1

      return

    end if

    call find_bin(this%D% _CS_NUM(_CS_I), &
      this%D% _CS_IDX(this%D% _CS_PTR(_CS_I) BUD_SM_PTR_A:), _CS_J, idx)
    if ( idx > 0 ) idx = this%D% _CS_PTR(_CS_I) BUD_SM_PTR_B + idx

  end function index_


  !> @param[inout] this sparse matrix to sort (in-place)
  !! @param[out] pvt @opt=@none if requested the pivoting array for the sorted sparsity pattern
  subroutine sort_(this, pvt)
    ! We use the quick-sort algorithm in this general finalization
    ! algorithm.
    use BUD_CC2(BUD_MOD,_utils), only: sort_quick

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(out), target, optional :: pvt(:)

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: i, j, nr, nc, nz, iptr
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nent(:), idx(:), tvt(:)
    integer(BUD_TYPE_VAR_PREC), allocatable :: temp(:)

    ! Initialize error
    call set_error(this, 0)

    ! Get sparse matrix array
    call attach(this, nr=nr, nc=nc, nz=nz, &
      ptr=ptr, indices=idx, nentries=nent)

    if ( present(pvt) ) then

      if ( size(pvt) < nz ) then
        call set_error(this, SM_INPUT + 3)
        pvt = -1
        return
      end if
      allocate(temp(maxval(nent)))

      do i = 1 , _CS_N

        nz = nent(i)
        iptr = ptr(i) - BUD_SM_PTR

        ! get pivoting array
        tvt => pvt(iptr+1:)

        ! We have to use a temporary array to ensure
        ! no overwriting
        do j = 1 , nz
          temp(j) = idx(iptr+j)
        end do
        call sort_quick(nz, idx(iptr+1:), tvt(:))
        do j = 1 , nz
          idx(iptr+j) = temp(tvt(j))
          tvt(j) = iptr + tvt(j)
        end do

      end do

    else

      do i = 1 , _CS_N

        idx => indices_p(this, i)
        nz = size(idx)

        call sort_quick(nz, idx(:))

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

    integer(BUD_TYPE_VAR_PREC) :: i, nc, nr

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: aidx(:), aent(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: bidx(:), bent(:)

    is = .true.
    if ( same(a, b) ) return

    nr = rows(a)
    nc = columns(a)
    is = nr == rows(b) .and. &
      nc == columns(b)
    if ( .not. is ) return

    ! get information
    call attach(a, nentries=aent)
    call attach(b, nentries=bent)

    do i = 1, _CS_N

      ! Check number of elements per column/row
      is = aent(i) == bent(i)
      if ( .not. is ) return

      ! check that there are elements
      if ( aent(i) == 0 ) cycle

      aidx => indices_p(a, i)
      bidx => indices_p(b, i)

      is = all(aidx == bidx)
      if ( .not. is ) return

    end do

  end function equivalent_


  !> @param[inout] this sparse matrix to finalize (in-place)
  subroutine finalize_(this)
    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    ! Local variables
    integer(BUD_TYPE_VAR_PREC) :: i, nr, nc, nz
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), idx(:)

    type(BUD_LIST_NAME) :: lptr, lidx
    integer(BUD_TYPE_VAR_PREC) :: p
    logical :: sorted

    ! Initialize error
    call set_error(this, 0)

    ! Get sparse matrix array
    call attach(this, nr=nr, nc=nc, nz=nz, indices=idx)

    ! initialize lists
    call new(lptr, _CS_N)
    call new(lidx, nz)

    ! Initialize the pointer
    p = BUD_SM_PTR
    call push(lptr, p)

    do i = 1 , _CS_N

      idx => indices_p(this, i)
      nz = size(idx)

      ! Push the new pointer index
      p = p + nz
      call push(lptr, p)

      ! Push the indices to the list
      call push(lidx, nz, idx)

    end do

    ! Determine whether we should sort it after-wards
    ! I.e. maintain state of object
    sorted = is_sorted(this)

    p = size(lidx)
    ! Get pointers
    ptr => list_p(lptr)
    idx => list_p(lidx)
    call new(this, nr, nc, p, ptr, idx)
    call delete(lptr)
    call delete(lidx)

    if ( sorted ) then
      call sort(this)
    end if
    this%D%finalized = .true.

  end subroutine finalize_


  !> @param[inout] this the sparse matrix to set a non-zero element in
  !! @param[in] ir the row index of the element
  !! @param[in] ic the column index of the element
  !! @param[in] dry_run @opt=.false., if `.true.` will do nothing but issue error messages as though it had runned.
  recursive subroutine add_element_(this, ir, ic, dry_run)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir, ic
    logical, intent(in), optional :: dry_run

    logical :: ldry

    type(BUD_TYPE_NAME) :: nthis
    integer(BUD_TYPE_VAR_PREC) :: i, j, k, ix, nr, nc, np

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nent(:), idx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr_(:), nent_(:), idx_(:)

    ldry = .false.
    if ( present(dry_run) ) ldry = dry_run

    call attach(this, nr=nr, nc=nc, &
      ptr=ptr, nentries=nent, indices=idx)

    ! check if there is place in the sparse matrix
    ! for an entry without re-allocation
    if ( ptr( _CS_I +ONE) - ptr(_CS_I) > nent(_CS_I) ) then

      ix = ptr(_CS_I) BUD_SM_PTR_A + nent(_CS_I)
      ! Current column/row, to check if we are still in a sorted
      ! sparse matrix
      if ( nent(_CS_I) == 0 ) then
        ! lowest value
        k = -1
      else
        ! Currently highest row/column
        k = idx(ix - ONE)
      end if
      if ( .not. ldry ) then
        ! insert the value
        idx(ix) = _CS_J
        nent(_CS_I) = nent(_CS_I) + ONE
        this%D%nz = this%D%nz + ONE

        if ( this%D%sorted .and. _CS_J < k ) then

          ! denote it non-sorted
          this%D%sorted = .false.

        end if
      end if

      call set_error(this, 0)

      ! Quick-return
      ! We have inserted the value and can carry on without
      ! re-allocation.
      return

    end if

    ! Re-allocate the sparse matrix and copy it
    ! We add 8 elements per row.
    ! Perhaps this should be defined by the user.
    np = maxval(nent) + BUD_CC2(8_,BUD_TYPE_VAR_PREC)
    ! Quick return if user request dry-run
    if ( ldry ) then
      call set_error(this, np)
      return
    end if

    call new(nthis, nr, nc, np=np)
    call attach(nthis, ptr=ptr_, nentries=nent_, indices=idx_)

    ! Copy all elements correctly
    ix = 1
    do j = 1 , _CS_N
      nent_(j) = nent(j)

      ix = ptr_(j) BUD_SM_PTR_A
      ! copy this column
      do i = ptr(j) BUD_SM_PTR_A , ptr(j) + nent(j) BUD_SM_PTR_B
        idx_(ix) = idx(i)
        ix = ix + ONE
      end do

    end do

    ! add the element to the new sparse pattern
    call add_element(nthis, ir, ic)
    ! copy the sparse pattern to this one
    this = nthis
    ! clean-up
    call delete(nthis)
    call set_error(this, 0)

  end subroutine add_element_


  !> @param[in] sp1 the first sparse matrix
  !> @param[in] sp2 the second sparse matrix
  !> @param[inout] sp the union of `sp1` and `sp2`
  subroutine union_(sp1, sp2, sp)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: sp1, sp2
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: sp

    type(BUD_LIST_NAME) :: L1, L2, L
    integer(BUD_TYPE_VAR_PREC) :: i, nr, nc, nz, nz1, nz2
    integer(BUD_TYPE_VAR_PREC), allocatable :: ptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx1(:), idx2(:)

    ! A union removes duplicate entries...
    ! This may be undesired.. :(
    call attach(sp1, nr=nr, nc=nc)

    ! Create a list with the pointers
    allocate(ptr(_CS_N+1))
    ptr(1) = BUD_SM_PTR
    do i = 1 , _CS_N

      ! Create union...
      idx1 => indices_p(sp1, i)
      nz1 = size(idx1)
      call new(L1, nz1, idx1)
      idx2 => indices_p(sp2, i)
      nz2 = size(idx2)
      call new(L2, nz2, idx2)

      ! Sort both lists (makes union faster)
      call sort(L1)
      call sort(L2)

      ! Create union...
      call union(L1, L2, L2)
      ! We also ensure it is sorted.
      call sort(L2)

      ! push the rows to the new list
      call push(L, L2)

      ptr(i+1) = ptr(i) + size(L2)

    end do

    idx1 => list_p(L)
    call new(sp, nr, nc, nz, ptr, idx1)

    call delete(L1)
    call delete(L2)
    call delete(L)

  end subroutine union_


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
  !! @param[in] in_row a list set of rows that will be translated into `out_row` (preferentially this should be sorted)
  !! @param[in] out_row a list set of translation rows
  !! @param[inout] to the resulting sparse matrix after translating the rows
#ifdef BUD_SM_CSC
  subroutine translate_row_list_(from, in_row, out_row, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: in_row, out_row
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lrow
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nz, ic, i, j, ix
    integer(BUD_TYPE_VAR_PREC), allocatable :: tptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nent(:), idx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: or(:)

    call attach(from, nr=nr, nc=nc, nz=nz, ptr=ptr, indices=idx, nentries=nent)

    ! Retrieve list
    or => list_p(out_row)

    ! Create a list with the column/rows
    call new(lrow, nz)

    allocate(tptr(nc+1))

    ! Now start create the sparse matrix
    tptr(1) = ptr(1)
    do i = 1, nc

      ! figure out if they should be translated
      do j = ptr(i) BUD_SM_PTR_A, ptr(i) + nent(i) BUD_SM_PTR_B
        ix = index(in_row, idx(j))
        if ( ix <= 0 ) then
          call push(lrow, idx(j))
        else if ( or(ix) >= BUD_SM_IDX(1) ) then
          ! we have a translation
          ! check if the resulting translated row is
          ! negative, if so, it means deletion

          ! we retain the item with a new index
          call push(lrow, or(ix))
        end if
      end do

      ! update the following pointer
      tptr(i+1) = size(lrow) + BUD_SM_PTR

    end do

    ! Get the list of sparse-elements
    idx => list_p(lrow)

    ! Note that even though we remove some rows
    ! we do not shrink the matrix size...
    call new(to, nr, nc, size(lrow), tptr, idx)

    call delete(lrow)

  end subroutine translate_row_list_

#else
  subroutine translate_row_list_(from, in_row, out_row, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: in_row, out_row
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME), allocatable :: l(:)
    type(BUD_LIST_NAME) :: tmp
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, n, i, j, ix
    integer(BUD_TYPE_VAR_PREC), allocatable :: ptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: o(:)

    call attach(from, nr=nr, nc=nc)

    ! Initialize the sparse lists
    allocate(l(nr))
    do i = 1 , nr
      call new(l(i), ONE)
    end do

    ! List of out
    o => list_p(out_row)

    ! Now actually populate the sparse rows
    do i = 1 , nr

      ! Get column in current row
      idx => indices_p(from, i)
      n = int(size(idx), BUD_TYPE_VAR_PREC)

      ! Figure out if this is in in_row (i.e. should it be translated)
      ix = index(in_row, i)
      if ( ix < 1 ) then

        ! The row is not translated, so put it in the same row
        call push(l(i), n, idx)

      else if ( o(ix) >= BUD_SM_IDX(1) ) then

        ! Find where the new row has moved
        j = BUD_SM_IDXF(o(ix))
        call push(l(j), i, idx)

      end if

    end do

    ! Now re-create the pointers
    allocate(ptr(nr+1))
    ptr(1) = BUD_SM_PTR

    do i = 1 , nr

      ! Create full list of rows
      call push(tmp, l(i))

      ! Update Create the pointer
      ptr(i+1) = ptr(i) + size(l(i))

      call delete(l(i))

    end do

    ! Now we have, cptr, and row
    idx => list_p(tmp)
    ! finally we also know that all elements of lcol
    ! has been deleted

    call new(to, nr, nc, size(tmp), ptr, idx)

    deallocate(ptr)
    call delete(tmp)
    deallocate(l)

  end subroutine translate_row_list_
#endif


  !> @param[in] from the originating sparse matrix
  !! @param[in] n number of elements in `remove`
  !! @param[in] remove columns that are removed
  !! @param[inout] to sparse matrix after deleting the columns
  subroutine remove_column_el_(from, n, remove, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: n, remove(n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lrem

    ! Create a new list
    call new(lrem, n, remove)
    call sort(lrem)

    call remove_column_list_(from, lrem, to)

    call delete(lrem)

  end subroutine remove_column_el_

  !> @param[in] from the originating sparse matrix
  !! @param[in] remove list with columns that are removed
  !! @param[inout] to sparse matrix after deleting the columns
  subroutine remove_column_list_(from, remove, to)

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

    call translate_column_list_(from, remove, ll, to)

    call delete(ll)

  end subroutine remove_column_list_



  !> @param[in] from the originating sparse matrix
  !! @param[in] nin number of elements in `in_col`
  !! @param[in] in_col the set of columns that will be translated into `out_col` (preferentially this should be sorted)
  !! @param[in] nout number of elements in `out_col`
  !! @param[in] out_col the set of translation columns
  !! @param[inout] to the resulting sparse matrix after translating the rows
  subroutine translate_column_el_(from, nin, in_col, nout, out_col, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    integer(BUD_TYPE_VAR_PREC), intent(in) :: nin, in_col(nin), nout, out_col(nout)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lin, lout

    call new(lin, nin, in_col)
    call new(lout, nout, out_col)

    call translate_column_list_(from, lin, lout, to)

    call delete(lin)
    call delete(lout)

  end subroutine translate_column_el_


  !> @param[in] from the originating sparse matrix
  !! @param[in] in_column a list set of columns that will be translated into `out_column` (preferentially this should be sorted)
  !! @param[in] out_column a list set of translation columns
  !! @param[inout] to the resulting sparse matrix after translating the columns
#ifdef BUD_SM_CSR
  subroutine translate_column_list_(from, in_column, out_column, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: in_column, out_column
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME) :: lcol
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nz, i, j, ix
    integer(BUD_TYPE_VAR_PREC), allocatable :: tptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nent(:), idx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: oc(:)

    call attach(from, nr=nr, nc=nc, nz=nz, ptr=ptr, indices=idx, nentries=nent)

    ! Retrieve list
    oc => list_p(out_column)

    ! Create a list with the column/rows
    call new(lcol, nz)

    allocate(tptr(nr+1))

    ! Now start create the sparse matrix
    tptr(1) = ptr(1)
    do i = 1, nr

      ! figure out if they should be translated
      do j = ptr(i) BUD_SM_PTR_A, ptr(i) + nent(i) BUD_SM_PTR_B
        ix = index(in_column, idx(j))
        if ( ix <= 0 ) then
          call push(lcol, idx(j))
        else if ( oc(ix) >= BUD_SM_IDX(1) ) then
          ! we have a translation
          ! check if the resulting translated row is
          ! negative, if so, it means deletion

          ! we retain the item with a new index
          call push(lcol, oc(ix))
        end if
      end do

      ! update the following pointer
      tptr(i+1) = size(lcol) + BUD_SM_PTR

    end do

    ! Get the list of sparse-elements
    idx => list_p(lcol)

    ! Note that even though we remove some rows
    ! we do not shrink the matrix size...
    call new(to, nr, nc, size(lcol), tptr, idx)

    call delete(lcol)

  end subroutine translate_column_list_
#else
  subroutine translate_column_list_(from, in_column, out_column, to)

    use BUD_CC3(BUD_MOD,_,BUD_LIST_NAME)

    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_LIST_NAME), intent(in) :: in_column, out_column
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    type(BUD_LIST_NAME), allocatable :: l(:)
    type(BUD_LIST_NAME) :: tmp
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, n, i, j, ix
    integer(BUD_TYPE_VAR_PREC), allocatable :: ptr(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx(:)
    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: o(:)

    call attach(from, nr=nr, nc=nc)

    ! Initialize the sparse lists
    allocate(l(nc))
    do i = 1 , nc
      call new(l(i), ONE)
    end do

    ! List of out
    o => list_p(out_column)

    ! Now actually populate the sparse columns
    do i = 1 , nc

      ! Get columns in current row
      idx => indices_p(from, i)
      n = int(size(idx), BUD_TYPE_VAR_PREC)

      ! Figure out if this is in in_column (i.e. should it be translated)
      ix = index(in_column, i)
      if ( ix < 1 ) then

        ! The row is not translated, so put it in the same column
        call push(l(i), n, idx)

      else if ( o(ix) >= BUD_SM_IDX(1) ) then

        ! Find where the new column has moved
        j = BUD_SM_IDXF(o(ix))
        call push(l(j), i, idx)

      end if

    end do

    ! Now re-create the pointers
    allocate(ptr(nc+1))
    ptr(1) = BUD_SM_PTR

    do i = 1 , nc

      ! Create full list of rows
      call push(tmp, l(i))

      ! Update Create the pointer
      ptr(i+1) = ptr(i) + size(l(i))

      call delete(l(i))

    end do

    ! Now we have, cptr, and row
    idx => list_p(tmp)
    ! finally we also know that all elements of lcol
    ! has been deleted

    call new(to, nr, nc, size(tmp), ptr, idx)

    deallocate(ptr)
    call delete(tmp)
    deallocate(l)

  end subroutine translate_column_list_
#endif


  !> @param[inout] f `File` bud
  !! @param[in] this the sparse matrix bud
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: idx(:), nent(:)

    logical :: formatted
    integer :: iu

    integer(BUD_TYPE_VAR_PREC) :: nz
    integer(BUD_TYPE_VAR_PREC) :: i
    integer(BUD_TYPE_VAR_PREC) :: nr, nc

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    ! First figure out if the file is an unformatted file
    formatted = is_formatted(f)
    iu = unit(f)

    call attach(this, nr=nr, nc=nc, nz=nz, nentries=nent, indices=idx)

    ! First we write the size of the sparse matrix
    if ( formatted ) then
      write(iu, '(i16)') nr, nc, nz
      write(iu, '(l16)') this%D%sorted
      write(iu, '(i16)') nent(:)
    else
      write(iu) nr, nc, nz
      write(iu) this%D%sorted
      write(iu) nent(:)
    end if

    ! Write the sparse indices
    do i = 1 , _CS_N
      idx => indices_p(this, i)
      if ( formatted ) then
        write(iu, '(i16)') idx(:)
      else
        write(iu) idx(:)
      end if
    end do

  end subroutine write_

  !> @param[inout] f `File` bud
  !! @param[inout] this the sparse matrix bud
  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), idx(:), nent(:)

    logical :: formatted
    integer :: iu

    logical :: sorted
    integer(BUD_TYPE_VAR_PREC) :: i
    integer(BUD_TYPE_VAR_PREC) :: nr, nc, nz

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
    call attach(this, ptr=ptr, nentries=nent)

    ! Read ncol
    if ( formatted ) then
      read(iu, '(i16)') nent
    else
      read(iu) nent
    end if

    ! Create the pointer array
    ptr(1) = BUD_SM_PTR
    do i = 2 , _CS_N + 1
      ptr(i) = ptr(i-1) + nent(i-1)
    end do

    ! Read the sparse rows
    do i = 1 , _CS_N
      idx => indices_p(this, i)
      if ( formatted ) then
        read(iu, '(i16)') idx
      else
        read(iu) idx
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

