# -*- Mode: makefile-gmake; indent-tabs-mode: t; tab-width: 2 -*-
#
# build.mk
#
# Copyright (C) 2012, Galois, Inc.
# All Rights Reserved.
#
# This software is released under the "BSD3" license.  Read the file
# "LICENSE" for more information.
#
# Written by Pat Hickey <pat@galois.com>, January 08, 2013
#

$(eval $(call ivory_pkg,IVORY_PKG_TOWER_RPC_TEST,ivory-tower-freertos-example-rpc))

$(eval $(call cbmc_pkg,APP_TOWER_RPC_TEST,IVORY_PKG_TOWER_RPC_TEST))

APP_TOWER_RPC_TEST_IMG          := tower-rpc-test

APP_TOWER_RPC_TEST_OBJECTS      := main.o
APP_TOWER_RPC_TEST_REAL_OBJECTS += $(IVORY_PKG_TOWER_RPC_TEST_OBJECTS)

APP_TOWER_RPC_TEST_INCLUDES     += $(FREERTOS_INCLUDES)
APP_TOWER_RPC_TEST_INCLUDES     += -I$(TOP)/src/bsp/hwf4/include

APP_TOWER_RPC_TEST_CFLAGS        = $(APP_TOWER_RPC_TEST_INCLUDES)
APP_TOWER_RPC_TEST_CFLAGS       += -DIVORY_DEPLOY
APP_TOWER_RPC_TEST_CFLAGS       += $(IVORY_PKG_TOWER_RPC_TEST_CFLAGS)
APP_TOWER_RPC_TEST_CXXFLAGS      = $(APP_TOWER_RPC_TEST_INCLUDES)
APP_TOWER_RPC_TEST_CXXFLAGS     += $(IVORY_PKG_TOWER_RPC_TEST_CFLAGS)

APP_TOWER_RPC_TEST_LIBRARIES    += libhwf4.a
APP_TOWER_RPC_TEST_LIBRARIES    += libstm32_usb.a
APP_TOWER_RPC_TEST_LIBRARIES    += libFreeRTOS.a

APP_TOWER_RPC_TEST_LIBS         += -lm

$(eval $(call image,APP_TOWER_RPC_TEST))

# vim: set ft=make noet ts=2: