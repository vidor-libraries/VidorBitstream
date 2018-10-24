set -e

if [ `quartus_sh --version | grep Lite | wc -l` == "1" ]; then
# compile for lite version
if [ -z $FORCE_FULL ]; then
LITE="_lite"
fi
fi

PROJECT_NAME=${PWD##*/}$LITE
qsys-generate --synthesis=VERILOG $PROJECT_NAME"_sys.qsys"
source build_launcher.sh
source build_sw.sh
quartus_sh --flow compile $PROJECT_NAME
source create_image.sh
