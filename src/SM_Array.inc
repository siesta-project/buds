  ! @@LICENSE@@ see Copyright notice in the top-directory

!# define BUD_INCLUDE_TYPE "SM_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "SM_Array_type_.inc"


  !> Retrieve pointer to values in the sparse matrix
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface matrixp
    module procedure get_matrixp_
  end interface
  public :: matrixp

  !> Retrieve pointer to the sparse pattern
  !!
  !! The array does not step the reference counter
  !! of BUD_COLL_1.
  interface get_SMp
    module procedure get_elem1p_
  end interface

  !> Retrieve the sparse pattern
  interface get_SM
    module procedure get_elem1_
  end interface

  !> Retrieve the array @bud
  interface get_array
    module procedure get_elem2_
  end interface

  !> Retrieve a pointer to the array @bud (not values)
  interface get_arrayp
    module procedure get_elem2p_
  end interface

  
#include "bud_coll.inc"


  !> @param[in] this sparse matrix
  !! @return a pointer to the data (contiguous)
  function get_matrixp_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC()
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC()
#endif
    
    p => arrayp(this%D%e2)
    
  end function get_matrixp_

  
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
  
  
