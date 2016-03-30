  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Include the common utilities
# include "bud_utils.inc"

  !> @defgroup bud-common Global functions for reference counted types
  !!
  !! @brief
  !! Common routines and interfaces that are required for
  !! handling and maintaing a @bud.
  !!
  !! @details
  !! The common routines are the initialization routines,
  !! deletion, checking for initialization, checking for
  !! equivalence between two @buds, number of references for
  !! the @bud.
  !!
  !! @dev_note
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !! I.e. you may not make _any_ routine/function public. Only interfaces.
  !!
  !! The routines defined in this file may not be defined in any
  !! of the reference counted type-modules. Doing so will create
  !! name-clashes.
  !!
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_TYPE_NAME, name of the resulting type
  !!  - _BUD_TYPE_NAME_, contained data structure (as a pointer)
  !!  - _BUD_ERROR, if this is not defined there will be
  !!    no error mechanism for catching erroneous calls.

  ! Start on function declarations
  
  !!> @addtogroup bud-common
  !!! @{

  !> @cond ALWAYS_SKIP
  
  !> Name of the reference counted type (@bud)
  public :: BUD_TYPE_NAME

#ifndef BUD_NOTYPE
  
  ! public data type
  ! however, the actual data is contained in the sub-structure
  type BUD_TYPE_NAME
    BUD_CLASS(BUD_TYPE_NAME_), pointer :: D => null()
    
#if BUD_FORTRAN >= 2003
  contains

    ! This is strictly not needed
    ! However, this may be used internally for
    ! automatic clean-up
    final :: delete_
    
#endif
  end type BUD_TYPE_NAME
  
#endif

  !> Internal data structure (@bud)
  private :: BUD_TYPE_NAME_

  !> @endcond ALWAYS_SKIP
  

  ! We force `delete_data` to be private.
  ! This will prohibit a mistaken public designation of the
  ! routine.
  !> Internal routine for clearing the data container in the @bud
  private :: delete_data

  !> Overiding assignment to ensure correct reference counting
  interface assignment(=)
    module procedure assign_
  end interface
  public :: assignment(=)

  !> Initialization routine of the @bud
  interface init
    module procedure initialize_
  end interface
  public :: init
  
  !> Check if a @bud has been initialized (function)
  interface is_initd
    module procedure is_initialized_
  end interface
  public :: is_initd

  !> Check if two @buds point to the same data (function)
  interface is_same
    module procedure is_same_
  end interface
  public :: is_same
  
#ifndef BUD_NO_DELETE
  !> Delete the @bud, stepping reference counter and clearing this type
  interface delete
    module procedure delete_
  end interface
  public :: delete
#endif

#ifndef BUD_NO_NULLIFY
  !> Nullify the @bud, equivalent to delete (without stepping reference counter)
  interface nullify
    module procedure nullify_
  end interface
  public :: nullify
#endif

  !> Return number of references to the @bud (function)
  interface references
    module procedure references_
  end interface
  public :: references

  !> Return number of references to the @bud (function)
  interface get_refs
    module procedure references_
  end interface
  public :: get_refs

  !> @cond ALWAYS_SKIP

#if defined(BUD_ERROR)
  ! Stand-alone routine which must be provided
  interface
    subroutine BUD_ERROR(str)
      character(len=*), intent(in), optional :: str
    end subroutine 
  end interface
#else
# ifndef _BUD_ERROR_H
#   warning "BUD_ERROR not defined! Expect sudden crashes without warnings!"
#   define _BUD_ERROR_H
# endif
#endif

contains

  ! contains in non-modules freak doxygen out...
  !> @endcond ALWAYS_SKIP

  !> Internal initialization of data container
  !!
  !! Initializes the data handler.
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this @bud that is initialized
  subroutine initialize_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this

    integer :: error
    
    ! First, remove the current reference
    call delete(this)

    ! Allocate fresh storage
    allocate(this%D, stat=error)
    
    if ( 0 /= error ) then
#if defined(BUD_ERROR)
      call BUD_ERROR("Error allocating data structure")
#endif
      return
    end if

    ! Set the initial reference count
    this%D%refs_ = 1

    ! Tag the object (always)
    call tag_object_(this)
    
  end subroutine initialize_

  !> Query whether type is associated
  !!
  !! Check whether a type is associated to a container.
  !!
  !! @param[in] this checked @bud
  !! @param[out] init `.true.` if it is associated, else `.false.`
  pure subroutine initialized_(this, init)
    type(BUD_TYPE_NAME), intent(in) :: this
    logical, intent(out) :: init
    
    init = associated(this%D)
    
  end subroutine initialized_

  !> Function equivalent of initialized
  !!
  !! @iSee initialized_
  !!
  !! @param[in] this checked @bud
  !! @return `.true.` if it is associated, else `.false.`
  pure function is_initialized_(this) result(init)
    type(BUD_TYPE_NAME), intent(in) :: this

    logical :: init

    init = associated(this%D)
    
  end function is_initialized_

#ifndef BUD_NO_DELETE
  
  !> Deletes the current @bud
  !!
  !! Deletes the @bud. The number of references will be
  !! decremented.
  !!
  !! If the number of references of the associated data is 0
  !! after decremention, the data will be
  !! deleted. Else this @bud will return an un-associated @bud.
  !!
  !! @param[inout] this deleted @bud
#ifndef BUD_DELETE_NOELEMENTAL
  elemental subroutine delete_(this)
#else
  subroutine delete_(this)
#endif
    type(BUD_TYPE_NAME), intent(inout) :: this

    integer :: error
    
    if (.not. is_initd(this) ) return
    
    this%D%refs_ = this%D%refs_ - 1
    
    if ( 0 == this%D%refs_ ) then
      
      ! Safe to delete the data now.
      ! Use the routine provided for this specific
      ! type to clean up any internal structure
      
      call delete_data(this%D)
      
      ! Deallocate the currently referenced storage
      deallocate(this%D, stat=error)
      if ( 0 /= error ) then
#if defined(BUD_ERROR)
        call BUD_ERROR("Error in deallocation")
#endif
      end if

    end if
    
    ! This is important to use the correct initialized functions
    nullify(this%D)
    
  end subroutine delete_

#endif

  
#ifndef BUD_NO_NULLIFY
  !> Nullifies the current @bud
  !!
  !! Nullify the @bud. Simple way to not fully delete
  !! an element, but rather remove the pointer to the data.
  !!
  !! @param[inout] this nullify references from this @bud
  elemental subroutine nullify_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this
    
    if (.not. is_initd(this) ) return
    
    nullify(this%D)
    
  end subroutine nullify_
#endif
  
  !> Assignment of @bud with another @bud
  !!
  !! Method for assigning a data container with another
  !! data-container.
  !!
  !! `lhs` is deleted before assignment.
  !!
  !! Make `lhs` contain the same data as in `rhs`.
  !! Internally a pointer to the same data is created and the reference
  !! counter will be incremented.
  !!
  !! `lhs` will be un-associated if `rhs` is un-associated.
  !!
  !! @param[inout] lhs @bud that is assigned to
  !! @param[in] rhs @bud that is dublicated
  subroutine assign_(lhs, rhs)

    ! No copying of data is involved, simply an increment of the
    ! reference counter.
    
    ! IMPORTANT NOTE: Assignment must take the form of a subroutine, and
    ! not of a function, since the "inout" intent is essential. One has
    ! to clean up "lhs" before making it point to the same place as
    ! "rhs".  In a function, the intrinsic "out" intent for "lhs"
    ! will destroy any prior information.

    type(BUD_TYPE_NAME), intent(inout) :: lhs
    type(BUD_TYPE_NAME), intent(in) :: rhs

    ! Delete to remove the current reference of "this"
    call delete(lhs)
    
    ! If RHS is empty, return immediately
    ! Note that LHS is still deleted
    if ( .not. is_initd(rhs) ) return

    ! Establish the new reference and increment the reference counter.
    lhs%D => rhs%D
    lhs%D%refs_ = rhs%D%refs_ + 1
    
  end subroutine assign_


  !> Check whether two @buds point to the same data (function)
  !!
  !! Same as same_
  !! Returns true if the two passed types are the same
  !! data.
  !!
  !! @param[in] lhs first @bud 
  !! @param[in] rhs second @bud
  !! @return `.true.` if `lhs` and `rhs` point to the same data
  elemental function is_same_(lhs, rhs) result(same)
    type(BUD_TYPE_NAME), intent(in) :: lhs, rhs
    logical :: same
    same = is_initd(lhs) .and. is_initd(rhs)
    if ( .not. same ) return
    ! this checks that the memory location of
    ! the underlying data is the same.
    same = associated(lhs%D, target=rhs%D)
    
  end function is_same_


  !> Query number of references to the @bud (function)
  !!
  !! Get number of times this @bud has been replicated
  !! in the reference counted object.
  !!
  !! @param[in] this @bud
  !! @return refs number of times `this` has been referenced
  elemental function references_(this) result(refs)
    type(BUD_TYPE_NAME), intent(in) :: this
    
    integer :: refs

    if ( is_initd(this) ) then
      refs = this%D%refs_
    else
      refs = 0
    end if
    
  end function references_

  !> @cond ALWAYS_SKIP
  
  ! The remaining procedures are currently
  ! not used by bud
  elemental function id_(this) result(str)
    type(BUD_TYPE_NAME), intent(in) :: this
    
    character(len=BUD_ID_LEN) :: str
    
    str = this%D%id_
    
  end function id_

  subroutine tag_object_(this)
    type(BUD_TYPE_NAME), intent(inout)  :: this

    ! currently we do not tag the types
    
  end subroutine tag_object_

  !> @endcond ALWAYS_SKIP

  !!> @}

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

