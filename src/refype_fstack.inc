  ! @@LICENSE@@ see Copyright notice in the top-directory
  
  ! Include the common utilities
#include "refype_utils.inc"

#define _R_MOD_NAME_STR _R_XSTR(_R_MOD_NAME)
#ifndef _R_TYPE_NAME_
#  define _R_TYPE_NAME_ _R_CC2(_R_TYPE_NAME,_)
#endif
#ifndef _R_TYPE_NAME_STR
#  define _R_TYPE_NAME_STR _R_STR(_R_TYPE_NAME)
#endif

#include "refype_common_declarations.inc"

  !> @defgroup refype-fstack Reference type of a finite stack
  !!
  !! @details
  !! The finite stack implementation is an extension of the regular
  !! reference counted objects.
  !!
  !! A stack is a "push-pop" sequence of values.
  !! A finite stack pushes new values at the top, if the stack is
  !! full the oldest entry is deleted and removed from the stack.
  !! A pop of a value removes the entry from the top of the stack.
  !!
  !! @note
  !! A type need not have _all_ these routines available if so desired.
  !! This is internally controlled and is mentioned for the relevant types.
  !!
  !! @dev-note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - __R_TYPE_NAME of the type
  !!  - __R_TYPE_NAME_, internal data pointer of the type
  !!  - __R_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - __R_TYPE_NEW, the routine name for creating a new type
  !!  - __R_STACK_TYPE, the variable type contained in the stack
  !!
  !! @acknowledge
  !! These declarations are community inspired from the FLIBS
  !! project by Arjen Markus as well as the PyF95 project.
  !! The initial draft of this library has been created by Alberto Garcia.

  type _R_TYPE_NAME_
    ! Consistent data in the reference counted object
#   include "refype_common_type.inc"
    
    ! allowed size of the stack
    integer :: nel_max
    ! current number of values stored
    integer :: nel
    ! Stack 
    type(_R_STACK_TYPE), pointer :: T(:) => null()
  end type _R_TYPE_NAME_

  ! public data type
  type _R_TYPE_NAME
    type(_R_TYPE_NAME_), pointer :: data => null()
  end type _R_TYPE_NAME

  ! special counter for deleting all elements
  ! of a stack using:
  !   call pop(<this>, RYP_FSTACK_ALL)
  integer, public, parameter :: RYP_FSTACK_ALL = 0

  public :: new, _R_TYPE_NEW
  public :: print
  public :: push, pop
  ! For finite stacks one may pop from behind easily
  public :: keep 
  public :: elem, elemp
  public :: nelem, fnelem, size
  public :: nelem_max, fnelem_max, size_max

  interface new
    module procedure new_
  end interface new
  interface _R_TYPE_NEW
    module procedure new_
  end interface _R_TYPE_NEW
  
  interface push
    module procedure push_
  end interface push
  interface pop
    module procedure pop_
    module procedure pop_delete_
  end interface pop
  interface keep
    module procedure keep_
  end interface keep

  interface elem
    module procedure elem_
  end interface elem
  interface elemp
    module procedure elemp_
  end interface elemp

  interface nelem
    module procedure nelem_
  end interface nelem
  interface fnelem
    module procedure fnelem_
  end interface fnelem
  interface size
    module procedure fnelem_
  end interface size
  
  interface nelem_max
    module procedure nelem_max_
  end interface nelem_max
  interface fnelem_max
    module procedure fnelem_max_
  end interface fnelem_max
  interface size_max
    module procedure fnelem_max_
  end interface size_max

  interface print
    module procedure print_
  end interface print

  ! all default reference counted stuff
# include "refype_common.inc"
  

  pure subroutine delete_data(data)
    type(_R_TYPE_NAME_), intent(inout) :: data
    if ( associated(data%T) ) then
      ! Deallocate array
      call delete(data%T)
      deallocate(data%T)
      nullify(data%T)
    end if
    data%nel_max = 0
    data%nel = 0
  end subroutine delete_data


  !> Internal initialization of finite stack holding _R_STACK_TYPE
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this Type that is initialized
  !! @param[in] nel maximum number of elements that may be contained
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine new_(this, nel)
    type(_R_TYPE_NAME), intent(inout) :: this
    integer, intent(in) :: nel

    if ( nel < 1 ) then
#ifdef REFYPE_ERROR
      call REFYPE_ERROR("Finite stack allocated with <1 maximum elements.")
