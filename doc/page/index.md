title: Documentation

# Introduction  {#mainIntroduction}

Implementation of generic interface for creating
reference counted variables.  

The historical origin of buds is rooted in the density-functional theory code
SIESTA and a deeper insight may be found [here](#history).

A recurring problem in large code bases (but also small) is the use of streamlined
data-structures and when and when not to store them in memory.  

This project seaks to solve some of these problems by implementing a template
driven scheme for creating custom derived types with automatic reference counting
(think of reference counting as garbage collection).  
Take for instance this small code snippet which has a module which requires
assigning an object inside the module and later interact with the module. In between
these actions one may want to interact with the object without having to worry about
data duplication.
~~~~~{.f90}
  module m_test
    type(ref_counted), save :: obj
  contains
    subroutine init_obj(calc_obj)
      obj = calc_obj
    end subroutine
    subroutine calc()
      ! ... do stuff with obj
    end subroutine
    subroutine clean_obj()
      call delete(obj)
    end subroutine
  end module
  
  program test
    use m_test
    type(ref_counted) :: obj
    call init_obj(obj)
    ! ... do stuff with obj
    call delete(obj)
    call calc() ! Will work, because obj is not deleted!
    call clean_obj() ! Now it is deleted
  end program
~~~~~	
The above code is runable and works even though the program `test` is
deleting the object `call delete(obj)`; it does not actually delete the data-content.
Only in the scope of the derived type. I.e. the object is deallocated in the program,
but the contained object in the `m_test` module is still allocated. 

This has two advantages, 1) for extremely complex computational patterns where certain
objects are required throughout the code one can easily store the data in those
modules/routines, 2) the data will never be duplicated because reference counting
tracks the number of assignments and deletions.

## Usage {#mainUsage}

buds is implemented with two usages in mind:

1. A template (pre-processor) library which _only_ consists of `.inc` files that provides
the basic functionality of reference counted objects and commands for manipulating
with pre-processors in fortran codes.
2. A basic buds library which provides basic functionality such as arrays, finite-stacks
of arrays.

It is highly recommended that users of buds use the buds library if the provided objects
encompass the wanted features. For a list of the implemented objects see the
<a href="modules.html">Modules page</a> which lists the full documentation of all objects.

### Example Grid bud {#mainUsageExample}

Using the buds library as a template one can easily create a simple reference counted
object which contain a 3D array, and a custom derived type:
~~~~{.f90}
module bud_Grid

#define BUD_TYPE_NAME Grid

  use bud_dArray3D
  use my_derived_type
  
#include "bud_common_declarations.inc"

  type BUD_TYPE_NAME ! This is "Grid"
     type(BUD_TYPE_NAME_), pointer :: D => null()
  
#    include "bud_common_type.inc"
  
  end type 

  type BUD_TYPE_NAME_
     
     type(dArray3D) :: grid
     type(derived_type) :: dt
    
#    include "bud_common_type_.inc"
     
  end type

  interface new
     module procedure new_
  end interface
  public :: new

#define BUD_NO_COPY
#define BUD_NO_PRINT
#define BUD_NO_IO
#include "bud_common.inc"

  subroutine delete_(this)
   type(BUD_TYPE_NAME), intent(inout) :: this

   call delete(this%D%grid)
   call delete(this%D%dt)
   
 end subroutine delete_

 subroutine new_(this, n, dt)
   type(BUD_TYPE_NAME), intent(inout) :: this
   integer, intent(in) :: n(3)
   type(derived_type), intent(in) :: dt

   call initialize(this)

   call new(this%D%grid, n)
   this%D%dt = dt
   
 end subroutine new_

end module bud_Grid
~~~~
With the above code one can use a derived type called `Grid`
which is reference counted and may be used as such.
Any assignment on the above derived type will handle reference counters
and will not duplicate memory.
Also, any deletion of the derived type will _only_ deallocate the memory
in case the reference counter hits 0 (meaning, no other references).

The above is the minimal bud-creation code. It requires the compiler to
use a recent pre-processor (such as `gfortran`) while it may be pre-processed
using `gfortran` or other GNU tools as well.


## Documentation  {#mainDocumentation}

- [Download](#download)

- [Installation instructions](#installInstructions)
  - [Basic installation](#installBasic)
  - [Advanced installation](#installAdvanced)
  - [Compiler settings](#installSettings)
  - [Functionality](#installFunctionality)
  - [Compiler vendors](#installVendor)
  
- [Creating/developing buds](#devel)
  - [Notes for developers](#develNotes)
  - [Developing a new bud](#develNew)

- [buds history](#history)

- [Contributors](#contrib)
  - [Acknowledgements](#contribAcknow)
