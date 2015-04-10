Author: Jordan Yu (v00727036)
Date:   April 9,2015
For:    CSC305 Assignment 3

INSTRUCTIONS:
To run this application you must copy the 'Data' directory into the build
folder that qt generates when you compile/build the application.

The data folder contains several resources such as:
    .obj files      - OBJ files which are used by the ray tracer
    materials.csv   - A csv files which contains all the materials


Features implemented in the ray tracer:

Basic Functionality
    Shadows
    Reflections
    Refraction (almost)
    Multiple Light Sources
    Arbitrary Camera Position

User Interface
    Interactive view for camera
    Two views allowing the user to compare opengl vs raytracer

Advanced Rendering
    Soft Shadows
    Area Light Source
    Anti-Alias
    Sphere in a room with 5 different coloured walls
    Reflective sphere

Modeling and Scene Graph
    Model a triangle mesh
    Import a .obj triangle mesh object and render (icosahedron)

Software Architecture
    Multithreaded ray-tracer
    Benchmarks ( see csc305_a3_mulithread.pdf)