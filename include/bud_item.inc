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

  !> @defgroup buds-item Item
  !!
  !! Container for a single element of another @bud (wrapper)
  !!
  !! This @bud enables containing another @bud and only one.
  !! 
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_TYPE_NAME of the type
  !!  - _BUD_TYPE_NAME_, internal data pointer of the type
  !!  - _BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - _BUD_TYPE_NEW, the routine name for creating a new type
  !!  - _BUD_ITEM_TYPE, the @bud contained in the single item


  !> @bud variable
  !!
  !! Handler for the pointer type stored
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    procedure, private :: new_
    procedure, private :: new_data_
    !> Initialization of the @bud
    generic, public :: new => new_, new_data_

    !> Set passed data as the contained item
    procedure, public :: set => set_data_
    procedure, public :: set_data => set_data_

    !> Query the stored data in the item
    procedure, public :: get => get_data_
    procedure, public :: get_data => get_data_

#endif
  end type BUD_TYPE_NAME

  
  !> Internal data structure for containing an item
  type BUD_TYPE_NAME_

    !> Contained item data
    type(BUD_ITEM_TYPE) :: item

#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_


  !> Create new @bud instances
  interface new
    module procedure new_
    module procedure new_data_
  end interface
  public :: new
  
  !> Create new @bud instances
  interface BUD_TYPE_NEW
    module procedure new_
    module procedure new_data_
  end interface
  public :: BUD_TYPE_NEW


  !> Set @bud in the item
  interface set_data
    module procedure set_data_
  end interface
  public :: set_data

  !> Get @bud from an item
  interface get_data
    module procedure get_data_
  end interface
  public :: get_data

  ! this should probably not be documented
  ! as it is defined twice.
  ! We override the assignment to
  ! also accept direct contained element
  interface assignment(=)
    module procedure set_data_
    module procedure get_data_assign_
  end interface

  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print
  
  ! all default reference counted stuff
# include "bud_common.inc"
  
  pure subroutine delete_data(D)
    type(BUD_TYPE_NAME_), intent(inout) :: D
    
    call delete(D%item)
    
  end subroutine delete_data


  !> Internal initialization of item containing BUD_ITEM_TYPE
  !!
  !! This creates a new BUD_TYPE_NAME without any contained
  !! @bud.
  !!
  !! @param[inout] this @bud that is initialized
  subroutine new_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    call initialize(this)

    call tag_object_(this)
    
  end subroutine new_

  !> Internal initialization of item containing BUD_ITEM_TYPE
  !!
  !! This creates a new BUD_TYPE_NAME by setting the contained
  !! @bud.
  !!
  !! @param[inout] this @bud that is initialized
  !! @param[inout] item stored BUD_ITEM_TYPE in the item
  subroutine new_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_ITEM_TYPE), intent(in) :: item

    call initialize(this)

    this = item

    call tag_object_(this)
    
  end subroutine new_data_


  !> Store BUD_ITEM_TYPE in the @bud
  !!
  !! Will store the passed BUD_ITEM_TYPE in the @bud
  !!
  !! @param[inout] this item to store @bud-data in
  !! @param[in] item @bud which will be contained in `this`
  subroutine set_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_ITEM_TYPE), intent(in) :: item
    
    if ( is_initd(this) ) then
      this%D%item = item
    end if
    
  end subroutine set_data_

  !> Retrieve BUD_ITEM_TYPE from @bud
  !!
  !! @param[in] this item containing @bud
  !! @param[inout] item item to store @bud-data in
  subroutine get_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_ITEM_TYPE), intent(inout) :: item
    
    if ( is_initd(this) ) then
      item = this%D%item
    else
      call delete(item)
    end if
    
  end subroutine get_data_

  !> Retrieve BUD_ITEM_TYPE from @bud
  !!
  !! @param[inout] item item to store @bud-data in
  !! @param[in] this item containing @bud
  subroutine get_data_assign_(item, this)
    type(BUD_ITEM_TYPE), intent(inout) :: item
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
    if ( is_initd(this) ) then
      item = this%D%item
    else
      call delete(item)
    end if
    
  end subroutine get_data_assign_

  
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in), target :: this
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

    write(fmt, '(a,i0,a)') '(t',lindent,',3a,i0,a)'
    write(*,fmt) "<<", trim(name), ">"
    lindent = lindent + 2 ! step indentation
    call print(this%D%item, indent = lindent)
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <item-refs: ", get_refs(this), ">>"
    
  end subroutine print_

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_ITEM_TYPE

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

