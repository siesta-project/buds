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
One possible solution is by using reference counted objects (derived
types) which enables an automatic garbage-collection once the object is free
from references.
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
modules/routines, 2) the data will never be duplicated because the reference counting
tracks the number of assignments.

Through the use of this library we try to circumvent some of the more basic
fortran data-types to be reference counted.

I.e. regular arrays, sparse-patterns, lists etc.


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

