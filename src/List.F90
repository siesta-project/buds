  ! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup list List with set-like operations
!! @ingroup bud-intrinsic
!!
!! A list implementation with set-like operations.
!!
!! The list is the equivalent of the #Array1D structure
!! however with a large set of intrinsic functionality
!! not available for the #Array1D @bud.
!!
!! The `List` enables:
!!  1. Sorting
!!  2. Set operations; union, intersection, complement
!!  3. Reverse elements
!!  4. Automatic re-allocation when more elements are needed.
!!  5. Binary search-algorithms if sorted.
!!
!!
!! The list also implements index functions which are extremely fast
!! if the list is sorted. In such cases the list may be used as pivoting
!! array.


! The first inputs are integers
#define BUD_IS_INTEGER

!> @defgroup List common parameters
!! @ingroup list
!! Module for retaining commonly used parameters.
!! @{
module BUD_CC3(BUD_MOD,_,List_common)
  implicit none

  ! Ensure these are public
  public

  !> Maximum name length of the list
  integer, parameter :: LIST_MAX_NAME_LEn = 64

end module
!! @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,sList)
!> @defgroup BUD_MOD_NAME Integer (short)
!! @ingroup list
!! `integer(selected_int_kind(4))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,sList)
# define BUD_TYPE_VAR integer
# define BUD_INT_PREC is_
# define BUD_TYPE_VAR_PREC is_
#include "List.inc"
# undef BUD_INT_PREC
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,iList)
!> @defgroup BUD_MOD_NAME Integer (int)
!! @ingroup list
!! `integer(selected_int_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,iList)
# define BUD_TYPE_VAR integer
# define BUD_INT_PREC ii_
# define BUD_TYPE_VAR_PREC ii_
#include "List.inc"
# undef BUD_INT_PREC
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,lList)
!> @defgroup BUD_MOD_NAME Integer (long)
!! @ingroup list
!! `integer(selected_int_kind(18))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,lList)
# define BUD_TYPE_VAR integer
# define BUD_INT_PREC il_
# define BUD_TYPE_VAR_PREC il_
#include "List.inc"
# undef BUD_INT_PREC
end module
!> @}

! Ensure that the following list stuff does
! not have sort routines etc.
#undef BUD_IS_INTEGER
# define BUD_INT_PREC ii_

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,rList)
!> @defgroup BUD_MOD_NAME Real (float)
!! @ingroup list
!! `real(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,rList)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rr_
#include "List.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,dList)
!> @defgroup BUD_MOD_NAME Real (double)
!! @ingroup list
!! `real(selected_real_kind(15))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,dList)
# define BUD_TYPE_VAR real
# define BUD_TYPE_VAR_PREC rd_
#include "List.inc"
end module
!> @}

#define BUD_IS_COMPLEX

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,cList)
!> @defgroup BUD_MOD_NAME Complex (float)
!! @ingroup list
!! `complex(selected_real_kind(6))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,cList)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rr_
#include "List.inc"
end module
!> @}

# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,zList)
!> @defgroup BUD_MOD_NAME Complex (double)
!! @ingroup list
!! `complex(selected_real_kind(9))` data type
!! @{
module BUD_MOD_NAME
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,zList)
# define BUD_TYPE_VAR complex
# define BUD_TYPE_VAR_PREC rd_
#include "List.inc"
end module
!> @}


!> @defgroup List module for all different objects
!! @ingroup list
!! Module for making everything public
!! @{
module BUD_CC3(BUD_MOD,_,List)

  use BUD_CC3(BUD_MOD,_,List_common)
  use BUD_CC3(BUD_MOD,_,sList)
  use BUD_CC3(BUD_MOD,_,iList)
  use BUD_CC3(BUD_MOD,_,lList)
  use BUD_CC3(BUD_MOD,_,rList)
  use BUD_CC3(BUD_MOD,_,dList)
  use BUD_CC3(BUD_MOD,_,cList)
  use BUD_CC3(BUD_MOD,_,zList)

end module
!> @}

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

