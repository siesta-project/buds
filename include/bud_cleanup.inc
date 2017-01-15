
  ! Here we clean up a mixture of preprocessor
  ! statements that are defined in the
  ! include/[inc] files
  ! Excluding the _utils.inc file!

  ! These flags are generic
#undef BUD_MOD_NAME
#undef BUD_MOD_NAME_STR
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR

  ! bud_coll.inc
#undef BUD_COLL_1
#undef BUD_COLL_2
#undef BUD_COLL_3
#undef BUD_COLL_4
#undef BUD_COLL_5
#undef BUD_COLL_6
#undef BUD_COLL_ALL_SAME

  ! bud_llist.inc
#undef BUD_LIST_DOUBLY
#undef BUD_LINKED_LIST_NAME

  ! bud_item
#undef BUD_ITEM_TYPE

  ! These flags control the input sequence
#undef BUD_COMMON_DECLARATIONS_H

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

