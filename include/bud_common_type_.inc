  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.

#ifdef BUD_INCLUDE_TYPE_
#   include BUD_INCLUDE_TYPE_
#endif

  !> @cond BUD_DEVELOPER

  !> Reference counter for @bud
  !! Tracks the number of times this data structure is referenced
  integer :: refs_ = 0

  !> Error message
  !!
  !! Each operation may return an error. This error may be retrieved
  !! through the error interface.
  integer :: error_ = 0

  !> Hash ID for the @bud (unique for different @buds)
  !! Enables a unique ID for any @bud of the same type.
  character(len=BUD_ID_LEN) :: id_ = "null_id"

  !> @endcond BUD_DEVELOPER


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

