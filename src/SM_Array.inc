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

!# define BUD_INCLUDE_TYPE "SM_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "SM_Array_type_.inc"


  !> Retrieve pointer data in the sparse matrix
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface matrix_p
    module procedure get_matrixp_
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

#include "bud_coll.inc"


  !> @param[in] this sparse matrix
  !! @return a pointer to the data (contiguous)
  function get_matrixp_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    p => array_p(this%D%e2)

  end function get_matrixp_


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


#undef BUD_TYPE_NEW

  ! Associated with the Array
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

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


