
#define BUD_TYPE_MP_COMM BUD_CC2(BUD_TYPE,MP_Comm)

#if BUD_DIM > 1
  !> Integer corresponding to the distributed index (`1<=dist_idx<=BUD_DIM`)
  !! The default dimension is the last dimension.
  integer :: dist_idx = BUD_DIM
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

