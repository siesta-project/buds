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

  !> @defgroup bud-fstack Finite stack with reference counting
  !!
  !! @details
  !! The finite stack implementation is an extension of the regular
  !! reference counted objects.
  !!
  !! A stack is a "push-pop" sequence of @buds.
  !! A finite stack pushes new values at the top, if the stack is
  !! full the oldest entry is deleted and removed from the stack.
  !! A pop of a value removes the entry from the top of the stack.
  !!
  !! @note
  !! A type need not have _all_ these routines available if so desired.
  !! This is internally controlled and is mentioned for the relevant types.
  !!
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_TYPE_NAME of the type
  !!  - _BUD_TYPE_NAME_, internal data pointer of the type
  !!  - _BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - _BUD_TYPE_NEW, the routine name for creating a new type
  !!  - _BUD_STACK_TYPE, the variable type contained in the stack
  !!
  !! @acknowledge
  !! These declarations are community inspired from the FLIBS
  !! project by Arjen Markus as well as the PyF95 project.
  !! The initial draft of this library has been created by Alberto Garcia.

  
  !> @bud variable
  !!
  !! Handler for the pointer type stored
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    
#endif
  end type BUD_TYPE_NAME


  !> @bud container for BUD_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @bud.
  !!
  !! This data container holds 3 variables,
  !!  1. track maximum number of elements allowed in the finite stack
  !!  2. track how many elements are currently stored
  !!  3. An array of the stack of variables stored.
  type BUD_TYPE_NAME_

    !> Maximum number of elements that can be stored in this stack.
    !!
    !! @note
    !! There is little to no memory overhead of maxing this excessively
    !! large as all elements which have not been assigned are
    !! allocations of data-pointers.
    !!
    !! Hence creating extreme stacks may still be memory efficient.
    integer :: nel_max

    !> Current number of elements in the stack. 
    integer :: nel
    
    !> Array of elements (stack).
    type(BUD_STACK_TYPE), pointer :: T(:) => null()

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_


  ! special counter for deleting all elements
  ! of a stack using:
  !   call pop(<this>, BUD_FSTACK_ALL)
  !> Special parameter for popping all elements.
  !!
  !! Allows one to pop/delete all elements in the stack.
  integer, public, parameter :: BUD_FSTACK_ALL = 0


  !> Create new @bud instances
  interface new
    module procedure new_
  end interface
  public :: new
  
  !> Create new @bud instances
  interface BUD_TYPE_NEW
    module procedure new_
  end interface
  public :: BUD_TYPE_NEW
  
  !> Push new elements to the stack
  !!
  !! @note
  !! One cannot push elements in the middle of the stack.
  interface push
    module procedure push_
  end interface
  public :: push
  !> Pop, and possibly return elements from the stack
  !!
  !! One can return the top element or pop-delete elements
  !! from the top or bottom.
  !!
  !! @note
  !! One cannot pop and return elements from the middle of the
  !! stack.
  interface pop
    module procedure pop_
    module procedure pop_delete_
  end interface
  public :: pop
  !> Keeps a fixed number of elements in the stack
  interface keep
    module procedure keep_
  end interface
  public :: keep
  
  !> Return an indexed element in the stack
  interface get_elem
    module procedure get_elem_
  end interface
  public :: get_elem
  !> Return a pointer to an indexed element in the stack
  interface get_elemp
    module procedure get_elemp_
  end interface
  public :: get_elemp

  !> Return number of elements in the stack
  interface size
    module procedure size_elem_
  end interface
  public :: size
  
  !> Return maximum number of elements in the stack (function)
  interface size_max
    module procedure size_max_elem_
  end interface
  public :: size_max
  
  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print
  
  ! all default reference counted stuff
# include "bud_common.inc"
  

  pure subroutine delete_data(D)
    type(BUD_TYPE_NAME_), intent(inout) :: D
    
    if ( associated(D%T) ) then
      
      ! Deallocate array
      call delete(D%T)
      deallocate(D%T)
      nullify(D%T)
      
    end if
    
    D%nel_max = 0
    D%nel = 0
    
  end subroutine delete_data


  !> Internal initialization of finite stack holding BUD_STACK_TYPE
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this Type that is initialized
  !! @param[in] nel maximum number of elements that may be contained
  subroutine new_(this, nel)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in) :: nel

    if ( nel < 1 ) then
#ifdef BUD_ERROR
      call BUD_ERROR("Finite stack allocated with <1 maximum elements.")
#endif
      call delete(this)
      return
    end if

    call init(this)

    this%D%nel_max = nel
    this%D%nel = 0
    ! prepare data locations
    allocate(this%D%T(nel))

    call tag_object_(this)
    
  end subroutine new_


  !> Query number of currently stored elements in this stack (function)
  !!
  !! @param[inout] this @bud that is checked
  !! @return number of currently stored elements
  elemental function size_elem_(this) result (nel)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nel
    if ( .not. is_initd(this) ) then
      nel = 0
    else
      nel = this%D%nel
    end if
  end function size_elem_
    
  
  !> Query maximum number of allowed elements in this stack (function)
  !!
  !! @param[inout] this @bud that is checked
  !! @return maximum number of allowed elements
  elemental function size_max_elem_(this) result (nel_max)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nel_max
    if ( .not. is_initd(this) ) then
      nel_max = 0
    else
      nel_max = this%D%nel_max
    end if
  end function size_max_elem_

  
  !> Adds new element to the stack
  !!
  !! Add new element to the stack. If the stack is full the oldest
  !! element will be deleted.
  !!
  !! @param[inout] this @bud that is added to
  !! @param[in] item element added to the stack
  subroutine push_(this,item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_STACK_TYPE), intent(in) :: item

    integer :: el

    ! if this has not been initialised we immediately return
    ! We _could_ initialize it, but we do not know how
    ! many maximum elements that is allowed?
    ! So we return
    if ( .not. is_initd(this) ) return

    el = size(this)
    
    if ( el == size_max(this) ) then
      
      ! cycle stack and push
      this%D%T = cshift(this%D%T,+1)
      call delete(this%D%T(el))
      this%D%T(el) = item
    else
      el = el + 1
      ! simply add it to the stack
      this%D%nel = el
      this%D%T(el) = item
    end if
    
  end subroutine push_


  !> Pop and return the latest item in the stack
  !!
  !! The top item is returned in `item` and the stack
  !! is decremented thus removing the returned item from the stack.
  !!
  !! If there are no elements the returned item will be un-initialized.
  !!
  !! @note
  !! The returned item will thus not have its counted references changed.
  !!
  !! @param[inout] this @bud to pop from
  !! @param[inout] item returned element from the top of the stack
  subroutine pop_(this, item)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_STACK_TYPE), intent(inout) :: item

    integer :: el

    if ( .not. is_initd(this) ) then
      call delete(item)
      return
    end if

    el = size(this)
    
    ! easy case if it is empty
    if ( 0 == el ) then
      
      call delete(item)
      return
      
    end if
    
    ! retrieve the latest value
    item = this%D%T(el)

    call pop(this)
    
  end subroutine pop_
  
  
  !> Deletes the latest item(s) in the stack
  !!
  !! Pops and deletes any _n_ items from the top of the stack.
  !!
  !! Defaults to popping(deleting) the top element.
  !!
  !! @param[inout] this @bud from which elements are popped
  !! @param[in] top @opt=1 number of popped elements from top.
  !!                If BUD_FSTACK_ALL is supplied it will
  !!                clear all elements.
  !! @param[in] bot @opt=@null has precedence if `top` not supplied.
  !!                Pops `bot` elements from the bottom of the stack.
  subroutine pop_delete_(this, top, bot)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: top, bot

    integer :: lnpop
    integer :: el

    ! it is already empty...
    if ( .not. is_initd(this) ) return
    
    ! number of elements
    el = size(this)

    if ( present(top) .or. .not. present(bot) ) then
      
      ! regular top-popping
      lnpop = 1
    
      if ( present(top) ) lnpop = top
      
      ! BUD_FSTACK_ALL == 0, we delete all
      if ( lnpop == BUD_FSTACK_ALL ) lnpop = el

      ! we should not pop anything 
      if ( lnpop < 0 ) return
      
      do while ( lnpop > 0 )
        
        ! easy case if it is empty
        if ( 0 == el ) return
        
        ! Delete the local item and decrease counter
        call delete(this%D%T(el))
        this%D%nel = this%D%nel - 1
        
        el = el - 1
        lnpop = lnpop - 1
      end do
      
    else ! bot has been supplied

      ! bottom popping
      lnpop = bot

      ! in case of a negative number we cannot pop anything
      if ( lnpop < 0 ) return
    
      do while ( lnpop > 0 )
        
        ! easy case if it is empty
        if ( 0 == el ) return
        
        ! Delete the local item and decrease counter
        call delete(this%D%T(1))
        this%D%nel = this%D%nel - 1
        ! shift to attain the correct order
        this%D%T = cshift(this%D%T,-1)
        
        el = el - 1
        lnpop = lnpop - 1
      end do

    end if
    
  end subroutine pop_delete_

  !> Keeps a fixed number of elements in the stack
  !!
  !! Retains a number of elements from either the top or the
  !! bottom of the stack.
  !!
  !! If any of the counts are larger than the currently
  !! number of stored elements, nothing will happen.
  !!
  !! @note
  !! Defaults to `top=fnelem(this)-1`
  !!
  !! @param[inout] this @bud to keep elements in
  !! @param[in] top @opt=1 number of elements kept from the top
  !! @param[in] bot @opt=@null number of elements kept from the bottom,
  !!                only used if `top` is not supplied.
  subroutine keep_(this, top, bot)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: top, bot
    
    integer :: lnpop
    integer :: el
    
    if ( .not. is_initd(this) ) return

    ! number of elements
    el = size(this)

    if ( present(top) .or. .not. present(bot) ) then

      ! regular bot-popping
      lnpop = 1
      if ( present(top) ) lnpop = el - top

      ! we should not pop anything 
      if ( lnpop < 0 ) return

      call pop(this, bot=lnpop)
      
    else

      ! keep 'n' items from the bottom
      lnpop = el - bot

      ! we should not pop anything 
      if ( lnpop < 0 ) return

      call pop(this, top=lnpop)
      
    end if
    
  end subroutine keep_

  !> Retrieve specific element in the stack
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be un-initialized upon return.
  !!
  !! @param[in] this @bud to retrieve element from
  !! @param[inout] elem returned @bud stored in element `iel`
  !! @param[in] iel @opt=`size(this)` index of element to return. May be negative to offset from the top
  subroutine get_elem_(this, elem, iel)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_STACK_TYPE), intent(inout) :: elem
    integer, intent(in), optional :: iel
    integer :: el

    call delete(elem)
    if ( .not. is_initd(this) ) return

    ! Default element
    el = size(this)

    if ( present(iel) ) then
      if ( iel < 0 ) then
        el = el + iel
      else
        el = iel
      end if
    end if

    if ( el <= size(this) ) then
      elem = this%D%T(el)
    end if
    
  end subroutine get_elem_

  !> Retrieve pointer to specific element in the stack
  !!
  !! Retrieving a pointer does not change the reference status
  !! of the @bud.
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be null.
  !!
  !! @param[in] this @bud to retrieve pointer element from
  !! @param p pointer to the element `iel`
  !! @param[in] iel @opt=`size(this)` index of element to return. May be negative to offset from top.
  subroutine get_elemp_(this, p, iel)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_STACK_TYPE), pointer :: p
    integer, intent(in), optional :: iel
    integer :: el

    nullify(p)
    if ( .not. is_initd(this) ) return

    ! Default element
    el = size(this)

    if ( present(iel) ) then
      if ( iel < 0 ) then
        el = el + iel
      else
        el = iel
      end if
    end if
    
    if ( el <= size(this) ) then
      p => this%D%T(el)
    end if
      
  end subroutine get_elemp_

  
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
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer :: i, nel

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
    write(*,fmt) "<<", trim(name), " stack>"
    do i = 1 , size(this)
      call print(this%D%T(i), indent = lindent)
    end do
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <stack-refs: ", get_refs(this), ">>"

  end subroutine print_

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_STACK_TYPE

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

