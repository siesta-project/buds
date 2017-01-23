  ! @@LICENSE@@ see Copyright notice in the top-directory

  !> @defgroup dist1d-array Distributed (1D), dense matrix with data
  !! @ingroup bud-intrinsic
  !!
  !! @bud containing a distribution _and_ the
  !! associated data corresponding to the dense matrix elements.
  !!

#include "bud_mpi.inc"

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

  interface new
    module procedure new_dist_index_
  end interface new
  ! New should already be public

#include "bud_collection.inc"


  !> @param[inout] this the collection of a distribution and Array
  !! @param[in] dist the distribution
  !! @param[in] arr the array
  !! @param[in] dist_idx the distributed index in the array
  subroutine new_dist_index_(this, dist, arr, dist_idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_COLL_1), intent(inout) :: dist
    BUD_CLASS(BUD_COLL_2), intent(inout) :: arr
    integer, intent(in) :: dist_idx

    call new(this, dist, arr)

#if BUD_DIM > 1
    ! Set the distributed index
    this%D%dist_idx = dist_idx
#endif

  end subroutine new_dist_index_


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
      use BUD_CC2(BUD_MOD,_Transfer), only: transfer_dim

      BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist

      ! Define the objects that are the new containers
      type(BUD_TYPE_MP_COMM) :: out_comm
      type(BUD_COLL_2) :: out_arr

      logical :: run

      ! The pointers to the input data
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
#endif
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: odat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: odat BUD_DIM_SPEC(:)
#endif

      ! The local counter integers (local and global)
      integer(BUD_PREC) :: il, ig, oil
      integer(BUD_PREC) :: out_nl

#if BUD_DIM > 1
      ! Number of elements copied
      integer(BUD_PREC) :: ncopy
#endif

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

#if BUD_DIM == 2
        ncopy = size(odat, 1)
#elif BUD_DIM == 3
        ncopy = size(odat, 1) + size(odat, 2)
#endif

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
            call transfer_dim(ncopy, odat(:,oil), &
              ncopy, dat(:,il))
#elif BUD_DIM == 3
            call transfer_dim(ncopy, 1, odat(:,:,oil), &
              ncopy, 1, dat(:,:,il))
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

       ncopy = size(odat, 1)

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

             call transfer_dim(ncopy, odat(:,oil,i3), &
               ncopy, dat(:,il,i3))

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


  !> @param[inout] f `File` bud
  !! @param[in] this the distributed array @bud
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    use BUD_CC2(BUD_MOD,_Transfer), only: transfer_dim

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    type(BUD_COLL_1) :: dist
    type(BUD_COLL_2) :: arr
    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm

    logical :: formatted, do_io
    integer :: iu, io_rank, dat_rank, rank

#ifdef BUD_IS_INTEGER
    character(len=64), parameter :: fmt_ = '(i16)'
#else
    character(len=64), parameter :: fmt_ = '(e20.16)'
#endif

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), allocatable :: rdat(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR, allocatable :: rdat(:)
#endif

    integer :: dist_idx, l, n, ndat
#if BUD_DIM == 1
    integer :: d1, i1
#elif BUD_DIM == 2
    integer :: d1, d2, i1, i2
#elif BUD_DIM == 3
    integer :: d1, d2, d3, i1, i2, i3
#endif
#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

    ! If file is not opened, return immediately
    if ( .not. is_initd(this) ) return

#ifndef BUD_MPI
    arr = this
    call write(f, arr)
    call delete(arr)
    return
#else

    ! Get contained data.
    dist = this
    comm = dist
    arr = this

    rank = comm_rank(comm)
    if ( is_open(f) ) then
      io_rank = rank
    else
      io_rank = -1
    end if
    call AllReduce_Max(io_rank, iu, comm)
    io_rank = iu

    ! Check if the file is open on one rank
    if ( io_rank < 0 ) then
      call delete(dist)
      call delete(comm)
      call delete(arr)
      return
    end if

    ! This ensures that only one rank will do the writing
    ! in case multiple ranks have the file open
    ! only @bud knows what will happen in this case!!!
    do_io = io_rank == rank

    if ( do_io ) then
      ! First figure out if the file is an unformatted file
      formatted = is_formatted(f)
      iu = unit(f)
    end if

    ! Retrieve information from the array
    ! Get size
    d1 = size(arr, 1)
# if BUD_DIM > 1
    d2 = size(arr, 2)
# endif
# if BUD_DIM > 2
    d3 = size(arr, 3)
# endif
    dat => array_p(arr)
    call delete(arr)

    ! Now figure out the maximum number of elements
    ! that is retrieved
    dist_idx = distributed_index(this)

    if ( dist_idx == 1 ) then
      ! We will collect all information from
      ! all nodes
      ndat = size_global(dist)
    else
      ndat = d1
    end if

    ! Correct the distributed data
# if BUD_DIM > 1
    select case ( dist_idx )
    case ( 2 )
      d2 = ndat
# if BUD_DIM > 2
    case ( 3 )
      d3 = ndat
# endif
    end select
# endif

    ! if we are the io-node we should allocate
    if ( do_io ) then
      allocate(rdat(ndat))

      if ( formatted ) then
        write(iu, '(i16)') BUD_DIM
# if BUD_DIM == 1
        write(iu, '(i16)') d1
# elif BUD_DIM == 2
        write(iu, '(i16,tr1,i16)') d1, d2
# elif BUD_DIM == 3
        write(iu, '(2(i16,tr1),i16)') d1, d2, d3
# elif BUD_DIM == 4
        write(iu, '(3(i16,tr1),i16)') d1, d2, d3, d4
# elif BUD_DIM == 5
        write(iu, '(4(i16,tr1),i16)') d1, d2, d3, d4, d5
# endif
      else
        write(iu) BUD_DIM
# if BUD_DIM == 1
        write(iu) d1
# elif BUD_DIM == 2
        write(iu) d1, d2
# elif BUD_DIM == 3
        write(iu) d1, d2, d3
# elif BUD_DIM == 4
        write(iu) d1, d2, d3, d4
# elif BUD_DIM == 5
        write(iu) d1, d2, d3, d4, d5
# endif
      end if

    end if

    if ( dist_idx == 1 ) then

      ! the distribution index is the inner-most
      ! data...
      ! this makes a lot of communication due
      ! to the outer loops.
      ! If this is a problem one
      ! can redistribute the data and then perform writing

# if BUD_DIM > 2
      do i3 = 1 , d3
# endif
# if BUD_DIM > 1
      do i2 = 1 , d2
# endif
        ! Collect
        i1 = 1
        do while ( i1 <= d1 )

          ! Figure out how much to grab
          n = consecutive(dist, i1)

          dat_rank = global2rank(dist, i1)

          if ( do_io .and. rank == dat_rank ) then

            l = global2local(dist, i1)

            ! Copy from local data to the gathered data
# if BUD_DIM > 2
            call transfer_dim(n, rdat(i1:), n, dat(i1:,i2,i3))
# elif BUD_DIM > 1
            call transfer_dim(n, rdat(i1:), n, dat(l:,i2))
# else
            call transfer_dim(n, rdat(i1:), n, dat(l:))
# endif

          else if ( do_io ) then

            call Recv(rdat(i1), n, dat_rank, i1, comm, status)

          else if ( rank == dat_rank ) then

            l = global2local(dist, i1)

# if BUD_DIM > 2
            call SSend(dat(l,i2,i3), n, io_rank, i1, comm)
# elif BUD_DIM > 1
            call SSend(dat(l,i2), n, io_rank, i1, comm)
# else
            call SSend(dat(l), n, io_rank, i1, comm)
# endif

          end if

          i1 = i1 + n

        end do

        if ( do_io ) then
          if ( formatted ) then
            write(iu, fmt_) rdat(:)
          else
            write(iu) rdat(:)
          end if
        end if

# if BUD_DIM > 1
      end do
# endif
# if BUD_DIM > 2
      end do
# endif

# if BUD_DIM > 1
    else if ( dist_idx == 2 ) then

      ! only one dimension to distribute
# if BUD_DIM > 2
      do i3 = 1 , d3
# endif
      do i2 = 1 , d2

        dat_rank = global2local(dist,i2)

        if ( do_io .and. rank == dat_rank ) then

          l = global2local(dist, i2)

# if BUD_DIM > 2
          call transfer_dim(d1, rdat(1:), d1, dat(1:,l,i3))
# else
          call transfer_dim(d1, rdat(1:), d1, dat(l:,l))
# endif

        else if ( do_io ) then

          call Recv(rdat(1), d1, dat_rank, i1, comm, status)

        else if ( rank == dat_rank ) then

          l = global2local(dist, i2)

# if BUD_DIM > 2
          call SSend(dat(1,l,i3), d1, io_rank, i1, comm)
# else
          call SSend(dat(l,l), d1, io_rank, i1, comm)
# endif

        end if

        if ( do_io ) then
          if ( formatted ) then
            write(iu, fmt_) rdat(:)
          else
            write(iu) rdat(:)
          end if
        end if

      end do
# if BUD_DIM > 2
      end do
# endif

# endif

# if BUD_DIM > 2
    else if ( dist_idx == 3 ) then

      ! only one dimension to distribute
      do i3 = 1 , d3
      do i2 = 1 , d2

        dat_rank = global2local(dist,i3)

        if ( do_io .and. rank == dat_rank ) then

          l = global2local(dist, i3)

          call transfer_dim(d1, rdat(1:), d1, dat(1:,i2,l))

        else if ( do_io ) then

          call Recv(rdat(1), d1, dat_rank, i1, comm, status)

        else if ( rank == dat_rank ) then

          l = global2local(dist, i3)

          call SSend(dat(1,i2,l), d1, io_rank, i1, comm)

        end if

        if ( do_io ) then
          if ( formatted ) then
            write(iu, fmt_) rdat(:)
          else
            write(iu) rdat(:)
          end if
        end if

      end do
      end do

# endif

    end if

    ! Ensure all has been de-allocated
    if ( do_io ) deallocate(rdat)

    call delete(dist)
    call delete(comm)
#endif

  end subroutine write_


  !> @param[inout] f `File` bud
  !! @param[inout] dist the the distribution of the data
  !! @param[inout] this the distributed array @bud
  !! @param[in] dist_idx @opt=BUD_DIM the distributed index
  subroutine read_(f, dist, this, dist_idx)
    use BUD_CC2(BUD_MOD,_File)
    use BUD_CC2(BUD_MOD,_Transfer), only: transfer_dim

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_COLL_1), intent(inout) :: dist
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: dist_idx

    type(BUD_COLL_2) :: arr
    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm

    logical :: formatted, do_io
    integer :: iu, io_rank, dat_rank, rank

#ifdef BUD_IS_INTEGER
    character(len=64), parameter :: fmt_ = '(i16)'
#else
    character(len=64), parameter :: fmt_ = '(e20.16)'
#endif

    integer(ii_) :: ashape(BUD_DIM)

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), allocatable :: rdat(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR, allocatable :: rdat(:)
#endif

    integer :: ldist_idx, l, n
#if BUD_DIM == 1
    integer :: d1, i1
#elif BUD_DIM == 2
    integer :: d1, d2, i1, i2
#elif BUD_DIM == 3
    integer :: d1, d2, d3, i1, i2, i3
#endif
#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

    ! If file is not opened, return immediately
    if ( .not. is_initd(this) ) return

    ldist_idx = BUD_DIM
    if ( present(dist_idx) ) ldist_idx = dist_idx

#ifndef BUD_MPI
    call read(f, arr)
    call new(this, dist, arr, ldist_idx)
    return
#else

    ! Get contained data.
    comm = dist

    rank = comm_rank(comm)
    if ( is_open(f) ) then
      io_rank = rank
    else
      io_rank = -1
    end if
    call AllReduce_Max(io_rank, iu, comm)
    io_rank = iu

    ! Check if the file is open on one rank
    if ( io_rank < 0 ) then
      call delete(comm)
      call delete(this)
      call set_error(this, -1)
      return
    end if

    ! This ensures that only one rank will do the writing
    ! in case multiple ranks have the file open
    ! only @bud knows what will happen in this case!!!
    do_io = io_rank == rank

    if ( do_io ) then
      ! First figure out if the file is an unformatted file
      formatted = is_formatted(f)
      iu = unit(f)

      ! Read sizes
      if ( formatted ) then
        read(iu, '(i16)') l
# if BUD_DIM == 1
        read(iu, '(i16)') ashape
        d1 = ashape(1)
# elif BUD_DIM == 2
        read(iu, '(i16,tr1,i16)') ashape
        d1 = ashape(1)
        d2 = ashape(2)
# elif BUD_DIM == 3
        read(iu, '(2(i16,tr1),i16)') ashape
        d1 = ashape(1)
        d2 = ashape(2)
        d3 = ashape(3)
# elif BUD_DIM == 4
        read(iu, '(3(i16,tr1),i16)') ashape
        d1 = ashape(1)
        d2 = ashape(2)
        d3 = ashape(3)
        d4 = ashape(4)
# elif BUD_DIM == 5
        read(iu, '(4(i16,tr1),i16)') ashape
        d1 = ashape(1)
        d2 = ashape(2)
        d3 = ashape(3)
        d4 = ashape(4)
        d5 = ashape(5)
# endif
      else
        read(iu) l
        read(iu) ashape
        d1 = ashape(1)
# if BUD_DIM > 1
        d2 = ashape(2)
# endif
# if BUD_DIM > 2
        d3 = ashape(3)
# endif
# if BUD_DIM > 3
        d4 = ashape(4)
# endif
# if BUD_DIM > 4
        d5 = ashape(5)
# endif
      end if

    end if

    ! Now figure out the maximum number of elements
    ! that is retrieved
    select case ( ldist_idx )
    case ( 1 )
      ashape(1) = size_local(dist)
# if BUD_DIM > 1
    case ( 2 )
      ashape(2) = size_local(dist)
# endif
# if BUD_DIM > 2
    case ( 3 )
      ashape(3) = size_local(dist)
# endif
# if BUD_DIM > 3
    case ( 4 )
      ashape(4) = size_local(dist)
# endif
# if BUD_DIM > 5
    case ( 5 )
      ashape(5) = size_local(dist)
# endif
    end select

    ! Initialize the array to be read
    ! Note this is already allocated distributed
    call new(arr, ashape)
    dat => array_p(arr)
    call new(this, dist, arr, ldist_idx)
    call delete(arr)

    ! if we are the io-node we should allocate
    if ( do_io ) then
      allocate(rdat(d1))
    end if

    ! Loop, read and distribute
# if BUD_DIM > 2
    do i3 = 1 , d3
# endif
# if BUD_DIM > 1
    do i2 = 1 , d2
# endif

      if ( do_io ) then
        if ( formatted ) then
          read(iu, fmt_) rdat(:)
        else
          read(iu) rdat(:)
        end if
      end if

      ! distribute data
      select case ( ldist_idx )
      case ( 1 )
        ! worst-case...
        ! We should loop and redistribute data

        i1 = 1
        do while ( i1 <= d1 )

          ! Figure out how much to grab
          n = consecutive(dist, i1)
          dat_rank = global2rank(dist, i1)

          if ( do_io .and. rank == dat_rank ) then

            l = global2local(dist, i1)

# if BUD_DIM > 2
            call transfer_dim(n, dat(l:,i2,i3), n, rdat(i1:))
# elif BUD_DIM > 1
            call transfer_dim(n, dat(l:,i2), n, rdat(i1:))
# else
            call transfer_dim(n, dat(l:), n, rdat(i1:))
# endif

          else if ( do_io ) then

            call SSend(rdat(i1), n, dat_rank, i1, comm)

          else if ( rank == dat_rank ) then

            l = global2local(dist, i1)

# if BUD_DIM > 2
            call Recv(dat(l,i2,i3), n, io_rank, i1, comm, status)
# elif BUD_DIM > 1
            call Recv(dat(l,i2), n, io_rank, i1, comm, status)
# else
            call Recv(dat(l), n, io_rank, i1, comm, status)
# endif

          end if

          i1 = i1 + n

        end do

# if BUD_DIM > 1
      case ( 2 )

        dat_rank = global2rank(dist, i2)

        if ( do_io .and. rank == dat_rank ) then

          l = global2local(dist, i2)

# if BUD_DIM > 2
          call transfer_dim(d1, dat(1:,l,i3), d1, rdat(:))
# else
          call transfer_dim(d1, dat(1:,l), d1, rdat(:))
# endif

        else if ( do_io ) then

          call SSend(rdat(1), d1, dat_rank, i2, comm)

        else if ( rank == dat_rank ) then

          l = global2local(dist, i2)

# if BUD_DIM > 2
          call Recv(dat(1,l,i3), d1, io_rank, i2, comm, status)
# else
          call Recv(dat(1,l), d1, io_rank, i2, comm, status)
# endif

        end if
# endif

# if BUD_DIM > 2
      case ( 3 )

        dat_rank = global2rank(dist, i3)

        if ( do_io .and. rank == dat_rank ) then

          l = global2local(dist, i3)

          call transfer_dim(d1, dat(1:,i2,l), d1, rdat(:))

        else if ( do_io ) then

          call SSend(rdat(1), d1, dat_rank, i3, comm)

        else if ( rank == dat_rank ) then

          l = global2local(dist, i3)

          call Recv(dat(1,i2,l), d1, io_rank, i3, comm, status)

        end if


# endif
      end select

# if BUD_DIM > 1
    end do
# endif
# if BUD_DIM > 2
    end do
# endif

    call delete(comm)
#endif

  end subroutine read_


  ! Associated with the Array
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

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


