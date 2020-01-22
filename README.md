# PolySurfRec : Implementation of an CGAL Polygonal Surface Reconstruction algorithm

## Abstract

Software to run the provided algorithms with different modifications on provided data.
Also available as small dynamic library (coming later).

### Usability

Implemented using C++17!

```
./PolySurfRec -informat [PLY | XYZ | OFF] -infile <path> -given [planes | points] -shdetection [ransac | region_growing] -lod [MOST | NORMAL | LEAST | LESS] -outformat [PLY | XYZ | OFF]
```

---

## Prerequisites

Needs following software installed:
1. Some kind of **Unix**-based OS
2. [CMake](https://cmake.org/) to build the project
3. [Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page) for using CGAL
4. [SCIP 6.0.2](https://scip.zib.de/) for CGAL to work with polygonal surface reconstruction
5. [CGAL 5.0](https://www.cgal.org/) for surface reconstruction

---

## Name

Poly(gonal) Surf(ace) Rec(onstruction).
It's that simple!