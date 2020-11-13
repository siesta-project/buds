  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! The following pre-processor variables are currently used when
  ! included:
  !  - BUD_TYPE_NAME, name of the resulting type
  !  - BUD_TYPE_NAME_, name of the internal data
  !  - BUD_TYPE_NEW, additional name of the `new` command
  !  - BUD_COLL_1, name of the first @bud
  !  - BUD_COLL_2, name of the second @bud
  !  - BUD_COLL_3, name of the third @bud (need not be used)
  !  - BUD_COLL_4, name of the fourth @bud (need not be used)
  !  - BUD_COLL_5, name of the fifth @bud (need not be used)
  !  - BUD_COLL_6, name of the sixth @bud (need not be used)
  !  - BUD_COLL_SAME, 0 if all contained @buds are individual, else 1.

#ifndef BUD_COLL_3
# undef BUD_COLL_4
# undef BUD_COLL_5
# undef BUD_COLL_6
#endif
#ifndef BUD_COLL_4
# undef BUD_COLL_5
# undef BUD_COLL_6
#endif
#ifndef BUD_COLL_5
# undef BUD_COLL_6
#endif

  ! Include the common utilities
#include "bud_utils.inc"

  !> @cond BUD_DOC_INC

  ! We use "collection" as "set" may be confusing as nothing is _set_.
  !> @defgroup bud-coll Collection @bud of several @buds
  !!
  !! A collection holds a certain number of @buds in a strict order.
  !! Hence the elements contained are predefined in the data type and
  !! any access to the stored elements may be accessed by retrieving the
  !! data and subsequently operating on that data-type.\n
  !! The collection data-type may be used to easily group together
  !! associated data from different @buds.
  !!
  !! The stored elements may be accessed either via routine calls:
  !! ~~~{.f90}
  !! call element1(this, A)
  !! ~~~
  !! which will be equivalent to deleting `A` and assigning the
  !! stored element.
  !! One can also access the data via a pointer:
  !! ~~~{.f90}
  !! A => element1_p(this)
  !! ~~~
  !! which does not increase the reference counter for the stored
  !! element. Hence no deletion should be performed.
  !!
  !! @dev_note
  !! For extensions of basic routines you _must_ make a wrapper interface
  !! to allow other routine names to have the same name for different
  !! interfaces.
  !!
  !! One __cannot__ store non-@bud types in this collection.
  !!
  !! Currently this collection only allows up to 5 @buds contained.
  !! This can easily be extended.

  !> @endcond BUD_DOC_INC

  ! There are no pre-processor string
  ! comparisons CPP
  ! Hence this _has_ to be supplied by the user.
#ifndef BUD_COLL_SAME
# define BUD_COLL_SAME 0
#endif


  !> Collection data type
  !!
  !! This collection contains these elements:
  !!  1. BUD_COLL_1
  !!  2. BUD_COLL_2
#ifdef BUD_COLL_3
  !!  3. BUD_COLL_3
#endif
#ifdef BUD_COLL_4
  !!  4. BUD_COLL_4
#endif
#ifdef BUD_COLL_5
  !!  5. BUD_COLL_5
#endif
#ifdef BUD_COLL_6
  !!  6. BUD_COLL_6
#endif
  !!
#if BUD_COLL_SAME == 0
  !! None of the contained elements are the same.
  !! This means that retrieval and storing new elements
  !! becomes easier as the `=` operator is overrided with
  !! interfaces.
#endif
  !!
  !! The collection does not require to have all elements
  !! initialized.
  type BUD_TYPE_NAME

    !> @cond BUD_DEVELOPER

    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

    !> @endcond BUD_DEVELOPER

#   include "bud_common_type.inc"
#if BUD_FORTRAN >= 2003

    !> @name Private procedures
    !> @{
    ! Doxygen needed line

    procedure, private :: new_ => new_
    procedure, private :: new_data_ => new_data_

    !> @}

    !> @iSee #new
    generic, public :: new => new_, new_data_

    !> @iSee #element1
    procedure, public :: element1 => get_elem1_
    !> @iSee #element1_p
    procedure, public :: element1_p => get_elem1p_
    !> @iSee #set_element1
    procedure, public :: set_element1 => set_elem1_

    !> @iSee #element2
    procedure, public :: element2 => get_elem2_
    !> @iSee #element2_p
    procedure, public :: element2_p => get_elem2p_
    !> @iSee #set_element2
    procedure, public :: set_element2 => set_elem2_
