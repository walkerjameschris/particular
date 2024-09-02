# verlet-motion

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
500 particles. I plan to add multithreading for better performance in the future!

 <img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/particle_ex.png" width="400"> 

# Build Instructions

This project depends on `SFML` to display the simulation window. You can visit
the SFML link below in _Resources_ for install instructions. Apart from this
one dependency, this follows a typical CMake workflow:

1. Create `build/` directory in this repo
2. Navigate to build and run `cmake ..`
3. Run `make`
4. Run the binary called `Verlet`

On Linux that is (assuming you are in the base directory of this repo):

```sh
mkdir build
cd build
cmake ..
make
./Verlet
```

Once runing, you can control gravity with _WASD_ and turn gravity off with _Z_. You
can also hold _I_ for realtime information about the simulation like FPS, etc.

# Contents

* `CMakeLists.txt`: A CMake file desined for cross-platform compilation
* `main.cpp`: The driver file for the simulation with `SFML`
* `src/particle.h`: The header file defining particle behavior

# Resources

* [SFML](https://www.sfml-dev.org/)
* [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
