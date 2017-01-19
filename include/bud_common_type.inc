  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.
  ! This adds all OO routines

  !> Error status
  !!
  !! Each operation may return an error. This error may be retrieved
  !! through the error interface.
  !!
  !! @note
  !! Note that this is *not* contained in the TYPE_ which would
  !! prohibit the query of errors on non-initialized types.
  !! However, when a type cannot be created (due to erroneous
  !! input) there should be a way to message the error message.
  !! @endnote
  integer :: error = 0

#if BUD_FORTRAN >= 2003
  contains

# ifndef BUD_NO_DELETE
  !> See [[delete]]
  procedure, public :: delete => common_delete_
# endif
# ifndef BUD_NO_NULLIFY
  !> See [[nullify]]
  procedure, public :: nullify => common_nullify_
# endif
# ifndef BUD_NO_COPY
  !> See [[copy]]
  procedure, public :: copy => copy_
# endif
# ifndef BUD_NO_PRINT
  !> See [[print]]
  procedure, public :: print => print_
# endif


  !> See [[initialize]]
  procedure, public :: initialize => common_initialize_

  !> See [[is_initialized]]
  procedure, public :: is_initialized => common_is_initialized_
  !> See [[is_initialized]]
  procedure, public :: is_initd => common_is_initialized_
  !> See [[is_initialized]]
  procedure, public :: initialized => common_is_initialized_
  !> See [[is_same]]
  procedure, public :: is_same => common_is_same_
  !> See [[is_same]]
  procedure, public :: same => common_is_same_

  !> See [[references]]
  procedure, public :: references => common_references_
  !> See [[references]]
  procedure, public :: refs => common_references_

  !> See [[error]]
  procedure, public :: error => common_error_

#ifndef BUD_NO_IO
  !> See [[write]]
  procedure, public, pass(this) :: write => write_

  !> See [[read]]
  procedure, public, pass(this) :: read => read_
#endif

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

