  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"
  
  !> This module makes heavy usage of the
  !! MPI variables
  use mpi

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)

  !> BUD_MOD_NAME documentation for various distribution types.
  !!
  !! This @bud implements a selected portion of different
  !! distributions.
  !! Currently the available distributions are:
  !!  - DIST_BLOCK_CYCLIC
  !!    This format of block-cyclic distributions
  !!    has 2 versions, each usable for different aspects:
  !!    * DIST_BLOCK_CYCLIC_FIRST
  !!      any remaining non-associated elements are assigned
  !!      to the first processor.
  !!    * DIST_BLOCK_FIRST
  !!      any remaining non-associated elements are assigned
  !!      to the first processor.
  !!    * DIST_BLOCK_LAST
  !!      any remaining non-associated elements are assigned
  !!      to the last processor.
  !! 
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_MOD_NAME of the module
  !!  - _BUD_TYPE_NAME of the type
  !!  - _BUD_TYPE_NAME_, internal data pointer of the type
  !!  - _BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - _BUD_TYPE_NEW, the routine name for creating a new type
  !!  - _BUD_TYPE_VAR, the variable type contained in the array
  !!  - _BUD_PREC, the precision specifier for the array
  !! If you add any new dependencies be sure to undefine the variable
  !! at the bottom of this file.

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"


  !> Denote no distribution
  integer(ii_), parameter, public :: DIST_NONE = 1

  !> Block-cyclic distribution, remaining goes to first processor
  integer(ii_), parameter, public :: DIST_BLOCK_CYCLIC_FIRST = 2
  !> Block-cyclic distribution, remaining goes to last processor
!  integer(ii_), parameter, public :: DIST_BLOCK_CYCLIC_LAST = 3

  !> Block-cyclic distribution (DIST_BLOCK_CYCLIC_FIRST)
  integer(ii_), parameter, public :: DIST_BLOCK_CYCLIC = DIST_BLOCK_CYCLIC_FIRST

  ! currently this is not enabled as an implementation
  ! is not complete. Hence it is private.
  !> User defined distribution (DIST_EXPLICIT)
  integer(ii_), parameter, private :: DIST_EXPLICIT = 4

  !> A simple block-partitioning (remainder to first process)
  integer(ii_), parameter, public :: DIST_BLOCK_FIRST = 5
  !> A simple block-partitioning (remainder to last process)
  integer(ii_), parameter, public :: DIST_BLOCK_LAST = 6


  !> @bud variable
  !!
  !! Handler for the pointer type stored
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @name Private procedures
    !> @{
    ! Doxygen needed line
    
    procedure, private :: new_, new_block_cyclic_, new_block_

    procedure, private :: size_local_default_, size_local_ng_
    procedure, private :: l2g_default_, l2g_ng_
    procedure, private :: g2l_default_, g2l_ng_
    procedure, private :: g2P_default_, g2P_ng_

    !>>@}
    
    !> @iSee new
    generic, public :: new => new_, new_block_cyclic_, new_block_

    !> @iSee #dtype
    procedure, public :: dtype => dtype_

    !> @iSee #comm
    procedure, public :: comm => comm_

    !> @iSee #group
    procedure, public :: group => group_

    !> @iSee #rank
    procedure, public :: rank => P_
    !> @iSee #size
    procedure, public :: size => NP_

    !> @iSee #is_dist
    procedure, public :: is_dist => is_dist_

    !> @iSee #is_comm
    procedure, public :: is_comm => is_comm_

    !> @iSee #size_dist_local
    generic, public :: size_dist_local => size_local_default_, size_local_ng_
    !> @iSee #size_dist
    procedure, public :: size_dist => size_


    !> @iSee #size_block
    procedure, public :: size_block => blocksize_
    !> @iSee #blocksize
    procedure, public :: blocksize => blocksize_

    !> @iSee #local2global
    generic, public :: local2global => l2g_default_, l2g_ng_
    !> @iSee #global2local
    generic, public :: global2local => g2l_default_, g2l_ng_

    !> @iSee #global2rank
    generic, public :: global2rank => g2P_default_, g2P_ng_
    
    !> @iSee #attach
    procedure, public :: attach => attach_

    !> @iSee #err
    procedure, public :: err => get_MPIerr_

#endif
  end type BUD_TYPE_NAME

  !> @bud container for the distributions
  !!
  !! Any information regarding the MPI processors
  !! are defined from processor number 0.
  !! Hence the range of ranks in the communicator
  !! are 0, 1, ..., NP - 1.
  !!
  !! In general this @bud owning processor will be
  !! referenced by `P` and the number of associated
  !! processors are `NP`.
  !!
  !! For advanced (future) distributions one will eventually
  !! implement routines specific for certain distributions.
  !! This is a needed abstraction to retain a single type
  !! as a container for _any_ distribution without increasing
  !! the complexity.
  !!
  !! @dev-note
  !! It has been considered to use transfer mechanisms for
  !! allowing a single container for multiple distribution
  !! types. However, the added complexity seems overkill
  !! in this regard.
  !! In any case we encourage that *any* additional
  !! distribution implementation creates routine names
  !! which does not per-see take note of the other routine
  !! names for other distribution types.
  !! This may help future splitting of the underlying data
  !! type without forcing code changes in the user code.
  type BUD_TYPE_NAME_

    !> Storage for MPI-error, allows external query.
    integer(ii_) :: err = MPI_SUCCESS

    !> The associated communicator
    integer(ii_) :: Comm = MPI_Comm_Null
    !> The group that encompass the equivalent communicator
    integer(ii_) :: Grp = MPI_Group_Null

    !> The associated processor number
    integer(ii_) :: P = 0
    !> The number of processors in `Comm`
    integer(ii_) :: NP = 1

    !> The distribution type
    !!
    !! @todo
    !! Consider changing this to `type`. However, it may be ambiguous
    !! with either choice.
    integer(ii_) :: dist = DIST_NONE

    !> Number of (total) elements in the distribution
    !!
    !! Although strictly not necessary this can
    !! be used to store a default size of the distribution
    integer(BUD_PREC) :: ng = -1

    ! Here we split the stored information in to sections

    ! ***** Block-distribution information
    !> Block-size if BLOCK_CYCLIC distribution is used.
    integer(ii_) :: BS = -1

    ! ***** Explicit distribution
    ! Here we store all information regarding the
    ! distribution.
    ! The current method is to store _everything_.
    ! Later implementations may provide "neighbour" only
    ! information. However, that increases MPI queries for
    ! non-local/neighbour elements.

    !> Number of (local) elements in the distribution
    integer(BUD_PREC), pointer, contiguous :: nl(:) => null()
    !> Local index to global index
    integer(BUD_PREC), pointer, contiguous :: l2g(:) => null()
    !> Global index to processor
    integer(BUD_PREC), pointer, contiguous :: g2P(:) => null()


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> Query communicator of the distribution
  !!
  !! @note Do not delete this communicator.
  interface comm
    module procedure comm_
  end interface
  public :: comm

  !> Query group of the distribution
  !!
  !! @note Do not delete this group.
  interface group
    module procedure group_
  end interface
  public :: group

  !> Query ID for current processor in distribution
  interface rank
    module procedure P_
  end interface
  public :: rank

  !> Query number of processors in distribution
  interface size
    module procedure NP_
  end interface
  public :: size

  !> Query block-size (if DIST_BLOCK_?)
  interface size_block
    module procedure blocksize_
  end interface
  public :: size_block

  !> Query block-size (if DIST_BLOCK_?)
  interface blocksize
    module procedure blocksize_
  end interface
  public :: blocksize

  !> Attach/Query several quantities at once
  interface attach
    module procedure attach_
  end interface
  public :: attach

  !> Assert the distribution type
  interface is_dist
    module procedure is_dist_
  end interface
  public :: is_dist

  !> Query whether a communicator is associated
  interface is_comm
    module procedure is_comm_
  end interface
  public :: is_comm

  !> Query the distribution type
  interface dtype
    module procedure dtype_
  end interface
  public :: dtype

  
  !> Create a new object
  interface new
    module procedure new_
    module procedure new_block_cyclic_
    module procedure new_block_
  end interface
  public :: new

  !> Create a new object
  interface BUD_TYPE_NEW
    module procedure new_
    module procedure new_block_cyclic_
    module procedure new_block_
  end interface
  public :: BUD_TYPE_NEW

  !> Create a new object which is "fake"
  !!
  !! Essentially this does not do any MPI function
  !! calls but may be used to emulate foreign distributions.
  interface newFake
    module procedure new_fake_block_cyclic_
    module procedure new_fake_block_
  end interface
  public :: newFake

  !> Query number of elements on this node
  interface size_dist_local
    module procedure size_local_default_
    module procedure size_local_ng_
  end interface
  public :: size_dist_local

  !> Query number of total elements 
  interface size_dist
    module procedure size_
  end interface
  public :: size_dist

  !> Query global index from a local index
  interface local2global
    module procedure l2g_default_
    module procedure l2g_ng_
  end interface
  public :: local2global

  !> Query local index from a global index
  interface global2local
    module procedure g2l_default_
    module procedure g2l_ng_
  end interface
  public :: global2local

  !> Query number of elements on this node
  interface global2rank
    module procedure g2P_default_
    module procedure g2P_ng_
  end interface
  public :: global2rank

   
  ! Here we create the interfaces for
  ! sub partitions

  !> Create a fake distribution on non-participating processors in a sub
  interface fake_sub
    module procedure fake_sub_
    module procedure fake_sub_explicit_
  end interface
  public :: fake_sub

  !> Query root processor and number of processors in one sub distribution
  interface sub1_root
    module procedure sub1_root_
  end interface
  public :: sub1_root

  !> Query root processor and number of processors in one sub distribution
  interface sub1_root_NP
    module procedure sub1_root_NP_
  end interface
  public :: sub1_root_NP

  !> Retrieve the ranks of a sub distribution within a top-level distribution
  interface sub1_ranks
    module procedure sub1_ranks_
  end interface
  public :: sub1_ranks

  !> Query subset information from multiple subsets simultaneously
  interface subN_groups
    module procedure subN_groups_
  end interface
  public :: subN_groups


  !> Query error of previous MPI calls
  interface MPIerr
    module procedure get_MPIerr_
  end interface
  public :: MPIerr
  
  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print

  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
