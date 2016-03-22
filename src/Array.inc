  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "bud_utils.inc"

#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)
  
  !> BUD_MOD_NAME documentation
  !!
  !! @details
  !! The detailed implementation of the BUD_TYPE_NAME type.
  !!
  !! @note
  !! This documentation is duplicated in all types with different precision.
  !!
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_MOD_NAME of the module
  !!  - _BUD_TYPE_NAME of the type
  !!  - _BUD_TYPE_NAME_, internal data pointer of the type
  !!  - _BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - _BUD_TYPE_NEW, the routine name for creating a new type
  !!  - _BUD_TYPE_VAR, the variable type contained in the array
  !!  - _BUD_TYPE_VAR_PREC, the precision specifier for the array
  !! If you add any new dependencies be sure to undefine the variable
  !! at the bottom of this file.

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  ! Determine the variable size
#if BUD_DIM == 0
#  define BUD_DIM_SPEC()
#elif BUD_DIM == 1
#  define BUD_DIM_SPEC() (:)
#elif BUD_DIM == 2
#  define BUD_DIM_SPEC() (:,:)
#elif BUD_DIM == 3
#  define BUD_DIM_SPEC() (:,:,:)
#elif BUD_DIM == 4
#  define BUD_DIM_SPEC() (:,:,:,:)
#else
#  error "mbud_Array: Does not handle array sizes larger than 4"
#endif

  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains an array of BUD_DIM dimensions.
  !!
  type BUD_TYPE_NAME_

    !> Array stored in the @bud
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer, contiguous :: array BUD_DIM_SPEC() => null()
#else
    BUD_TYPE_VAR, pointer, contiguous :: array BUD_DIM_SPEC() => null()
#endif

    ! Consistent data in the reference counted object
#   include "bud_common_type.inc"

  end type BUD_TYPE_NAME_

  
  !> Create new @bud instances
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: new
  !> Create new @bud instances
  interface BUD_TYPE_NEW
    module procedure new_dim_
    module procedure new_copy_
  end interface
  public :: BUD_TYPE_NEW

  !> Retrieve pointer/value to/in the stored array
  !!
  !! With a `=>` a pointer to the stored array is returned.
  !! With an indexed call the value in the specific array
  !! at that index is returned.
  !!
  !! @note
  !! For excessive access to the array one should retrieve
  !! a pointer, and then access the elements in that array.
  interface getp
    module procedure get_arrayp_
  end interface
  public :: getp

  interface get_arrayp
    module procedure get_arrayp_
  end interface
  public :: get_arrayp

  interface get
    module procedure get_array_idx_
  end interface
  public :: get
  interface get_array
    module procedure get_array_idx_
  end interface
  public :: get_array


  !> Number of elements is the stored array (same as regular `size`)
  interface size
    module procedure size_
  end interface
  public :: size

  !> Initialize all values in the array to a specific value
  interface init_array
    module procedure init_array_
  end interface
  public :: init_array
  
  !> Print information about the @bud
  interface print
    module procedure print_
  end interface
  public :: print

  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "bud_common.inc"

  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  pure subroutine delete_data(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. associated(this%array) ) return
    deallocate(this%array)
    nullify(this%array)
    
  end subroutine delete_data

  !> Create new BUD_TYPE_NAME via dimension sizes.
  !!
  !! Initializes a new type container from the specified
  !! size in the call.
  !!
  !! @param[inout] this the @bud
  !! @param[in] d1 size of the internal reference counted array (dim=1)
#if BUD_DIM > 1
  !! @param[in] d2 size of the internal reference counted array (dim=2)
#if BUD_DIM > 2
  !! @param[in] d3 size of the internal reference counted array (dim=3)
#if BUD_DIM > 3
  !! @param[in] d4 size of the internal reference counted array (dim=4)
#endif
#endif
#endif
  !! @param[in] val @opt=@null initial value of all elements
  subroutine new_dim_(this, &
#if BUD_DIM == 1
    d1, &
#elif BUD_DIM == 2
    d1, d2, &
#elif BUD_DIM == 3
    d1, d2, d3, &
#elif BUD_DIM == 4
    d1, d2, d3, d4, &
#endif
    val)
    type(BUD_TYPE_NAME), intent(inout) :: this
#if BUD_DIM == 1
    integer, intent(in) :: d1
#elif BUD_DIM == 2
    integer, intent(in) :: d1, d2
#elif BUD_DIM == 3
    integer, intent(in) :: d1, d2, d3
#elif BUD_DIM == 4
    integer, intent(in) :: d1, d2, d3, d4
#endif
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in), optional :: val
#else
    BUD_TYPE_VAR, intent(in), optional :: val
#endif

    ! Make sure the container is initialized and ready for
    ! data-creation.

    call init(this)

    ! Allocate the value array
#if BUD_DIM == 1
    allocate(this%D%array(d1))
#elif BUD_DIM == 2
    allocate(this%D%array(d1,d2))
#elif BUD_DIM == 3
    allocate(this%D%array(d1,d2,d3))
#elif BUD_DIM == 4
    allocate(this%D%array(d1,d2,d3,d4))
