  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @addtogroup bud-intrinsic

!> @defgroup list List
!! @ingroup bud-intrinsic
!!
!! @bud containing a fixed dimension list.
!!
!! The list is the equivalent of the #Array1D structure
!! however with a large set of intrinsic functionality
!! not available for the #Array1D @bud.
!!
!! @{

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sList)
!> @defgroup BUD_MOD_NAME Integer (short)
!! `integer(selected_int_kind(4))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sList)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,List)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC is_
#include "List.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iList)
!> @defgroup BUD_MOD_NAME Integer (int)
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iList)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,List)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "List.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lList)
!> @defgroup BUD_MOD_NAME Integer (long)
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lList)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,List)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "List.inc"
end module
!> @}

! GROUP list
!> @}


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

