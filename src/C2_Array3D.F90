  ! @@LICENSE@@ see Copyright notice in the top-directory

!> @cond DOC_ALL

! Get default commands
#include "bud_utils.inc"

! Integer (short)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,sArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,sArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,sArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,sArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,sArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,sArray3D)
#include "bud_coll.inc"
end module

! Integer (int)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,iArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,iArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,iArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,iArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,iArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,iArray3D)
#include "bud_coll.inc"
end module

! Integer (long)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,lArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,lArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,lArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,lArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,lArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,lArray3D)
#include "bud_coll.inc"
end module

! Real (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,rArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,rArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,rArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,rArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,rArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,rArray3D)
#include "bud_coll.inc"
end module

! Real (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,dArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,dArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,dArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,dArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,dArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,dArray3D)
#include "bud_coll.inc"
end module

! Complex (single)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,cArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,cArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,cArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,cArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,cArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,cArray3D)
#include "bud_coll.inc"
end module

! Complex (double)
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_C2_,zArray3D)
!> @file BUD_MOD_NAME
module BUD_MOD_NAME
  ! Requirement
  use BUD_CC3(BUD_MOD,_,zArray3D)
# define BUD_TYPE_NAME BUD_CC3(BUD_TYPE,C2_,zArray3D)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_C2_,zArray3D)
# define BUD_COLL_A BUD_CC2(BUD_TYPE,zArray3D)
# define BUD_COLL_B BUD_CC2(BUD_TYPE,zArray3D)
#include "bud_coll.inc"
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

