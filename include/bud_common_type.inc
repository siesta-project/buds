  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.
  ! This adds all OO routines 
  
#if BUD_FORTRAN >= 2003
  
#ifndef BUD_NO_DELETE
  !> @iSee delete
  procedure, public :: delete => delete_
#endif
#ifndef BUD_NO_NULLIFY
  !> @iSee nullify
  procedure, public :: nullify => nullify_
#endif

  !> @iSee initialize
  procedure, public :: initialize => initialize_

  !> @iSee is_initialized
  procedure, public :: is_initialized => is_initialized_
  !> @iSee is_initialized
  procedure, public :: is_initd => is_initialized_
  !> @iSee is_initialized
  procedure, public :: initialized => is_initialized_
  !> @iSee is_same
  procedure, public :: is_same => is_same_
  !> @iSee is_same
  procedure, public :: same => is_same_
  
  !> @iSee references
  procedure, public :: references => references_
  !> @iSee references
  procedure, public :: refs => references_

#ifndef BUD_NO_PRINT
  !> @iSee print
  procedure, public :: print => print_
#endif

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

