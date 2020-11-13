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

  ! These fields are used in the sparse matrix stuff.
  integer(BUD_PREC), parameter :: ONE = BUD_CC2(1_,BUD_PREC)
  integer(BUD_PREC), parameter :: ZERO = BUD_CC2(0_,BUD_PREC)

!# define BUD_INCLUDE_TYPE "Dist1D_SM_Array_type.inc"
# define BUD_INCLUDE_TYPE_ "Dist1D_SM_Array_type_.inc"

  ! Currently we only allow two different sparse matrices
#if defined(BUD_SM_CSC)
#elif defined(BUD_SM_CSR)
#else
# error "Could not figure out which sparse matrix this is!"
#endif

  !> Create a new object only with the distribution and sparse matrix
  interface new
    module procedure new_dist_sm_
  end interface

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
    module procedure get_matrix_p_
    module procedure get_matrix_ip_
  end interface
  public :: matrix_p

  !> Query the index of the sparse matrix (distributed index)
  interface sparse_index
    module procedure sparse_index_
  end interface
  public :: sparse_index

  !> Query the distributed index (sparse matrix index)
  interface distributed_index
    module procedure sparse_index_
  end interface
  public :: distributed_index

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

  !> @param[inout] this sparse matrix with distribution and array
  !! @param[in] dist the distribution function
  !! @param[in] sm the sparse matrix to instantiate from
#if BUD_DIM > 1
  !! @param[in] shape the shape (`-1` refers to the sparse index)
#endif
  subroutine new_dist_sm_(this, dist, sm &
#if BUD_DIM > 1
    , shape &
#endif
    )
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    BUD_CLASS(BUD_COLL_1), intent(in) :: dist
    BUD_CLASS(BUD_COLL_2), intent(in) :: sm
#if BUD_DIM > 1
    integer, intent(in) :: shape(BUD_DIM)
#endif
    type(BUD_COLL_3) :: arr
#if BUD_DIM > 1
    integer :: shap(BUD_DIM), i
    integer :: found

    found = 0
    do i = 1 , BUD_DIM
      if ( shape(i) < 0 ) then
        shap(i) = max_nonzeros(sm)
        found = i
      else
        shap(i) = shape(i)
      end if
    end do

    ! The sparse index was not specified.
    ! Tell the user that something went wrong
    if ( found == 0 ) then
      call delete(this)
      call set_error(this, -1)
      return
    end if

    call new(arr, shap)
#else
    call new(arr, max_nonzeros(sm))
#endif

    ! Create the array, then the new type
    call new(this, dist, sm, arr)
    call delete(arr)
#if BUD_DIM > 1
    this%D%sm_idx = found
#endif

  end subroutine new_dist_sm_

  !> @param[in] this sparse matrix
  !! @return a pointer to the data (contiguous)
  function get_matrix_p_(this) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    p => array_p(this%D%e3)

  end function get_matrix_p_


  !> @param[in] this sparse matrix
  !! @return a pointer to the data (not necessarily contiguous)
  function get_matrix_ip_(this, i) result(p)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer(BUD_PREC), intent(in) :: i
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer :: p BUD_DIM_SPEC(:)
#endif
    integer(BUD_PREC) :: i1, i2
    integer :: sm_idx

    p => array_p(this%D%e3)

    ! get sparse indices
    i1 = this%D%e2%D% _CS_PTR(i)BUD_SM_PTR_A
    i2 = this%D%e2%D% _CS_PTR(i)+this%D%e2%D% _CS_NUM(i)BUD_SM_PTR_B

    ! Get the sub-pointer
#if BUD_DIM == 1
    p => p(i1:i2)
#elif BUD_DIM == 2
    select case ( sm_idx )
    case ( 1 )
      ! non-contiguous
      p => p(i1:i2,:)
    case ( 2 )
      ! contiguous
      p => p(:,i1:i2)
    end select
#elif BUD_DIM == 3
    select case ( sm_idx )
    case ( 1 )
      ! non-contiguous
      p => p(i1:i2,:,:)
    case ( 2 )
      ! non-contiguous
      p => p(:,i1:i2,:)
    case ( 3 )
      ! contiguous
      p => p(:,:,i1:i2)
    end select
#endif

  end function get_matrix_ip_

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
      call MP_AllReduce_Sum(_CS_N, ir, comm)
      _CS_N = ir
    end if
    call MP_AllReduce_Max(nr, ir, parent)
    nr = ir
    call MP_AllReduce_Max(nc, ir, parent)
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
    call MP_AllReduce_Max(sm_idx, ir, parent)
    sm_idx = ir
    call MP_AllReduce_Max(dims, ir, parent)
    dims = ir
#endif
    allocate(ashape(dims))
    ashape = -1
    do ir = 1 , dims
      ashape(ir) = size(arr, ir)
    end do
#ifdef BUD_MPI
    allocate(itmp1(dims))
    call MP_AllReduce_Max(ashape, itmp1, parent)
    ashape = itmp1
    deallocate(itmp1)
    call MP_AllReduce_Max(msg_size, ltmp, parent)
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
      call MP_AllReduce_Max(ir, my_root, out_comm)
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
    call MP_AllReduce_Max(ng, nl, parent)
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
      call MP_AllReduce_LOr(my_distr, is_distr, parent)
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

      integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), ind(:)

      integer :: ir, recv_R
      integer(BUD_PREC) :: n, id1, id2
      integer(BUD_PREC) :: il, ig

      nz = 0

      call attach(sm, ptr=ptr, indices=ind)

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
        call MP_Reduce_Sum(n, nz, recv_R, parent)
#else
        ! With no MPI it must be its own distribution
        nz = n
#endif

      end do

    end function reduce_size

    subroutine sub_dist(out_dist)
      use BUD_CC2(BUD_MOD,_Transfer), only: transfer_dim

      BUD_CLASS(BUD_COLL_1), intent(inout) :: out_dist

      ! Define the objects that are the new containers
      type(BUD_TYPE_MP_COMM) :: out_comm
      type(BUD_COLL_2) :: out_sm
      type(BUD_COLL_3) :: out_arr

      logical :: run

      ! The pointers to the input data
      integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), ind(:), nrc(:)
#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
#endif
      ! The pointers to the output data
      integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: optr(:), oind(:), onrc(:)

#ifdef BUD_TYPE_VAR_PREC
      BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: odat BUD_DIM_SPEC(:)
#else
      BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: odat BUD_DIM_SPEC(:)
#endif

      ! The local counter integers (local and global)
      integer(BUD_PREC) :: il, ig, oil
      ! index pointers
      integer(BUD_PREC) :: id1, id2, oid1, oid2
      integer(BUD_PREC) :: out_nl

#if BUD_DIM > 1
      ! Number of elements copied
      integer(BUD_PREC) :: ncopy
#endif

      integer(BUD_PREC) :: i, i2
#if BUD_DIM > 2
      integer(BUD_PREC) :: i3
#endif
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
      call attach(sm, nz=nz, ptr=ptr, indices=ind, nentries=nrc)

      if ( .not. run ) return

      nullify(optr, oind, onrc)

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

        call attach(out_sm, ptr=optr, indices=oind, nentries=onrc)

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
          id2 = id1 + nrc(il) BUD_SM_PTR_B

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

          call MP_Recv(oind(oid1:), MPI_ANY_SOURCE, ig, &
            parent, stat)
          call MP_Get_Count(stat, MPI_INTEGER, oid2, parent)
          optr(oil+1) = optr(oil) + oid2

        else if ( send_R == in_rank ) then
          ! only the sending rank

          il = g2l(dist, ig)

          ! Get the local indices in the incoming
          ! distribution
          id1 = ptr(il) BUD_SM_PTR_A
          id2 = id1 + nrc(il) BUD_SM_PTR_B

          call MP_ISSend(ind(id1:id2), recv_R, ig, parent, reqs(il))

#else
        else
          ! TODO something went wrong...!
#endif
        end if

      end do

#ifdef BUD_MPI
      ! Wait untill everything has been sent
      if ( allocated(reqs) ) &
        call MP_WaitAll(nl, reqs, stats, parent)
