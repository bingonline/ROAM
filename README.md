ROAM
====

This repository contains a simple implementation of the [ROAM](http://en.wikipedia.org/wiki/ROAM) algoritmh.

Code is mostly based on the gamasutra article 'Real-Time Dynamic Level of Detail Terrain Rendering with ROAM' by Bryan Turner [(link)](http://www.gamasutra.com/view/feature/131596/realtime_dynamic_level_of_detail_.php?print=1).

Building
========

This application has the following requirements:

 * SDL2
 * OpenGL 3.3
 * cmake

To build, you can execute the following commands:

    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

This builds an executable named ROAM in build directory

Usage
=====

Before you can use ROAM-executable, shaders must be linked to current directory (e.g. build), by symlinking or copying

    ln -s ../shaders .

Now you just need to supply the executable an appropriate heightmap of the form:

    <width> <height>
    <width amount of space separeted float values>
    ....
    <width amount of space separeted float values>

Where each row is on it's own row, totalling height amount of rows. A sample might be grabbed from [here](http://www.cs.helsinki.fi/u/jenijeni/ROAM/simplex_noise_1024.7z).

If you've gotten this far something like this might be displayed:

![Yay screen](https://raw.github.com/jesseniemisto/ROAM/master/screenshot.png)

Togge wireframe with number 1, move with wasd and look around with mouse.
