  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.
  ! This adds all OO routines

#if BUD_FORTRAN >= 2003

  !> @name Intrinsic @bud procedures
  !! These procedures are common for nearly all
  !! @buds.
  !>@{
  ! needed comment to separate the next interface

# ifndef BUD_NO_DELETE
  !> @iSee delete
  procedure, public :: delete => common_delete_
# endif
# ifndef BUD_NO_NULLIFY
  !> @iSee nullify
  procedure, public :: nullify => common_nullify_
# endif
# ifndef BUD_NO_COPY
  !> @iSee copy
  procedure, public :: copy => copy_
# endif
# ifndef BUD_NO_PRINT
  !> @iSee print
  procedure, public :: print => print_
# endif


  !> @iSee initialize
  procedure, public :: initialize => common_initialize_

  !> @iSee is_initialized
  procedure, public :: is_initialized => common_is_initialized_
  !> @iSee is_initialized
  procedure, public :: is_initd => common_is_initialized_
  !> @iSee is_initialized
  procedure, public :: initialized => common_is_initialized_
  !> @iSee is_same
  procedure, public :: is_same => common_is_same_
  !> @iSee is_same
  procedure, public :: same => common_is_same_

  !> @iSee references
  procedure, public :: references => common_references_
  !> @iSee references
  procedure, public :: refs => common_references_

  !> @iSee error
  procedure, public :: error => common_error_

#ifndef BUD_NO_IO
  !> @iSee #write
  procedure, public, pass(this) :: write => write_

  !> @iSee #read
  procedure, public, pass(this) :: read => read_
#endif

  !>>@}

#ifdef BUD_INCLUDE_TYPE
#   include BUD_INCLUDE_TYPE
#endif

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

