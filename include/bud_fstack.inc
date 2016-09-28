  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW, the routine name for creating a new type
  !  - BUD_STACK_TYPE, the variable type contained in the stack

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

  !> @cond BUD_DOC_INC

  !> @defgroup bud-fstack Finite stack of arbitrary @bud
  !!
  !! The finite stack implementation is an extension of the regular
  !! reference counted objects.
  !!
  !! A stack is a "push-pop" sequence of @buds.
  !! A `push` of a new @bud is inserted at the top, if the stack is
  !! full the oldest entry is deleted and removed from the stack.
  !!
  !! A `pop` of a @bud removes the top entry of the stack and possibly
  !! returns the @bud.
  !!
  !! @note
  !! A type need not have _all_ these routines available if so desired.
  !! This is internally controlled and is mentioned for the relevant types.

  !> @endcond BUD_DOC_INC
  
  !> Finite stack with cyclic ability
  !!
  !! Container for a finite stack of a single @bud (BUD_STACK_TYPE).
  !!
  !! This data container holds 3 variables,
  !!  1. track maximum number of elements allowed in the finite stack
  !!  2. track how many elements are currently stored
  !!  3. An array of the stack of variables stored.
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @iSee BUD_NEW_TYPE
    procedure, public :: BUD_NEW_TYPE => new_
    !> @iSee BUD_NEW_TYPE
    procedure, public :: new => new_

    !> @iSee #push
    procedure, public :: push => push_
    !> @iSee #pop
    procedure, public :: pop => pop_
    !> @iSee #retain
    procedure, public :: retain => retain_

    !> @iSee #element
    procedure, public :: element => get_elem_
    !> @iSee #element
    procedure, public :: get => get_elem_

    !> @iSee #element_p
    procedure, public :: element_p => get_elemp_
    !> @iSee #element_p
    procedure, public :: get_p => get_elemp_

    !> @iSee #size
    procedure, public :: size => size_elem_
    !> @iSee #size_max
    procedure, public :: size_max => size_max_elem_
    
#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  type BUD_TYPE_NAME_

    !> Maximum number of elements that can be stored in this stack.
    !!
    !! @note
    !! There is little to no memory overhead of having an excessively
    !! large `max` as all elements which have not been assigned are
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

  !> @endcond BUD_DEVELOPER
  

  ! special counter for deleting all elements
  ! of a stack using:
  !   call pop(<this>, BUD_FSTACK_ALL)
  !> Special parameter for popping all elements.
  !!
  !! Allows one to pop/delete all elements in the stack.
  !! This basically performs a
  !! \code{.f90}
  !!  n = size_max(this)
  !!  call delete(this)
  !!  call new(this, n)
  !! \endcode
  integer, public, parameter :: BUD_FSTACK_ALL = 0


  !> Internal initialization of finite stack holding BUD_STACK_TYPE
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  interface BUD_TYPE_NEW
    module procedure new_
  end interface
  public :: BUD_TYPE_NEW
  
  !> @iSee BUD_TYPE_NEW
  interface new
    module procedure new_
  end interface
  public :: new
  
  !> Adds/Pushes new element to the stack
  !!
  !! If the stack is full the oldest element will be deleted
  !! and the second oldest becomes the first element of the stack.
  !!
  !! @note
  !! One cannot push elements in the middle of the stack.
  interface push
    module procedure push_
  end interface
  public :: push

  !> Pop and (possibly) return the latest item in the stack
  !!
  !! The top item is returned in `item` and the stack
  !! is decremented thus removing the returned item from the stack.
  !!
  !! An optional index may be given, see #pop_delete_.
  !!
  !! If the top element has not been initialized the returned item will
  !! be un-initialized.
  !!
  !! @note
  !! The returned item will thus not have its counted references changed.
  interface pop
    module procedure pop_
    module procedure pop_delete_
  end interface
  public :: pop


  !> Retains a fixed number of elements in the stack
  !!
  !! Retains a number of elements from either the top or the
  !! bottom of the stack.
  !!
  !! If any of the counts are larger than the currently
  !! number of stored elements, nothing will happen.
  !!
  !! @note
  !! Defaults to `top=fnelem(this)-1`
  interface retain
    module procedure retain_
  end interface
  public :: retain
  
  !> Retrieve specific element from the stack
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be un-initialized upon return.
  interface element
    module procedure get_elem_
  end interface
  public :: element
  
  !> Retrieve pointer to specific element in the stack
  !!
  !! Retrieving a pointer does not change the reference status
  !! of the @bud.
  !!
  !! If the requested element is beyond the number of
  !! stored elements `elem` will be null.
  !!
  interface element_p
    module procedure get_elemp_
  end interface
  public :: element_p

  !> Query number of currently stored elements in this stack
  interface size
    module procedure size_elem_
  end interface
  public :: size
  
  !> Query maximum number of allowed elements in this stack
  interface size_max
    module procedure size_max_elem_
  end interface
  public :: size_max

  
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  interface print
    module procedure print_
  end interface
  public :: print
  
  ! all default reference counted stuff
# include "bud_common.inc"

  !> @cond BUD_DEVELOPER

  pure subroutine common_delete_(D)
    type(BUD_TYPE_NAME_), intent(inout) :: D
    
    if ( associated(D%T) ) then
      
      ! Deallocate array
      call delete(D%T)
      deallocate(D%T)
      nullify(D%T)
      
    end if
    
    D%nel_max = 0
    D%nel = 0
    
  end subroutine common_delete_

  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full copied data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to
    integer :: i
    type(BUD_STACK_TYPE), pointer :: p
    type(BUD_STACK_TYPE) :: cp
    
    call delete(to)
    if ( .not. is_initd(from) ) return

    call new(to, from%D%nel_max)

    do i = 1 , size(from)
      p => element_p(from, i)
      call copy(p, cp)
      call push(to, cp)
      call delete(cp)
    end do
    
    ! Copy the common data-content
    call common_copy_(from, to)
    
  end subroutine copy_

  !> @param[inout] this initialize the finite stack
  !! @param[in] nel maximum number of elements that may be contained in the stack
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

    call initialize(this)

    this%D%nel_max = nel
    this%D%nel = 0
    
    ! prepare data locations
    allocate(this%D%T(nel))

    call tag_object_(this)
    
  end subroutine new_


  !> @param[inout] this stack that gets added an element
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


  !> @param[inout] this stack that gets removed an item
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
  
  
  !> @param[inout] this stack that gets removed an item
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


  !> @param[inout] this stack to operate on
  !! @param[in] top @opt=1 number of elements kept from the top
  !! @param[in] bot @opt=@null number of elements kept from the bottom,
  !!                only used if `top` is not supplied.
  subroutine retain_(this, top, bot)
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

      ! retain 'n' items from the bottom
      lnpop = el - bot

      ! we should not pop anything 
      if ( lnpop < 0 ) return

      call pop(this, top=lnpop)
      
    end if
    
  end subroutine retain_

  
  !> @param[in] this stack to retrieve element from
  !! @param[inout] elem returned item stored in element `iel`
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

  !> @param[in] this stack to retrieve pointer element from
  !! @param[in] iel @opt=`size(this)` index of element to return. May be negative to offset from top.
  !! @result pointer to the element `iel`
  function get_elemp_(this, iel) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: iel
    type(BUD_STACK_TYPE), pointer :: p
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
      
  end function get_elemp_


  !> @param[inout] this stack to count element from
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
    
  
  !> @param[inout] this stack that is queried maximum size of
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

  
  !> @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer :: i
    
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
    write(*,fmt) " <stack-refs: ", references(this), ">>"

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
