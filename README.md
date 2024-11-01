# particular <a><img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/logo.png" align="right" height="138" /></a>

A particle simulator written in C++ using Verlet integration for collision and
motion. Verlet integreation solves for particle collisions by repeatedly moving
particles away from each other on their axis of collision. This happens several
times per frame per particle and is _extremely_ computationally intensive.

I made several prototypes in Python, but found that the language simply did not
have the necessary horsepower, desipite the use of `numba` and `numpy`. Thus,
I transitioned this to C++. I have some experience working with C++ through
writing extensions for R via `Rcpp`. However, a pure C++ project is an entirely
different beast. This was my first foray into a purely C++ codebase.

Once you build the simulation, you can control gravity with WASD keys. This
allows for some very pretty fluid dynamics! Currently, the simulation is single
threaded and uses a high-performance simulation grid system designed to make
the collision algorithim move from _O(N^2)_ to approximately _O(N)_. On my laptop,
which is an x86 Linux machine, I can acheive good frame rates for about 5000
particles. Without these algorithimic improvements, I could only handle around
500 particles.

# Build Instructions

This project depends on `SFML` to display the simulation window. You can visit
the SFML link below in _Resources_ for install instructions. Apart from this
one dependency, this follows a typical CMake workflow:

1. Create `build/` directory in this repo
2. Navigate to build and run `cmake ..`
3. Run `make`
4. Run the binary

On Linux that is (assuming you are in the base directory of this repo):

```sh
mkdir build
cd build
cmake ..
make
./particular ../spec/fluid.csv --pass --pass
```

> [!IMPORTANT]
> `particular` can only be run from the `build/` directory. This is because
> it loads a font file from `font/` for rendering the FPS counter while the
> simulation is running.

# Running Simulations

This repository comes with several simulations ready to go! You can find these
in the `spec/` directory. `particular` ingests three different types of files
when describing a simulation. These files are all comma-separated without
any column header. This means the first row in each file is data.

> [!IMPORTANT]  
> Every time you run `particular` you must provide a path to each of the
> three file types or put `--pass` in that position. For example, if I 
> wanted to use a specification file and no other files, I would put:

`./particular ../spec/fluid.csv --pass --pass`

### Specification Files

The most basic type of file is a _specification_ file. This is the most
fundamental file in any simulation. Each row corresponds to one particle
in a running simulation. `particular` is efficient enough to handle a
few thousand particles at once, but this depends on the performance of
your machine. It contains four columns described below:

| Column | Note | Example |
| ------ | ---- | ------- |
| 1 | The starting x position of a particle (0-1280) | `126.32` |
| 2 | The starting y position of a particle (0-720) | `322.12` |
| 3 | The index of a linked particle (-1 for no links) | `2` |
| 4 | Whether the particle should be fixed in one place (0 or 1) | `0` |

> [!TIP]
> If you want the first to particles to be linked, put -1 in the first
> row and 0 in the second row in column 3.

`./particular ../spec/fluid.csv --pass --pass`

<img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/spec.png" height="150" />

### Motion Files

This file type allows you to describe a particle with a motion path.
For example, suppose you wanted to describe a particle which moves back
and forth with regularity and crashes into other particles. You can
do that with motion files!

Unlike specification files, _motion_ files do not create a new particle
for each row in the file. Instead, each row describes the position of
a particle for one frame within that path ID. Once a path ends, it 
will restart as long as the simulation runs.

| Column | Note | Example |
| ------ | ---- | ------- |
| 1 | The motion path ID | `1` |
| 2 | The y position at this step | `322.12` |
| 3 | The x position at this step | `223.56` |

`./particular ../spec/fluid.csv ../spec/circle.csv --pass`

<img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/motion.png" height="150" />

### Softbody Files

Finally, we have softbody files. These files allow you to describe
softbodies where a softbody is a collection of particles which remain
at constant distance from each other to form semi-rigid shapes. For
example, a square, circle, or any other arbitrary shape!

Each row in this file is a particle which is part of a softbody ID.
Even though particles in a softbody will move with other particles in
the simulation, they will solve for distance within each other.

| Column | Note | Example |
| ------ | ---- | ------- |
| 1 | The softbody path ID | `1` |
| 2 | The y position at this step | `322.12` |
| 3 | The x position at this step | `223.56` |

> [!WARNING]  
> Softbodies are very computationally intensive, espescially for bodies
> with many particles.

`./particular ../spec/fluid.csv ../spec/circle.csv ..spec/square.csv`

<img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/softbody.png" height="150" />

# Feature Manifest

Here is a list of features I am working on:

* More control over the shape, color, and render effects of particles in config
* More control over the simulation state

# Contents

* `CMakeLists.txt`: A CMake file desined for cross-platform compilation
* `main.cpp`: The driver file for the simulation with `SFML`
* `spec/`: Specification files for simulations
* `src/`: Header files defining particle behavior
* `font/`: The font for the HUD

# Resources

* [SFML](https://www.sfml-dev.org/)
* [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
