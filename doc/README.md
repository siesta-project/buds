# Welcome to buds  {#mainWelcome}

__Welcome to the documentation of buds BUDS_VERSION.__

# Introduction  {#mainIntroduction}

Implementation of generic interface for creating
reference counted variables.  

The creation of buds originated from the SIESTA density functional
theory code which is comprising more than 100.000 lines of code.
A recuring problem was how to silently handle whether certain data-structures
used throughout the code was actually needed or whether they could be
deleted.  
Another problem was a decoherence throughout the code resulting in many similar,
yet distinct, duplications of storage and data-objects.

One possible solution is by using reference counted objects (derived
types) which enables an automatic garbage-collection once the object is free
from references. And having the container being explicitly defined the second
problem is silently solved as well.  
This helps in storing and keeping the needed quantities in the modules and routines
that actually needs them.  
Take for instance this small code snippet which has a module which requires
assigning an object into the module and later interact with the module. In between
these actions one may want to interact with the object without worrying.
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
objects are required throughout the code one may easily store the data in those
modules/routines, 2) the data will never be duplicated because reference counting
tracks the number of assignments/deletions.

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
#define BUD_TYPE_NAME_ Grid_
#define BUD_TYPE_NAME_STR BUD_STR(BUD_TYPE_NAME)

  use bud_dArray3D
  use my_derived_type
  
#include "bud_common_declarations.inc"

  type BUD_TYPE_NAME ! This is "Grid"
     type(BUD_TYPE_NAME_), pointer :: D => null()
  end type 

  type BUD_TYPE_NAME_
     
     type(dArray3D) :: grid
     type(derived_type) :: dt
    
#   include "bud_common_type_.inc"
     
  end type

  interface new
     module procedure new_
  end interface
  public :: new

#define BUD_NO_COPY
#define BUD_NO_PRINT
#include "bud_common.inc"

  subroutine delete_(D)
   type(BUD_TYPE_NAME_), intent(inout) :: D

   call delete(D%grid)
   call delete(D%dt)
   
 end subroutine delete_

 subroutine new_(this,n,dt)
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

- @subpage installInstructions
  - @ref installBasic
  - @ref installAdvanced
  - @ref installSettings
  - @ref installFunctionality
  - @ref installVendor
  
- @subpage download

- @subpage devel
  - @ref develNotes
  - @ref develNew

- @subpage contrib
  - @ref contribAcknow

