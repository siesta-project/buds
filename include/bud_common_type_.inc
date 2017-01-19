  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.

#ifdef BUD_INCLUDE_TYPE_
#   include BUD_INCLUDE_TYPE_
#endif

  !> Reference counter for this bud
  !!
  !! This integer tracks the number of times this derived type
  !! is referenced.
  !! It will _always_ be \( \ge 0 \).
  !!
  !! On every assignment this counter is incremented by `1`,
  !! on each deletion it is decremented by `1` until it hits
  !! `0` upon which a complete deallocation of the derived type
  !! is performed.
  integer :: refs_ = 0

  !> Hash ID for the bud (unique for different buds)
  !!
  !! Enables a unique ID for any bud of the same type.
  character(len=BUD_ID_LEN) :: id_ = "null_id"


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

