  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_MOD_NAME of the module
  !  - BUD_TYPE_NAME of the type
  !  - BUD_TYPE_NAME_, internal data pointer of the type
  !  - BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !  - BUD_TYPE_VAR, the variable type contained in the array
  !  - BUD_TYPE_VAR_PREC, the precision specifier for the array

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"


  ! Determine the variable size
#if BUD_DIM > 5
#  error "bud_Array: Does not handle array sizes larger than 5"
#endif


  !> Array data type
  !!
  !! This data type contains the reference counted
  !! array.
  type BUD_TYPE_NAME

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()


#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    procedure, private :: new_dim_ii_
    procedure, private :: new_dim_il_
    procedure, private :: new_shape_ii_
    procedure, private :: new_shape_il_

    !> See [[new]]
    generic, public :: new => &
      new_dim_ii_, new_dim_il_, &
      new_shape_ii_, new_shape_il_

    !> See [[associate]]
    procedure, public :: associate => associate_

    !> See [[array_p]]
    procedure, public :: array_p => array_p_

    !> See [[size]]
    procedure, public :: size => size_

#endif
  end type BUD_TYPE_NAME


  !> bud container for BUD_TYPE_NAME
  !!
  !! Contains an array of BUD_DIM dimensions.
  !!
  type BUD_TYPE_NAME_

    !> Array stored in the bud
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: array BUD_DIM_SPEC(:) => null()
#else
    BUD_TYPE_VAR, pointer :: array BUD_DIM_SPEC(:) => null()
#endif

    !> Whether this is a pointer to an outside array (`.false.`) or this is the container
    logical :: container = .true.

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> Create a new array object either by size or by copy
  !!
  !! Initialization routine for allocating an array.
  !!
  !! The dimensions of the contained array may be
  !! explicitly passed, or an array may be passed
  !! which will be copied to the contained array.
  !!
  !! @note
  !! If the array is created from dimensions, the initial
  !! value of the array may be arbitrary (compiler dependent).
  interface new
    module procedure new_dim_ii_, new_dim_il_
    module procedure new_shape_ii_, new_shape_il_
  end interface
  public :: new


  !> Associate an external array to the internals of this method
  !!
  !! Instead of allocating a new array it may be advantageous to
  !! use the pointer assignment to set the contained data to
  !! something from outside.
  !! This is handy when for instance creating sub-arrays
  !!
  !! \code{.f90}
  !! ... :: array(2,4)
  !! call associate(this, array(:,1))
  !! \endcode
  interface associate
    module procedure associate_
  end interface
  public :: associate


  !> Retrieve pointer to array in the bud
  !!
  !! The array-pointer will _always_ be contiguous.
  !!
  !! @note
  !! The internal array is `allocatable` so you may not deallocate the array-pointer.
  interface array_p
    module procedure array_p_
  end interface
  public :: array_p


  !> Query dimension of array (or total size)
  !!
  !! Returns the size of the array contained.
  !!
  !! @note
  !! There is no checking of the dimension queried.
  !! @endnote
  !!
  !! This is equivalent to:
  !! \code{.f90}
  !! a => array_p(this)
  !! ... = size(a [,dim=])
  !! \endcode
  interface size
    module procedure size_
  end interface
  public :: size

  !> Query dimensions of array
  interface dimensions
    module procedure dimensions_
  end interface
  public :: dimensions


  !> Set all values in the array to a specific value.
  !!
  !! This is preferred over a routine call as it
  !! is not ambiguous.
  interface assignment(=)
    module procedure assign_value_
  end interface
  private :: assign_value_


  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "bud_common.inc"


  !> Internal routine for cleaning up the data container.
  !!
  !! @note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  !! @endnote
  subroutine delete_(this)

    !> This bud which is to be deleted
    type(BUD_TYPE_NAME), intent(inout) :: this

    integer :: stat

    ! Currently we do not allow external memory
    ! tracking.
    stat = 0
    if ( associated(this%D%array) ) then
      if ( this%D%container ) then
        deallocate(this%D%array, stat=stat)
      end if
    end if
    nullify(this%D%array)

    this%D%container = .true.
    call set_error(this, stat)

  end subroutine delete_


  subroutine copy_(from, to)
    !> Contains the bud which is to be copied into `to`
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    !> Upon exit, this is the copy of `from` by copying the data-content
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    integer :: i
    integer(il_) :: ashape( BUD_DIM )

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: arr BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer :: arr BUD_DIM_SPEC(:)
#endif

    call delete(to)
    if ( .not. is_initd(from) ) return

    do i = 1 , BUD_DIM
      ashape(i) = size(from, i)
    end do
    ! Create the output data
    call new(to, ashape)
    arr => array_p(to)
    arr BUD_DIM_SPEC(:) = from%D%array BUD_DIM_SPEC(:)

    call common_copy_(from, to)

  end subroutine copy_


  subroutine new_dim_ii_(this, &
#if BUD_DIM == 1
    d1, &
#elif BUD_DIM == 2
    d1, d2, &
#elif BUD_DIM == 3
    d1, d2, d3, &
#elif BUD_DIM == 4
    d1, d2, d3, d4, &
#elif BUD_DIM == 5
    d1, d2, d3, d4, d5, &
#endif
    val)
    !> Creation of a new Array-bud with given dimensions.
    !!
    !! This bud will always be deallocated.
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    !> The size of the first dimension.
    integer(ii_), intent(in) :: d1
#if BUD_DIM > 1
    !> The size of the second dimension.
    integer(ii_), intent(in) :: d2
#endif
#if BUD_DIM > 2
    !> The size of the third dimension.
    integer(ii_), intent(in) :: d3
#endif
#if BUD_DIM > 3
    !> The size of the fourth dimension.
    integer(ii_), intent(in) :: d4
#endif
#if BUD_DIM > 4
    !> The size of the fifth dimension.
    integer(ii_), intent(in) :: d5
#endif

    !> Specify the default value of the allocated array.
    !!
    !! By default there will be no initialization.
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: val
#else
    BUD_TYPE_VAR, intent(in), optional :: val
#endif
    integer(il_) :: ashape( BUD_DIM )

    ashape(1) = d1
#if BUD_DIM > 1
    ashape(2) = d2
#endif
#if BUD_DIM > 2
    ashape(3) = d3
#endif
#if BUD_DIM > 3
    ashape(4) = d4
#endif
#if BUD_DIM > 4
    ashape(5) = d5
#endif

    call new(this, ashape, val=val)

  end subroutine new_dim_ii_

  subroutine new_dim_il_(this, &
#if BUD_DIM == 1
    d1, &
#elif BUD_DIM == 2
    d1, d2, &
#elif BUD_DIM == 3
    d1, d2, d3, &
#elif BUD_DIM == 4
    d1, d2, d3, d4, &
#elif BUD_DIM == 5
    d1, d2, d3, d4, d5, &
#endif
    val)
    !> Creation of a new Array-bud with given dimensions.
    !!
    !! This bud will always be deallocated.
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    !> The size of the first dimension.
    integer(il_), intent(in) :: d1
#if BUD_DIM > 1
    !> The size of the second dimension.
    integer(il_), intent(in) :: d2
#endif
#if BUD_DIM > 2
    !> The size of the third dimension.
    integer(il_), intent(in) :: d3
#endif
#if BUD_DIM > 3
    !> The size of the fourth dimension.
    integer(il_), intent(in) :: d4
#endif
#if BUD_DIM > 4
    !> The size of the fifth dimension.
    integer(il_), intent(in) :: d5
#endif

    !> Specify the default value of the allocated array.
    !!
    !! By default there will be no initialization.
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: val
#else
    BUD_TYPE_VAR, intent(in), optional :: val
#endif

    ! Make sure the container is initialized and ready for
    ! data-creation.

    call initialize(this)

    ! Allocate the value array
#if BUD_DIM == 1
    allocate(this%D%array(d1))
#elif BUD_DIM == 2
    allocate(this%D%array(d1,d2))
#elif BUD_DIM == 3
    allocate(this%D%array(d1,d2,d3))
#elif BUD_DIM == 4
    allocate(this%D%array(d1,d2,d3,d4))
#elif BUD_DIM == 5
    allocate(this%D%array(d1,d2,d3,d4,d5))
#endif

    this%D%container = .true.

    if ( present(val) ) this = val

  end subroutine new_dim_il_

  subroutine new_shape_ii_(this, shape, val)
    !> Creation of a new Array-bud with given dimensions.
    !!
    !! This bud will always be deallocated.
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    !> An array specifying the shape of the array
    integer(ii_), intent(in) :: shape(:)

    !> Specify the default value of the allocated array.
    !!
    !! By default there will be no initialization.
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: val
#else
    BUD_TYPE_VAR, intent(in), optional :: val
