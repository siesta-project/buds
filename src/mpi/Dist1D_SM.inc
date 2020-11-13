  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"
#include "bud_mpi.inc"

  ! We require the use of the communicator
  use BUD_CC3(BUD_MOD,_,MP_Comm)

  use BUD_CC3(BUD_MOD,_,Dist_common)

#include "bud_common_declarations.inc"

#define BUD_IO_OVERLOAD
  ! Before contains
#include "bud_collection.inc"
#undef BUD_IO_OVERLOAD


  !> @param[inout] f `File` bud
  !! @param[in] this the distributed sparse matrix @bud
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm
    type(BUD_COLL_1) :: dist
    type(BUD_COLL_2) :: sm

    logical :: formatted, do_io
    integer :: iu, io_rank

    integer(BUD_PREC) :: nr, nc, nz
    integer(BUD_PREC) :: gnr, gnc, gnl, gnz
    integer(BUD_PREC) :: ir, il, ig, i, ic

    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nrc(:), indx(:)
    integer(BUD_PREC), allocatable :: data(:)

#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

    ! If file is not opened, return immediately
    if ( .not. is_initd(this) ) return

#ifndef BUD_MPI
    sm = this
    call write(f, sm)
    call delete(sm)
    return
#else

    ! Get contained data.
    dist = this
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
      call delete(sm)
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

    ! the size of the distribution
    gnl = size_global(dist)

    call attach(sm, nr=nr, nc=nc, nz=nz, &
      ptr=ptr, nentries=nrc)
    BUD_CC2(g,_CS_N) = gnl
    BUD_CC2(g,_CS_M) = _CS_M

    ! get total number of non-zero elements
    call MP_Reduce_Sum(nz, gnz, io_rank, comm)

    if ( do_io ) then

      ! First we write the size of the array
      if ( formatted ) then
        write(iu, '(i16)') gnr, gnc, gnz
        write(iu, '(l16)') sm%D%sorted
      else
        write(iu) gnr, gnc, gnz
        write(iu) sm%D%sorted
      end if

      ! Allocate space for the data
      allocate(data(gnl))

    end if

    ! Retrieve the nrc data
    ig = 1
    do while ( ig <= gnl )

      ! Get number of consecutive elements
      ic = consecutive(dist, ig)

      ! Get hosting rank and local element
      ir = global2rank(dist, ig)
      il = global2local(dist, ig)

      if ( ir == io_rank ) then

        ! Copy
        do i = 1 , ic
          data(ig-1+i) = nrc(il-1+i)
        end do

      else if ( do_io ) then

        ! recieve data
        call MP_Recv(data(ig:ig+ic-1), ir, ig, &
          comm, status)

      else

        ! send data
        call MP_SSend(nrc(il:il+ic-1), io_rank, ig, comm)

      end if

      ig = ig + ic

    end do

    if ( do_io ) then

      ! First we write the size of the array
      if ( formatted ) then
        write(iu, '(i16)') data
      else
        write(iu) data
      end if

      ig = maxval(data)
      if ( ig > gnl ) then
        deallocate(data)
        allocate(data(ig))
      end if

    end if

    ! Write the column/row indices
    do ig = 1 , gnl

      ! Get hosting rank and local element
      ir = global2rank(dist, ig)
      il = global2local(dist, ig)

      if ( ir == io_rank ) then

        indx => indices_p(sm, il)
        ! Simply write it
        if ( formatted ) then
          write(iu, '(i16)')  indx(:)
        else
          write(iu) indx(:)
        end if

      else if ( do_io ) then

        ! recieve data
        call MP_Recv(data, ir, ig, comm, status)
        call MP_Get_Count(status, BUD_MPI_PREC, ic, comm)
        if ( formatted ) then
          write(iu, '(i16)')  data(1:ic)
        else
          write(iu) data(1:ic)
        end if

      else

        ! send data
        indx => indices_p(sm, il)
        call MP_SSend(indx, io_rank, ig, comm)

      end if

    end do

    if ( allocated(data) ) deallocate(data)

    call delete(dist)
    call delete(comm)
    call delete(sm)

#endif
  end subroutine write_

  !> @param[inout] f `File` bud
  !! @param[in] this the distributed sparse matrix @bud
  subroutine read_(f, dist, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_COLL_1) :: dist
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    type( BUD_CC2(BUD_TYPE,MP_Comm) ) :: comm
    type(BUD_COLL_2) :: sm

    logical :: formatted, do_io, sorted
    integer :: iu, io_rank, my_rank

    integer(BUD_PREC) :: nr, nc, nz, nl
    integer(BUD_PREC) :: gnr, gnc, gnz, gnl
    integer(BUD_PREC) :: ir, il, ig

    integer(BUD_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: ptr(:), nrc(:), indx(:)
    integer(BUD_PREC), allocatable :: data(:), idx(:)

#ifdef BUD_MPI
    integer :: status(MPI_STATUS_SIZE)
#endif

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    ! How should we deal with the distribution when reading?
    ! Should we just create a random one and let the external use re-distribute
    ! the data? Yeah, probably
    ! Also, we need a communicator...
    ! So this one needs an additional argument.
    comm = dist

#ifndef BUD_MPI
    call read(f, sm)
    call new(this, dist, sm)
    call delete(sm)
    return
#else

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

    ! First we need to read the array dimensions...
    if ( do_io ) then

      if ( formatted ) then
        read(iu, '(i16)') gnr, gnc, gnz
        read(iu, '(l16)') sorted
      else
        read(iu) gnr, gnc, gnz
        read(iu) sorted
      end if

    end if

    if ( size_global(dist) /= BUD_CC2(g, _CS_N) ) then
      call delete(this)
      call set_error(this, -1)
      return
    end if

    ! Now get local sizes...
    _CS_N = size_local(dist)
    _CS_M = BUD_CC2(g,_CS_M)
    nl = _CS_N
    gnl = BUD_CC2(g,_CS_N)

    ! Allocate the container for the local data
    allocate(data(gnl))

    if ( do_io ) then
      if ( formatted ) then
        read(iu, '(i16)') data
      else
        read(iu) data
      end if
    end if

    ! Now distribute data so every processor knows the size
    call MP_Bcast(data, io_rank, comm)

    ! Retrieve my rank ID
    my_rank = comm_rank(comm)

    ! All ranks should now figure out how many elements
    nz = 0
    do ig = 1 , gnl

      ir = global2rank(dist, ig)

      if ( ir == my_rank ) then
        nz = nz + data(ig)
      end if

    end do

    ! Create the SM
    call new(sm, nr, nc, nz)
    ! Create the new object
    call new(this, dist, sm)

    ! Retrieve pointers and populate
    call attach(sm, ptr=ptr, nentries=nrc)

    ! distribute number of elements per row/column
    do ig = 1 , gnl

      ir = global2rank(dist, ig)

      if ( ir == my_rank ) then
        il = global2local(dist, ig)
        nrc(il) = data(ig)
      end if

    end do

    ! Update pointer
    do il = 2 , nl
      ptr(il) = ptr(il-1) + nrc(il-1)
    end do
    ptr(nl+1) = ptr(nl) + nrc(nl)

    ! Read the sparse data
    ig = maxval(data)
    allocate(idx(ig))

    ! Read the sparse matrix
    do ig = 1 , gnl

      ! get global rank
      ir = global2rank(dist, ig)

      if ( ir == my_rank ) then
        il = global2local(dist, ig)

        indx => indices_p(sm, il)
      end if

      if ( do_io ) then

        if ( ir == my_rank ) then
          if ( formatted ) then
            read(iu, '(i16)') indx
          else
            read(iu) indx
          end if
        else
          if ( formatted ) then
            read(iu, '(i16)') idx(1:data(ig))
          else
            read(iu) idx(1:data(ig))
          end if

          ! Send the data
          call MP_SSend(idx(:data(ig)), ir, ig, comm)

        end if

      else if ( ir == my_rank ) then

        ! Recieve data from io-node
        call MP_Recv(indx(:data(ig)), io_rank, ig, comm, status)

      end if

    end do

    if ( sorted ) then
      call sort(sm)
    end if

    ! done, clean-up
    call delete(sm)
    deallocate(data, idx)

#endif
  end subroutine read_

  ! after contains
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

