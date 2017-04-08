  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_TYPE_NEW, the routine name for creating a new type
  !  - BUD_LIST_TYPE, the variable type contained in the linked list
  !  - BUD_LIST_DOUBLY, if /= 0 the linked list is doubly connected
  !  - BUD_LINKED_LIST_NAME, the name of the self-contained linked list (not required to be set)

  ! Include the common utilities
#include "bud_utils.inc"

  ! We default the linked list to be a doubly
  ! linked list
#ifndef BUD_LIST_DOUBLY
# define BUD_LIST_DOUBLY 1
#endif

#ifndef BUD_LINKED_LIST_NAME
# define BUD_LINKED_LIST_NAME BUD_CC2(BUD_TYPE_NAME_,LList)
#endif

  ! Create internal flag
#if BUD_LIST_DOUBLY != 1
# error "Currently only doubly linked lists are implemented"
#endif

  !> @cond BUD_DOC_INC

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

  !> @endcond BUD_DOC_INC

  !> Linked list with each node containing BUD_LIST_TYPE
  !!
#ifdef BUD_LIST_DOUBLY
  !! This data container holds 3 variables (doubly linked list):
  !!  1. The data of the current item
  !!  2. The next target
  !!  3. The previous target
#else
  !! This data container holds 2 variables (single linked list):
  !!  1. The data of the current item
  !!  2. The next target
#endif
  !!
  !! The data in each node does not necessarily require to be
  !! populated and they can as such be used as "empty" nodes.
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    !> @name Private procedures
    !> @{
    ! Doxygen needed line

    procedure, private :: new_
    procedure, private :: new_data_

    procedure, private :: set_data_
    procedure, private :: set_data_idx_

    procedure, private :: add_node_
    procedure, private :: add_node_data_

    procedure, private :: itt_step_
    procedure, private :: itt_step_i_

    !>>@}

    !> @iSee #new
    generic, public :: new => new_, new_data_

    !> @iSee #nodes
    procedure, public :: nodes => size_llist_
    !> @iSee #nodes
    procedure, public :: size => size_llist_

    !> @iSee #set_data
    generic, public :: set_data => set_data_, set_data_idx_
    !> @iSee #set_data
    generic, public :: set => set_data_, set_data_idx_

    procedure, public :: get_node_data => get_node_data_idx_
    !> @iSee #get_data
    procedure, public :: get_data => get_node_data_idx_
    !> @iSee #get_data
    procedure, public :: get => get_node_data_idx_

    !> @iSee #add_node
    generic, public :: add_node => add_node_, add_node_data_

    !> @iSee #remove_node
    procedure, public :: remove_node => remove_node_
    !> @iSee #remove_node_data
    procedure, public :: remove_node_data => remove_node_data_

    !> @iSee #set_head
    procedure, public :: set_head => set_head_idx_

    !> @iSee #copy
    procedure, public :: copy_list => copy_

    !> @iSee #itt_step
    generic, public :: itt_step => itt_step_, itt_step_i_

#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DOC_INC

  !> Linked list container for BUD_TYPE_NAME
  !!
  !! The actual data container for the linked list.
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

#ifdef BUD_LIST_DOUBLY
    !> Previous node in the linked list
    type(BUD_LINKED_LIST_NAME), pointer :: prev => null()
#endif

    !> Next node in the linked list
    type(BUD_LINKED_LIST_NAME), pointer :: next => null()

    !> Contained data in the linked list.
    type(BUD_LIST_TYPE) :: D

  end type
  private :: BUD_LINKED_LIST_NAME

  !> @endcond BUD_DOC_INC

  !> Create a new linked list with 1 node.
  !!
  !! The linked list `head` will be at the initial
  !! node.
  !!
  !! One may later use #set_head to change the
  !! head of the linked list.
  interface new
    module procedure new_
    module procedure new_data_
  end interface
  public :: new


  ! this should probably not be documented
  ! as it is defined twice.
  interface assignment(=)
    module procedure set_data_
  end interface

  !> Query number of currently stored nodes in the linked list
  !!
#ifdef BUD_LIST_DOUBLY
  !! Will count all previous and following elements.
#else
  !! Will only count elements in front of the element.
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


  !> Overwrite/Set data at an element in the list
  !!
  !! Assign the data at the index or head of the
  !! linked list.
  interface set_data
    module procedure set_data_
    module procedure set_data_idx_
  end interface
  public :: set_data


  !> Add a new node to the end of the linked list
  !!
  !! Possibly assign data to the new node.
  !!
  !! One cannot add elements in the middle of the stack.
  !!
#ifdef BUD_LIST_DOUBLY
  !! A doubly linked list may create circular lists
  !! which is a valid linked list.
  !! Adding a node on a circular list will add the
  !! node right before the `head`.
#endif
  interface add_node
    module procedure add_node_
    module procedure add_node_data_
  end interface
  public :: add_node


  !> Query data from node in the linked list
  !!
  !! Retrieve the contained `BUD_LIST_TYPE` via index specification,
  !! or the `head` if the index is not specified.
  !!
  !! @note
