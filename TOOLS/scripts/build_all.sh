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
if [ -d software/softcore_launcher ] || [ -d software/softcore ]; then
source build_launcher.sh
fi
if [ -d software/softcore ]; then
source build_sw.sh
fi
quartus_sh --flow compile build/$PROJECT_NAME$LITE
source create_image.sh
source assemble_library.sh
