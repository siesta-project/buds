src_DIR = $(TOP_DIR)/src
# Append to VPATH
VPATH := $(VPATH):$(src_DIR)

# Shorthand for compiling and deleting the
# temporary file
src_FPP = $(FPP) $(FPPFLAGS) $(INCLUDES) $(_TMP_FILE) > $@ && rm $(_TMP_FILE)

ARRAY_OBJECTS = \
	bud_Array1D.o \
	bud_Array2D.o \
	bud_Array3D.o
$(ARRAY_OBJECTS:.o=.f90): Array.inc
src_OBJECTS += $(ARRAY_OBJECTS)
bud_Array1D.f90: Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Array2D.f90: _TMP_FILE=Array2D.F90
bud_Array2D.f90: Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_Array3D.f90: _TMP_FILE=Array3D.F90
bud_Array3D.f90: Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)



FS_ARRAY_OBJECTS = \
	bud_FS_Array1D.o \
	bud_FS_Array2D.o \
	bud_FS_Array3D.o
$(FS_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(FS_ARRAY_OBJECTS)
bud_FS_Array1D.f90: FS_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_Array2D.f90: _TMP_FILE=FS_Array2D.F90
bud_FS_Array2D.f90: FS_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/FS_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_FS_Array3D.f90: _TMP_FILE=FS_Array3D.F90
bud_FS_Array3D.f90: FS_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/FS_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


LL_ARRAY_OBJECTS = \
	bud_LL_Array1D.o \
	bud_LL_Array2D.o \
	bud_LL_Array3D.o
$(LL_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(LL_ARRAY_OBJECTS)
bud_LL_Array1D.f90: LL_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_LL_Array2D.f90: _TMP_FILE=LL_Array2D.F90
bud_LL_Array2D.f90: LL_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/LL_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_LL_Array3D.f90: _TMP_FILE=LL_Array3D.F90
bud_LL_Array3D.f90: LL_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/LL_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


SP_R = \
	bud_SP_CSR.o \
	bud_SP_CSR0.o \
	bud_SP_CSR_C.o
$(SP_R:.o=.f90): SP_common.inc SP_CSR.inc
$(SP_R): SP_common.inc SP_CSR.inc
src_OBJECTS += $(SP_R)
bud_SP_CSR.f90: SP_CSR.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSR0.f90: SP_CSR0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSR_C.f90: SP_CSR_C.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

SP_C = \
	bud_SP_CSC.o \
	bud_SP_CSC0.o \
	bud_SP_CSC_C.o
$(SP_C:.o=.f90): SP_common.inc SP_CSC.inc
$(SP_C): SP_common.inc SP_CSC.inc
src_OBJECTS += $(SP_C)
bud_SP_CSC.f90: SP_CSC.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSC0.f90: SP_CSC0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSC_C.f90: SP_CSC_C.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


SP_ARR_R = \
	bud_SP_CSR_Array1D.o \
	bud_SP_CSR_Array2D.o \
	bud_SP_CSR_Array3D.o \
	bud_SP_CSR0_Array1D.o \
	bud_SP_CSR0_Array2D.o \
	bud_SP_CSR0_Array3D.o \
	bud_SP_CSR_C_Array1D.o \
	bud_SP_CSR_C_Array2D.o \
	bud_SP_CSR_C_Array3D.o
$(SP_ARR_R): $(SP_R) $(ARRAY_OBJECTS)
src_OBJECTS += $(SP_ARR_R)
bud_SP_CSR_Array1D.f90: SP_CSR_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSR_Array2D.f90: _TMP_FILE=SP_CSR_Array2D.F90
bud_SP_CSR_Array2D.f90: SP_CSR_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSR_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSR_Array3D.f90: _TMP_FILE=SP_CSR_Array3D.F90
bud_SP_CSR_Array3D.f90: SP_CSR_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSR_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSR0_Array1D.f90: SP_CSR0_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSR0_Array2D.f90: _TMP_FILE=SP_CSR0_Array2D.F90
bud_SP_CSR0_Array2D.f90: SP_CSR0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSR0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSR0_Array3D.f90: _TMP_FILE=SP_CSR0_Array3D.F90
bud_SP_CSR0_Array3D.f90: SP_CSR0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSR0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSR_C_Array1D.f90: SP_CSR_C_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSR_C_Array2D.f90: _TMP_FILE=SP_CSR_C_Array2D.F90
bud_SP_CSR_C_Array2D.f90: SP_CSR_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSR_C_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSR_C_Array3D.f90: _TMP_FILE=SP_CSR_C_Array3D.F90
bud_SP_CSR_C_Array3D.f90: SP_CSR_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSR_C_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)

SP_ARR_C = \
	bud_SP_CSC_Array1D.o \
	bud_SP_CSC_Array2D.o \
	bud_SP_CSC_Array3D.o \
	bud_SP_CSC0_Array1D.o \
	bud_SP_CSC0_Array2D.o \
	bud_SP_CSC0_Array3D.o \
	bud_SP_CSC_C_Array1D.o \
	bud_SP_CSC_C_Array2D.o \
	bud_SP_CSC_C_Array3D.o
$(SP_ARR_C): $(SP_C) $(ARRAY_OBJECTS)
#src_OBJECTS += $(SP_ARR_C)
bud_SP_CSC_Array1D.f90: SP_CSC_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSC_Array2D.f90: _TMP_FILE=SP_CSC_Array2D.F90
bud_SP_CSC_Array2D.f90: SP_CSC_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSC_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSC_Array3D.f90: _TMP_FILE=SP_CSC_Array3D.F90
bud_SP_CSC_Array3D.f90: SP_CSC_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSC_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSC0_Array1D.f90: SP_CSC0_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSC0_Array2D.f90: _TMP_FILE=SP_CSC0_Array2D.F90
bud_SP_CSC0_Array2D.f90: SP_CSC0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSC0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSC0_Array3D.f90: _TMP_FILE=SP_CSC0_Array3D.F90
bud_SP_CSC0_Array3D.f90: SP_CSC0_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSC0_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSC_C_Array1D.f90: SP_CSC_C_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SP_CSC_C_Array2D.f90: _TMP_FILE=SP_CSC_C_Array2D.F90
bud_SP_CSC_C_Array2D.f90: SP_CSC_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $(src_DIR)/SP_CSC_C_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_SP_CSC_C_Array3D.f90: _TMP_FILE=SP_CSC_C_Array3D.F90
bud_SP_CSC_C_Array3D.f90: SP_CSC_C_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $(src_DIR)/SP_CSC_C_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


LIST_OBJECTS = \
	bud_List.o
$(LIST_OBJECTS:.o=.f90): List.inc
src_OBJECTS += $(LIST_OBJECTS)
bud_List.f90: List.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


FILE_OBJECTS = \
	bud_File.o
src_OBJECTS += $(FILE_OBJECTS)
bud_File.f90: File.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


# For now everything depends on the utility program
UTIL_OBJECTS = \
	bud_utils.o
$(UTIL_OBJECTS): FIND_integer.inc
$(UTIL_OBJECTS): SORT_integer.inc
$(UTIL_OBJECTS): UNIQ_integer.inc
$(src_OBJECTS): $(UTIL_OBJECTS)
src_OBJECTS += $(UTIL_OBJECTS)
bud_utils.f90: utils.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@




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
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Item_Array2D.f90: _TMP_FILE=Item_Array2D.F90
bud_Item_Array2D.f90: Item_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/Item_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_Item_Array3D.f90: _TMP_FILE=Item_Array3D.F90
bud_Item_Array3D.f90: Item_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/Item_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)


C_ARRAY_OBJECTS = \
	bud_C2_Array1D.o \
	bud_C2_Array2D.o \
	bud_C2_Array3D.o
$(C_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
#src_OBJECTS += $(C_ARRAY_OBJECTS)
bud_C2_Array1D.f90: C2_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_C2_Array2D.f90: _TMP_FILE=C2_Array2D.F90
bud_C2_Array2D.f90: C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/C2_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_C2_Array3D.f90: _TMP_FILE=C2_Array3D.F90
bud_C2_Array3D.f90: C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/C2_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)

FS_C_ARRAY_OBJECTS = \
	bud_FS_C2_Array1D.o \
	bud_FS_C2_Array2D.o \
	bud_FS_C2_Array3D.o
$(FS_C_ARRAY_OBJECTS): $(C_ARRAY_OBJECTS)
#src_OBJECTS += $(FS_C_ARRAY_OBJECTS)
bud_FS_C2_Array1D.f90: FS_C2_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_C2_Array2D.f90: _TMP_FILE=FS_C2_Array2D.F90
bud_FS_C2_Array2D.f90: FS_C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $(src_DIR)/FS_C2_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)
bud_FS_C2_Array3D.f90: _TMP_FILE=FS_C2_Array3D.F90
bud_FS_C2_Array3D.f90: FS_C2_Array1D.F90
	@sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $(src_DIR)/FS_C2_Array1D.F90 > $(_TMP_FILE)
	$(src_FPP)



# Define objects that needs compilation
src_SOURCES = $(patsubst %.o, %.f90, $(src_OBJECTS))
# FPP_SOURCES are those sources that needs deletion upon make clean
src_FPP_SOURCES = $(src_SOURCES)
src_MODS = $(patsubst %.f90, %.mod, $(src_SOURCES))

SOURCES += $(src_SOURCES)
FPP_SOURCES += $(src_FPP_SOURCES)
F_MODS  += $(src_MODS)
OBJECTS += $(src_OBJECTS)


# Define rules for all objects
$(src_FPP_SOURCES): INCLUDES += -I$(src_DIR)


clean-src:
	-rm -f $(src_FPP_SOURCES)
	-rm -f $(src_OBJECTS)
	-rm -f *.mod


clean: clean-src


# Local Variables:
#  mode: makefile-gmake
# End:
