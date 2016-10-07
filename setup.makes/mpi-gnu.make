# Enable MPI
MPI = 1

# Define Fortran compiler and flags
FC = gfortran
FFLAGS = -O2
MPIFC = mpif90



# Define the installation prefix of the library and
# include files.
PREFIX = $(HOME)/libraries/buds
