  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "refype_utils.inc"

! Integer (short)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,sArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,sArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,sArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,sArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,sArray1D)
#include "refype_fstack.inc"
end module

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,iArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,iArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,iArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,iArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,iArray1D)
#include "refype_fstack.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,lArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,lArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,lArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,lArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,lArray1D)
#include "refype_fstack.inc"
end module

! Real (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,rArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,rArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,rArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,rArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,rArray1D)
#include "refype_fstack.inc"
end module

! Real (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,dArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,dArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,dArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,dArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,dArray1D)
#include "refype_fstack.inc"
end module

! Complex (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,cArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,cArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,cArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,cArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,cArray1D)
#include "refype_fstack.inc"
end module

! Complex (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,zArray1D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,zArray1D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,_FS_,zArray1D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,zArray1D)
# define _R_STACK_TYPE _R_CC3(_R_TYPE,_,zArray1D)
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