#ifdef BUD_LIST_DOUBLY
  !! If the index is negative it will correspond to the
  !! previous elements.
  !! If the linked list is circular the node will _always_
  !! be found and will correspond to the element `mod(idx, nodes(this))`.
  !! For non-circular list specifying an index outside the previous/next
  !! number of nodes will return an un-initialized `BUD_LIST_TYPE`.
#else
  !! If the requested index is beyond the number of
  !! stored nodes the `BUD_LIST_TYPE` will be un-initialized.
#endif
  interface get_data
    module procedure get_node_data_idx_
  end interface
  public :: get_data

  !> @iSee get_data
  interface get_node
    module procedure get_node_data_idx_
  end interface
  public :: get_node


  !> Set the head of the linked list to the indexed node
  !!
  !! This will change the head of the linked list.
  !!
  !! If you want to preserve two linked lists with different heads
  !! the linked list _has_ to be copied (#copy) before the head may
  !! be changed.
  interface set_head
    module procedure set_head_idx_
  end interface
  public :: set_head


  !> Itterator Forward iterator for a linked list from the head
  !!
  !! Will step an iterator that will be connected to a
  !! linked list.
  interface itt_step
    module procedure itt_step_
    module procedure itt_step_i_
  end interface
  public :: itt_step


  !> Remove a specific node from the linked list
  !!
  !! This reorders the linked list in case
  !! the deleted node is not the last node.
  !!
  !! I.e. the node is removed entirely from the list.
  interface remove_node
    module procedure remove_node_
  end interface
  public :: remove_node

  !> Remove data contained at a specific node
  !!
  !! The node is preserved in the linked list.
  !!
  !! I.e. _only_ the data is deleted, not the node.
  interface remove_node_data
    module procedure remove_node_data_
  end interface
  public :: remove_node_data

  !> Copy a linked list to another linked list
  !!
  !! This will assign the node data to the new
  !! linked list nodes.
  interface copy_list
    module procedure copy_
  end interface
  public :: copy_list


  ! all default reference counted stuff
# include "bud_common.inc"


  !> @cond BUD_DEVELOPER

  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LINKED_LIST_NAME), pointer :: head, tmp

    ! We first distinguish between an iterator
    ! and a proper linked list
    if ( associated(this%D%head) .and. .not. &
      associated(this%D%ll) ) then
      ! we have an iterator
      nullify(this%D%head)
      nullify(this%D%ll)

      return
    end if


    ! First move
    head => this%D%ll

    ! If the linked list is not associated we
    ! need not do anything
    if ( associated(head) ) then

#ifdef BUD_LIST_DOUBLY
      do while ( associated(head%prev) )
        if ( associated(head%prev, this%D%ll) ) exit
        head => head%prev
      end do
      ! we have a circular list. Hence
      ! the head is the tip
      if ( associated(head%prev, this%D%ll) ) then
        head => this%D%ll
      end if
#endif

      ! Perform deletion
      head => head%next
      tmp => head
      do while ( associated(head) )
        ! circular list
        if ( associated(head%next, this%D%ll) ) exit
        head => head%next
        call delete(tmp%D)
        deallocate(tmp)
        tmp => head
      end do

    end if

    ! deallocate head of list
    if ( associated(this%D%ll) ) then
      call delete(this%D%ll%D)
      deallocate(this%D%ll)
    end if

    ! nullify
    nullify(this%D%ll)
    nullify(this%D%head)

  end subroutine delete_

  !> @endcond BUD_DEVELOPER



  !> @param[inout] this initialized linked list upon exit
  subroutine new_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    call initialize(this)

  end subroutine new_

  !! @param[inout] this new @bud upon exit
  !! @param[in] D stored data in the first (head) node
  subroutine new_data_(this, D)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D

    call new(this)

    allocate(this%D%ll)
    this%D%head => this%D%ll
    this%D%head%D = D

  end subroutine new_data_

  !> @cond BUD_DEVELOPER

  subroutine get_ll_headp(this, head)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_LINKED_LIST_NAME), pointer :: head

    if ( .not. is_initd(this) ) then
      nullify(head)
      return
    end if

    head => this%D%head
#ifdef BUD_LIST_DOUBLY
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

