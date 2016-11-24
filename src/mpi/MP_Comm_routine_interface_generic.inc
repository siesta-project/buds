#define __BUD_MP_COMM_NAME BUD_CC2(BUD_MP_COMM_NAME,_)
  !> @iSee BUD_MP_COMM_NAME
  generic, public :: BUD_MP_COMM_NAME => &
#ifdef BUD_IS_INTEGER
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,0,ii_), BUD_CC5(__BUD_MP_COMM_NAME,integer,_,1,ii_), &
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,2,ii_), BUD_CC5(__BUD_MP_COMM_NAME,integer,_,3,ii_), &
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,4,ii_), &
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,0,il_), BUD_CC5(__BUD_MP_COMM_NAME,integer,_,1,il_), &
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,2,il_), BUD_CC5(__BUD_MP_COMM_NAME,integer,_,3,il_), &
# if defined(BUD_IS_REAL) || defined(BUD_IS_COMPLEX) || defined(BUD_IS_LOGICAL)
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,4,il_), &
# else
    BUD_CC5(__BUD_MP_COMM_NAME,integer,_,4,il_)
# endif
#endif
#ifdef BUD_IS_REAL
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,0,rr_), BUD_CC5(__BUD_MP_COMM_NAME,real,_,1,rr_), &
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,2,rr_), BUD_CC5(__BUD_MP_COMM_NAME,real,_,3,rr_), &
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,4,rr_), &
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,0,rd_), BUD_CC5(__BUD_MP_COMM_NAME,real,_,1,rd_), &
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,2,rd_), BUD_CC5(__BUD_MP_COMM_NAME,real,_,3,rd_), &
# if defined(BUD_IS_COMPLEX) || defined(BUD_IS_LOGICAL)
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,4,rd_), &
# else
    BUD_CC5(__BUD_MP_COMM_NAME,real,_,4,rd_)
# endif
#endif
#ifdef BUD_IS_COMPLEX
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,0,rr_), BUD_CC5(__BUD_MP_COMM_NAME,complex,_,1,rr_), &
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,2,rr_), BUD_CC5(__BUD_MP_COMM_NAME,complex,_,3,rr_), &
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,4,rr_), &
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,0,rd_), BUD_CC5(__BUD_MP_COMM_NAME,complex,_,1,rd_), &
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,2,rd_), BUD_CC5(__BUD_MP_COMM_NAME,complex,_,3,rd_), &
# if defined(BUD_IS_LOGICAL)
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,4,rd_), &
# else
      BUD_CC5(__BUD_MP_COMM_NAME,complex,_,4,rd_)
# endif
#endif
#ifdef BUD_IS_LOGICAL
      BUD_CC4(__BUD_MP_COMM_NAME,logical,_,0), BUD_CC4(__BUD_MP_COMM_NAME,logical,_,1), &
      BUD_CC4(__BUD_MP_COMM_NAME,logical,_,2), BUD_CC4(__BUD_MP_COMM_NAME,logical,_,3), &
      BUD_CC4(__BUD_MP_COMM_NAME,logical,_,4)
#endif

#undef BUD_MP_COMM_NAME
#undef __BUD_MP_COMM_NAME

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
