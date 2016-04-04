! @@LICENSE@@ see Copyright notice in the top-directory

! This extension currently uses these pre-processor flags:
!  - BUD_MOD_NAME name of the module
!  - BUD_TYPE_NAME name of the public type
!  - BUD_TYPE_NAME_ name of the internal data pointer
!  - BUD_TYPE_NAME_STR the routine name in "string" format (for IO)
!  - BUD_TYPE_NEW the routine name for creating a new type
  
#include "bud_utils.inc"


! Define default variable for the file
# define BUD_MOD_NAME BUD_CC3(BUD_MOD,_,File)
# define BUD_TYPE_NAME BUD_CC2(BUD_TYPE,File)
# define BUD_TYPE_NEW BUD_CC3(BUD_NEW,_,File)
  
#define BUD_MOD_NAME_STR BUD_XSTR(BUD_MOD_NAME)
#define BUD_TYPE_NAME_ BUD_CC2(BUD_TYPE_NAME,_)
#define BUD_TYPE_NAME_STR BUD_XSTR(BUD_TYPE_NAME)

!> @defgroup file File
!! @ingroup bud-intrinsic
!!
!! Perform file operations such as open/close/delete etc.
!!
!! Exposes functionality regarding files.
!! It allows interaction with the files in a standard way
!! by opening, closing, deleting files in a simple and clean
!! interface.
!! @{
module BUD_MOD_NAME

  ! This *MUST* be the first statement
  ! Common parameters and variables used
# include "bud_common_declarations.inc"

  !> Maximum path length for the filenames
  integer, parameter :: FILE_NAME_LEN = 256

  
  !> File @bud
  !!
  !! This generic file type enables opening/closing/deleting
  !! etc. using a single file handle.
  !!
  !! It may also be used to retrieve a new file-unit.
  type BUD_TYPE_NAME
    !> Stored pointer which contains the reference counting etc.
    type(BUD_TYPE_NAME_), pointer :: D => null()

#if BUD_FORTRAN >= 2003
  contains
#   include "bud_common_type.inc"

    !> @iSee new
    procedure, public :: new => new_

    !> @iSee open
    procedure, public :: open => open_

    !> @iSee close
    procedure, public :: close => close_

    !> @iSee rewind
    procedure, public :: rewind => rewind_

    !> @iSee backspace
    procedure, public :: backspace => backspace_

    !> @iSee filename
    procedure, public :: file => filename_
    !> @iSee filename
    procedure, public :: name => filename_
    !> @iSee filename
    procedure, public :: filename => filename_

    !> @iSee get_unit
    procedure, public :: unit => get_unit_

    !> @iSee unopened_unit
    procedure, public, nopass :: unopened_unit => unopened_unit_

    !> @iSee is_open
    procedure, public :: is_open => is_open_

    !> @iSee exists
    procedure, public :: exists => exists_

    !> @iSee file_delete
    procedure, public :: file_delete => file_delete_

    !> @iSee get_stat
    procedure, public :: stat => get_stat_

#endif
  end type BUD_TYPE_NAME


  !> @cond BUD_DEVELOPER
  
  !> @bud container for BUD_TYPE_NAME
  !!
  !! Contains information regarding a file.
  !!
  type BUD_TYPE_NAME_

    !> File name/path for this object
    character(len=FILE_NAME_LEN) :: file = ' '

    !> Unit for the file (negative if non-opened)
    integer :: unit = -1

    !> Status, which may be queried by the user
    integer :: stat = 0

    ! Consistent data in the reference counted object
#   include "bud_common_type_.inc"
    
  end type BUD_TYPE_NAME_

  !> @endcond BUD_DEVELOPER

  

  !> Create a new file @bud
  !!
  !! Initializes the file with the associated
  !! filename.
  !!
  !! @note
  !! This will _not_ open the file.
  interface new
    module procedure new_
  end interface
  public :: new
  !> @iSee `new`
  interface BUD_CC3(BUD_NEW,_,File)
    module procedure new_
  end interface
  public :: BUD_CC3(BUD_NEW,_,File)


  !> Open file via object
  !!
  !! Equivalent to `open` statement.
  !!
  !! If the file is already opened, the file will be
  !! closed and subsequently re-opened with the passed
  !! options.
  !!
  !! Once a file is opened several procedures may
  !! be used to query information, in case one does not
  !! know the options used for opening the file.
  interface open
    module procedure open_
  end interface
  public :: open

  !> Close file via object
  !!
  !! This transforms the file into an un-opened
  !! state and several of the procedures will return
  !! default values in this case.
  interface close
    module procedure close_
  end interface
  public :: close

  !> Rewind file to the beginning of the file
  !!
  !! This is equivalent to the `rewind` statement.
  interface rewind
    module procedure rewind_
  end interface
  public :: rewind

  !> Backspace the file
  !!
  !! Move back in the file records.
  interface backspace
    module procedure backspace_
  end interface
  public :: backspace


  !> Retrieve new unused unit (global function)
  !!
  !! Will always return an integer > 1000 with a unit
  !! that is currently not in use.
  interface unopened_unit
    module procedure unopened_unit_
  end interface
  public :: unopened_unit
  
  !> Query filename of the file @bud
  !!
  !! This *MUST* only be called on an initialized
  !! @bud.
  !! The function will return a trimmed filename
  !! which is only possible by using `len_trim`.
  interface filename
    module procedure filename_
  end interface
  public :: filename
  
  !> Unit of opened file @bud
  !! 
  !! Retrieve the unit of the file in case it is
  !! opened.  
  !! If the file is not opened, `-1` will be returned.
  interface get_unit
    module procedure get_unit_
  end interface
  public :: get_unit

  !> @iSee #get_unit
  interface unit
    module procedure get_unit_
  end interface
  public :: unit

  !> Query whether file is open
  !!
  !! Checks whether the file is opened by `open`.
  interface is_open
    module procedure is_open_
  end interface
  public :: is_open

  !> Query whether file exists 
  !!
  !! Checks whether the file exists on disk.
  !! This is independent of `is_open` and may
  !! be called on unopened files.
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

  !> Status of the latest action with iostat
  !!
  !! This may be to check the `iostat` of the last internal
  !! routine.
  !!
  !! Calling this immediately twice in a row will always return
  !! 0 on the second call.
  interface get_stat
    module procedure get_stat_
  end interface
  public :: get_stat

  
  !> Delete file on disk
  !!
  !! Perform an actual delete of the file.
  !! If the file is opened it will be closed afterwards.
  !!
  !! The file @bud will not be deleted, nor the filename.
  interface file_delete
    module procedure file_delete_
  end interface
  public :: file_delete

  !> Print, to std-out, some basic information of the data-container
  !!
  !! Print out XML-like information regarding the data-container.
  !!
  !! @wanted
  !! Retrieval function of the string that represents the data.
  !! This will enable the parent program to show it in the way it wants.
  interface print
    module procedure print_
  end interface
  public :: print
  
  ! Include common data routines
  ! Note that 'CONTAINS' is present in this include file.
  ! the delete_data ensures a closed file
  ! Hence we *must* not use elemental
#define BUD_DELETE_NOELEMENTAL
# include "bud_common.inc"
#undef BUD_DELETE_NOELEMENTAL

  !> @cond BUD_DEVELOPER


  !> Internal routine for cleaning up the data container.
  !!
  !! @dev_note
  !! This routine is only used internally to clean-up
  !! any data in the type.
  !! Should never be made public.
  !!
  !! @param[inout] this contained data to be deleted
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


  !> @param[inout] this force the status to be 0
  subroutine stat_reset_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    if ( is_initd(this) ) this%D%stat = 0
  end subroutine stat_reset_

  !> @endcond BUD_DEVELOPER


  !> @param[inout] this file @bud
  !! @param[in] filename the path `this` will contain
  subroutine new_(this, filename)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    character(len=*), intent(in) :: filename

    ! initialize object
    call initialize(this)

    this%D%file = filename
    
  end subroutine new_


  !> @return unit currently un-used
  function unopened_unit_() result(unit)
    integer :: unit
    logical :: is_open

    unit = 999
    is_open = .true.
    
    do while ( is_open )
      unit = unit + 1
      inquire( unit , opened = is_open )
    end do
    
  end function unopened_unit_
  
  !> @param[in] this query filename from this file @bud
  !! @return filename of the file @bud
  pure function filename_(this) result(filename)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    
    character(len=len_trim(this%D%file)) :: filename

    filename = trim(this%D%file)

  end function filename_
  
  !> @param[in] this file @bud
  !! @return unit of the opened file (-1 if unopened)
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
  
  !> @param[in] this file @bud
  !! @return `.true.` if the file is opened
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

    ! restore unit in case it really is not opened...
    ! this should only happen if the user closes the file
    ! without using the `close` function.
    if ( .not. is ) this%D%unit = -1

  end function is_open_


  !> @param[in] this file @bud
  !! @return `.true.` if the file exists on disk
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


  !> @param[in] this file @bud
  !! @return `.true.` if the file is opened in access=DIRECT mode
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

  
  !> @param[in] this file @bud
  !! @return `.true.` if the file is opened in access=SEQUENTIAL mode
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


  !> @param[in] this file @bud
  !! @return `.true.` if the file is opened in form=FORMATTED mode
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


  !> @param[in] this file @bud
  !! @return `.true.` if the file is opened in form=UNFORMATTED mode
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


  !> @param[in] this file @bud
  !! @return status from the last operation
  function get_stat_(this) result(stat)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    integer :: stat

    if ( is_open(this) ) then
      stat = this%D%stat
      call stat_reset_(this)
    else
      stat = 0
    end if
    
  end function get_stat_

  
  !> @param[inout] this file @bud
  !! @param[in] D (dummy argument which should _NEVER_ be used, forces explicit interface usage)
  !! @param[in] form @opt='FORMATTED' format of opened file
  !! @param[in] access @opt='SEQUENTIAL' file access pattern
  !! @param[in] action @opt='READWRITE' file R/W access
  !! @param[in] status @opt='OLD' file-existance 
  subroutine open_(this, D, form, access, action, status)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    character(len=*), intent(in), optional :: D, form, access, action, status

    character(len=32) :: lform, laccess, laction, lstatus

    if ( .not. is_initd(this) ) return

    ! ensure that the file is closed
    call close(this)

    ! process options
    lform = 'FORMATTED'
    if ( present(form) ) lform = trim(form)
    laccess = 'SEQUENTIAL'
    if ( present(access) ) laccess = trim(access)
    laction = 'READWRITE'
    if ( present(action) ) laction = trim(action)
    lstatus = 'OLD'
    if ( present(status) ) lstatus = trim(status)

    ! Get new unit
    this%D%unit = unopened_unit()

    open( this%D%unit, file=trim(this%D%file), &
      form = lform, access = laccess, action = laction, &
      status = lstatus, &
      iostat = this%D%stat )
    
  end subroutine open_

  !> @param[inout] this file @bud
  subroutine close_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    if ( .not. is_open(this) ) then
      call stat_reset_(this)
      return
    end if

    ! Close file-unit
    close( this%D%unit, &
      iostat = this%D%stat )

    this%D%unit = -1
    
  end subroutine close_

  !> @param[inout] this file @bud
  subroutine rewind_(this)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this

    if ( .not. is_open(this) ) return
      
    rewind( this%D%unit, &
      iostat = this%D%stat )
    
  end subroutine rewind_

  !> @param[inout] this file @bud
  !! @param[in] n @opt=1 number of times to backspace
  subroutine backspace_(this, n)
    BUD_CLASS(BUD_TYPE_NAME), intent(inout) :: this
    integer, intent(in), optional :: n
    integer :: i

    if ( .not. is_open(this) ) then
      call stat_reset_(this)
      return
    end if
    
    if ( present(n) ) then
      
      do i = 1, n
        
        backspace( this%D%unit, &
          iostat = this%D%stat )
        
        if ( this%D%stat /= 0 ) return
      end do
      
    else
      
      backspace( this%D%unit, &
        iostat = this%D%stat )
      
    end if
    
  end subroutine backspace_

  !> @param[inout] this file @bud
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
      
      unit = unopened_unit()
      ! it does not matter how it is opened
      open( unit, file = this%D%file )
      close( unit, STATUS = 'DELETE', &
        iostat = this%D%stat )
      
    end if
    
  end subroutine file_delete_

  
  !> @param[in] this data type
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
      ", refs: ", references(this), ">"

    call stat_reset_(this)

  end subroutine print_

end module
!> @}


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

