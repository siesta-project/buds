TEST_DIR = $(TOP_DIR)/src/test

# Ensure the include path gets updated
INC += -I$(TEST_DIR)

# A list of common objects
# These objects are required by ALL the
# regular objects
TEST_COMMON_OBJS = test_utils.o
TEST_COMMON_OBJS := $(addprefix $(TEST_DIR)/,$(TEST_COMMON_OBJS))

TEST_TESTS += array
TEST_OBJS += array.o
$(TEST_DIR)/array: $(TEST_COMMON_OBJS) $(TEST_DIR)/array.o
	$(FC) -o $@ $^ $(BUDS_LIB_STATIC) $(INC)
	@echo "#### TEST -- running: $@"
	-./$@

TEST_TESTS += fs_array
TEST_OBJS += fs_array.o
$(TEST_DIR)/fs_array: $(TEST_COMMON_OBJS) $(TEST_DIR)/fs_array.o
	$(FC) -o $@ $^ $(BUDS_LIB_STATIC) $(INC)
	@echo "#### TEST -- running: $@"
	-./$@

TEST_TESTS += c2_array
TEST_OBJS += c2_array.o
$(TEST_DIR)/c2_array: $(TEST_COMMON_OBJS) $(TEST_DIR)/c2_array.o
	$(FC) -o $@ $^ $(BUDS_LIB_STATIC) $(INC)
	@echo "#### TEST -- running: $@"
	-./$@

TEST_TESTS += ll_array
TEST_OBJS += ll_array.o
$(TEST_DIR)/ll_array: $(TEST_COMMON_OBJS) $(TEST_DIR)/ll_array.o
	$(FC) -o $@ $^ $(BUDS_LIB_STATIC) $(INC)
	@echo "#### TEST -- running: $@"
	-./$@


TESTS_OBJS += $(TEST_COMMON_OBJS) $(addprefix $(TEST_DIR)/,$(TEST_OBJS))
TEST_TESTS := $(addprefix $(TEST_DIR)/,$(TEST_TESTS))

# This forces all tests in this directory to ALWAYS be runned
.PHONY: $(TEST_TESTS)
TESTS += $(TEST_TESTS)


# Local Variables:
#  mode: makefile-gmake
# End:
