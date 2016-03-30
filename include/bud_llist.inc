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

  ! We default the linked list to be a doubly
  ! linked list
#ifndef BUD_LIST_SINGLY_DOUBLY
# define BUD_LIST_SINGLY_DOUBLY 2
#endif

#ifndef BUD_LINKED_LIST_NAME
# define BUD_LINKED_LIST_NAME BUD_CC2(BUD_TYPE_NAME_,LList)
#endif

  ! Create internal flag
#if BUD_LIST_SINGLY_DOUBLY == 2
# define BUD_LIST_IS_DOUBLY
#else
# undef BUD_LIST_IS_DOUBLY
# error "Currently single linked lists are not implemented"
#endif

  !> @defgroup buds-llist Linked list with reference counting
  !!
  !! @details
  !! The linked list implementation is an extension of the regular
  !! reference counted objects.
  !!
  !! A linked list is a list of entries tied together in a sequence.
  !! It has unlimited size and access is O(n).
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
  !!  - _BUD_LIST_TYPE, the variable type contained in the linked list
  !!  - _BUD_LINKED_LIST_NAME, the name of the self-contained linked list


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
    generic, public :: new => new_, new_data_

    procedure, public :: nodes => size_llist_
    procedure, public :: size => size_llist_

    procedure, private :: set_data_
    procedure, private :: set_data_idx_
    generic, public :: set => set_data_, set_data_idx_
    generic, public :: set_data => set_data_, set_data_idx_

    procedure, public :: get_node_data => get_node_data_idx_
    procedure, public :: get_data => get_node_data_idx_

    procedure, private :: add_node_
    procedure, private :: add_node_data_
    generic, public :: add_node => add_node_, add_node_data_

    procedure, public :: remove_node => remove_node_
    procedure, public :: remove_node_data => remove_node_data_

    procedure, public :: set_head => set_head_idx_

    procedure, public :: copy_list => copy_list_
    procedure, public :: copy => copy_list_

    procedure, public :: itt_step => itt_step_
    
#endif
  end type BUD_TYPE_NAME

  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @bud.
  !!
#ifdef BUD_LIST_IS_DOUBLY
  !! This data container holds 3 variables,
  !!  1. The data of the current item
  !!  2. The next target
  !!  3. The previous target
#else
  !! This data container holds 2 variables,
  !!  1. The data of the current item
  !!  2. The next target
#endif
  !!
  !! @dev-note
  !! There are various ways to implement a linked-list
  !! using the reference counting scheme.
  !! Here we use the reference counting scheme to
  !! retain the list in a linked way with the object it self.
  !! This is instead of using a direct pointer linked list
  !! implementation which will have difficulties when deallocating
  !! the object.
  !! Only by having *clean* data-types can we delete nodes without
  !! worrying about post/pre-deallocation.
  type BUD_TYPE_NAME_

    !> The linked list
    type(BUD_LINKED_LIST_NAME), pointer :: ll => null()

    !> The head of the linked list
    type(BUD_LINKED_LIST_NAME), pointer :: head => null()

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  type BUD_LINKED_LIST_NAME

#ifdef BUD_LIST_IS_DOUBLY
    !> Previous node in the linked list
    type(BUD_LINKED_LIST_NAME), pointer :: prev => null()
#endif

    !> Next node in the linked list
    type(BUD_LINKED_LIST_NAME), pointer :: next => null()

    !> Contained data in the linked list.
    type(BUD_LIST_TYPE) :: D

  end type
  private :: BUD_LINKED_LIST_NAME


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

  !> Return number of nodes in the list
#ifndef BUD_LIST_IS_DOUBLY
  !!
  !! In a single-linked list this may return
  !! different sizes dependent on where the list
  !! is counted.
#endif
  interface nodes
    module procedure size_llist_
  end interface
  public :: nodes

  !> @iSee nodes
  interface size
    module procedure size_llist_
  end interface
  public :: size


  ! this should probably not be documented
  ! as it is defined twice.
  interface assignment(=)
    module procedure set_data_
  end interface

  !> Set data for a node
  interface set_data
    module procedure set_data_
    module procedure set_data_idx_
  end interface
  public :: set_data

  
  !> Add a new element to the head of the linked list
  !!
  !! @iSee add
  !!
  !! @note
  !! One cannot push elements in the middle of the stack.
  interface add_node
    module procedure add_node_
    module procedure add_node_data_
  end interface
  public :: add_node
  

  !> Retrieve the data/node from the linked list
  interface get_node
    module procedure get_node_data_idx_
  end interface
  public :: get_node

  !> Retrieve the data/node from the linked list
  interface get_data
    module procedure get_node_data_idx_
  end interface
  public :: get_data


  !> Set the head of the linked list to the indexed node
  interface set_head
    module procedure set_head_idx_
  end interface
  public :: set_head


  !> Set the head of the linked list to the indexed node
  interface itt_step
    module procedure itt_step_
  end interface
  public :: itt_step


  !> Remove a specific node from the linked list
  !!
  !! This reorders the linked list in case
  !! the deleted node is not the last node.
  interface remove_node
    module procedure remove_node_
  end interface
  public :: remove_node

  !> Remove data contained at a specific node
  !!
  !! The node is preserved in the linked list.
  interface remove_node_data
    module procedure remove_node_data_
  end interface
  public :: remove_node_data

  !> Deep-copy a list to another list
  interface copy_list
    module procedure copy_list_
  end interface
  public :: copy_list

  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print
  
  ! all default reference counted stuff
# include "bud_common.inc"
  
  pure subroutine delete_data(D)
    type(BUD_TYPE_NAME_), intent(inout) :: D
    type(BUD_LINKED_LIST_NAME), pointer :: head, tmp

    ! We first distinguish between an iterator
    ! and a proper linked list
    if ( associated(D%head) .and. .not. &

      associated(D%ll) ) then
      ! we have an iterator
      nullify(D%head)
      nullify(D%ll)

      return
    end if


    ! First move
    head => D%ll

    ! If the linked list is not associated we
    ! need not do anything
    if ( associated(head) ) then
      
#ifdef BUD_LIST_IS_DOUBLY
      do while ( associated(head%prev) )
        if ( associated(head%prev, D%ll) ) exit
        head => head%prev
      end do
      ! we have a circular list. Hence
      ! the head is the tip
      if ( associated(head%prev, D%ll) ) then
        head => D%ll
      end if
#endif

      ! Perform deletion
      head => head%next
      tmp => head
      do while ( associated(head) )
        ! circular list
        if ( associated(head%next, D%ll) ) exit
        head => head%next
        call delete(tmp%D)
        deallocate(tmp)
        tmp => head
      end do

    end if
    
    ! deallocate head of list
    if ( associated(D%ll) ) then
      call delete(D%ll%D)
      deallocate(D%ll)
    end if

    ! nullify
    nullify(D%ll)
    nullify(D%head)

  end subroutine delete_data


  !> Internal initialization of linked list BUD_LIST_TYPE
  !!
  !! @param[inout] this new @bud upon exit
  subroutine new_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    call initialize(this)

    call tag_object_(this)
    
  end subroutine new_

  !> Internal initialization of linked list BUD_LIST_TYPE
  !!
  !! @param[inout] this new @bud upon exit
  !! @param[in] D stored data in the first node
  subroutine new_data_(this, D)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D

    call new(this)

    allocate(this%D%ll)
    this%D%head => this%D%ll
    this%D%head%D = D
    
  end subroutine new_data_

  ! ********
  !   Internal routines
  subroutine get_ll_headp(this, head)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_LINKED_LIST_NAME), pointer :: head

    if ( .not. is_initd(this) ) then
      nullify(head)
      return
    end if

    head => this%D%head
#ifdef BUD_LIST_IS_DOUBLY
    do while ( associated(head%prev) )
      head => head%prev
      if ( associated(head, this%D%head) ) exit
    end do
