  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_common_declarations.inc"

  ! Before contains

  !> Retrieve pointer to one of the contained arrays in the stack
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! If the element does not exist, the pointer will be nullified.
  !!
  !! @note
  !! The internal array is `allocatable` so you may not deallocate the array-pointer.
  interface fs_array_p
    module procedure array_p_
  end interface fs_array_p
  public :: fs_array_p

#include "bud_finitestack.inc"
  ! after contains

  !> @param[in] this array @bud
  !! @return a pointer to the data (contiguous)
  function array_p_(this, i) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: i
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer  BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    integer :: li

    if ( present(i) ) then
      li = i
    else
      li = size(this)
    end if

    if ( 0 < li .and. li <= size(this) ) then
      p => array_p(this%D%T(li))
    else
      nullify(p)
    end if

  end function

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


