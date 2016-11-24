  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"

! Define default variable for the file
#define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,MP_Comm)
#define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,MP_Comm)
#define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,MP_Comm)

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)

!> @defgroup mp Message Passing object
!! @ingroup bud-intrinsic
!!
!! Perform MPI communications using a simpler interface
!!
!! @{
module BUD_MOD_NAME

#ifdef BUD_MPI
  !> This module makes heavy usage of the
  !! MPI variables
# ifdef BUD_MPI_INCLUDE
  include BUD_MPI_INCLUDE
# else
  use mpi
# endif
#endif

  !> BUD_MOD_NAME documentation for the Message Passing interface.
  !1
  !! This @bud implements generic MPI commands in a stringent
  !! communicator.
  !!
  !! The type contains information such as:
  !!  - rank
  !!  - comm-size
  !!  - communicator
  !!  - group
  !!
  !! It does not contain information about any top-level @bud it may
  !! be created from.
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
  !! If you add any new dependencies be sure to undefine the variable
  !! at the bottom of this file.

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"


  !> @bud variable for a message passing object
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @name Private procedures
    !> @{
    ! Doxygen needed line

    procedure, private :: new_
    procedure, private :: new_remote_
#ifdef BUD_MPI
    procedure, private :: new_remote_child_
#endif
    
#define BUD_DEFINE_PROCEDURE procedure, private, pass(this) ::

#ifdef BUD_MPI

    ! Define all interfaces
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX
    
#define BUD_MP_COMM_NAME Send
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISSend
# include "MP_Comm_routine_interface.inc"
    
#define BUD_MP_COMM_NAME Send_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_Init
# include "MP_Comm_routine_interface.inc"
    
#define BUD_MP_COMM_NAME Recv
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRecv
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Recv_Init
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Bcast
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBcast
# include "MP_Comm_routine_interface.inc"
    
#define BUD_MP_COMM_NAME Gather
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IGather
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME AllGather
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllGather
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Scatter
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IScatter
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Reduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce
# include "MP_Comm_routine_interface.inc"


    ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL
    
#define BUD_MP_COMM_NAME Reduce_Prod
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_Prod
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Reduce_Sum
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_Sum
# include "MP_Comm_routine_interface.inc"

    
    ! INTEGER/REAL
# undef BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME Reduce_Max
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_Max
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Reduce_Min
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_Min
# include "MP_Comm_routine_interface.inc"

    
    ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME Reduce_LAND
#define BUD_IS_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_LAND
#define BUD_IS_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Reduce_LOR
#define BUD_IS_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_LOR
#define BUD_IS_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Reduce_LXOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_LXOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
    

# undef BUD_IS_LOGICAL
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME AllReduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce
# include "MP_Comm_routine_interface.inc"

    
    ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME AllReduce_Prod
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_Prod
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME AllReduce_Sum
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_Sum
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"

    
    ! INTEGER/REAL
# undef BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME AllReduce_Max
#define BUD_MP_COMM_NO_COMPLEX
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_Max
#define BUD_MP_COMM_NO_COMPLEX
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME AllReduce_Min
#define BUD_MP_COMM_NO_COMPLEX
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_Min
#define BUD_MP_COMM_NO_COMPLEX
#define BUD_MP_COMM_NO_LOGICAL
# include "MP_Comm_routine_interface.inc"

    
    ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL
    
#define BUD_MP_COMM_NAME AllReduce_LAND
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_LAND
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME AllReduce_LOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_LOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME AllReduce_LXOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_LXOR
#define BUD_MP_COMM_ONLY_LOGICAL
# include "MP_Comm_routine_interface.inc"

#undef BUD_IS_LOGICAL
#undef BUD_IS_INTEGER
#undef BUD_IS_REAL
#undef BUD_IS_COMPLEX

#endif
    
#undef BUD_DEFINE_PROCEDURE

    procedure, private :: Comm_split_
    procedure, private :: comm_split_type_

    procedure, private :: Comm_create_, comm_create_commgrp_
    procedure, private :: Comm_create_group_, comm_create_group_commgrp_

    !>>@}
    
    !> @iSee new
    procedure, public :: new => new_

    !> @iSee new_remote
#ifdef BUD_MPI
    generic, public :: new_remote => new_remote_, new_remote_child_
#else
    generic, public :: new_remote => new_remote_
#endif

    !> @iSee #communicator
    procedure, public :: communicator => comm_

    !> @iSee #group
    procedure, public :: group => group_

    !> @iSee #comm_rank
    procedure, public :: comm_rank => P_
    !> @iSee #comm_size
    procedure, public :: comm_size => NP_

    !> @iSee #is_communicator
    procedure, public :: is_communicator => is_comm_

    !> @iSee #is_group
    procedure, public :: is_group => is_group_

    !> @iSee #error_mpi
    procedure, public :: error_mpi => get_MPIerr_

    !> @iSee #is_success_mpi
    procedure, public :: is_success_mpi => is_MPIsuccess_

#ifdef BUD_MPI
    
    ! Define all interfaces
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME Send
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME BSend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME RSend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME SSend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME ISend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IBSend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IRSend
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME ISSend
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Send_Init
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME BSend_Init
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME RSend_Init
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME SSend_Init
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Recv
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IRecv
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Recv_Init
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Bcast
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IBcast
# include "MP_Comm_routine_interface_generic.inc"
    
#define BUD_MP_COMM_NAME Gather
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IGather
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME AllGather
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllGather
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Scatter
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IScatter
# include "MP_Comm_routine_interface_generic.inc"

#define BUD_MP_COMM_NAME Reduce
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce
# include "MP_Comm_routine_interface_generic.inc"


    ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME Reduce_Prod
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_Prod
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME Reduce_Sum
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_Sum
# include "MP_Comm_routine_interface_generic.inc"

    
    ! INTEGER/REAL
# undef BUD_IS_COMPLEX
    
#define BUD_MP_COMM_NAME Reduce_Max
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_Max
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME Reduce_Min
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_Min
# include "MP_Comm_routine_interface_generic.inc"

    
    ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME Reduce_LAND
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_LAND
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME Reduce_LOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_LOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME Reduce_LXOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IReduce_LXOR
# include "MP_Comm_routine_interface_generic.inc"

    
# undef BUD_IS_LOGICAL
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME AllReduce
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce
# include "MP_Comm_routine_interface_generic.inc"


    ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME AllReduce_Prod
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_Prod
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME AllReduce_Sum
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_Sum
# include "MP_Comm_routine_interface_generic.inc"


    ! INTEGER/REAL
# undef BUD_IS_COMPLEX

#define BUD_MP_COMM_NAME AllReduce_Max
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_Max
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME AllReduce_Min
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_Min
# include "MP_Comm_routine_interface_generic.inc"

    ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL

#define BUD_MP_COMM_NAME AllReduce_LAND
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_LAND
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME AllReduce_LOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_LOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME AllReduce_LXOR
# include "MP_Comm_routine_interface_generic.inc"
#define BUD_MP_COMM_NAME IAllReduce_LXOR
# include "MP_Comm_routine_interface_generic.inc"

#undef BUD_IS_LOGICAL
#undef BUD_IS_INTEGER
#undef BUD_IS_REAL
#undef BUD_IS_COMPLEX

#endif

    procedure, public :: Barrier => Barrier_
    procedure, public :: IBarrier => IBarrier_

# ifdef BUD_MPI
    generic, public :: Comm_split => comm_split_, comm_split_type_
    procedure, public :: Comm_Compare => comm_compare_

    generic, public :: Comm_Create => comm_Create_, comm_create_commgrp_, &
      comm_Create_group_, comm_create_group_commgrp_
# endif
#endif
  end type BUD_TYPE_NAME

#ifndef BUD_MPI
  !> Public MPI_SUCCESS to always be able to compare.
  integer(ii_), parameter, public :: MPI_SUCCESS = 0
  integer(ii_), parameter :: MPI_Comm_Null = -huge(1)
  integer(ii_), parameter :: MPI_Group_Null = -huge(1)
#endif

  !> @bud container for message passing information.
  type BUD_TYPE_NAME_

    !> The associated communicator
    integer(ii_) :: Comm = MPI_Comm_Null
    !> The group that encompass the equivalent communicator
    integer(ii_) :: Grp = MPI_Group_Null

    !> The associated processor number in the associated communicator
    integer(ii_) :: P = 0
    !> The number of processors in the associated communicator
    integer(ii_) :: NP = 1

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  

  !> Create a new message passing object.
  !!
  !! This creates a new message passing object
  !! by duplicating a communicator and creates
  !! the associated group.
  interface new
    module procedure new_
  end interface
  public :: new

  !> Create a new message passing object.
  !!
  !! @iSee new
  interface BUD_TYPE_NEW
    module procedure new_
  end interface
  public :: BUD_TYPE_NEW


  !> Create a new (remote) message passing object.
  !!
  !! This is a _fake_ message passing object
  !! by creating an object with no associated
  !! communicator, nor group.
  !! This is handy if one knows the _size_ of a
  !! remote message passing object in a remote
  !! group and one wishes to know which elements
  !! belong to which rank in the remote group.
  !!
  !! This may be called with a parent and a child.
  !! In this case a remote child will be created on
  !! those nodes that do not belong to child.
  interface new_remote
    module procedure new_remote_
#ifdef BUD_MPI
    module procedure new_remote_child_
#endif
  end interface
  public :: new_remote

  !> Creating a remote communicator
  !!
  !! @iSee new_remote
  interface BUD_CC2(BUD_TYPE_NEW,_remote)
    module procedure new_remote_
#ifdef BUD_MPI
    module procedure new_remote_child_
#endif
  end interface
  public :: BUD_CC2(BUD_TYPE_NEW,_remote)

  
  !> Query communicator of the distribution
  !!
  !! @note Do not free this communicator.
  interface communicator
    module procedure comm_
  end interface
  public :: communicator

  !> Query group of the distribution
  !!
  !! @note Do not free this group.
  interface group
    module procedure group_
  end interface
  public :: group

  !> Query ID for current processor in distribution
  interface comm_rank
    module procedure P_
  end interface
  public :: comm_rank

  !> Query number of processors in distribution
  interface comm_size
    module procedure NP_
  end interface
  public :: comm_size

  !> @iSee #comm_size
  interface size
    module procedure NP_
  end interface
  public :: size

  !> Query whether the communicator is anything but `MPI_Comm_Null`
  interface is_communicator
    module procedure is_comm_
  end interface
  public :: is_communicator

  !> Query whether the group is anything but `MPI_Group_Null`
  interface is_group
    module procedure is_group_
  end interface
  public :: is_group

#ifdef BUD_MPI
  !> Query data in a child communicator
  interface child_Bcast
    module procedure child_Bcast_
  end interface
  public :: child_Bcast

  !> Query ranks in a child communicator
  interface child_Bcast_ranks
    module procedure child_Bcast_ranks_
  end interface
  public :: child_Bcast_ranks
#endif
  
  !> Query error of previous MPI call
  interface error_MPI
    module procedure get_MPIerr_
  end interface
  public :: error_MPI

  !> Returns `.true.` if the previous MPI call was a success.
  interface is_success_MPI
    module procedure is_MPIsuccess_
  end interface
  public :: is_success_MPI

  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print

#ifdef BUD_MPI
  
  ! Define all interfaces
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX

  ! Define interfaces
  !> Interface for `MPI_Send`
  interface Send
#define BUD_MP_COMM_NAME Send
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Send
  !> Interface for `MPI_BSend`
  interface BSend
#define BUD_MP_COMM_NAME BSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: BSend
  !> Interface for `MPI_RSend`
  interface RSend
#define BUD_MP_COMM_NAME RSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: RSend
  !> Interface for `MPI_SSend`
  interface SSend
#define BUD_MP_COMM_NAME SSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: SSend
  !> Interface for `MPI_ISend`
  interface ISend
#define BUD_MP_COMM_NAME ISend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: ISend
  !> Interface for `MPI_IBSend`
  interface IBSend
#define BUD_MP_COMM_NAME IBSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IBSend
  !> Interface for `MPI_IRSend`
  interface IRSend
#define BUD_MP_COMM_NAME IRSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IRSend
  !> Interface for `MPI_ISSend`
  interface ISSend
#define BUD_MP_COMM_NAME ISSend
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: ISSend

  !> Interface for `MPI_Send_Init`
  interface Send_Init
#define BUD_MP_COMM_NAME Send_Init
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Send_Init
  !> Interface for `MPI_BSend_Init`
  interface BSend_Init
#define BUD_MP_COMM_NAME BSend_Init
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: BSend_Init
  !> Interface for `MPI_RSend_Init`
  interface RSend_Init
#define BUD_MP_COMM_NAME RSend_Init
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: RSend_Init
  !> Interface for `MPI_SSend_Init`
  interface SSend_Init
#define BUD_MP_COMM_NAME SSend_Init
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: SSend_Init

  !> Interface for `MPI_Recv`
  interface Recv
#define BUD_MP_COMM_NAME Recv
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Recv
  !> Interface for `MPI_IRecv`
  interface IRecv
#define BUD_MP_COMM_NAME IRecv
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IRecv

  !> Interface for `MPI_Recv_Init`
  interface Recv_Init
#define BUD_MP_COMM_NAME Recv_Init
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Recv_Init

  !> Interface for `MPI_Bcast`
  interface Bcast
#define BUD_MP_COMM_NAME Bcast
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Bcast
  !> Interface for `MPI_IBcast`
  interface IBcast
#define BUD_MP_COMM_NAME IBcast
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IBcast

  !> Interface for `MPI_Gather`
  interface Gather
#define BUD_MP_COMM_NAME Gather
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Gather
  !> Interface for `MPI_IGather`
  interface IGather
#define BUD_MP_COMM_NAME IGather
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IGather

  !> Interface for `MPI_AllGather`
  interface AllGather
#define BUD_MP_COMM_NAME AllGather
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllGather
  !> Interface for `MPI_IAllGather`
  interface IAllGather
#define BUD_MP_COMM_NAME IAllGather
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllGather

  !> Interface for `MPI_Scatter`
  interface Scatter
#define BUD_MP_COMM_NAME Scatter
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Scatter
  !> Interface for `MPI_IScatter`
  interface IScatter
#define BUD_MP_COMM_NAME IScatter
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IScatter

  !> Interface for `MPI_Reduce`
  interface Reduce
#define BUD_MP_COMM_NAME Reduce
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce
  !> Interface for `MPI_IReduce`
  interface IReduce
#define BUD_MP_COMM_NAME IReduce
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce


  ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL
  
  !> Interface for `MPI_Reduce` with `op = MPI_PROD`
  interface Reduce_Prod
#define BUD_MP_COMM_NAME Reduce_Prod
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_Prod
  !> Interface for `MPI_IReduce` with `op = MPI_PROD`
  interface IReduce_Prod
#define BUD_MP_COMM_NAME IReduce_Prod
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_Prod
  
  !> Interface for `MPI_Reduce` with `op = MPI_SUM`
  interface Reduce_Sum
#define BUD_MP_COMM_NAME Reduce_Sum
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_Sum
  !> Interface for `MPI_IReduce` with `op = MPI_SUM`
  interface IReduce_Sum
#define BUD_MP_COMM_NAME IReduce_Sum
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_Sum

  
  ! INTEGER/REAL
# undef BUD_IS_COMPLEX
  
  !> Interface for `MPI_Reduce` with `op = MPI_MAX`
  interface Reduce_Max
#define BUD_MP_COMM_NAME Reduce_Max
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_Max
  !> Interface for `MPI_IReduce` with `op = MPI_MAX`
  interface IReduce_Max
#define BUD_MP_COMM_NAME IReduce_Max
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_Max
  
  !> Interface for `MPI_Reduce` with `op = MPI_MIN`
  interface Reduce_Min
#define BUD_MP_COMM_NAME Reduce_Min
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_Min
  !> Interface for `MPI_IReduce` with `op = MPI_MIN`
  interface IReduce_Min
#define BUD_MP_COMM_NAME IReduce_Min
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_Min

  
  ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL

  !> Interface for `MPI_Reduce` with `op = MPI_LAND`
  interface Reduce_LAND
#define BUD_MP_COMM_NAME Reduce_LAND
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_LAND
  !> Interface for `MPI_IReduce` with `op = MPI_LAND`
  interface IReduce_LAND
#define BUD_MP_COMM_NAME IReduce_LAND
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_LAND

  !> Interface for `MPI_Reduce` with `op = MPI_LOR`
  interface Reduce_LOR
#define BUD_MP_COMM_NAME Reduce_LOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_LOR
  !> Interface for `MPI_IReduce` with `op = MPI_LOR`
  interface IReduce_LOR
#define BUD_MP_COMM_NAME IReduce_LOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_LOR

  !> Interface for `MPI_Reduce` with `op = MPI_LXOR`
  interface Reduce_LXOR
#define BUD_MP_COMM_NAME Reduce_LXOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce_LXOR
  !> Interface for `MPI_IReduce` with `op = MPI_LXOR`
  interface IReduce_LXOR
#define BUD_MP_COMM_NAME IReduce_LXOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IReduce_LXOR

  
# undef BUD_IS_LOGICAL
# define BUD_IS_LOGICAL
# define BUD_IS_INTEGER
# define BUD_IS_REAL
# define BUD_IS_COMPLEX

  !> Interface for `MPI_AllReduce`
  interface AllReduce
#define BUD_MP_COMM_NAME AllReduce
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce
  !> Interface for `MPI_IAllReduce`
  interface IAllReduce
#define BUD_MP_COMM_NAME IAllReduce
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce
  
  
  ! INTEGER/REAL/COMPLEX
# undef BUD_IS_LOGICAL
  
  !> Interface for `MPI_AllReduce` with `op = MPI_PROD`
  interface AllReduce_Prod
#define BUD_MP_COMM_NAME AllReduce_Prod
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_Prod
  !> Interface for `MPI_IAllReduce` with `op = MPI_PROD`
  interface IAllReduce_Prod
#define BUD_MP_COMM_NAME IAllReduce_Prod
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_Prod
  
  !> Interface for `MPI_AllReduce` with `op = MPI_SUM`
  interface AllReduce_Sum
#define BUD_MP_COMM_NAME AllReduce_Sum
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_Sum
  !> Interface for `MPI_IAllReduce` with `op = MPI_SUM`
  interface IAllReduce_Sum
#define BUD_MP_COMM_NAME IAllReduce_Sum
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_Sum
  
  
  ! INTEGER/REAL
# undef BUD_IS_COMPLEX
  
  !> Interface for `MPI_AllReduce` with `op = MPI_MAX`
  interface AllReduce_Max
#define BUD_MP_COMM_NAME AllReduce_Max
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_Max
  !> Interface for `MPI_IAllReduce` with `op = MPI_MAX`
  interface IAllReduce_Max
#define BUD_MP_COMM_NAME IAllReduce_Max
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_Max
  
  !> Interface for `MPI_AllReduce` with `op = MPI_MIN`
  interface AllReduce_Min
#define BUD_MP_COMM_NAME AllReduce_Min
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_Min
  !> Interface for `MPI_IAllReduce` with `op = MPI_MIN`
  interface IAllReduce_Min
#define BUD_MP_COMM_NAME IAllReduce_Min
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_Min


  ! LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# define BUD_IS_LOGICAL
  
  !> Interface for `MPI_AllReduce` with `op = MPI_LAND`
  interface AllReduce_LAND
#define BUD_MP_COMM_NAME AllReduce_LAND
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_LAND
  !> Interface for `MPI_IAllReduce` with `op = MPI_LAND`
  interface IAllReduce_LAND
#define BUD_MP_COMM_NAME IAllReduce_LAND
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_LAND

  !> Interface for `MPI_AllReduce` with `op = MPI_LOR`
  interface AllReduce_LOR
#define BUD_MP_COMM_NAME AllReduce_LOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_LOR
  !> Interface for `MPI_IAllReduce` with `op = MPI_LOR`
  interface IAllReduce_LOR
#define BUD_MP_COMM_NAME IAllReduce_LOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_LOR

  !> Interface for `MPI_AllReduce` with `op = MPI_LXOR`
  interface AllReduce_LXOR
#define BUD_MP_COMM_NAME AllReduce_LXOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce_LXOR
  !> Interface for `MPI_IAllReduce` with `op = MPI_LXOR`
  interface IAllReduce_LXOR
#define BUD_MP_COMM_NAME IAllReduce_LXOR
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IAllReduce_LXOR

  
#undef BUD_IS_LOGICAL
#undef BUD_IS_INTEGER
#undef BUD_IS_REAL
#undef BUD_IS_COMPLEX
  
#endif
  
  !> Interface for `MPI_Barrier`
  interface Barrier
    module procedure Barrier_
  end interface
  public :: Barrier
  !> Interface for `MPI_IBarrier`
  interface IBarrier
    module procedure IBarrier_
  end interface
  public :: IBarrier

#ifdef BUD_MPI
  !> Interface for `MPI_Comm_Split` and `MPI_Comm_Split_Type`
  interface Comm_Split
    module procedure Comm_Split_, Comm_Split_type_
  end interface
  public :: Comm_Split

  !> Interface for `MPI_Comm_Compare`
  interface Comm_Compare
    module procedure Comm_Compare_
  end interface
  public :: Comm_Compare

  !> Interface for `MPI_Comm_Create` and `MPI_Comm_Create_group`
  interface Comm_Create
    module procedure Comm_Create_, Comm_Create_Commgrp_
    module procedure Comm_Create_group_, Comm_Create_group_Commgrp_
  end interface
  public :: Comm_Create
#endif
  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "bud_common.inc"


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

#ifdef BUD_MPI
    ! Currently we do not allow external memory
    ! tracking.
    if ( this%Comm /= MPI_Comm_Null ) then
      ! reset everything
      ! A disconnect is less obstructive than
      ! a free. Perhaps we may choose to use Free
      ! anyway... ?
      call MPI_Group_Free(this%Grp, this%error_)
      call MPI_Comm_Disconnect(this%Comm, this%error_)
    end if
#endif
    
    ! Ensure they are nullified
    this%Comm = MPI_Comm_Null
    this%Grp = MPI_Group_Null
    
    this%P = 0
    this%NP = 1

  end subroutine delete_


  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full stuff (Comm_dup is issued
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    if ( from%D%comm == MPI_Comm_Null ) then
      call new_remote(to, from%D%P, from%D%NP)
    else
      call new(to, from%D%comm)
    end if

    ! Copy the common data-content
    call common_copy_(from, to)

  end subroutine copy_


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

  !> Basic routine for initializing a new communicator
  !!
  !! @param[inout] this the @bud distribution container
  !! @param[in] Comm the communicator that we will dublicate and attach
  !! @param[in] dup @opt=.true. whether the input communicator is dublicated.
  subroutine new_(this, Comm, dup)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: Comm
    logical, intent(in), optional :: dup
    logical :: ldup

    call initialize(this)

#ifdef BUD_MPI
    ! duplicate communicator (ensures that we can delete it again)
    ldup = .true.
    if ( present(dup) ) ldup = dup
    if ( ldup ) then
      call MPI_Comm_dup(Comm, this%D%comm, this%D%error_)
    else
      this%D%comm = Comm
    end if

    ! Create the group
    call MPI_Comm_group(this%D%comm, this%D%Grp, this%D%error_)
    
    ! Figure out number of processors and the rank
    call MPI_Comm_Rank( this%D%comm, this%D%P, this%D%error_)
    call MPI_Comm_Size( this%D%comm, this%D%NP, this%D%error_)

#else

    this%D%comm = Comm
    this%D%P = 0
    this%D%NP = 1
#endif

  end subroutine new_

  !! @param[inout] this the @bud distribution container
  !! @param[in] rank the rank of this processor
  !! @param[in] size the size of the communicator
  subroutine new_remote_(this, rank, size)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: rank, size

    call initialize(this)

    this%D%comm = MPI_Comm_Null
    this%D%grp = MPI_Group_Null

    this%D%P = rank
    this%D%NP = size
        
  end subroutine new_remote_

#ifdef BUD_MPI
  !> Create a fake communicator for those ranks in `child` which are not in `parent`
  !!
  !! This routine requires all ranks in `child` to participate.
  !! It requires that the `parent` communicator is defined for 
  !! some of the ranks, while the remaining ranks in `child`, not in `parent`, should pass an empty communicator.
  !!
  !! Upon exit those ranks in `child` which are not in `parent` will have
  !! a communicator with size of the `parent` which does exist.
  !! However, this communicator does not have any MPI_Comm
  !! assigned and cannot be used for communication.
  !! It should *only* be used to query stuff on the remaining processors
  !! regarding the size of the communicator.
  !!
  !! This routine together with #Bcast_ranks may be used
  !! to distribute data through a common communicator.
  !! @param[inout] parent the sub communicator
  !! @param[inout] child the communicator which contains `parent`
  subroutine new_remote_child_(parent, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: child

    integer :: child_size
    
    if ( .not. is_communicator(child) ) return

    ! Retrieve the root of the child
    call child_Bcast(parent, child, size = child_size )

    ! Create a new distribution on those that is not child
    if ( .not. is_communicator(child) ) then
      ! All are the root!
      call new_remote(child, 0, child_size)
    end if
    
  end subroutine new_remote_child_
#endif


#ifdef BUD_MPI
  !> @cond BUD_DEVELOPER
  ! Add all interfaces
# define BUD_TYPE_VAR integer
# define BUD_IS_INTEGER
# define BUD_TYPE_VAR_PREC ii_
# define BUD_TYPE_VAR_MP MPI_Integer
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"
  
# define BUD_TYPE_VAR integer
# define BUD_IS_INTEGER
# define BUD_TYPE_VAR_PREC il_
# define BUD_TYPE_VAR_MP MPI_Integer8
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

# define BUD_TYPE_VAR real
# define BUD_IS_REAL
# define BUD_TYPE_VAR_PREC rr_
# define BUD_TYPE_VAR_MP MPI_Real
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

# define BUD_TYPE_VAR real
# define BUD_IS_REAL
# define BUD_TYPE_VAR_PREC rd_
# define BUD_TYPE_VAR_MP MPI_Double_Precision
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

# define BUD_TYPE_VAR complex
# define BUD_IS_COMPLEX
# define BUD_TYPE_VAR_PREC rr_
# define BUD_TYPE_VAR_MP MPI_Complex
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

# define BUD_TYPE_VAR complex
# define BUD_IS_COMPLEX
# define BUD_TYPE_VAR_PREC rd_
# define BUD_TYPE_VAR_MP MPI_Double_Complex
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

# define BUD_TYPE_VAR logical
# define BUD_IS_LOGICAL
# define BUD_TYPE_VAR_MP MPI_Logical
# define BUD_DIM 0
# include "MP_Comm_routine.inc"
# define BUD_DIM 1
# include "MP_Comm_routine.inc"
# define BUD_DIM 2
# include "MP_Comm_routine.inc"
# define BUD_DIM 3
# include "MP_Comm_routine.inc"
# define BUD_DIM 4
# include "MP_Comm_routine.inc"

#endif
  
  ! Common things
  subroutine Barrier_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
#ifdef BUD_MPI
    if ( .not. is_initd(this) ) return

    call MPI_Barrier(this%D%comm, this%D%error_)
#endif
      
  end subroutine

  subroutine IBarrier_(this, request)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(inout) :: request

#ifdef BUD_MPI
    if ( .not. is_initd(this) ) return

    call MPI_IBarrier(this%D%comm, request, this%D%error_)
#endif
    
  end subroutine

  !> @endcond BUD_DEVELOPER
  

  !> Query whether a communicator is not MPI_COMM_NULL
  !!
  !! This may be used to identify `fake` communicators
  !! vs. real communicators.
  elemental function is_comm_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    is = is_initd(this)
    if ( is ) is = this%D%Comm /= MPI_Comm_Null
  end function is_comm_

  
  !> Query whether the associated group is not MPI_GROUP_NULL
  !!
  !! This may be used to identify `fake` communicators
  !! vs. real communicators.
  elemental function is_group_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    is = is_initd(this)
    if ( is ) is = this%D%grp /= MPI_Group_Null
  end function is_group_


  !> Function for retrieving the latest error message
  elemental function get_MPIerr_(this) result(err)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: err
    if ( is_initd(this) ) then
      err = this%D%error_
    else
      err = MPI_SUCCESS
    end if
  end function get_MPIerr_


  !> Return whether the last MPI call was a success
  elemental function is_MPIsuccess_(this) result(success)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: success
    if ( is_initd(this) ) then
      success = this%D%error_ == MPI_SUCCESS
    else
      success = .true.
    end if
  end function is_MPIsuccess_

#ifdef BUD_MPI

  !> Bcast information about a child communicator
  !!
  !! @param[inout] parent the receiving communicator
  !! @param[inout] child the communicator we wish to query
  !! @param[out] root @opt=-1 the rank of the root in the `child` communicator
  !! @param[out] size @opt=0 the size of the `child` communicator
  subroutine child_Bcast_(parent, child, D, root, size)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    integer, intent(in), optional :: D ! should neven be used
    integer, intent(out), optional :: root, size

    integer :: tmp

    ! signal no root (in case there is an error)
    if ( present(root) ) root = -1
    if ( present(size) ) size = 0
    
    if ( .not. is_communicator(parent) ) return

    if ( is_communicator(child) ) then
      if ( comm_rank(child) == 0 ) then
        if ( present(root) ) root = comm_rank(parent)
        if ( present(size) ) size = comm_size(child)
      end if
    end if

    ! Reduce the node rank
    if ( present(root) ) then
      call AllReduce_Max(root, tmp, parent)
      root = tmp
    end if
    if ( present(size) ) then
      call AllReduce_Max(size, tmp, parent)
      size = tmp
    end if
    
  end subroutine child_Bcast_

  
  !> Bcast the ranks of all processors in a `child` communicator to a `parent` communicator
  !!
  !! @param[inout] parent the receiving communicator (must contain all processors of `child`)
  !! @param[inout] child communicator which is queried in the `parent` communicator
  !! @param[in] n size of the rank list (must be larger than the size of `child`, else -2 is returned in all elements)
  !! @param[out] ranks list of ranks
  subroutine child_Bcast_ranks_(parent, child, n, ranks)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    integer, intent(in) :: n
    integer, intent(out) :: ranks(n)

    integer, allocatable :: tmp(:)

    ! signal no root (in case there is an error)
    ranks = -1
    if ( .not. is_communicator(parent) ) return

    if ( is_communicator(child) ) then
      if ( comm_size(child) < n ) then
        ! do nothing, this will signal a too small array
        ranks = -2
      else
        ranks(comm_rank(child)+1) = comm_rank(parent)
      end if
    end if

    ! Reduce the node rank
    allocate(tmp(n))
    call AllReduce_Max(ranks, tmp, parent)
    ranks = tmp
    deallocate(tmp)

  end subroutine child_Bcast_ranks_



  !> Split a communicator to a new communicator.
  !!
  !! Wrapper for `MPI_Comm_Split`
  !! @param[inout] this the communicator to split
  !! @param[in] color the color of the new communicators
  !! @param[in] key designation of the rank-assignment for the communicators
  !! @param[inout] split the new communicator
  subroutine Comm_Split_(this, color, key, split)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: color, key
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: split
    integer(ii_) :: Com

    call delete(split)
    if ( .not. is_communicator(this) ) return

    ! Split the communicators
    call MPI_Comm_Split(this%D%comm, color, key, &
      Com, this%D%error_)

    if ( this%D%error_ == MPI_SUCCESS ) then
      call new(split, Com, dup = .false.)
    end if

  end subroutine Comm_Split_

  !> Split a communicator to a new communicator using specific split types
  !!
  !! Wrapper for `MPI_Comm_Split`
  !! @param[inout] this the communicator to split
  !! @param[in] split_type the type of split
  !! @param[in] key designation of the rank-assignment for the communicators
  !! @param[in] info an `MPI_Info` parameter for information passing
  !! @param[inout] split the new communicator
  subroutine Comm_Split_type_(this, split_type, key, info, split)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: split_type, key
    integer(ii_), intent(inout) :: info
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: split
    integer(ii_) :: Com

    call delete(split)
    if ( .not. is_communicator(this) ) return
    
    ! Split the communicators
    call MPI_Comm_Split_Type(this%D%comm, split_type, key, &
      info, Com, this%D%error_)

    if ( this%D%error_ == MPI_SUCCESS ) then
      call new(split, Com, dup = .false.)
    end if

  end subroutine Comm_Split_Type_


  !> Wrapper for `MPI_Comm_Compare`
  !! @param[inout] comm1 first communicator
  !! @param[inout] comm2 second communicator
  !! @param[out] result comparison of the communicators
  subroutine Comm_Compare_(comm1, comm2, result)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: comm1, comm2
    integer(ii_), intent(inout) :: result

    ! Split the communicators
    call MPI_Comm_Compare(comm1%D%comm, comm2%D%comm, result, &
      comm1%D%error_)
    comm2%D%error_ = comm1%D%error_

  end subroutine Comm_Compare_

  !> Wrapper for `MPI_Comm_Create`
  !! @param[inout] parent parent communicator
  !! @param[in] group the group of the communicator
  !! @param[inout] child new communicator
  subroutine Comm_Create_(parent, group, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    integer(ii_), intent(in) :: group
    integer(ii_) :: Com

    call MPI_Comm_Create(parent%D%comm, group, Com, parent%D%error_)

    if ( is_success_MPI(parent) ) then
      call new(child, Com, dup = .false.)
    else
      call delete(child)
    end if

  end subroutine Comm_Create_

  !> Wrapper for `MPI_Comm_Create`
  !! @param[inout] parent parent communicator
  !! @param[in] comm_group a communicator with the associated group
  !! @param[inout] child new communicator
  subroutine Comm_Create_CommGrp_(parent, group, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: group

    call Comm_Create_(parent, group%D%grp, child)
    
  end subroutine Comm_Create_CommGrp_


  !> Wrapper for `MPI_Comm_Create_group`
  !! @param[inout] parent parent communicator
  !! @param[in] group the group of the communicator
  !! @param[in] tag the tag of the new communicator
  !! @param[inout] child new communicator
  subroutine Comm_Create_group_(parent, group, tag, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    integer(ii_), intent(in) :: group, tag
    integer(ii_) :: Com

    call MPI_Comm_Create_Group(parent%D%comm, group, tag, Com, parent%D%error_)

    if ( is_success_MPI(parent) ) then
      call new(child, Com, dup = .false.)
    else
      call delete(child)
    end if

  end subroutine Comm_Create_Group_

  !> Wrapper for `MPI_Comm_Create`
  !! @param[inout] parent parent communicator
  !! @param[in] comm_group a communicator with the associated group
  !! @param[in] tag the tag of the new communicator
  !! @param[inout] child new communicator
  subroutine Comm_Create_group_CommGrp_(parent, group, tag, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: group
    integer(ii_), intent(in) :: tag

    call Comm_Create_group_(parent, group%D%grp, tag, child)
    
  end subroutine Comm_Create_Group_CommGrp_

#endif
  
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
      write(fmt, '(a,i0,a)') '(t',lindent,',4a,3(i0,a))'
      write(*,fmt) "<", trim(name), " (remote)Comm", &
        ", P=",this%D%P, ", NP=",this%D%NP, &
        ", refs: ", references(this), ">"
    else
      write(fmt, '(a,i0,a)') '(t',lindent,',3a,4(i0,a))'
      write(*,fmt) "<", trim(name), " Comm=", this%D%Comm, &
        ", P=",this%D%P, ", NP=",this%D%NP, &
        ", refs: ", references(this), ">"
    end if
    
  end subroutine print_

end module
!> @}


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

