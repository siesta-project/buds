The buds library implements a generic template-like interface for creating
reference counted derived types in fortran.  
buds is created with two things in mind:

1. A template library for dynamically creating a _bud_ for one own needs, and
2. A standard bud library with commonly encountered buds.

The template library encompass all necessary pre-processor macros and only requires
a working CPP preprocessor (a recent `gfortran` compiler suffices).

The standard library enables the most commonly encounted data types such as:

- Arrays
- Sparse matrices (CSR, CSR0, CSC, CSC0)
- File
- List (additional functionality compared to Arrays)
- 1D distributions with MPI
- Overloaded MPI communicators
- Distributted arrays
- Distributted sparse matrices

# Download

buds is hosted [here][buds@git] and the buds releases may be found [here][buds-rel].

For information on how to install buds see [here](page/20-install.html#installInstructions).

# License

buds is released under the [MIT][license] license.


<!---
Links to external and internal sites.
-->
[buds@git]: https://github.com/siesta-project/buds
[buds-api]: https://siesta-project.github.io/buds/index.html
[buds-rel]: https://github.com/siesta-project/buds/releases
[issue]: https://github.com/siesta-project/buds/issues
[pr]: https://github.com/siesa-projec/buds/pulls
[license]: https://opensource.org/licenses/MIT

