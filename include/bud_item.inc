  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_ITEM_TYPE, the @bud contained in the single item


  ! Include the common utilities
#include "bud_utils.inc"

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)

#include "bud_common_declarations.inc"


  !> Item wrapper for a bud
  !!
  !! This wrapper type contains another bud. It may
  !! be used as a container for sub-partitioning.
  !!
  !! Generally you need (should) not use this Item type
  !! unless you have a specific use case.
  !! The direct data storage of a bud is more efficient
  !! and requires fewer additional wrapper functions.
  type BUD_TYPE_NAME

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    procedure, private :: new_
    procedure, private :: new_data_

    !> See [[new]]
    generic, public :: new => new_, new_data_

    !> See [[set_data]]
    procedure, public :: set => set_data_
    !> See [[set_data]]
    procedure, public :: set_data => set_data_

    !> See [[get_data]]
    procedure, public :: get => get_data_
    !> See [[get_data]]
    procedure, public :: get_data => get_data_

#endif
  end type BUD_TYPE_NAME


  !> Data container for the bud.
  !!
  !! Data is stored in this type which is
  !! pointed to by the parent type.
  !! This is an internal type only used as a pointer and
  !! should not be made public.
  type BUD_TYPE_NAME_

    !> Contained item data
    type(BUD_ITEM_TYPE) :: item

#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_


  !> Create a new bud prepared for containing a BUD_ITEM_TYPE
  !!
  !! Initialize the object and prepare it for use.
  !!
  !! There are two variants:
  !!  1. no passed BUD_ITEM_TYPE, and
  !!  2. passed BUD_ITEM_TYPE
  !! which either initializes the object with
  !! an empty contained BUD_ITEM_TYPE or with a reference to the passed
  !! type.
  interface new
    module procedure new_
    module procedure new_data_
  end interface
  public :: new


  !> Store BUD_ITEM_TYPE in the bud
  !!
  !! Stores passed BUD_ITEM_TYPE in the bud.
  !!
  !! The currently stored BUD_ITEM_TYPE will be deleted
  !! before assigning the passed BUD_ITEM_TYPE.
  !!
  !! If the passed item is not initialized, so will the
  !! contained bud.
  interface set_data
    module procedure set_data_
  end interface
  public :: set_data
  !> See [[set_data]]
  interface set
    module procedure set_data_
  end interface
  public :: set


  !> Retrieve a reference counted BUD_ITEM_TYPE from bud
  !!
  !! Returns the contained bud by using a reference counter
  !! pointing to the currently contained BUD_ITEM_TYPE.
  interface get_data
    module procedure get_data_
  end interface
  public :: get_data
  !> See [[get_data]]
  interface get
    module procedure get_data_
  end interface
  public :: get

  !> Overriding `assignment(=)` by allowing contained data
  !!
  !! Allow assigning BUD_TYPE_NAME with BUD_ITEM_TYPE.
  interface assignment(=)
    module procedure set_data_
    module procedure get_data_assign_
  end interface


#define BUD_NO_IO
  ! all default reference counted stuff
# include "bud_common.inc"

  !> @cond BUD_DEVELOPER

  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this

    call delete(this%D%item)

    call set_error(this, error(this%D%item))

  end subroutine delete_


  !> @param[in] from the origin of data
  !! @param[inout] to the destination of the data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    call initialize(to)
    call common_copy_(from, to)

    call copy(from%D%item, to%D%item)

  end subroutine copy_


  !> @param[inout] this type to be initialized
  subroutine new_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    call initialize(this)

  end subroutine new_

  !> @param[inout] this type to be initialized
  !! @param[in] item BUD_ITEM_TYPE to be referenced in the item
  subroutine new_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_ITEM_TYPE), intent(in) :: item

    call new(this)

    this = item

  end subroutine new_data_


  !> @param[inout] this bud to contain a reference to `item`
  !! @param[in] item BUD_ITEM_TYPE to store in `this`
  subroutine set_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_ITEM_TYPE), intent(in) :: item

    if ( is_initd(this) ) then
      this%D%item = item
    else
      call new(this, item)
    end if

  end subroutine set_data_

  !> @param[in] this item containing bud
  !! @param[inout] item item to store bud-data in
  subroutine get_data_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_ITEM_TYPE), intent(inout) :: item

    if ( is_initd(this) ) then
      item%D = this%D%item%D
    else
      call delete(item)
    end if

  end subroutine get_data_

  !> @param[inout] item item to store bud-data in
  !! @param[in] this item containing bud
  subroutine get_data_assign_(item, this)
    type(BUD_ITEM_TYPE), intent(inout) :: item
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    if ( is_initd(this) ) then
      item = this%D%item
    else
      call delete(item)
    end if

  end subroutine get_data_assign_


  !> @param[in] this data type
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
    write(*,fmt) " <item-refs: ", references(this), ">>"

  end subroutine print_

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
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

