  ! @@LICENSE@@ see Copyright notice in the top-directory

!> @cond DOC_ALL

! Get default commands
#include "bud_utils.inc"

! Integer (short)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,sArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,sArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,sArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,sArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,sArray3D)
#include "bud_fstack.inc"
end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,iArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,iArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,iArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,iArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,iArray3D)
#include "bud_fstack.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,lArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,lArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,lArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,lArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,lArray3D)
#include "bud_fstack.inc"
end module

! Real (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,rArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,rArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,rArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,rArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,rArray3D)
#include "bud_fstack.inc"
end module

! Real (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,dArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,dArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,dArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,dArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,dArray3D)
#include "bud_fstack.inc"
end module

! Complex (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,cArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,cArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,cArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,cArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,cArray3D)
#include "bud_fstack.inc"
end module

! Complex (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_FS_,zArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,zArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,FS_,zArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_FS_,zArray3D)
# define BUD_STACK_TYPE BUD_CC2(BUD_TYPE,zArray3D)
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

