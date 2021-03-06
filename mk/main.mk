# -*- Mode: makefile-gmake; indent-tabs-mode: t; tab-width: 2 -*-
#
# Makefile --- SMACCMPilot firmware build system.
#
# Copyright (C) 2013, Galois, Inc.
# All Rights Reserved.
#
# This software is released under the "BSD3" license.  Read the file
# "LICENSE" for more information.
#

.SUFFIXES:
MAKEFLAGS += -r -j4

include Config.mk
# Commsec vars
include Keys.mk

include mk/platform/platform_$(CONFIG_PLATFORM).mk

TOP := .

BUILD_DIR := $(TOP)/build/$(CONFIG_PLATFORM)
OBJ_DIR   := $(BUILD_DIR)/obj
LIB_DIR   := $(BUILD_DIR)/lib
IMG_DIR   := $(BUILD_DIR)/img

# Build output directory containing generated sources.
GEN_DIR   := $(BUILD_DIR)/gen

# Add the built library directory to the default linker flags.
LDFLAGS += -L$(LIB_DIR)

.PHONY: all
all: all-targets

include mk/cmd.lib
include mk/library.mk
include mk/image.mk
include mk/ivory.mk
include mk/echronos.mk
include mk/frama-c-check.mk
include mk/conditional.mk
include mk/cppcheck.mk

define project
  include $(1)/build.mk
endef

OTHER_TARGETS :=

# Search for subprojects and include their "build.mk" makefiles.
$(foreach p,$(shell find . -name build.mk -exec dirname {} \;), \
          $(eval $(call project,$(p))))

ALL_TARGETS := $(LIBRARIES) $(IMAGES) $(OTHER_TARGETS)
ALL_DEPS    := $(patsubst %.o,%.d,$(ALL_OBJECTS))

######################################################################
## Targets

.PHONY: all-targets
all-targets: $(ALL_TARGETS)

.PHONY: frama-c-check
frama-c-check: $(FRAMA_C)

.PHONY: clean
clean:
	$(Q)rm -rf $(TOP)/build

.PHONY: veryclean
veryclean: clean

######################################################################
## Compilation Rules

quiet_cmd_cc_i_c = CC       $<
      cmd_cc_i_c = $(CC) $(CFLAGS) -E -o $<.i $<

.SECONDARY: $(OBJ_DIR)/%.i

quiet_cmd_cc_o_c = CC       $<
      cmd_cc_o_c = $(CC) $(CFLAGS) -MMD -c -o $@ $<

# Compile a C source file to an object and dependency file.
$(OBJ_DIR)/%.o: %.c
	$(Q)mkdir -p $(dir $@)
	$(call cmd,cc_i_c)
	$(call cmd,cc_o_c)

# Compile a generated C source file to an object and dependency file.
$(OBJ_DIR)/%.o: $(GEN_DIR)/%.c
	$(Q)mkdir -p $(dir $@)
	$(call cmd,cc_i_c)
	$(call cmd,cc_o_c)

quiet_cmd_cxx_o_c = CXX      $<
      cmd_cxx_o_c = $(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Compile a C++ source file to an object and dependency file.
$(OBJ_DIR)/%.o: %.cpp
	$(Q)mkdir -p $(dir $@)
	$(call cmd,cxx_o_c)

quiet_cmd_link = LINK     $@
      cmd_link = $(CC) -o $@ $(LDFLAGS) -Wl,-Map=$@.map $(2) $(LIBS)

quiet_cmd_lib = AR       $@
      cmd_lib = $(AR) rcs $@ $(2) && $(RANLIB) $@

quiet_cmd_as_o_S = AS       $<
      cmd_as_o_S = $(CC) $(CFLAGS) -c -o $@ $<

quiet_cmd_cpp_lds_S = CPP      $<
      cmd_cpp_lds_S = $(CPP) -P $(LDSCRIPT_OPTS) -o $@ $<

# Compile an assembly source (.S) file to an object file.
$(OBJ_DIR)/%.o: %.S
	$(Q)mkdir -p $(dir $@)
	$(call cmd,as_o_S)

quiet_cmd_as_o_s = AS       $<
      cmd_as_o_s = $(CC) $(CFLAGS) -c -o $@ $<

# Compile an assembly source (.s) file to an object file.
$(OBJ_DIR)/%.o: %.s
	$(Q)mkdir -p $(dir $@)
	$(call cmd,as_o_s)

# Compile a generated assembly source (.s) file to an object file.
$(OBJ_DIR)/%.o: $(GEN_DIR)/%.s
	$(Q)mkdir -p $(dir $@)
	$(call cmd,as_o_s)

quiet_cmd_elf_to_bin = OBJCOPY  $@
      cmd_elf_to_bin = $(OBJCOPY) -O binary $(2) $(2).bin

quiet_cmd_bin_to_px4 = PX4IMG   $@
      cmd_bin_to_px4 = $(PYTHON) $(TOP)/boot/px_mkfw.py
      cmd_bin_to_px4+= --prototype mk/platform/$($(CONFIG_PLATFORM)_PX4_IMG_PROTO).prototype
      cmd_bin_to_px4+= --image $(2) > $(2:.bin=.px4)

# Make all object files depend on all included Makefiles, to force a
# rebuild if the build system or configuration is modified.
$(ALL_OBJECTS): $(MAKEFILE_LIST)

# Include generated dependency files if they exist.
-include $(ALL_DEPS)

# vim: set ft=make noet ts=2:
