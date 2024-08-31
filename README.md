# verlet-motion

A particle simulator written in C++ using Verlet integration for collision and
motion. Verlet integreation solves for particle collisions by repeatedly moving
particles away from each other on their axis of collision. This happens several
times per frame per particle and is _extremely_ computationally intensive.

I made several prototypes in Python, but found that the language simply did not
have the necessary horsepower, desipite the use of `numba` and `numpy`. Thus,
I transitioned this to C++. I have some experience working with C++ through
writing extensions for R via `Rcpp`. However, a pure C++ project is an entirely
different beast. I also found that C++ was generally a pretty nice language to
work with _so long as_ you stick with less esoteric features. If you stick
to `for () {}` loops, basic types, `vectors` from the standard library, plus
some other quality of life things (e.g., namespaces and classes), it isn't too
bad!

# Contents

* `main.cpp`: The driver file for the simulation with `SFML`
* `particle.h`: The header file defining particle behavior
* `build.sh`: Build instructions (currently only Linux is supported)
