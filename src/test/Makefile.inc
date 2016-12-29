test_src_DIR = $(TOP_DIR)/src/test
# Where to create the output of tests
test_out_DIR = test

# Ensure the include path gets updated
INCLUDES += -I$(test_src_DIR)
# Ensure the linking to the library
LDFLAGS += -L.

# A list of common objects
# These objects are required by ALL the
# regular objects
test_COMMON_OBJECTS = test_utils.o
test_COMMON_OBJECTS := $(addprefix $(test_out_DIR)/,$(test_COMMON_OBJECTS))

src_TESTS += array
t_src_OBJECTS += array.o
$(test_out_DIR)/array: $(test_COMMON_OBJECTS) $(test_out_DIR)/array.o
	$(FC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo $@ >> run_tests.sh

src_TESTS += fs_array
t_src_OBJECTS += fs_array.o
$(test_out_DIR)/fs_array: $(test_COMMON_OBJECTS) $(test_out_DIR)/fs_array.o
	$(FC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo $@ >> run_tests.sh

#src_TESTS += c2_array
#t_src_OBJECTS += c2_array.o
$(test_out_DIR)/c2_array: $(test_COMMON_OBJECTS) $(test_out_DIR)/c2_array.o
	$(FC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo $@ >> run_tests.sh

src_TESTS += ll_array
t_src_OBJECTS += ll_array.o
$(test_out_DIR)/ll_array: $(test_COMMON_OBJECTS) $(test_out_DIR)/ll_array.o
	$(FC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo $@ >> run_tests.sh

src_TESTS += transfer_p
t_src_OBJECTS += transfer_p.o
$(test_out_DIR)/transfer_p: $(test_COMMON_OBJECTS) $(test_out_DIR)/transfer_p.o
	$(FC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo $@ >> run_tests.sh


$(t_src_OBJECTS): $(test_COMMON_OBJECTS)
t_src_OBJECTS := $(test_COMMON_OBJECTS) $(addprefix $(test_out_DIR)/,$(t_src_OBJECTS))
src_TESTS := $(addprefix $(test_out_DIR)/,$(src_TESTS))

# This forces all tests in this directory to ALWAYS be runned
.PHONY: $(src_TESTS)
TESTS += $(src_TESTS)

# Ensure the directory is created
$(t_src_OBJECTS): test-buds
$(t_src_OBJECTS): $(BUDS_LIB_SHARED)


test-buds:
	-mkdir -p $(test_out_DIR)
	@echo 'export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH' > run_tests.sh


test-run: $(src_TESTS)
test-run:
	sh run_tests.sh
test: test-run


clean-src-test:
	-rm -f $(t_src_OBJECTS) run_tests.sh
	-rm -f $(test_out_DIR)/*.mod
	-rm -f $(src_TESTS)

clean: clean-src-test

# Local Variables:
#  mode: makefile-gmake
# End:
