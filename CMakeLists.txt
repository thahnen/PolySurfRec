cmake_minimum_required(VERSION 3.15)
project(PolySurfRec
        VERSION         1.0.0
        DESCRIPTION     "Implementation of an CGAL Polygonal Surface Reconstruction algorithm for city models"
        HOMEPAGE_URL    "https://github.com/thahnen/PolySurfRec"
        LANGUAGES       CXX)

set(CMAKE_CXX_STANDARD 14)

# Eigen required for CGAL
find_package(Eigen3 3.3 REQUIRED)

# SCIP required for CGAL to work with polygonal surface reconstruction
# => an alternative would be GLPK
find_package(SCIP REQUIRED)

# CGAL required for surface reconstruction
find_package(CGAL REQUIRED)

#
add_executable(PolySurfRec main.cpp)

#
target_compile_definitions(PolySurfRec
        PUBLIC
            CGAL_USE_SCIP)

#
target_link_libraries(PolySurfRec
        ${SCIP_LIBRARIES}
        Eigen3::Eigen
        CGAL::CGAL)