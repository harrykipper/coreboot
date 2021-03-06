##
## This file is part of the coreboot project.
##
## Copyright (C) 2012 secunet Security Networks AG
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##

romstage-y += dock.c

subdirs-y += variants/$(VARIANT_DIR)/

ramstage-y += dock.c
ramstage-y += cstates.c
ramstage-y += blc.c

ramstage-$(CONFIG_MAINBOARD_USE_LIBGFXINIT) += gma-mainboard.ads
