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
  !> @defgroup bud-coll Reference type of a collection
  !!
  !! @details
  !! The implementation details of a collection of reference counted types.
  !! This type implements a common container for N>1 @buds.
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
  !!  - _BUD_COLL_A, name of the first @bud
  !!  - _BUD_COLL_B, name of the second @bud
  !!  - _BUD_COLL_C, name of the third @bud (need not be used)
  !!  - _BUD_COLL_D, name of the fourth @bud (need not be used)
  !!  - _BUD_COLL_E, name of the fifth @bud (need not be used)
  !!
  !! @acknowledge
  !! These declarations are community inspired from the FLIBS
  !! project by Arjen Markus as well as the PyF95 project.
  !! The initial draft of this library has been created by Alberto Garcia.

  !> @bud container for BUD_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @bud.
  !!
  !! The container handles a collection up to 5 elements.
  !! Each element is stored via alphabetic sequences
  !! Hence the first item is `A`, second is `B` etc.
  !!
#ifndef BUD_COLL_C
  !! This @bud is a collection of 2 elements:
#else
#ifndef BUD_COLL_D
  !! This @bud is a collection of 3 elements:
#else
#ifndef BUD_COLL_E
  !! This @bud is a collection of 4 elements:
#else
  !! This @bud is a collection of 5 elements:
#endif
#endif
#endif
  !! 1. BUD_COLL_A
  !! 2. BUD_COLL_B
#ifdef BUD_COLL_C
  !! 3. BUD_COLL_C
#ifdef BUD_COLL_D
  !! 4. BUD_COLL_D
#ifdef BUD_COLL_E
  !! 5. BUD_COLL_E
#endif
#endif
#endif
  type BUD_TYPE_NAME_    
    !> First element of collection
    type(BUD_COLL_A) :: A
    !> Second element of collection
    type(BUD_COLL_B) :: B
#ifdef BUD_COLL_C
    !> Third element of collection
    type(BUD_COLL_C) :: C
#ifdef BUD_COLL_D
    !> Fourth element of collection
    type(BUD_COLL_D) :: D
#ifdef BUD_COLL_E
    !> Fifth element of collection
    type(BUD_COLL_E) :: E
#endif
#endif
#endif
    
    ! Consistent data in the reference counted object
#   include "bud_common_type.inc"

  end type BUD_TYPE_NAME_

  public :: print
  
  !> Create a new @bud. This will be allocated with 1 reference.
  interface new
    module procedure new_
  end interface
  public :: new
  !> Create a new @bud. This will be allocated with 1 reference.
  interface BUD_TYPE_NEW
    module procedure new_
  end interface
  public :: BUD_TYPE_NEW

  !> Retrieve the first element of the collection (routine)
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface get_elema
    module procedure get_elema_
  end interface
  public :: get_elema
  
  !> Point to the first element of the collection (routine)
  !!
  !! This routine does not step the reference counting as it
  !! is a pointer.
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface get_elempa
    module procedure get_elempa_
  end interface
  public :: get_elempa
  

  !> Point to the first element of the collection (function)
  !!
  !! This routine does not step the reference counting as it
  !! is a pointer.
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface elempa
    module procedure elempa_
  end interface
  public :: elempa
  
  !> @cond ALWAYS_SKIP

  ! we do not document these sub-routines as they are encompassed
  ! by elema etc.

  interface get_elemb
    module procedure get_elemb_
  end interface
  public :: get_elemb
  interface get_elempb
    module procedure get_elempb_
  end interface
  public :: get_elempb
  interface elempb
    module procedure elempb_
  end interface
  public :: elempb

#ifdef BUD_COLL_C
  interface get_elemc
    module procedure get_elemc_
  end interface
  public :: get_elemc
  interface get_elempc
    module procedure get_elempc_
  end interface
  public :: get_elempc
  interface elempc
    module procedure elempc_
  end interface
  public :: elempc
  
