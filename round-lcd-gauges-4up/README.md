I recreated the Vivado project from the repo. 

1) launch Vivado 2020.2 or later, 

2) execute the tcl script in the 4up directory, 

3) add design sources and add the .bd file, 

4) create system wrapper, 

5) set the generated system wrapper verilog file as the top level design.

6) Run generate bitstream. 

7) Export hardware. Select include bitstream. 

8) Launch Vitis SDK 2020.2 or later. 

9) Create an empty workspace. 

10) file, import, vitis project exported .zip file 

11) browse and select vitis_exported_archive(dot)ide. 

12) select everything in the archive and import

13) right click on design_1_wrapper and select update hardware

14) select the .xsa hardware specification file from Vivado that was exported in step 7

15) right click on design_1_wrapper and select build

16) right click on hello_world_system and select build

17) right click on hello_world and select build

18) right click on hello_world and select run as launch on hardware single application debug
