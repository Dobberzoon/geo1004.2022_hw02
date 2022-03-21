# Enriching the 3D BAG with new attributes

Assignment 2 for geo1004.2022


## Introduction 
With this program, the user can input cleaned/upgraded 3dbag files (of which one triangulated), and add new attributes to the 3dbag files. For the non-triangulated file the added attributes (added as semantics) will be area and orientation per roofSurface per building. For the triangulated file the added attributes will be volume and number of floors (no_floors) per building.


Authors: 		    Daniël Dobson, Irina Gheorghiu, Leo Kan, Carolin Bachert

Student numbers:	5152739, 5627834, 5505801, 5382998

## Preprocessing data instructions
Before running the project, the user has to clean and upgrade the 3dbag file of choice. Specifically, to upgrade the file's standard CityJSON version (1.0) to version 1.1. Also, the user should remove LoD 0, 1.1, and 1.3 and only keep 2.2. Furthermore, it is advised to clean metadata and duplicate vertices. We recommend to use ```cjio``` for this, as said upgrade and cleaning can be achieved by:
```
$ cjio 3dbag_v210908_fd2cee53_5910.json upgrade vertices_clean lod_filter 2.2 metadata_remove save myfile.city.json
``` 

More on ```cjio``` can be found here: https://github.com/cityjson/cjio

## Add own path for input dataset
Change path for input_data location at lines 45 & 50, to link an upgraded 3dbag file and upgraded triangulated 3dbag file, accordingly. 

## How to build from command line

Below is an example of how to build the project on Unix based machine, however to clone you need to ask permission first to access this private repo @Dobberzoon on github.com.

Make sure you check the CMakelists.txt, such that it finds the libraries from their relative paths accordingly. Edit where needed.

Get project from git:

```
$ git clone https://github.com/Dobberzoon/geo1004.2022_hw02.git
$ cd geo1004.2022_hw02/hw/02/cpp
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Or with zip file, extract and:

```
$ cd geo1004.2022_hw02/hw/02/cpp
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Now, to start the program from the build folder in cmd line:

```
$ ./hw02
```


## Alternative build: simply use IDE of choice (e.g. CLion 2021.3)

## Postprocessing data instructions

When project is run successfully, two files as mentioned in the introduction will be obtained (non-triangulated & triangulated, + new attributes). The user can choose to overwrite input files, or name each output file uniquely (advised). Nevertheless, to achieve a single CityJSON file containing all the new attributes a merge operation is necessary. Again, the use of ```cjio``` is recommended. The merge can be acieved by:

```
$ cjio non-triangulated.city.json merge triangulated.city.json save merged_newattributes.city.json
```

## Provided dataset

[placeholder]