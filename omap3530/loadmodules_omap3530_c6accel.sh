#!/bin/sh
#
# Copyright (C) $year Texas Instruments Incorporated - http://www.ti.com/
#
# This program is free software; you can redistribute it and/or modify 
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation version 2.1 of the License.
#
# This program is distributed #as is# WITHOUT ANY WARRANTY of any kind,
# whether express or implied; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# remove previously loaded cmem to ensure that it configured to use our pool configuration
rmmod cmemk 2>/dev/null

# Allocate 15MB for CMEM
modprobe cmemk phys_start=0x86300000 phys_end=0x87200000 pools=20x4096 allowOverlap=1
modprobe dsplinkk
modprobe lpm_omap3530

# make /dev/dsplink
rm -rf /dev/dsplink
mknod /dev/dsplink c `awk "\\$2==\"dsplink\" {print \\$1}" /proc/devices` 0

