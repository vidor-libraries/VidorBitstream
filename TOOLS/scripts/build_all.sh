set -e

if [ `quartus_sh --version | grep Lite | wc -l` == "1" ]; then
# compile for lite version
if [ -z $FORCE_FULL ]; then
LITE="_lite"
fi
fi

STANDARD_PROJECT_NAME=${PWD##*/}
PROJECT_NAME=${STANDARD_PROJECT_NAME}$LITE

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

if [ -f build/${STANDARD_PROJECT_NAME}$LITE.qpf ]; then
PROJECT_NAME=${STANDARD_PROJECT_NAME}$LITE
else
PROJECT_NAME=${STANDARD_PROJECT_NAME}
fi

quartus_sh --flow compile build/$PROJECT_NAME
source create_image.sh
source assemble_library.sh
