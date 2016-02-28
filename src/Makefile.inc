SRC_DIR = $(TOP_DIR)/src

# Ensure the include path gets updated
INC += -I$(SRC_DIR)

ARRAY_OBJS = \
	mryp_Array1D.o \
	mryp_Array2D.o \
	mryp_Array3D.o
ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(ARRAY_OBJS))
$(ARRAY_OBJS:.o=.f90): $(SRC_DIR)/Array.inc
$(ARRAY_OBJS): $(SRC_DIR)/Array.inc
SRC_OBJS += $(ARRAY_OBJS)
$(SRC_DIR)/mryp_Array1D.f90: $(SRC_DIR)/Array1D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_Array2D.f90: $(SRC_DIR)/Array1D.F90
	@cp $(SRC_DIR)/Array1D.F90 $(SRC_DIR)/Array2D.F90
	@sed -i -e 's/\(define _R_DIM\) 1/\1 2/g;s/Array1D/Array2D/g' $(SRC_DIR)/Array2D.F90
	$(CPP) $(INC) $(SRC_DIR)/Array2D.F90 > $@
$(SRC_DIR)/mryp_Array3D.f90: $(SRC_DIR)/Array1D.F90
	@cp $(SRC_DIR)/Array1D.F90 $(SRC_DIR)/Array3D.F90
	@sed -i -e 's/\(define _R_DIM\) 1/\1 3/g;s/Array1D/Array3D/g' $(SRC_DIR)/Array3D.F90
	$(CPP) $(INC) $(SRC_DIR)/Array3D.F90 > $@

C_ARRAY_OBJS = \
	mryp_C2_Array1D.o \
	mryp_C2_Array2D.o \
	mryp_C2_Array3D.o
C_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(C_ARRAY_OBJS))
$(C_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(C_ARRAY_OBJS)
$(SRC_DIR)/mryp_C2_Array1D.f90: $(SRC_DIR)/C2_Array1D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_C2_Array2D.f90: $(SRC_DIR)/C2_Array2D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_C2_Array3D.f90: $(SRC_DIR)/C2_Array3D.F90
	$(CPP) $(INC) $< > $@

FS_ARRAY_OBJS = \
	mryp_FS_Array1D.o \
	mryp_FS_Array2D.o \
	mryp_FS_Array3D.o
FS_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(FS_ARRAY_OBJS))
$(FS_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(FS_ARRAY_OBJS)
$(SRC_DIR)/mryp_FS_Array1D.f90: $(SRC_DIR)/FS_Array1D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_FS_Array2D.f90: $(SRC_DIR)/FS_Array2D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_FS_Array3D.f90: $(SRC_DIR)/FS_Array3D.F90
	$(CPP) $(INC) $< > $@

FS_C_ARRAY_OBJS = \
	mryp_FS_C2_Array1D.o \
	mryp_FS_C2_Array2D.o \
	mryp_FS_C2_Array3D.o
FS_C_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(FS_C_ARRAY_OBJS))
$(FS_C_ARRAY_OBJS): $(C_ARRAY_OBJS)
SRC_OBJS += $(FS_C_ARRAY_OBJS)
$(SRC_DIR)/mryp_FS_C2_Array1D.f90: $(SRC_DIR)/FS_C2_Array1D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_FS_C2_Array2D.f90: $(SRC_DIR)/FS_C2_Array2D.F90
	$(CPP) $(INC) $< > $@
$(SRC_DIR)/mryp_FS_C2_Array3D.f90: $(SRC_DIR)/FS_C2_Array3D.F90
	$(CPP) $(INC) $< > $@

SM_OBJS = \
	mryp_SM_CSR.o
SM_OBJS := $(addprefix $(SRC_DIR)/,$(SM_OBJS))
$(SM_OBJS:.o=.f90): $(SRC_DIR)/SM_common.inc $(SRC_DIR)/SM_CSR.inc
$(SM_OBJS): $(SRC_DIR)/SM_common.inc $(SRC_DIR)/SM_CSR.inc
SRC_OBJS += $(SM_OBJS)
$(SRC_DIR)/mryp_SM_CSR.f90: $(SRC_DIR)/SM_CSR.F90
	$(CPP) $(INC) $< > $@


# For now everything depends on the utility program
UTIL_OBJS = \
	mryp_utils.o
UTIL_OBJS := $(addprefix $(SRC_DIR)/,$(UTIL_OBJS))
$(UTIL_OBJS): $(SRC_DIR)/FIND_integer.inc
$(UTIL_OBJS): $(SRC_DIR)/SORT_integer.inc
$(UTIL_OBJS): $(SRC_DIR)/UNIQ_integer.inc
$(SRC_OBJS): $(UTIL_OBJS)
SRC_OBJS += $(UTIL_OBJS)
$(SRC_DIR)/mryp_utils.f90: $(SRC_DIR)/utils.F90
	$(CPP) $(INC) $< > $@


# Populate the correct OBJS file
OBJS += $(SRC_OBJS)

# Local Variables:
#  mode: makefile-gmake
# End:
