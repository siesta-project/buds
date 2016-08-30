mpi_DIR = $(TOP_DIR)/src/mpi
# Append to VPATH
VPATH := $(VPATH):$(mpi_DIR)


DIST_OBJECTS = \
	bud_Dist1D.o
$(DIST_OBJECTS:.o=.f90): Dist1D.inc
mpi_OBJECTS += $(DIST_OBJECTS)
bud_Dist1D.f90: Dist1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


# Define objects that needs compilation
mpi_SOURCES = $(patsubst %.o, %.f90, $(mpi_OBJECTS))
# FPP_SOURCES are those sources that needs deletion upon make clean
mpi_FPP_SOURCES = $(mpi_SOURCES)
SOURCES += $(mpi_SOURCES)
FPP_SOURCES += $(mpi_FPP_SOURCES)
F_MODS  += $(patsubst %.f90, %.mod, $(mpi_SOURCES))
OBJECTS += $(mpi_OBJECTS)


# Ensure we use the correct compiler
$(mpi_FPP_SOURCES): INCLUDES += -I$(mpi_DIR)
$(mpi_OBJECTS): FC  = $(MPIFC)
$(mpi_OBJECTS): CC  = $(MPICC)
$(mpi_OBJECTS): CXX = $(MPICXX)

# Local Variables:
#  mode: makefile-gmake
# End:
