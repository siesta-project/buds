
#ifdef BUD_MPI
# ifdef BUD_MPI_INCLUDE
   include BUD_MPI_INCLUDE
# else
#  ifndef BUD_MPI_MODULE
#   define BUD_MPI_MODULE mpi
#  endif
   use BUD_MPI_MODULE
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