#endif

  end subroutine get_ll_headp
  
  subroutine get_ll_tailp(this, tail)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_LINKED_LIST_NAME), pointer :: tail
    
    if ( .not. is_initd(this) ) then
      nullify(tail)
      return
    end if

    tail => this%D%head
    do while ( associated(tail%next) )
      tail => tail%next
      if ( associated(tail, this%D%head) ) exit
    end do

  end subroutine get_ll_tailp


  !> Query the node at `index` away from the `head`.
  !!
  !! If the requested index is beyond the number of
  !! stored nodes `node` will be `null()` upon return.
  !!
  !! @param[in] this @bud to retrieve node from
  !! @param[inout] node returned @bud in node `index`
  !! @param[in] index @opt=`0` index of element to return. May be negative to offset from the head
  subroutine get_ll_idxp(this, node, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(in), target :: this
    type(BUD_LINKED_LIST_NAME), pointer :: node
    integer, intent(in), optional :: index

    integer :: lindex

    nullify(node)
    if ( .not. is_initd(this) ) return

    lindex = 0
    if ( present(index) ) lindex = index

    node => this%D%head

#ifdef BUD_LIST_IS_DOUBLY
    do while ( lindex < 0 .and. associated(node%prev) )
      node => node%prev
      lindex = lindex + 1
    end do
#else
    if ( lindex < 0 ) return
#endif
    
    do while ( lindex > 0 .and. associated(node%next) )
      node => node%next
      lindex = lindex - 1
    end do

    ! not found element
    if ( lindex /= 0 ) then
      nullify(node)
      return
    end if
    
  end subroutine get_ll_idxp

  ! Create a new node at this tip
  subroutine append_node(ll)
    type(BUD_LINKED_LIST_NAME), pointer :: ll

    ! the new node
    type(BUD_LINKED_LIST_NAME), pointer :: tmp => null()

    if ( .not. associated(ll) ) return

    allocate(tmp)

    ! possible forward tracking
    tmp%next => ll%next

    ! instantiate pointers
#ifdef BUD_LIST_IS_DOUBLY
    ! setup previous links
    tmp%prev => ll
    if ( associated(ll%next) ) then
      ll%next%prev => tmp
    end if
#endif
    ! point to the new node
    ll%next => tmp

    nullify(tmp)

  end subroutine append_node
    


  ! ********
  
  

  !> Query number of currently stored elements in this list
  !!
#ifdef BUD_LIST_IS_DOUBLY
  !! Will count all previous and following elements.
#endif
  !!
  !! @param[inout] this @bud that is checked
  !! @return number of currently nodes
  function size_llist_(this) result (nnodes)
    BUD_CLASS(BUD_TYPE_NAME), intent(in), target :: this
    integer :: nnodes
    type(BUD_LINKED_LIST_NAME), pointer :: head, tmp
    
    nnodes = 0

    if ( .not. is_initd(this) ) return

    ! Retrieve a pointer
    call get_ll_headp(this, head)
    
    ! Start counting from the top node
    tmp => head
    do while ( associated(tmp) )
      ! step size of list
      nnodes = nnodes + 1

      tmp => tmp%next
      if ( associated(tmp, head) ) exit

    end do

  end function size_llist_

    
  !> Add an empty node to the end of the linked list
  !!
  !! @param[inout] this @bud that is added to
  subroutine add_node_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LINKED_LIST_NAME), pointer :: tail

    if ( .not. is_initd(this) ) then

      ! simply create a new "empty" node
      ! and thus a linked-list with one element
      call new(this)

      ! Create node
      allocate(this%D%ll)
      this%D%head => this%D%ll
      
      return
      
    end if

    ! get current head from this head
    call get_ll_tailp(this, tail)

    ! create new node
    call append_node(tail)
    
  end subroutine add_node_

  
  !> Add new node with to the end of the linked list
  !!
  !! @param[inout] this @bud that is added to
  !! @param[in] item node added to the stack
  subroutine add_node_data_(this, D)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D
    type(BUD_LINKED_LIST_NAME), pointer :: tail

    call get_ll_tailp(this, tail)
    if ( .not. associated(tail) ) then
      call new(this, D)
      return
    end if
    call append_node(tail)
    tail%next%D = D
    
  end subroutine add_node_data_


  !> Attach BUD_LIST_TYPE to current node
  subroutine set_data_(this, D)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D
    
    if ( is_initd(this) ) then
      this%D%head%D = D
    end if
    
  end subroutine set_data_

  !> Attach BUD_LIST_TYPE to current node
  subroutine set_data_idx_(this, D, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D
    integer, intent(in) :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node

    call get_ll_idxp(this, node, index)
    if ( .not. associated(node) ) return
    node%D = D
    
  end subroutine set_data_idx_

  !> Set the head of the linked list
  subroutine set_head_idx_(this, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in) :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node

    call get_ll_idxp(this, node, index)
    if ( .not. associated(node) ) return
    this%D%head => node
    
  end subroutine set_head_idx_


  !> Query data from indexed node in the list
  !!
  !! If the requested index is beyond the number of
  !! stored nodes `D` will be un-initialized upon return.
  !!
  !! @param[in] this @bud to retrieve node from
  !! @param[inout] D returned @bud stored in node `i`
  !! @param[in] index @opt=`1` index of element to return. May be negative to offset from the head
  subroutine get_node_data_idx_(this, D, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(in), target :: this
    type(BUD_LIST_TYPE), intent(inout) :: D
    integer, intent(in), optional :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node

    ! ensure the data is empty
    call delete(D)
    if ( .not. is_initd(this) ) return

    call get_ll_idxp(this, node, index)

    if ( associated(node) ) D = node%D

  end subroutine get_node_data_idx_



  !> Forward iterator for a linked list from the head
  !!
  !! Will step an itterator that will be connected to a
  !! linked list.
  !!
  !! @param[in] this the linked list to be iterated
  !! @param[inout] itt the itterator container
  !! @return .true. if the next element can be reached
  function itt_step_(this, itt) result(itterated)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: itt
    logical :: itterated

    itterated = .false.
    if ( .not. is_initd(this) ) then
      call delete(itt)
      return
    end if

    ! check whether the itterator is currently
    ! being used
    if ( .not. is_initd(itt) ) then

      ! we are starting a new itterator
      call new(itt)

      itt%D%head => this%D%head
      
      itterated = associated(itt%D%head)
      
    else

      itt%D%head => itt%D%head%next

      itterated = associated(itt%D%head)

      if ( itterated ) then
        itterated = .not. associated(itt%D%head, this%D%head)
      end if
      
    end if

    if ( .not. itterated ) then
      ! we are at the end of the circular reference
      call delete(itt)
    end if

  end function itt_step_


  !> Delete specific node in a linked list
  !!
  !! The deletion process deletes the node but re-creates
  !! the connections such that the linked list items are
  !! preserved.
  subroutine remove_node_(this, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout), target :: this
    integer, intent(in) :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node, pnode, nnode

    if ( .not. is_initd(this) ) return

#ifdef BUD_LIST_IS_DOUBLY
    ! For a doubly list this procedure is very
    ! easy

    ! Find the node
    call get_ll_idxp(this, node, index)
    ! Quick return if the node does not exist
    if ( .not. associated(node) ) return

    ! retrieve nodes
    pnode => node%prev
    nnode => node%next
    ! reset the current node
    nullify(node%prev)
    nullify(node%next)

    ! re-create correct links
    if ( associated(pnode) ) then
      pnode%next => nnode
    end if
    if ( associated(nnode) ) then
      nnode%prev => pnode
    end if

    call delete(node%D)
    deallocate(node)
#else

#error "MISSING: remove_node for single lists"
    
#endif
    
  end subroutine remove_node_

  !> Delete specific data in node in a linked list
  subroutine remove_node_data_(this, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout), target :: this
    integer, intent(in), optional :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node

    if ( .not. is_initd(this) ) return

    ! Find the node
    call get_ll_idxp(this, node, index)
    if ( .not. associated(node) ) return

    call delete(node%D)

  end subroutine remove_node_data_


  subroutine copy_list_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to, from
    type(BUD_LINKED_LIST_NAME), pointer :: t, f

    if ( .not. is_initd(from) ) then
      call delete(to)
      return
    end if

    call delete(to)
    call new(to)

    ! First create the previous track
    f => from%D%head
    t => to%D%head

    t%D = f%D
    do while ( associated(f%next) )
      f => f%next

      allocate(t%next)
#ifdef BUD_LIST_IS_DOUBLY
      t%next%prev => t
#endif
      t => t%next
      t%D = f%D

      ! check for circular list
#ifdef BUD_LIST_IS_DOUBLY
      ! In this case we have already progressed
      ! the entire list
      ! Hence we should return and NOT perform
      ! an additional copy.
#endif
      if ( associated(t, to%D%head) ) return
      
    end do

#ifdef BUD_LIST_IS_DOUBLY
    f => from%D%head
    t => to%D%head
    do while ( associated(f%prev) )
      f => f%prev

      allocate(t%prev)
      t%prev%next => t
      t => t%prev
      t%D = f%D

    end do
#endif

  end subroutine copy_list_
  
  
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
    integer :: i, nel
    type(BUD_LINKED_LIST_NAME), pointer :: node
    type(BUD_LIST_TYPE) :: empty

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
    write(*,fmt) "<<", trim(name), " llist, nodes=",nodes(this),">"
    lindent = lindent + 2 ! step indentation
    call get_ll_headp(this, node)
    do while ( associated(node) )
      if ( associated(node, this%D%head) ) then
        write(fmt, '(a,i0,a)') '(t',lindent,',a)'
        call print(node%D, indent = lindent+2)
      else
        call print(node%D, indent = lindent)
      end if
      node => node%next
    end do
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <llist-refs: ", get_refs(this), ">>"
    
  end subroutine print_

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_LIST_TYPE
#undef BUD_LIST_SINGLY_DOUBLY
#undef BUD_LIST_IS_DOUBLY
#undef BUD_LINKED_LIST_NAME

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

