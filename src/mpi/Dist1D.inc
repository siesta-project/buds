  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"
  
  !> This module makes heavy usage of the
  !! MPI variables
#ifndef BUD_MPI_INCLUDE
  use mpi
#else
  include BUD_STR(BUD_MPI_INCLUDE)
#endif

  ! We require the use of the communicator
  use BUD_CC3(BUD_MOD,_,MP_Comm)

  use BUD_CC3(BUD_MOD,_,Dist_generic)

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)

  !> BUD_MOD_NAME documentation for various distribution types.
  !!
  !! This @bud implements a selected portion of different
  !! distributions.
  !! Currently the available distributions are:
  !!  - DIST_BLOCK_CYCLIC_FIRST_SPLIT
  !!    This format of block-cyclic distributions
  !!    with any remaining non-associated elements are
  !!    assigned to the first processors in a round-robin
  !!    fashion.
  !!  - DIST_BLOCK_LAST_ALL
  !!    This format is a simple block distribution
  !!    where all remaining elements are assigned to the
  !!    last process.
  !!  - DIST_CYCLIC_FIRST
  !!    This format is a simple distribution equivalent to
  !!    the DIST_BLOCK_CYCLIC_FIRST_SPLIT with block-size 1.
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
    
    procedure, private :: new_, new_block_cyclic_, new_block_or_cyclic_

    !>>@}
    
    !> @iSee new
    generic, public :: new => new_, new_block_cyclic_, new_block_or_cyclic_

    !> @iSee #dist_type
    procedure, public :: dist_type => dist_type_

    !> @iSee #comm
    procedure, public :: comm => comm_

    !> @iSee #group
    procedure, public :: group => group_

    !> @iSee #comm_rank
    procedure, public :: comm_rank => P_
    !> @iSee #comm_rank
    procedure, public :: rank => P_
    !> @iSee #comm_size
    procedure, public :: comm_size => NP_

    !> @iSee #is_dist
    procedure, public :: is_dist => is_dist_

    !> @iSee #is_comm
    procedure, public :: is_comm => is_comm_

    !> @iSee #size_local
    procedure, public :: size_local => size_local_
    !> @iSee #size_global
    procedure, public :: size_global => size_
    !> @iSee #size_global
    procedure, public :: size => size_


    !> @iSee #size_block
    procedure, public :: size_block => blocksize_
    !> @iSee #blocksize
    procedure, public :: blocksize => blocksize_

    !> @iSee #local2global
    procedure, public :: local2global => local_to_global_
    !> @iSee #local2global
    procedure, public :: l2g => local_to_global_
    !> @iSee #global2local
    procedure, public :: global2local => global_to_local_
    !> @iSee #global2local
    procedure, public :: g2l => global_to_local_

    !> @iSee #global2rank
    procedure, public :: global2rank => global_to_rank_
    !> @iSee #global2rank
    procedure, public :: g2rank => global_to_rank_
    
    !> @iSee #attach
    procedure, public :: attach => attach_

    !> @iSee #error_mpi
    procedure, public :: error_mpi => get_MPIerr_

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
  !! This is of course only possible if the interfaces are
  !! distinct.
  type BUD_TYPE_NAME_

#define BUD_TYPE_MP_COMM BUD_CC2(BUD_TYPE,MP_Comm)
    
    !> The associated communicator
    type(BUD_TYPE_MP_COMM) :: comm

    !> The distribution type
    !!
    !! @todo
    !! Consider changing this to `type`. However, it may be ambiguous
    !! with either choice.
    integer :: dist = DIST_NONE

    !> Number of (total) elements in the distribution
    !!
    !! Although strictly not necessary this can
    !! be used to store a default size of the distribution
    integer(BUD_PREC) :: ng = -1

    ! Here we split the stored information in to sections

    ! ***** Block-distribution information
    !> Block-size if BLOCK_CYCLIC distribution is used.
    integer(BUD_PREC) :: BS = -1

    ! ***** Explicit distribution
    ! Here we store all information regarding the
    ! distribution.
    ! The current method is to store _everything_.
    ! Later implementations may provide "neighbour" only
    ! information. However, that increases MPI queries for
    ! non-local/neighbour elements.

    !> Number of (local) elements in the distribution
    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: nl(:) => null()
    !> Local index to global index
    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: l2g(:) => null()
    !> Global index to processor
    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: g2P(:) => null()


    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> Create a new distribution object.
  interface BUD_TYPE_NEW
    module procedure new_
    module procedure new_block_cyclic_
    module procedure new_block_or_cyclic_
  end interface
  public :: BUD_TYPE_NEW

  !> Create a new object
  interface new
    module procedure new_
    module procedure new_block_cyclic_
    module procedure new_block_or_cyclic_
  end interface
  public :: new

  
  !> Query communicator of the distribution
  interface comm
    module procedure comm_
  end interface
  public :: comm

  !> Query ID for current processor in distribution
  interface comm_rank
    module procedure P_
  end interface
  public :: comm_rank

  !> @iSee #comm_rank
  interface rank
    module procedure P_
  end interface
  public :: rank

  !> Query number of processors in distribution
  interface comm_size
    module procedure NP_
  end interface
  public :: comm_size

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
  interface dist_type
    module procedure dist_type_
  end interface
  public :: dist_type


  !> Query number of elements on this node
  interface size_local
    module procedure size_local_
  end interface
  public :: size_local

  !> Query number of total elements 
  interface size_global
    module procedure size_
  end interface
  public :: size_global

  !> @iSee size_global
  interface size
    module procedure size_
  end interface
  public :: size

  !> Query global index from a local index
  interface local2global
    module procedure local_to_global_
  end interface
  public :: local2global

  !> @iSee local2global
  interface l2g
    module procedure local_to_global_
  end interface
  public :: l2g

  !> Query local index from a global index
  interface global2local
    module procedure global_to_local_
  end interface
  public :: global2local

  !> @iSee global2local
  interface g2l
    module procedure global_to_local_
  end interface
  public :: g2l

  !> Query number of elements on this node
  interface global2rank
    module procedure global_to_rank_
  end interface
  public :: global2rank

  !> @iSee global2rank
  interface g2rank
    module procedure global_to_rank_
  end interface
  public :: g2rank

   
  ! Here we create the interfaces for
  ! sub partitions

  !> Query error of previous MPI calls
  interface error_MPI
    module procedure get_MPIerr_
  end interface
  public :: error_MPI
  
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


  !> @cond BUD_DEVELOPER
  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  subroutine delete_(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this
    integer :: err

    call delete(this%comm)

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
    
  end subroutine delete_


  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full stuff (Comm_dup is issued
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    call new(to, from%D%comm)

    to%D%dist = from%D%dist
    to%D%ng = from%D%ng
    to%D%BS = from%D%BS
    to%D%error_ = from%D%error_

    call assert(from%D%nl, to%D%nl)
    call assert(from%D%l2g, to%D%l2g)
    call assert(from%D%g2P, to%D%g2P)

    ! Copy the common data-content
    call common_copy_(from, to)

  contains

    subroutine assert(froml, tol)
      integer(BUD_PREC), pointer :: froml(:), tol(:)
      integer(BUD_PREC) :: lb, ub

      if ( associated(froml) ) then
        lb = lbound(froml,1)
        ub = ubound(froml,1)
        allocate(tol(lb:ub))
      end if

    end subroutine assert
      
  end subroutine copy_


  ! First we implement the easy stuff
  !> Query the associated communicator
  function comm_(this) result(comm)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    type(BUD_TYPE_MP_COMM) :: comm
    comm = this%D%comm
  end function comm_

  !> Query the current processor ID in the communicator
  elemental function P_(this) result(P)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: P
    P = comm_rank(this%D%comm)
  end function P_

  !> Query the number of processors in the communicator
  elemental function NP_(this) result(NP)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: NP
    NP = comm_size(this%D%comm)
  end function NP_


  !> Query the default size of the distribution
  elemental function size_(this) result(ng)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC) :: ng
    ng = this%D%ng
  end function size_

  !> Query the block-size, `-1` if non-block-cyclic
  elemental function blocksize_(this) result(BS)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC) :: BS
    BS = this%D%BS
  end function blocksize_

  !> Assert the distribution type
  elemental function is_dist_(this, dist) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in) :: dist
    logical :: is
    is = this%D%dist == dist
  end function is_dist_

  !> Query distribution type used 
  elemental function dist_type_(this) result(dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: dist
    dist = this%D%dist
  end function dist_type_


  !> Query several contained quantities
  subroutine attach_(this, D, dist, P, NP, ng, BS)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical, intent(in), optional :: D
    integer, intent(out), optional :: dist
    integer(ii_), intent(out), optional :: P, NP
    integer(BUD_PREC), intent(out), optional :: BS, ng

    if ( present(dist) ) dist = this%D%dist
    if ( present(P) ) P = rank(this%D%comm)
    if ( present(NP) ) NP = comm_size(this%D%comm)
    if ( present(ng) ) ng = this%D%ng
    if ( present(BS) ) BS = this%D%BS

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
    BUD_CLASS(BUD_TYPE_MP_COMM), intent(inout) :: Comm
    
    call initialize(this)

    this%D%comm = Comm
    
  end subroutine new_

  
  !> Create a new block cyclic distribution
  subroutine new_block_cyclic_(this, Comm, bs, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_TYPE_MP_COMM), intent(inout) :: Comm
    integer(BUD_PREC), intent(in) :: BS, ng
    integer, intent(in) :: dist

    select case ( dist )
    case ( &
      DIST_BLOCK_CYCLIC_FIRST_SPLIT &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = BS
      this%D%ng = ng

    case ( &
      DIST_BLOCK_LAST_ALL &
      )
      
      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = BS
      this%D%ng = ng
      
      ! Assert that this is a viable distribution
      ! I.e. for block-distributions no processor
      ! must be empty...
      if ( ng - BS * comm_size(comm) > BS ) then
        call delete(this)
        return
      end if

    case default

      call delete(this)
      return
      
    end select

    ! Copy distribution to the container
    this%D%dist = dist

  end subroutine new_block_cyclic_

  !> Create a new block/cyclic distribution
  subroutine new_block_or_cyclic_(this, Comm, ng, dist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_TYPE_MP_COMM), intent(inout) :: Comm
    integer(BUD_PREC), intent(in) :: ng
    integer, intent(in) :: dist

    select case ( dist )
    case ( &
      DIST_BLOCK_LAST_ALL &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%BS = ng / comm_size(Comm)
      this%D%ng = ng
      
    case ( &
      DIST_CYCLIC_FIRST &
      )

      ! Create a new object with communicator
      ! and populate 'P' and 'NP'
      call new(this, Comm)

      ! we do not use any explicit terminology
      ! for splitting this information.
      this%D%ng = ng

    case default

      call delete(this)
      return
      
    end select

    ! Copy distribution to the container
    this%D%dist = dist

  end subroutine new_block_or_cyclic_


  ! Interface for number of local elements
  elemental function size_local_(this, P) result(nl)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_), intent(in), optional :: P

    integer(ii_) :: lP, NP
    integer(BUD_PREC) :: nl, i, ir, irB

    NP = comm_size(this%D%Comm)
    if ( NP == 1 ) then
      nl = this%D%ng
      return
    end if

    if ( present(P) ) then
      lP = P
    else
      lP = comm_rank(this%D%comm)
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST_SPLIT )

      ! ASSERTED
      
      ! count number of blocks per node
      i = this%D%ng / ( NP * this%D%BS )
      ! count elements not in default blocks
      ir = this%D%ng - i * NP*this%D%BS
      ! elements not in blocks may still be divided into blocks
      irB = ir / this%D%BS
      if ( lP < irB ) then
        ! this is the blocks that are completely filled
        nl = i * this%D%BS + this%D%BS
      else if ( lP == irB ) then
        ! correct for number of elements in the last block
        nl = i * this%D%BS + ir - irB * this%D%BS
      end if
      
    case ( DIST_BLOCK_LAST_ALL )

      ! ASSERTED
      
      if ( lP + 1 == NP ) then
        nl = this%D%ng - this%D%BS * lP
      else
        nl = this%D%BS
      end if
      
    case ( DIST_CYCLIC_FIRST )

      ! ASSERTED

      nl = this%D%ng / NP
      i = this%D%ng - nl * NP
      if ( lP < i ) then
        nl = nl + 1
      end if

    end select
    
  end function size_local_


  elemental function local_to_global_(this, il, P) result(ig)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: il
    integer(ii_), intent(in), optional :: P

    integer(ii_) :: lP, NP
    integer(BUD_PREC) :: ig

    NP = comm_size(this%D%comm)

    if ( NP == 1 ) then
      ig = il
      return
    end if

    if ( present(P) ) then
      lP = P
    else
      lP = comm_rank(this%D%comm)
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST_SPLIT )

      ! ASSERTED
      
      ! get local block
      ig = (il - 1) / this%D%BS
      !  ig*(this%D%NP-1) == the number of blocks in all other
      !     P (remember il contains the local offset-blocks)
      ig = ( ig*(NP-1) + lP ) * this%D%BS + il
      
    case ( DIST_BLOCK_LAST_ALL )

      ! ASSERTED
      
      ig = this%D%BS * lP + il

    case ( DIST_CYCLIC_FIRST )
            
      ! ASSERTED 

      ig = (il - 1) * NP + lP

    end select
    
  end function local_to_global_


  elemental function global_to_local_(this, ig, P) result(il)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig
    integer(ii_), intent(in), optional :: P
    integer(BUD_PREC) :: il

    integer(ii_) :: lP, NP
    integer(BUD_PREC) :: tmp(2)

    NP = comm_size(this%D%comm)
    if ( NP == 1 ) then
      il = ig
      return
    end if

    if ( present(P) ) then
      lP = P
    else
      lP = comm_rank(this%D%comm)
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST_SPLIT )

      ! ASSERTED
      
      ! Find global block number
      il = (ig-1) / this%D%BS
      ! Subtract global base line to find
      ! element number within the block
      tmp(1) = ig - il * this%D%BS
      ! Find the block number on the local node
      tmp(2) = (il - lP) / NP
      ! Generate the local orbital pointer based
      ! on the local block number
      il = tmp(2) * this%D%BS + tmp(1)
      ! Check that this is consistent
      !  if it is not then this
      !  local orbital is not on this node
      !  and so we return -1 to indicate this.
      if ( ( tmp(2)*NP+lP ) * this%D%BS + tmp(1) /= ig ) then
        il = -1
      end if

    case ( DIST_BLOCK_LAST_ALL )

      ! ASSERTED

      ! get residing node
      tmp(1) = (ig-1) / this%D%BS
      il = ig - this%D%BS * tmp(1)

    case ( DIST_CYCLIC_FIRST )

      ! ASSERTED
      
      il = (ig-1) / NP + 1
      
    end select
    
  end function global_to_local_


  elemental function global_to_rank_(this, ig) result(P)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: ig
    integer(ii_) :: P
    integer(ii_) :: NP

    NP = comm_size(this%D%comm)
    if ( NP == 1 ) then
      P = 0
      return
    end if

    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST_SPLIT )

      P = mod( (ig-1) / this%D%BS , NP)

    case ( DIST_BLOCK_LAST_ALL )

      P = (ig-1) / this%D%BS

    case ( DIST_CYCLIC_FIRST )

      P = mod(ig-1,NP)
      
    end select
    
  end function global_to_rank_
  


  !> Query whether a communicator is not MPI_COMM_NULL
  !!
  !! This may be used to identify `fake` distributions
  !! vs. real distributions.
  elemental function is_comm_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    is = is_initd(this)
    if ( is ) is = is_communicator(this%D%Comm)
  end function is_comm_



  !> Function for retrieving the latest error messag
  elemental function get_MPIerr_(this) result(err)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: err
    if ( is_initd(this) ) then
      err = this%D%error_
      if ( err == MPI_SUCCESS ) then
        err = error_MPI(this%D%comm)
      end if
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

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<<", trim(name), " Dist1D>"
    ! Print the communicator
    call print(this%D%comm, indent = lindent)
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,10(i0,a))'
    select case ( this%D%dist )
    case ( DIST_BLOCK_CYCLIC_FIRST_SPLIT ) 
      write(*,fmt) "< Block-Cyclic-FS", &
        ", bs=", this%D%BS, ", ng=", this%D%ng, ">"
    case ( DIST_BLOCK_LAST_ALL ) 
      write(*,fmt) "< Block-LA", &
        ", bs=", this%D%BS, ", ng=", this%D%ng, ">"
    case ( DIST_CYCLIC_FIRST ) 
      write(*,fmt) "< Cyclic-F", &
        ", ng=", this%D%ng, ">"
    case default
      write(*,'(a,i0,a)') "<Not implemented distribution: ",this%D%dist,">"
    end select
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <Dist1D-refs: ", references(this), ">>"
    
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

