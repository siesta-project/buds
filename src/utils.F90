! @@LICENSE@@ see Copyright notice in the top-directory

! Get default commands
#include "refype_utils.inc"

# define _R_MOD_NAME _R_CC2(_R_MOD,_utils)
!> Generic utility functions used throughout
!!
!! Several utility functions which enables
!! a large variety of optimized algorithms used in various
!! routines in the refype library.
module _R_MOD_NAME

  ! Include basic stuff
# include "refype_common_declarations.inc"

  ! Sort routines
  public :: sort_ps, sort_heap, sort_quick

  !> Sort an array which is partially sorted
  interface sort_ps
    module procedure _R_CC2(sort_ps_,ii_)
    module procedure _R_CC2(sort_ps_,il_)
  end interface

  !> Sort an array using the heap-sort algorithm
  interface sort_heap
    module procedure _R_CC2(sort_heap_,ii_)
    module procedure _R_CC2(sort_heap_,il_)
    module procedure _R_CC2(sort_heap_idx_,ii_)
    module procedure _R_CC2(sort_heap_idx_,il_)
  end interface 

  !> Sort an array using the quick-sort algorithm
  interface sort_quick
    module procedure _R_CC2(sort_quick_,ii_)
    module procedure _R_CC2(sort_quick_,il_)
    module procedure _R_CC2(sort_quick_idx_,ii_)
    module procedure _R_CC2(sort_quick_idx_,il_)
  end interface 


  ! Find routines
  public :: find_bin, find_binheur

  !> Find an element in a sorted array using the binary search algorithm
  interface find_bin
    module procedure _R_CC2(find_bin_,ii_)
    module procedure _R_CC2(find_bin_,il_)
  end interface 

  !> Find an element in a sorted array using a binary heuristic search algorithm
  interface find_binheur
    module procedure _R_CC2(find_binheur_,ii_)
    module procedure _R_CC2(find_binheur_,il_)
  end interface 

  
  ! Unique count scenes
  public :: nuniq_sorted
  !> Query number of unique values in a sorted array
  interface nuniq_sorted
    module procedure _R_CC2(nuniq_sorted_,ii_)
    module procedure _R_CC2(nuniq_sorted_,il_)
  end interface 

  public :: nuniq
  !> Query number of unique values in an array
  interface nuniq
    module procedure _R_CC2(nuniq_,ii_)
    module procedure _R_CC2(nuniq_,il_)
    module procedure _R_CC2(nuniq_pvt_,ii_)
    module procedure _R_CC2(nuniq_pvt_,il_)
  end interface 

  
  public :: modp
  !> 1-based modulo operator
  interface modp
    module procedure _R_CC2(modp_,ii_)
    module procedure _R_CC2(modp_,il_)
  end interface 

contains

  !> Return wrapped integer by modulo operation
  !!
  !! Routine for performing 1-index based modulo
  !! operations.
  !!
  !! For any numbers `i` and `n` performing `modp(i,n)`
  !! returns `n` if `mod(i,n) == 0`, otherwise it
  !! returns `i`.
  elemental function _R_CC2(modp_,ii_)(i,n) result(p)
    integer(ii_), intent(in) :: i, n
    integer(ii_) :: p
    p = mod(i-1,n) + 1
  end function

  !> Return wrapped integer by modulo operation
  !!
  !! Routine for performing 1-index based modulo
  !! operations.
  !!
  !! For any numbers `i` and `n` performing `modp(i,n)`
  !! returns `n` if `mod(i,n) == 0`, otherwise it
  !! returns `i`.
  elemental function _R_CC2(modp_,il_)(i,n) result(p)
    integer(il_), intent(in) :: i, n
    integer(il_) :: p
    p = mod(i-1,n) + 1
  end function 

  
#undef _R_PREC
#define _R_PREC ii_
# include "SORT_integer.inc"
# include "FIND_integer.inc"
# include "UNIQ_integer.inc"

#undef _R_PREC
#define _R_PREC il_
# include "SORT_integer.inc"
# include "FIND_integer.inc"
# include "UNIQ_integer.inc"

end module


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