#endif
      call delete(this)
      return
    end if

    call init(this)

    this%data%nel_max = nel
    this%data%nel = 0
    ! prepare data locations
    allocate(this%data%T(nel))

    call tag_object_(this)
    
  end subroutine new_


  elemental subroutine nelem_(this, nel)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(out) :: nel
    if ( .not. is_initd(this) ) then
      nel = 0
    else
      nel = this%data%nel
    end if
  end subroutine nelem_
  elemental function fnelem_(this) result (nel)
    type(_R_TYPE_NAME), intent(in) :: this
    integer :: nel
    call nelem(this, nel)
  end function fnelem_
    

  elemental subroutine nelem_max_(this, nel_max)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(out) :: nel_max
    if ( .not. is_initd(this) ) then
      nel_max = 0
    else
      nel_max = this%data%nel_max
    end if
  end subroutine nelem_max_
  elemental function fnelem_max_(this) result (nel_max)
    type(_R_TYPE_NAME), intent(in) :: this
    integer :: nel_max
    call nelem_max(this, nel_max)
  end function fnelem_max_

  
  subroutine push_(this,item)
    type(_R_TYPE_NAME), intent(inout) :: this
    type(_R_STACK_TYPE), intent(in) :: item

    integer :: el

    ! if this has not been initialised we immediately return
    if ( .not. is_initd(this) ) return

    el = size(this)
    
    if ( el == size_max(this) ) then
      
      ! cycle stack and push
      this%data%T = cshift(this%data%T,+1)
      call delete(this%data%T(el))
      this%data%T(el) = item
    else
      el = el + 1
      ! simply add it to the stack
      this%data%nel = el
      this%data%T(el) = item
    end if
    
  end subroutine push_


  !> Pop and return the latest item in the stack
  !!
  !! The top item is returned in `item` and the saved stack
  !! is decremented thus removing the returned item from the stack.
  !!
  !! @param[inout] this Type that is initialized
  !! @param[inout] item returned type from the top of the stack
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine pop_(this, item)
    type(_R_TYPE_NAME), intent(inout) :: this
    type(_R_STACK_TYPE), intent(inout) :: item

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
    item = this%data%T(el)

    call pop(this)
    
  end subroutine pop_
  
  
  !> Pop the latest item(s) in the stack
  !!
  !! Pops any _n_ items from the top of the stack without returning
  !! any of them.
  !! This defaults to only popping the top element.
  !!
  !! @param[inout] this stack type from which elements are popped
  !! @param[inout] top @opt=1 number of popped elements from top.
  !!                   If RYP_FSTACK_ALL is issued it will
  !!                   clear all elements.
  !! @param[inout] bot @opt=<none> has precedence if `top` not supplied.
  !!                   Pops `bot` elements from the bottom of the stack.
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine pop_delete_(this, top, bot)
    type(_R_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: top, bot

    integer :: lnpop
    integer :: el
    
    if ( .not. is_initd(this) ) return
    
    ! number of elements
    el = size(this)

    if ( present(top) .or. .not. present(bot) ) then
      
      ! regular top-popping
      lnpop = 1
    
      if ( present(top) ) lnpop = top
      
      ! RYP_FSTACK_ALL == 0, we delete all
      if ( lnpop == RYP_FSTACK_ALL ) lnpop = el

      ! we should not pop anything 
      if ( lnpop < 0 ) return
      
      do while ( lnpop > 0 )
        
        ! easy case if it is empty
        if ( 0 == el ) return
        
        ! Delete the local item and decrease counter
        call delete(this%data%T(el))
        this%data%nel = this%data%nel - 1
        
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
        call delete(this%data%T(1))
        this%data%nel = this%data%nel - 1
        ! shift to attain the correct order
        this%data%T = cshift(this%data%T,-1)
        
        el = el - 1
        lnpop = lnpop - 1
      end do

    end if
    
  end subroutine pop_delete_

  !> Keeps a fixed number of elements in the stack
  !!
  !! Retains a number of elements from either the top or the
  !! bottom of the stack (top has precedence).
  !!
  !! This defaults to only popping the bottom element.
  !!
  !! @param[inout] this stack type from which elements are popped
  !! @param[inout] top @opt=1 number of elements kept from the top
  !! @param[inout] bot @opt=<none> number of elements kept from the bottom,
  !!                   only used if `top` is not supplied.
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine keep_(this, top, bot)
    type(_R_TYPE_NAME), intent(inout) :: this
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

  subroutine elem_(this,el,val)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(in) :: el
    type(_R_STACK_TYPE), intent(inout) :: val

    call delete(val)
    if ( .not. is_initd(this) ) return

    if ( el <= size(this) ) then
      val = this%data%T(el)
    end if
    
  end subroutine elem_

  subroutine elemp_(this,el,p)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(in) :: el
    type(_R_STACK_TYPE), pointer :: p

    nullify(p)
    if ( .not. is_initd(this) ) return

    if ( el <= size(this) ) then
      p => this%data%T(el)
    end if
    
  end subroutine elemp_

  
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=_R_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  !!
  !! @author Nick R. Papior
  !! @author 2016, February
  subroutine print_(this, info, indent)
    type(_R_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    integer :: i, nel

    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name
    
    name = _R_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent

    write(fmt, '(a,i0,a)') '(t',indent,',2a)'
    
    if ( .not. is_initd(this) ) then
      write(*,fmt) trim(name), " not initialized."
      return
    end if

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<", trim(name), " stack:>"
    do i = 1 , size(this)
      call print(this%data%T(i), indent = lindent)
    end do
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <stack-refs: ", frefs(this), ">"

  end subroutine print_

#undef _R_MOD_NAME
#undef _R_TYPE_NAME
#undef _R_TYPE_NAME_
#undef _R_TYPE_NAME_STR
#undef _R_TYPE_NEW
#undef _R_STACK_TYPE

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

