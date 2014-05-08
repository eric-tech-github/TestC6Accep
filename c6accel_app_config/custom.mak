## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/c6accel_app_xv5T.ov5T \
  package/cfg/c6accel_app_xv5T.xdl
	$(SED) 's"^\"\(package/cfg/c6accel_app_xv5Tcfg.cmd\)\"$""\"/home/dvsdk/dm3730/DVSDK4_2/c6accel_1_01_00_06/soc/app/c6accel_app_config/\1\""' package/cfg/c6accel_app_xv5T.xdl > $@
