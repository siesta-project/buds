# Include user-defined setup
-include ../setup.make

# Common makefile stuff:
ifndef _INCLUDED_common
  include ../Makefile.common
endif

ifndef _INCLUDED_default
  include ../Makefile.default
endif

# Include the top-dir include directory
override INC += -I../include

ARRAY_OBJS = \
	mryp_Array1D.o \
	mryp_Array2D.o \
	mryp_Array3D.o
mryp_Array1D.f90: Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_Array2D.f90: Array2D.F90
	$(CPP) $(INC) $< > $@
mryp_Array3D.f90: Array3D.F90
	$(CPP) $(INC) $< > $@

C_ARRAY_OBJS = \
	mryp_C2_Array1D.o \
	mryp_C2_Array2D.o \
	mryp_C2_Array3D.o
mryp_C2_Array1D.f90: C2_Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_C2_Array2D.f90: C2_Array2D.F90
	$(CPP) $(INC) $< > $@
mryp_C2_Array3D.f90: C2_Array3D.F90
	$(CPP) $(INC) $< > $@

FS_ARRAY_OBJS = \
	mryp_FS_Array1D.o \
	mryp_FS_Array2D.o \
	mryp_FS_Array3D.o
mryp_FS_Array1D.f90: FS_Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_Array2D.f90: FS_Array2D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_Array3D.f90: FS_Array3D.F90
	$(CPP) $(INC) $< > $@

FS_C_ARRAY_OBJS = \
	mryp_FS_C2_Array1D.o \
	mryp_FS_C2_Array2D.o \
	mryp_FS_C2_Array3D.o
mryp_FS_C2_Array1D.f90: FS_C2_Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_C2_Array2D.f90: FS_C2_Array2D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_C2_Array3D.f90: FS_C2_Array3D.F90
	$(CPP) $(INC) $< > $@

# Build list of objects
OBJS  = $(ARRAY_OBJS)
OBJS += $(C_ARRAY_OBJS)
OBJS += $(FS_ARRAY_OBJS)
OBJS += $(FS_C_ARRAY_OBJS)

.PHONY: static shared
static: $(OBJS)
	$(AR) $(ARFLAGS) ../$(REFYPE_LIB_STATIC) $^
	$(RANLIB) ../$(REFYPE_LIB_STATIC)

shared: $(OBJS)
	$(CC) $(CFLAGS) -shared -o ../$(REFYPE_LIB_SHARED) $^
	$(RANLIB) ../$(REFYPE_LIB_STATIC)

.PHONY: clean
clean:
	@rm -rf *.o *.mod *.MOD mryp*.f90
