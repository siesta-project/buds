program dist

  ! we use everything
#include "bud_mpi.inc"

  use bud_Dist_common
  use bud_MP_Comm
  use bud_iDist1D
  use bud_rArray1D
  use bud_iDist1D_r1D

  implicit none

  integer, parameter :: N = 21
  integer, parameter :: BS = 2

  integer :: rank, nrank

  ! Distributions
  type(MP_Comm) :: world, comm1, self
  type(iDist1D) :: d1, d2, d3
  type(rArray1D) :: arr1, arr2, arr3
  type(iD1D_r1D) :: dA1, dA2, dA3

  integer :: err

#ifdef BUD_MPI
  call MPI_Init(err)
#endif

  ! This is a duplicate of the world communicator.
  call new(world, MPI_Comm_World)
  rank = comm_rank(world)
  nrank = comm_size(world)

  ! Also create a duplicate of the self communicator
  call new(self, MPI_Comm_Self)

  call print_world_self()
  call dist_1()
  call print_world_self()
  call dist_1a()
  call print_world_self()
  call dist_2()
  call print_world_self()
  call dist_3()
  call print_world_self()
  call dist_4()
  call print_world_self()

  call delete(self)
  call delete(world)

  call print_world_self()

#ifdef BUD_MPI
  call MPI_Finalize(err)
#endif

contains

  subroutine print_world_self()

    if ( rank == 0 ) then
       call print(self)
       call print(world)
       call flush(6)
    end if

    if ( is_initd(world) ) &
         call Barrier(world)

  end subroutine print_world_self


  subroutine dist_1()
    integer, pointer :: ptr(:), ind(:), ncol(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx
    print *,'dist-arr_1 -- start'

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       call new(arr1, (/N * 3/) )
       arr => array_p(arr1)

       ! Create the input distribution
       call new(dA1, d1, arr1)

       ! clean-up
       call delete(d1)
       call delete(arr1)

    end if

    ! Create the distribution
    call new(d1, self, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)

    ! Create all the output distributions
    call distribute(dA1, world, d1, dA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA1)
       call flush(6)
       call Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA2)
       call flush(6)
       call Barrier(world)
    end do

    call delete(dA1)
    call delete(dA2)

    print *,'dist-arr_1 -- end', rank

    call Barrier(world)

  end subroutine dist_1


  subroutine dist_1a()
    integer, pointer :: ptr(:), ind(:), ncol(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx
    print *,'dist-arr_1a -- start'

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       call new(arr1, (/N * 3/) )
       arr => array_p(arr1)

       ! Create the input distribution
       call new(dA1, d1, arr1)

       ! clean-up
       call delete(d1)
       call delete(arr1)

    end if

    ! Create the distribution
    if ( rank == nrank - 1 ) then
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    end if

    ! Create all the output distributions
    call distribute(dA1, world, d1, dA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA1)
       call flush(6)
       call Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA2)
       call flush(6)
       call Barrier(world)
    end do

    call delete(dA1)
    call delete(dA2)

    print *,'dist-arr_1a -- end', rank

    call Barrier(world)

  end subroutine dist_1a


  subroutine dist_2()
    integer, pointer :: ptr(:), ind(:), ncol(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx
    print *,'dist-arr_2 -- start'

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       call new(arr1, N * 3)
       arr => array_p(arr1)

       ! Create the input distribution
       call new(dA1, d1, arr1)

       ! clean-up
       call delete(d1)
       call delete(arr1)

    end if

    call Comm_Split(world, mod(rank, 2), rank, comm1)

    ! Create the distribution
    call new(d1, comm1, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dA1, world, d1, dA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA1)
       call flush(6)
       call Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA2)
       call flush(6)
       call Barrier(world)
    end do

    call delete(dA1)
    call delete(dA2)

    print *,'dist-arr_2 -- end', rank

    call Barrier(world)

  end subroutine dist_2

  subroutine dist_3()
    integer, pointer :: ptr(:), ind(:), ncol(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx
    print *,'dist-arr_3 -- start'

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       call new(arr1, N * 3)
       arr => array_p(arr1)

       ! Create the input distribution
       call new(dA1, d1, arr1)

       ! clean-up
       call delete(d1)
       call delete(arr1)

    end if

    call Comm_Split(world, mod(rank, 3), rank, comm1)

    ! Create the distribution
    call new(d1, comm1, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dA1, world, d1, dA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA1)
       call flush(6)
       call Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA2)
       call flush(6)
       call Barrier(world)
    end do

    call delete(dA1)
    call delete(dA2)

    print *,'dist-arr_3 -- end', rank

    call Barrier(world)

  end subroutine dist_3

  subroutine dist_4()
    integer, pointer :: ptr(:), ind(:), ncol(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx
    print *,'dist-arr_4 -- start'

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       call new(arr1, N * 3)
       arr => array_p(arr1)

       ! Create the input distribution
       call new(dA1, d1, arr1)

       ! clean-up
       call delete(d1)
       call delete(arr1)

    end if

    call Comm_Split(world, 0, rank, comm1)

    ! Create the distribution
    call new(d1, comm1, N, DIST_BLOCK_LAST_ALL)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dA1, world, d1, dA2)
    call delete(d1)

    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA1)
       call flush(6)
       call Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA2)
       call flush(6)
       call Barrier(world)
    end do

    ! Now dsmA1 is on rank == 0
    !     dsmA2 is distributed on all ranks
    ! Lets try and recreate dsmA1 by creating
    ! only the output distribution on rank == 0
    if ( rank == 0 ) then
       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    end if

    ! Create all the output distributions
    call distribute(dA2, world, d1, dA3)
    call delete(d1)

    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dA3)
       call flush(6)
       call Barrier(world)
    end do

    call delete(dA1)
    call delete(dA2)
    call delete(dA3)

    print *,'dist-arr_4 -- end', rank

    call Barrier(world)

  end subroutine dist_4

end program dist
