  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.
  ! This adds all OO routines 
  
#if BUD_FORTRAN >= 2003
  
#ifndef BUD_NO_DELETE
  procedure, public :: delete => delete_
#endif
#ifndef BUD_NO_NULLIFY
  procedure, public :: nullify => nullify_
#endif

  procedure, public :: init => initialize_
  
  procedure, public :: is_initd => is_initialized_
  procedure, public :: is_same => is_same_
  
  procedure, public :: references => references_
  procedure, public :: get_refs => references_

#ifndef BUD_NO_PRINT
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

