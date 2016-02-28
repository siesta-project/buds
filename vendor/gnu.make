# This contains default compiler options
# for the Intel vendor compiler suite.
V_VENDOR = gnu

# Compiler binaries
V_CC = gcc
V_CXX = g++
V_FC = gfortran

V_MPICC = mpicc
V_MPICXX = mpic++
V_MPIFC = mpif90

# Flag for output directory of modules
V_FC_MODDIR = -J

# default compiler flags
V_CFLAGS =
V_CXXFLAGS =
V_FCFLAGS =

# default compiler flags for debugging
V_FLAGS_debug = -g -fbacktrace -fbounds-check
V_CFLAGS_debug = $(V_FLAGS_debug)
V_CXXFLAGS_debug = $(V_FLAGS_debug)
V_FCFLAGS_debug = $(V_FLAGS_debug)

# These are gcc compiler options
V_FLAGS_weak = -O1 -funroll-loops
V_CFLAGS_weak = $(V_FLAGS_weak)
V_CXXFLAGS_weak = $(V_FLAGS_weak)
V_FCFLAGS_weak = $(V_FLAGS_weak)

V_FLAGS_medium = -O2 -funroll-loops -ftree-vectorize -fprefetch-loop-arrays
V_CFLAGS_medium = $(V_FLAGS_medium)
V_CXXFLAGS_medium = $(V_FLAGS_medium)
V_FCFLAGS_medium = $(V_FLAGS_medium)

V_FLAGS_hard = -O3 -funroll-loops -ftree-vectorize -fprefetch-loop-arrays -fgraphite -fipa-icf -fipa-pure-const -fschedule-fusion -fselective-scheduling
V_CFLAGS_hard = $(V_FLAGS_hard)
V_CXXFLAGS_hard = $(V_FLAGS_hard)
V_FCFLAGS_hard = $(V_FLAGS_hard)


# Local Variables:
#  mode: makefile-gmake
# End:
