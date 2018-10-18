PROJECT_NAME=${PWD##*/}
elf2flash --input=software/$PROJECT_NAME/$PROJECT_NAME.elf --output=output_files/$PROJECT_NAME.flash --base=0x008E0000 --end=0x008FFFFF --verbose --save
nios2-elf-objcopy --input-target srec --output-target binary output_files/$PROJECT_NAME.flash output_files/$PROJECT_NAME.bin
../../TOOLS/makeCompositeBinary/MakeCompositeBinary -i output_files/$PROJECT_NAME.ttf:1:512,output_files/$PROJECT_NAME.bin:0:896 -o output_files/app.ttf -t 1 >output_files/signature.h
