  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Simple type for inclusion in the type scopes.
  
  ! Number of times referenced 
  integer :: ryp_refs = 0
  ! Hash ID
  ! Currently this is not used, but it may prove useful
  ! in later implementations.
  character(len=RYP_ID_LEN) :: ryp_id = "null_id"
  
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

