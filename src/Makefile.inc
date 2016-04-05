SRC_DIR = $(TOP_DIR)/src

# Ensure the include path gets updated
INC += -I$(SRC_DIR)



ARRAY_OBJS = \
	bud_Array1D.o \
	bud_Array2D.o \
	bud_Array3D.o
ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(ARRAY_OBJS))
$(ARRAY_OBJS:.o=.f90): $(SRC_DIR)/Array.inc
$(ARRAY_OBJS): $(SRC_DIR)/Array.inc
SRC_OBJS += $(ARRAY_OBJS)
$(SRC_DIR)/bud_Array1D.f90: $(SRC_DIR)/Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_Array2D.f90: _TMP_FILE=$(SRC_DIR)/Array2D.F90
$(SRC_DIR)/bud_Array2D.f90: $(SRC_DIR)/Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_Array3D.f90: _TMP_FILE=$(SRC_DIR)/Array3D.F90
$(SRC_DIR)/bud_Array3D.f90: $(SRC_DIR)/Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


ITEM_ARRAY_OBJS = \
	bud_Item_Array1D.o \
	bud_Item_Array2D.o \
	bud_Item_Array3D.o
ITEM_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(ITEM_ARRAY_OBJS))
$(ITEM_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(ITEM_ARRAY_OBJS)
$(SRC_DIR)/bud_Item_Array1D.f90: $(SRC_DIR)/Item_Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_Item_Array2D.f90: _TMP_FILE=$(SRC_DIR)/Item_Array2D.F90
$(SRC_DIR)/bud_Item_Array2D.f90: $(SRC_DIR)/Item_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/Item_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_Item_Array3D.f90: _TMP_FILE=$(SRC_DIR)/Item_Array3D.F90
$(SRC_DIR)/bud_Item_Array3D.f90: $(SRC_DIR)/Item_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/Item_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


C_ARRAY_OBJS = \
	bud_C2_Array1D.o \
	bud_C2_Array2D.o \
	bud_C2_Array3D.o
C_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(C_ARRAY_OBJS))
$(C_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(C_ARRAY_OBJS)
$(SRC_DIR)/bud_C2_Array1D.f90: $(SRC_DIR)/C2_Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_C2_Array2D.f90: _TMP_FILE=$(SRC_DIR)/C2_Array2D.F90
$(SRC_DIR)/bud_C2_Array2D.f90: $(SRC_DIR)/C2_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/C2_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_C2_Array3D.f90: _TMP_FILE=$(SRC_DIR)/C2_Array3D.F90
$(SRC_DIR)/bud_C2_Array3D.f90: $(SRC_DIR)/C2_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/C2_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


FS_ARRAY_OBJS = \
	bud_FS_Array1D.o \
	bud_FS_Array2D.o \
	bud_FS_Array3D.o
FS_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(FS_ARRAY_OBJS))
$(FS_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(FS_ARRAY_OBJS)
$(SRC_DIR)/bud_FS_Array1D.f90: $(SRC_DIR)/FS_Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_FS_Array2D.f90: _TMP_FILE=$(SRC_DIR)/FS_Array2D.F90
$(SRC_DIR)/bud_FS_Array2D.f90: $(SRC_DIR)/FS_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/FS_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_FS_Array3D.f90: _TMP_FILE=$(SRC_DIR)/FS_Array3D.F90
$(SRC_DIR)/bud_FS_Array3D.f90: $(SRC_DIR)/FS_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/FS_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


LL_ARRAY_OBJS = \
	bud_LL_Array1D.o \
	bud_LL_Array2D.o \
	bud_LL_Array3D.o
LL_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(LL_ARRAY_OBJS))
$(LL_ARRAY_OBJS): $(ARRAY_OBJS)
SRC_OBJS += $(LL_ARRAY_OBJS)
$(SRC_DIR)/bud_LL_Array1D.f90: $(SRC_DIR)/LL_Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_LL_Array2D.f90: _TMP_FILE=$(SRC_DIR)/LL_Array2D.F90
$(SRC_DIR)/bud_LL_Array2D.f90: $(SRC_DIR)/LL_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/LL_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_LL_Array3D.f90: _TMP_FILE=$(SRC_DIR)/LL_Array3D.F90
$(SRC_DIR)/bud_LL_Array3D.f90: $(SRC_DIR)/LL_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/LL_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


FS_C_ARRAY_OBJS = \
	bud_FS_C2_Array1D.o \
	bud_FS_C2_Array2D.o \
	bud_FS_C2_Array3D.o
FS_C_ARRAY_OBJS := $(addprefix $(SRC_DIR)/,$(FS_C_ARRAY_OBJS))
$(FS_C_ARRAY_OBJS): $(C_ARRAY_OBJS)
SRC_OBJS += $(FS_C_ARRAY_OBJS)
$(SRC_DIR)/bud_FS_C2_Array1D.f90: $(SRC_DIR)/FS_C2_Array1D.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_FS_C2_Array2D.f90: _TMP_FILE=$(SRC_DIR)/FS_C2_Array2D.F90
$(SRC_DIR)/bud_FS_C2_Array2D.f90: $(SRC_DIR)/FS_C2_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(SRC_DIR)/FS_C2_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@
$(SRC_DIR)/bud_FS_C2_Array3D.f90: _TMP_FILE=$(SRC_DIR)/FS_C2_Array3D.F90
$(SRC_DIR)/bud_FS_C2_Array3D.f90: $(SRC_DIR)/FS_C2_Array1D.F90
	@rm -f $(_TMP_FILE)
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(SRC_DIR)/FS_C2_Array1D.F90 >> $(_TMP_FILE)
	$(CPP) $(INC) $(CPPFLAGS) $(_TMP_FILE) > $@


SM_OBJS = \
	bud_SM_CSR.o \
	bud_SM_CSR0.o \
	bud_SM_CSR_C.o
SM_OBJS := $(addprefix $(SRC_DIR)/,$(SM_OBJS))
$(SM_OBJS:.o=.f90): $(SRC_DIR)/SM_common.inc $(SRC_DIR)/SM_CSR.inc
$(SM_OBJS): $(SRC_DIR)/SM_common.inc $(SRC_DIR)/SM_CSR.inc
SRC_OBJS += $(SM_OBJS)
$(SRC_DIR)/bud_SM_CSR.f90: $(SRC_DIR)/SM_CSR.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_SM_CSR0.f90: $(SRC_DIR)/SM_CSR0.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@
$(SRC_DIR)/bud_SM_CSR_C.f90: $(SRC_DIR)/SM_CSR_C.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@


FILE_OBJS = \
	bud_File.o
FILE_OBJS := $(addprefix $(SRC_DIR)/,$(FILE_OBJS))
SRC_OBJS += $(FILE_OBJS)
$(SRC_DIR)/bud_File.f90: $(SRC_DIR)/File.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@


# For now everything depends on the utility program
UTIL_OBJS = \
	bud_utils.o
UTIL_OBJS := $(addprefix $(SRC_DIR)/,$(UTIL_OBJS))
$(UTIL_OBJS): $(SRC_DIR)/FIND_integer.inc
$(UTIL_OBJS): $(SRC_DIR)/SORT_integer.inc
$(UTIL_OBJS): $(SRC_DIR)/UNIQ_integer.inc
$(SRC_OBJS): $(UTIL_OBJS)
SRC_OBJS += $(UTIL_OBJS)
$(SRC_DIR)/bud_utils.f90: $(SRC_DIR)/utils.F90
	$(CPP) $(INC) $(CPPFLAGS) $< > $@


# Populate the correct OBJS file
OBJS += $(SRC_OBJS)

# Local Variables:
#  mode: makefile-gmake
# End:
