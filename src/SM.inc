  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This include file creates the necessary preprocessor
  ! commands for interacting with the different sparse
  ! patterns.
  !

  ! It currently only handles C-interoperability

#ifndef BUD_SM_INTEROP_C
# define BUD_SM_INTEROP_C 0
#endif

#if BUD_SM_INTEROP_C == 0
# define BUD_SM_MOD modp
  ! This is the initial pointer value for the
  ! first pointer element
# define BUD_SM_PTR 1
  ! this is the addition to the pointer arrays
# define BUD_SM_PTR_A
# define BUD_SM_PTR_B -1
#else
# define BUD_SM_MOD mod
# define BUD_SM_PTR 0
# define BUD_SM_PTR_A +1
# define BUD_SM_PTR_B
#endif

  ! Correct mod for the SM_CS[RC] == 1 case
#ifdef BUD_SM_CSR
# if BUD_SM_CSR == 1
#  undef BUD_SM_MOD
#  undef BUD_SM_PTR
#  undef BUD_SM_PTR_A
#  undef BUD_SM_PTR_B
#  define BUD_SM_MOD modp
#  define BUD_SM_PTR 0
#  define BUD_SM_PTR_A +1
#  define BUD_SM_PTR_B 
# endif
#endif
#ifdef BUD_SM_CSC
# if BUD_SM_CSC == 1
#  undef BUD_SM_MOD
#  undef BUD_SM_PTR
#  undef BUD_SM_PTR_A
#  undef BUD_SM_PTR_B
#  define BUD_SM_MOD modp
#  define BUD_SM_PTR 0
#  define BUD_SM_PTR_A +1
#  define BUD_SM_PTR_B 
# endif
#endif



#if BUD_SM_INTEROP_C == 0
# define BUD_SM_IDX(a) (a)
# define BUD_SM_IDXF(a) (a)
#elif BUD_SM_INTEROP_C == 1
# define BUD_SM_IDX(a) (a-1)
# define BUD_SM_IDXF(a) (a+1)
#else
# error "SM_INTEROP_C *MUST* be either 0 or 1"
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

