! We need to redefine the BUD_DIM_SPEC at each inclusion
#undef BUD_DIM_SPEC
#include "bud_utils.inc"

! Create A and B definitions
#ifdef BUD_TYPE_VAR_PREC
# define _MP_COMM_A_INOUT BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(inout) :: a BUD_DIM_SPEC(:)
# define _MP_COMM_A_IN BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: a BUD_DIM_SPEC(:)
# define _MP_COMM_B_INOUT BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(inout) :: b BUD_DIM_SPEC(:)
# define _MP_COMM_VAR_PREC BUD_TYPE_VAR_PREC
#else
# define _MP_COMM_A_INOUT BUD_TYPE_VAR, intent(inout) :: a BUD_DIM_SPEC(:)
# define _MP_COMM_A_IN BUD_TYPE_VAR, intent(in) :: a BUD_DIM_SPEC(:)
# define _MP_COMM_B_INOUT BUD_TYPE_VAR, intent(inout) :: b BUD_DIM_SPEC(:)
# define _MP_COMM_VAR_PREC
#endif

! Implement the Send routines
subroutine BUD_CC5(send_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Send(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_Send(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(send_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Send(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_Send(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(bsend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_BSend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_BSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(bsend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_BSend(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_BSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(rsend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_RSend(a BUD_DIM_SPEC(1) ,size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_RSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(rsend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_RSend(a BUD_DIM_SPEC(1) ,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_RSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ssend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_SSend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_SSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ssend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_SSend(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#else
  call MPI_SSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(isend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_ISend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_ISend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(isend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_ISend(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_ISend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ibsend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IBSend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_IBSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ibsend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IBSend(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_IBSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(irsend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IRSend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_IRSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(irsend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IRSend(a BUD_DIM_SPEC(1) ,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_IRSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(issend_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_ISSend(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_ISSend(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(issend_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_ISSend(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_ISSend(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine

! Implement the Send_Init routines
subroutine BUD_CC5(send_init_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Send_Init(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_Send_Init(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(send_init_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N,dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Send_Init(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_Send_Init(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(bsend_init_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_BSend_Init(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_BSend_Init(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(bsend_init_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_BSend_Init(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_BSend_Init(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(rsend_init_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_RSend_Init(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_RSend_Init(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(rsend_init_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_RSend_Init(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_RSend_Init(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ssend_init_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_SSend_Init(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_SSend_Init(a,1,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ssend_init_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,dest,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, dest, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_SSend_Init(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#else
  call MPI_SSend_Init(a,N,BUD_TYPE_VAR_MP,dest,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine


! Implement the Recv routines
subroutine BUD_CC5(recv_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,source,tag,this,status)
  _MP_COMM_A_INOUT
  integer, intent(in) :: source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: status(MPI_STATUS_SIZE)
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Recv(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,source,tag,this%D%comm,status,this%error_)
#else
  call MPI_Recv(a,1,BUD_TYPE_VAR_MP,source,tag,this%D%comm,status,this%error_)
#endif
#endif
end subroutine
! Implement the Recv routines
subroutine BUD_CC5(recv_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,source,tag,this,status)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: status(MPI_STATUS_SIZE)
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Recv(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,status,this%error_)
#else
  call MPI_Recv(a,N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,status,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(irecv_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,source,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IRecv(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#else
  call MPI_IRecv(a,1,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(irecv_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,source,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IRecv(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#else
  call MPI_IRecv(a,N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine

! Implement the Recv_init routines
subroutine BUD_CC5(recv_init_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,source,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Recv_Init(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#else
  call MPI_Recv_Init(a,1,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(recv_init_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,source,tag,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, source, tag
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Recv_Init(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#else
  call MPI_Recv_Init(a,N,BUD_TYPE_VAR_MP,source,tag,this%D%comm,request,this%error_)
#endif
#endif
end subroutine

! Implement the Bcast routines
subroutine BUD_CC5(bcast_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,root,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Bcast(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,root,this%D%comm,this%error_)
#else
  call MPI_Bcast(a,1,BUD_TYPE_VAR_MP,root,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(bcast_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,root,this)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Bcast(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,root,this%D%comm,this%error_)
#else
  call MPI_Bcast(a,N,BUD_TYPE_VAR_MP,root,this%D%comm,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ibcast_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,root,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IBcast(a BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,root,this%D%comm,request,this%error_)
#else
  call MPI_IBcast(a,1,BUD_TYPE_VAR_MP,root,this%D%comm,request,this%error_)
#endif
#endif
end subroutine
subroutine BUD_CC5(ibcast_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,N,root,this,request)
  _MP_COMM_A_INOUT
  integer, intent(in) :: N, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IBcast(a BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,root,this%D%comm,request,this%error_)
#else
  call MPI_IBcast(a,N,BUD_TYPE_VAR_MP,root,this%D%comm,request,this%error_)
#endif
#endif
end subroutine

! Implement the Gather routines (currently different types are not available)
subroutine BUD_CC5(gather_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Gather(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,this%error_)
#else
  call MPI_Gather(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(igather_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IGather(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,request,this%error_)
#else
  call MPI_IGather(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine

! Implement the allgather routines (currently different types are not available)
subroutine BUD_CC5(allgather_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_AllGather(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    this%D%comm,this%error_)
#else
  call MPI_AllGather(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(iallgather_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IAllGather(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    this%D%comm,request,this%error_)
#else
  call MPI_IAllGather(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine

! Implement the Scatter routines (currently different types are not available)
subroutine BUD_CC5(scatter_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Scatter(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,this%error_)
#else
  call MPI_Scatter(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(iscatter_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,scount,b,rcount,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: scount, rcount, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IScatter(a BUD_DIM_SPEC(1),scount,BUD_TYPE_VAR_MP, &
    b BUD_DIM_SPEC(1),rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,request,this%error_)
#else
  call MPI_IScatter(a,scount,BUD_TYPE_VAR_MP, &
    b,rcount,BUD_TYPE_VAR_MP, &
    root, this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine

! Implement the reduce routines (currently MPI_IN_PLACE does not work)
subroutine BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,op,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: op, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Reduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,op,root,this%D%comm,this%error_)
#else
  call MPI_Reduce(a,b,1,BUD_TYPE_VAR_MP,op,root,this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
! Implement the reduce routines (currently MPI_IN_PLACE does not work)
subroutine BUD_CC5(reduce_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,N,op,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: N, op, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_Reduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,op,root,this%D%comm,this%error_)
#else
  call MPI_Reduce(a,b,N,BUD_TYPE_VAR_MP,op,root,this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
! Implement the ireduce routines (currently MPI_IN_PLACE does not work)
subroutine BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,op,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: op, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,op,root,this%D%comm,request,this%error_)
#else
  call MPI_IReduce(a,b,1,BUD_TYPE_VAR_MP,op,root,this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine
! Implement the ireduce routines (currently MPI_IN_PLACE does not work)
subroutine BUD_CC5(ireduce_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,N,op,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: N, op, root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  call MPI_IReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,op,root,this%D%comm,request,this%error_)
#else
  call MPI_IReduce(a,b,N,BUD_TYPE_VAR_MP,op,root,this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine

! Implement shorthands for reduce routines (currently MPI_IN_PLACE does not work)
! Note that these only work for integer, real, complex
#ifndef BUD_IS_LOGICAL
# ifndef BUD_IS_COMPLEX

  ! this means we are either dealing with integer, real, complex
! MAX (no complex)
subroutine BUD_CC5(reduce_max_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MAX,root,this)
end subroutine
subroutine BUD_CC5(ireduce_max_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MAX,root,this,request)
end subroutine
! MIN (no complex)
subroutine BUD_CC5(reduce_min_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MIN,root,this)
end subroutine
subroutine BUD_CC5(ireduce_min_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MIN,root,this,request)
end subroutine
# endif

! PROD
subroutine BUD_CC5(reduce_prod_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_PROD,root,this)
end subroutine
subroutine BUD_CC5(ireduce_prod_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_PROD,root,this,request)
end subroutine
! SUM
subroutine BUD_CC5(reduce_sum_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_SUM,root,this)
end subroutine
subroutine BUD_CC5(ireduce_sum_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_SUM,root,this,request)
end subroutine
#endif

#ifdef BUD_IS_LOGICAL
  ! this means we are dealing with logical
! LAND
subroutine BUD_CC5(reduce_land_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LAND,root,this)
end subroutine
subroutine BUD_CC5(ireduce_land_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LAND,root,this,request)
end subroutine
! LOR
subroutine BUD_CC5(reduce_lor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LOR,root,this)
end subroutine
subroutine BUD_CC5(ireduce_lor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LOR,root,this,request)
end subroutine
! LXOR
subroutine BUD_CC5(reduce_lxor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(reduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LXOR,root,this)
end subroutine
subroutine BUD_CC5(ireduce_lxor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,root,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: root
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(ireduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LXOR,root,this,request)
end subroutine
#endif

! Implement the AllReduce routines (currently MPI_IN_PLACE does not work)
subroutine BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,op,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: op
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  ! we only reduce the size of the a matrix
  call MPI_AllReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,op,this%D%comm,this%error_)
#else
  call MPI_AllReduce(a,b,1,BUD_TYPE_VAR_MP,op,this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(allreduce_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,N,op,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: N, op
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_MPI
#if BUD_DIM > 0
  ! we only reduce the size of the a matrix
  call MPI_AllReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,op,this%D%comm,this%error_)
#else
  call MPI_AllReduce(a,b,N,BUD_TYPE_VAR_MP,op,this%D%comm,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,op,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: op
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  ! we only reduce the size of the a matrix
  call MPI_IAllReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),size(A),BUD_TYPE_VAR_MP,op,this%D%comm,request,this%error_)
#else
  call MPI_IAllReduce(a,b,1,BUD_TYPE_VAR_MP,op,this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine
subroutine BUD_CC5(iallreduce_N_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,N,op,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  integer, intent(in) :: N, op
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
#ifdef BUD_MPI
#if BUD_DIM > 0
  ! we only reduce the size of the a matrix
  call MPI_IAllReduce(a BUD_DIM_SPEC(1),b BUD_DIM_SPEC(1),N,BUD_TYPE_VAR_MP,op,this%D%comm,request,this%error_)
#else
  call MPI_IAllReduce(a,b,N,BUD_TYPE_VAR_MP,op,this%D%comm,request,this%error_)
#endif
#else
  b = a
#endif
end subroutine

! Implement shorthands for allreduce routines (currently MPI_IN_PLACE does not work)
! Note that these only work for integer, real, complex
#ifndef BUD_IS_LOGICAL
# ifndef BUD_IS_COMPLEX
  ! this means we are either dealing with integer, real, complex
! MAX (no complex)
subroutine BUD_CC5(allreduce_max_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MAX,this)
end subroutine
subroutine BUD_CC5(iallreduce_max_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MAX,this,request)
end subroutine
! MIN (no complex)
subroutine BUD_CC5(allreduce_min_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MIN,this)
end subroutine
subroutine BUD_CC5(iallreduce_min_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_MIN,this,request)
end subroutine
# endif

! PROD
subroutine BUD_CC5(allreduce_prod_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_PROD,this)
end subroutine
subroutine BUD_CC5(iallreduce_prod_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_PROD,this,request)
end subroutine
! SUM
subroutine BUD_CC5(allreduce_sum_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_SUM,this)
end subroutine
subroutine BUD_CC5(iallreduce_sum_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_SUM,this,request)
end subroutine
#endif

#ifdef BUD_IS_LOGICAL
  ! this means we are dealing with logical
! LAND
subroutine BUD_CC5(allreduce_land_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LAND,this)
end subroutine
subroutine BUD_CC5(iallreduce_land_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LAND,this,request)
end subroutine
! LOR
subroutine BUD_CC5(allreduce_lor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LOR,this)
end subroutine
subroutine BUD_CC5(iallreduce_lor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LOR,this,request)
end subroutine
! LXOR
subroutine BUD_CC5(allreduce_lxor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  call BUD_CC5(allreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LXOR,this)
end subroutine
subroutine BUD_CC5(iallreduce_lxor_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,this,request)
  _MP_COMM_A_IN
  _MP_COMM_B_INOUT
  BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
  integer, intent(inout) :: request
  call BUD_CC5(iallreduce_,BUD_TYPE_VAR,_,BUD_DIM,_MP_COMM_VAR_PREC)(a,b,MPI_LXOR,this,request)
end subroutine
#endif



#if BUD_DIM > 3
! This makes the inclusion section much cleaner.
! as we do not need to redefine them all the time
# undef BUD_TYPE_VAR
# ifdef BUD_TYPE_VAR_PREC
#  undef BUD_TYPE_VAR_PREC
# endif
# undef BUD_TYPE_VAR_MP
# undef BUD_IS_LOGICAL
# undef BUD_IS_INTEGER
# undef BUD_IS_REAL
# undef BUD_IS_COMPLEX
#endif

#undef BUD_DIM
#undef _MP_COMM_A_INOUT
#undef _MP_COMM_A_IN
#undef _MP_COMM_B_INOUT
#undef _MP_COMM_VAR_PREC

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
