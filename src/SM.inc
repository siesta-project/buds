  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! This include file creates the necessary preprocessor
  ! commands for interacting with the different sparse
  ! patterns.
  !

  ! It currently only handles C-interoperability

#ifndef BUD_SM_INTEROP_C
# define BUD_SM_INTEROP_C 0
#endif

#undef BUD_SM_MOD
#undef BUD_SM_PTR
#undef BUD_SM_PTR_A
#undef BUD_SM_PTR_B

#if BUD_SM_INTEROP_C == 0
# define BUD_SM_MOD modp
  ! This is the initial pointer value for the
  ! first pointer element
# define BUD_SM_PTR ONE
  ! this is the addition to the pointer arrays
# define BUD_SM_PTR_A
# define BUD_SM_PTR_B -ONE
#else
# define BUD_SM_MOD mod
# define BUD_SM_PTR ZERO
# define BUD_SM_PTR_A +ONE
# define BUD_SM_PTR_B
#endif

  ! Correct mod for the SM_CS[RC] == 1 case
#ifdef BUD_SM_CS
# if BUD_SM_CS == 1
#  undef BUD_SM_MOD
#  undef BUD_SM_PTR
#  undef BUD_SM_PTR_A
#  undef BUD_SM_PTR_B
#  define BUD_SM_MOD modp
#  define BUD_SM_PTR ZERO
#  define BUD_SM_PTR_A +ONE
#  define BUD_SM_PTR_B
# endif
#endif

#if BUD_SM_INTEROP_C == 0
# define BUD_SM_IDX(a) (a)
# define BUD_SM_IDXF(a) (a)
#elif BUD_SM_INTEROP_C == 1
# define BUD_SM_IDX(a) (a-ONE)
# define BUD_SM_IDXF(a) (a+ONE)
#else
# error "SM_INTEROP_C *MUST* be either 0 or 1"
#endif


#ifdef BUD_SM_CSC
# define _CS_I ic
# define _CS_J ir
# define _CS_N nc
# define _CS_M nr
# define _CS_PTR cptr
# define _CS_PTR_N column
# define _CS_PTR_S nc
# define _CS_NUM n_row
# define _CS_NUM_S nc
# define _CS_IDX row
# define _CS_IDX_N row
#endif
#ifdef BUD_SM_CSR
# define _CS_I ir
# define _CS_J ic
# define _CS_N nr
# define _CS_M nc
# define _CS_PTR rptr
# define _CS_PTR_N row
# define _CS_PTR_S nr
# define _CS_NUM n_column
# define _CS_NUM_S nr
# define _CS_IDX column
# define _CS_IDX_N column
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

