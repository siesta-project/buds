  ! @@LICENSE@@ see Copyright notice in the top-directory

  !> @defgroup dist1d-sm-array Distributed (1D), sparse matrix with data
  !! @ingroup bud-intrinsic
  !!
  !! @bud containing a distribution, a sparse matrix pattern _and_ the
  !! associated data corresponding to the sparse matrix elements.
  !!
  !! From this object the full matrix may be constructed or interacted
  !! with.
  !!

#include "bud_mpi.inc"

  use BUD_CC3(BUD_MOD,_,MP_Comm)

#include "bud_common_declarations.inc"


!# define BUD_INCLUDE_TYPE "Dist1D_SM_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "Dist1D_SM_Array_type_.inc"

  ! Currently we only allow two different sparse matrices
#if defined(BUD_SM_CSC)
#elif defined(BUD_SM_CSR)
#else
# error "Could not figure out which sparse matrix this is!"
#endif


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

  !> Retrieve the sparse matrix
  interface sparse_matrix
    module procedure get_elem2_
  end interface
  public :: sparse_matrix

  !> Retrieve pointer to the sparse matrix
  interface sparse_matrix_p
    module procedure get_elem2p_
  end interface
  public :: sparse_matrix_p

  !> Retrieve the array @bud
  interface array
    module procedure get_elem3_
  end interface
  public :: array

  !> Retrieve a pointer to the array @bud
  interface array_p
    module procedure get_elem3p_
  end interface
  public :: array_p

  !> Retrieve pointer data in the sparse matrix
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! Do not deallocate the array-pointer.
  interface matrix_p
    module procedure get_matrixp_
  end interface
  public :: matrix_p

  !> Query the index of the sparse matrix
  interface sparse_index
    module procedure sparse_index_
  end interface
  public :: sparse_index

  !> Query the size of the matrix (dimension)
  interface dimensions
    module procedure dimensions_
  end interface
  public :: dimensions

  !> Distribute data from one distribution to another
  !! distribution.
  !!
  !! This *must* only be called with the new distribution
  !! having a unique distribution.
  !! And all ranks *must* call with either:
  !! 1) the same input distributed matrix
  !! 2) a non-initialized distributed matrix for those
  !!    ranks that is not holding the input distributed
  !!    matrix.
  !!
  !! Note that this function is itself distributed and
  !! will distribute to all the different ranks having
  !! a new distribution.
  interface distribute
    module procedure distribute_
  end interface
  public :: distribute

#include "bud_collection.inc"


  !> @param[in] this sparse matrix
  !! @return a pointer to the data (contiguous)
  function get_matrixp_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    p => array_p(this%D%e3)

  end function get_matrixp_

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

  !> @param[in] this sparse matrix
  !! @return the index of the sparse index
  function sparse_index_(this) result(idx)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: idx

    if ( is_initd(this) ) then
#if BUD_DIM > 1
      idx = this%D%sm_idx
#else
      idx = 1
#endif
    else
      idx = -1
    end if

  end function sparse_index_

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

    ! The sparse matrix in `this`
    type(BUD_COLL_2) :: sm
    ! The data in the sparse matrix in `this`
    type(BUD_COLL_3) :: arr

    integer(ii_) :: ir, nranks, my_root, rank, in_rank
    logical :: is_distr, my_distr

    ! The sparse matrix information
    integer(ii_) :: sm_idx, dims
    integer(BUD_PREC) :: msg_size, ltmp

    ! Distribution information
    integer(BUD_PREC) :: nr, nc, nl, ng, out_nz

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
    sm = this
    arr = this

    ! We need to bcast the basic information
    ! regarding the matrix size
    call attach(sm, nr=nr, nc=nc)
#ifdef BUD_MPI
    ! First the local nodes should sum the row/columns
    ! because they may be distributed.
    if ( is_communicator(comm) ) then
# if defined(BUD_SM_CSC)
      call AllReduce_Sum(nc, ir, comm)
      ir = nc
# elif defined(BUD_SM_CSR)
      call AllReduce_Sum(nr, ir, comm)
      ir = nr
# endif
    end if
    call AllReduce_Max(nr, ir, parent)
    nr = ir
    call AllReduce_Max(nc, ir, parent)
    nc = ir
#endif


    ! figure out the sparse-index, and the
    ! dimensions up till the sparse-index
    ! First, find the sparse index
    sm_idx = sparse_index(this)
    ! figure out number of dimensions
    dims = dimensions(this)
    msg_size = 1
    do ir = 1 , sm_idx - 1
      msg_size = msg_size * size(arr, ir)
    end do

#ifdef BUD_MPI
    ! Bcast this for the remaining participants.
    ! This is necessary as not all may have `this`
    call AllReduce_Max(sm_idx, ir, parent)
    sm_idx = ir
    call AllReduce_Max(dims, ir, parent)
    dims = ir
#endif
    allocate(ashape(dims))
    ashape = -1
    do ir = 1 , dims
      ashape(ir) = size(arr, ir)
    end do
#ifdef BUD_MPI
    allocate(itmp1(dims))
    call AllReduce_Max(ashape, itmp1, parent)
    ashape = itmp1
    deallocate(itmp1)
    call AllReduce_Max(msg_size, ltmp, parent)
    msg_size = ltmp
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
        ! Get the resulting number of non-zero
        ! elements on this rank
        out_nz = reduce_size(out_dist)
        ! correct the shape of the output matrix
        ashape(sm_idx) = out_nz
        call sub_dist(out_dist)

      else

        ! Create a fake distribution to succesfully
        ! distribute the data to the correct sources.
        call new_remote(parent, fake_dist)
        call create_ranks(fake_dist)
        out_nz = reduce_size(fake_dist)
        call sub_dist(fake_dist)
        call delete(fake_dist)

      end if

      ! Be sure to deallocate the ranks for the next
      ! iteration.
      deallocate(ranks)

    end do

    call delete(dist)
    call delete(comm)
    call delete(sm)
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

    function reduce_size(out_dist) result(nz)
      BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist
      integer(BUD_PREC) :: nz

      integer(BUD_PREC), pointer :: ptr(:), ind(:)

      integer :: ir, recv_R
      integer(BUD_PREC) :: n, id1, id2
      integer(BUD_PREC) :: il, ig

      nz = 0

#ifdef BUD_SM_CSC
      call attach(sm, cptr=ptr, row=ind)
#elif defined(BUD_SM_CSR)
      call attach(sm, rptr=ptr, col=ind)
#endif

      ! First we need to know the size of the sparse
      ! matrix. So we do a loop on each rank that
      ! should recieve data.
      do ir = 0 , nout - 1

        ! number of sparse-elements on this
        ! node, that is send to ir
        n = 0
        do il = 1 , nl

          ! Get global index
          ig = l2g(dist, il)

          ! Get recieving node
          recv_R = g2rank(out_dist, ig)
          if ( recv_R /= ir ) cycle

          ! Now we simply need to post the sends
          id1 = ptr(il)
          id2 = ptr(il+1)

          ! Count number of elements send
          n = n + id2 - id1

        end do

#ifdef BUD_MPI
        recv_R = ranks(ir)
        call Reduce_Sum(n, nz, recv_R, parent)
#else
        ! With no MPI it must be its own distribution
        nz = n
#endif

      end do

    end function reduce_size

    subroutine sub_dist(out_dist)
      BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist

      ! Define the objects that are the new containers
      type(BUD_TYPE_MP_COMM) :: out_comm
      type(BUD_COLL_2) :: out_sm
      type(BUD_COLL_3) :: out_arr

      logical :: run

      ! The pointers to the input data
      integer(BUD_PREC), pointer :: ptr(:), ind(:)
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: dat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer :: dat BUD_DIM_SPEC(:)
#endif
      ! The pointers to the output data
      integer(BUD_PREC), pointer :: optr(:), oind(:), nptr(:)

#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: odat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer :: odat BUD_DIM_SPEC(:)
#endif

      ! The local counter integers (local and global)
      integer(BUD_PREC) :: il, ig, oil
      ! index pointers
      integer(BUD_PREC) :: id1, id2, oid1, oid2
      integer(BUD_PREC) :: out_nl

#if BUD_DIM > 2
      integer(BUD_PREC) :: i3
#endif
      integer(BUD_PREC) :: i, i2
      ! Ranks on the recieving and sending end
      integer :: send_R, recv_R

      ! sparse matrix information
      integer(BUD_PREC) :: nz

#ifdef BUD_MPI
      ! MPI requests and statuses
      integer(ii_), allocatable :: reqs(:), stats(:,:)
      integer(ii_) :: stat(MPI_STATUS_SIZE)
#endif

      ! Whether we should run or quit immediately
      run = .true.

      ! Check that the sparse matrix coincides
      ! with the distribution
      nullify(ptr, ind)
#ifdef BUD_SM_CSC
      call attach(sm, nz=nz, cptr=ptr, row=ind)
#elif defined(BUD_SM_CSR)
      call attach(sm, nz=nz, rptr=ptr, col=ind)
#endif

      if ( .not. run ) return

      nullify(optr, oind, nptr)

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

#ifdef BUD_SM_CSC
        call new(out_sm, nr, out_nl, out_nz)
#elif defined(BUD_SM_CSR)
        call new(out_sm, out_nl, nc, out_nz)
#endif
        call new(out_arr, ashape(:))
        call new(out, out_dist, out_sm, out_arr)

#ifdef BUD_SM_CSC
        call attach(out_sm, cptr=optr, row=oind)
# if BUD_SM_CSC == 1
        call attach(out_sm, ncol=nptr)
# endif
#elif defined(BUD_SM_CSR)
        call attach(out_sm, rptr=optr, col=oind)
# if BUD_SM_CSR == 1
        call attach(out_sm, nrow=nptr)
# endif
#endif

        odat => array_p(out_arr)

        ! Immediately clean-up after
        ! we have retrieved the wanted information
        call delete(out_arr)
        call delete(out_sm)

        ! Initialize the first pointer value
        ! TODO check that the sparsity pattern
        ! has consecutive indices
        optr(1) = BUD_SM_PTR

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

      ! Loop on global indices
      do ig = 1 , ng

        ! Figure out the sending rank
        ! Now this rank, has to be the rank
        ! in the input distribution.
        ! So no conversion is needed here.
        send_R = g2rank(dist, ig)

        ! Figure out the recieving rank
        recv_R = ranks(g2rank(out_dist, ig))

        !print '(a,5(tr1,i2))','rank-distr', ig, rank,in_rank, send_R, recv_R

        ! Now it depends on what happens
        if ( recv_R == rank .and. &
          send_R == in_rank ) then
          ! both the recieving and the sending rank

          il = g2l(dist, ig)
          oil = g2l(out_dist, ig)

          ! Get the local indices in the incoming
          ! distribution
          id1 = ptr(il) BUD_SM_PTR_A
          id2 = ptr(il+1) BUD_SM_PTR_B

          ! Update the next pointer
          optr(oil+1) = optr(oil) + id2 - id1 + 1

          ! Immediately copy (correct for offset)
          oid1 = optr(oil) BUD_SM_PTR_A - id1

          do i = id1 , id2
            oind(oid1+i) = ind(i)
          end do

#ifdef BUD_MPI
        else if ( recv_R == rank ) then
          ! only the recieving rank

          oil = g2l(out_dist, ig)
          oid1 = optr(oil) BUD_SM_PTR_A

          call Recv(oind(oid1:), MPI_ANY_SOURCE, ig, &
            parent, stat)
          call MPI_Get_Count(stat, MPI_INTEGER, oid2, i)
          optr(oil+1) = optr(oil) + oid2

        else if ( send_R == in_rank ) then
          ! only the sending rank

          il = g2l(dist, ig)

          ! Get the local indices in the incoming
          ! distribution
          id1 = ptr(il) BUD_SM_PTR_A
          id2 = ptr(il+1) BUD_SM_PTR_B

          call ISSend(ind(id1:id2), recv_R, ig, parent, reqs(il))

#else
        else
          ! TODO something went wrong...!
#endif
        end if

      end do

#ifdef BUD_MPI
      ! Wait untill everything has been sent
      if ( allocated(reqs) ) &
        call WaitAll(nl, reqs, stats, parent)
#endif


      ! We need to update the number of elements
      ! per sparse row/column for the special sparse
      ! matrices.
#if defined(BUD_SM_CSC)
# if BUD_SM_CSC == 1
      if ( is_communicator(out_comm) ) then
        do oil = 1 , out_nl
          nptr(oil) = optr(oil+1) - optr(oil)
        end do
      end if
# endif
#elif defined(BUD_SM_CSR)
# if BUD_SM_CSR == 1
      if ( is_communicator(out_comm) ) then
        do oil = 1 , out_nl
          nptr(oil) = optr(oil+1) - optr(oil)
        end do
      end if
# endif
#endif


      ! Now we need to redistribute the array data
      ! Retrieve the data-pointer
      dat => array_p(arr)

      if ( sm_idx == dims ) then
        ! The easy stuff is when the sparse
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

            id1 = ptr(il) BUD_SM_PTR_A
            id2 = ptr(il+1) BUD_SM_PTR_B
            oid1 = optr(oil) BUD_SM_PTR_A - id1

            do i = id1 , id2
#if BUD_DIM == 1
              odat(oid1+i) = dat(i)
#elif BUD_DIM == 2
              odat(:,oid1+i) = dat(:,i)
#elif BUD_DIM == 3
              odat(:,:,oid1+i) = dat(:,:,i)
#else
# error "Only up till BUD_DIM == 3 has been implemented"
#endif
            end do

#ifdef BUD_MPI
          else if ( recv_R == rank ) then

            oil = g2l(out_dist, ig)
            oid1 = optr(oil) BUD_SM_PTR_A
            oid2 = optr(oil+1) BUD_SM_PTR_B

# if BUD_DIM == 1
            call Recv(odat(oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 2
            call Recv(odat(:,oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 3
            call Recv(odat(:,:,oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# else
#  error "Only up till BUD_DIM == 3 has been implemented"
# endif
            ! TODO check that Get_Count == oid2-oid1+1
            ! But here we do not know the data-type

          else if ( send_R == in_rank ) then

            il = g2l(dist, ig)
            id1 = ptr(il) BUD_SM_PTR_A
            id2 = ptr(il+1) BUD_SM_PTR_B

#if BUD_DIM == 1
            call ISSend(dat(id1:id2), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 2
            call ISSend(dat(:,id1:id2), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 3
            call ISSend(dat(:,:,id1:id2), recv_R, ig, &
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

      else if ( sm_idx == 1 ) then

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

             id1 = ptr(il) BUD_SM_PTR_A
             id2 = ptr(il+1) BUD_SM_PTR_B
             oid1 = optr(oil) BUD_SM_PTR_A - id1

             do i = id1 , id2
#if BUD_DIM == 2
               odat(oid1+i,i2) = dat(i,i2)
#elif BUD_DIM == 3
               odat(oid1+i,i2,i3) = dat(i,i2,i3)
#endif
             end do

#ifdef BUD_MPI
           else if ( recv_R == rank ) then

             oil = g2l(out_dist, ig)
             oid1 = optr(oil) BUD_SM_PTR_A
             oid2 = optr(oil+1) BUD_SM_PTR_B

# if BUD_DIM == 2
             call Recv(odat(oid1:oid2,i2), MPI_ANY_SOURCE, ig, &
               parent, stat)
# elif BUD_DIM == 3
             call Recv(odat(oid1:oid2,i2,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
# endif

             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)
             id1 = ptr(il) BUD_SM_PTR_A
             id2 = ptr(il+1) BUD_SM_PTR_B

#if BUD_DIM == 2
             call ISSend(dat(id1:id2,i2), recv_R, ig, &
               parent, reqs(il))
#elif BUD_DIM == 3
             call ISSend(dat(id1:id2,i2,i3), recv_R, ig, &
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
      else if ( sm_idx == 2 ) then

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

             id1 = ptr(il) BUD_SM_PTR_A
             id2 = ptr(il+1) BUD_SM_PTR_B
             oid1 = optr(oil) BUD_SM_PTR_A - id1

             do i = id1 , id2
               odat(:,oid1+i,i3) = dat(:,i,i3)
             end do

#  ifdef BUD_MPI
           else if ( recv_R == rank ) then

             oil = g2l(out_dist, ig)
             oid1 = optr(oil) BUD_SM_PTR_A
             oid2 = optr(oil+1) BUD_SM_PTR_B

             call Recv(odat(:,oid1:oid2,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)
             id1 = ptr(il) BUD_SM_PTR_A
             id2 = ptr(il+1) BUD_SM_PTR_B

             call ISSend(dat(:,id1:id2,i3), recv_R, ig, &
               parent, reqs(il))
#  else
           else
             ! TODO something went wrong!
#  endif
           end if

         end do ! ig

#  ifdef BUD_MPI
         if ( allocated(reqs) ) &
           call WaitAll(nl, reqs, stats, parent)
#  endif

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