#ifdef BUD_COLL_3
    !> @iSee #element3
    procedure, public :: element3 => get_elem3_
    !> @iSee #element3_p
    procedure, public :: element3_p => get_elem3p_
    !> @iSee #set_element3
    procedure, public :: set_element3 => set_elem3_
#endif
#ifdef BUD_COLL_4
    !> @iSee #element4
    procedure, public :: element4 => get_elem4_
    !> @iSee #element4_p
    procedure, public :: element4_p => get_elem4p_
    !> @iSee #set_element4
    procedure, public :: set_element4 => set_elem4_
#endif
#ifdef BUD_COLL_5
    !> @iSee #element5
    procedure, public :: element5 => get_elem5_
    !> @iSee #element5_p
    procedure, public :: element5_p => get_elem5p_
    !> @iSee #set_element5
    procedure, public :: set_element5 => set_elem5_
#endif
#ifdef BUD_COLL_6
    !> @iSee #element6
    procedure, public :: element6 => get_elem6_
    !> @iSee #element6_p
    procedure, public :: element6_p => get_elem6p_
    !> @iSee #set_element6
    procedure, public :: set_element6 => set_elem6_
#endif

#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DEVELOPER

  !> Internal data container for BUD_TYPE_NAME
  !!
  !! @details
  !! Container for the data in the @bud.
  !!
  !! The container handles a collection of up to 6 elements.
  !! Each element is stored via numeric sequences
  !! Hence the first item is `e1`, second is `e2` etc.
  !!
#ifndef BUD_COLL_3
  !! This @bud is a collection of 2 elements:
#endif
#ifndef BUD_COLL_4
  !! This @bud is a collection of 3 elements:
#endif
#ifndef BUD_COLL_5
  !! This @bud is a collection of 4 elements:
#endif
#ifndef BUD_COLL_6
  !! This @bud is a collection of 5 elements:
#else
  !! This @bud is a collection of 6 elements:
#endif
  !! 1. BUD_COLL_1
  !! 2. BUD_COLL_2
#ifdef BUD_COLL_3
  !! 3. BUD_COLL_3
#endif
#ifdef BUD_COLL_4
  !! 4. BUD_COLL_4
#endif
#ifdef BUD_COLL_5
  !! 5. BUD_COLL_5
#endif
#ifdef BUD_COLL_6
  !! 6. BUD_COLL_6
#endif
  type BUD_TYPE_NAME_

    !> First element of collection (BUD_COLL_1)
    type(BUD_COLL_1) :: e1
    !> Second element of collection (BUD_COLL_2)
    type(BUD_COLL_2) :: e2
#ifdef BUD_COLL_3
    !> Third element of collection (BUD_COLL_3)
    type(BUD_COLL_3) :: e3
#endif
#ifdef BUD_COLL_4
    !> Fourth element of collection (BUD_COLL_4)
    type(BUD_COLL_4) :: e4
#endif
#ifdef BUD_COLL_5
    !> Fifth element of collection (BUD_COLL_5)
    type(BUD_COLL_5) :: e5
#endif
#ifdef BUD_COLL_6
    !> Sixth element of collection (BUD_COLL_6)
    type(BUD_COLL_6) :: e6
#endif

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"

  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER


  !> Initialize collection
  !!
  !! Initializes the @bud either with
  !! no data or with every data in the collection.
  !!
  !! This will call `delete` before re-initializing the
  !! data.
  interface new
    module procedure new_
    module procedure new_data_
  end interface
  public :: new



#if BUD_COLL_SAME == 0
  interface assignment(=)
    module procedure BUD_CC3(get_elem,1,_assign_)
    module procedure BUD_CC3(get_elem,2,_assign_)
# ifdef BUD_COLL_3
    module procedure BUD_CC3(get_elem,3,_assign_)
# endif
# ifdef BUD_COLL_4
    module procedure BUD_CC3(get_elem,4,_assign_)
# endif
# ifdef BUD_COLL_5
    module procedure BUD_CC3(get_elem,5,_assign_)
# endif
# ifdef BUD_COLL_6
    module procedure BUD_CC3(get_elem,6,_assign_)
# endif
    module procedure BUD_CC3(set_elem,1,_)
    module procedure BUD_CC3(set_elem,2,_)
# ifdef BUD_COLL_3
    module procedure BUD_CC3(set_elem,3,_)
# endif
# ifdef BUD_COLL_4
    module procedure BUD_CC3(set_elem,4,_)
# endif
# ifdef BUD_COLL_5
    module procedure BUD_CC3(set_elem,5,_)
# endif
# ifdef BUD_COLL_6
    module procedure BUD_CC3(set_elem,6,_)
# endif
  end interface

  interface element
    module procedure BUD_CC3(get_elem,1,_)
    module procedure BUD_CC3(get_elem,2,_)
# ifdef BUD_COLL_3
    module procedure BUD_CC3(get_elem,3,_)
# endif
# ifdef BUD_COLL_4
    module procedure BUD_CC3(get_elem,4,_)
# endif
# ifdef BUD_COLL_5
    module procedure BUD_CC3(get_elem,5,_)
# endif
# ifdef BUD_COLL_6
    module procedure BUD_CC3(get_elem,6,_)
# endif
  end interface

  interface set_element
    module procedure BUD_CC3(set_elem,1,_)
    module procedure BUD_CC3(set_elem,2,_)
# ifdef BUD_COLL_3
    module procedure BUD_CC3(set_elem,3,_)
# endif
# ifdef BUD_COLL_4
    module procedure BUD_CC3(set_elem,4,_)
# endif
# ifdef BUD_COLL_5
    module procedure BUD_CC3(set_elem,5,_)
# endif
# ifdef BUD_COLL_6
    module procedure BUD_CC3(set_elem,6,_)
# endif
  end interface

#endif

  !> Retrieve the first @bud of the collection
  !!
  !! Query the data contained in the BUD_TYPE_NAME
  !! and return the data in `item`.
  !!
  !! If `this` is not initialized, then `item` will be deleted
  !! and an un-initialized @bud will be returned.
  !!
  !! Change the suffix number `1` to `2`, ... to get the
  !! corresponding element in the collection.
  interface element1
    module procedure get_elem1_
  end interface
  public :: element1

  !> Set the first @bud in the collection
  !!
  !! Set the data in the element to the passed item.
  !!
  !! If `this` is not initialized, then nothing will
  !! happen.
  !!
  !! Change the suffix number `1` to `2`, ... to set the
  !! corresponding element in the collection.
  interface set_element1
    module procedure set_elem1_
  end interface
  public :: set_element1

  !> Return a pointer to the first element of the collection
  !!
  !! @note
  !! This does not step the reference counting as it
  !! returns a pointer.
  !!
  !! Change the suffix number `1` to `2`, ... to get the
  !! corresponding element in the collection.
  interface element1_p
    module procedure get_elem1p_
  end interface
  public :: element1_p

  !> @cond BUD_DEVELOPER

  ! we do not document these sub-routines as they are encompassed
  ! by the first documentation

  interface element2
    module procedure get_elem2_
  end interface
  public :: element2
  interface set_element2
    module procedure set_elem2_
  end interface
  public :: set_element2
  interface element2_p
    module procedure get_elem2p_
  end interface
  public :: element2_p

#ifdef BUD_COLL_3
  interface element3
    module procedure get_elem3_
  end interface
  public :: element3
  interface set_element3
    module procedure set_elem3_
  end interface
  public :: set_element3
  interface element3_p
    module procedure get_elem3p_
  end interface
  public :: element3_p
#endif

#ifdef BUD_COLL_4
  interface element4
    module procedure get_elem4_
  end interface
  public :: element4
  interface set_element4
    module procedure set_elem4_
  end interface
  public :: set_element4
  interface element4_p
    module procedure get_elem4p_
  end interface
  public :: element4_p
#endif

#ifdef BUD_COLL_5
  interface element5
    module procedure get_elem5_
  end interface
  public :: element5
  interface set_element5
    module procedure set_elem5_
  end interface
  public :: set_element5
  interface element5_p
    module procedure get_elem5p_
  end interface
  public :: element5_p
#endif

#ifdef BUD_COLL_6
  interface element6
    module procedure get_elem6_
  end interface
  public :: element6
  interface set_element6
    module procedure set_elem6_
  end interface
  public :: set_element6
  interface element6_p
    module procedure get_elem6p_
  end interface
  public :: element6_p
#endif

  !> @endcond BUD_DEVELOPER


  ! all default reference counted stuff
# include "bud_common.inc"


  !> @cond BUD_DEVELOPER

  subroutine delete_(this)
    type(BUD_TYPE_NAME), intent(inout) :: this

    call set_error(this, 0)

    call delete(this%D%e1)
    if ( 0 /= error(this%D%e1) ) &
      call set_error(this, error(this%D%e1))
    call delete(this%D%e2)
    if ( 0 /= error(this%D%e2) ) &
      call set_error(this, error(this%D%e2))
#ifdef BUD_COLL_3
    call delete(this%D%e3)
    if ( 0 /= error(this%D%e3) ) &
      call set_error(this, error(this%D%e3))
#endif
#ifdef BUD_COLL_4
    call delete(this%D%e4)
    if ( 0 /= error(this%D%e4) ) &
      call set_error(this, error(this%D%e4))
#endif
#ifdef BUD_COLL_5
    call delete(this%D%e5)
    if ( 0 /= error(this%D%e5) ) &
      call set_error(this, error(this%D%e5))
#endif
#ifdef BUD_COLL_6
    call delete(this%D%e6)
    if ( 0 /= error(this%D%e6) ) &
      call set_error(this, error(this%D%e6))
#endif

  end subroutine delete_

  !> @endcond BUD_DEVELOPER


  !> @param[in] from the original `bud` which is copied to `to`
  !! @param[inout] to the output `bud` with the full copied data
  subroutine copy_(from, to)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: from
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: to

    call delete(to)
    if ( .not. is_initd(from) ) return

    call initialize(to)
    ! Copy the common data-content
    call common_copy_(from, to)

    call copy(from%D%e1, to%D%e1)
    call copy(from%D%e2, to%D%e2)
#ifdef BUD_COLL_3
    call copy(from%D%e3, to%D%e3)
#endif
#ifdef BUD_COLL_4
    call copy(from%D%e4, to%D%e4)
#endif
#ifdef BUD_COLL_5
    call copy(from%D%e5, to%D%e5)
#endif
#ifdef BUD_COLL_6
    call copy(from%D%e6, to%D%e6)
#endif

  end subroutine copy_

  !> @param[inout] this new initialized @bud
  !! @param[in] A first element in the @bud
  !! @param[in] B second element in the @bud
#ifdef BUD_COLL_3
  !! @param[in] C third element in the @bud
#endif
#ifdef BUD_COLL_4
  !! @param[in] D fourth element in the @bud
#endif
#ifdef BUD_COLL_5
  !! @param[in] E fifth element in the @bud
#endif
#ifdef BUD_COLL_6
  !! @param[in] F sixth element in the @bud
#endif
  subroutine new_data_(this, a, b &
#ifdef BUD_COLL_3
    , c &
#endif
#ifdef BUD_COLL_4
    , d &
#endif
#ifdef BUD_COLL_5
    , e &
#endif
#ifdef BUD_COLL_6
    , f &
#endif
    )
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    type(BUD_COLL_1), intent(in) :: a
    type(BUD_COLL_2), intent(in) :: b
#ifdef BUD_COLL_3
    type(BUD_COLL_3), intent(in) :: c
#endif
#ifdef BUD_COLL_4
    type(BUD_COLL_4), intent(in) :: d
#endif
#ifdef BUD_COLL_5
    type(BUD_COLL_5), intent(in) :: e
#endif
#ifdef BUD_COLL_6
    type(BUD_COLL_6), intent(in) :: f
#endif

    call new(this)

    ! set objects
    this%D%e1 = a
    this%D%e2 = b
#ifdef BUD_COLL_3
    this%D%e3 = c
#endif
#ifdef BUD_COLL_4
    this%D%e4 = d
#endif
#ifdef BUD_COLL_5
    this%D%e5 = e
#endif
#ifdef BUD_COLL_6
    this%D%e6 = f
#endif

  end subroutine new_data_


  !> @param[inout] this initialize @bud with no data
  subroutine new_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    call initialize(this)

  end subroutine new_


  ! Create data-retrieval routines
# define BUD_COLL_NUM 1
# define BUD_COLL_CURRENT BUD_COLL_1
# include "bud_collection_element.inc"
# define BUD_COLL_NUM 2
# define BUD_COLL_CURRENT BUD_COLL_2
# include "bud_collection_element.inc"
#ifdef BUD_COLL_3
# define BUD_COLL_NUM 3
# define BUD_COLL_CURRENT BUD_COLL_3
# include "bud_collection_element.inc"
#endif
#ifdef BUD_COLL_4
# define BUD_COLL_NUM 4
# define BUD_COLL_CURRENT BUD_COLL_4
# include "bud_collection_element.inc"
#endif
#ifdef BUD_COLL_5
# define BUD_COLL_NUM 5
# define BUD_COLL_CURRENT BUD_COLL_5
# include "bud_collection_element.inc"
#endif
#ifdef BUD_COLL_6
# define BUD_COLL_NUM 6
# define BUD_COLL_CURRENT BUD_COLL_6
# include "bud_collection_element.inc"
#endif


#ifndef BUD_NO_IO
# ifndef BUD_IO_OVERLOAD

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be written
  subroutine write_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return
    if ( .not. is_initd(this) ) return

    call write(f, this%D%e1)
    call write(f, this%D%e2)
#ifdef BUD_COLL_3
    call write(f, this%D%e3)
#endif
#ifdef BUD_COLL_4
    call write(f, this%D%e4)
#endif
#ifdef BUD_COLL_5
    call write(f, this%D%e5)
#endif
#ifdef BUD_COLL_6
    call write(f, this%D%e6)
#endif

  end subroutine write_

  !> @param[in] f a file (bud_File)
  !! @param[in] this collection @bud to be readed
  subroutine read_(f, this)
    use BUD_CC2(BUD_MOD,_File)
    BUD_CLASS( BUD_CC2(BUD_TYPE,File) ), intent(inout) :: f
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    ! If file is not opened, return immediately
    if ( .not. is_open(f) ) return

    ! prepare for reading
    call initialize(this)

    call read(f, this%D%e1)
    call read(f, this%D%e2)
#ifdef BUD_COLL_3
    call read(f, this%D%e3)
#endif
#ifdef BUD_COLL_4
    call read(f, this%D%e4)
#endif
#ifdef BUD_COLL_5
    call read(f, this%D%e5)
#endif
#ifdef BUD_COLL_6
    call read(f, this%D%e6)
#endif

  end subroutine read_

# endif
#endif

  !> @param[in] this data type
  !! @param[in] info @opt=BUD_TYPE_NAME_STR additional information printed
  !! @param[in] indent @opt=1 possible indentation of printed statement
  subroutine print_(this, info, indent)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    character(len=*), intent(in), optional :: info
    integer, intent(in), optional :: indent
    integer :: lindent

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

    write(fmt, '(a,i0,a)') '(t',lindent,',3a)'
    lindent = lindent + 2 ! step indentation
    write(*,fmt) "<<", trim(name), " coll>"
    call print(this%D%e1, indent = lindent)
    call print(this%D%e2, indent = lindent)
#ifdef BUD_COLL_3
    call print(this%D%e3, indent = lindent)
#endif
#ifdef BUD_COLL_4
    call print(this%D%e4, indent = lindent)
#endif
#ifdef BUD_COLL_5
    call print(this%D%e5, indent = lindent)
#endif
#ifdef BUD_COLL_6
    call print(this%D%e6, indent = lindent)
#endif
    lindent = lindent - 2 ! go back to requested indentation
    write(fmt, '(a,i0,a)') '(t',lindent,',a,i0,a)'
    write(*,fmt) " <coll-refs: ", references(this), ">>"

  end subroutine print_


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
