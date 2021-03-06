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
    module procedure common_assign_
  end interface
  public :: assignment(=)
  private :: common_assign_

  !> Initialization of data container
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  interface initialize
    module procedure common_initialize_
  end interface
  public :: initialize
  private :: common_initialize_

  !> Query whether @bud is associated with any data
  !!
  !! Returns `.true.` if the @bud is already containing data.\n
  !! Typically the `new` routine will call `initialize`.
  interface is_initialized
    module procedure common_is_initialized_
  end interface
  public :: is_initialized
  private :: common_is_initialized_

  !> @iSee is_initialized
  interface initialized
    module procedure common_is_initialized_
  end interface
  public :: initialized

  !> @iSee is_initialized
  interface is_initd
    module procedure common_is_initialized_
  end interface
  public :: is_initd

  !> Check whether two @buds point to the same data (function)
  !!
  !! Returns `.true.` if the two passed @buds are the same
  !! data.
  interface is_same
    module procedure common_is_same_
  end interface
  public :: is_same
  private :: common_is_same_


  !> @iSee is_same
  interface same
    module procedure common_is_same_
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
    module procedure common_delete_
  end interface
  public :: delete
  private :: common_delete_
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
    module procedure common_nullify_
  end interface
  public :: nullify
  private :: common_nullify_
#endif

#ifndef BUD_NO_COPY
  !> Copies the @bud by creating an entirely new object with the same data
  !!
  !! This will for all purposes be regarded as a new instance.
  !!
  interface copy
    module procedure copy_
  end interface
  public :: copy
  private :: common_copy_
#endif

#ifndef BUD_NO_PRINT
  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in any way
  !! it wants.
  interface print
    module procedure print_
  end interface
  public :: print
#endif

#ifndef BUD_NO_IO

  !> Read @bud information from a file (already opened)
  !!
  !! Reads the information as `write` would have written it.
  interface read
    module procedure read_
  end interface
  public :: read

  !> Write the @bud information to a file (already opened)
  !!
  !! The file can also be re-read using the `read` function
  interface write
    module procedure write_
  end interface
  public :: write

#endif

  !> Query number of references to the @bud
  !!
  !! Get number of times this @bud has been assigned
  !! in the reference counted object.
  interface references
    module procedure common_references_
  end interface
  public :: references
  private :: common_references_

  !> @iSee references
  interface refs
    module procedure common_references_
  end interface
  public :: refs

  !> Set the error to some value.
  interface set_error
    module procedure common_set_error_is_
    module procedure common_set_error_ii_
    module procedure common_set_error_il_
  end interface
  public :: set_error
  private :: common_set_error_is_
  private :: common_set_error_ii_
  private :: common_set_error_il_


  !> Query the latest routine which may have produced an error.
  !!
  !! Return the error value of the latest routine which issued an error.
  !! If the error-value is non-zero an error occurred and one should track
  !! the value.
  !! If it is zero, there has been no error and one may safely proceed.
  interface error
    module procedure common_error_
  end interface
  public :: error
  private :: common_error_

  !>>@}

contains


  !> @name Intrinsic @bud routines
  !! Generic routines for all @buds
  !>@{
  ! needed comment to separate the next interface

  !> Copies the common data types in the buds
  !!
  !! Whenever some variable is added in bud_common_type_.inc
  !! it should be copied in this routine.
  !!
  !! @note this routine is local and should only be called
  !! from routines inside.
  subroutine common_copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call set_error(to, error(from))

  end subroutine common_copy_



  !> @param[inout] this @bud that is initialized
  subroutine common_initialize_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer :: error

    ! First, remove the current reference
    call delete(this)

    ! Allocate fresh storage
    allocate(this%D, stat=error)

    ! Ensure the error is set
    call set_error(this, error)
    if ( error /= 0 ) return

    ! Set the initial reference count
    this%D%refs_ = 1

    ! Tag the object (always)
    call common_tag_object_(this)

  end subroutine common_initialize_

  !> @param[in] this check if this @bud is initialized
  !! @return `.true.` if it is associated, else `.false.`
  pure function common_is_initialized_(this) result(init)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    logical :: init

    init = associated(this%D)

  end function common_is_initialized_

  !> @param[in] lhs first @bud
  !! @param[in] rhs second @bud
  !! @return `.true.` if `lhs` and `rhs` point to the same data
  elemental function common_is_same_(lhs, rhs) result(same)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: lhs, rhs
    logical :: same

    same = is_initd(lhs) .and. is_initd(rhs)
    if ( .not. same ) return
    ! this checks that the memory location of
    ! the underlying data is the same.
    same = associated(lhs%D, target=rhs%D)

  end function common_is_same_


#ifndef BUD_NO_DELETE

  !> @param[inout] this returned @bud is un-initialized with no data associated
  subroutine common_delete_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer :: error

    call set_error(this, 0)
    if (.not. is_initd(this) ) return

    this%D%refs_ = this%D%refs_ - 1

    if ( 0 == this%D%refs_ ) then

      ! Safe to delete the data now.
      ! Use the routine provided for this specific
      ! type to clean up any internal structure

      call delete_(this)

      ! Deallocate the currently referenced storage
      deallocate(this%D, stat=error)
      call set_error(this, error)

    end if

    ! This is important to use the correct initialized functions
    nullify(this%D)

  end subroutine common_delete_

#endif


#ifndef BUD_NO_NULLIFY
  !> @param[inout] this nullify references from this @bud (without stepping the counter)
  elemental subroutine common_nullify_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    if (.not. is_initd(this) ) return

    nullify(this%D)

  end subroutine common_nullify_
#endif

  !> @param[inout] lhs contains `rhs` after exit
  !! @param[in] rhs copy the containing element @bud
  subroutine common_assign_(lhs, rhs)

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

  end subroutine common_assign_


  !> @param[in] this reference counted @bud
  !! @return number of times `this` has been referenced
  elemental function common_references_(this) result(refs)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer :: refs

    if ( is_initd(this) ) then
      refs = this%D%refs_
    else
      refs = 0
    end if

  end function common_references_


  !> @param[in] this reference counted @bud
  !! @return current error from last call
  elemental function common_error_(this) result(error)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    integer :: error

    if ( is_initd(this) ) then
      error = this%error_
    else
      error = 0
    end if

  end function common_error_

  !> @param[inout] this reference counted @bud
  !! @param[in] error set the error to this value
  elemental subroutine common_set_error_is_(this, error)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(is_), intent(in) :: error
    this%error_ = error
  end subroutine common_set_error_is_
  !> @param[inout] this reference counted @bud
  !! @param[in] error set the error to this value
  elemental subroutine common_set_error_ii_(this, error)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(ii_), intent(in) :: error
    this%error_ = error
  end subroutine common_set_error_ii_
  !> @param[inout] this reference counted @bud
  !! @param[in] error set the error to this value
  elemental subroutine common_set_error_il_(this, error)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer(il_), intent(in) :: error
    this%error_ = error
  end subroutine common_set_error_il_


  !> @cond BUD_DEVELOPER

  ! The remaining procedures are currently
  ! not used by public buds
  elemental function common_id_(this) result(str)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    character(len=BUD_ID_LEn) :: str

    str = this%D%id_

  end function common_id_

  subroutine common_tag_object_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout)  :: this

    ! currently we do not tag the types

  end subroutine common_tag_object_

  !> @endcond BUD_DEVELOPER

  !> @cond BUD_DOC_INC

  !> @}

  !> @endcond BUD_DOC_INC

  !>>@}


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

