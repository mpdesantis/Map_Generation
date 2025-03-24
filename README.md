# README
* Author: Michael De Santis
* CUID: 101213450
* Date: 2025/03/23

## Description
Cell Discrete Event System Specification (Cell-DEVS) Model and Simulation for the Procedural Content Generation (PCG) of a game map. For additional information, please see the `doc/` directory.

## Contents
This repository's contents.

* `bin/`
    - Generated directory containing built executables for this repository.
* `build/`
    - Generated directory containing build files for this repository.
* `build_sim.sh`
    - Shell script defining build rules for host-architecture target.
* `cadmium_v2/`
    - Cadmium V2 repository `git` submodule.
* `CMakeLists.txt`
    - CMake build instructions for this repository.
* `doc/`
    - Directory containing documentation for this repository.
        1. Report: `Map_Generation.pdf`
        2. Model Source Article: `wu-map-gen-from-ca.pdf`
* `env.sh`
    - Shell script defining additional environment variables for this repository.
* `example-output/`
    - Directory containing sample output logs from model simulation, accompanied by the corresponding visualization files (images and videos).
* `img/`
    - Directory containing images for this repository, as included inline in the project report.
* `main/`
    - C++ source and header files for this repository's main code module, `map_generation`.
* `output/`
    - Generated directory containing output logs from model simulation.
* `run_all_examples.sh`
    - Shell script to run all available simulation examples from pre-defined configuration.
* `test/`
    - C++ header files for this repository's tests.
    
## Usage
Usage instructions for this project and repository.

### Installing and Configuring Cadmium V2
In order to compile this repository's source code, your system must have [Cadmium V2](https://github.com/Sasisekhar/cadmium_v2) available on your system, and accessible via the environment variable `$CADMIUM`. This library, and its own dependencies, are included within this repository as a `git` submodule. To ensure that your system's submodules are up-to-date prior to compilation, issue the following command:
```sh
git submodule update --init --recursive
```
Once the `cadmium_v2` sudmodule directory is available on your file system, enter that directory and follow the instructions provided therein for installation and configuration.  Once complete, you should be able to verify that your configuration is correct with the output of the following command:
```sh
$ echo $CADMIUM
<path-to>/Game_Map/cadmium_v2/include
```
If the above environment variable is discrepant on your system, you can overwrite it by sourcing `env.sh` in this directory with the following command:
```sh
$ . env.sh
```

### Build
To build this project, issue the following command:
```sh
$ . build_sim.sh
```
Find the resultant binary at `bin/map_generation`.

### Execute Custom Simulation
Tp see a list of runtime options and defaults for this project's binary, issue the following command:
```sh
$ ./bin/map_generation [-h | --help]
```
For example, an invocation with all three parameters receiving arguments may look as follows:
```sh
$ ./bin/map_generation \
  --config config/mapgen_config.json \    # specify input configuration file (JSON)
  --output output/mapgen_grid_log.csv \   # specify output log file (CSV)
  --duration 15                           # specify simulation duration (time units)
```

### Execute Examples
To execute this project's example simulations, issue the following command:
```sh
$ ./run_all_examples.sh
```
Upon successful execution, corresponding output for each simulation binary will be found in the `output/` directory.

### Visualizing the Results
Carleton's DEVSsim [Cell-DEVS Viewer](https://devssim.carleton.ca/cell-devs-viewer://devssim.carleton.ca/cell-devs-viewer/) may be used to visualize the simulations executed by this project's binary. To ensure your files are recognized by the visualizer, constrain your file names as follows:
1. The JSON simulation configuration file:
    * `<name>_config.json`
2. The JSON simulation visualization configuration file:
    * `<name>Visualization_config.json`
3. The CSV output grid log file:
    * `<name>_grid_log.csv`

## Notes
* With permission and by instruction, this project uses the [blank project template](https://github.com/Sasisekhar/blank_project_rt) provided in the Cadmium V2 manual.
* With permission and by instruction, this project is informed by the [sample project](https://github.com/Sasisekhar/cell-devs-manual-example) provided in the Cadmium V2 manual.
* With permission, this project is informed by and reuses parts of the author's original work for the [LEO User Link](https://github.com/mpdesantis/LEO_User_Link).
* Thanks Professor Wainer!! 
