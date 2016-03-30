MPI_DIR = $(TOP_DIR)/src/mpi

# Include the top-dir include directory
INC += -I$(MPI_DIR)


DIST_OBJS = \
	mbud_Dist1D.o
DIST_OBJS := $(addprefix $(MPI_DIR)/,$(DIST_OBJS))
$(DIST_OBJS:.o=.f90): $(MPI_DIR)/Dist1D.inc
$(DIST_OBJS): $(MPI_DIR)/Dist1D.inc
MPI_OBJS += $(DIST_OBJS)
$(MPI_DIR)/mbud_Dist1D.f90: $(MPI_DIR)/Dist1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@


# Populate the correct OBJS file
OBJS += $(MPI_OBJS)

# Ensure we use the correct compiler
$(MPI_OBJS): FC  = $(MPIFC)
$(MPI_OBJS): CC  = $(MPICC)
$(MPI_OBJS): CXX = $(MPICXX)

# Local Variables:
#  mode: makefile-gmake
# End:
