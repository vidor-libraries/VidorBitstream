PROJECT_NAME=${PWD##*/}
if [ -d software/softcore ]; then
echo create ram + flash app.ttf
elf2flash --input=build/software/$PROJECT_NAME/$PROJECT_NAME.elf --output=build/output_files/$PROJECT_NAME.flash --base=0x008E0000 --end=0x008FFFFF --verbose --save
nios2-elf-objcopy --input-target srec --output-target binary build/output_files/$PROJECT_NAME.flash build/output_files/$PROJECT_NAME.bin
../../TOOLS/makeCompositeBinary/makeCompositeBinary -i build/output_files/$PROJECT_NAME.ttf:1:512,build/output_files/$PROJECT_NAME.bin:0:896 -o build/output_files/app.ttf -t 1 > build/output_files/signature.h
else
echo create ram app.ttf
../../TOOLS/makeCompositeBinary/makeCompositeBinary -i build/output_files/$PROJECT_NAME.ttf:1:512 -o build/output_files/app.ttf -t 1 > build/output_files/signature.h
fi
