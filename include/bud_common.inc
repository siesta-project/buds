  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME, name of the resulting type
  !  - BUD_TYPE_NAME_, contained data structure (as a pointer)
  !  - BUD_ERROR, if this is not defined there will be
  !    no error mechanism for catching erroneous calls.

  ! Include the common utilities
# include "bud_utils.inc"

  !> @cond BUD_DOC_INC

  !> @defgroup bud-common Global functions for @buds
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
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !! I.e. you may not make _any_ routine/function public. Only interfaces.
  !!
  !! The routines defined in this file may not be defined in any
  !! of the reference counted type-modules. Doing so will create
  !! name-clashes.
  !!
  !! @{

  !> @endcond BUD_DOC_INC


  public :: BUD_TYPE_NAME
  private :: BUD_TYPE_NAME_

  !> @name Intrinsic @bud routines
  !! These routines are common for nearly all @buds.
  !>@{
  ! needed comment

  ! We force `delete_data` to be private.
  ! This will prohibit a mistaken public designation of the
  ! routine.
  ! Internal routine for clearing the data container in the @bud
  private :: delete_data


  !> Assignment of @bud to another @bud
  !!
  !! `lhs` is deleted before assignment.
  !!
  !! Make `lhs` contain the same data as in `rhs`.
  !! Internally a pointer to the same data is created and the reference
  !! counter will be incremented.
  !!
  !! `lhs` will be un-associated if `rhs` is un-associated.
  interface assignment(=)
    module procedure assign_
  end interface
  public :: assignment(=)

  !> Initialization of data container
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  interface initialize
    module procedure initialize_
  end interface
  public :: initialize

  !> Query whether @bud is associated with any data
  !!
  !! Returns `.true.` if the @bud is already containing data.\n
  !! Typically the `new` routine will call `initialize`.
  interface is_initialized
    module procedure is_initialized_
  end interface
  public :: is_initialized

  !> @iSee is_initialized
  interface initialized
    module procedure is_initialized_
  end interface
  public :: initialized

  !> @iSee is_initialized
  interface is_initd
    module procedure is_initialized_
  end interface
  public :: is_initd

  !> Check whether two @buds point to the same data (function)
  !!
  !! Returns `.true.` if the two passed @buds are the same
  !! data.
  interface is_same
    module procedure is_same_
  end interface
  public :: is_same

  !> @iSee #is_same
  interface same
    module procedure is_same_
  end interface
  public :: same

#ifndef BUD_NO_DELETE
  !> Deletes the @bud by decreasing references to the underlying data
  !!
  !! Number of references will be decremented and the data associated
  !! with this object will be nullified.
  !!
  !! If the number of references of the associated data is 0
  !! after decremention, the associated data will be
  !! deleted. Else this @bud will return an un-associated @bud
  !! and any other @buds that point to the same data will retain the data.
  interface delete
    module procedure delete_
  end interface
  public :: delete
#endif

#ifndef BUD_NO_NULLIFY
  
  !> Nullifies the current @bud
  !!
  !! This will not decrement the reference counter but
  !! enables to _clear_ the data in the @bud.
  !!
  !! @note
  !! This routine should be used with _extreme_ care and very
  !! rarely.
  interface nullify
    module procedure nullify_
  end interface
  public :: nullify
  
#endif

  !> Query number of references to the @bud
  !!
  !! Get number of times this @bud has been assigned
  !! in the reference counted object.
  interface references
    module procedure references_
  end interface
  public :: references

  !> @iSee #references
  interface refs
    module procedure references_
  end interface
  public :: refs

  !>>@}

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
!#   warning "BUD_ERROR not defined! Expect sudden crashes without warnings!"
#   define _BUD_ERROR_H
# endif
#endif


contains

  
  ! contains in non-modules freak doxygen out...
  !> @endcond ALWAYS_SKIP

  !> @name Intrinsic @bud routines
  !! Generic routines for all @buds
  !>@{
  ! needed comment to separate the next interface

  !> @param[inout] this @bud that is initialized
  subroutine initialize_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

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

  !> @param[in] this check if this @bud is initialized
  !! @return `.true.` if it is associated, else `.false.`
  pure function is_initialized_(this) result(init)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    logical :: init

    init = associated(this%D)
    
  end function is_initialized_

  !> @param[in] lhs first @bud 
  !! @param[in] rhs second @bud
  !! @return `.true.` if `lhs` and `rhs` point to the same data
  elemental function is_same_(lhs, rhs) result(same)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: lhs, rhs
    logical :: same
    
    same = is_initd(lhs) .and. is_initd(rhs)
    if ( .not. same ) return
    ! this checks that the memory location of
    ! the underlying data is the same.
    same = associated(lhs%D, target=rhs%D)
    
  end function is_same_

  
#ifndef BUD_NO_DELETE
  
  !> @param[inout] this returned @bud is un-initialized with no data associated
#ifndef BUD_DELETE_NOELEMENTAL
  elemental subroutine delete_(this)
#else
  subroutine delete_(this)
#endif
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

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
  !> @param[inout] this nullify references from this @bud
  elemental subroutine nullify_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    
    if (.not. is_initd(this) ) return
    
    nullify(this%D)
    
  end subroutine nullify_
#endif
  
  !> @param[inout] lhs contains `rhs` after exit
  !! @param[in] rhs copy the containing element @bud
  subroutine assign_(lhs, rhs)

    ! No copying of data is involved, simply an increment of the
    ! reference counter.
    
    ! IMPORTANT NOTE: Assignment must take the form of a subroutine, and
    ! not of a function, since the "inout" intent is essential. One has
    ! to clean up "lhs" before making it point to the same place as
    ! "rhs".  In a function, the intrinsic "out" intent for "lhs"
    ! will destroy any prior information.

    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: lhs
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: rhs

    ! Delete to remove the current reference of "this"
    call delete(lhs)
    
    ! If RHS is empty, return immediately
    ! Note that LHS is still deleted
    if ( .not. is_initd(rhs) ) return

    ! Establish the new reference and increment the reference counter.
    lhs%D => rhs%D
    lhs%D%refs_ = rhs%D%refs_ + 1
    
  end subroutine assign_


  !> @param[in] this reference counted @bud
  !! @return number of times `this` has been referenced
  elemental function references_(this) result(refs)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
    integer :: refs

    if ( is_initd(this) ) then
      refs = this%D%refs_
    else
      refs = 0
    end if
    
  end function references_

  
  !> @cond BUD_DEVELOPER
  
  ! The remaining procedures are currently
  ! not used by public buds
  elemental function id_(this) result(str)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
    character(len=BUD_ID_LEN) :: str
    
    str = this%D%id_
    
  end function id_

  subroutine tag_object_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout)  :: this

    ! currently we do not tag the types
    
  end subroutine tag_object_

  !> @endcond BUD_DEVELOPER

  !> @cond BUD_DOC_INC
  
  !> @}

  !> @endcond BUD_DOC_INC

  !>>@}

  
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

