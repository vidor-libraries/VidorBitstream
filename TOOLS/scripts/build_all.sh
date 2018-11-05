set -e

if [ `quartus_sh --version | grep Lite | wc -l` == "1" ]; then
# compile for lite version
if [ -z $FORCE_FULL ]; then
LITE="_lite"
fi
fi

PROJECT_NAME=${PWD##*/}$LITE

mkdir -p build
cp -r project/* build/
if [ -f build/$PROJECT_NAME"_sys.qsys" ]; then
qsys-generate --synthesis=VERILOG build/$PROJECT_NAME"_sys.qsys"
fi
if [ -f software/softcore ]; then
source build_launcher.sh
source build_sw.sh
fi
quartus_sh --flow compile build/$PROJECT_NAME
source create_image.sh