#endif


      ! We need to update the number of elements
      ! per sparse row/column for the special sparse
      ! matrices.
      if ( is_communicator(out_comm) ) then
        do oil = 1 , out_nl
          onrc(oil) = optr(oil+1) - optr(oil)
        end do
      end if


      ! Now we need to redistribute the array data
      ! Retrieve the data-pointer
      dat => array_p(arr)

      if ( sm_idx == dims ) then
        ! The easy stuff is when the sparse
        ! dimension is the same as the last dimension

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

            id1 = ptr(il) BUD_SM_PTR_A
            id2 = id1 + nrc(il) BUD_SM_PTR_B
            oid1 = optr(oil) BUD_SM_PTR_A - id1

            do i = id1 , id2
#if BUD_DIM == 1
              odat(oid1+i) = dat(i)
#elif BUD_DIM == 2
              call transfer_dim(ncopy, odat(:,oid1+i), ncopy, dat(:,i))
#elif BUD_DIM == 3
              call transfer_dim(ncopy, 1, odat(:,:,oid1+i), ncopy, 1, dat(:,:,i))
#else
# error "Only up till BUD_DIM == 3 has been implemented"
#endif
            end do

#ifdef BUD_MPI
          else if ( recv_R == rank ) then

            oil = g2l(out_dist, ig)
            oid1 = optr(oil) BUD_SM_PTR_A
            oid2 = oid1 + onrc(oil) BUD_SM_PTR_B

