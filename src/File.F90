  ! @@LICENSE@@ see Copyright notice in the top-directory
  
#include "bud_utils.inc"
  
  ! Define default variable for the file
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,File)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,File)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,File)
  
#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)

module BUD_MOD_NAME

  !> BUD_MOD_NAME documentation
  !!
  !! @details
  !! This module creates a file type which is reference counted
  !! and may be used in a variety of environments.
  !!
  !! @dev_note
  !! The following pre-processor variables are currently used when
  !! included:
  !!  - _BUD_MOD_NAME of the module
  !!  - _BUD_TYPE_NAME of the type
  !!  - _BUD_TYPE_NAME_, internal data pointer of the type
  !!  - _BUD_TYPE_NAME_STR, the routine name in "string" format (for IO)
  !!  - _BUD_TYPE_NEW, the routine name for creating a new type
  !! If you add any new dependencies be sure to undefine the variable
  !! at the bottom of this file.
  
  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  integer, parameter :: FILENAME_LEN = 256

  
  !> @bud variable
  !!
  !! Handler for the pointer type stored
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> Open file via object
    procedure, public :: open => open_

    !> Close file via object
    procedure, public :: close => close_

    !> Filename of object
    procedure, public :: file => filename_
    procedure, public :: filename => filename_

    !> Unit of object
    procedure, public :: unit => get_unit_

    !> Query whether file is open
    procedure, public :: is_open => is_open_

    !> Query whether file exists 
    procedure, public :: exists => exists_

    !> Delete file on-disk
    procedure, public :: file_delete => file_delete_

#endif
  end type BUD_TYPE_NAME

  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains information regarding a file.
  !!
  type BUD_TYPE_NAME_

    !> File name/path for this object
    character(len=FILENAME_LEN) :: file = ' '

    !> Unit for the file (negative if non-opened)
    integer :: unit = -1

    !> Status counter which may be queried by the user
    integer :: stat = 0

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"
    
  end type BUD_TYPE_NAME_


  !> Create new @bud instances
  interface new
    module procedure new_
  end interface
  public :: new
  !> Create new @bud instances
  interface BUD_CC3(BUD_NEW,_,File)
    module procedure new_
  end interface
  public :: BUD_CC3(BUD_NEW,_,File)


  !> @cond BUD_INTERNAL
  ! Internal interfaces
  interface new_unit
    module procedure new_unit_
  end interface
  !> @endcond BUD_INTERNAL
  

  !> Get filename
  interface filename
    module procedure filename_
  end interface
  public :: filename
  
  !> Query unit for the opened file
  interface get_unit
    module procedure get_unit_
  end interface
  public :: get_unit

  !> Query whether file is opened
  interface is_open
    module procedure is_open_
  end interface
  public :: is_open

  !> Query whether file exists (need not be opened)
  interface exists
    module procedure exists_
  end interface
  public :: exists

  !> Query file opened in DIRECT mode
  interface is_direct
    module procedure is_direct_
  end interface
  public :: is_direct

  !> Query file opened in SEQUENTIAL mode
  interface is_sequential
    module procedure is_sequential_
  end interface
  public :: is_sequential

  !> Query file opened in FORMATTED mode
  interface is_formatted
    module procedure is_formatted_
  end interface
  public :: is_formatted
  
  !> Query file opened in UNFORMATTED mode
  interface is_unformatted
    module procedure is_unformatted_
  end interface
  public :: is_unformatted

  !> Query file status from last operation
  interface get_stat
    module procedure get_stat_
  end interface
  public :: get_stat

  !> Open file with settings
  interface open
    module procedure open_
  end interface
  public :: open

  !> Close file
  interface close
    module procedure close_
  end interface
  public :: close


  !> Delete (on-disk) file
  !!
  !! Sadly the `delete` function is used
  !! to delete the object and hence the prefix
  !! `file_` is required in this instance.
  interface file_delete
    module procedure file_delete_
  end interface
  public :: file_delete

  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
  ! the delete_data ensures a closed file
  ! Hence we *must* not use elemental
#define BUD_DELETE_NOELEMENTAL
# include "bud_common.inc"
#undef BUD_DELETE_NOELEMENTAL

  !> @cond BUD_INTERNAL

  
  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  subroutine delete_data(this)
    type(BUD_TYPE_NAME_), intent(inout) :: this
    integer :: stat
    
    logical :: is_open

    inquire( this%unit , opened = is_open )

    ! Only close if the file is open
    if ( is_open ) then
      close( this%unit, iostat = stat )
    end if

    this%file = ' '
    this%unit = -1
    this%stat = 0
    
  end subroutine delete_data

  !> @endcond BUD_INTERNAL


  !> Create a new file object
  !!
  !! Initializes the file-object with the associated
  !! file-name.
  !!
  !! One is required to perform a subsequent `call open`
  !! to interact with the file.
  !!
  !! @param[inout] this new file-object
  subroutine new_(this, filename)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    character(len=*), intent(in) :: filename

    ! initialize object
    call initialize(this)

    this%D%file = filename
    
  end subroutine new_
  
  
  !> Internal routine for re-setting the status indicator
  subroutine stat_reset_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    if ( is_initd(this) ) this%D%stat = 0
  end subroutine stat_reset_


  !> Internal routine to get an un-used unit for new file
  function new_unit_() result(unit)
    integer :: unit
    logical :: is_open

    unit = 999
    is_open = .true.
    
    do while ( is_open )
      unit = unit + 1
      inquire( unit , opened = is_open )
    end do
    
  end function new_unit_
  
  !> Query the filename
  pure function filename_(this) result(filename)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
    character(len=len_trim(this%D%file)) :: filename

    filename = this%D%file

  end function filename_
  
  !> Query the unit number, if file is opened
  function get_unit_(this) result(unit)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    
    integer :: unit

    if ( .not. is_initd(this) ) then
      unit = -1
      return
    end if

    call stat_reset_(this)
    unit = this%D%unit

  end function get_unit_
  
  
  !> Query if the file is opened
  function is_open_(this) result(is)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    logical :: is

    if ( .not. is_initd(this) ) then
      is = .false.
      return
    end if

    if ( this%D%unit < 0 ) then
      call stat_reset_(this)
      is = .false.
      return
    end if
    
    inquire( this%D%unit, opened = is, &
      iostat = this%D%stat)

  end function is_open_

  
  !> Query if the file exists
  function exists_(this) result(exist)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    logical :: exist

    if ( is_initd(this) ) then
      inquire( file = this%D%file, exist = exist, &
        iostat = this%D%stat)
    else
      exist = .false.
    end if
    
  end function exists_

  
  !> Query if the file is opened in DIRECT mode
  function is_direct_(this) result(direct)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    character(len=10) :: dir
    logical :: direct

    if ( is_open(this) ) then
      inquire( this%D%unit, direct = dir, &
        iostat = this%D%stat)

      direct = dir == 'YES'

    else

      call stat_reset_(this)
      direct = .false.
      
    end if
    
  end function is_direct_


  !> Query if the file is opened in SEQUENTIAL mode
  function is_sequential_(this) result(sequential)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    character(len=10) :: seq
    logical :: sequential

    if ( is_open(this) ) then
      inquire( this%D%unit, sequential = seq, &
        iostat = this%D%stat)

      sequential = seq == 'YES'

    else

      call stat_reset_(this)
      sequential = .false.
      
    end if
    
  end function is_sequential_

  
  !> Query if the file is opened in FORMATTED mode
  function is_formatted_(this) result(formatted)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    character(len=10) :: form
    logical :: formatted

    if ( is_open(this) ) then
      inquire( this%D%unit, formatted = form, &
        iostat = this%D%stat)

      formatted = form == 'YES'

    else

      call stat_reset_(this)
      formatted = .false.
      
    end if
    
  end function is_formatted_


  !> Query if the file is opened in UNFORMATTED mode
  function is_unformatted_(this) result(unformatted)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    character(len=10) :: unform
    logical :: unformatted

    if ( is_open(this) ) then
      inquire( this%D%unit, unformatted = unform, &
        iostat = this%D%stat)

      unformatted = unform == 'YES'

    else

      call stat_reset_(this)
      unformatted = .false.
      
    end if
    
  end function is_unformatted_


  !> Query if the file is opened in UNFORMATTED mode
  function get_stat_(this) result(stat)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer :: stat

    if ( is_open(this) ) then
      stat = this%D%stat
    else
      stat = 0
    end if
    
  end function get_stat_

  
  !> Open file, file must be initialized
  !!
  !! If the file is already opened, this is a no-op.
  !!
  !! @param[inout] this file-object to be opened
  !! @param[in] form @opt='FORMATTED' format of opened file
  !! @param[in] access @opt='SEQUENTIAL' file access pattern
  !! @param[in] action @opt='READWRITE' file R/W access
  !! @param[in] status @opt='OLD' file-existance 
  subroutine open_(this, D, form, access, action, status)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    character(len=*), intent(in), optional :: D, form, access, action, status

    character(len=32) :: lform, laccess, laction, lstatus

    if ( .not. is_initd(this) ) return
    if ( is_open(this) ) return

    lform = 'FORMATTED'
    if ( present(form) ) lform = trim(form)
    laccess = 'SEQUENTIAL'
    if ( present(access) ) laccess = trim(access)
    laction = 'READWRITE'
    if ( present(action) ) laction = trim(action)
    lstatus = 'OLD'
    if ( present(status) ) lstatus = trim(status)

    ! Get new unit
    this%D%unit = new_unit()

    open( this%D%unit, file=trim(this%D%file), &
      form = lform, access = laccess, action = laction, &
      status = lstatus, &
      iostat = this%D%stat )
    
  end subroutine open_

  !> Close file, if opened
  subroutine close_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    if ( .not. is_open(this) ) return

    ! Close file-unit
    close( this%D%unit, &
      iostat = this%D%stat )

    this%D%unit = -1
    
  end subroutine close_
  
  !> Delete file on disk
  !!
  !! Perform an actual delete of the file.
  !! If the file is opened it will be closed afterwards.
  !!
  !! This object will not be deleted
  !!
  !! @param[inout] this file handle to be deleted
  subroutine file_delete_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer :: unit

    ! immediately return if the object
    ! has not been created.
    if ( .not. is_initd(this) ) return
    
    if ( is_open(this) ) then
      
      close( this%D%unit, STATUS = 'DELETE', &
        iostat = this%D%stat )

      ! reset status as not opened
      this%D%unit = -1
      
    else if ( exists(this) ) then
      
      unit = new_unit()
      ! it does not matter how it is opened
      open( unit, file = this%D%file )
      close( unit, STATUS = 'DELETE', &
        iostat = this%D%stat )
      
    end if
    
  end subroutine file_delete_

  
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
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
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

    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',4a,3(a,l1),a,i0,a)'
    
    write(*,fmt) "<", trim(name), &
      " file=", filename(this), &
      ", open=", is_open(this), &
      ", formatted=", is_formatted(this), &
      ", exists=", exists(this), &
      ", refs: ", get_refs(this), ">"

  end subroutine print_

end module

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

