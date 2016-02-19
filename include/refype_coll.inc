  ! @@LICENSE@@ see Copyright notice in the top-directory
  
  ! Include the common utilities
#include "refype_utils.inc"

#define _R_MOD_NAME_STR _R_XSTR(_R_MOD_NAME)
#ifndef _R_TYPE_NAME_
#  define _R_TYPE_NAME_ _R_CC2(_R_TYPE_NAME,_)
#endif
#ifndef _R_TYPE_NAME_STR
#  define _R_TYPE_NAME_STR _R_STR(_R_TYPE_NAME)
#endif

#include "refype_common_declarations.inc"
  
  ! We use "collection" as "set" may be confusing as nothing is _set_.
  !> @defgroup refype-coll Reference type of a collection
  !!
  !! @details
  !! The implementation details of a collection of reference counted types.
  !! This type implements a common container for N>1 @rypes.
  !!
  !! @dev_note
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !!
  !! One __cannot__ store non-@rype types in this collection.
  !!
  !! Currently this collection only allows up to 5 @rypes contained.
  !! This can easily be extended.
  !!
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - __R_TYPE_NAME, name of the resulting type
  !!  - __R_TYPE_NAME_, name of the internal data
  !!  - __R_COLL_A, name of the first @rype
  !!  - __R_COLL_B, name of the second @rype
  !!  - __R_COLL_C, name of the third @rype (need not be used)
  !!  - __R_COLL_D, name of the fourth @rype (need not be used)
  !!  - __R_COLL_E, name of the fifth @rype (need not be used)
  !!
  !! @acknowledge
  !! These declarations are community inspired from the FLIBS
  !! project by Arjen Markus as well as the PyF95 project.
  !! The initial draft of this library has been created by Alberto Garcia.

  !> @rype container for _R_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @rype.
  !!
  !! The container handles a collection up to 5 elements.
  !! Each element is stored via alphabetic sequences
  !! Hence the first item is `A`, second is `B` etc.
  !!
#ifndef _R_COLL_C
  !! This @rype is a collection of 2 elements:
#else
#ifndef _R_COLL_D
  !! This @rype is a collection of 3 elements:
#else
#ifndef _R_COLL_E
  !! This @rype is a collection of 4 elements:
#else
  !! This @rype is a collection of 5 elements:
#endif
#endif
#endif
  !! 1. _R_COLL_A
  !! 2. _R_COLL_B
#ifdef _R_COLL_C
  !! 3. _R_COLL_C
#ifdef _R_COLL_D
  !! 4. _R_COLL_D
#ifdef _R_COLL_E
  !! 5. _R_COLL_E
#endif
#endif
#endif
  type _R_TYPE_NAME_
    ! Consistent data in the reference counted object
#   include "refype_common_type.inc"
    
    !> First element of collection
    type(_R_COLL_A) :: A
    !> Second element of collection
    type(_R_COLL_B) :: B
#ifdef _R_COLL_C
    !> Third element of collection
    type(_R_COLL_C) :: C
#ifdef _R_COLL_D
    !> Fourth element of collection
    type(_R_COLL_D) :: D
#ifdef _R_COLL_E
    !> Fifth element of collection
    type(_R_COLL_E) :: E
#endif
#endif
#endif
  end type _R_TYPE_NAME_

  ! Exported procedures and functions
  ! (in addition to init, delete, and assignment(=) )
  public :: new, _R_TYPE_NEW

  public :: print
  
  public :: elema, elempa, felempa
  public :: elemb, elempb, felempb
#ifdef _R_COLL_C
  public :: elemc, elempc, felempc
#ifdef _R_COLL_D
  public :: elemd, elempd, felempd
#ifdef _R_COLL_E
  public :: eleme, elempe, felempe
#endif
#endif
#endif

  !> Create a new @rype. This will be allocated with 1 reference.
  interface new
    module procedure new_
  end interface new
  !> Create a new @rype. This will be allocated with 1 reference.
  interface _R_TYPE_NEW
    module procedure new_
  end interface _R_TYPE_NEW

  !> Retrieve the first element of the collection (routine)
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface elema
    module procedure elema_
  end interface elema
  !> Point to the first element of the collection (routine)
  !!
  !! This routine does not step the reference counting as it
  !! is a pointer.
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface elempa
    module procedure elempa_
  end interface elempa
  !> Point to the first element of the collection (function)
  !!
  !! This routine does not step the reference counting as it
  !! is a pointer.
  !!
  !! Change the suffix letter `A` to `B`, ... to get the
  !! corresponding element in the collection.
  interface felempa
    module procedure felempa_
  end interface felempa

  !> @cond ALWAYS_SKIP

  ! we do not document these sub-routines as they are encompassed
  ! by elema etc.
  
  interface elemb
    module procedure elemb_
  end interface elemb
  interface elempb
    module procedure elempb_
  end interface elempb
  interface felempb
    module procedure felempb_
  end interface felempb

#ifdef _R_COLL_C
  interface elemc
    module procedure elemc_
  end interface elemc
  interface elempc
    module procedure elempc_
  end interface elempc
  interface felempc
    module procedure felempc_
  end interface felempc
  
