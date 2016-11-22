  ! @@LICENSE@@ see Copyright notice in the top-directory

  !> @defgroup sm Sparse matrix
  !! @ingroup bud-intrinsic
  !!
  !! @brief
  !! Sparse matrix for reduced storage formats of dense patterns
  !! with many zero elements.
  !!
  !! A sparse matrix is denoted by a two-dimensional format based on
  !! rows and columns.
  !! There are several different types of sparse matrixs, and here we
  !! implement a couple of variants.
  !! The most used ones are probably the CSR and CSC patterns.


  !> Return number of non-zero elements stored in the sparse matrix
  !!
  !! This corresponds to the `size` of the `col` array
  !! contained in the object.
  interface nonzeros
    module procedure nonzeros_
  end interface
  public :: nonzeros

  !> Number of rows in the sparse matrix
  interface rows
    module procedure rows_
  end interface
  public :: rows

  !> Number of columns in the sparse matrix
  interface columns
    module procedure columns_
  end interface
  public :: columns


  !> Dimensions of the sparse matrix
  !!
  !! This routine will return 1 of 3 possible scenarios:
  !!  1. `size(this)` will return `nonzeros(this)`
  !!  2. `size(this, 1)` will return `rows(this)`
  !!  3. `size(this, 2)` will return `columns(this)`
  interface size
    module procedure size_
  end interface
  public :: size


  !> Retrieve access to sparse matrix variables via pointers.
  !!
  !! Manually access the sparsity pattern by retrieving
  !! pointers and interact externally with the
  !! sparsity pattern.
  !!
  !! All arguments are optional and *must* be accessed by
  !! keyword.
  !!
  !! @note
  !! Currently this is the only way to interact with the
  !! sparse matrix.
  !!
  !! @todo
  !! Add error checking for `D` parameter.
  interface attach
    module procedure attach_
  end interface
  public :: attach


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
# include "bud_common.inc"


  !> @param[in] this the sparse matrix
  !! @return number of non-zero elements in the pattern
  elemental function nonzeros_(this) result(nnzs)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nnzs
    nnzs = this%D%nz
  end function nonzeros_


  !> @param[in] this the sparse matrix
  !! @return number of rows in the pattern
  elemental function rows_(this) result(rows)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: rows
    rows = this%D%nr
  end function rows_


  !> @param[in] this the sparse matrix
  !! @return number of columns in the pattern
  elemental function columns_(this) result(cols)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: cols
    cols = this%D%nc
  end function columns_


  !> @param[in] this the sparse matrix
  !> @param[in] dim @opt=0 the dimension query
  !! @return size of the sparse matrix dependent on value of `dim`: 0=nonzeros, 1=rows, 2=columns.
  elemental function size_(this, dim) result(size)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer, intent(in), optional :: dim
    integer :: size

    if ( present(dim) ) then
      if ( dim == 1 ) then
        size = this%D%nr
      else if ( dim == 2 ) then
        size = this%D%nc
      else if ( dim == 0 ) then
        size = this%D%nz
      else
        size = -1
      end if
    else
      size = this%D%nz
    end if

  end function size_

  !> @param[in] this sparse matrix
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

    ! Create fmt
    write(fmt, '(a,i0,a)') '(t',lindent,',3a,4(i0,a))'

    write(*,fmt) "<", trim(name), " rows=", this%D%nr, &
      ", cols=", this%D%nc, ", nnzs=",this%D%nz, &
      ", refs: ", references(this), ">"

  end subroutine print_


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
