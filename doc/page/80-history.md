title: History

# Origin of buds {#history}

The creation of buds originated from the [SIESTA][siesta] density functional
theory code which is comprising more than 100.000 lines of code.
A recuring problem was how to silently handle certain data-structures
used throughout the code. It became problematic to infer in each section
which data-structures were superfluous (could be deleted) or whether they should
be retained in memory because of dependencies.

Secondly, due to large contributions from a wide variety of people many of the
implemented routines used similar data-structures with many different aliases (variable
names). Largely this was caused by passing data-structures through as combined arguments.
For instance the sparse matrices were stored as individual arrays (not derived types):

    :::fortran 
    integer :: nrows          ! number of rows
    integer :: row_pointer(:) ! pointer in `columns` for the `i`th row
	integer :: nrow(:)        ! number of non-zero elements per row
	integer :: nnz            ! total number of non-zero elements
	integer :: columns(:)     ! column indices

which resulted in routine calls such as this:

    :::fortran
    call sub(nrows, row_pointer, nrow, nnz, columns, ...)

however when combining all the sparse matrix information to a derived type with reference
counting one 1) tracks the usage of the sparse matrix, and 2) the interface may be changed
to:

    :::fortran
    call sub(sparse_pattern, ...)

This does not, per see, solve the problem of forcing a standard naming scheme _in_ the
routines, but it does provide readability because the extraction of the sparse-matrix
information _has_ to be performed in the routine `sub` and thus one does not need to
look at the interface for the routine _and_ the calling routine.


[siesta]: http://departments.icmab.es/leem/siesta/
