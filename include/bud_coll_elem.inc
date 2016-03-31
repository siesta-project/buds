  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"

!> @param[in] this @bud container
!! @param[inout] item contains data collection upon exit (or null)
subroutine BUD_CC3(get_elem,BUD_COLL_NUM,_)(this, item)
  BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
  type(BUD_COLL_CURRENT), intent(inout) :: item

  if ( .not. is_initd(this) ) then
    ! it is empty, so return an empty item
    call delete(item)
  else
    item = this%D% BUD_CC2(e,BUD_COLL_NUM)
  end if
  
end subroutine


#if BUD_COLL_SAME == 0

!> @param[inout] item contains equivalent collection data upon exit
!! @param[in] this collection with data
subroutine BUD_CC3(get_elem,BUD_COLL_NUM,_assign_)(item, this)
  type(BUD_COLL_CURRENT), intent(inout) :: item
  BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

  if ( .not. is_initd(this) ) then
    ! it is empty, so return an empty item
    call delete(item)
  else
    item = this%D% BUD_CC2(e,BUD_COLL_NUM)
  end if
  
end subroutine

#endif 


!> @param[inout] this @bud container
!! @param[in] item store this in the collection
subroutine BUD_CC3(set_elem,BUD_COLL_NUM,_)(this, item)
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  type(BUD_COLL_CURRENT), intent(in) :: item

  if ( .not. is_initd(this) ) return

  this%D% BUD_CC2(e,BUD_COLL_NUM) = item
  
end subroutine

!> @param[inout] this collection to retrieve data from
!! @return pointer to element in collection (or null if non-initialized)
function BUD_CC3(get_elem,BUD_COLL_NUM,p_)(this) result(p)
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  type(BUD_COLL_CURRENT), pointer :: p
  
  if ( .not. is_initd(this) ) then
    ! it is empty, so return an empty item
    nullify(p)
  else
    p => this%D% BUD_CC2(e,BUD_COLL_NUM)
  end if
  
end function 


! Prepare next input
#undef BUD_COLL_NUM
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