#define BUD_DELETE_NOELEMENTAL
# include "bud_common.inc"
#undef BUD_DELETE_NOELEMENTAL

  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  subroutine delete_data(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this
    integer :: err

    ! Currently we do not allow external memory
    ! tracking.
    if ( this%Comm /= MPI_Comm_Null ) then
      ! reset everything
      ! A disconnect is less obstructive than
      ! a free. Perhaps we may choose to use Free
      ! anyway... ?
      call MPI_Group_Free(this%Grp, this%err)
      call MPI_Comm_Disconnect(this%Comm, this%err)
    end if

    ! Ensure they are nullified
    this%Comm = MPI_Comm_Null
    this%Grp = MPI_Group_Null
    
    this%P = 0
    this%NP = 1
    this%dist = DIST_NONE
    this%BS = -1

    ! Non BLOCK_CYCLIC
    this%ng = -1
    if ( associated(this%nl) ) then
      deallocate(this%nl)
      deallocate(this%l2g)
      deallocate(this%g2P)
      nullify(this%nl,this%l2g,this%g2P)
    end if
    
  end subroutine delete_data

  ! First we implement the easy stuff
  !> Query the associated communicator
  elemental function comm_(this) result(comm)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: comm
    comm = this%D%comm
  end function comm_

  !> Query the associated group
  elemental function group_(this) result(grp)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: grp
    grp = this%D%Grp
  end function group_

  !> Query the current processor ID in the communicator
  elemental function P_(this) result(P)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: P
    P = this%D%P
  end function P_

  !> Query pointer to the current processor ID in the communicator
  function Pp_(this) result(Pp)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_), pointer :: Pp
    Pp => this%D%P
  end function Pp_

  !> Query the number of processors in the communicator
  elemental function NP_(this) result(NP)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: NP
    NP = this%D%NP
  end function NP_

  !> Query pointer to the number of processors in the communicator
  function NPp_(this) result(NPp)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_), pointer :: NPp
    NPp => this%D%NP
  end function NPp_

  !> Query the default size of the distribution
  elemental function size_(this) result(n)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: n
    n = this%D%ng
  end function size_

  !> Query the block-size, `-1` if non-block-cyclic
  elemental function blocksize_(this) result(BS)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: BS
    BS = this%D%BS
  end function blocksize_

  !> Assert the distribution type
  elemental function is_dist_(this, dist) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_), intent(in) :: dist
    logical :: is
    is = this%D%dist == dist
  end function is_dist_

  !> Query distribution type used 
  elemental function dtype_(this) result(dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: dist
    dist = this%D%dist
  end function dtype_


  !> Query several contained quantities
  subroutine attach_(this, D, P, NP, ng, Comm, Grp, BS, err)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical, intent(in), optional :: D
    integer(ii_), intent(out), optional :: P, NP, ng, Comm, Grp, BS, err

    if ( present(P) ) P = this%D%P
    if ( present(NP) ) NP = this%D%NP
    if ( present(ng) ) ng = this%D%ng
    if ( present(Comm) ) Comm = this%D%Comm
    if ( present(Grp) ) Grp = this%D%Grp
    if ( present(BS) ) BS = this%D%BS
    if ( present(err) ) err = this%D%err

  end subroutine attach_
  

  !> Basic routine for initializing a new distribution
  !!
  !! This basic method does not attach anything associated with the
  !! distributino.
  !! Hence it may be used to initialize and use a distribution
  !! only via the communicator.
  !! @param[inout] this the @bud distribution container
  !! @param[in] Comm the communicator that we will dublicate and attach
  subroutine new_(this, Comm)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: Comm

    call initialize(this)

    ! dublicate communicator (ensures that we can delete it again)
    call MPI_Comm_dup(Comm, this%D%comm, this%D%err)

    ! Create the group
    call MPI_Comm_group(this%D%comm, this%D%Grp, this%D%err)
    
    ! Figure out number of processors and the rank
    call MPI_Comm_Rank( this%D%comm, this%D%P, this%D%err)
    call MPI_Comm_Size( this%D%comm, this%D%NP, this%D%err)

  end subroutine new_


  !> Create a new block cyclic distribution
  subroutine new_block_cyclic_(this, Comm, bs, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: Comm, BS
    integer(BUD_PREC), intent(in) :: ng
    integer(ii_), intent(in) :: dist

    select case ( dist )
    case ( &
      DIST_BLOCK_CYCLIC_FIRST &
!      DIST_BLOCK_CYCLIC_LAST, &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = BS
      this%D%ng = ng

    case default

      call delete(this)
      return
      
    end select

    ! Copy distribution to the container
    this%D%dist = dist

  end subroutine new_block_cyclic_

  !> Create a new block distribution
  subroutine new_block_(this, Comm, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: Comm
    integer(BUD_PREC), intent(in) :: ng
    integer(ii_), intent(in) :: dist

    select case ( dist )
    case ( &
      DIST_BLOCK_FIRST, &
      DIST_BLOCK_LAST &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = ng / this%D%NP
      this%D%ng = ng

    case default

      call delete(this)
      return
      
    end select

    ! Copy distribution to the container
    this%D%dist = dist

  end subroutine new_block_

    !> Create a new block cyclic distribution
  subroutine new_fake_block_cyclic_(this, NP, BS, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: NP, BS
    integer(BUD_PREC), intent(in) :: ng
    integer(ii_), intent(in) :: dist

    select case ( dist )
    case ( &
      DIST_BLOCK_CYCLIC_FIRST &
!      DIST_BLOCK_CYCLIC_LAST, &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call initialize(this)
      this%D%NP = NP

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = BS
      this%D%ng = ng
      
    case default

      call delete(this)
      return
      
    end select

    this%D%dist = dist

  end subroutine new_fake_block_cyclic_


  !> Create a new block distribution (non-cyclic)
  subroutine new_fake_block_(this, NP, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(BUD_PREC), intent(in) :: NP, ng
    integer(ii_), intent(in) :: dist
    
    select case ( dist )
    case ( &
      DIST_BLOCK_FIRST, &
      DIST_BLOCK_LAST &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call initialize(this)
      this%D%NP = NP

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = ng / this%D%NP
      this%D%ng = ng
      
    case default

      call delete(this)
      return
      
    end select

    this%D%dist = dist

  end subroutine new_fake_block_


  ! Interface for number of local elements
  elemental function size_local_default_(this) result(nl)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC) :: nl, i, ir, irB

    if ( this%D%NP == 1 ) then
      nl = this%D%ng
      return
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )
      
      ! count number of blocks per node
      i = this%D%ng / ( this%D%NP * this%D%BS )
      ! count remaining elements
      ir = this%D%ng - i * this%D%NP*this%D%BS
      ! remaining blocks
      irB = ir / this%D%BS
      ! correct for number of filled blocks
      ir = ir - irB * this%D%BS
      ! Calculate number of elements
      nl = i * this%D%BS
      if ( this%D%P < irB ) then
        nl = nl + this%D%BS
      else if ( this%D%P == irB ) then
        nl = nl + ir
      end if
      
!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$
!!$      ! count number of blocks per node
!!$      i = this%D%ng / ( this%D%NP * this%D%BS )
!!$      ! count remaining elements
!!$      ir = this%D%ng - i * this%D%NP*this%D%BS
!!$      ! remaining blocks
!!$      irB = ir / this%D%BS
!!$      ! correct for number of filled blocks
!!$      ir = ir - irB * this%D%BS
!!$      ! Calculate number of elements
!!$      nl = i * this%D%BS
!!$      irB = this%D%NP - irB
!!$      if ( irB < this%D%P ) then
!!$        nl = nl + this%D%BS
!!$      else if ( this%D%P == irB ) then
!!$        nl = nl + ir
!!$      end if
      
    case ( DIST_BLOCK_LAST )
      
      if ( this%D%P + 1 == this%D%NP ) then
        nl = this%D%ng - this%D%BS * this%D%P
      else
        nl = this%D%BS
      end if

    case ( DIST_BLOCK_FIRST )
      
      if ( this%D%P == 0 ) then
        nl = this%D%ng - this%D%BS * (this%D%NP-1)
      else
        nl = this%D%BS
      end if

    end select
    
  end function size_local_default_

  ! Interface for number of local elements
  elemental function size_local_ng_(this, ng) result(nl)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ng

    integer(BUD_PREC) :: nl, i, ir, irB

    if ( this%D%NP == 1 ) then
      nl = ng
      return
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )
      
      ! count number of blocks per node
      i = ng / ( this%D%NP * this%D%BS )
      ! count remaining elements
      ir = ng - i * this%D%NP*this%D%BS
      ! remaining blocks
      irB = ir / this%D%BS
      ! correct for number of filled blocks
      ir = ir - irB * this%D%BS
      ! Calculate number of elements
      nl = i * this%D%BS
      if ( this%D%P < irB ) then
        nl = nl + this%D%BS
      else if ( this%D%P == irB ) then
        nl = nl + ir
      end if
      
!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$
!!$      ! count number of blocks per node
!!$      i = ng / ( this%D%NP * this%D%BS )
!!$      ! count remaining elements
!!$      ir = ng - i * this%D%NP*this%D%BS
!!$      ! remaining blocks
!!$      irB = ir / this%D%BS
!!$      ! correct for number of filled blocks
!!$      ir = ir - irB * this%D%BS
!!$      ! Calculate number of elements
!!$      nl = i * this%D%BS
!!$      irB = this%D%NP - irB
!!$      if ( irB < this%D%P ) then
!!$        nl = nl + this%D%BS
!!$      else if ( this%D%P == irB ) then
!!$        nl = nl + ir
!!$      end if
      
    case ( DIST_BLOCK_LAST )
      
      nl = ng / this%D%NP
      if ( this%D%P + 1 == this%D%NP ) then
        nl = this%D%ng - nl * this%D%P
      end if

    case ( DIST_BLOCK_FIRST )
      
      nl = ng / this%D%NP
      if ( this%D%P == 0 ) then
        nl = this%D%ng - nl * (this%D%NP-1)
      end if

    end select
    
  end function size_local_ng_


  elemental function l2g_default_(this, il) result(ig)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: il
    integer(BUD_PREC) :: ig

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )
      
      ! get local block
      ig = (il - 1) / this%D%BS
      !  Calculate global index
      ig = ( ig*(this%D%NP-1) + this%D%P ) * this%D%BS + il
      
!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$      ! get local block
!!$      ig = (il -1) / this%D%BS
!!$      !  Calculate global index
!!$      ig = ( ig*(this%D%NP-1) + this%D%NP-this%D%P-1 ) * this%D%BS + il

    case ( DIST_BLOCK_LAST )

      ig = this%D%BS * this%D%P + il

    case ( DIST_BLOCK_FIRST )

      if ( this%D%P == 0 ) then
        ! simple case
        ig = il
      else
        ig = this%D%ng + this%D%BS * (this%D%P-this%D%NP) &
          + il
      end if

    end select
    
  end function l2g_default_


  elemental function l2g_ng_(this, il, ng) result(ig)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: il, ng
    integer(BUD_PREC) :: ig

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )
      
      ! get local block
      ig = (il - 1) / this%D%BS
      !  Calculate global index
      ig = ( ig*(this%D%NP-1) + this%D%P ) * this%D%BS + il
      
!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$      ! get local block
!!$      ig = (il -1) / this%D%BS
!!$      !  Calculate global index
!!$      ig = ( ig*(this%D%NP-1) + this%D%NP-this%D%P-1 ) * this%D%BS + il

    case ( DIST_BLOCK_LAST )

      ig = (ng / this%D%NP) * this%D%P + il

    case ( DIST_BLOCK_FIRST )

      if ( this%D%P == 0 ) then
        ! simple case
        ig = il
      else
        ! block-size
        ig = ng / this%D%NP
        ! calculate global index (correcting for
        ! initial remainder)
        ig = ng + ig * (this%D%P-this%D%NP) + il
      end if

    end select
    
  end function l2g_ng_


  elemental function g2l_default_(this, ig) result(il)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig
    integer(BUD_PREC) :: il

    integer(BUD_PREC) :: tmp(2)

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )

      ! Find global block number
      il = (ig-1) / this%D%BS
      ! Subtract global base line to find
      ! element number within the block
      tmp(1) = ig - il * this%D%BS
      ! Find the block number on the local node
      tmp(2) = (il - this%D%P) / this%D%NP
      ! Generate the local orbital pointer based
      ! on the local block number
      il = tmp(2) * this%D%BS + tmp(1)
      ! Check that this is consistent
      !  if it is not then this
      !  local orbital is not on this node
      !  and so we return -1 to indicate this.
      if ( ( tmp(2)*this%D%NP+this%D%P ) * this%D%BS + tmp(1) /= ig ) then
        il = -1
      end if

    case ( DIST_BLOCK_LAST )

      ! get residing node
      tmp(1) = (ig-1) / this%D%BS
      ! we are on the last block
      if ( tmp(1) == this%D%NP ) tmp(1) = tmp(1) - 1
      if ( tmp(1) == this%D%P ) then
        ! this asserts that we are on the correct node
        il = ig - tmp(1) * this%D%BS
      else
        ! we are on the wrong node and hence return
        ! -1 to signal this
        il = -1
      end if

    case ( DIST_BLOCK_FIRST )

      ! calculate the offset of the first node
      tmp(2) = this%D%ng - this%D%BS * this%D%NP
      
      if ( ig <= tmp(2) .and. this%D%P == 0 ) then
        il = ig
      else
        ! calculate node
        il = ig - tmp(2)
        tmp(1) = il / this%D%BS

        if ( tmp(1) == this%D%P - 1 ) then
          ! we are on the correct node
          il = il - this%D%BS * tmp(1)
        else
          ! we are on the wrong node and hence return
          ! -1 to signal this
          il = -1
        end if
      end if
      
    end select
    
  end function g2l_default_


  elemental function g2l_ng_(this, ig, ng) result(il)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig, ng
    integer(BUD_PREC) :: il

    integer(BUD_PREC) :: tmp(2)

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )

      ! Find global block number
      il = (ig-1) / this%D%BS
      ! Subtract global base line to find
      ! element number within the block
      tmp(1) = ig - il * this%D%BS
      ! Find the block number on the local node
      tmp(2) = (il - this%D%P) / this%D%NP
      ! Generate the local orbital pointer based
      ! on the local block number
      il = tmp(1) + tmp(2) * this%D%BS
      ! Check that this is consistent
      !  if it is not then this
      !  local orbital is not on this node
      !  and so we return -1 to indicate this.
      if ( (il - tmp(1) + this%D%P) * this%D%BS + tmp(1) /= ig ) then
        il = -1
      end if

    case ( DIST_BLOCK_LAST )

      ! calculate block size
      tmp(2) = ng / this%D%NP
      
      tmp(1) = (ig-1) / tmp(2)
      ! we are on the last block
      if ( tmp(1) == this%D%NP ) tmp(1) = tmp(1) - 1
      if ( tmp(1) == this%D%P ) then
        ! this asserts that we are on the correct node
        il = ig - tmp(1) * tmp(2)
      else
        ! we are on the wrong node and hence return
        ! -1 to signal this
        il = -1
      end if

    case ( DIST_BLOCK_FIRST )

      ! calculate block-size and the offset of the first node
      tmp(1) = ng / this%D%NP
      tmp(2) = ng - tmp(1) * this%D%NP
      
      if ( ig <= tmp(2) .and. this%D%P == 0 ) then
        il = ig
      else
        ! calculate node
        il = ig - tmp(2)
        tmp(2) = il / tmp(1)

        if ( tmp(2) == this%D%P - 1 ) then
          ! we are on the correct node
          il = il - tmp(1) * tmp(2)
        else
          ! we are on the wrong node and hence return
          ! -1 to signal this
          il = -1
        end if
      end if

    end select
    
  end function g2l_ng_



  elemental function g2P_default_(this, ig) result(P)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig
    integer(BUD_PREC) :: P

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )

      P = mod( (ig-1) / this%D%BS , this%D%NP)

!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$      ! get local block
!!$      ig = (il -1) / this%D%BS
!!$      !  Calculate global index
!!$      ig = ( ig*(this%D%NP-1) + this%D%NP-this%D%P-1 ) * this%D%BS + il

    case ( DIST_BLOCK_LAST )

      P = (ig-1) / this%D%BS
      if ( P == this%D%NP ) P = P - 1

    case ( DIST_BLOCK_FIRST )

      P = this%D%ng - this%D%BS * this%D%NP
      if ( ig <= this%D%BS + P ) then
        P = 0
      else
        P = (ig-P-1) / this%D%BS
      end if
      
    end select
    
  end function g2P_default_
  
  elemental function g2P_ng_(this, ig, ng) result(P)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig, ng
    integer(BUD_PREC) :: P

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST )
      ! get local block

      P = mod( (ig-1) / this%D%BS , this%D%NP)