#ifdef BUD_COLL_D
  interface get_elemd
    module procedure get_elemd_
  end interface
  public :: get_elemd
  interface get_elempd
    module procedure get_elempd_
  end interface
  public :: get_elempd
  interface elempd
    module procedure elempd_
  end interface
  public :: elempd

#ifdef BUD_COLL_E
  interface get_eleme
    module procedure get_eleme_
  end interface
  public :: get_eleme
  interface get_elempe
    module procedure get_elempe_
  end interface
  public :: get_elempe
  interface elempe
    module procedure elempe_
  end interface
  public :: elempe
#endif
#endif
#endif

  !> @endcond ALWAYS_SKIP

  !> Print, to std-out, some basic information of the data-container
  interface print
    module procedure print_
  end interface print
  
  ! all default reference counted stuff
# include "bud_common.inc"
  
  ! This routine takes care of deleting the 
  ! types storage

  pure subroutine delete_data(data)
    type(BUD_TYPE_NAME_), intent(inout) :: data
    call delete(data%a)
    call delete(data%b)
#ifdef BUD_COLL_C
    call delete(data%c)
#ifdef BUD_COLL_D
    call delete(data%d)
#ifdef BUD_COLL_E
    call delete(data%e)
#endif
#endif
#endif
  end subroutine delete_data

  !> Internal initialization of data container
  !!
  !! Initializes the data handler to a new data handler.
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this initialized @bud
  !! @param[in] A first element in the @bud
  !! @param[in] B second element in the @bud
#ifdef BUD_COLL_C
  !! @param[in] C third element in the @bud
#ifdef BUD_COLL_D
  !! @param[in] D fourth element in the @bud
#ifdef BUD_COLL_E
  !! @param[in] E fifth element in the @bud
#endif
#endif
#endif
  !!
  !! @author Nick R. Papior (2016, February)
  subroutine new_(this, a, b &
#ifdef BUD_COLL_C
    , c &
#ifdef BUD_COLL_D
    , d &
#ifdef BUD_COLL_E
    , e &
#endif
#endif
#endif
    )
    type(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_COLL_A), intent(in) :: a
    type(BUD_COLL_B), intent(in) :: b
#ifdef BUD_COLL_C
    type(BUD_COLL_C), intent(in) :: c
#ifdef BUD_COLL_D
    type(BUD_COLL_D), intent(in) :: d
#ifdef BUD_COLL_E
    type(BUD_COLL_E), intent(in) :: e
#endif
#endif
#endif

    call init(this)

    ! set objects
    this%data%a = a
    this%data%b = b
#ifdef BUD_COLL_C
    this%data%c = c
#ifdef BUD_COLL_D
    this%data%d = d
#ifdef BUD_COLL_E
    this%data%e = e
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
  !!
  !! @author Nick R. Papior (2016, February)
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
    call print(this%data%a, indent = lindent)
    call print(this%data%b, indent = lindent)
#ifdef BUD_COLL_C
    call print(this%data%c, indent = lindent)
#ifdef BUD_COLL_D
    call print(this%data%d, indent = lindent)
#ifdef BUD_COLL_E
    call print(this%data%e, indent = lindent)
#endif
#endif
#endif
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <coll-refs: ", get_refs(this), ">>"

  end subroutine print_

  ! Create data-retrieval routines
# define BUD_COLL_LETTER A
# define BUD_COLL_CURRENT BUD_COLL_A
# include "bud_coll_elem.inc"
# define BUD_COLL_LETTER B
# define BUD_COLL_CURRENT BUD_COLL_B
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_C
# define BUD_COLL_LETTER C
# define BUD_COLL_CURRENT BUD_COLL_C
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_D
# define BUD_COLL_LETTER D
# define BUD_COLL_CURRENT BUD_COLL_D
# include "bud_coll_elem.inc"
#ifdef BUD_COLL_E
# define BUD_COLL_LETTER E
# define BUD_COLL_CURRENT BUD_COLL_E
# include "bud_coll_elem.inc"
#endif
#endif
#endif

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_COLL_A
#undef BUD_COLL_B
#undef BUD_COLL_C
#undef BUD_COLL_D
#undef BUD_COLL_E

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

