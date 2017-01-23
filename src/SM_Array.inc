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

#ifdef NOT_IMPLEMENTED_YET
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
#endif

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
#ifdef BUD_SM_CSR
    p => p(this%D%e1%D%rptr(i)BUD_SM_PTR_A:this%D%e1%D%rptr(i)+this%D%e1%D%nrow(i)BUD_SM_PTR_B)
#else
    p => p(this%D%e1%D%cptr(i)BUD_SM_PTR_A:this%D%e1%D%cptr(i)+this%D%e1%D%ncol(i)BUD_SM_PTR_B)
#endif

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
#ifdef BUD_SM_CSR
    p => p(this%D%e1%D%rptr(i)BUD_SM_PTR_A:this%D%e1%D%rptr(i)+this%D%e1%D%nrow(i)BUD_SM_PTR_B)
#else
    p => p(this%D%e1%D%cptr(i)BUD_SM_PTR_A:this%D%e1%D%cptr(i)+this%D%e1%D%ncol(i)BUD_SM_PTR_B)
#endif

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


#ifdef NOT_IMPLEMENTED_YET
  !> @param[inout] this the sparse matrix to add an element to
  !! @param[in] ir the row index of the element
  !! @param[in] ic the column index of the element
  recursive subroutine add_element_(this, ir, ic, val)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_TYPE_VAR_PREC), intent(in) :: ir, ic
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
    integer(BUD_INT_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), ncol(:), row(:)
    integer(BUD_INT_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr_(:), ncol_(:), row_(:)
    integer :: e

    if ( .not. is_initd(this) ) return

    sm = this
#ifdef BUD_SM_CSC
    call attach(sm, nr=nr, nc=nc, &
      cptr=ptr, ncol=ncol, row=row)
#else
    call attach(sm, nr=nr, nc=nc, &
      rptr=ptr, nrow=ncol, col=col)
#endif

    call attach(sm, nr=nr, nc=nc, &
      cptr=ptr, ncol=ncol, row=row)

    ! check if there is place in the sparse matrix
    ! for an entry without re-allocation
    if ( ptr(ic+ONE) - ptr(ic) > ncol(ic) ) then

      ix = ptr(ic)+ncol(ic)BUD_SM_PTR_A
      if ( ncol(ic) == 0 ) then
        i = 0
      else
        i = row(ix - ONE)
      end if
      row(ix) = ir
      ncol(ic) = ncol(ic) + ONE
      this%D%nz = this%D%nz + ONE

      if ( this%D%sorted .and. ir < i ) then

        ! denote it non-sorted
        this%D%sorted = .false.

      end if

      call set_error(this, 0)

      return

    end if

    ! Re-allocate the sparse matrix and copy it
    npc = maxval(ncol) + BUD_CC2(8_,BUD_INT_PREC)
    call new(nthis, nr, nc, npc=npc)
    call attach(nthis, cptr=ptr_, ncol=ncol_, row=row_)

    ! Copy all elements correctly
    ix = 1
    do c = 1 , nc
      ncol_(c) = ncol(c)

      ix = ptr_(c) BUD_SM_PTR_A
      ! copy this column
      do i = ptr(c)BUD_SM_PTR_A , ptr(c) + ncol(c) BUD_SM_PTR_B
        row_(ix) = row(i)
        ix = ix + ONE
      end do

    end do

    ! add the element to the new sparse pattern
    call add_nonzero(nthis, ir, ic)
    ! copy the sparse pattern to this one
    this = nthis
    ! clean-up
    call delete(nthis)
    ! specify the error message which then
    ! is the new size per row
    e = npc
    call set_error(this, e)

  end subroutine add_element_
#endif


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


