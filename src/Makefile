TOP_DIR = ..

# Include user-defined setup
-include $(TOP_DIR)/setup.make

# Common makefile stuff:
ifndef _INCLUDED_common
  include $(TOP_DIR)/Makefile.common
endif

ifndef _INCLUDED_default
  include $(TOP_DIR)/Makefile.default
endif

# Include the top-dir include directory
override INC += -I$(TOP_DIR)/include

# Unset OBJS
OBJS = 

ARRAY_OBJS = \
	mryp_Array1D.o \
	mryp_Array2D.o \
	mryp_Array3D.o
$(ARRAY_OBJS:.o=.f90): Array.inc
$(ARRAY_OBJS): Array.inc
OBJS += $(ARRAY_OBJS)
mryp_Array1D.f90: Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_Array2D.f90: Array1D.F90
	@cp Array1D.F90 Array2D.F90
	@sed -i -e 's/\(define _R_DIM\) 1/\1 2/g;s/Array1D/Array2D/g' Array2D.F90
	$(CPP) $(INC) Array2D.F90 > $@
mryp_Array3D.f90: Array1D.F90
	@cp Array1D.F90 Array3D.F90
	@sed -i -e 's/\(define _R_DIM\) 1/\1 3/g;s/Array1D/Array3D/g' Array3D.F90
	$(CPP) $(INC) Array3D.F90 > $@

C_ARRAY_OBJS = \
	mryp_C2_Array1D.o \
	mryp_C2_Array2D.o \
	mryp_C2_Array3D.o
$(C_ARRAY_OBJS): $(ARRAY_OBJS)
OBJS += $(C_ARRAY_OBJS)
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
$(FS_ARRAY_OBJS): $(ARRAY_OBJS)
OBJS += $(FS_ARRAY_OBJS)
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
$(FS_C_ARRAY_OBJS): $(C_ARRAY_OBJS)
OBJS += $(FS_C_ARRAY_OBJS)
mryp_FS_C2_Array1D.f90: FS_C2_Array1D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_C2_Array2D.f90: FS_C2_Array2D.F90
	$(CPP) $(INC) $< > $@
mryp_FS_C2_Array3D.f90: FS_C2_Array3D.F90
	$(CPP) $(INC) $< > $@

SM_OBJS = \
	mryp_SM_CSR.o
$(SM_OBJS:.o=.f90): SM_common.inc SM_CSR.inc
$(SM_OBJS): SM_common.inc SM_CSR.inc
OBJS += $(SM_OBJS)
mryp_SM_CSR.f90: SM_CSR.F90
	$(CPP) $(INC) $< > $@


# For now everything depends on the utility program
UTIL_OBJS = \
	mryp_utils.o
$(UTIL_OBJS): FIND_integer.inc
$(UTIL_OBJS): SORT_integer.inc
$(UTIL_OBJS): UNIQ_integer.inc
$(OBJS): $(UTIL_OBJS)
OBJS += $(UTIL_OBJS)
mryp_utils.f90: utils.F90
	$(CPP) $(INC) $< > $@


.PHONY: static shared
static: $(OBJS)
	$(AR) $(ARFLAGS) $(TOP_DIR)/$(REFYPE_LIB_STATIC) $^
	$(RANLIB) $(TOP_DIR)/$(REFYPE_LIB_STATIC)

shared: $(OBJS)
	$(CC) $(CFLAGS) -shared -o $(TOP_DIR)/$(REFYPE_LIB_SHARED) $^

.PHONY: clean
clean:
	@rm -rf *.o *.mod *.MOD mryp*.f90