# if BUD_DIM == 1
            call MP_Recv(odat(oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 2
            call MP_Recv(odat(:,oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# elif BUD_DIM == 3
            call MP_Recv(odat(:,:,oid1:oid2), MPI_ANY_SOURCE, ig, &
              parent, stat)
# else
#  error "Only up till BUD_DIM == 3 has been implemented"
# endif
            ! TODO check that Get_Count == oid2-oid1+1
            ! But here we do not know the data-type

          else if ( send_R == in_rank ) then

            il = g2l(dist, ig)
            id1 = ptr(il) BUD_SM_PTR_A
            id2 = id1 + nrc(il) BUD_SM_PTR_B

#if BUD_DIM == 1
            call MP_ISSend(dat(id1:id2), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 2
            call MP_ISSend(dat(:,id1:id2), recv_R, ig, &
              parent, reqs(il))
#elif BUD_DIM == 3
            call MP_ISSend(dat(:,:,id1:id2), recv_R, ig, &
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
          call MP_WaitAll(nl, reqs, stats, parent)
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
             id2 = id1 + nrc(il) BUD_SM_PTR_B
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
             oid2 = oid1 + onrc(il) BUD_SM_PTR_B

# if BUD_DIM == 2
             call MP_Recv(odat(oid1:oid2,i2), MPI_ANY_SOURCE, ig, &
               parent, stat)
# elif BUD_DIM == 3
             call MP_Recv(odat(oid1:oid2,i2,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
# endif

             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)
             id1 = ptr(il) BUD_SM_PTR_A
             id2 = id1 + nrc(il) BUD_SM_PTR_B

#if BUD_DIM == 2
             call MP_ISSend(dat(id1:id2,i2), recv_R, ig, &
               parent, reqs(il))
#elif BUD_DIM == 3
             call MP_ISSend(dat(id1:id2,i2,i3), recv_R, ig, &
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
           call MP_WaitAll(nl, reqs, stats, parent)
#endif

        end do ! i2
#if BUD_DIM > 2
       end do ! i3
#endif

#if BUD_DIM > 1
      else if ( sm_idx == 2 ) then

       ncopy = size(dat, 1)

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
             id2 = id1 + nrc(il) BUD_SM_PTR_B
             oid1 = optr(oil) BUD_SM_PTR_A - id1

             do i = id1 , id2
               call transfer_dim(ncopy, odat(:,oid1+i,i3), &
                 ncopy, dat(:,i,i3))
             end do

#  ifdef BUD_MPI
           else if ( recv_R == rank ) then

             oil = g2l(out_dist, ig)
             oid1 = optr(oil) BUD_SM_PTR_A
             oid2 = oid1 + onrc(il) BUD_SM_PTR_B

             call MP_Recv(odat(:,oid1:oid2,i3), MPI_ANY_SOURCE, ig, &
               parent, stat)
             ! TODO check that Get_Count == oid2-oid1+1
             ! But here we do not know the data-type

           else if ( send_R == in_rank ) then

             il = g2l(dist, ig)
             id1 = ptr(il) BUD_SM_PTR_A
             id2 = id1 + nrc(il) BUD_SM_PTR_B

             call MP_ISSend(dat(:,id1:id2,i3), recv_R, ig, &
               parent, reqs(il))
#  else
           else
             ! TODO something went wrong!
#  endif
           end if

         end do ! ig

#  ifdef BUD_MPI
         if ( allocated(reqs) ) &
           call MP_WaitAll(nl, reqs, stats, parent)
#  endif

       end do ! i3
# endif
#endif
      end if

#ifdef BUD_MPI
      if ( allocated(reqs) ) deallocate(reqs)

      ! Be sure to wait for the next iteration
      call MP_Barrier(parent)
#endif

      call delete(out_comm)

    end subroutine sub_dist

  end subroutine distribute_


  !> @param[inout] f `File` bud
  !! @param[in] this the distributed sparse matrix @bud
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    use BUD_MOD_1_2
#ifndef BUD_MPI
    use BUD_MOD_2_3
#endif

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm
    type(BUD_COLL_1) :: dist
    type(BUD_COLL_2) :: sm
    type(BUD_COLL_3) :: arr

    logical :: formatted, do_io
    integer :: iu, io_rank

    ! Indices
    integer :: i, i2, i3, ir
    integer(BUD_PREC) :: ig, il, gnr, gnc, gnl, gnrc, nr, nc, nz

    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), ind(:), nrc(:)
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), allocatable :: rdat(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: dat BUD_DIM_SPEC(:)
    BUD_TYPE_VAR, allocatable :: rdat(:)
#endif

    integer :: sm_idx, dims
    integer, allocatable :: ashape(:)
#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

    type(BUD_TYPE_1_2) :: d1d
#ifndef BUD_MPI
    type(BUD_TYPE_2_3) :: sm_arr
#endif

    ! If file is not opened, return immediately
    if ( .not. is_initd(this) ) return

#ifndef BUD_MPI
    sm = this
    arr = this
    call new(sm_arr, sm, arr)
    call write(f, sm_arr)
    call delete(sm_arr)
    call delete(sm)
    call delete(arr)
    return
#else

    ! Get contained data.
    dist = this
    comm = dist
    sm = this
    arr = this

    if ( is_open(f) ) then
      io_rank = comm_rank(comm)
    else
      io_rank = -1
    end if
    call MP_AllReduce_Max(io_rank, iu, comm)
    io_rank = iu

    ! Check if the file is open on one rank
    if ( io_rank < 0 ) then
      call delete(dist)
      call delete(comm)
      call delete(sm)
      call delete(arr)
      return
    end if

    ! This ensures that only one rank will do the writing
    ! in case multiple ranks have the file open
    ! only @bud knows what will happen in this case!!!
    do_io = io_rank == comm_rank(comm)

    if ( do_io ) then
      ! First figure out if the file is an unformatted file
      formatted = is_formatted(f)
      iu = unit(f)
    end if

    call new(d1d)
    d1d = dist
    d1d = sm
    call write(f, d1d)
    call delete(d1d)

    ! Retrieve the shape of the arrays
    allocate(ashape(dims))
    ashape = -1
    do i = 1 , dims
      ashape(i) = size(arr, i)
    end do
    call delete(arr)

    ! Get the sparse-index
    sm_idx = sparse_index(this)
    ! figure out number of dimensions
    dims = dimensions(this)
    ! Denote the sparse index here
    ashape(sm_idx) = -1

    ! Now we should write the dimensionality of the data
    if ( do_io ) then

      if ( formatted ) then
        write(iu, '(i16)') dims, sm_idx
      else
        write(iu) dims, sm_idx
      end if

      ! Now write all the other dimenions, if it is
      ! anything but 1
      if ( formatted ) then
        write(iu, '(i16)') ashape
      else
        write(iu) ashape
      end if

    end if

    call attach(sm, nr=nr, nc=nc, nz=nz, &
      ptr=ptr, nentries=nrc)
    BUD_CC2(g,_CS_N) = _CS_N
    BUD_CC2(g,_CS_M) = _CS_M
    gnrc = maxval(nrc)
    call MP_AllReduce_Max(gnrc, ig, comm)
    gnrc = ig
    if ( sm_idx > 1 ) then
      ! Rescale the number of send elements
      gnrc = gnrc * product(ashape(1:sm_idx-1))
    end if
    if ( do_io ) then
      allocate(rdat(gnrc))
    end if

    ! Now there are a couple of cases
    if ( sm_idx == dims ) then
      ! The easy stuff is when the sparse
      ! dimension is the same as the last dimension

      ! Now write data
      do ig = 1 , gnl

        ! Get hosting rank and local element
        ir = global2rank(dist, ig)
        il = global2local(dist, ig)

        if ( ir == io_rank ) then

          dat => matrix_p(this, il)

          ! Simply write it
          if ( formatted ) then
#ifdef BUD_IS_INTEGER
            write(iu, '(i16)') dat
#else
            write(iu, '(e20.16)') dat
#endif
          else
            write(iu) dat
          end if

        else if ( do_io ) then

          ! recieve data
          call MP_Recv(rdat(1), gnrc, ir, ig, comm, status)
          if ( formatted ) then
#ifdef BUD_IS_INTEGER
            write(iu, '(i16)') rdat
#else
            write(iu, '(e20.16)') rdat
#endif
          else
            write(iu) rdat
          end if

        else

          ! send data
          dat => matrix_p(this, il)
          call MP_SSend(dat BUD_DIM_SPEC(1), gnrc, io_rank, ig, comm)

        end if

      end do

    else if ( sm_idx == 1 ) then

      ! The first index is the sparse element...
#if BUD_DIM > 2
      do i3 = 1 , ashape(3)
#endif
        do i2 = 1 , ashape(2)

          ! Now write data
          do ig = 1 , gnl

            ! Get hosting rank and local element
            ir = global2rank(dist, ig)
            il = global2local(dist, ig)

            if ( ir == io_rank ) then

              dat => matrix_p(this, il)

              ! Simply write it
              if ( formatted ) then
#ifdef BUD_IS_INTEGER
#if BUD_DIM == 2
                write(iu, '(i16)') dat(:, i2)
#elif BUD_DIM == 3
                write(iu, '(i16)') dat(:, i2, i3)
#endif
#else
#if BUD_DIM == 2
                write(iu, '(e20.16)') dat(:, i2)
#elif BUD_DIM == 3
                write(iu, '(e20.16)') dat(:, i2, i3)
#endif
#endif
              else
#if BUD_DIM == 2
                write(iu) dat(:, i2)
#elif BUD_DIM == 3
                write(iu) dat(:, i2, i3)
#endif
              end if

            else if ( do_io ) then

              ! recieve data
              call MP_Recv(rdat(1), gnrc, ir, ig, comm, status)
              if ( formatted ) then
#ifdef BUD_IS_INTEGER
                write(iu, '(i16)') rdat
#else
                write(iu, '(e20.16)') rdat
#endif
              else
                write(iu) rdat
              end if

            else

              ! send data
              dat => matrix_p(this, il)
#if BUD_DIM == 2
              call MP_SSend(dat(1,i2), gnrc, io_rank, ig, comm)
#elif BUD_DIM == 3
              call MP_SSend(dat(1,i2,i3), gnrc, io_rank, ig, comm)
#endif
            end if

          end do ! distributed (global) index

        end do
#if BUD_DIM > 2
      end do
#endif

#if BUD_DIM > 1
    else if ( sm_idx == 2 ) then

# if BUD_DIM == 3
      do i3 = 1 , ashape(3)

        ! Now write data
        do ig = 1 , gnl

          ! Get hosting rank and local element
          ir = global2rank(dist, ig)
          il = global2local(dist, ig)

          if ( ir == io_rank ) then

            dat => matrix_p(this, il)

            ! Simply write it
            if ( formatted ) then
#ifdef BUD_IS_INTEGER
              write(iu, '(i16)') dat(:, :, i3)
#else
              write(iu, '(e20.16)') dat(:, :, i3)
#endif
            else
              write(iu) dat(:, :, i3)
            end if

          else if ( do_io ) then

            ! recieve data
            call MP_Recv(rdat(1), gnrc, ir, ig, comm, status)
            if ( formatted ) then
#ifdef BUD_IS_INTEGER
              write(iu, '(i16)') rdat
#else
              write(iu, '(e20.16)') rdat
#endif
            else
              write(iu) rdat
            end if

          else

            ! send data
            dat => matrix_p(this, il)
            call MP_SSend(dat(1,1,i3), gnrc, io_rank, ig, comm)

          end if

        end do ! distributed (global) index

      end do
# endif
#endif
    end if

    deallocate(ashape)
    if ( allocated(rdat) ) deallocate(rdat)

    call delete(dist)
    call delete(comm)
    call delete(sm)

#endif
  end subroutine write_

  !> @param[inout] f `File` bud
  !! @param[in] this the distributed sparse matrix @bud
  subroutine read_(f, dist, this)
    use BUD_CC2(BUD_MOD,_File)
    use BUD_MOD_1_2
#ifndef BUD_MPI
    use BUD_MOD_2_3
#endif

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_COLL_1) :: dist
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm
    type(BUD_COLL_2) :: sm
    type(BUD_COLL_3) :: arr

    logical :: formatted, do_io
    integer :: iu, io_rank

    ! Indices
    integer :: i, i2, i3, ir
    integer(BUD_PREC) :: ig, il, gnr, gnc, gnl, gnrc, nr, nc, nz

    type(BUD_TYPE_1_2) :: d1d
#ifndef BUD_MPI
    type(BUD_TYPE_2_3) :: sm_arr
#endif

    integer :: sm_idx, dims
    integer, allocatable :: ashape(:)
#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

        ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

#ifndef BUD_MPI
    call read(f, sm_arr)
    sm = sm_arr
    arr = sm_arr
    call delete(sm_arr)
    call new(this, dist, sm, arr)
    call delete(sm)
    call delete(arr)
    return
#else

    ! How should we deal with the distribution when reading?
    ! Should we just create a random one and let the external use re-distribute
    ! the data? Yeah, probably
    ! Also, we need a communicator...
    ! So this one needs an additional argument.
    comm = dist

    if ( is_open(f) ) then
      io_rank = comm_rank(comm)
    else
      io_rank = -1
    end if
    call MP_AllReduce_Max(io_rank, iu, comm)
    io_rank = iu

    ! Check if the file is open on one rank
    if ( io_rank < 0 ) then
      call delete(dist)
      call delete(comm)
      return
    end if

    ! Read the distribution and the sparse matrix
    call read(f, dist, d1d)
    sm = d1d
    call delete(d1d)

    ! Now we should read the sparse data
    ! This ensures that only one rank will do the writing
    ! in case multiple ranks have the file open
    ! only @bud knows what will happen in this case!!!
    do_io = io_rank == comm_rank(comm)

    if ( do_io ) then
      ! First figure out if the file is an unformatted file
      formatted = is_formatted(f)
      iu = unit(f)
    end if

    ! Now we should read the dimensionality of the data
    if ( do_io ) then

      if ( formatted ) then
        read(iu, '(i16)') dims, sm_idx
      else
        read(iu) dims, sm_idx
      end if

      allocate(ashape(dims))

      ! Now write all the other dimenions, if it is
      ! anything but 1
      if ( formatted ) then
        read(iu, '(i16)') ashape
      else
        read(iu) ashape
      end if

    end if

    call MP_Bcast(dims, io_rank, comm)
    call MP_Bcast(sm_idx, io_rank, comm)
    if ( .not. do_io ) then
      allocate(ashape(dims))
    end if
    call MP_Bcast(ashape, io_rank, comm)

    ! Now we have the size of the arrays
#endif
  end subroutine read_


#undef BUD_MOD_2_3
#undef BUD_TYPE_2_3


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


