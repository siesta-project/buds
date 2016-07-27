src_DIR = $(TOP_DIR)/src
# Append to VPATH
VPATH := $(VPATH):$(src_DIR)

# Shorthand for compiling and deleting the
# temporary file
src_CPP = $(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $(_TMP_FILE) > $@ && rm $(_TMP_FILE)

ARRAY_OBJECTS = \
	bud_Array1D.o \
	bud_Array2D.o \
	bud_Array3D.o
$(ARRAY_OBJECTS:.o=.f90): Array.inc
src_OBJECTS += $(ARRAY_OBJECTS)
bud_Array1D.f90: Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Array2D.f90: _TMP_FILE=Array2D.F90
bud_Array2D.f90: Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_Array3D.f90: _TMP_FILE=Array3D.F90
bud_Array3D.f90: Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)



FS_ARRAY_OBJECTS = \
	bud_FS_Array1D.o \
	bud_FS_Array2D.o \
	bud_FS_Array3D.o
$(FS_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(FS_ARRAY_OBJECTS)
bud_FS_Array1D.f90: FS_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_Array2D.f90: _TMP_FILE=FS_Array2D.F90
bud_FS_Array2D.f90: FS_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/FS_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_FS_Array3D.f90: _TMP_FILE=FS_Array3D.F90
bud_FS_Array3D.f90: FS_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/FS_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)


LL_ARRAY_OBJECTS = \
	bud_LL_Array1D.o \
	bud_LL_Array2D.o \
	bud_LL_Array3D.o
$(LL_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(LL_ARRAY_OBJECTS)
bud_LL_Array1D.f90: LL_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_LL_Array2D.f90: _TMP_FILE=LL_Array2D.F90
bud_LL_Array2D.f90: LL_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/LL_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_LL_Array3D.f90: _TMP_FILE=LL_Array3D.F90
bud_LL_Array3D.f90: LL_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/LL_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)


SM_OBJECTS = \
	bud_SM_CSR.o \
	bud_SM_CSR0.o \
	bud_SM_CSR_C.o \
	bud_SM_CSC.o \
	bud_SM_CSC0.o
$(SM_OBJECTS:.o=.f90): SM_common.inc SM_CSR.inc
$(SM_OBJECTS): SM_common.inc SM_CSR.inc
src_OBJECTS += $(SM_OBJECTS)
bud_SM_CSR.f90: SM_CSR.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR0.f90: SM_CSR0.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_C.f90: SM_CSR_C.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC.f90: SM_CSC.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC0.f90: SM_CSC0.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@


SM_AR_OBJECTS = \
	bud_SM_CSR_Array1D.o \
	bud_SM_CSR_Array2D.o \
	bud_SM_CSR_Array3D.o \
	bud_SM_CSR0_Array1D.o \
	bud_SM_CSR0_Array2D.o \
	bud_SM_CSR0_Array3D.o \
	bud_SM_CSR_C_Array1D.o \
	bud_SM_CSR_C_Array2D.o \
	bud_SM_CSR_C_Array2D.o
$(SM_AR_OBJECTS): $(SM_OBJECTS) $(ARRAY_OBJECTS)
src_OBJECTS += $(SM_AR_OBJECTS)
bud_SM_CSR_Array1D.f90: SM_CSR_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_Array2D.f90: _TMP_FILE=SM_CSR_Array2D.F90
bud_SM_CSR_Array2D.f90: SM_CSR_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SM_CSR_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_SM_CSR_Array3D.f90: _TMP_FILE=SM_CSR_Array3D.F90
bud_SM_CSR_Array3D.f90: SM_CSR_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SM_CSR_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)

bud_SM_CSR0_Array1D.f90: SM_CSR0_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR0_Array2D.f90: _TMP_FILE=SM_CSR0_Array2D.F90
bud_SM_CSR0_Array2D.f90: SM_CSR0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SM_CSR0_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_SM_CSR0_Array3D.f90: _TMP_FILE=SM_CSR0_Array3D.F90
bud_SM_CSR0_Array3D.f90: SM_CSR0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SM_CSR0_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)

bud_SM_CSR_C_Array1D.f90: SM_CSR_C_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_C_Array2D.f90: _TMP_FILE=SM_CSR_C_Array2D.F90
bud_SM_CSR_C_Array2D.f90: SM_CSR_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SM_CSR_C_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_SM_CSR_C_Array3D.f90: _TMP_FILE=SM_CSR_C_Array3D.F90
bud_SM_CSR_C_Array3D.f90: SM_CSR_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/3D/g' $(src_DIR)/SM_CSR_C_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)


FILE_OBJECTS = \
	bud_File.o
src_OBJECTS += $(FILE_OBJECTS)
bud_File.f90: File.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@


# For now everything depends on the utility program
UTIL_OBJECTS = \
	bud_utils.o
$(UTIL_OBJECTS): FIND_integer.inc
$(UTIL_OBJECTS): SORT_integer.inc
$(UTIL_OBJECTS): UNIQ_integer.inc
$(src_OBJECTS): $(UTIL_OBJECTS)
src_OBJECTS += $(UTIL_OBJECTS)
bud_utils.f90: utils.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@




# These objects below are more specialized in the sense
# that they are typically not of huge use.
# However, they may be used as template sources for creating
# other objects of the same type.
ITEM_ARRAY_OBJECTS = \
	bud_Item_Array1D.o \
	bud_Item_Array2D.o \
	bud_Item_Array3D.o
$(ITEM_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
#src_OBJECTS += $(ITEM_ARRAY_OBJECTS)
bud_Item_Array1D.f90: Item_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Item_Array2D.f90: _TMP_FILE=Item_Array2D.F90
bud_Item_Array2D.f90: Item_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/Item_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_Item_Array3D.f90: _TMP_FILE=Item_Array3D.F90
bud_Item_Array3D.f90: Item_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/Item_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)


C_ARRAY_OBJECTS = \
	bud_C2_Array1D.o \
	bud_C2_Array2D.o \
	bud_C2_Array3D.o
$(C_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
#src_OBJECTS += $(C_ARRAY_OBJECTS)
bud_C2_Array1D.f90: C2_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_C2_Array2D.f90: _TMP_FILE=C2_Array2D.F90
bud_C2_Array2D.f90: C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/C2_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_C2_Array3D.f90: _TMP_FILE=C2_Array3D.F90
bud_C2_Array3D.f90: C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/C2_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)

FS_C_ARRAY_OBJECTS = \
	bud_FS_C2_Array1D.o \
	bud_FS_C2_Array2D.o \
	bud_FS_C2_Array3D.o
$(FS_C_ARRAY_OBJECTS): $(C_ARRAY_OBJECTS)
#src_OBJECTS += $(FS_C_ARRAY_OBJECTS)
bud_FS_C2_Array1D.f90: FS_C2_Array1D.F90
	$(CPP) $(CPPFLAGS) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_C2_Array2D.f90: _TMP_FILE=FS_C2_Array2D.F90
bud_FS_C2_Array2D.f90: FS_C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/FS_C2_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)
bud_FS_C2_Array3D.f90: _TMP_FILE=FS_C2_Array3D.F90
bud_FS_C2_Array3D.f90: FS_C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/FS_C2_Array1D.F90 > $(_TMP_FILE)
	$(src_CPP)



# Define objects that needs compilation
src_SOURCES = $(patsubst %.o, %.f90, $(src_OBJECTS))
# CPP_SOURCES are those sources that needs deletion upon make clean
src_CPP_SOURCES = $(src_SOURCES)
SOURCES += $(src_SOURCES)
CPP_SOURCES += $(src_CPP_SOURCES)
F_MODS  += $(patsubst %.f90, %.mod, $(src_SOURCES))
OBJECTS += $(src_OBJECTS)


# Define rules for all objects
$(src_OBJECTS): INCLUDES += -I$(src_DIR)


# Local Variables:
#  mode: makefile-gmake
# End:
