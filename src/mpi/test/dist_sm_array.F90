program dist

#include "bud_mpi.inc"
  use test_utils

  use bud_Dist_common
  use bud_MP_Comm
  use bud_iDist1D
  use bud_iSM_CSC0
  use bud_rArray1D
  use bud_iDist1D_CSC0_r1D

  implicit none

  integer, parameter :: N = 21
  integer, parameter :: BS = 2

  integer :: rank, nrank

  ! Distributions
  type(MP_Comm) :: world, comm1, self
  type(iDist1D) :: d1, d2, d3
  type(iSM_CSC0) :: sm1, sm2, sm3
  type(rArray1D) :: arr1, arr2, arr3
  type(iD1D_CSC0_r1D) :: dsmA1, dsmA2, dsmA3

  integer :: err

#ifdef BUD_MPI
  call MPI_Init(err)
#else
  integer, parameter :: MPI_Comm_World = 1
  integer, parameter :: MPI_Comm_Self = 2
#endif

#ifndef BUD_MPI
  ! Currently we HAVE to rely on MPI
  return
#endif

  ! This is a duplicate of the world communicator.
  call new(world, MPI_Comm_World)
  rank = comm_rank(world)
  nrank = comm_size(world)

  ! Also create a duplicate of the self communicator
  call new(self, MPI_Comm_Self)

  call print_world_self()
  call start_test('DIST_SM_ARRAY - World to Self (all)',rank==0)
  call dist_1()
  call end_test('DIST_SM_ARRAY - World to Self (all)',rank==0)
  call print_world_self()
  call start_test('DIST_SM_ARRAY - World to Self (1)',rank==0)
  call dist_1a()
  call end_test('DIST_SM_ARRAY - World to Self (1)',rank==0)
  call print_world_self()
  call start_test('DIST_SM_ARRAY - World to Split(2)',rank==0)
  call dist_2()
  call end_test('DIST_SM_ARRAY - World to Split(2)',rank==0)
  call print_world_self()
  call start_test('DIST_SM_ARRAY - World to Split(3)',rank==0)
  call dist_3()
  call end_test('DIST_SM_ARRAY - World to Split(3)',rank==0)
  call print_world_self()
  call start_test('DIST_SM_ARRAY - World to Split(N)',rank==0)
  call dist_4()
  call end_test('DIST_SM_ARRAY - World to Split(N)',rank==0)
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
         call MP_Barrier(world)

  end subroutine print_world_self


  subroutine dist_1()
    integer, pointer :: ptr(:), ind(:), nrow(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       ! Now create the sparse matrix
       ! We will create a tri-diagonal sparse
       ! matrix...
       call new(sm1, N, N, N * 3)
       call new(arr1, N * 3)
       arr => array_p(arr1)

       call attach(sm1, cptr=ptr, row=ind, n_row=nrow)

       ! Create the tri-diagonal thing
       ptr(1) = 0
       do ic = 1 , N
          ! Correct pointers...
          nrow(ic) = 3
          ptr(ic+1) = ptr(ic) + 3

          idx = ptr(ic) + 1 ! we know it is CSC0
          if ( ic - 1 < 1 ) then
             ind(idx) = N
          else
             ind(idx) = ic - 1
          end if
          arr(idx) = -1

          idx = idx + 1
          ind(idx) = ic
          arr(idx) = 0

          idx = idx + 1
          if ( ic + 1 > N ) then
             ind(idx) = 1
          else
             ind(idx) = ic + 1
          end if
          arr(idx) = 1

       end do

       ! Create the input distribution
       call new(dsmA1, d1, sm1, arr1)

       ! clean-up
       call delete(d1)
       call delete(sm1)
       call delete(arr1)

    end if

    ! Create the distribution
    call new(d1, self, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)

    ! Create all the output distributions
    call distribute(dsmA1, world, d1, dsmA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA1)
       call flush(6)
       call MP_Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA2)
       call flush(6)
       call MP_Barrier(world)
    end do

    call delete(dsmA1)
    call delete(dsmA2)

    call MP_Barrier(world)

  end subroutine dist_1

  subroutine dist_1a()
    integer, pointer :: ptr(:), ind(:), nrow(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       ! Now create the sparse matrix
       ! We will create a tri-diagonal sparse
       ! matrix...
       call new(sm1, N, N, N * 3)
       call new(arr1, N * 3)
       arr => array_p(arr1)

       call attach(sm1, cptr=ptr, row=ind, n_row=nrow)

       ! Create the tri-diagonal thing
       ptr(1) = 0
       do ic = 1 , N
          ! Correct pointers...
          nrow(ic) = 3
          ptr(ic+1) = ptr(ic) + 3

          idx = ptr(ic) + 1 ! we know it is CSC0
          if ( ic - 1 < 1 ) then
             ind(idx) = N
          else
             ind(idx) = ic - 1
          end if
          arr(idx) = -1

          idx = idx + 1
          ind(idx) = ic
          arr(idx) = 0

          idx = idx + 1
          if ( ic + 1 > N ) then
             ind(idx) = 1
          else
             ind(idx) = ic + 1
          end if
          arr(idx) = 1

       end do

       ! Create the input distribution
       call new(dsmA1, d1, sm1, arr1)

       ! clean-up
       call delete(d1)
       call delete(sm1)
       call delete(arr1)

    end if

    ! Create the distribution
    if  ( rank == nrank - 1 ) then
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    end if

    ! Create all the output distributions
    call distribute(dsmA1, world, d1, dsmA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA1)
       call flush(6)
       call MP_Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA2)
       call flush(6)
       call MP_Barrier(world)
    end do

    call delete(dsmA1)
    call delete(dsmA2)

    call MP_Barrier(world)

  end subroutine dist_1a


  subroutine dist_2()
    integer, pointer :: ptr(:), ind(:), nrow(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       ! Now create the sparse matrix
       ! We will create a tri-diagonal sparse
       ! matrix...
       call new(sm1, N, N, N * 3)
       call new(arr1, N * 3)
       arr => array_p(arr1)

       call attach(sm1, cptr=ptr, row=ind, n_row=nrow)

       ! Create the tri-diagonal thing
       ptr(1) = 0
       do ic = 1 , N
          ! Correct pointers...
          nrow(ic) = 3
          ptr(ic+1) = ptr(ic) + 3

          idx = ptr(ic) + 1 ! we know it is CSC0
          if ( ic - 1 < 1 ) then
             ind(idx) = N
          else
             ind(idx) = ic - 1
          end if
          arr(idx) = -1

          idx = idx + 1
          ind(idx) = ic
          arr(idx) = 0

          idx = idx + 1
          if ( ic + 1 > N ) then
             ind(idx) = 1
          else
             ind(idx) = ic + 1
          end if
          arr(idx) = 1

       end do

       ! Create the input distribution
       call new(dsmA1, d1, sm1, arr1)

       ! clean-up
       call delete(d1)
       call delete(sm1)
       call delete(arr1)

    end if

#ifdef BUD_MPI
    call Comm_Split(world, mod(rank, 2), rank, comm1)
#endif

    ! Create the distribution
    call new(d1, comm1, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dsmA1, world, d1, dsmA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA1)
       call flush(6)
       call MP_Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA2)
       call flush(6)
       call MP_Barrier(world)
    end do

    call delete(dsmA1)
    call delete(dsmA2)

    call MP_Barrier(world)

  end subroutine dist_2

  subroutine dist_3()
    integer, pointer :: ptr(:), ind(:), nrow(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       ! Now create the sparse matrix
       ! We will create a tri-diagonal sparse
       ! matrix...
       call new(sm1, N, N, N * 3)
       call new(arr1, N * 3)
       arr => array_p(arr1)

       call attach(sm1, cptr=ptr, row=ind, n_row=nrow)

       ! Create the tri-diagonal thing
       ptr(1) = 0
       do ic = 1 , N
          ! Correct pointers...
          nrow(ic) = 3
          ptr(ic+1) = ptr(ic) + 3

          idx = ptr(ic) + 1 ! we know it is CSC0
          if ( ic - 1 < 1 ) then
             ind(idx) = N
          else
             ind(idx) = ic - 1
          end if
          arr(idx) = -1

          idx = idx + 1
          ind(idx) = ic
          arr(idx) = 0

          idx = idx + 1
          if ( ic + 1 > N ) then
             ind(idx) = 1
          else
             ind(idx) = ic + 1
          end if
          arr(idx) = 1

       end do

       ! Create the input distribution
       call new(dsmA1, d1, sm1, arr1)

       ! clean-up
       call delete(d1)
       call delete(sm1)
       call delete(arr1)

    end if

#ifdef BUD_MPI
    call Comm_Split(world, mod(rank, 3), rank, comm1)
#endif

    ! Create the distribution
    call new(d1, comm1, BS, N, &
         DIST_BLOCK_CYCLIC_FIRST_SPLIT)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dsmA1, world, d1, dsmA2)
    call delete(d1)
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA1)
       call flush(6)
       call MP_Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA2)
       call flush(6)
       call MP_Barrier(world)
    end do

    call delete(dsmA1)
    call delete(dsmA2)

    call MP_Barrier(world)

  end subroutine dist_3

  subroutine dist_4()
    integer, pointer :: ptr(:), ind(:), nrow(:)
    real, pointer :: arr(:)
    integer :: ic, i, idx

    if ( comm_rank(world) == 0 ) then

       ! Create the distribution
       call new(d1, self, BS, N, &
            DIST_BLOCK_CYCLIC_FIRST_SPLIT)

       ! Now create the sparse matrix
       ! We will create a tri-diagonal sparse
       ! matrix...
       call new(sm1, N, N, N * 3)
       call new(arr1, N * 3)
       arr => array_p(arr1)

       call attach(sm1, cptr=ptr, row=ind, n_row=nrow)

       ! Create the tri-diagonal thing
       ptr(1) = 0
       do ic = 1 , N
          ! Correct pointers...
          nrow(ic) = 3
          ptr(ic+1) = ptr(ic) + 3

          idx = ptr(ic) + 1 ! we know it is CSC0
          if ( ic - 1 < 1 ) then
             ind(idx) = N
          else
             ind(idx) = ic - 1
          end if
          arr(idx) = -1

          idx = idx + 1
          ind(idx) = ic
          arr(idx) = 0

          idx = idx + 1
          if ( ic + 1 > N ) then
             ind(idx) = 1
          else
             ind(idx) = ic + 1
          end if
          arr(idx) = 1

       end do

       ! Create the input distribution
       call new(dsmA1, d1, sm1, arr1)

       ! clean-up
       call delete(d1)
       call delete(sm1)
       call delete(arr1)

    end if

#ifdef BUD_MPI
    call Comm_Split(world, 0, rank, comm1)
#endif

    ! Create the distribution
    call new(d1, comm1, N, DIST_BLOCK_LAST_ALL)
    call delete(comm1)

    ! Create all the output distributions
    call distribute(dsmA1, world, d1, dsmA2)
    call delete(d1)

    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA1)
       call flush(6)
       call MP_Barrier(world)
    end do
    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA2)
       call flush(6)
       call MP_Barrier(world)
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
    call distribute(dsmA2, world, d1, dsmA3)
    call delete(d1)

    do i = 0 , nrank - 1
       if ( i == rank ) &
            call print(dsmA3)
       call flush(6)
       call MP_Barrier(world)
    end do

    call delete(dsmA1)
    call delete(dsmA2)
    call delete(dsmA3)

    call MP_Barrier(world)

  end subroutine dist_4

end program dist