#ifdef BUD_LIST_DOUBLY
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
#ifdef BUD_LIST_DOUBLY
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

  !> @endcond BUD_DEVELOPER


  !> @param[inout] this linked list that is queried
  !! @return number of nodes in the list
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


  !> @param[inout] this add a new element to the linked list
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


  !> @param[inout] this add a new element to the linked list
  !! @param[in] D stored data at the new node
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


  !> @param[inout] this linked list item where the data is overwritten
  !! @param[in] D item to be stored in the head of `this`
  subroutine set_data_(this, D)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_LIST_TYPE), intent(in) :: D

    if ( is_initd(this) ) then
      this%D%head%D = D
    end if

  end subroutine set_data_

  !> @param[inout] this linked list item where the data is overwritten
  !! @param[in] D item to be stored in the head of `this`
  !! @param[in] index index of element in linked list to be stored
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


  !> @param[in] this retrieve data from this linked list with associated head
  !! @param[inout] D the data stored at the element in the linked list
  !! @param[in] index @opt=`1` index of element to return. May be negative to offset from the head for doubly linked lists
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


  !> @param[in] this the linked list to be iterated
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

  !> @param[in] this the linked list to be iterated
  !! @param[inout] itt the itterator container
  !! @param[in] i the number of elements that should be stepped (the first iteration will _always_ be the head)
  !! @return .true. if the next element can be reached
  function itt_step_i_(this, itt, i) result(itterated)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: itt
    integer, intent(in) :: i
    logical :: itterated
    integer :: is

    itterated = .false.
    if ( .not. is_initd(this) ) then
      call delete(itt)
      return
    end if

    is = 0

    ! check whether the itterator is currently
    ! being used
    if ( .not. is_initd(itt) ) then

      ! we are starting a new itterator
      call new(itt)

      itt%D%head => this%D%head

      itterated = associated(itt%D%head)

    else if ( i > 0 ) then

      do while ( is /= i )
        is = is + 1
        itt%D%head => itt%D%head%next

        itterated = associated(itt%D%head)

        if ( itterated ) then
          itterated = .not. associated(itt%D%head, this%D%head)
        end if
        if ( .not. itterated ) exit

      end do

    else if ( i < 0 ) then

#ifdef BUD_LIST_DOUBLY
      do while ( is /= i )
        is = is - 1

        itt%D%head => itt%D%head%prev

        itterated = associated(itt%D%head)

        if ( itterated ) then
          itterated = .not. associated(itt%D%head, this%D%head)
        end if
        if ( .not. itterated ) exit

      end do
#endif

    end if

    if ( .not. itterated ) then

      ! be sure to not accidentially delete the
      ! looped construct
      nullify(itt%D%head)

      ! we are at the end of the linked list
      call delete(itt)

    end if

  end function itt_step_i_


  !> @param[inout] this linked list which should get a node removed
  !> @param[in] index the index of the node that should be removed from
  !!            the linked list
  subroutine remove_node_(this, index)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout), target :: this
    integer, intent(in) :: index
    type(BUD_LINKED_LIST_NAME), pointer :: node, pnode, nnode

    if ( .not. is_initd(this) ) return

#ifdef BUD_LIST_DOUBLY
    ! For a doubly list this procedure is very easy

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

  !> @param[inout] this the linked list which should get the data in a node removed
  !> @param[in] index the index of the node where the contained data should
  !!            be removed
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


  !> @param[inout] from the linked list that is the original list
  !! @param[inout] to a copy of `from` upon exit
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: from, to
    type(BUD_LINKED_LIST_NAME), pointer :: t, f

    call delete(to)
    if ( .not. is_initd(from) ) return

    call new(to)

    ! First create the previous track
    f => from%D%head
    t => to%D%head

    t%D = f%D
    do while ( associated(f%next) )
      f => f%next

      allocate(t%next)
#ifdef BUD_LIST_DOUBLY
      t%next%prev => t
#endif
      t => t%next
      t%D = f%D

      ! check for circular list
#ifdef BUD_LIST_DOUBLY
      ! In this case we have already progressed
      ! the entire list
      ! Hence we should return and NOT perform
      ! an additional copy.
#endif
      if ( associated(t, to%D%head) ) return

    end do

#ifdef BUD_LIST_DOUBLY
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

  end subroutine copy_


#ifndef BUD_NO_IO

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be written
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    logical :: formatted
    integer :: iu
    integer :: nel, nel_max, i

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    formatted = is_formatted(f)
    iu = unit(f)

    ! Get information about this stack
    nel = size(this)
    nel_max = size_max(this)

    if ( formatted ) then
      write(iu, '(i16)') nel_max, nel
    else
      write(iu) nel_max, nel
    end if

    ! Now write each of them...
    do i = 1 , nel
      call write(f, this%D%T(i))
    end do

  end subroutine write_

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be readed
  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    ! To read each of them individually
    type(BUD_LIST_TYPE) :: cp

    logical :: formatted
    integer :: iu
    integer :: nel, nel_max, i

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    formatted = is_formatted(f)
    iu = unit(f)

    ! Get information about this stack
    if ( formatted ) then
      read(iu, '(i16)') nel_max, nel
    else
      read(iu) nel_max, nel
    end if

    ! Initialize this object
    call new(this, nel_max)

    ! Now write each of them...
    do i = 1 , nel
      call read(f, cp)
      call push(this, cp)
      call delete(cp)
    end do

  end subroutine read_

#endif

  !> @param[in] this linked list
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in), target :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
    type(BUD_LINKED_LIST_NAME), pointer :: node

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
    write(*,fmt) " <llist-refs: ", references(this), ">>"

  end subroutine print_

#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_LIST_TYPE
#undef BUD_LINKED_LIST_NAME
#undef BUD_LIST_DOUBLY


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

