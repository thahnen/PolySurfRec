cmake_minimum_required(VERSION 3.15)
project(PolySurfRec
        VERSION         1.1.0
        DESCRIPTION     "Implementation of an CGAL Polygonal Surface Reconstruction algorithm for city models"
        HOMEPAGE_URL    "https://github.com/thahnen/PolySurfRec"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 17)

# Eigen required for CGAL
find_package(Eigen3 3.3 REQUIRED)

# SCIP required for CGAL to work with polygonal surface reconstruction
# => an alternative would be GLPK
# => TODO: add GLPK support (also in C++)
find_package(SCIP REQUIRED)

# CGAL required for surface reconstruction
find_package(CGAL REQUIRED)

# Include directory
include_directories(./include)

# Adds files to the executable
add_executable(PolySurfRec
        include/filehandler.h
        include/arghandler.h
        include/shape_detection_methods.h
        main.cpp)

# Add preprocessor macro for C++
# TODO: add GLPK support
target_compile_definitions(PolySurfRec
        PUBLIC
            CGAL_USE_SCIP)

# Appends library to executable
target_link_libraries(PolySurfRec
        stdc++fs
        ${SCIP_LIBRARIES}
        Eigen3::Eigen
        CGAL::CGAL)