!> @param[in] o1 first dimension of output array
!! @param[in] o2 second dimension of output array
!! @param[in] o3 third dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,1Dto3D)(o1,o2,o3,out,i1,in)
  integer(ii_), intent(in) :: o1, o2, o3, i1
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR, intent(in) :: in(i1)
#endif
  call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,o1,i1/(o2*o3),in(1))
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[in] o2 second dimension of output array
!! @param[in] o3 third dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] in array to be copied
recursive subroutine BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,i1,i2,in)
  integer(ii_), intent(in) :: o1, o2, o3, i1, i2
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2)
#endif
  if ( o1 == i1 ) then
    ! Cycle call to assert correct memory alignment
    call BUD_TRANSFER_FUNC(copy,3Dto3D)(o1,o2,o3,out,i1,o2,i2/o2,in(1,1))
  else
    call BUD_TRANSFER_FUNC(copy,3Dto3D)(o1,o2,o3,out,o1,o2,(i1*i2)/(o1*o2),in(1,1))
  end if
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[in] o2 second dimension of output array
!! @param[in] o3 third dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] in array to be copied
recursive subroutine BUD_TRANSFER_FUNC(copy,3Dto3D)(o1,o2,o3,out,i1,i2,i3,in)
  integer(ii_), intent(in) :: o1, o2, o3, i1, i2, i3
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3)
#endif
  integer(ii_) :: j1, j2, j3
  if ( o1 == i1 .and. o2 == i2 ) then
    do j3 = 1 , min(o3,i3)
      do j2 = 1 , o2
        do j1 = 1 , o1
          out(j1,j2,j3) = in(j1,j2,j3)
        end do
      end do
    end do
    return
  end if
  ! Cycle down in dimension to retrive a correct dimension
  call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,o1,(i1*i2*i3)/o1,in(1,1,1))
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[in] o2 second dimension of output array
!! @param[in] o3 third dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,4Dto3D)(o1,o2,o3,out,i1,i2,i3,i4,in)
  integer(ii_), intent(in) :: o1, o2, o3, i1, i2, i3, i4
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3,i4)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3,i4)
#endif
  if ( o1 == i1 .and. o2 == i2 ) then
    call BUD_TRANSFER_FUNC(copy,3Dto3D)(o1,o2,o3,out,i1,i2,i3*i4,in(1,1,1,1))
  else if ( o1 == i1 ) then
    call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,i1,i2*i3*i4,in(1,1,1,1))
  else
    call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,o1,(i1*i2*i3*i4)/o1,in(1,1,1,1))
  end if
end subroutine

!> @param[in] o1 first dimension of output array
!! @param[in] o2 second dimension of output array
!! @param[in] o3 third dimension of output array
!! @param[out] out the resulting array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] i5 fifth dimension of in array
!! @param[in] in array to be copied
subroutine BUD_TRANSFER_FUNC(copy,5Dto3D)(o1,o2,o3,out,i1,i2,i3,i4,i5,in)
  integer(ii_), intent(in) :: o1, o2, o3, i1, i2, i3, i4, i5
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: in(i1,i2,i3,i4,i5)
#else
  BUD_TYPE_VAR, intent(out) :: out(o1,o2,o3)
  BUD_TYPE_VAR, intent(in) :: in(i1,i2,i3,i4,i5)
#endif
  if ( o1 == i1 .and. o2 == i2 ) then
    call BUD_TRANSFER_FUNC(copy,3Dto3D)(o1,o2,o3,out,i1,i2,i3*i4*i5,in(1,1,1,1,1))
  else if ( o1 == i1 ) then
    call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,i1,i2*i3*i4*i5,in(1,1,1,1,1))
  else
    call BUD_TRANSFER_FUNC(copy,2Dto3D)(o1,o2,o3,out,o1,(i1*i2*i3*i4*i5)/o1,in(1,1,1,1,1))
  end if
end subroutine

#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC
#undef BUD_TYPE_VAR_P


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

