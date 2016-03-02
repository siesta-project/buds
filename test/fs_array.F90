program test_fs_array

  use test_utils

  use mbud_iArray1D
  use mbud_iArray2D
  use mbud_iArray3D

  use mbud_FS_iArray1D
  use mbud_FS_iArray2D
  use mbud_FS_iArray3D

  type(iArray1D) :: a1, b1, c1
  type(iArray2D) :: a2, b2, c2
  type(iArray3D) :: a3, b3, c3

  type(FS_iArray1D) :: fa1, fb1, fc1
  type(FS_iArray2D) :: fa2, fb2, fc2
  type(FS_iArray3D) :: fa3, fb3, fc3

  call t_1
  write(*,*)
  call t_2
  write(*,*)
  call t_3

contains

  subroutine t_1()
    call new(a1,10)
    call new(fa1, 2)
    call push(fa1, a1)
    call print(fa1)
    b1 = a1
    call print(fa1)
    call push(fa1, b1)
    call print(fa1)
    c1 = b1
    call push(fa1, c1)
    call print(fa1)
    call delete(c1)
    call print(fa1)
    call delete(b1)
    call delete(a1)
    call print(fa1)
    fb1 = fa1
    call print(fb1)
    call delete(fb1)
    call delete(fa1)
  end subroutine t_1

  subroutine t_2()
    call new(a2,10, 10)
    call new(fa2, 2)
    call push(fa2, a2)
    call print(fa2)
    b2 = a2
    call print(fa2)
    call push(fa2, b2)
    call print(fa2)
    c2 = b2
    call push(fa2, c2)
    call print(fa2)
    call delete(c2)
    call print(fa2)
    call delete(b2)
    call delete(a2)
    call print(fa2)
    fb2 = fa2
    call print(fb2)
    call delete(fb2)
    call delete(fa2)
  end subroutine t_2

  subroutine t_3()
    call new(a3,10,10,10)
    call new(fa3, 2)
    call push(fa3, a3)
    call print(fa3)
    b3 = a3
    call print(fa3)
    call push(fa3, b3)
    call print(fa3)
    c3 = b3
    call push(fa3, c3)
    call print(fa3)
    call delete(c3)
    call print(fa3)
    call delete(b3)
    call delete(a3)
    call print(fa3)
    fb3 = fa3
    call print(fb3)
    call delete(fb3)
    call delete(fa3)
  end subroutine t_3

end program test_fs_array

