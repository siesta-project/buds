  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Get default commands
#include "bud_utils.inc"


!> @defgroup transfer Transfer arrays between dimensions
!! @ingroup bud-intrinsic
!!
!! Enables transfering one data-type from one dimension to another
!! dimension.
!! As all the dimensions are explicitly defined the transfer functions
!! also enables resizing.
!!
!! @{
module BUD_CC2(BUD_MOD,_Transfer)

#include "bud_common_declarations.inc"

#define BUD_TRANSFER_FUNC(prefix,name) BUD_CC5(prefix,_,BUD_TYPE_VAR_P,name,_)

  !> Transfer an associated array from `N`-D to `M`-D.
  !!
  !! The general calling scheme is:
  !! \code{.f90}
  !!   call transfer_dim_p(dim-sizes<out>,out,dim-sizes<in>,in)
  !! \endcode
  !! For instance to transfer a 2D real array to a 3D array
  !! pointer one may use this call.
  !! \code{.f90}
  !!   real :: in(100,10)
  !!   real, pointer :: out(:,:,:)
  !!   call transfer_dim_p(10,10,10,out,100,10,in)
  !! \endcode
  !! In the above code the `out` array has dimensions
  !! `(10,10,10)` while the input array has `(100,10)`.
  !! As the `transfer_dim_p` function is interfaced
  !! one could also realize the call by:
  !! \code{.f90}
  !!   real :: in(100,10)
  !!   real, pointer :: out(:,:,:)
  !!   call transfer_dim_p(10,10,10,out,1000,in(:,1,1))
  !! \endcode
  !! Remark that the last call only works because the
  !! memory is consecutive, if not, the association would
  !! be to a temporary array which will result in seg-faults
  !! if used incorrectly (this is because most compilers
  !! creates temporary arrays to ensure alignment).
  !!
  !! @note If the total size of the output array is larger than
  !! the input array the resulting output array will be nullified.
  !! However, the output array size may be smaller to reduce
  !! the array extent.
  !!
  !! @note It is imperative that the passed `in`-array is
  !! consecutive in memory!
  !!
  !! @note The table below illustrates which transfers
  !! are possible (top-row is `in` array, left-column in `out` array:
  !!
  !!   Dim | 1D | 2D | 3D | 4D | 5D
  !!    1D | X  | X  | X  | X  | X
  !!    2D | X  | X  | X  | X  | X
  !!    3D | X  | X  | X  | X  | X
  !!    4D |    |    |    |    |
  !!    5D |    |    |    |    |
  !!
  !! @param[in] o<> dimension(s) of output associated array
  !! @param out the resulting associated array
  !! @param[in] i<> dimension(s) of in array
  !! @param[in] in array to be associated
  interface transfer_dim_p
# define BUD_TYPE_VAR_P b
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P s
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P i
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P l
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P r
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P d
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P c
# include "TransferD_Assoc_interface.inc"
# define BUD_TYPE_VAR_P z
# include "TransferD_Assoc_interface.inc"
  end interface
  public :: transfer_dim_p

  !> Transfer by copy an array from `N`-D to `M`-D.
  !!
  !! The general calling scheme is:
  !! \code{.f90}
  !!   call transfer_dim(dim-sizes<out>,out,dim-sizes<in>,in)
  !! \endcode
  !! For instance to transfer a 2D real array to a 3D array
  !! one may use this call.
  !! \code{.f90}
  !!   real :: in(100,10)
  !!   real :: out(10,10,10)
  !!   call transfer_dim(10,10,10,out,100,10,in)
  !! \endcode
  !! In the above code the `out` array has dimensions
  !! `(10,10,10)` while the input array has `(100,10)`.
  !! As the `transfer_dim` function is interfaced
  !! one could also realize the call by:
  !! \code{.f90}
  !!   real :: in(100,10)
  !!   real :: out(10,10,10)
  !!   call transfer_dim(10,10,10,out,1000,in(:,1,1))
  !! \endcode
  !!
  !! @note If the total size of the output array is larger than
  !! the input array only the elements up to the size of the
  !! input array will be copied, the remaining elements are not
  !! initialized to any value.
  !!
  !! @note The table below illustrates which transfers
  !! are possible (top-row is `in` array, left-column in `out` array:
  !!
  !!   Dim | 1D | 2D | 3D | 4D | 5D
  !!    1D | X  | X  | X  | X  | X
  !!    2D | X  | X  | X  | X  | X
  !!    3D | X  | X  | X  | X  | X
  !!    4D |    |    |    |    |
  !!    5D |    |    |    |    |
  !!
  !!
  !! @param[in] o<> dimension(s) of output associated array
  !! @param out the resulting associated array
  !! @param[in] i<> dimension(s) of in array
  !! @param[in] in array to be associated
  interface transfer_dim
# define BUD_TYPE_VAR_P b
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P s
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P i
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P l
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P r
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P d
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P c
# include "TransferD_Copy_interface.inc"
# define BUD_TYPE_VAR_P z
# include "TransferD_Copy_interface.inc"
  end interface
  public :: transfer_dim

contains

#define BUD_TYPE_VAR logical
#define BUD_TYPE_VAR_P b
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR integer
#define BUD_TYPE_VAR_PREC is_
#define BUD_TYPE_VAR_P s
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR integer
#define BUD_TYPE_VAR_PREC ii_
#define BUD_TYPE_VAR_P i
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR integer
#define BUD_TYPE_VAR_PREC il_
#define BUD_TYPE_VAR_P l
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR real
#define BUD_TYPE_VAR_PREC rr_
#define BUD_TYPE_VAR_P r
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR real
#define BUD_TYPE_VAR_PREC rd_
#define BUD_TYPE_VAR_P d
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR complex
#define BUD_TYPE_VAR_PREC rr_
#define BUD_TYPE_VAR_P c
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"
#define BUD_TYPE_VAR complex
#define BUD_TYPE_VAR_PREC rd_
#define BUD_TYPE_VAR_P z
#include "TransferD_Assoc1D.inc"
#include "TransferD_Assoc2D.inc"
#include "TransferD_Assoc3D.inc"
#include "TransferD_Copy1D.inc"
#include "TransferD_Copy2D.inc"
#include "TransferD_Copy3D.inc"

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
