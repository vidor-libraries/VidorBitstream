PROJECT_NAME=${PWD##*/}
qsys-generate --synthesis=VERILOG $PROJECT_NAME.qsys
source build_sw.sh
quartus_sh --flow compile $PROJECT_NAME

../../../TOOLS/makeCompositeBinary/MakeCompositeBinary -i output_files/$PROJECT_NAME.ttf:1:0 -o output_files/app.ttf -t 0
