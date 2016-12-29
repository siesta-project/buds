  ! @@LICENSE@@ see Copyright notice in the top-directory

#ifndef BUD_COMMON_DECLARATIONS_H
#define BUD_COMMON_DECLARATIONS_H

# include "bud_utils.inc"

  ! Force explicit declarations of variables
  implicit none

  ! Privatize all modules.
  ! Perhaps this may be illeviated as it limits control.
  ! However, it does reduce name-space cluttering, immensely.
  private

  ! Ensure that the precision for the buds objects are prepended
  ! with a _
# define BUD_PRECISION_SUFFIX _
# include "bud_precision.inc"

  !> Size of ID parameter for bud data-containers
  integer, parameter, private :: BUD_ID_LEn = 36

# ifdef BUD_MOD_NAME_STR
  !> Name of module
  ! Note // is often interpreted as a C-comment, hence we simply
  ! skip to a newline to make it expand in the preprocessor.
  character(len=*), parameter, private :: &
    BUD_MOd = 'BUD_CC2(BUD_MOD,_)BUD_MOD_NAME'
# endif

# ifdef BUD_TYPE_NAME_STR
  !> Name of @bud in this module
  character(len=*), parameter, private :: &
    BUD_TYPe = BUD_TYPE_NAME_STR
# endif


#endif


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

