# PIOSS

A Parallel I/O and Storage System (PIOSS) simulation model for the analysis of spatial aspects of load balance in Parallel File Systems.
PIOSS simulates a scenario with multiple processes generating write requests to a shared or multiple independent files into a Parallel File System (PFS).
At the end of the simulation, the load distribution across PFS data servers are presented.

## Requirements

1. PIOSS is written in C, and, thus, requires a C compiler (C99 or higher);
2. The build system is [CMake](http://cmake.org) (version 2.8 or higher).

## Build Instructions

1. Clone PIOSS repository to your local machine:

    ```
    git clone git@github.com:ecamiloinacio/pioss.git
    ```	

2. Create a build directory and the Makefile:

    ```
    mkdir build
    cd build
    ccmake ..
    ```

3. Make PIOSS:

    ```
    make
    ```

## Running PIOSS

To run PIOSS, execute in the PIOSS build directory:

    ```
    ./pioss [options]
    ```

### Options

`--num_cli=NUM | -n NUM`: number of I/O generating processes (default: 1)

`--data_size=NUM | -b NUM`: size (in bytes) of the contiguous block of data transferred by each I/O process (default: 1 MiB = 1048576)

`--shared | -s`: all I/O processes transfer data to a single shared file (default: each I/O process transfers to an independent file)

`--num_dts=NUM | -m NUM`: number of data servers (default: 2)

`--stripe_width=NUM | -w NUM`: number of data servers per file; must be smaller or equal to `--num_dts` (default: 1)

`--stripe_size=NUM | -z NUM`: size of a file stripe fragment (default: 64 KiB = 65536)

`--file_dist=STR | -f STR`: data server load balancer; currently two options are available: "`Random`", and "`RoundRobin`" (default: "`Random`")

`--rng_seed=NUM | -s NUM`: seed for load balancer RNG

`--out_path=STR | -o STR`: append output statistics to the file (default: no output)

`--quiet | -d`: do not display parameters and results (default: false)

`--help | -h`: display command-line options

## Remarks

PIOSS is a work-in-progress. Although it is not supposed to cause any damage to your system, use it at your own risk.
