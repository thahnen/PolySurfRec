cmake_minimum_required(VERSION 3.15)
project(PolySurfRec
        VERSION         1.5.0
        DESCRIPTION     "Implementation of multiple CGAL Surface Reconstruction algorithms used on city models"
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

########################################################################################################################
#
#           SHARED LIBRARY (using Eigen/ SCIP/ CGAL)
#
########################################################################################################################
add_library(SurfRec
        SHARED
            include/Error_Handling.h
            include/Definitions.h
            src/Shape_Detection.cpp
            src/File_Handling.cpp
            src/SurfRec.cpp)

set_target_properties(SurfRec
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/SurfRec.h)

target_include_directories(SurfRec
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(SurfRec
        PUBLIC
            CGAL_USE_SCIP)

target_link_libraries(SurfRec
        stdc++fs
        ${SCIP_LIBRARIES}
        Eigen3::Eigen
        CGAL::CGAL)


########################################################################################################################
#
#           AFTER LIBRARY IS BUILD FIND IT!
#
########################################################################################################################
find_library(SurfRec REQUIRED)


########################################################################################################################
#
#           TEST FILE (USING SPECIAL POINT CLOUDS)
#           TODO: rename test!
#
########################################################################################################################
# Adds files to the executable
add_executable(PolySurfRec
        main.cpp)

# Add preprocessor macro for C++
# TODO: add GLPK support
target_compile_definitions(PolySurfRec
        PUBLIC
            CGAL_USE_SCIP)

# Appends library to executable
target_link_libraries(PolySurfRec
        SurfRec)