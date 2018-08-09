PROJECT_NAME=${PWD##*/}
qsys-generate --synthesis=VERILOG $PROJECT_NAME.qsys
source build_sw.sh
quartus_sh --flow compile $PROJECT_NAME
