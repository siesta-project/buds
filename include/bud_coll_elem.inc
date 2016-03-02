  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"

!> Retrieve BUD_COLL_LETTER @bud in BUD_TYPE_NAME
!!
!! Query the data contained in the BUD_TYPE_NAME
!! and return the data in `item`.
!!
!! If `this` is not initialized, then `item` will be deleted
!! and an un-initialized @bud will be returned.
!!
!! @param[inout] this @bud container
!! @param[inout] item contains data collection upon exit (or null)
!!
!! @author Nick R. Papior (2016, February)
subroutine BUD_CC3(get_elem,BUD_COLL_LETTER,_)(this, item)
  type(BUD_TYPE_NAME), intent(inout) :: this
  type(BUD_COLL_CURRENT), intent(inout) :: item

  if ( .not. is_initd(this) ) then
    ! it is empty, so return an empty item
    call delete(item)
  else
    item = this%D% BUD_COLL_LETTER
  end if
  
end subroutine

!> Retrieve BUD_COLL_LETTER @bud in BUD_TYPE_NAME
!!
!! Query the data contained in the BUD_TYPE_NAME
!! and return the data in `item` as a pointer
!!
!! If `this` is not initialized, then `item` will be deleted
!! and an un-initialized type will be returned.
!!
!! @param[inout] this @bud container
!! @param p pointer to element in collection (or null if non-initialized)
!!
!! @author Nick R. Papior (2016, February)
subroutine BUD_CC3(get_elemp,BUD_COLL_LETTER,_)(this, p)
  type(BUD_TYPE_NAME), intent(inout) :: this
  type(BUD_COLL_CURRENT), pointer :: p

  if ( .not. is_initd(this) ) then
    ! it is empty, so return an empty item
    nullify(p)
  else
    p => this%D% BUD_COLL_LETTER
  end if

end subroutine

!> Retrieve BUD_COLL_LETTER @bud in BUD_TYPE_NAME (function)
!!
!! @iSee fcollp## BUD_COLL_LETTER ## _
!!
!! @param[inout] this data container
!! @return pointer to element in collection (or null if non-initialized)
!!
!! @author Nick R. Papior (2016, February)
function BUD_CC3(elemp,BUD_COLL_LETTER,_)(this) result(p)
  type(BUD_TYPE_NAME), intent(inout) :: this
  type(BUD_COLL_CURRENT), pointer :: p
  call BUD_CC3(get_elemp,BUD_COLL_LETTER,_)(this, p)
end function 


! Prepare next input
#undef BUD_COLL_LETTER
#undef BUD_COLL_CURRENT


! project-bud -- local file settings
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

