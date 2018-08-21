PROJECT_NAME=${PWD##*/}
source build_sw.sh
quartus_cdb --update_mif $PROJECT_NAME
quartus_asm $PROJECT_NAME
source create_image.sh