src_DIR = $(TOP_DIR)/src
# Append both this directory and iff preprocessed sources exist
VPATH := $(VPATH):$(src_DIR):$(SOURCES_DIR)/src

# Shorthand for preprocessing from pipe
src_FPP = $(FPP) $(FPPFLAGS) $(INCLUDES) -

ARRAY_OBJECTS = \
	bud_Array1D.o \
	bud_Array2D.o \
	bud_Array3D.o \
	bud_Array.o
$(ARRAY_OBJECTS:.o=.f90): Array.inc
$(ARRAY_OBJECTS): bud_File.o
src_OBJECTS += $(ARRAY_OBJECTS)
bud_Array1D.f90: Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Array2D.f90: Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_Array3D.f90: Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@
bud_Array.f90: Array.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_Array.o: bud_Array1D.o bud_Array2D.o bud_Array3D.o



FS_ARRAY_OBJECTS = \
	bud_FS_Array1D.o \
	bud_FS_Array2D.o \
	bud_FS_Array3D.o
$(FS_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(FS_ARRAY_OBJECTS)
bud_FS_Array1D.f90: FS_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_Array2D.f90: FS_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_FS_Array3D.f90: FS_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@


LL_ARRAY_OBJECTS = \
	bud_LL_Array1D.o \
	bud_LL_Array2D.o \
	bud_LL_Array3D.o
$(LL_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
src_OBJECTS += $(LL_ARRAY_OBJECTS)
bud_LL_Array1D.f90: LL_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_LL_Array2D.f90: LL_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_LL_Array3D.f90: LL_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@

SM_common = bud_SM_common.o
src_OBJECTS += $(SM_common)
bud_SM_common.f90: SM_common.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

SM_R = \
	bud_SM_CSR.o \
	bud_SM_CSR0.o \
	bud_SM_CSR_C.o
$(SM_R:.o=.f90): SM_common.inc SM_CSR.inc
$(SM_R): bud_List.o bud_File.o
src_OBJECTS += $(SM_R)
bud_SM_CSR.f90: SM_CSR.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR0.f90: SM_CSR0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_C.f90: SM_CSR_C.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@

SM_C = \
	bud_SM_CSC.o \
	bud_SM_CSC0.o \
	bud_SM_CSC_C.o
$(SM_C:.o=.f90): SM_common.inc SM_CSC.inc
$(SM_C): bud_List.o bud_File.o
src_OBJECTS += $(SM_C)
bud_SM_CSC.f90: SM_CSC.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC0.f90: SM_CSC0.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC_C.f90: SM_CSC_C.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@


SM_ARR_R = \
	bud_SM_CSR_Array1D.o \
	bud_SM_CSR_Array2D.o \
	bud_SM_CSR_Array3D.o \
	bud_SM_CSR0_Array1D.o \
	bud_SM_CSR0_Array2D.o \
	bud_SM_CSR0_Array3D.o \
	bud_SM_CSR_C_Array1D.o \
	bud_SM_CSR_C_Array2D.o \
	bud_SM_CSR_C_Array3D.o
$(SM_ARR_R): SM_Array.inc SM_Array_type_.inc $(SM_R) $(ARRAY_OBJECTS)
src_OBJECTS += $(SM_ARR_R)
bud_SM_CSR_Array1D.f90: SM_CSR_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_Array2D.f90: SM_CSR_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSR_Array3D.f90: SM_CSR_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@
bud_SM_CSR0_Array1D.f90: SM_CSR0_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR0_Array2D.f90: SM_CSR0_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSR0_Array3D.f90: SM_CSR0_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@
bud_SM_CSR_C_Array1D.f90: SM_CSR_C_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSR_C_Array2D.f90: SM_CSR_C_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSR_C_Array3D.f90: SM_CSR_C_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@

SM_ARR_C = \
	bud_SM_CSC_Array1D.o \
	bud_SM_CSC_Array2D.o \
	bud_SM_CSC_Array3D.o \
	bud_SM_CSC0_Array1D.o \
	bud_SM_CSC0_Array2D.o \
	bud_SM_CSC0_Array3D.o \
	bud_SM_CSC_C_Array1D.o \
	bud_SM_CSC_C_Array2D.o \
	bud_SM_CSC_C_Array3D.o
$(SM_ARR_C): SM_Array.inc SM_Array_type_.inc $(SM_C) $(ARRAY_OBJECTS)
src_OBJECTS += $(SM_ARR_C)
bud_SM_CSC_Array1D.f90: SM_CSC_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC_Array2D.f90: SM_CSC_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSC_Array3D.f90: SM_CSC_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@
bud_SM_CSC0_Array1D.f90: SM_CSC0_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC0_Array2D.f90: SM_CSC0_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSC0_Array3D.f90: SM_CSC0_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@
bud_SM_CSC_C_Array1D.f90: SM_CSC_C_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_SM_CSC_C_Array2D.f90: SM_CSC_C_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/1D/2D/g' $< | $(src_FPP) > $@
bud_SM_CSC_C_Array3D.f90: SM_CSC_C_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/1D/3D/g' $< | $(src_FPP) > $@


LIST_OBJECTS = \
	bud_List.o
$(LIST_OBJECTS:.o=.f90): List.inc
$(LIST_OBJECTS): bud_File.o
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
	bud_Utils.o \
	bud_Transfer.o
bud_Utils.f90: Utils.F90
Utils.F90: FIND_integer.inc SORT_integer.inc UNIQ_integer.inc
$(src_OBJECTS): $(UTIL_OBJECTS)
src_OBJECTS += $(UTIL_OBJECTS)
bud_Utils.f90: Utils.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
Transfer.F90: TransferD_Assoc_interface.inc
Transfer.F90: TransferD_Assoc1D.inc TransferD_Assoc2D.inc TransferD_Assoc3D.inc
Transfer.F90: TransferD_Copy_interface.inc
Transfer.F90: TransferD_Copy1D.inc TransferD_Copy2D.inc TransferD_Copy3D.inc
bud_Transfer.f90: Transfer.F90
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
bud_Item_Array2D.f90: Item_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_Item_Array3D.f90: Item_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@


C_ARRAY_OBJECTS = \
	bud_C2_Array1D.o \
	bud_C2_Array2D.o \
	bud_C2_Array3D.o
$(C_ARRAY_OBJECTS): $(ARRAY_OBJECTS)
#src_OBJECTS += $(C_ARRAY_OBJECTS)
bud_C2_Array1D.f90: C2_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_C2_Array2D.f90: C2_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_C2_Array3D.f90: C2_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@

FS_C_ARRAY_OBJECTS = \
	bud_FS_C2_Array1D.o \
	bud_FS_C2_Array2D.o \
	bud_FS_C2_Array3D.o
$(FS_C_ARRAY_OBJECTS): $(C_ARRAY_OBJECTS)
#src_OBJECTS += $(FS_C_ARRAY_OBJECTS)
bud_FS_C2_Array1D.f90: FS_C2_Array1D.F90
	$(FPP) $(FPPFLAGS) $(INCLUDES) $< > $@
bud_FS_C2_Array2D.f90: FS_C2_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 2/;s/Array1D/Array2D/g' $< | $(src_FPP) > $@
bud_FS_C2_Array3D.f90: FS_C2_Array1D.F90
	sed -e 's/\(BUD_DIM\) 1/\1 3/;s/Array1D/Array3D/g' $< | $(src_FPP) > $@



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


source-src: $(src_SOURCES)
	mkdir -p $(SOURCES_DIR)/src
	mv $(src_SOURCES) $(SOURCES_DIR)/src
	sed -i -s -e '/[[:space:]]*!/d' $(SOURCES_DIR)/src/*.f90

clean-src:
	-rm -f $(src_FPP_SOURCES)
	-rm -f $(src_OBJECTS)
	-rm -f *.mod


clean: clean-src


# Local Variables:
#  mode: makefile-gmake
# End:
