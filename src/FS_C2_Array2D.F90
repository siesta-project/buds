! @@LICENSE@@ see Copyright notice in the top-directory

!> @cond DOC_ALL

! Get default commands
#include "bud_utils.inc"

! Integer (short)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,sArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,sArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,sArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,sArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,sArray2D)
#include "bud_fstack.inc"
end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,iArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,iArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,iArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,iArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,iArray2D)
#include "bud_fstack.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,lArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,lArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,lArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,lArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,lArray2D)
#include "bud_fstack.inc"
end module

! Real (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,rArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,rArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,rArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,rArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,rArray2D)
#include "bud_fstack.inc"
end module

! Real (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,dArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,dArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,dArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,dArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,dArray2D)
#include "bud_fstack.inc"
end module

! Complex (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,cArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,cArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,cArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,cArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,cArray2D)
#include "bud_fstack.inc"
end module

! Complex (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_C2_,zArray2D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_C2_,zArray2D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_C2_,zArray2D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_C2_,zArray2D)
# define BUD_STACK_TYPE BUD_CC3(BUD_TYPE,C2_,zArray2D)
#include "bud_fstack.inc"
end module

!> @endcond DOC_ALL

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

