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

# Feature Manifest

Here is a list of features I am working on:

* Pathing to define objects which move with regularity
* Semi-rigid shapes like particles which form a box by targeting some angle
* More control over the shape, color, and render effects of particles in config

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
./Verlet ../spec/fluid.csv
```

# Contents

* `CMakeLists.txt`: A CMake file desined for cross-platform compilation
* `main.cpp`: The driver file for the simulation with `SFML`
* `spec/`: Specification files for simulations
* `src/`: Header files defining particle behavior
* `font/`: The font for the HUD

# Resources

* [SFML](https://www.sfml-dev.org/)
* [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
