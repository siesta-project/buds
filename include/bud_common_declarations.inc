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

  ! Data precisions (PRIVATE)
  ! Although these names may be "uncommon"
  ! they are only internal variables and should only be used as such.
  ! They represent the 3 allowed integer variables:
  !  is == short (2-byte)
  !  ii == int (4-byte)
  !  il == long (8-byte)
  ! and the real variables
  !  rr == real (4-byte)
  !  rd == double (8-byte)
  !  re == long double, currently not supported 

  !> @cond BUD_DEVELOPER
  
  integer, private, parameter :: rr_ = selected_real_kind(p=6) ! single (real*4)
  integer, private, parameter :: rd_ = selected_real_kind(p=15) ! double (real*8)
  !integer, private, parameter :: re_ = selected_real_kind(p=18)
    
  integer, private, parameter :: is_ = selected_int_kind(4) ! short (integer*2)
  integer, private, parameter :: ii_ = selected_int_kind(9) ! int (integer*4)
  integer, private, parameter :: il_ = selected_int_kind(18) ! long (integer*8)

  !> @endcond BUD_DEVELOPER
  
  !> Size of ID parameter for bud data-containers
  integer, private, parameter :: BUD_ID_LEn = 36

# ifdef BUD_MOD_NAME_STR
  !> Name of module
  ! Note // is often interpreted as a C-comment, hence we simply
  ! skip to a newline to make it expand in the preprocessor.
  character(len=*), parameter, private :: BUD_MOd = BUD_XSTR(BUD_CC2(BUD_MOD,_)) // &
    BUD_MOD_NAME_STR
# endif
  
# ifdef BUD_TYPE_NAME_STR
  !> Name of @bud in this module
  character(len=*), parameter, private :: BUD_TYPe = BUD_TYPE_NAME_STR
# endif

  
#endif
  
  
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

