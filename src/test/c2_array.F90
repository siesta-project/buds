program test_c2_array

  use test_utils

  use bud_iArray1D
  use bud_iArray2D
  use bud_iArray3D

  use bud_C2_iArray1D
  use bud_C2_iArray2D
  use bud_C2_iArray3D

  type(iArray1D) :: a1, b1, c1
  type(iArray2D) :: a2, b2, c2
  type(iArray3D) :: a3, b3, c3

  type(C2_iArray1D) :: ca1, cb1, cc1
  type(C2_iArray2D) :: ca2, cb2, cc2
  type(C2_iArray3D) :: ca3, cb3, cc3

  call start_test('C2_ARRAY-1D')
  call t_1
  call end_test('C2_ARRAY-1D')
  call start_test('C2_ARRAY-2D')
  call t_2
  call end_test('C2_ARRAY-2D')
  call start_test('C2_ARRAY-3D')
  call t_3
  call end_test('C2_ARRAY-3D')

contains

  subroutine t_1()
    call new(a1,10)
    b1 = a1
    call new(ca1, a1, b1)
    call print(ca1)
    c1 = a1
    call print(ca1)
    call get_elem1(ca1,c1)
    call print(ca1)
    cb1 = ca1
    cc1 = ca1
    call delete(a1)
    call delete(b1)
    call delete(c1)
    call print(ca1)
    call delete(ca1)
    call delete(cb1)
    call delete(cc1)
    call print(a1)
    call print(ca1)
  end subroutine t_1

  subroutine t_2()
    call new(a2,10, 10)
    b2 = a2
    call new(ca2, a2, b2)
    call print(ca2)
    c2 = a2
    call print(ca2)
    call get_elem2(ca2,c2)
    call print(ca2)
    cb2 = ca2
    cc2 = ca2
    call delete(a2)
    call delete(b2)
    call delete(c2)
    call print(ca2)
    call delete(ca2)
    call delete(cb2)
    call delete(cc2)
    call print(a2)
    call print(ca2)
  end subroutine t_2

  subroutine t_3()
    call new(a3,10, 10,10)
    b3 = a3
    call new(ca3, a3, b3)
    call print(ca3)
    c3 = a3
    call print(ca3)
    call get_elem2(ca3,c3)
    call print(ca3)
    cb3 = ca3
    cc3 = ca3
    call delete(a3)
    call delete(b3)
    call delete(c3)
    call print(ca3)
    call delete(ca3)
    call delete(cb3)
    call delete(cc3)
    call print(a3)
    call print(ca3)
  end subroutine t_3

end program test_c2_array

