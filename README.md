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
the collision algorithim move from $O(N^2)$ to approximately $O(N)$. On my laptop,
which is an x86 Linux machine, I can acheive good frame rates for about 5000
particles. Without these algorithimic improvements, I could only handle around
500 particles. I plan to add multithreading for better performance in the future!

 <img src="https://raw.githubusercontent.com/walkerjameschris/verlet-motion/main/img/particles.png" width="400"> 

# Requirements

This project depends on `SFML` to display the simulation window. This is an
excellent C++ library that makes displaying particles in C++ similarly
challenging to `pygame` in Python. Which is to say, not challenging at all!

# Contents

* `CMakeLists.txt`: A CMake file desined for cross-platform compilation
* `main.cpp`: The driver file for the simulation with `SFML`
* `src/particle.h`: The header file defining particle behavior

# Resources

* [SFML](https://www.sfml-dev.org/)
* [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
