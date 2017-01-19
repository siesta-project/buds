  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"


! Define the number of dimensions in this Array
#define BUD_DIM 1
#define BUD_DIMD BUD_CC2(BUD_DIM,D)


# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,bArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,bArray1D)
# define BUD_TYPE_VAR logical
#undef BUD_TYPE_VAR_PREC
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC is_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC ii_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lArray1D)
# define BUD_TYPE_VAR integer
# define BUD_TYPE_VAR_PREC il_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,rArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,rArray1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,dArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,dArray1D)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,cArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,cArray1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
#include "Array.inc"
end module

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,zArray1D)
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,zArray1D)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
#include "Array.inc"
end module


module BUD_CC3(BUD_MOD,_,Array1D)

  use BUD_CC3(BUD_MOD,_,sArray1D)
  use BUD_CC3(BUD_MOD,_,iArray1D)
  use BUD_CC3(BUD_MOD,_,lArray1D)
  use BUD_CC3(BUD_MOD,_,rArray1D)
  use BUD_CC3(BUD_MOD,_,dArray1D)
  use BUD_CC3(BUD_MOD,_,cArray1D)
  use BUD_CC3(BUD_MOD,_,zArray1D)

end module


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

