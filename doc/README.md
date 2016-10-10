# Welcome to buds  {#mainWelcome}

__Welcome to the documentation of buds BUDS_VERSION.__

# Introduction  {#mainIntroduction}

Implementation of generic interface for creating
reference counted variables.  

The creation of buds was originated from the SIESTA density functional
theory code which is comprising more than 100.000 lines of code.
A recuring problem was how to silently handle whether certain data-structures
used throughout the code was actually needed or whether they could be
deleted.  
One possible solution is via the use of reference counted objects (derived
types) which enables an automatic garbage-collection once the object is free
from references.
Secondly, this method of storing data-structures ensures that no data-dublication
is necessary.

Through the use of this library we try to circumvent some of the more basic
fortran data-types to be reference counted.

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

