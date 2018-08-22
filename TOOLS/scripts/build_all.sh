PROJECT_NAME=${PWD##*/}
qsys-generate --synthesis=VERILOG $PROJECT_NAME"_sys.qsys"
source build_launcher.sh
source build_sw.sh
quartus_sh --flow compile $PROJECT_NAME
source create_image.sh
