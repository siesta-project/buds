! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "bud_utils.inc"

!> @defgroup bud-intrinsic Intrinsic @buds
!!
!! Intrinsic buds for usage and reference (best viewed at level 2).

# define BUD_MOD_NAME BUD_CC2(BUD_MOD,_utils)


!> @defgroup utility Utilities for generic usage in different @buds
!! @ingroup bud-intrinsic
!!
!! Generic utility functions used throughout
!!
!! Several utility functions which enables
!! a large variety of optimized algorithms used in various
!! routines in the bud library.
!! @{
module BUD_MOD_NAME

  ! Include basic stuff
# include "bud_common_declarations.inc"

  ! Sort routines
  public :: sort_ps, sort_heap, sort_quick

  !> Sorting routine for integer arrays (partially sorted)
  !!
  !! Re-arranges the values in an integer array
  !! and returns the resulting sorted array in another
  !! array.
  !!
  !! This routine is fastest for partially sorted
  !! arrays such as:
  !! \code{.f90}
  !!   array = (/1,2,3,4,25,26,27,28,15,16,17/)
  !! \endcode
  !!
  !! To sort generic arrays using the quick-sort algorithm
  !! may be faster.
  interface sort_ps
    module procedure BUD_CC2(sort_ps_,is_)
    module procedure BUD_CC2(sort_ps_,ii_)
    module procedure BUD_CC2(sort_ps_,il_)
  end interface

  !> Heap sort algorithm for integers
  !!
  !! Heap sort algorithm for sorting integer arrays.
  !! Two variants exists where one is in-place, the
  !! other returns the pivoting indices to create a
  !! sorted array.
  interface sort_heap
    module procedure BUD_CC2(sort_heap_,is_)
    module procedure BUD_CC2(sort_heap_,ii_)
    module procedure BUD_CC2(sort_heap_,il_)
    module procedure BUD_CC2(sort_heap_idx_,is_)
    module procedure BUD_CC2(sort_heap_idx_,ii_)
    module procedure BUD_CC2(sort_heap_idx_,il_)
  end interface 

  !> Quick-sort array
  !!
  !! Sorts an integer array using the quick-sort algorithm.
  !! Two variants exists where one is in-place, the
  !! other returns the pivoting indices to create a
  !! sorted array.
  interface sort_quick
    module procedure BUD_CC2(sort_quick_,is_)
    module procedure BUD_CC2(sort_quick_,ii_)
    module procedure BUD_CC2(sort_quick_,il_)
    module procedure BUD_CC2(sort_quick_idx_,is_)
    module procedure BUD_CC2(sort_quick_idx_,ii_)
    module procedure BUD_CC2(sort_quick_idx_,il_)
  end interface 


  ! Find routines
  public :: find_bin, find_binheur

  !> Searches for a specific integer value in a sorted array (binary search)
  !!
  !! Efficient binary search algorithm for searching
  !! for specific elements in a sorted integer array.
  interface find_bin
    module procedure BUD_CC2(find_bin_,is_)
    module procedure BUD_CC2(find_bin_,ii_)
    module procedure BUD_CC2(find_bin_,il_)
  end interface 

  !> Searches for a specific integer value in a sorted array (heuristic search)
  !!
  !! Efficient binary-like search algorithm for searching
  !! for specific elements in an integer array.
  interface find_binheur
    module procedure BUD_CC2(find_binheur_,is_)
    module procedure BUD_CC2(find_binheur_,ii_)
    module procedure BUD_CC2(find_binheur_,il_)
  end interface 

  
  ! Unique count scenes
  public :: nuniq_sorted
  !> Count number of unique value for a sorted array
  !!
  !! Returns number of unique values in a sorted array.
  interface nuniq_sorted
    module procedure BUD_CC2(nuniq_sorted_,is_)
    module procedure BUD_CC2(nuniq_sorted_,ii_)
    module procedure BUD_CC2(nuniq_sorted_,il_)
  end interface 

  public :: nuniq
  !> Count number of unique values in a non-sorted array.
  !!
  !! Return the number of unique values in the passed array.
  !! The array need not be sorted but one may optionally pass
  !! a pivoting array
  !! which sorts the array. This pivoting array may be
  !! retrieved from the sorting routines (`sort_quick`, `sort_heap`).
  interface nuniq
    module procedure BUD_CC2(nuniq_,is_)
    module procedure BUD_CC2(nuniq_,ii_)
    module procedure BUD_CC2(nuniq_,il_)
    module procedure BUD_CC2(nuniq_pvt_,is_)
    module procedure BUD_CC2(nuniq_pvt_,ii_)
    module procedure BUD_CC2(nuniq_pvt_,il_)
  end interface 

  
  public :: modp
  !> Return wrapped integer by modulo operation
  !!
  !! Routine for performing 1-index based modulo
  !! operations.
  !!
  !! For any numbers `i` and `n` performing `modp(i,n)`
  !! returns `n` if `mod(i,n) == 0`, otherwise it
  !! returns `i`.
  interface modp
    module procedure BUD_CC2(modp_,is_)
    module procedure BUD_CC2(modp_,ii_)
    module procedure BUD_CC2(modp_,il_)
  end interface 

contains

  
  !> @param[in] i number to extract the remainer of
  !! @param[in] n divisor
  !! @return the remainder of `(i+1) % n`
  elemental function BUD_CC2(modp_,is_)(i,n) result(p)
    integer(is_), intent(in) :: i, n
    integer(is_) :: p
    p = mod(i-1,n) + 1
  end function

  !> @param[in] i number to extract the remainer of
  !! @param[in] n divisor
  !! @return the remainder of `(i+1) % n`
  elemental function BUD_CC2(modp_,ii_)(i,n) result(p)
    integer(ii_), intent(in) :: i, n
    integer(ii_) :: p
    p = mod(i-1,n) + 1
  end function

  !> @param[in] i number to extract the remainer of
  !! @param[in] n divisor
  !! @return the remainder of `(i+1) % n`
  elemental function BUD_CC2(modp_,il_)(i,n) result(p)
    integer(il_), intent(in) :: i, n
    integer(il_) :: p
    p = mod(i-1,n) + 1
  end function 

  
#undef BUD_PREC
#define BUD_PREC is_
# include "SORT_integer.inc"
# include "FIND_integer.inc"
# include "UNIQ_integer.inc"
  
#undef BUD_PREC
#define BUD_PREC ii_
# include "SORT_integer.inc"
# include "FIND_integer.inc"
# include "UNIQ_integer.inc"

#undef BUD_PREC
#define BUD_PREC il_
# include "SORT_integer.inc"
# include "FIND_integer.inc"
# include "UNIQ_integer.inc"

end module
!> @}


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