#ifdef _R_COLL_D
  interface elemd
    module procedure elemd_
  end interface elemd
  interface elempd
    module procedure elempd_
  end interface elempd
  interface felempd
    module procedure felempd_
  end interface felempd

#ifdef _R_COLL_E
  interface eleme
    module procedure eleme_
  end interface eleme
  interface elempe
    module procedure elempe_
  end interface elempe
  interface felempe
    module procedure felempe_
  end interface felempe
#endif
#endif
#endif

  !> @endcond ALWAYS_SKIP

  !> Print, to std-out, some basic information of the data-container
  interface print
    module procedure print_
  end interface print
  
  ! all default reference counted stuff
# include "refype_common.inc"
  
  ! This routine takes care of deleting the 
  ! types storage

  pure subroutine delete_data(data)
    type(_R_TYPE_NAME_), intent(inout) :: data
    call delete(data%a)
    call delete(data%b)
#ifdef _R_COLL_C
    call delete(data%c)
#ifdef _R_COLL_D
    call delete(data%d)
#ifdef _R_COLL_E
    call delete(data%e)
#endif
#endif
#endif
  end subroutine delete_data

  !> Internal initialization of data container
  !!
  !! Initializes the data handler to a new data handler.
  !!
  !! If called on a previously allocated data scope this
  !! will clear the reference (@isee delete).
  !! Subsequently the type container will be allocated
  !! and the reference counted data will be 1.
  !!
  !! @param[inout] this initialized @rype
  !! @param[in] A first element in the @rype
  !! @param[in] B second element in the @rype
#ifdef _R_COLL_C
  !! @param[in] C third element in the @rype
#ifdef _R_COLL_D
  !! @param[in] D fourth element in the @rype
#ifdef _R_COLL_E
  !! @param[in] E fifth element in the @rype
#endif
#endif
#endif
  !!
  !! @author Nick R. Papior (2016, February)
  subroutine new_(this, a, b &
#ifdef _R_COLL_C
    , c &
#ifdef _R_COLL_D
    , d &
#ifdef _R_COLL_E
    , e &
#endif
#endif
#endif
    )
    type(_R_TYPE_NAME), intent(inout) :: this
    type(_R_COLL_A), intent(in) :: a
    type(_R_COLL_B), intent(in) :: b
#ifdef _R_COLL_C
    type(_R_COLL_C), intent(in) :: c
#ifdef _R_COLL_D
    type(_R_COLL_D), intent(in) :: d
#ifdef _R_COLL_E
    type(_R_COLL_E), intent(in) :: e
#endif
#endif
#endif

    call init(this)

    ! set objects
    this%data%a = a
    this%data%b = b
#ifdef _R_COLL_C
    this%data%c = c
#ifdef _R_COLL_D
    this%data%d = d
#ifdef _R_COLL_E
    this%data%e = e
#endif
#endif
#endif
    call tag_object_(this)
    
  end subroutine new_

  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !!
  !! @param[in] this data type
  !! @param[in] info @opt=_R_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  !!
  !! @author Nick R. Papior (2016, February)
  subroutine print_(this, info, indent)
    type(_R_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent
    integer :: lindent
    
    ! 4-byte variable
    character(len=32) :: fmt
    character(len=256) :: name

    name = _R_TYPE_NAME_STR
    if ( present(info) ) name = info
    lindent = 1
    if ( present(indent) ) lindent = indent
    
    write(fmt, '(a,i0,a)') '(t',lindent,',2a)'

    if ( .not. is_initd(this) ) then
      write(*,fmt) trim(name), " not initialized."
      return
    end if

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<<", trim(name), ">"
    call print(this%data%a, indent = lindent)
    call print(this%data%b, indent = lindent)
#ifdef _R_COLL_C
    call print(this%data%c, indent = lindent)
#ifdef _R_COLL_D
    call print(this%data%d, indent = lindent)
#ifdef _R_COLL_E
    call print(this%data%e, indent = lindent)
#endif
#endif
#endif
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " < refs: ", frefs(this), ">>"

  end subroutine print_

  ! Create data-retrieval routines
# define _R_COLL_LETTER A
# define _R_COLL_CURRENT _R_COLL_A
# include "refype_coll_elem.inc"
# define _R_COLL_LETTER B
# define _R_COLL_CURRENT _R_COLL_B
# include "refype_coll_elem.inc"
#ifdef _R_COLL_C
# define _R_COLL_LETTER C
# define _R_COLL_CURRENT _R_COLL_C
# include "refype_coll_elem.inc"
#ifdef _R_COLL_D
# define _R_COLL_LETTER D
# define _R_COLL_CURRENT _R_COLL_D
# include "refype_coll_elem.inc"
#ifdef _R_COLL_E
# define _R_COLL_LETTER E
# define _R_COLL_CURRENT _R_COLL_E
# include "refype_coll_elem.inc"
#endif
#endif
#endif

#undef _R_MOD_NAME
#undef _R_TYPE_NAME
#undef _R_TYPE_NAME_
#undef _R_TYPE_NAME_STR
#undef _R_TYPE_NEW
#undef _R_COLL_A
#undef _R_COLL_B
#undef _R_COLL_C
#undef _R_COLL_D
#undef _R_COLL_E

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

