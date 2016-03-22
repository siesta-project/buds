  ! @@LICENSE@@ see Copyright notice in the top-directory
  
  ! Include the common utilities
#include "bud_utils.inc"

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#ifndef BUD_TYPE_NAME_
#  define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#endif
#ifndef BUD_TYPE_NAME_STR
#  define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
#endif

#include "bud_common_declarations.inc"
  
  ! We use "collection" as "set" may be confusing as nothing is _set_.
  !> @defgroup bud-coll Collection @bud of several @buds
  !!
  !! A collection holds a certain number of @buds in a strict order.
  !! Hence the elements contained are predefined in the data type and
  !! any access to the stored elements may be accessed by retrieving the
  !! data and subsequently operating on that data-type.\n
  !! The collection data-type may be used to easily group together
  !! associated data from different @buds.
  !!
  !! The stored elements may be accessed either via routine calls:
  !! ~~~{.f90}
  !! call get_elem1(this, A)
  !! ~~~
  !! which will be equivalent to deleting `A` and assigning the
  !! stored element.
  !! One can also access the data via a pointer:
  !! ~~~{.f90}
  !! A => get_elem1p(this)
  !! ~~~
  !! which does not increase the reference counter for the stored
  !! element. Hence no deletion should be performed.
  !!
  !! @dev_note
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !!
  !! One __cannot__ store non-@bud types in this collection.
  !!
  !! Currently this collection only allows up to 5 @buds contained.
  !! This can easily be extended.
  !!
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_TYPE_NAME, name of the resulting type
  !!  - _BUD_TYPE_NAME_, name of the internal data
  !!  - _BUD_TYPE_NEW, additional name of the `new` command
  !!  - _BUD_COLL_1, name of the first @bud
  !!  - _BUD_COLL_2, name of the second @bud
  !!  - _BUD_COLL_3, name of the third @bud (need not be used)
  !!  - _BUD_COLL_4, name of the fourth @bud (need not be used)
  !!  - _BUD_COLL_5, name of the fifth @bud (need not be used)
  !!

  !> Internal data container for BUD_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @bud.
  !!
  !! The container handles a collection of up to 5 elements.
  !! Each element is stored via numeric sequences
  !! Hence the first item is `e1`, second is `e2` etc.
  !!
#ifndef BUD_COLL_3
  !! This @bud is a collection of 2 elements:
#else
#ifndef BUD_COLL_4
  !! This @bud is a collection of 3 elements:
#else
#ifndef BUD_COLL_5
  !! This @bud is a collection of 4 elements:
#else
  !! This @bud is a collection of 5 elements:
#endif
#endif
#endif
  !! 1. BUD_COLL_1
  !! 2. BUD_COLL_2
#ifdef BUD_COLL_3
  !! 3. BUD_COLL_3
#ifdef BUD_COLL_4
  !! 4. BUD_COLL_4
#ifdef BUD_COLL_5
  !! 5. BUD_COLL_5
#endif
#endif
#endif
  type BUD_TYPE_NAME_    
    !> First element of collection
    type(BUD_COLL_1) :: e1
    !> Second element of collection
    type(BUD_COLL_2) :: e2
#ifdef BUD_COLL_3
    !> Third element of collection
    type(BUD_COLL_3) :: e3
#ifdef BUD_COLL_4
    !> Fourth element of collection
    type(BUD_COLL_4) :: e4
#ifdef BUD_COLL_5
    !> Fifth element of collection
    type(BUD_COLL_5) :: e5
#endif
#endif
#endif
    
    ! Consistent data in the reference counted object
#   include "bud_common_type.inc"

  end type BUD_TYPE_NAME_

  !> Initialize new BUD_TYPE_NAME
  interface new
    module procedure new_
  end interface
  public :: new
  !> Initialize new BUD_TYPE_NAME (@isee new)
  interface BUD_TYPE_NEW
    module procedure new_
  end interface
  public :: BUD_TYPE_NEW

  !> Retrieve the first element of the collection (routine)
  !!
  !! Change the suffix number `1` to `2`, ... to get the
  !! corresponding element in the collection.
  interface get_elem1
    module procedure get_elem1_
  end interface
  public :: get_elem1
  
  !> Return a pointer to the first element of the collection (function)
  !!
  !! This routine does not step the reference counting as it
  !! returns a pointer.
  !!
  !! Change the suffix number `1` to `2`, ... to get the
  !! corresponding element in the collection.
  interface get_elem1p
    module procedure get_elem1p_
  end interface
  public :: get_elem1p
    
  !> @cond ALWAYS_SKIP

  ! we do not document these sub-routines as they are encompassed
  ! by elema etc.

  interface get_elem2
    module procedure get_elem2_
  end interface
  public :: get_elem2
  interface get_elem2p
    module procedure get_elem2p_
  end interface
  public :: get_elem2p

#ifdef BUD_COLL_3
  interface get_elem3
    module procedure get_elem3_
  end interface
  public :: get_elem3
  interface get_elem3p
    module procedure get_elem3p_
  end interface
  public :: get_elem3p
  
#ifdef BUD_COLL_4
  interface get_elem4
    module procedure get_elem4_
  end interface
  public :: get_elem4
  interface get_elem4p
    module procedure get_elem4p_
  end interface
  public :: get_elem4p

#ifdef BUD_COLL_5
  interface get_elem5
    module procedure get_elem5_
  end interface
  public :: get_elem5
  interface get_elem5p
    module procedure get_elem5p_
  end interface
  public :: get_elem5p
#endif
#endif
#endif

  !> @endcond ALWAYS_SKIP

  !> Print, to std-out, some basic information of the data-container
  interface print
    module procedure print_
  end interface print
  public :: print
  
  ! all default reference counted stuff
# include "bud_common.inc"
  
  ! This routine takes care of deleting the 
  ! types storage

  pure subroutine delete_data(D)
    type(BUD_TYPE_NAME_), intent(inout) :: D

    call delete(D%e1)
    call delete(D%e2)
#ifdef BUD_COLL_3
    call delete(D%e3)
#ifdef BUD_COLL_4
    call delete(D%e4)
#ifdef BUD_COLL_5
    call delete(D%e5)
#endif
#endif
#endif
    
  end subroutine delete_data

  !> Initialize of data container with containing elements
  !!
  !! A new @bud is initialized with each element
  !! being specified in the routine interface.
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this new initialized @bud
  !! @param[in] A first element in the @bud
  !! @param[in] B second element in the @bud
#ifdef BUD_COLL_3
  !! @param[in] C third element in the @bud
#ifdef BUD_COLL_4
  !! @param[in] D fourth element in the @bud
#ifdef BUD_COLL_5
  !! @param[in] E fifth element in the @bud
#endif
#endif
#endif
  subroutine new_(this, a, b &
#ifdef BUD_COLL_3
    , c &
#ifdef BUD_COLL_4
    , d &
#ifdef BUD_COLL_5
    , e &
#endif
#endif
#endif
    )
    type(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_COLL_1), intent(in) :: a
    type(BUD_COLL_2), intent(in) :: b
#ifdef BUD_COLL_3
    type(BUD_COLL_3), intent(in) :: c
#ifdef BUD_COLL_4
    type(BUD_COLL_4), intent(in) :: d
#ifdef BUD_COLL_5
    type(BUD_COLL_5), intent(in) :: e
#endif
#endif
#endif

    call init(this)

    ! set objects
    this%D%e1 = a
    this%D%e2 = b
#ifdef BUD_COLL_3
    this%D%e3 = c
#ifdef BUD_COLL_4
    this%D%e4 = d
#ifdef BUD_COLL_5
    this%D%e5 = e
#endif
#endif
#endif

    call tag_object_(this)
    
  end subroutine new_

  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    type(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent
    integer :: lindent
    
    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = BUD_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent
    
    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    
    if ( .not. is_initd(this) ) then
      write(*,fmt) "<", trim(name), " not initialized>"
      return
    end if

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<<", trim(name), " coll>"
    call print(this%D%e1, indent = lindent)
    call print(this%D%e2, indent = lindent)
#ifdef BUD_COLL_3
    call print(this%D%e3, indent = lindent)
#ifdef BUD_COLL_4
    call print(this%D%e4, indent = lindent)
#ifdef BUD_COLL_5
    call print(this%D%e5, indent = lindent)
#endif
#endif
#endif
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <coll-refs: ", get_refs(this), ">>"

  end subroutine print_

  ! Create data-retrieval routines
# define BUD_COLL_NUM 1
# define BUD_COLL_CURRENT BUD_COLL_1
# include "bud_coll_elem.inc"
# define BUD_COLL_NUM 2
# define BUD_COLL_CURRENT BUD_COLL_2
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_3
# define BUD_COLL_NUM 3
# define BUD_COLL_CURRENT BUD_COLL_3
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_4
# define BUD_COLL_NUM 4
# define BUD_COLL_CURRENT BUD_COLL_4
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_5
# define BUD_COLL_NUM 5
# define BUD_COLL_CURRENT BUD_COLL_5
# include "bud_coll_elem.inc"
#endif
#endif
#endif

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_COLL_1
#undef BUD_COLL_2
#undef BUD_COLL_3
#undef BUD_COLL_4
#undef BUD_COLL_5

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

