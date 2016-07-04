mpi_DIR = $(TOP_DIR)/src/mpi
mpi_VDIR = $(VPATH)/$(mpi_DIR)


DIST_OBJECTS = \
	bud_Dist1D.o
DIST_OBJECTS := $(addprefix $(mpi_DIR)/,$(DIST_OBJECTS))
$(DIST_OBJECTS:.o=.f90): $(mpi_DIR)/Dist1D.inc
$(DIST_OBJECTS): $(mpi_DIR)/Dist1D.inc
mpi_OBJECTS += $(DIST_OBJECTS)
$(mpi_DIR)/bud_Dist1D.f90: $(mpi_DIR)/Dist1D.F90
	$(CPP) $(INCLUDES) $(CPPFLAGS) $< > $@


# Define objects that needs compilation
mpi_SOURCES = $(patsubst %.o, %.f90, $(mpi_OBJECTS))
# CPP_SOURCES are those sources that needs deletion upon make clean
mpi_CPP_SOURCES = $(mpi_SOURCES)
SOURCES += $(mpi_SOURCES)
CPP_SOURCES += $(mpi_CPP_SOURCES)
F_MODS  += $(patsubst %.f90, %.mod, $(mpi_SOURCES))
OBJECTS += $(mpi_OBJECTS)


# Ensure we use the correct compiler
$(mpi_OBJECTS): INCLUDES += -I$(mpi_DIR)
$(mpi_OBJECTS): FC  = $(MPIFC)
$(mpi_OBJECTS): CC  = $(MPICC)
$(mpi_OBJECTS): CXX = $(MPICXX)

# Local Variables:
#  mode: makefile-gmake
# End:
