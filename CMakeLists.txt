cmake_minimum_required(VERSION 3.15)
project(SurfRec
        VERSION         1.5.5
        DESCRIPTION     "Implementation of multiple CGAL Surface Reconstruction algorithms used on city models"
        HOMEPAGE_URL    "https://github.com/thahnen/SurfRec"
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
add_library(${PROJECT_NAME}
        SHARED
            include/Error_Handling.h
            include/Definitions.h
            src/Shape_Detection.cpp
            src/File_Handling.cpp
            src/SurfRec.cpp)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
            VERSION ${PROJECT_VERSION}
            CXX_VISIBILITY_PRESET hidden
            PUBLIC_HEADER include/SurfRec.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            CGAL_USE_SCIP)

target_link_libraries(${PROJECT_NAME}
        stdc++fs
        ${SCIP_LIBRARIES}
        Eigen3::Eigen
        CGAL::CGAL)


########################################################################################################################
#
#           AFTER LIBRARY IS BUILD FIND IT!
#
########################################################################################################################
find_library(${PROJECT_NAME} REQUIRED)


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