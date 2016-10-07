mpi_DIR = $(TOP_DIR)/src/mpi
# Append to VPATH
VPATH := $(VPATH):$(mpi_DIR)

MP_OBJECTS = bud_MP_Comm.o
$(MP_OBJECTS:.o=.f90): MP_Comm_routine.inc MP_Comm_routine_interface.inc MP_Comm_routine_interface_generic.inc
mpi_OBJECTS += $(MP_OBJECTS)
bud_MP_Comm.f90: MP_Comm.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

DIST_generic = bud_Dist_generic.o
mpi_OBJECTS += $(DIST_generic)
bud_Dist_generic.f90: Dist_generic.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

DIST_OBJECTS = \
	bud_Dist1D.o
$(DIST_OBJECTS): $(MP_OBJECTS) $(DIST_generic)
$(DIST_OBJECTS:.o=.f90): Dist1D.inc
mpi_OBJECTS += $(DIST_OBJECTS)
bud_Dist1D.f90: Dist1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


# Define objects that needs compilation
mpi_SOURCES = $(patsubst %.o, %.f90, $(mpi_OBJECTS))
# FPP_SOURCES are those sources that needs deletion upon make clean
mpi_FPP_SOURCES = $(mpi_SOURCES)
ifneq ($(MPI),0)
OBJECTS += $(mpi_OBJECTS)
SOURCES += $(mpi_SOURCES)
FPP_SOURCES += $(mpi_FPP_SOURCES)
F_MODS  += $(patsubst %.f90, %.mod, $(mpi_SOURCES))
endif


# Ensure we use the correct compiler
$(mpi_FPP_SOURCES): INCLUDES += -I$(mpi_DIR)


source-mpi: $(mpi_SOURCES)


clean-src-mpi:
	-rm -f $(mpi_FPP_SOURCES)
	-rm -f $(mpi_OBJECTS)
	-rm -f *.mod


clean: clean-src-mpi


# Local Variables:
#  mode: makefile-gmake
# End:
