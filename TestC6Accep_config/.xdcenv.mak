#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/dvsdk/dm3730/DVSDK4_2/c6accel_1_01_00_06/soc/packages;/home/dvsdk/dm3730/DVSDK4_2/codec-engine_2_26_02_11/packages;/home/dvsdk/dm3730/DVSDK4_2/framework-components_2_26_00_01/packages;/home/dvsdk/dm3730/DVSDK4_2/dsplink_1_65_00_02/packages;/home/dvsdk/dm3730/DVSDK4_2/dsplink_1_65_00_02;/home/dvsdk/dm3730/DVSDK4_2/xdais_6_26_01_03/packages;/home/dvsdk/dm3730/DVSDK4_2/linuxutils_2_26_01_02/packages;/home/dvsdk/dm3730/DVSDK4_2/local-power-manager_1_24_02_09/packages;/home/dvsdk/dm3730/DVSDK4_2/codecs-omap3530_4_02_00_00/packages
override XDCROOT = /home/dvsdk/dm3730/DVSDK4_2/xdctools_3_16_03_36
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = "prod"
override XDCTARGETS = ti.targets.C64P
endif
#
ifeq (0,1)
PKGPATH = /home/dvsdk/dm3730/DVSDK4_2/c6accel_1_01_00_06/soc/packages;/home/dvsdk/dm3730/DVSDK4_2/codec-engine_2_26_02_11/packages;/home/dvsdk/dm3730/DVSDK4_2/framework-components_2_26_00_01/packages;/home/dvsdk/dm3730/DVSDK4_2/dsplink_1_65_00_02/packages;/home/dvsdk/dm3730/DVSDK4_2/dsplink_1_65_00_02;/home/dvsdk/dm3730/DVSDK4_2/xdais_6_26_01_03/packages;/home/dvsdk/dm3730/DVSDK4_2/linuxutils_2_26_01_02/packages;/home/dvsdk/dm3730/DVSDK4_2/local-power-manager_1_24_02_09/packages;/home/dvsdk/dm3730/DVSDK4_2/codecs-omap3530_4_02_00_00/packages;/home/dvsdk/dm3730/DVSDK4_2/xdctools_3_16_03_36/packages;..
HOSTOS = Linux
endif