#endif

    if ( present(val) ) then
      call init_array(this, val)
    end if

  end subroutine new_dim_


  !> Create new BUD_TYPE_NAME by coping another array.
  !!
  !! Initializes a new type container from a copy of another
  !! array.
  !!
  !! The output data container will have the same
  !! size as the value array.
  !!
  !! @param[inout] this @bud container
  !! @param[in] array array to dublicate in `this`
  subroutine new_copy_(this, array)
    type(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: array BUD_DIM_SPEC()
#else
    BUD_TYPE_VAR, intent(in) :: array BUD_DIM_SPEC()
#endif

#if BUD_DIM == 1
    integer :: d1
#elif BUD_DIM == 2
    integer :: d1, d2
#elif BUD_DIM == 3
    integer :: d1, d2, d3
#elif BUD_DIM == 4
    integer :: d1, d2, d3, d4
#endif

    d1 = size(array,dim=1)
#if BUD_DIM > 1
    d2 = size(array,dim=2)
#endif
#if BUD_DIM > 2
    d3 = size(array,dim=3)
#endif
#if BUD_DIM > 3
    d4 = size(array,dim=4)
#endif

    ! initialize and create data-container
#if BUD_DIM == 1
    call new_dim_(this, d1)
#elif BUD_DIM == 2
    call new_dim_(this, d1, d2)
#elif BUD_DIM == 3
    call new_dim_(this, d1, d2, d3)
#elif BUD_DIM == 4
    call new_dim_(this, d1, d2, d3, d4)
#endif

    ! copy data
    this%D%array = array

  end subroutine new_copy_


  !> Retrieve pointer to data array in @bud
  !!
  !! Returns a pointer to the data-array that resides in
  !! the @bud.
  !!
  !! @param[in] this the containing @bud
  !! @return a pointer to the data
  function get_arrayp_(this) result(p)
    type(BUD_TYPE_NAME), intent(in) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), pointer :: p BUD_DIM_SPEC()
#else
    BUD_TYPE_VAR, pointer :: p BUD_DIM_SPEC()
#endif

    p => this%D%array

  end function get_arrayp_


  !> Retrieve value in specific index of data array in the container
  !!
  !! Returns a value in the data container from a specific index.
  !!
  !! @dev_note
  !! This routine should be avoided for extensive retrieval
  !! of data is required.
  !! It may do bounds-check.
  !!
  !! @param[in] this the containing @bud
  !! @param[in] i1 index in the array
#if BUD_DIM > 1
  !! @param[in] i2 index in the array
#if BUD_DIM > 2
  !! @param[in] i3 index in the array
#if BUD_DIM > 3
  !! @param[in] i4 index in the array
#endif
#endif
#endif
  !! @return the value in the specific indexed location of the array
  pure function get_array_idx_(this, &
#if BUD_DIM == 1
    i1 &
#elif BUD_DIM == 2
    i1, i2 &
#elif BUD_DIM == 3
    i1, i2, i3 &
#elif BUD_DIM == 4
    i1, i2, i3, i4 &
#endif
    ) result(v)
    type(BUD_TYPE_NAME), intent(in) :: this
#if BUD_DIM == 1
    integer, intent(in) :: i1
#elif BUD_DIM == 2
    integer, intent(in) :: i1, i2
#elif BUD_DIM == 3
    integer, intent(in) :: i1, i2, i3
#elif BUD_DIM == 4
    integer, intent(in) :: i1, i2, i3, i4
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
#endif

  end function get_array_idx_


  !> Query size of array
  !!
  !! Returns the size of the array contained.
  !!
  !! @note
  !! There is no checking of the dimension queried.
  !!
  !! @param[in] this @bud container
  !! @param[in] dim @opt=@null along the specified dimension
  !! @return the size of the contained array
  pure function size_(this, dim) result(d)
    type(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: dim

    integer :: d

    if ( is_initd(this) ) then
      d = size(this%D%array, dim=dim)
    else
      d = 0
    end if

  end function size_


  !> Initialize the array in @bud to a specific value.
  !!
  !! Set all array elements to a specific value.
  !!
  !! @param[in] this @bud containing the array
  !! @param[in] v value to set all elements equal to
  subroutine init_array_(this, v)
    type(BUD_TYPE_NAME), intent(inout) :: this
#ifdef BUD_TYPE_VAR_PREC
    BUD_TYPE_VAR(BUD_TYPE_VAR_PREC), intent(in) :: v
#else
    BUD_TYPE_VAR, intent(in) :: v
#endif

    if ( is_initd(this) ) then
      this%D%array = v
    end if

  end subroutine init_array_


  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    type(BUD_TYPE_NAME), intent(in) :: this
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
      ", refs: ", get_refs(this), ">"

  end subroutine print_

  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef BUD_MOD_NAME
#undef BUD_TYPE_NAME
#undef BUD_TYPE_NAME_
#undef BUD_TYPE_NAME_STR
#undef BUD_TYPE_NEW
#undef BUD_TYPE_VAR
#undef BUD_TYPE_VAR_PREC

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

