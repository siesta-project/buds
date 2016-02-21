
# the default target
.PHONY: default
.NOTPARALLEL: default
default: static

# the default target
.PHONY: all
.NOTPARALLEL: all
all: static shared

# Include user-defined setup
-include setup.make

# Common makefile stuff:
ifndef _INCLUDED_common
include Makefile.common
endif

ifndef _INCLUDED_default
include Makefile.default
endif

# Add the installation makefile
include Makefile.install

# Add the documentation makefile
include Makefile.doc

# Add the distribution creation tool
include Makefile.dist

# Now create the actual compilation make file
.PHONY: lib libs shared static
#.NOTPARALLEL: lib libs shared static
# Define default library creations
ifndef NO_SHARED
ifndef NO_STATIC
# No options given
NO_SHARED = 1
endif
endif

lib: libs
ifndef NO_STATIC
 ifndef NO_SHARED
libs: static shared
 else
libs: static
 endif
else
 ifndef NO_SHARED
libs: shared
 else
libs:
	@echo "No library build."
 endif
endif

shared:
	$(MAKE) $(MFLAGS) -C src shared

static:
	$(MAKE) $(MFLAGS) -C src static

clean:
	rm -f $(REFYPE_LIB_STATIC) $(REFYPE_LIB_SHARED)
	$(MAKE) -C src clean
