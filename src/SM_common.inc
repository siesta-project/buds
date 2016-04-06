  ! @@LICENSE@@ see Copyright notice in the top-directory

  ! Constants defined to be used as error messages
  !

  !> Errors with a negative number refers to the `-err` element
  !!
  !! This may mean that the entry is beyond the matrix size.

  !> Definition of no errors in the call
  integer, parameter, public :: SM_NONE = 0

  !> Signals two or more equivalent sparse elements
  integer, parameter, public :: SM_ENTRY_MULTIPLE = 1

  !> The input argument that is wrong by `err-SM_INPUT`
  !!
  !! This constant is an offset in the error number that
  !! refers to the input parameter `err - SM_INPUT`.
  !! I.e. if `err = SM_INPUT + 2`, the second argument
  !! is erroneous.
  integer, parameter, public :: SM_INPUT = 1000


    !> Return number of non-zero elements stored in the sparse matrix
  !!
  !! This corresponds to the `size` of the `col` array
  !! contained in the object.
  interface nonzeros
    module procedure nonzeros_
  end interface
  public :: nonzeros

  !> Number of rows in matrix
  interface mat_rows
    module procedure mat_rows_
  end interface 
  public :: mat_rows
  
  !> Number of columns in matrix
  interface mat_cols
    module procedure mat_cols_
  end interface 
  public :: mat_cols


  !> Retrieve access to sparse pattern variables via pointers.
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
  !! sparse pattern.
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
  !! @return number of non-zero elements in the matrix
  elemental function nonzeros_(this) result(nnzs)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: nnzs
    nnzs = this%D%nz
  end function nonzeros_
  
  
  !> @param[in] this the sparse matrix
  !! @return number of rows in the matrix
  elemental function mat_rows_(this) result(rows)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: rows
    rows = this%D%nr
  end function mat_rows_

  
  !> @param[in] this the sparse matrix
  !! @return number of columns in the matrix
  elemental function mat_cols_(this) result(cols)
    BUD_CLASS(BUD_TYPE_NAME), intent(in) :: this
    integer :: cols
    cols = this%D%nc
  end function mat_cols_

  
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
