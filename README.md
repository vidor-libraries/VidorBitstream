# VidorFPGA

This repository includes FPGA IP Blocks compatible with the Arduino Vidor family of products and is aimed to users already familiar with FPGA development process.
FPGA development using native tools, although encouraged, is not supported by Arduino as it is quite difficult to support. If you feel this challenge is for you please know that we can only provide very limited support as our main efforts will be targeted at providing a smooth experience within the Arduino IDE and Arduino Create through precompiled libraries and with the web tool that will provide an easy way to assemble IP blocks.

## Directory structure
The directory structure is summarized in the following table:

Directory  | Contents
---------- | --------
ip | source code for IP blocks
projects | sample project files for the various boards
constraints | constraint files for the various boards. includes pinout and timings
TOOLS | scripts and tools needed to generate FPGA images
distrib | Directory created by the toolchain during compilation. Contains Arduino libraries created by compiling projects 

## Things to know before getting started
Once again this repository is intended only for people already familiar with FPGA programming. At the moment the primary intent is to disclose IP block functionality and present the infrastructure we created so that potential contributors can start to evaluate it. As of today this repository does not contain full source code required to compile the released libraries as parts of it requires some more polishing both in terms of code and in terms of licensing (in some cases from third parties). 
Full examples of working FPGAs, along with instructions to create a library and access the FPGA, will be posted here but will not necessarily reflect the official images we are publishing.

**NOTE** depending on Quartus II version used (lite or standard) scripts will compile different versions. The reason is that standard edition, which requires licensing, takes advantage of IP blocks included in Quartus and in particular NIOS II/f which adds cache and allows much faster code execution from flash memory. Of course precompiled images from Arduino are compiled with standard edition so they will be much faster than the ones compiled with lite edition. Right now some APIs, especially the ones related to gfx, are too slow with lite edition so you may experience timeouts in jtag communication

**NOTE** if you're running Windows you will notice that running NIOS II Command shell all the files under VidorBitstream don't have any permission. in order to fix this run `chmod -R a+rw *` from the VidorBitstream directory. Failing to do this will cause scripts to fail and you may have trouble deleting files and directories. in any case issuing the above instruction will provide at any time permissions to delete anything.

## Getting started
The prerequisite to compile MKRVIDOR4000 board FPGA images is Quartus II 18.1 Lite or Standard which can be downloaded from Altera/Intel web site.
Once Quartus is installed you need to patch it as we had to modify some IP blocks to optimize their functionality. In order to do it you need to open NIOS II Command shell, move to the TOOLS/scripts directory and launch the `apply_quartus_patches.sh`.
Download the Go Programming Language and compile makeCompositeBinary, move to TOOLS/makeCompositeBinary and build make_composite_binary.go with go build -o makeCompositeBinary make_composite_binary.go.

**NOTE** This step is required only once

Once setup is complete you can open a NIOS II Command shell, add the TOOLS/scripts directory to your PATH environment variable and you're ready to compile a project.
To compile a project you simply have to change to its directory and run the `build_all.sh` script. 

**NOTE**  that the script has to be run from the project directory, e.g. VidorBitstream/projects/MKRVIDOR4000_graphics

The build_all script will create a directory with the same name of the project under the **distrib** directory. This directory contains the library to be loaded in Arduino IDE or in Arduino Create and includes the FPGA image along with all the required Arduino library files required to start up the system, along with the examples directory with sample sketches.
If you are interested in intermediate files, you will find them under the <project_dir>/build/output_files directory in the project folder which contains, among other things, the <project_name>.sof file that can be programmed via Quartus and USB Blaster.

**NOTE**  The <project_name>.ttf file you find under the <project_dir>/build/output_files directory can't be directly used in the Arduino environment. In order to use it you have to run the makeCompositeBinary tool. for details on it check out its [documentation](TOOLS/makeCompositeBinary/README.md)


Programming the FPGA is possible in various ways:
1. flashing the image along with Arduino code creating a library which incorporates the app.ttf file (found under the distrib directory)
1. programming the image in RAM through USB Blaster (this requires mounting the FPGA JTAG header). this can be done safely only when SAM D21 is in bootloader mode as in other conditions it may access JTAG and cause a contention
1. programming the image in RAM through the emulated USB Blaster via SAM D21 (use [USBBlaster library](https://github.com/vidor-libraries/USBBlaster) and SAMD core >= 1.6.25)

Other useful scripts are:

Script | Description
-------|------------
build_sw.sh | builds all the nios software components
build_all.sh | builds everything from scratch and produces app.ttf and signature.h under output_files
build_launcher.sh | builds the launcher image to be embedded in FPGA image
create_image.sh | creates app.ttf and signature.h from the existing precompiled image and binary files
update_fw.sh | updates nios firmware, embeds it in FPGA and creates app.ttf and signature.h without recompiling FPGA
assemble_library.sh | creates the Arduino library, using generated app.ttf, signature.h and relevant code from `arduino` subfolders (under `ip`)

**NOTE** To allow preparing a library without installing Quartus, the build script will try to use the precompiled bitstream located in ${PROJECT_FOLDER}/software/arduino/src

## Sample Projects
At the time of writing available projects are the following:

Project   | Description
----------|-------------
MKRVIDOR4000_graphics      | source code of the VidorGraphics library
MKRVIDOR4000_peripherals   | source code of the VidorPeripherals library
MKRVIDOR4000_template_mbox | empty project for who wants to use Arduino softcore based RPC mechanism (see [documentation](ip/RPC/doc/RPC_sw.adoc) ) to communicate between ARM and FPGA
MKRVIDOR4000_template_bare | empty project for who wants to experiment bare metal programming. with this template you will have to create your own communication mechanism with the FPGA
MKRVIDOR4000_pacman        | you guessed it. unfortunately we can't (yet) release the ip behind this so compilation will fail as the git submodule is not public

## Anatomy of a project
Each project directory (located under VidorBitstream/projects) contains the following sub-directories:

Directory | Contents
----------|-------------
build     | Contains the environment used for Quartus compilation and is automatically created by the build scripts
project   | Contains the Quartus project (qpf) and settings (qsf) files along with platform builder (qsys), config and ipx files (details below)
rtl       | Contains the top level file of the project and all FPGA source code that does not belong to a component (ip)
scripts   | Contains the scripts required to customize compilation flow for the softcore (BSP and application)
software  | Contains the software source files to create the softcore main application and the Arduino library

### project sub-directory
This is the main directory your custom project should start from. Most of the times you'll just copy the qpf and qsf files from other projects as these contain just the board related settings. Depending on the template chosen you may find also a qsys file which contains an assembly of components created with Intel's Platform Builder. 
More advanced users would tweak the qsf file which contains settings to tweak compilation flow for speed/area optimization and much more.
The ip.ipx file, also contained in here is used to instruct quartus to search for ip blocks in the ip directory in the root of this environment and can be used to add project specific ip directories in case you prefer to keep them separate

### rtl sub-directory
Here you're supposed to have the top level file of your FPGA and eventual other source code that you don't have already packetized in an ip block. In the simplest projects, top level file contains the definition of FPGA pins and basic stuff such as the PLL that generates all clocks required in the FPGA. More complex projects instantiate the subdesign automatically generated by platform builder (see qsys file) and assignments for pin muxing.

### scripts sub-directory
Files contained in this folder are used to instruct Intel tools on how to customize the BSP for the softcore. If you're using a bare metal template you won't have this directory as there won't be a softcore at all. Altering these files involves deeper knowledge of what you're doing so before getting there it's maybe good to read some Intel documentation.

### software sub-directory
In here you will find the source code for the softcore (unless you're starting from a bare template) and for the Arduino side.

#### Arduino
Arduino folder contains the skeleton files required to assemble the arduino library that will be generated in the distrib directory once the compilation is complete. library.properties contains metadata about the library name, purpose, author etc.. In the src subdirectory you will find all the required files to initialize the FPGA and eventually additional code you may need to have your FPGA do its job. In the examples subdirectory you can store your sample sketches that demonstrate how the FPGA works. Note that the final library contains many more files as if you use the Arduino infrastructure, each component will automatically add its own piece of softcore and Arduino code, taken from each ip directory.

#### Softcore
The softcore folder contains the source code for the soft processor (NIOS) running on the FPGA. This processor is used to execute on the FPGA the calls done from the Arduino side. The underlying idea is that drivers for each component in the FPGA is executed on the softcore so that Arduino code just calls high level functions with low CPU overhead.

Note that when building softcore code two projects are generated: the launcher and the application. launcher is the same for every project and is used only to perform some basic initialization, if needed, and it runs only from FPGA internal RAM. application instead is (generally) contained in external QSPI flash and (for now) is executed in place, so it runs slower than the launcher, unless you're using NIOS II/f (which requires a license) which has support for cache. For timing critical functions it's still possible have some code copied from QSPI flash to internal RAM or even to SDRAM, however for the latter care should be taken as executing NIOS code from SDRAM will steal quite a lot of bandwidth from it and it would make it impossible to implement video applications.

**NOTE** if you want to modify/recompile softcore code in a graphical environment you can use NIOS II Software Build Tools for Eclipse. In order to import projects in Eclipse you should create a workspace (possibly pointing to the <Project_dir>/build/software). The created workspace will be empty as Eclipse requires its metadata in order to index projects. So to import the generated projects you have to use file->import from the dialog. Choose Nios II Software Build Tools Project/Import Nios II Software Build Tools Project and click next. In the following page browse to the <Project_dir>/build/software/<bsp> (eg. MKRVIDOR4000_template_mbox/build/software/MKRVIDOR4000_template_mbox_bsp) directory and use the directory name as project name (eg. MKRVIDOR4000_template_mbox) and finally click on finish.
You will have to repeat the same steps for all the four projects (application, launcher and relative BSPs). Once this is done you can use Eclipse in the same way you would do normally (check Intel documentation) and you can even debug the softcore by using the USBBlaster sketch.
