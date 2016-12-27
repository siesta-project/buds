  ! @@LICENSE@@ see Copyright notice in the top-directory

  !> @defgroup dist1d-array Distributed (1D), dense matrix with data
  !! @ingroup bud-intrinsic
  !!
  !! @bud containing a distribution _and_ the
  !! associated data corresponding to the dense matrix elements.
  !!

#ifdef BUD_MPI
  !> This module makes heavy usage of the
  !! MPI variables
# if defined(BUD_MPI_INCLUDE)
  include BUD_MPI_INCLUDE
# elif defined(BUD_MPI_MODULE)
  use BUD_MPI_MODULE
# else
  use mpi
# endif
#endif

  use BUD_CC3(BUD_MOD,_,MP_Comm)

#include "bud_common_declarations.inc"


!# define BUD_INCLUDE_TYPE "Dist1D_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "Dist1D_Array_type_.inc"


  !> Retrieve the 1D distribution
  interface dist1d
    module procedure get_elem1_
  end interface
  public :: dist1d

  !> Retrieve pointer to the 1D distribution
  interface dist1d_p
    module procedure get_elem1p_
  end interface
  public :: dist1d_p

  !> Retrieve the array @bud
  interface array
    module procedure get_elem2_
  end interface
  public :: array

  !> Retrieve a pointer to the array @bud
  interface array_p
    module procedure get_elem2p_
  end interface
  public :: array_p

  !> Query the size of the array (dimension)
  interface dimensions
    module procedure dimensions_
  end interface
  public :: dimensions

  !> Query the distributed dimension of the array
  interface distributed_index
    module procedure distributed_index_
  end interface
  public :: distributed_index

  !> Distribute data from one distribution to another
  !! distribution.
  !!
  !! This *must* only be called with the new distribution
  !! having a unique distribution.
  !! And all ranks *must* call with either:
  !! 1) the same input distributed matrix
  !! 2) a non-initialized distributed array for those
  !!    ranks that is not holding the input distributed
  !!    array.
  !!
  !! Note that this function is itself distributed and
  !! will distribute to all the different ranks having
  !! a new distribution.
  interface distribute
    module procedure distribute_
  end interface
  public :: distribute

#include "bud_coll.inc"


  !> @param[in] this sparse matrix
  !! @return the index of the sparse index
  function distributed_index_(this) result(idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: idx

    if ( is_initd(this) ) then
#if BUD_DIM > 1
      idx = this%D%dist_idx
#else
      idx = 1
#endif
    else
      idx = -1
    end if

  end function distributed_index_

  !> @param[in] this @bud container
  !! @return the number of dimensions of contained array
  pure function dimensions_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: d

    if ( is_initd(this) ) then
      d = BUD_DIM
    else
      d = -1
    end if

  end function dimensions_

  ! Now we should construct the specific routines for redistribution
  ! of the matrices...
  !> @param[in] this an already distributed sparse matrix
  !! @param[in] parent the parent communicator which the information goes through, all ranks in parent *must* participate
  !! @param[in] out_dist the output distribution, this may be called with several dist subsets from different ranks. However, each rank must only occur _once_ in each distribution.
  !! @param[inout] out the output distributed sparse matrix on the ranks belonging to `dist`
  subroutine distribute_(this, parent, out_dist, out)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_TYPE_MP_COMM), intent(inout) :: parent
    BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: out

    ! The contained data in "dist"
    type(BUD_TYPE_MP_COMM) :: comm, out_comm
    ! This is used as a "fake" distribution
    type(BUD_COLL_1) :: fake_dist, dist

    ! The data in the sparse matrix in `this`
    type(BUD_COLL_2) :: arr

    integer(ii_) :: ir, nranks, my_root, rank, in_rank
    logical :: is_distr, my_distr

    ! The sparse matrix information
    integer(ii_) :: dist_idx, dims

    ! Distribution information
    integer(BUD_PREC) :: nl, ng

    integer(ii_) :: nout
    integer(ii_), allocatable :: ranks(:)
    integer(ii_), allocatable :: ashape(:), itmp1(:)

    if ( .not. is_communicator(parent) ) return

    ! Get the total ranks in the main distribution.
    rank = comm_rank(parent)
    nranks = comm_size(parent)

    ! "dist" communicator
    ! This will only be initialized if `dist` has
    ! an initialized communicator.
    dist = this
    ! Get the rank in the distribution
    in_rank = comm_rank(dist)
    ! Step the input rank so that it will
    ! never be found when we do the distribution.
    if ( in_rank < 0 ) in_rank = in_rank - 1
    comm = dist
    arr = this

    ! We need to bcast the basic information
    ! regarding the matrix size
    dims = dimensions(arr)
#ifdef BUD_MPI
    call AllReduce_Max(dims, ir, parent)
    dims = ir
#endif
    allocate(ashape(dims))
    if ( is_initd(arr) ) then
      dist_idx = distributed_index(this)
      do ir = 1 , dims
        ashape(ir) = size(arr, ir)
      end do
    else
      dist_idx = 0
      do ir = 1 , dims
        ashape(ir) = 0
      end do
    end if

#ifdef BUD_MPI
    ! First distribute the distributed index
    call AllReduce_Max(dist_idx, ir, parent)
    dist_idx = ir

    allocate(itmp1(dims))
    call AllReduce_Max(ashape, itmp1, parent)
    do ir = 1 , dims
      ! Do not update the distributed index
      if ( ir /= dist_idx ) then
        ashape(ir) = itmp1(ir)
      end if
    end do
    deallocate(itmp1)
#endif

    ! Get the output communicator
    out_comm = out_dist

    ! First we let all ranks know their root-rank in the
    ! top distribution
    if ( is_communicator(out_comm) ) then

      if ( comm_rank(out_comm) == 0 ) then
        ir = comm_rank(parent)
      else
        ir = -1
      end if
#ifdef BUD_MPI
      ! We only need to reduce on the output communicator.
      ! I.e. we want them to know the root rank in the
      ! parent communicator.
      call AllReduce_Max(ir, my_root, out_comm)
#else
      my_root = ir
#endif

      ! at this point my_root is the rank of the
      ! root node in the `this` distribution

    else

      ! this node is not going to recieve any
      ! data. Thus it has no "root" node.
      my_root = -1

    end if

    ! Figure out the sizes of the input distribution
    ng = size_global(dist)
#ifdef BUD_MPI
    call AllReduce_Max(ng, nl, parent)
    ng = nl
#endif
    nl = size_local(dist)

    ! Loop on all ranks and do the actual distribution
    ! on each individual distribution...
    ! This will only do something
    ! when ir == root rank on any of the output distributions
    do ir = 0 , nranks - 1

      if ( my_root == ir ) then
        ! This is "my" distribution
        my_distr = .true.
      else
        my_distr = .false.
      end if

#ifdef BUD_MPI
      ! Reduce so that everybody knows whether this is a new
      ! distribution
      call AllReduce_LOr(my_distr, is_distr, parent)
#else
      is_distr = my_distr
#endif

      ! Immediately cycle if this is not a new distribution
      if ( .not. is_distr ) cycle

      ! Now we will perform the distribution stuff...
      ! First we need to create a "remote" group that
      ! has the same size as the remote distribution
      if ( my_distr ) then

        ! This is the group that has dist
        call new_remote(parent, out_dist)
        call create_ranks(out_dist)
        call sub_dist(out_dist)

      else

        ! Create a fake distribution to succesfully
        ! distribute the data to the correct sources.
        call new_remote(parent, fake_dist)
        call create_ranks(fake_dist)
        call sub_dist(fake_dist)
        call delete(fake_dist)

      end if

      ! Be sure to deallocate the ranks for the next
      ! iteration.
      deallocate(ranks)

    end do

    call delete(dist)
    call delete(comm)
    call delete(arr)
    call delete(out_comm)

  contains

    subroutine create_ranks(dist)
      BUD_CLASS(BUD_COLL_1), intent(inout) :: dist
#ifdef BUD_MPI
      type(BUD_TYPE_MP_COMM) :: comm

      comm = dist

      ! Allocate the ranks that are part of dist
      call child_Bcast(parent, comm, size=nout)
      allocate(ranks(-1:nout-1))
      ranks(-1) = -1
      call child_Bcast_ranks(parent, comm, nout, ranks(0:))

      call delete(comm)
#else
      allocate(ranks(-1:0))
      ranks(-1) = -1
      ranks(0) = 0
#endif

    end subroutine create_ranks

    subroutine sub_dist(out_dist)
      BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist

      ! Define the objects that are the new containers
      type(BUD_TYPE_MP_COMM) :: out_comm
      type(BUD_COLL_2) :: out_arr

      logical :: run

      ! The pointers to the input data
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: dat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer :: dat BUD_DIM_SPEC(:)
#endif
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: odat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer :: odat BUD_DIM_SPEC(:)
#endif

      ! The local counter integers (local and global)
      integer(BUD_PREC) :: il, ig, oil
      integer(BUD_PREC) :: out_nl

#if BUD_DIM > 2
      integer(BUD_PREC) :: i3
#endif
      integer(BUD_PREC) :: i2
      ! Ranks on the recieving and sending end
      integer :: send_R, recv_R

#ifdef BUD_MPI
      ! MPI requests and statuses
      integer(ii_), allocatable :: reqs(:), stats(:,:)
      integer(ii_) :: stat(MPI_STATUS_SIZE)
#endif

      ! Whether we should run or quit immediately
      run = .true.

      if ( .not. run ) return

      out_comm = out_dist

      ! Figure out the sizes of the output distribution
      out_nl = size_local(out_dist)

!      print *,'Start:', my_distr, rank, nl, ng, out_nl

      ! At this point this rank knows,
      ! the distribution type, and everything
      ! about the size of the sparse matrix it
      ! holds.
      ! First we recreate the arrays
      ! and sparse matrices...
      if ( is_communicator(out_comm) ) then

        ! Create the array
        ! The ashape only contains
        ! information regarding the number of
        ! elements in this local one.
        ashape(dist_idx) = out_nl

        call new(out_arr, ashape(:))
        call new(out, out_dist, out_arr)

        odat => array_p(out_arr)

        ! Immediately clean-up after
        ! we have retrieved the wanted information
        call delete(out_arr)

      end if

#ifdef BUD_MPI
      if ( is_initd(dist) ) then

        ! Only ranks that have dist allocated
        ! will be sending stuff
        allocate(reqs(nl), stats(MPI_STATUS_SIZE, nl))
        ! Initialize the requests to NULL
        do il = 1 , nl
          reqs(il) = MPI_REQUEST_NULL
        end do

      end if
#endif

      ! Now we need to redistribute the array data
      ! Retrieve the data-pointer
      dat => array_p(arr)

      if ( dist_idx == dims ) then
        ! The easy stuff is when the distributed
        ! dimension is the same as

        ! Loop on global indices
        do ig = 1 , ng

          send_R = g2rank(dist, ig)
          recv_R = ranks(g2rank(out_dist, ig))

          if ( recv_R == rank .and. &
            send_R == in_rank ) then
            ! both the recieving and the sending rank

            il = g2l(dist, ig)
            oil = g2l(out_dist, ig)

#if BUD_DIM == 1
            odat(oil) = dat(il)
#elif BUD_DIM == 2
            odat(:,oil) = dat(:,il)
#elif BUD_DIM == 3
            odat(:,:,oil) = dat(:,:,il)
#else
# error "Only up till BUD_DIM == 3 has been implemented"
#endif

#ifdef BUD_MPI
          else if ( recv_R == rank ) then

            oil = g2l(out_dist, ig)

# if BUD_DIM == 1
            call Recv(odat(oil), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 2
            call Recv(odat(:,oil), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 3
            call Recv(odat(:,:,oil), MPI_ANY_SOURCE, ig, &
              parent, stat)
# else
#  error "Only up till BUD_DIM == 3 has been implemented"
# endif
            ! TODO check that Get_Count == oid2-oid1+1
            ! But here we do not know the data-type

          else if ( send_R == in_rank ) then

            il = g2l(dist, ig)

#if BUD_DIM == 1
            call ISSend(dat(il), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 2
            call ISSend(dat(:,il), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 3
            call ISSend(dat(:,:,il), recv_R, ig, &
              parent, reqs(il))
#else
# error "Only up till BUD_DIM == 3 has been implemented"
#endif
#else
          else

            ! TODO something went wrong
#endif
          end if

        end do

#ifdef BUD_MPI
        if ( allocated(reqs) ) &
          call WaitAll(nl, reqs, stats, parent)
#endif

      else if ( dist_idx == 1 ) then

        ! the first index is the sparse element
#if BUD_DIM > 2
       do i3 = 1 , ashape(3)
#endif
        do i2 = 1 , ashape(2)

         ! Loop on global indices
         do ig = 1 , ng

           send_R = g2rank(dist, ig)
           recv_R = ranks(g2rank(out_dist, ig))

           if ( recv_R == rank .and. &
             send_R == in_rank ) then

             il = g2l(dist, ig)
             oil = g2l(out_dist, ig)

#if BUD_DIM == 2
             odat(oil,i2) = dat(il,i2)
#elif BUD_DIM == 3
             odat(oil,i2,i3) = dat(il,i2,i3)
#endif

#ifdef BUD_MPI
           else if ( recv_R == rank ) then

             oil = g2l(out_dist, ig)

#if BUD_DIM == 2
             call Recv(odat(oil,i2), MPI_ANY_SOURCE, ig, &
               parent, stat)
#elif BUD_DIM == 3
             call Recv(odat(oil,i2,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
#endif

             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)

#if BUD_DIM == 2
             call ISSend(dat(il,i2), recv_R, ig, &
               parent, reqs(il))
#elif BUD_DIM == 3
             call ISSend(dat(il,i2,i3), recv_R, ig, &
               parent, reqs(il))
#endif

#else
           else
             ! TODO something went wrong
#endif
           end if

         end do ! ig

#ifdef BUD_MPI
         if ( allocated(reqs) ) &
           call WaitAll(nl, reqs, stats, parent)
#endif

        end do ! i2
#if BUD_DIM > 2
       end do ! i3
#endif

#if BUD_DIM > 1
      else if ( dist_idx == 2 ) then

# if BUD_DIM == 3
       do i3 = 1 , ashape(3)

         ! Loop on global indices
         do ig = 1 , ng

           send_R = g2rank(dist, ig)
           recv_R = ranks(g2rank(out_dist, ig))

           if ( recv_R == rank .and. &
             send_R == in_rank ) then

             il = g2l(dist, ig)
             oil = g2l(out_dist, ig)

             odat(:,oil,i3) = dat(:,il,i3)

#ifdef BUD_MPI
           else if ( recv_R == rank ) then

             oil = g2l(out_dist, ig)

             call Recv(odat(:,oil,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)

             call ISSend(dat(:,il,i3), recv_R, ig, &
               parent, reqs(il))
#else
           else
             ! TODO something went wrong!
#endif
           end if

         end do ! ig

#ifdef BUD_MPI
         if ( allocated(reqs) ) &
           call WaitAll(nl, reqs, stats, parent)
#endif

       end do ! i3
# endif
#endif
      end if

#ifdef BUD_MPI
      if ( allocated(reqs) ) deallocate(reqs)

      ! Be sure to wait for the next iteration
      call Barrier(parent)
#endif

      call delete(out_comm)

    end subroutine sub_dist

  end subroutine distribute_


#undef BUD_TYPE_NEW

  ! Associated with the Array
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

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


