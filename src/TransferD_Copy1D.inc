!> @param[in] o1 first dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,1Dto1D)(o1,out,i1,in)
  integer(ii_), intent(in) :: o1, i1
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1)
  BUD_TYPE_VAR, intent(in) :: in(i1)
#endif
  integer(ii_) :: j1
  do j1 = 1 , min(o1, i1)
    out(j1) = in(j1)
  end do
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,2Dto1D)(o1,out,i1,i2,in)
  integer(ii_), intent(in) :: o1, i1, i2
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2)
#endif
  call BUD_TRANSFER_FUNC(copy,1Dto1D)(o1,out,i1*i2,in(1,1))
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,3Dto1D)(o1,out,i1,i2,i3,in)
  integer(ii_), intent(in) :: o1, i1, i2, i3
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3)
#endif
  call BUD_TRANSFER_FUNC(copy,1Dto1D)(o1,out,i1*i2*i3,in(1,1,1))
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,4Dto1D)(o1,out,i1,i2,i3,i4,in)
  integer(ii_), intent(in) :: o1, i1, i2, i3, i4
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3,i4)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3,i4)
#endif
  call BUD_TRANSFER_FUNC(copy,1Dto1D)(o1,out,i1*i2*i3*i4,in(1,1,1,1))
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] i5 fifth dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,5Dto1D)(o1,out,i1,i2,i3,i4,i5,in)
  integer(ii_), intent(in) :: o1, i1, i2, i3, i4, i5
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3,i4,i5)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3,i4,i5)
#endif
  call BUD_TRANSFER_FUNC(copy,1Dto1D)(o1,out,i1*i2*i3*i4*i5,in(1,1,1,1,1))
end subroutine

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

