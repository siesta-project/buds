  ! @@LICENSE@@ see Copyright notice in the top-directory

#include "refype_utils.inc"

#define _R_MOD_NAME_STR _R_XSTR(_R_MOD_NAME)
#define _R_TYPE_NAME_ _R_CC2(_R_TYPE_NAME,_)
#define _R_TYPE_NAME_STR _R_XSTR(_R_TYPE_NAME)
  
  !> _R_MOD_NAME documentation
  !!
  !! @details
  !! The detailed implementation of the _R_TYPE_NAME type.
  !!
  !! @note
  !! This documentation is duplicated in all types with different precision.
  !!
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - __R_MOD_NAME of the module
  !!  - __R_TYPE_NAME of the type
  !!  - __R_TYPE_NAME_, internal data pointer of the type
  !!  - __R_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - __R_TYPE_NEW, the routine name for creating a new type
  !!  - __R_TYPE_VAR, the variable type contained in the array
  !!  - __R_TYPE_VAR_PREC, the precision specifier for the array
  !! If you add any new dependencies be sure to undefine the variable
  !! at the bottom of this file.

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "refype_common_declarations.inc"

  ! Determine the variable size
#if _R_DIM == 0
#  define _R_DIM_SPEC()
#elif _R_DIM == 1
#  define _R_DIM_SPEC() (:)
#elif _R_DIM == 2
#  define _R_DIM_SPEC() (:,:)
#elif _R_DIM == 3
#  define _R_DIM_SPEC() (:,:,:)
#elif _R_DIM == 4
#  define _R_DIM_SPEC() (:,:,:,:)
#else
#  error "mryp_Array: Does not handle array sizes larger than 4"
#endif

  !> @rype container for _R_TYPE_NAME
  !!
  !! Contains an array of _R_DIM dimensions.
  !!
  type _R_TYPE_NAME_
    ! Consistent data in the reference counted object
#   include "refype_common_type.inc"

    !> Array stored in the @rype
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), pointer :: val _R_DIM_SPEC() => null()
#else
    _R_TYPE_VAR, pointer :: val _R_DIM_SPEC() => null()
#endif
    
  end type _R_TYPE_NAME_

  ! Public data-type routines defined particularly for
  ! this type.
  public :: new, _R_TYPE_NEW
  public :: val, init_val, size
  public :: print
  
  !> Create new @rype instances
  interface new
    module procedure new_dim_
    module procedure new_copy_
  end interface new
  !> Create new @rype instances
  interface _R_TYPE_NEW
    module procedure new_dim_
    module procedure new_copy_
  end interface _R_TYPE_NEW

  !> Retrieve pointer/vaule to/in the stored array
  !!
  !! With a `=>` a pointer to the stored array is returned.
  !! With an indexed call the value in the specific array
  !! at that index is returned.
  !!
  !! @note
  !! For excessive access to the array one should retrieve
  !! a pointer, and then access the elements in that array.
  interface val
    module procedure val_
    module procedure val_idx_
  end interface val

  !> Number of elements is the stored array (same as regular `size`)
  interface size
    module procedure size_
  end interface size

  !> Initialize all values in the array to a specific value
  interface init_val
    module procedure init_val_
  end interface init_val

  !> Print information about the @rype
  interface print
    module procedure print_
  end interface print

  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
# include "refype_common.inc"

  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  pure subroutine delete_data(this)
    type(_R_TYPE_NAME_), intent(inout) :: this

    ! Currently we do not allow external memory
    ! tracking.
    if ( .not. associated(this%val) ) return
    deallocate(this%val)
    nullify(this%val)
    
  end subroutine delete_data

  !> Create new _R_TYPE_NAME via dimension sizes.
  !!
  !! Initializes a new type container from the specified
  !! size in the call.
  !!
  !! @param[inout] this the @rype
  !! @param[in] d1 size of the internal reference counted array (dim=1)
#if _R_DIM > 1
  !! @param[in] d2 size of the internal reference counted array (dim=2)
#if _R_DIM > 2
  !! @param[in] d3 size of the internal reference counted array (dim=3)
#endif
#endif
  !! @param[in] val @opt=@null initial value of the array
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine new_dim_(this, &
#if _R_DIM == 1
    d1, &
#elif _R_DIM == 2
    d1, d2, &
#elif _R_DIM == 3
    d1, d2, d3, &
#endif
    val)
    type(_R_TYPE_NAME), intent(inout) :: this
#if _R_DIM == 1
    integer, intent(in) :: d1
#elif _R_DIM == 2
    integer, intent(in) :: d1, d2
#elif _R_DIM == 3
    integer, intent(in) :: d1, d2, d3
#endif
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), intent(in), optional :: val 
#else
    _R_TYPE_VAR, intent(in), optional :: val
#endif

    ! Make sure the container is initialized and ready for
    ! data-creation.

    call init(this)

    ! Allocate the value array
#if _R_DIM == 1
    allocate(this%data%val(d1))
#elif _R_DIM == 2
    allocate(this%data%val(d1,d2))
#elif _R_DIM == 3
    allocate(this%data%val(d1,d2,d3))
#endif

    if ( present(val) ) then
      call init_val(this, val)
    end if

  end subroutine new_dim_


  !> Create new _R_TYPE_NAME by coping another array.
  !!
  !! Initializes a new type container from a copy of another
  !! array.
  !!
  !! The output data container will have the same
  !! size as the value array.
  !!
  !! @param[inout] this @rype container
  !! @param[in] val array to dublicate in `this`
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine new_copy_(this, val)
    type(_R_TYPE_NAME), intent(inout) :: this
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), intent(in) :: val _R_DIM_SPEC()
#else
    _R_TYPE_VAR, intent(in) :: val _R_DIM_SPEC()
#endif

#if _R_DIM == 1
    integer :: d1
#elif _R_DIM == 2
    integer :: d1, d2
#elif _R_DIM == 3
    integer :: d1, d2, d3
#endif

    d1 = size(val,dim=1)
#if _R_DIM > 1
    d2 = size(val,dim=2)
#elif _R_DIM > 2
    d3 = size(val,dim=3)
#endif

    ! initialize and create data-container
#if _R_DIM == 1
    call new_dim_(this, d1)
#elif _R_DIM == 2
    call new_dim_(this, d1, d2)
#elif _R_DIM == 3
    call new_dim_(this, d1, d2, d3)
#endif

    ! copy data
    this%data%val = val

  end subroutine new_copy_


  !> Retrieve pointer to data array in @rype
  !!
  !! Returns a pointer to the data-array that resides in
  !! the @rype.
  !!
  !! @param[in] this the containing @rype
  !! @return a pointer to the data
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  function val_(this) result(p)
    type(_R_TYPE_NAME), intent(in) :: this
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), pointer :: p _R_DIM_SPEC()
#else
    _R_TYPE_VAR, pointer :: p _R_DIM_SPEC()
#endif

    p => this%data%val

  end function val_


  !> Retrieve value in specific index of data array in the container
  !!
  !! Returns a value in the data container from a specific index.
  !!
  !! @dev_note
  !! This routine should be avoided for extensive retrieval
  !! of data is required.
  !! It may do bounds-check.
  !!
  !! @param[in] this the containing @rype
  !! @param[in] i1 index in the array
#if _R_DIM > 1
  !! @param[in] i2 index in the array
#if _R_DIM > 2
  !! @param[in] i3 index in the array
#endif
#endif
  !! @return the value in the specific indexed location of the array
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure function val_idx_(this, &
#if _R_DIM == 1
    i1 &
#elif _R_DIM == 2
    i1, i2 &
#elif _R_DIM == 3
    i1, i2, i3 &
#endif
    ) result(v)
    type(_R_TYPE_NAME), intent(in) :: this
#if _R_DIM == 1
    integer, intent(in) :: i1
#elif _R_DIM == 2
    integer, intent(in) :: i1, i2
#elif _R_DIM == 3
    integer, intent(in) :: i1, i2, i3
#endif
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC) :: v
#else
    _R_TYPE_VAR :: v
#endif

#if _R_DIM == 1
    v = this%data%val(i1)
#elif _R_DIM == 2
    v = this%data%val(i1,i2)
#elif _R_DIM == 3
    v = this%data%val(i1,i2,i3)
#endif

  end function val_idx_


  !> Query size of array
  !!
  !! Returns the size of the array contained.
  !!
  !! @note
  !! There is no checking of the dimension queried.
  !!
  !! @param[in] this @rype container
  !! @param[in] dim @opt=@null along the specified dimension
  !! @return the size of the contained array
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure function size_(this, dim) result(d)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: dim

    integer :: d

    if ( .not. is_initd(this) ) then
      d = 0
    else
      d = size(this%data%val, dim=dim)
    end if

  end function size_


  !> Initialize the array in @rype to a specific value.
  !!
  !! Set all array elements to a specific value.
  !!
  !! @param[in] this @rype containing the array
  !! @param[in] v value to set all elements equal to
  !!
  !! @author Nick R. Papior
  !! @author 2016, February
  subroutine init_val_(this, v)
    type(_R_TYPE_NAME), intent(inout) :: this
#ifdef _R_TYPE_VAR_PREC
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), intent(in) :: v
    _R_TYPE_VAR(_R_TYPE_VAR_PREC), pointer :: p _R_DIM_SPEC()
#else
    _R_TYPE_VAR, intent(in) :: v
    _R_TYPE_VAR, pointer :: p _R_DIM_SPEC()
#endif

    ! Retrieve data
    p => val(this)
    ! set data
    p = v

  end subroutine init_val_


  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=_R_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  !!
  !! @author Nick R. Papior
  !! @author 2016, February
  subroutine print_(this, info, indent)
    type(_R_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent

    integer :: lindent
#if _R_DIM == 1
    integer :: d1
#elif _R_DIM == 2
    integer :: d1, d2
#elif _R_DIM == 3
    integer :: d1, d2, d3
#endif

    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = _R_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent

    write(fmt, '(a,i0,a)') '(t',indent,',2a)'

    if ( .not. is_initd(this) ) then
      write(*,fmt) trim(name), " not initialized."
      return
    end if

    ! Get size
    d1 = size_(this, 1)
#if _R_DIM > 1
    d2 = size_(this, 2)
#endif
#if _R_DIM > 3
    d3 = size_(this, 3)
#endif
    
    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,10(i0,a))'
    
    write(*,fmt) "<", trim(name), " d1=", d1, &
#if _R_DIM > 1
      " d2=",d2, &
#endif
#if _R_DIM > 2
      " d3=",d3, &
#endif
      ", refs: ", frefs(this), ">"

  end subroutine print_

  ! Local pre-processor variables that
  ! undefine the variables that are not needed anymore.
#undef _R_MOD_NAME
#undef _R_TYPE_NAME
#undef _R_TYPE_NAME_
#undef _R_TYPE_NAME_STR
#undef _R_TYPE_NEW
#undef _R_TYPE_VAR
#undef _R_TYPE_VAR_PREC

! project-refype -- local file settings
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