!!$    case ( DIST_BLOCK_CYCLIC_LAST )
!!$      ! get local block
!!$      ig = (il -1) / this%D%BS
!!$      !  Calculate global index
!!$      ig = ( ig*(this%D%NP-1) + this%D%NP-this%D%P-1 ) * this%D%BS + il

    case ( DIST_BLOCK_LAST )

      P = (ig-1) / (ng / this%D%NP)
      if ( P == this%D%NP ) P = P - 1

    case ( DIST_BLOCK_FIRST )

      P = ng / this%D%NP
      if ( ig <= P + ng - P * this%D%NP ) then
        P = 0
      else
        P = (ig-ng+P*this%D%NP-1) / P + 1
      end if

    end select
    
  end function g2P_ng_

  !> Query whether a communicator is not MPI_COMM_NULL
  !!
  !! This may be used to identify `fake` distributions
  !! vs. real distributions.
  elemental function is_comm_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    is = is_initd(this)
    if ( is ) is = this%D%Comm /= MPI_Comm_Null
  end function is_comm_



  !> Convert a `sub` distribution to a fake distribution on non-`sub` processors
  !!
  !! Enables to *copy* a distribution to a *fake* distribution
  !! via a common set-distribution and a common `root`.
  !! Note that `root` must be equivalent on all nodes.
  subroutine fake_sub_(set, sub)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set, sub
    integer(ii_) :: root, nbot

    ! if the node is not part of the set distribution
    if ( .not. is_comm(set) ) return
    
    call sub1_root_NP(set, sub, root, nbot)

    call fake_sub(set, sub, root)

  end subroutine fake_sub_


  !> Copy a distribution via a set communicator and "sub_root" node
  !!
  !! Enables to *copy* a distribution to a *fake* distribution
  !! via a common set-distribution and a common `sub_root`.
  !! Note that `sub_root` must be equivalent on all nodes.
  !!
  !! A fake distribution allows one to use a common
  !! interface for determining the parent processor.
  subroutine fake_sub_explicit_(set, sub, sub_root)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: sub
    integer(ii_), intent(in) :: sub_root
    integer(ii_), allocatable :: rank(:)

    ! The ID of the root process in `sub` on distribution
    ! `set`.
    integer(ii_) :: set_root
    integer(ii_) :: Grp, Comm
    ! The ID of the root process in the new communication
    ! group.
    integer(ii_) :: grp_root
    integer(ii_) :: i, j

    if ( .not. is_comm(set) ) return

    ! Create a group of all processor not
    ! in sub and the root node of sub.
    allocate(rank(set%D%NP))
    rank(:) = -set%D%NP
    if ( .not. is_comm(sub) ) then
      rank(set%D%P+1) =  set%D%P
    else if ( set%D%P == sub_root ) then
      ! this signals to all processors which
      ! will be the b-casting node
      rank(set%D%P+1) = -set%D%P
    end if

    ! reduce ranks needed in the new group
    call MPI_AllReduce( MPI_IN_PLACE, rank, &
      set%D%NP, MPI_Integer, MPI_MAX, set%D%Comm, &
      set%D%err)
    
    ! Compress the list and figure out
    ! the set_root and grp_root
    set_root = -1
    grp_root = -1
    j = 0
    do i = 1 , set%D%NP
      if ( rank(i) /= -set%D%NP ) then
        j = j + 1
        if ( rank(i) < 0 ) then
          rank(j) = -rank(i)
          set_root = -rank(i)
          grp_root = j - 1
        else
          rank(j) = rank(i)
          ! this catches whether the ID is 0
          if ( rank(i) == 0 .and. grp_root == -1 ) then
            set_root = 0
            grp_root = j - 1
          end if
        end if
      end if
    end do

    ! At this point we have a list of ranks
    ! that we need to communicate between.

    ! Create the group
    call MPI_Group_incl(set%D%Grp, j, rank, Grp, set%D%err)

    ! Create the common communicator
    call MPI_Comm_create(set%D%Comm, Grp, Comm, set%D%err)

    ! If the communicator is NULL we return
    ! immediately.
    ! We free the Group as that has still been created
    ! on the remote processes that are not part
    ! of the group.
    if ( Comm == MPI_Comm_Null ) then
      call MPI_Group_Free(Grp, set%D%err)
      return
    end if

    ! Ensure empty sub
    if ( .not. is_comm(sub) ) then
      call delete(sub)
      call initialize(sub)
    else if ( set%D%P == set_root ) then
      ! or  ( sub%D%P == 0 )
      ! or  ( MPI_Group_Rank(Grp) == grp_root )
    end if
    
    call MPI_Bcast(sub%D%NP, 1, MPI_Integer, &
      grp_root, Comm, sub%D%err)
    call MPI_Bcast(sub%D%dist, 1,MPI_Integer, &
      grp_root, Comm, sub%D%err)
    
    select case ( sub%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST, &
      DIST_BLOCK_FIRST, &
      DIST_BLOCK_LAST )

      call MPI_Bcast(sub%D%ng, 1, MPI_Integer, &
        grp_root, Comm, sub%D%err)
      call MPI_Bcast(sub%D%BS, 1, MPI_Integer, &
        grp_root, Comm, sub%D%err)

    case ( DIST_EXPLICIT )

      ! TODO, implement Bcast of arrays
      
    case default

    end select

    call MPI_Group_Free(Grp, set%D%err)
    call MPI_Comm_Free(Comm, set%D%err)

  end subroutine fake_sub_explicit_

  
  !> Retrieve a common rank array from a subset distribution
  !!
  !! This returns an array which contains the
  !! `sub` processor IDs equivalent in the `set`
  !! communicator.
  !!
  !! This equates to the equivalence equation:
  !! `rank(dist_P(sub)) == dist_P(set)`
  !!
  !! Hence, upon exit `rank` has size `dist_NP(sub)`.
  subroutine sub1_ranks_(set, sub, rank)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: sub
    integer(ii_), intent(inout), allocatable :: rank(:)
    integer(ii_) :: sub_root, nbot
    integer(ii_) :: err

    if ( .not. is_comm(set) ) return

    ! gather information about the root node
    ! of the sub communicator as well as the
    ! number of processors in the sub communicator
    call sub1_root_NP(set, sub, sub_root, nbot)
    
    ! be sure to allocate array
    if ( .not. allocated(rank) ) &
      allocate(rank(nbot))
    if ( size(rank) /= nbot ) then
      deallocate(rank)
      allocate(rank(nbot))
    end if

    ! Initialize to "non-set" array
    rank = -1

    ! populate the rank array
    if ( is_comm(sub) ) then
      rank(sub%D%P+1) = set%D%P
    end if

    ! Make sure everybody has aligned data
    call MPI_AllReduce( MPI_IN_PLACE, rank, &
      nbot, MPI_Integer, MPI_MAX, set%D%Comm, &
      set%D%err)

    ! Now every processor knows which processor
    ! has which node ID

    ! For the processors in the set which
    ! did not have `sub` we create a "fake"
    ! distribution such that they can re-create
    ! the data
    call fake_sub(set, sub, sub_root)
    
  end subroutine sub1_ranks_


  !> Retrieve common information about set/sub relationship
  !!
  !! Query the `root` and number of nodes in `sub` distribution in the
  !! `set` distribution.
  subroutine sub1_root_(set, sub, sub_root)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: sub
    integer(ii_), intent(out) :: sub_root
    
    if ( .not. is_comm(set) ) return
    
    ! First figure out size of sub
    sub_root = -1
    if ( is_comm(sub) ) then
      if ( sub%D%P == 0 ) then
        sub_root = set%D%P
      end if
    end if

    ! Distribute root node and number of nodes
    call MPI_AllReduce( MPI_IN_PLACE, sub_root, 1, &
      MPI_Integer, MPI_Max, set%D%Comm, set%D%err)

  end subroutine sub1_root_

  !> Retrieve common information about set/sub relationship
  !!
  !! Query the `root` and number of nodes in `sub` distribution in the
  !! `set` distribution.
  subroutine sub1_root_NP_(set, sub, sub_root, sub_NP)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: sub
    integer(ii_), intent(out) :: sub_root, sub_NP
    
    if ( .not. is_comm(set) ) return
    
    ! First figure out size of sub
    sub_root = -1
    if ( is_comm(sub) ) then
      if ( sub%D%P == 0 ) then
        sub_root = set%D%P
        sub_NP = sub%D%NP
      end if
    end if

    ! Distribute root node and number of nodes
    call MPI_AllReduce( MPI_IN_PLACE, sub_root, 1, &
      MPI_Integer, MPI_Max, set%D%Comm, set%D%err)
    if ( sub_root >= 0 ) then
      call MPI_Bcast( sub_NP, 1, &
        MPI_Integer, sub_root, set%D%Comm, set%D%err)
    end if

    ! For now we actually do not check
    ! whether the action is an illegal or valid
    ! action.
    ! This may happen if the user passes different
    ! `sub` distributions which may or
    ! may not be associated.

  end subroutine sub1_root_NP_


  !> Retrieve common information about set/sub relationship for N subsets
  !!
  !! Query the `root` and number of nodes in `sub` distribution in the
  !! `set` distribution.
  subroutine subN_groups_(set, sub, sub_root, Ng, groups)
    use BUD_CC2(BUD_MOD,_utils), only: sort_quick
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: set, sub
    integer, intent(out) :: sub_root
    integer, intent(out) :: Ng
    integer, intent(inout), allocatable :: groups(:)
    
    integer, allocatable :: procs(:)
    integer :: i

    Ng = 0
    ! first deallocate groups
    if ( allocated(groups) ) deallocate(groups)
    
    if ( .not. is_comm(set) ) return

    ! figure out how many groups there are
    ! 1. the root node from each sub-group
    !    b-cast its parent rank value
    !    which is unique.
    ! 2. Every sub-node has information about the parent
    !    node ID
    if ( is_comm(sub) ) then
      sub_root = rank(set)
      call MPI_Bcast(sub_root, 1, MPI_Integer, &
        0, sub%D%Comm, sub%D%err)
    else
      ! signal this node is not part of a
      ! sub-set group
      sub_root = -1
    end if

    ! make list of each groups parent ID
    Ng = size(set)

    ! Allocate an array to contain the groups
    allocate(procs(Ng))
    ! initialize to minimum
    procs = -1
    procs(rank(set)+1) = sub_root

    ! Distribute root node and number of nodes
    call MPI_AllReduce( MPI_IN_PLACE, procs, Ng, &
      MPI_Integer, MPI_Max, set%D%Comm, set%D%err)

    ! sort the array to easier figure out the
    ! number of different elements
    call sort_quick(Ng, procs)

    ! figure out how many different segments there are
    Ng = 0
    do i = 1 , size(procs)
      if ( procs(i) == -1 ) cycle
      if ( i == 1 ) then
        Ng = 1
      else if ( procs(i-1) /= procs(i) ) then
        Ng = Ng + 1
      end if
    end do

    ! allocate and populate root-ID for the groups
    allocate(groups(Ng))

    ! figure out how many different segments there are
    Ng = 0
    do i = 1 , size(procs)
      if ( procs(i) == -1 ) cycle
      if ( i == 1 ) then
        Ng = 1
        groups(Ng) = procs(i)
      else if ( procs(i-1) /= procs(i) ) then
        Ng = Ng + 1
        groups(Ng) = procs(i)
      end if
    end do

    if ( Ng /= size(groups) ) then
      sub_root = -1
      Ng = 0
      deallocate(groups)
    end if

    ! clean-up
    deallocate(procs)
    
  end subroutine subN_groups_

  
  !> Function for retrieving the latest error messag
  elemental function get_MPIerr_(this) result(err)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: err
    if ( is_initd(this) ) then
      err = this%D%err
    else
      err = MPI_SUCCESS
    end if
  end function get_MPIerr_

  
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

    
    if ( this%D%Comm == MPI_Comm_Null ) then
      write(fmt, '(a,i0,a)') '(t',lindent,',4a,10(i0,a))'
      select case ( this%D%dist )
      case ( DIST_BLOCK_CYCLIC_FIRST ) 
        write(*,fmt) "<", trim(name), " FakeComm", &
          ", BCF block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"
      case ( DIST_BLOCK_FIRST ) 
        write(*,fmt) "<", trim(name), " FakeComm", &
          ", BF block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"
      case ( DIST_BLOCK_LAST ) 
        write(*,fmt) "<", trim(name), " FakeComm", &
          ", BL block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"
      case default
        write(*,'(a,i0,a)') "<Not implemented distribution: ",this%D%dist,">"
      end select
      
    else
      write(fmt, '(a,i0,a)') '(t',lindent,',3a,10(i0,a))'
      select case ( this%D%dist )
      case ( DIST_BLOCK_CYCLIC_FIRST ) 
        write(*,fmt) "<", trim(name), " Comm=", this%D%Comm, &
          ", BCF block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"
      case ( DIST_BLOCK_FIRST ) 
        write(*,fmt) "<", trim(name), " Comm=", this%D%Comm, &
          ", BF block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"
      case ( DIST_BLOCK_LAST ) 
        write(*,fmt) "<", trim(name), " Comm=", this%D%Comm, &
          ", BL block-size=", this%D%BS, &
          ", default-ng=", this%D%ng, &
          ", P=",this%D%P, ", NP=",this%D%NP, &
          ", refs: ", references(this), ">"

      case default
        write(*,'(a,i0,a)') "<Not implemented distribution: ",this%D%dist,">"
      end select

    end if
    
  end subroutine print_


  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_TYPE_VAR
#undef BUD_PREC

#include "bud_cleanup.inc"
  

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

