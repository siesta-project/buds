program dist

  ! we use everything
  use mpi

  use bud_iDist1D

  implicit none

  integer, parameter :: N = 11
  integer, parameter :: BS = 2

  integer :: Status(MPI_STATUS_SIZE)

  ! block-cyclic
  type(iDist1D) :: bc1, bc2, bc3
  ! block
  type(iDist1D) :: b1, b2, b3

  integer :: err

  call MPI_Init(err)

  call bc_1
  call bc_2
  call bc_3

  call b_1
  call b_2

  call bc2b_1
  call bc2b_2

  call MPI_Finalize(err)
  
contains
  
  subroutine bc_1()
    print *,'bc_1 -- start'
    call new(bc1, MPI_Comm_World, BS, N, DIST_BLOCK_CYCLIC)
    call print(bc1)
    call delete(bc1)
    call print(bc1)
    print *,'bc_1 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine bc_1
  
  subroutine bc_2()
    integer :: Comm, err, i
    integer, allocatable :: ranks(:)
    print *,'bc_2 -- start'
    call new(bc1, MPI_Comm_World, BS, N, DIST_BLOCK_CYCLIC)
    call print(bc1)
    if ( get_P(bc1) == 0 ) then
       do i = 1 , size(bc1)
          print '(2(a,i0))', 'Element: ',i,' processor: ',global2P(bc1,i)
       end do
    end if
    call MPI_Comm_Split(get_comm(bc1), &
         mod(get_P(bc1),3), 0, Comm, err)
    
    call new(bc2, Comm, BS, N, DIST_BLOCK_CYCLIC)
    call MPI_Comm_Free(Comm, err)
    do i = 0 , 2
       if ( mod(get_P(bc1),3) == i ) then
          ! create a fake one
          call fake_sub(bc1, bc2)
       else
          call fake_sub(bc1, bc3)
       end if
       if ( get_P(bc2) == 0 ) then
          call print(bc2, info="Color")
          call print(bc3, info="Fake")
       end if
    end do
    call delete(bc1)
    call delete(bc2)
    call delete(bc3)
    print *,'bc_2 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine bc_2

  subroutine bc_3()
    integer :: Comm, err, i, gl, rl, p, rP, rS
    integer, allocatable :: ranks(:)
    integer, allocatable :: data1(:), data2(:)
    integer :: pP
    logical :: fake, recv
    print *,'bc_3 -- start'
    ! test for communication
    call new(bc1, MPI_Comm_World, BS, N, DIST_BLOCK_CYCLIC)
    ! allocate data
    allocate(data1(size_local(bc1)))
    do i = 1 , size_local(bc1)
       data1(i) = local2global(bc1, i)
    end do
    call MPI_Comm_Split(get_comm(bc1), &
         mod(get_P(bc1),3), 0, Comm, err)
    call new(bc2, Comm, BS, N, DIST_BLOCK_CYCLIC)
    call MPI_Comm_Free(Comm, err)

    ! distribute data
    allocate(data2(size_local(bc2)))
    
    ! Get the ranks
    do p = 0 , 2
       data2 = -1
       call delete(bc3)
       
       ! everybody should get this information
       fake = mod(get_P(bc1),3) /= p
       
       if ( .not. fake ) then
          ! this is the distribution we wish to create
          call sub1_ranks(bc1, bc2, ranks)
       else
          call sub1_ranks(bc1, bc3, ranks)
       end if
       
       ! At this point those processors which belong to
       ! `bc2` are the "receiving" nodes
       ! Those in `bc3` are a fake distribution
       ! which crosses information from `bc2` to `bc1`.

       ! loop all elements to distribute them
       do i = 1 , size(bc1)
          
          ! from the parent distribution
          ! decide the host of the data
          pP = global2P(bc1, i)

          ! get the receiving end of the data
          if ( .not. fake ) then
             rP = global2P(bc2, i)
             rS = ranks(global2P(bc2, i)+1)
          else
             rP = -1 ! ensure check
             rS = ranks(global2P(bc3, i)+1)
          end if

          ! check whether we should recieve the
          ! data
          recv = .not. fake
          if ( recv ) recv = rP == get_P(bc2)
          
          if ( recv ) then
             ! we know that we should receive data

             ! get local index
             rl = global2local(bc2, i)
             
             if ( pP == get_P(bc1) ) then
                
                ! we are also the containing node
                gl = global2local(bc1, i)
                
                ! the data is already local
                ! simple copy
                data2(rl) = data1(gl)
                
             else
                
                !print'(a7,4(tr1,i2))','recv: ',i,pP,gl,get_P(bc1)
                ! we have the data
                call MPI_Recv(data2(rl), 1, MPI_Integer, &
                     pP, i, get_comm(bc1), Status, err)
                
             end if
             
          else if ( pP == get_P(bc1) ) then
             ! we should just send the information
             
             gl = global2local(bc1, i)
             
             !print'(a7,4(tr1,i2))','send: ',i,pP,gl,rS
             ! we have the data
             call MPI_Send(data1(gl), 1, MPI_Integer, &
                  rS, i, get_comm(bc1), err)
          end if
          
          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)
          
          if ( rP == get_P(bc2) ) then
             print '(3(a,i2))', 'Element: ',i,' processor: ',ranks(rP+1), ' data: ',data2(global2local(bc2,i))
          end if
          
          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)
          
       end do
       
    end do

    call delete(bc1)
    call delete(bc2)
    call delete(bc3)

    print *,'bc_3 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine bc_3

  subroutine b_1()
    print *,'b_1 -- start'
    call new(b1, MPI_Comm_World, N, DIST_BLOCK_LAST)
    call print(b1)
    call delete(b1)
    call print(b1)
    print *,'b_1 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine b_1

  subroutine b_2()
    integer :: Comm, err
    integer, allocatable :: ranks(:)
    print *,'b_2 -- start'
    call new(b1, MPI_Comm_World, N, DIST_BLOCK_LAST)
    call print(b1)
    call MPI_Comm_Split(get_comm(b1), &
         mod(get_P(b1),3), 0, Comm, err)
    call new(b2, Comm, N, DIST_BLOCK_LAST)
    call MPI_Comm_Free(Comm, err)
    if ( mod(get_P(b1),2) == 0 ) then
       ! create a fake one
       call fake_sub(b1, b2)
       call fake_sub(b1, b3)
    else
       call fake_sub(b1, b3)
       call fake_sub(b1, b2)
    end if
    if ( get_P(b2) == 0 ) then
       call print(b2, info="Color")
       call print(b3, info="Fake")
    end if
    call delete(b1)
    call delete(b2)
    call delete(b3)
    print *,'b_2 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine b_2


  subroutine bc2b_1()
    integer :: Comm, err, i, gl, rl, p, rP, rS
    integer, allocatable :: ranks(:)
    integer, allocatable :: data1(:), data2(:)
    integer :: pP
    logical :: fake, recv
    
    print *,'bc2b_1 -- start'
    ! test for communication
    call new(bc1, MPI_Comm_World, BS, N, DIST_BLOCK_CYCLIC)
    ! allocate data
    allocate(data1(size_local(bc1)))
    do i = 1 , size_local(bc1)
       data1(i) = local2global(bc1, i)
    end do
    
    call MPI_Comm_Split(get_comm(bc1), &
         mod(get_P(bc1),3), 0, Comm, err)
    
    call new(b1, Comm, N, DIST_BLOCK_LAST)
    call MPI_Comm_Free(comm,err)
    allocate(data2(size_local(b1)))
    
    ! Get the ranks
    do p = 0 , 2
       data2 = -1
       call delete(b2)
       
       ! everybody should get this information
       fake = mod(get_P(bc1),3) /= p
       
       if ( .not. fake ) then
          ! this is the distribution we wish to create
          call sub1_ranks(bc1, b1, ranks)
       else
          call sub1_ranks(bc1, b2, ranks)
       end if
       
       ! At this point those processors which belong to
       ! `b1` are the "receiving" nodes
       ! Those in `b2` are a fake distribution
       ! which crosses information from `b1` to `bc1`.

       ! loop all elements to distribute them
       do i = 1 , size(bc1)
          
          ! from the parent distribution
          ! decide the host of the data
          pP = global2P(bc1, i)

          ! get the receiving end of the data
          if ( .not. fake ) then
             rP = global2P(b1, i)
             rS = ranks(global2P(b1, i)+1)
          else
             rP = -1
             rS = ranks(global2P(b2, i)+1)
          end if

          ! check whether we should recieve the data
          recv = .not. fake
          if ( recv ) recv = rP == get_P(b1)

          if ( recv ) then
             ! we know that we should receive data

             ! get local index
             rl = global2local(b1, i)
             
             if ( pP == get_P(bc1) ) then
                ! we are also the containing node
                gl = global2local(bc1, i)

                ! the data is already local
                ! simple copy
                data2(rl) = data1(gl)
                
             else

                !print'(a7,4(tr1,i2))','recv: ',i,pP,gl,get_P(bc1)
                ! we have the data
                call MPI_Recv(data2(rl), 1, MPI_Integer, &
                     pP, i, get_comm(bc1), Status, err)

             end if
             
          else if ( pP == get_P(bc1) ) then
             ! we should just send the information
             
             gl = global2local(bc1, i)
             
             !print'(a7,4(tr1,i2))','send: ',i,pP,gl,rS
             ! we have the data
             call MPI_Send(data1(gl), 1, MPI_Integer, &
                  rS, i, get_comm(bc1), err)
             
          end if

          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)

          if ( rP == get_P(b1) ) then
             print '(3(a,i2))', 'Element: ',i,' processor: ',ranks(rP+1), ' data: ',data2(global2local(b1,i))
          end if

          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)

       end do

    end do

    call delete(bc1)
    call delete(bc2)
    call delete(b1)
    call delete(b2)

    print *,'bc2b_1 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine bc2b_1

  subroutine bc2b_2()
    integer :: Comm, err, i, gl, rl, p, rP, rS, Ng
    integer, allocatable :: ranks(:), groups(:)
    integer, allocatable :: data1(:), data2(:)
    integer :: pP, sub_root
    logical :: in_sub, recv
    
    print *,'bc2b_2 -- start'
    ! test for communication
    call new(bc1, MPI_Comm_World, BS, N, DIST_BLOCK_CYCLIC)
    
    ! allocate data
    allocate(data1(size_local(bc1)))
    do i = 1 , size_local(bc1)
       data1(i) = local2global(bc1, i)
    end do
    
    call MPI_Comm_Split(get_comm(bc1), &
         mod(get_P(bc1),3), 0, Comm, err)
    
    call new(b1, Comm, N, DIST_BLOCK_LAST)
    call MPI_Comm_Free(comm,err)
    
    allocate(data2(size_local(b1)))

    ! figure out how many groups and each local
    ! group ID
    call subN_groups(bc1, b1, sub_root, Ng, groups)
    print *,Ng,groups

    ! loop different groups
    do p = 1 , Ng

       ! initialize
       data2 = -1
       call delete(b2)
       
       ! everybody should get this information
       in_sub = sub_root == groups(p)
          
       if ( in_sub ) then
          ! this is the distribution we wish to create
          ! a fake of on the other nodes
          call sub1_ranks(bc1, b1, ranks)
       else
          call sub1_ranks(bc1, b2, ranks)
       end if
       
       ! At this point those processors which belong to
       ! `b1` are the "receiving" nodes
       ! Those in `b2` are a fake distribution
       ! which crosses information from `b1` to `bc1`.

       ! loop all elements to distribute them
       do i = 1 , size(bc1)
          
          ! from the parent distribution
          ! decide the host of the data
          pP = global2P(bc1, i)

          ! get the receiving end of the data
          if ( in_sub ) then
             rP = global2P(b1, i)
             rS = ranks(global2P(b1, i)+1)
          else
             rP = -1
             rS = ranks(global2P(b2, i)+1)
          end if

          ! check whether we should recieve the data
          recv = in_sub
          if ( recv ) recv = rP == get_P(b1)

          if ( recv ) then
             ! we know that we should receive data

             ! get local index
             rl = global2local(b1, i)
             
             if ( pP == get_P(bc1) ) then
                ! we are also the containing node
                gl = global2local(bc1, i)

                ! the data is already local
                ! simple copy
                data2(rl) = data1(gl)
                
             else

                !print'(a7,4(tr1,i2))','recv: ',i,pP,gl,get_P(bc1)
                ! we have the data
                call MPI_Recv(data2(rl), 1, MPI_Integer, &
                     pP, i, get_comm(bc1), Status, err)

             end if
             
          else if ( pP == get_P(bc1) ) then
             ! we should just send the information
             
             gl = global2local(bc1, i)
             
             !print'(a7,4(tr1,i2))','send: ',i,pP,gl,rS
             ! we have the data
             call MPI_Send(data1(gl), 1, MPI_Integer, &
                  rS, i, get_comm(bc1), err)
             
          end if

          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)

          if ( rP == get_P(b1) ) then
             print '(3(a,i2))', 'Element: ',i,' processor: ',ranks(rP+1), ' data: ',data2(global2local(b1,i))
          end if

          call flush(6)
          call MPI_Barrier(get_comm(bc1), err)

       end do

    end do

    deallocate(ranks, groups)
    
    call delete(bc1)
    call delete(bc2)
    call delete(b1)
    call delete(b2)

    print *,'bc2b_2 -- end'
    call MPI_Barrier(MPI_Comm_World, err)
  end subroutine bc2b_2

end program dist
