  ! @@LICENSE@@ see Copyright notice in the top-directory

  !> @defgroup sm-array Sparse matrix (pattern and data)
  !! @ingroup bud-intrinsic
  !!
  !! @bud containing a sparse matrix pattern _and_ the
  !! associated data corresponding to the sparse matrix elements.
  !!
  !! From this object the full matrix may be constructed or interacted
  !! with.
  !!

#include "bud_common_declarations.inc"

# define BUD_IO_OVERLOAD

  ! Ensure we have the sparse matrix
#include "SM.inc"

!# define BUD_INCLUDE_TYPE "SM_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "SM_Array_type_.inc"

  ! These fields are used in the sparse matrix stuff.
  integer(BUD_INT_PREC), parameter :: ONE = BUD_CC2(1_,BUD_INT_PREC)
  integer(BUD_INT_PREC), parameter :: ZERO = BUD_CC2(0_,BUD_INT_PREC)

  !> Retrieve pointer data in the sparse matrix
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface matrix_p
    module procedure matrix_p_
#if BUD_DIM == 1
    module procedure matrix_ip_
    module procedure matrix_lp_
#endif
  end interface
  public :: matrix_p


  !> Retrieve the sparse matrix
  interface sparse_matrix
    module procedure get_elem1_
  end interface
  public :: sparse_matrix

  !> Retrieve pointer to the sparse matrix
  interface sparse_matrix_p
    module procedure get_elem1p_
  end interface
  public :: sparse_matrix_p

  !> Query the index of the sparse matrix
  interface sparse_index
    module procedure sparse_index_
  end interface
  public :: sparse_index

  !> Query the size of the matrix (dimension)
  interface dimensions
    module procedure dimensions_
  end interface
  public :: dimensions

  !> Retrieve the array @bud
  interface array
    module procedure get_elem2_
  end interface
  public :: array

  !> Retrieve a pointer to the array @bud
  interface array_p
    module procedure get_elem2p_
  end interface
  public :: array_p

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

#include "bud_collection.inc"


  !> @param[in] this sparse matrix
  !! @return a pointer to the data (contiguous)
  function matrix_p_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    p => array_p(this%D%e2)

  end function matrix_p_

#if BUD_DIM == 1
  !> @param[in] this sparse matrix
#ifdef BUD_SM_CSC
  !! @param[in] i sparse row index
#else
  !! @param[in] i sparse column index
#endif
  !! @return a pointer to the data (contiguous)
  function matrix_ip_(this, i) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif
    integer(ii_), intent(in) :: i

    p => array_p(this%D%e2)
    p => p( this%D%e1%D%_CS_PTR(i)BUD_SM_PTR_A: &
      this%D%e1%D%_CS_PTR(i)+this%D%e1%D%_CS_NUM(i)BUD_SM_PTR_B)

  end function matrix_ip_

  !> @param[in] this sparse matrix
#ifdef BUD_SM_CSC
  !! @param[in] i sparse row index
#else
  !! @param[in] i sparse column index
#endif
  !! @return a pointer to the data (contiguous)
  function matrix_lp_(this, i) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif
    integer(il_), intent(in) :: i

    p => array_p(this%D%e2)
    p => p( this%D%e1%D%_CS_PTR(i)BUD_SM_PTR_A: &
      this%D%e1%D%_CS_PTR(i)+this%D%e1%D%_CS_NUM(i)BUD_SM_PTR_B)

  end function matrix_lp_

#endif


  !> @param[in] this sparse matrix
  !! @return the index of the sparse index
  function sparse_index_(this) result(idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: idx

#if BUD_DIM > 1
    idx = this%D%sm_idx
#else
    idx = 1
#endif

  end function sparse_index_

  !> @param[in] this @bud container
  !! @return the number of dimensions of contained array
  pure function dimensions_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: d

    if ( is_initd(this) ) then
      d = BUD_DIM
    else
      d = -1
    end if

  end function dimensions_


  !> @param[inout] this the sparse matrix to add an element to
  !! @param[in] ir the row index of the element
  !! @param[in] ic the column index of the element
  recursive subroutine add_element_(this, ir, ic, val)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_INT_PREC), intent(in) :: ir, ic
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: val
#else
    BUD_TYPE_VAR, intent(in) :: val
#endif

    type(BUD_COLL_1) :: sm
    type(BUD_TYPE_NAME) :: nthis
    integer(BUD_INT_PREC) :: i, c, ix, nr, nc, npc

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    if ( .not. is_initd(this) ) return

    sm = this

    ! We will only allow adding an element if there is no error
    call add_element(sm, ir, ic, dry_run = .true.)
    if ( error(sm) /= 0 ) then
      ! there is not room for adding an element
      ! without disrupting anything
      call delete(sm)
      call set_error(this, -1)

      return

    end if

    ! We now adding an element will not re-allocate
    ! the sparsity pattern
    call add_element(sm, ir, ic)
    i = index(sm, ir, ic)

    p => matrix_p(this)

    select case ( sparse_index(this) )
    case ( 1 )

#if BUD_DIM == 1
      p(i) = val
#elif BUD_DIM == 2
      p(i,:) = val
#elif BUD_DIM == 3
      p(i,:,:) = val
#endif

    case ( 2 )

#if BUD_DIM == 2
      p(:,i) = val
#elif BUD_DIM == 3
      p(:,i,:) = val
#endif

    case ( 3 )

#if BUD_DIM == 3
      p(:,:,i) = val
#endif

    end select

    ! clean-up
    call delete(sm)

  end subroutine add_element_


  !> @param[inout] f `File` bud
  !! @param[in] this the sparse matrix bud with data
  !! @param[in] only_array @opt whether only the sparse data should be written
  subroutine write_(f, this, only_array)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    logical, intent(in), optional :: only_array

    type(BUD_COLL_1) :: sm
    type(BUD_COLL_2) :: arr

    logical :: lonly_array

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    ! Write the sparse matrix
    lonly_array = .false.
    if ( present(only_array) ) lonly_array = only_array

    sm = this
    arr = this
    if ( .not. is_finalized(sm) ) then
      call delete(sm)
      call delete(arr)
      call set_error(this, -1)
      return
    end if

    if ( .not. lonly_array ) then
      call write(f, sm)
    end if
    call write(f, arr)

    call delete(sm)
    call delete(arr)

  end subroutine write_

  !> @param[inout] f `File` bud
  !! @param[inout] this the sparse matrix bud
  !! @param[inout] sm @opt the sparse pattern, if this is passed there will be no reading of the sparse matrix
  subroutine read_(f, this, sm)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_COLL_1), intent(inout), optional :: sm

    type(BUD_COLL_1) :: lsm
    type(BUD_COLL_2) :: arr

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    if ( present(sm) ) then
      lsm = sm
    else
      ! Read the sparse pattern
      call read(f, lsm)
    end if

    call read(f, arr)

    call new(this, lsm, arr)

    call delete(lsm)

  end subroutine read_


  ! Associated with the Array
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

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


