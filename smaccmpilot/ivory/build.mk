SMACCMDIR=smaccmpilot

# Relative to current dir
SANDBOX=../dsl/cabal-dev

SRCDIR=$(SMACCMDIR)/src
INCDIR=$(SMACCMDIR)/include/smaccmpilot

GEN=smaccmpilot-gen
EXEC=$(SMACCMDIR)/ivory/dist/build/$(GEN)/$(GEN)

IVORY-OPTS=--const-fold --overflow --div-zero
# A little too noisy: --fp-check

HDRS=																										 \
 $(INCDIR)/position_type.h															 \
 $(INCDIR)/pid_stabilize.h															 \
 $(INCDIR)/servo_type.h																	 \
 $(INCDIR)/sensors_type.h																 \
 $(INCDIR)/motorsoutput_type.h													 \
 $(INCDIR)/userinput_type.h                              \
 $(INCDIR)/gcs_transmit_type.h													 \
 $(INCDIR)/userinput_decode.h                            \
 $(INCDIR)/optflow_type.h																 \
 $(INCDIR)/position_estimate_type.h											 \
 $(INCDIR)/ivory_string.h																 \
 $(INCDIR)/console.h																		 \
 $(INCDIR)/driver_i2c.h																	 \
 $(INCDIR)/storage_eeprom.h															 \
 $(INCDIR)/storage_partition.h													 \
 $(INCDIR)/param.h

SRCS=																										 \
  $(SRCDIR)/pid_stabilize.c															 \
  $(SRCDIR)/gcs_transmit_driver.c												 \
  $(SRCDIR)/userinput_decode.c													 \
  $(SRCDIR)/storage_partition.c													 \
  $(SRCDIR)/param.c

# ------------------------------------------------------------------------------

# Generate the srcs and headers.
IVORY += ivory-build
.PHONY: ivory-build
ivory-build: $(EXEC) $(HDRS) $(SRCS)

# We don't add the .hs files as dependencies.  It's up to the user to clean
# them.  (It's not even clear if that's the right thing to do, since even if
# they don't change, if the compiler changes, you should rebuild.)
$(HDRS) $(SRCS):
	$(SANDBOX)/bin/$(GEN) --src-dir=$(SRCDIR) --include-dir=$(INCDIR) $(IVORY-OPTS)

# Build the binary to generate the code.
.PRECIOUS: $(EXEC)
$(EXEC):
	cabal-dev -s $(SANDBOX) install --builddir=$(SMACCMDIR)/ivory $(SMACCMDIR)/ivory

CLEAN     += $(HDRS) $(SRCS)
VERYCLEAN += $(SMACCMDIR)/ivory/dist

# ------------------------------------------------------------------------------
# CBMC stuff
# ------------------------------------------------------------------------------

.PHONY: verify
verify: # $(HDRS) $(SRCS)
	sh $(SMACCMDIR)/ivory/cbmc.sh




