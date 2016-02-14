! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "refype_utils.inc"

! Integer (short)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,sArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_,sArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,sArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,sArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_,sArray3D)
#include "refype_fstack.inc"
end module

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,iArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_iArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,iArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,iArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_iArray3D)
#include "refype_fstack.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,lArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_lArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,lArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,lArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_lArray3D)
#include "refype_fstack.inc"
end module

! Real (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,rArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_rArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,rArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,rArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_rArray3D)
#include "refype_fstack.inc"
end module

! Real (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,dArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_dArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,dArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,dArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_dArray3D)
#include "refype_fstack.inc"
end module

! Complex (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,cArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_cArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,cArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,cArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_cArray3D)
#include "refype_fstack.inc"
end module

! Complex (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_C2_,zArray3D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_C2_zArray3D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_C2_,zArray3D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_C2_,zArray3D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_C2_zArray3D)
#include "refype_fstack.inc"
end module

! project-refype -- local file settings
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

