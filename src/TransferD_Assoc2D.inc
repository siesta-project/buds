!> @param[in] o1 first dimension of output associated array
!! @param[in] o2 second dimension of output associated array
!! @param out the resulting associated array
!! @param[in] i1 first dimension of in array
!! @param[in] in array to be associated
recursive subroutine BUD_TRANSFER_FUNC(assoc,1Dto2D)(o1,o2,out,i1,in)
  integer(ii_), intent(in) :: o1, o2, i1
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: out (:,:)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: in(i1)
#else
  BUD_TYPE_VAR, pointer :: out (:,:)
  BUD_TYPE_VAR, intent(in), target :: in(i1)
#endif
  call BUD_TRANSFER_FUNC(assoc,2Dto2D)(o1,o2,out,o1,i1/o1,in(1))
end subroutine

!> @param[in] o1 first dimension of output associated array
!! @param[in] o2 second dimension of output associated array
!! @param out the resulting associated array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] in array to be associated
recursive subroutine BUD_TRANSFER_FUNC(assoc,2Dto2D)(o1,o2,out,i1,i2,in)
  integer(ii_), intent(in) :: o1, o2, i1, i2
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: out (:,:)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: in(i1,i2)
#else
  BUD_TYPE_VAR, pointer :: out (:,:)
  BUD_TYPE_VAR, intent(in), target :: in(i1,i2)
#endif
  if ( o1 /= i1 ) then
    ! Cycle call to assert correct memory alignment
    call BUD_TRANSFER_FUNC(assoc,1Dto2D)(o1,o2,out,i1*i2,in(1,1))
    return
  end if
  if ( o2 > i2 ) then
    nullify(out)
  else
    out => in(1:o1,1:o2)
  end if
end subroutine

!> @param[in] o1 first dimension of output associated array
!! @param[in] o2 second dimension of output associated array
!! @param out the resulting associated array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] in array to be associated
subroutine BUD_TRANSFER_FUNC(assoc,3Dto2D)(o1,o2,out,i1,i2,i3,in)
  integer(ii_), intent(in) :: o1, o2, i1, i2, i3
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: out (:,:)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: in(i1,i2,i3)
#else
  BUD_TYPE_VAR, pointer :: out (:,:)
  BUD_TYPE_VAR, intent(in), target :: in(i1,i2,i3)
#endif
  if ( o1 == i1 ) then
    ! Reduces chances of overflow
    call BUD_TRANSFER_FUNC(assoc,2Dto2D)(o1,o2,out,i1,i2*i3,in(1,1,1))
  else
    call BUD_TRANSFER_FUNC(assoc,1Dto2D)(o1,o2,out,i1*i2*i3,in(1,1,1))
  end if
end subroutine

!> @param[in] o1 first dimension of output associated array
!! @param[in] o2 second dimension of output associated array
!! @param out the resulting associated array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] in array to be associated
subroutine BUD_TRANSFER_FUNC(assoc,4Dto2D)(o1,o2,out,i1,i2,i3,i4,in)
  integer(ii_), intent(in) :: o1, o2, i1, i2, i3, i4
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: out (:,:)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: in(i1,i2,i3,i4)
#else
  BUD_TYPE_VAR, pointer :: out (:,:)
  BUD_TYPE_VAR, intent(in), target :: in(i1,i2,i3,i4)
#endif
  if ( o1 == i1 ) then
    call BUD_TRANSFER_FUNC(assoc,2Dto2D)(o1,o2,out,i1,i2*i3*i4,in(1,1,1,1))
  else
    call BUD_TRANSFER_FUNC(assoc,1Dto2D)(o1,o2,out,i1*i2*i3*i4,in(1,1,1,1))
  end if
end subroutine

!> @param[in] o1 first dimension of output associated array
!! @param[in] o2 second dimension of output associated array
!! @param out the resulting associated array
!! @param[in] i1 first dimension of in array
!! @param[in] i2 second dimension of in array
!! @param[in] i3 third dimension of in array
!! @param[in] i4 fourth dimension of in array
!! @param[in] i5 fifth dimension of in array
!! @param[in] in array to be associated
subroutine BUD_TRANSFER_FUNC(assoc,5Dto2D)(o1,o2,out,i1,i2,i3,i4,i5,in)
  integer(ii_), intent(in) :: o1, o2, i1, i2, i3, i4, i5
#ifdef BUD_TYPE_VAR_PREC
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: out (:,:)
  BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: in(i1,i2,i3,i4,i5)
#else
  BUD_TYPE_VAR, pointer :: out (:,:)
  BUD_TYPE_VAR, intent(in), target :: in(i1,i2,i3,i4,i5)
#endif
  if ( o1 == i1 ) then
    call BUD_TRANSFER_FUNC(assoc,2Dto2D)(o1,o2,out,i1,i2*i3*i4*i5,in(1,1,1,1,1))
  else
    call BUD_TRANSFER_FUNC(assoc,1Dto2D)(o1,o2,out,i1*i2*i3*i4*i5,in(1,1,1,1,1))
  end if
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

