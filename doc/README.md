# Welcome to buds  {#mainWelcome}

__Welcome to the documentation of buds BUDS_VERSION.__

# Introduction  {#mainIntroduction}

Implementation of generic interface for creating
reference counted variables.  
Reference counted variables enables garbage-collection
for variables with the added benefit of never having to
dublicate data.

This is useful when using data-types/objects which you do not know
when to delete because of unknown depedencies.

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


## Generic interface  {#budsGenericInterface}

The basic buds-library adds certain files which are useful for creating
custom reference counted types using a standard. buds may be installed
in the most basic format which _only_ adds the interfaces for creating
custom data types. buds may be used in this minimal form.


## The standard bud library  {#budsStandardLibrary}

In addition to the generic interfaces buds implements a selection of
data-types which have been encountered in the development of a density
functional theory code, [SIESTA][siesta]. All data-types are relying on the
generic interfaces.

You are encouraged to scour the documentation to find possible compatible
reference counted data types that you may find useful.

## Contributing  {#budsContributing}

We encourage contribution, be it data-type suggestions and/or pull-requests.

Please see [how to contribute](@ref Development).



[siesta]: http://departments.icmab.es/leem/siesta/ "SIESTA"
