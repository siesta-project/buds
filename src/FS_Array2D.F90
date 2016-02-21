  ! @@LICENSE@@ see Copyright notice in the top-directory

!> @cond DOC_ALL

! Get default commands
#include "refype_utils.inc"

! Integer (short)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,sArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,sArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,sArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,sArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,sArray2D)
#include "refype_fstack.inc"
end module

! Integer (int)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,iArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,iArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,iArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,iArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,iArray2D)
#include "refype_fstack.inc"
end module

! Integer (long)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,lArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,lArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,lArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,lArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,lArray2D)
#include "refype_fstack.inc"
end module

! Real (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,rArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,rArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,rArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,rArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,rArray2D)
#include "refype_fstack.inc"
end module

! Real (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,dArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,dArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,dArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,dArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,dArray2D)
#include "refype_fstack.inc"
end module

! Complex (single)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,cArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,cArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,cArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,cArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,cArray2D)
#include "refype_fstack.inc"
end module

! Complex (double)
# define _R_MOD_NAME _R_CC3(_R_MOD,_FS_,zArray2D)
!> @file _R_MOD_NAME
module _R_MOD_NAME
  ! Requirement
  use _R_CC3(_R_MOD,_,zArray2D)
# define _R_TYPE_NAME _R_CC3(_R_TYPE,FS_,zArray2D)
# define _R_TYPE_NEW _R_CC3(_R_NEW,_FS_,zArray2D)
# define _R_STACK_TYPE _R_CC2(_R_TYPE,zArray2D)
#include "refype_fstack.inc"
end module

!> @endcond DOC_ALL

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

