mpi_DIR = $(TOP_DIR)/src/mpi
# Append to VPATH
VPATH := $(VPATH):$(mpi_DIR)

MP_OBJECTS = bud_MP_Comm.o
$(MP_OBJECTS:.o=.f90): MP_Comm_routine.inc MP_Comm_routine_interface.inc MP_Comm_routine_interface_generic.inc
mpi_OBJECTS += $(MP_OBJECTS)
bud_MP_Comm.f90: MP_Comm.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

DIST_common = bud_Dist_common.o
mpi_OBJECTS += $(DIST_common)
bud_Dist_common.f90: Dist_common.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

DIST_OBJECTS = \
	bud_Dist1D.o
$(DIST_OBJECTS): $(MP_OBJECTS) $(DIST_common)
$(DIST_OBJECTS:.o=.f90): Dist1D.inc
mpi_OBJECTS += $(DIST_OBJECTS)
bud_Dist1D.f90: Dist1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


DIST_SM_OBJECTS = \
	bud_Dist1D_CSR.o \
	bud_Dist1D_CSR0.o \
	bud_Dist1D_CSC.o \
	bud_Dist1D_CSC0.o
$(DIST_SM_OBJECTS): $(DIST_OBJECTS) $(SM_C) $(SM_R)
$(DIST_SM_OBJECTS:.o=.f90): Dist1D_SM.inc
mpi_OBJECTS += $(DIST_SM_OBJECTS)
bud_Dist1D_CSR.f90: Dist1D_SM_CSR.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Dist1D_CSR0.f90: Dist1D_SM_CSR0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Dist1D_CSC.f90: Dist1D_SM_CSC.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Dist1D_CSC0.f90: Dist1D_SM_CSC0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


DIST_SM_ARR_C = \
	bud_Dist1D_SM_CSC0_Array1D.o \
	bud_Dist1D_SM_CSC0_Array2D.o \
	bud_Dist1D_SM_CSC0_Array3D.o
$(DIST_SM_ARR_C): $(DIST_OBJECTS) $(SM_C) $(ARRAY_OBJECTS)
$(DIST_SM_ARR_C:.o=.f90): Dist1D_SM_Array.inc
$(DIST_SM_ARR_C:.o=.f90): Dist1D_SM_Array_type_.inc
mpi_OBJECTS += $(DIST_SM_ARR_C)
bud_Dist1D_SM_CSC0_Array1D.f90: Dist1D_SM_CSC0_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Dist1D_SM_CSC0_Array2D.f90: _TMP_FILE=Dist1D_SM_CSC0_Array2D.F90
bud_Dist1D_SM_CSC0_Array2D.f90: Dist1D_SM_CSC0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/\([^Dt]\)1D/\12D/g' $(mpi_DIR)/Dist1D_SM_CSC0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_Dist1D_SM_CSC0_Array3D.f90: _TMP_FILE=Dist1D_SM_CSC0_Array3D.F90
bud_Dist1D_SM_CSC0_Array3D.f90: Dist1D_SM_CSC0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/\([^Dt]\)1D/\13D/g' $(mpi_DIR)/Dist1D_SM_CSC0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


DIST_ARR = \
	bud_Dist1D_Array1D.o \
	bud_Dist1D_Array2D.o \
	bud_Dist1D_Array3D.o
$(DIST_ARR): $(DIST_OBJECTS) $(ARRAY_OBJECTS)
$(DIST_ARR:.o=.f90): Dist1D_Array.inc
$(DIST_ARR:.o=.f90): Dist1D_Array_type_.inc
mpi_OBJECTS += $(DIST_ARR)
bud_Dist1D_Array1D.f90: Dist1D_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Dist1D_Array2D.f90: _TMP_FILE=Dist1D_Array2D.F90
bud_Dist1D_Array2D.f90: Dist1D_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/\([^t]\)1D/\12D/g' $(mpi_DIR)/Dist1D_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_Dist1D_Array3D.f90: _TMP_FILE=Dist1D_Array3D.F90
bud_Dist1D_Array3D.f90: Dist1D_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/\([^t]\)1D/\13D/g' $(mpi_DIR)/Dist1D_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


# Define objects that needs compilation
mpi_SOURCES = $(patsubst %.o, %.f90, $(mpi_OBJECTS))
# FPP_SOURCES are those sources that needs deletion upon make clean
mpi_FPP_SOURCES = $(mpi_SOURCES)
OBJECTS += $(mpi_OBJECTS)
SOURCES += $(mpi_SOURCES)
FPP_SOURCES += $(mpi_FPP_SOURCES)
F_MODS  += $(patsubst %.f90, %.mod, $(mpi_SOURCES))


# Ensure we use the correct compiler
$(mpi_FPP_SOURCES): INCLUDES += -I$(src_DIR) -I$(mpi_DIR)


source-mpi: $(mpi_SOURCES)


clean-src-mpi:
	-rm -f $(mpi_FPP_SOURCES)
	-rm -f $(mpi_OBJECTS)
	-rm -f *.mod


clean: clean-src-mpi


# Local Variables:
#  mode: makefile-gmake
# End:
