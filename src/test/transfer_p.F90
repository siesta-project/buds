program test_transfer_p

  use test_utils
  use bud_Transfer

  implicit none

  call start_test('DIM_TRANSFER_P')
  call t_1
  write(*,*)
  call end_test('DIM_TRANSFER_P')

contains

  subroutine t_1()
    real :: a(10,10,10)
    real, pointer :: b(:) => null()
    a = 100
    call dim_transfer_p(10,b,10,10,10,a)
    write(*,*) size(b), 10, b(10)
    call dim_transfer_p(10,b,10,100,a(:,:,1))
    write(*,*) size(b), 10, b(10)
    call dim_transfer_p(10,b,1000,a(:,1,1))
    write(*,*) size(b), 10, b(10)
    call dim_transfer_p(100,b,10,10,10,a)
    write(*,*) size(b), 100, b(100)
    call dim_transfer_p(100,b,10,100,a(:,:,1))
    write(*,*) size(b), 100, b(100)
    call dim_transfer_p(100,b,1000,a(:,1,1))
    write(*,*) size(b), 100, b(100)
  end subroutine t_1

end program test_transfer_p