#endif

    integer :: i
    integer(il_) :: as( BUD_DIM )

    ! Make sure the container is initialized and ready for
    ! data-creation.

    as(:) = 1
    do i = 1 , min( BUD_DIM , size(shape) )
      as(i) = shape(i)
    end do

    call new(this, as, val=val)

  end subroutine new_shape_ii_

  subroutine new_shape_il_(this, shape, val)
    !> Creation of a new Array-bud with given dimensions.
    !!
    !! This bud will always be deallocated.
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    !> An array specifying the shape of the array
    integer(il_), intent(in) :: shape(:)

    !> Specify the default value of the allocated array.
    !!
    !! By default there will be no initialization.
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: val
#else
    BUD_TYPE_VAR, intent(in), optional :: val
#endif

    integer :: i
    integer(il_) :: as( BUD_DIM )

    ! Make sure the container is initialized and ready for
    ! data-creation.

    as(:) = 1
    do i = 1 , min( BUD_DIM , size(shape) )
      as(i) = shape(i)
    end do

    ! Allocate the value array
#if BUD_DIM == 1
    call new(this, as(1), val=val)
#elif BUD_DIM == 2
    call new(this, as(1), as(2), val=val)
#elif BUD_DIM == 3
    call new(this, as(1), as(2), as(3), val=val)
#elif BUD_DIM == 4
    call new(this, as(1), as(2), as(3), as(4), val=val)
#elif BUD_DIM == 5
    call new(this, as(1), as(2), as(3), as(4), as(5), val=val)
#endif

  end subroutine new_shape_il_

  subroutine associate_(this, array)
    !> Creation of a new Array-bud by association
    !!
    !! Instead of forcing the allocation of the array, one may
    !! use an external array as container for the array.
    !! If this is performed it is up to the programmer to
    !! assert that the external array gets deleted _after_
    !! this bud has been deleted.
    !! Else this bud will point to non-allocated memory.
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    !> The external array which is to be associated with this array bud
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), target :: array BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, intent(in), target :: array BUD_DIM_SPEC(:)
#endif

    ! Make sure the container is initialized and ready for
    ! data-creation.
    call initialize(this)
    this%D%array => array BUD_DIM_SPEC(:)
    this%D%container = .false.

  end subroutine associate_

  function array_p_(this) result(p)
    !> The array bud where we should retrieve a data pointer from
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    !> The data-pointer that points to this array bud.
    !!
    !! If `this` has not been initialized the resulting pointer
    !! will be a `null()` pointer.
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    if ( is_initd(this) ) then
      p => this%D%array
    else
      nullify(p)
    end if

  end function array_p_


  pure function get_array_idx_(this, &
#if BUD_DIM == 1
    i1 &
#elif BUD_DIM == 2
    i1, i2 &
#elif BUD_DIM == 3
    i1, i2, i3 &
#elif BUD_DIM == 4
    i1, i2, i3, i4 &
#elif BUD_DIM == 5
    i1, i2, i3, i4, i5 &
#endif
    ) result(v)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
#if BUD_DIM == 1
    integer, intent(in) :: i1
#elif BUD_DIM == 2
    integer, intent(in) :: i1, i2
#elif BUD_DIM == 3
    integer, intent(in) :: i1, i2, i3
#elif BUD_DIM == 4
    integer, intent(in) :: i1, i2, i3, i4
#elif BUD_DIM == 5
    integer, intent(in) :: i1, i2, i3, i4, i5
#endif
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC) :: v
#else
    BUD_TYPE_VAR :: v
#endif

#if BUD_DIM == 1
    v = this%D%array(i1)
#elif BUD_DIM == 2
    v = this%D%array(i1,i2)
#elif BUD_DIM == 3
    v = this%D%array(i1,i2,i3)
#elif BUD_DIM == 4
    v = this%D%array(i1,i2,i3,i4)
#elif BUD_DIM == 5
    v = this%D%array(i1,i2,i3,i4,i5)
#endif

  end function get_array_idx_


  pure function size_(this, dim) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: dim
    integer :: d

    if ( is_initd(this) ) then
      if ( present(dim) ) then
        ! this is required because
        ! g95 complains about missing interface
        ! due to dim being optional
        d = dim
        d = size(this%D%array, d)
      else
        d = size(this%D%array)
      end if
    else
      d = 0
    end if

  end function size_

  pure function dimensions_(this) result(d)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: d

    if ( is_initd(this) ) then
      d = BUD_DIM
    else
      d = -1
    end if

  end function dimensions_


  subroutine assign_value_(this, v)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: v
#else
    BUD_TYPE_VAR, intent(in) :: v
#endif

    if ( .not. is_initd(this) ) return

    this%D%array BUD_DIM_SPEC(:) = v

  end subroutine assign_value_

  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer  BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    logical :: formatted
    integer :: iu

#if BUD_DIM == 1
    integer :: d1
