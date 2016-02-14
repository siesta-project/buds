  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Include the common utilities
# include "refype_utils.inc"

  !> @defgroup refype-common Common reference counted type functions
  !!
  !! @details
  !! The common data structure routines listed here are available
  !! in all reference counted types.
  !!
  !! The basic functionality of reference counting is performed
  !! in these few routines.
  !!
  !! @note
  !! A type need not have _all_ these routines available if so desired.
  !! This is internally controlled and is mentioned for the relevant types.
  !!
  !! @dev-note
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !!
  !! The routines defined in this file may not be defined in any
  !! of the reference counted type-modules. Doing so will create
  !! name-clashes.
  !!
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - __R_TYPE_NAME, name of the resulting type
  !!
  !! @acknowledge
  !! These declarations are community inspired from the FLIBS
  !! project by Arjen Markus as well as the PyF95 project.
  !! The initial draft of this library has been created by Alberto Garcia.

  ! Start on function declarations
  
  !!> @addtogroup refype-common
  !!! @{


  public :: _R_TYPE_NAME
  public :: init, delete
  ! We force `delete_data` to be private.
  ! This will prohibit a mistaken public designation of the
  ! routine.
  private :: delete_data
  public :: refs

  public :: same, is_same
  public :: initd, is_initd

  public :: assignment(=)

  !> Interfaced assignment of types
  interface assignment(=)
    module procedure assign_
  end interface assignment(=)

  !> Interfaced initialization of types
  !! @iSee initialize_
  interface init
    module procedure initialize_
  end interface init
  
  !> Interfaced checking of initialization
  !! @iSee initialized_
  interface initd
    module procedure initialized_
  end interface initd
  !> Interfaced checking of initialization
  !! @iSee is_initialized_
  interface is_initd
    module procedure is_initialized_
  end interface is_initd

  !> Interfaced checking of comparison
  !! @iSee same_
  interface same
    module procedure same_
  end interface same
  !> Interfaced checking of comparison
  !! @iSee is_same_
  interface is_same
    module procedure is_same_
  end interface is_same

#ifndef _R_NO_DELETE
  !> Interfaced checking of comparison
  !! @iSee delete_
  interface delete
    module procedure delete_
  end interface delete
#endif

  !> Interfaced checking of comparison
  !! @iSee sub_references__
  interface refs
    module procedure sub_references_
  end interface refs

  !> Interfaced checking of comparison
  !! @iSee func_references_
  interface frefs
    module procedure func_references_
  end interface frefs

  !> @cond ALWAYS_SKIP

#if defined(REFYPE_ERROR)
  ! Stand-alone routine which must be provided
  interface
    subroutine error(str)
      character(len=*), intent(in), optional :: str
    end subroutine error
  end interface
#else
  ! Issue a COMPILER warning
#  warning "REFYPE_ERROR not defined! Expect sudden crashes without warnings!"
#endif

contains

  ! contains in non-modules freak doxygen out...
  !> @endcond ALWAYS_SKIP

  !> Internal initialization of data container
  !!
  !! Initializes the data handler to a new data handler.
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this Type that is initialized
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine initialize_(this)
    type(_R_TYPE_NAME), intent(inout) :: this

    integer :: error
    
    ! First, remove the current reference
    call delete(this)

    ! Allocate fresh storage
    allocate(this%data, stat=error)
    
    if ( 0 /= error ) then
#if defined(REFYPE_ERROR)
      call REFYPE_ERROR("Error allocating data structure")
#endif
      return
    end if

    ! Set the initial reference count
    this%data%ryp_refs = 1

    ! Tag the object (always)
    call tag_object_(this)
    
  end subroutine initialize_

  !> Query whether type is associated
  !!
  !! Check whether a type is associated to a container.
  !!
  !! @param[in] this The data type.
  !! @param[out] init true if it is associated
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure subroutine initialized_(this, init)
    type(_R_TYPE_NAME), intent(in) :: this
    logical, intent(out) :: init
    
    init = associated(this%data)
    
  end subroutine initialized_

  !> Function equivalent of initialized
  !!
  !! @iSee initialized_
  !!
  !! @param[in] this data type.
  !! @return true if it is associated.
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure function is_initialized_(this) result(init)
    type(_R_TYPE_NAME), intent(in) :: this

    logical :: init
    
    call initd(this, init)
    
  end function is_initialized_

#ifndef _R_NO_DELETE
  
  !> Deletes the current type
  !!
  !! Deletes the data type. The number of references will be
  !! decremented.
  !! If the number of references of the associated data is 0
  !! after decremention, the data will be
  !! deleted. Else only this type will be un-associated upon return.
  !!
  !! This routine is elemental and may be called on arrays.
  !!
  !! @param[inout] this Type that is deleted
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  elemental subroutine delete_(this)
    type(_R_TYPE_NAME), intent(inout) :: this

    integer :: error
    
    if (.not. is_initd(this) ) return
    
    this%data%ryp_refs = this%data%ryp_refs - 1
    
    if ( 0 == this%data%ryp_refs ) then
      
      ! Safe to delete the data now.
      ! Use the routine provided for this specific
      ! type to clean up any internal structure
      
      call delete_data(this%data)
      
      ! Deallocate the currently referenced storage
      deallocate(this%data, stat=error)
      if ( 0 /= error ) then
#if defined(REFYPE_ERROR)
        call REFYPE_ERROR("Error in deallocation")
#endif
      end if

    end if
    
    ! This is important to use the correct initialized functions
    nullify(this%data)
    
  end subroutine delete_

#endif
  
  !> Internal assignment of data container
  !!
  !! Method for assigning a data container with another
  !! data-container.
  !!
  !! In case `rhs` is not initialized, so will `lhs`.
  !!
  !! @param[inout] lhs Type that is assigned to (deleted before assignment)
  !! @param[in] rhs Type that is assigned
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  subroutine assign_(lhs, rhs)

    ! No copying of data is involved, simply an increment of the
    ! reference counter.
    
    ! IMPORTANT NOTE: Assignment must take the form of a subroutine, and
    ! not of a function, since the "inout" intent is essential. One has
    ! to clean up "lhs" before making it point to the same place as
    ! "rhs".  In a function, the intrinsic "out" intent for "lhs"
    ! will destroy any prior information.

    type(_R_TYPE_NAME), intent(inout) :: lhs
    type(_R_TYPE_NAME), intent(in) :: rhs

    ! Delete to remove the current reference of "this"
    call delete(lhs)
    
    ! If RHS is empty, return immediately
    ! Note that LHS is still deleted
    if ( .not. is_initd(rhs) ) return

    ! Establish the new reference and increment the reference counter.
    lhs%data => rhs%data
    lhs%data%ryp_refs = rhs%data%ryp_refs + 1
    
  end subroutine assign_


  !> Check whether two types are the same
  !!
  !! Returns true if the two passed types are the same
  !! data.
  !!
  !! @param[in] lhs type
  !! @param[in] rhs type
  !! @param[out] same True if `lhs` and `rhs` point to the same data
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure subroutine same_(lhs, rhs, same)
    type(_R_TYPE_NAME), intent(in) :: lhs, rhs
    logical, intent(out) :: same
    ! If they are not both initialized they can not be the same
    same = is_initd(lhs) .and. is_initd(rhs)
    if ( .not. same ) return
    ! this checks that the memory location of
    ! the underlying data is the same.
    same = associated(lhs%data, target=rhs%data)
  end subroutine same_

  !> Check whether two types are the same
  !!
  !! Returns true if the two passed types are the same
  !! data.
  !!
  !! @param[in] lhs type-1
  !! @param[in] rhs type-2
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure function is_same_(lhs, rhs) result(same)
    type(_R_TYPE_NAME), intent(in) :: lhs, rhs
    logical :: same
    call same_(lhs, rhs, same)
  end function is_same_

  !> Query number of references to the equivalent type
  !!
  !! Get number of times this data type has been replicated
  !! in the reference counted object.
  !!
  !! @param[in] this data type
  !! @param[out] refs number of times `this` has been referenced
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure subroutine sub_references_(this, refs)
    type(_R_TYPE_NAME), intent(in) :: this
    integer, intent(out) :: refs
    
    if ( is_initd(this) ) then
      refs = this%data%ryp_refs
    else
      refs = 0
    end if
    
  end subroutine sub_references_

  !> Query number of references to the equivalent type
  !!
  !! @iSee @sub_references_
  !!
  !! @param[in] this data type
  !! @return number of times `this` has been referenced
  !!
  !! @author Nick R. Papior
  !! @date 2016, February
  pure function func_references_(this) result(refs)
    type(_R_TYPE_NAME), intent(in) :: this
    
    integer :: refs
    
    call sub_references_(this, refs)
    
  end function func_references_

  ! The remaining procedures are currently
  ! not used by refype
  function id_(this) result(str)
    type(_R_TYPE_NAME), intent(in) :: this
    
    character(len=RYP_ID_LEN) :: str
    
    str = this%data%ryp_id
    
  end function id_

  subroutine tag_object_(this)
    type(_R_TYPE_NAME), intent(inout)  :: this

    ! currently we do not tag the types
    
  end subroutine tag_object_

  !> @}

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

