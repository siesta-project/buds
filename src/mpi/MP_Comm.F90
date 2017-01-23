  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"

! This bud is not able to read/write itself.
#define BUD_NO_IO

! Define default variable for the communicator
#define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,MP_Comm)
#define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,MP_Comm)

module BUD_MOD_NAME

#include "bud_mpi.inc"

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"


  !> A bud for wrapping the MPI library
  type BUD_TYPE_NAME

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

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
#define BUD_MP_COMM_NAME Send_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBSend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRSend_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISSend_N
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Send_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Send_Init_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_Init_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_Init_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_Init_N
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Recv
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Recv_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRecv
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRecv_N
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Recv_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Recv_Init_N
# include "MP_Comm_routine_interface.inc"

#define BUD_MP_COMM_NAME Bcast
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Bcast_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBcast
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBcast_N
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
#define BUD_MP_COMM_NAME Reduce_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_N
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
#define BUD_MP_COMM_NAME AllReduce_N
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_N
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

    procedure, private, :: Comm_split_
    procedure, private :: comm_split_type_

    procedure, private :: Comm_create_, comm_create_commgrp_
    procedure, private :: Comm_create_group_, comm_create_group_commgrp_

    !> See [[new]]
    procedure, public :: new => new_

    !> See [[new_remote]]
#ifdef BUD_MPI
    generic, public :: new_remote => new_remote_, new_remote_child_
#else
    procedure, public :: new_remote => new_remote_
#endif

    !> See [[communicator]]
    procedure, public :: communicator => comm_

    !> See [[group]]
    procedure, public :: group => group_

    !> See [[comm_rank]]
    procedure, public :: comm_rank => rank_
    !> See [[comm_size]]
    procedure, public :: comm_size => size_

    !> See [[is_communicator]]
    procedure, public :: is_communicator => is_comm_

    !> See [[is_group]]
    procedure, public :: is_group => is_group_

    !> See [[error_mpi]]
    procedure, public :: error_mpi => get_MPIerr_

    !> See [[is_success_mpi]]
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

    procedure, public, pass(this) :: Barrier => Barrier_
    procedure, public, pass(this) :: IBarrier => IBarrier_

    procedure, public, pass(this) :: Get_Count => Get_Count_

    procedure, public, pass(this) :: Wait => Wait_
    procedure, public, pass(this) :: WaitAll => WaitAll_
    procedure, public, pass(this) :: WaitAny => WaitAny_
    procedure, public, pass(this) :: Test => Test_
    procedure, public, pass(this) :: TestAll => TestAll_
    procedure, public, pass(this) :: TestAny => TestAny_
    procedure, public, pass(this) :: Test_Cancelled => Test_Cancelled_

# ifdef BUD_MPI
    generic, public, pass(this) :: Comm_split => comm_split_, comm_split_type_
    procedure, public, pass(this) :: Comm_Compare => comm_compare_

    generic, public, pass(this) :: Comm_Create => comm_Create_, comm_create_commgrp_, &
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

  !> bud container for message passing information.
  type BUD_TYPE_NAME_

    !> The associated communicator
    integer(ii_) :: Comm = MPI_Comm_Null
    !> The group that encompass the equivalent communicator
    integer(ii_) :: Grp = MPI_Group_Null

    !> The associated processor number in the associated communicator
    integer(ii_) :: rank = 0
    !> The number of processors in the associated communicator
    integer(ii_) :: size = 1

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


  !> Query communicator of the distribution
  !!
  !! @note
  !! Do not free this communicator.
  !! @endnote
  interface communicator
    module procedure comm_
  end interface
  public :: communicator

  !> Query group of the distribution
  !!
  !! @note
  !! Do not free this group.
  !! @endnote
  interface group
    module procedure group_
  end interface
  public :: group

  !> Query ID for current processor in distribution
  interface comm_rank
    module procedure rank_
  end interface
  public :: comm_rank

  !> Query number of processors in distribution
  interface comm_size
    module procedure size_
  end interface
  public :: comm_size

  !> See [[comm_size]]
  interface size
    module procedure size_
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
#define BUD_MP_COMM_NAME Send_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Send
  !> Interface for `MPI_BSend`
  interface BSend
#define BUD_MP_COMM_NAME BSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: BSend
  !> Interface for `MPI_RSend`
  interface RSend
#define BUD_MP_COMM_NAME RSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: RSend
  !> Interface for `MPI_SSend`
  interface SSend
#define BUD_MP_COMM_NAME SSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: SSend
  !> Interface for `MPI_ISend`
  interface ISend
#define BUD_MP_COMM_NAME ISend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: ISend
  !> Interface for `MPI_IBSend`
  interface IBSend
#define BUD_MP_COMM_NAME IBSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IBSend
  !> Interface for `MPI_IRSend`
  interface IRSend
#define BUD_MP_COMM_NAME IRSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IRSend
  !> Interface for `MPI_ISSend`
  interface ISSend
#define BUD_MP_COMM_NAME ISSend
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME ISSend_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: ISSend

  !> Interface for `MPI_Send_Init`
  interface Send_Init
#define BUD_MP_COMM_NAME Send_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Send_Init_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Send_Init
  !> Interface for `MPI_BSend_Init`
  interface BSend_Init
#define BUD_MP_COMM_NAME BSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME BSend_Init_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: BSend_Init
  !> Interface for `MPI_RSend_Init`
  interface RSend_Init
#define BUD_MP_COMM_NAME RSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME RSend_Init_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: RSend_Init
  !> Interface for `MPI_SSend_Init`
  interface SSend_Init
#define BUD_MP_COMM_NAME SSend_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME SSend_Init_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: SSend_Init

  !> Interface for `MPI_Recv`
  interface Recv
#define BUD_MP_COMM_NAME Recv
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Recv_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Recv
  !> Interface for `MPI_IRecv`
  interface IRecv
#define BUD_MP_COMM_NAME IRecv
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IRecv_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: IRecv

  !> Interface for `MPI_Recv_Init`
  interface Recv_Init
#define BUD_MP_COMM_NAME Recv_Init
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Recv_Init_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Recv_Init

  !> Interface for `MPI_Bcast`
  interface Bcast
#define BUD_MP_COMM_NAME Bcast
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME Bcast_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Bcast
  !> Interface for `MPI_IBcast`
  interface IBcast
#define BUD_MP_COMM_NAME IBcast
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IBcast_N
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
#define BUD_MP_COMM_NAME Reduce_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: Reduce
  !> Interface for `MPI_IReduce`
  interface IReduce
#define BUD_MP_COMM_NAME IReduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IReduce_N
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
#define BUD_MP_COMM_NAME AllReduce_N
# include "MP_Comm_routine_interface.inc"
  end interface
  public :: AllReduce
  !> Interface for `MPI_IAllReduce`
  interface IAllReduce
#define BUD_MP_COMM_NAME IAllReduce
# include "MP_Comm_routine_interface.inc"
#define BUD_MP_COMM_NAME IAllReduce_N
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

  !> Interface for `MPI_Get_Count`
  interface Get_Count
    module procedure Get_Count_
  end interface
  public :: Get_Count

  !> Interface for `MPI_Wait`
  interface Wait
    module procedure Wait_
    module procedure Wait_err_
  end interface
  public :: Wait
  !> Interface for `MPI_WaitAll`
  interface WaitAll
    module procedure WaitAll_
    module procedure WaitAll_err_
  end interface
  public :: WaitAll
  !> Interface for `MPI_WaitAny`
  interface WaitAny
    module procedure WaitAny_
    module procedure WaitAny_err_
  end interface
  public :: WaitAny

  !> Interface for `MPI_Test`
  interface Test
    module procedure Test_
    module procedure Test_err_
  end interface
  public :: Test
  !> Interface for `MPI_TestAll`
  interface TestAll
    module procedure TestAll_
    module procedure TestAll_err_
  end interface
  public :: TestAll
  !> Interface for `MPI_TestAny`
  interface TestAny
    module procedure TestAny_
    module procedure TestAny_err_
  end interface
  public :: TestAny
  !> Interface for `MPI_Test_Cancelled`
  interface Test_Cancelled
    module procedure Test_Cancelled_
    module procedure Test_Cancelled_err_
  end interface
  public :: Test_Cancelled

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


  !> Internal routine for cleaning up the data container.
  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this

    this%error = 0
#ifdef BUD_MPI
    ! Currently we do not allow external memory
    ! tracking.
    if ( this%D%Comm /= MPI_Comm_Null ) then
      ! reset everything
      ! free the group and communicator...
      call MPI_Group_Free(this%D%Grp, this%error)
      call MPI_Comm_Free(this%D%Comm, this%error)
    end if
#endif

    ! Ensure they are nullified
    this%D%Comm = MPI_Comm_Null
    this%D%Grp = MPI_Group_Null

    this%D%rank = 0
    this%D%size = 1

  end subroutine delete_



  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full stuff (Comm_dup is issued
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    if ( from%D%comm == MPI_Comm_Null ) then
      call new_remote(to, from%D%rank, from%D%size)
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
  elemental function rank_(this) result(rank)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: rank
    if ( is_initd(this) ) then
      rank = this%D%rank
    else
      rank = -1
    end if
  end function rank_

  !> Query the number of processors in the communicator
  elemental function size_(this) result(size)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: size
    if ( is_initd(this) ) then
      size = this%D%size
    else
      size = 0
    end if
  end function size_

  !> Basic routine for initializing a new communicator
  !!
  !! @param[inout] this the bud distribution container
  !! @param[in] Comm the communicator that we will dublicate and attach
  !! @param[in] dup opt=.true. whether the input communicator is dublicated.
  subroutine new_(this, Comm, dup)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: Comm
    logical, intent(in), optional :: dup
#ifdef BUD_MPI
    logical :: ldup
#endif

    call initialize(this)

#ifdef BUD_MPI
    ! duplicate communicator (ensures that we can delete it again)
    ldup = .true.
    if ( present(dup) ) ldup = dup
    if ( ldup ) then
      call MPI_Comm_dup(Comm, this%D%comm, this%error)
    else
      this%D%comm = Comm
    end if

    ! Create the group
    call MPI_Comm_group(this%D%comm, this%D%Grp, this%error)

    ! Figure out number of processors and the rank
    call MPI_Comm_Rank( this%D%comm, this%D%rank, this%error)
    call MPI_Comm_Size( this%D%comm, this%D%size, this%error)

#else
    this%D%comm = Comm
    this%D%rank = 0
    this%D%size = 1

#endif

  end subroutine new_

  !! @param[inout] this the bud distribution container
  !! @param[in] rank the rank of this processor
  !! @param[in] size the size of the communicator
  subroutine new_remote_(this, rank, size)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: rank, size

    call initialize(this)

    this%D%comm = MPI_Comm_Null
    this%D%grp = MPI_Group_Null

    this%D%rank = rank
    this%D%size = size

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
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    if ( .not. is_initd(this) ) return

    call MPI_Barrier(this%D%comm, this%error)
#endif

  end subroutine

  subroutine IBarrier_(this, request)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(inout) :: request

#ifdef BUD_MPI
    if ( .not. is_initd(this) ) return

    call MPI_IBarrier(this%D%comm, request, this%error)
#endif

  end subroutine

  subroutine Get_Count_(status, prec, count, this)
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    integer, intent(in) :: prec
    integer, intent(out) :: count
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_Get_Count(status, prec, count, this%error)
#else
    count = 0
#endif

  end subroutine Get_Count_


  ! Wait routines
  subroutine Wait_(req, status, this)
    integer, intent(inout) :: req
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_Wait(req, status, this%error)
#endif

  end subroutine Wait_
  subroutine Wait_err_(req, status, err)
    integer, intent(inout) :: req
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_Wait(req, status, err)
#else
    err = MPI_SUCCESS
#endif

  end subroutine Wait_Err_

  subroutine WaitAll_(n, req, status, this)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE,n)
#else
    integer, intent(inout) :: status(:,:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_WaitAll(n, req, status, this%error)
#endif

  end subroutine WaitAll_
  subroutine WaitAll_err_(n, req, status, err)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE,n)
#else
    integer, intent(inout) :: status(:,:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_WaitAll(n, req, status, err)
#else
    err = MPI_SUCCESS
#endif

  end subroutine WaitAll_Err_

  subroutine WaitAny_(n, req, index, status, this)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    integer, intent(out) :: index
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_WaitAny(n, req, index, status, this%error)
#else
    index = 0
#endif

  end subroutine WaitAny_
  subroutine WaitAny_err_(n, req, index, status, err)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    integer, intent(out) :: index
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_WaitAny(n, req, index, status, err)
#else
    index = 0
    err = MPI_SUCCESS
#endif

  end subroutine WaitAny_Err_

  ! Test routines
  subroutine Test_(req, flag, status, this)
    integer, intent(inout) :: req
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_Test(req, flag, status, this%error)
#else
    flag = .true.
#endif

  end subroutine Test_
  subroutine Test_err_(req, flag, status, err)
    integer, intent(inout) :: req
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_Test(req, flag, status, err)
#else
    flag = .true.
    err = MPI_SUCCESS
#endif

  end subroutine Test_err_

  subroutine TestAll_(n, req, flag, status, this)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE,n)
#else
    integer, intent(inout) :: status(:,:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_TestAll(n, req, flag, status, this%error)
#else
    flag = .true.
#endif

  end subroutine TestAll_
  subroutine TestAll_err_(n, req, flag, status, err)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE,n)
#else
    integer, intent(inout) :: status(:,:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_TestAll(n, req, flag, status, err)
#else
    flag = .true.
    err = MPI_SUCCESS
#endif

  end subroutine TestAll_err_

  subroutine TestAny_(n, req, index, flag, status, this)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    integer, intent(out) :: index
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_TestAny(n, req, index, flag, status, this%error)
#else
    index = 0
    flag = .true.
#endif

  end subroutine TestAny_
  subroutine TestAny_err_(n, req, index, flag, status, err)
    integer, intent(in) :: n
    integer, intent(inout) :: req(n)
    integer, intent(out) :: index
    logical, intent(out) :: flag
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_TestAny(n, req, index, flag, status, err)
#else
    index = 0
    flag = .true.
    err = MPI_SUCCESS
#endif

  end subroutine TestAny_err_

  subroutine Test_Cancelled_(status, flag, this)
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    logical, intent(out) :: flag
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_MPI
    call MPI_Test_Cancelled(status, flag, this%error)
#else
    flag = .false.
#endif

  end subroutine Test_Cancelled_
  subroutine Test_Cancelled_err_(status, flag, err)
#ifdef BUD_MPI
    integer, intent(inout) :: status(MPI_STATUS_SIZE)
#else
    integer, intent(inout) :: status(:)
#endif
    logical, intent(out) :: flag
    integer, intent(out) :: err

#ifdef BUD_MPI
    call MPI_Test_Cancelled(status, flag, err)
#else
    flag = .false.
    err = MPI_SUCCESS
#endif

  end subroutine Test_Cancelled_Err_


  !> Query whether a communicator is not MPI_COMM_NULL
  !!
  !! This may be used to identify `fake` communicators
  !! vs. real communicators.
  elemental function is_comm_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    if ( is_initd(this) ) then
      is = this%D%Comm /= MPI_Comm_Null
    else
      is = .false.
    end if
  end function is_comm_


  !> Query whether the associated group is not MPI_GROUP_NULL
  !!
  !! This may be used to identify `fake` communicators
  !! vs. real communicators.
  elemental function is_group_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: is
    if ( is_initd(this) ) then
      is = this%D%grp /= MPI_Group_Null
    else
      is = .false.
    end if
  end function is_group_


  !> Function for retrieving the latest error message
  elemental function get_MPIerr_(this) result(err)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(ii_) :: err
    if ( is_initd(this) ) then
      err = this%error
    else
      err = MPI_SUCCESS
    end if
  end function get_MPIerr_


  !> Return whether the last MPI call was a success
  elemental function is_MPIsuccess_(this) result(success)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    logical :: success
    if ( is_initd(this) ) then
      success = this%error == MPI_SUCCESS
    else
      success = .true.
    end if
  end function is_MPIsuccess_

#ifdef BUD_MPI

  !> Bcast information about a child communicator
  !!
  !! @param[inout] parent the receiving communicator
  !! @param[inout] child the communicator we wish to query
  !! @param[out] root opt=-1 the rank of the root in the `child` communicator
  !! @param[out] size opt=0 the size of the `child` communicator
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
      Com, this%error)

    if ( this%error == MPI_SUCCESS ) then
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
      info, Com, this%error)

    if ( this%error == MPI_SUCCESS ) then
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
      comm1%error)
    comm2%error = comm1%error

  end subroutine Comm_Compare_

  !> Wrapper for `MPI_Comm_Create`
  !! @param[inout] parent parent communicator
  !! @param[in] group the group of the communicator
  !! @param[inout] child new communicator
  subroutine Comm_Create_(parent, group, child)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: parent, child
    integer(ii_), intent(in) :: group
    integer(ii_) :: Com

    call MPI_Comm_Create(parent%D%comm, group, Com, parent%error)

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

    call MPI_Comm_Create_Group(parent%D%comm, group, tag, Com, parent%error)

    if ( is_success_MPI(parent) ) then
      call new(child, Com, dup = .false.)
    else
      call delete(child)
      call set_error(child, 1)
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
  !! @param[in] this data type
  !! @param[in] info opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent opt=1 possible indentation of printed statement
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
        ", P=", comm_rank(this), ", NP=", comm_size(this), &
        ", refs: ", references(this), ">"
    else
      write(fmt, '(a,i0,a)') '(t',lindent,',3a,4(i0,a))'
      write(*,fmt) "<", trim(name), " Comm=", this%D%Comm, &
        ", P=", comm_rank(this), ", NP=", comm_size(this), &
        ", refs: ", references(this), ">"
    end if

  end subroutine print_

end module


  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_VAR
#undef BUD_PREC

#include "bud_cleanup.inc"


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