#elif BUD_DIM == 2
    integer :: d1, d2, i2
#elif BUD_DIM == 3
    integer :: d1, d2, d3, i2, i3
#elif BUD_DIM == 4
    integer :: d1, d2, d3, d4, i2, i3, i4
#elif BUD_DIM == 5
    integer :: d1, d2, d3, d4, d5, i2, i3, i4, i5
#endif

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    ! First figure out if the file is an unformatted file
    formatted = is_formatted(f)
    iu = unit(f)

    ! Get size
    d1 = size_(this, 1)
#if BUD_DIM > 1
    d2 = size_(this, 2)
#endif
#if BUD_DIM > 2
    d3 = size_(this, 3)
#endif
#if BUD_DIM > 3
    d4 = size_(this, 4)
#endif
#if BUD_DIM > 4
    d5 = size_(this, 5)
#endif

    ! First we write the size of the array
    if ( formatted ) then
      write(iu, '(i16)') BUD_DIM
#if BUD_DIM == 1
      write(iu, '(i16)') d1
#elif BUD_DIM == 2
      write(iu, '(i16,tr1,i16)') d1, d2
#elif BUD_DIM == 3
      write(iu, '(2(i16,tr1),i16)') d1, d2, d3
#elif BUD_DIM == 4
      write(iu, '(3(i16,tr1),i16)') d1, d2, d3, d4
#elif BUD_DIM == 5
      write(iu, '(4(i16,tr1),i16)') d1, d2, d3, d4, d5
#endif
    else
      write(iu) BUD_DIM
#if BUD_DIM == 1
      write(iu) d1
#elif BUD_DIM == 2
      write(iu) d1, d2
#elif BUD_DIM == 3
      write(iu) d1, d2, d3
#elif BUD_DIM == 4
      write(iu) d1, d2, d3, d4
#elif BUD_DIM == 5
      write(iu) d1, d2, d3, d4, d5
#endif
    end if

    ! Retrieve data pointer...
    p => array_p(this)

#if BUD_DIM > 4
    do i5 = 1 , d5
#endif
#if BUD_DIM > 3
    do i4 = 1 , d4
#endif
#if BUD_DIM > 2
    do i3 = 1 , d3
#endif
#if BUD_DIM > 1
    do i2 = 1 , d2
#endif
      if ( formatted ) then
#if BUD_DIM == 1
        write(iu, '(e20.16)') p
#elif BUD_DIM == 2
        write(iu, '(e20.16)') p(:,i2)
#elif BUD_DIM == 3
        write(iu, '(e20.16)') p(:,i2,i3)
#elif BUD_DIM == 4
        write(iu, '(e20.16)') p(:,i2,i3,i4)
#elif BUD_DIM == 5
        write(iu, '(e20.16)') p(:,i2,i3,i4,i5)
#endif
      else
#if BUD_DIM == 1
        write(iu) p
#elif BUD_DIM == 2
        write(iu) p(:,i2)
#elif BUD_DIM == 3
        write(iu) p(:,i2,i3)
#elif BUD_DIM == 4
        write(iu) p(:,i2,i3,i4)
#elif BUD_DIM == 5
        write(iu) p(:,i2,i3,i4,i5)
#endif
      end if
#if BUD_DIM > 1
    end do
#endif
#if BUD_DIM > 2
    end do
#endif
#if BUD_DIM > 3
    end do
#endif
#if BUD_DIM > 4
    end do
#endif

  end subroutine write_

  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)

    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#else
    BUD_TYPE_VAR, pointer  BUD_FORTRAN_CONTIGUOUS :: p BUD_DIM_SPEC(:)
#endif

    logical :: formatted, same_size
    integer :: iu, d

#ifdef BUD_IS_INTEGER
    character(len=64), parameter :: fmt_ = '(i16)'
#else
    character(len=64), parameter :: fmt_ = '(e20.16)'
#endif

    integer :: d1, d2, d3, d4, d5, i2, i3, i4, i5

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    ! First figure out if the file is an unformatted file
    formatted = is_formatted(f)
    iu = unit(f)

    ! First we need to read the array dimensions...
    if ( formatted ) then
      read(iu, '(i16)') d
    else
      read(iu) d
    end if

    if ( d > BUD_DIM ) then
      call delete(this)
      ! Signal wrong dimensions
      call set_error(this, -d)
      return
    end if

    ! Initialize all dimensions to 1
    d1 = 1
    d2 = 1
    d3 = 1
    d4 = 1
    d5 = 1

    if ( formatted ) then

      select case ( d )
      case ( 1 )
        read(iu, '(i16)') d1
      case ( 2 )
        read(iu, '(i16,tr1,i16)') d1, d2
      case ( 3 )
        read(iu, '(2(i16,tr1),i16)') d1, d2, d3
      case ( 4 )
        read(iu, '(3(i16,tr1),i16)') d1, d2, d3, d4
      case ( 5 )
        read(iu, '(4(i16,tr1),i16)') d1, d2, d3, d4, d5
      end select

    else

      select case ( d )
      case ( 1 )
        read(iu) d1
      case ( 2 )
        read(iu) d1, d2
      case ( 3 )
        read(iu) d1, d2, d3
      case ( 4 )
        read(iu) d1, d2, d3, d4
      case ( 5 )
        read(iu) d1, d2, d3, d4, d5
      end select

    end if

    ! First, check for dimensions
    same_size = size_(this, 1) == d1
#if BUD_DIM > 1
    same_size = same_size .and. size_(this, 2) == d2
#endif
#if BUD_DIM > 2
    same_size = same_size .and. size_(this, 3) == d3
#endif
#if BUD_DIM > 3
    same_size = same_size .and. size_(this, 4) == d4
#endif
#if BUD_DIM > 4
    same_size = same_size .and. size_(this, 5) == d5
#endif
    if ( .not. same_size ) then
      ! Re-allocate
#if BUD_DIM == 1
      call new(this, d1)
#elif BUD_DIM == 2
      call new(this, d1, d2)
#elif BUD_DIM == 3
      call new(this, d1, d2, d3)
#elif BUD_DIM == 4
      call new(this, d1, d2, d3, d4)
#elif BUD_DIM == 5
      call new(this, d1, d2, d3, d4, d5)
#endif
    end if

    ! Retrieve data pointer...
    p => array_p(this)

#if BUD_DIM > 4
    do i5 = 1 , d5
#endif
#if BUD_DIM > 3
    do i4 = 1 , d4
#endif
#if BUD_DIM > 2
    do i3 = 1 , d3
#endif
#if BUD_DIM > 1
    do i2 = 1 , d2
#endif
      if ( formatted ) then
#if BUD_DIM == 1
        read(iu, fmt_) p(:)
#elif BUD_DIM == 2
        read(iu, fmt_) p(:,i2)
#elif BUD_DIM == 3
        read(iu, fmt_) p(:,i2,i3)
#elif BUD_DIM == 4
        read(iu, fmt_) p(:,i2,i3,i4)
#elif BUD_DIM == 5
        read(iu, fmt_) p(:,i2,i3,i4,i5)
#endif
      else
#if BUD_DIM == 1
        read(iu) p
#elif BUD_DIM == 2
        read(iu) p(:,i2)
#elif BUD_DIM == 3
        read(iu) p(:,i2,i3)
#elif BUD_DIM == 4
        read(iu) p(:,i2,i3,i4)
#elif BUD_DIM == 5
        read(iu) p(:,i2,i3,i4,i5)
#endif
      end if
#if BUD_DIM > 1
    end do
#endif
#if BUD_DIM > 2
    end do
#endif
#if BUD_DIM > 3
    end do
#endif
#if BUD_DIM > 4
    end do
#endif

  end subroutine read_

  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
#if BUD_DIM == 1
    integer :: d1
#elif BUD_DIM == 2
    integer :: d1, d2
#elif BUD_DIM == 3
    integer :: d1, d2, d3
#elif BUD_DIM == 4
    integer :: d1, d2, d3, d4
#elif BUD_DIM == 5
    integer :: d1, d2, d3, d4, d5
#endif

    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = BUD_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'

    if ( .not. is_initd(this) ) then
      write(*,fmt) "<", trim(name), " not initialized>"
      return
    end if

    ! Get size
    d1 = size_(this, 1)
#if BUD_DIM > 1
    d2 = size_(this, 2)
#endif
#if BUD_DIM > 2
    d3 = size_(this, 3)
#endif
#if BUD_DIM > 3
    d4 = size_(this, 4)
#endif
#if BUD_DIM > 4
    d5 = size_(this, 5)
#endif

    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,10(i0,a))'

    write(*,fmt) "<", trim(name), " d1=", d1, &
#if BUD_DIM > 1
      " d2=",d2, &
#endif
#if BUD_DIM > 2
      " d3=",d3, &
#endif
#if BUD_DIM > 3
      " d4=",d4, &
#endif
#if BUD_DIM > 5
      " d5=",d5, &
#endif
      ", refs: ", references(this), ">"

  end subroutine print_


  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

#include "bud_cleanup.inc"


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

