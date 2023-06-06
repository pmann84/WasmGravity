# Gravity In WebAssembly

## Overview
An experimental project to write a small N-body simulator in C++ that is exported to WebAssembly.

## Todos
A list of things that I can think of that need doing and some stuff I want to do:
- Memory tops out at 2Gb - fix this!
- Add trails to particles for when you are doing simulations with lower numbers of bodies 
- Add visualisation stats to the web interface e.g. energy/number of bodies etc
- Improve simulator performance (to handle larger number of bodies)
- Add collisions - including generation of larger mass bodies from smaller ones
- 