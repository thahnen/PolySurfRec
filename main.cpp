#include <iostream>

#include <CGAL/property_map.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygonal_surface_reconstruction.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/Writer_OFF.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_point_set.h>


#ifdef CGAL_USE_SCIP
#include <CGAL/SCIP_mixed_integer_program_traits.h>
typedef CGAL::SCIP_mixed_integer_program_traits<double>     MIP_Solver;
#else
#   error "SCIP not found, use alternatively GLPK!"
#endif

#include <CGAL/Timer.h>
#include <fstream>




int main(int argc, char* argv[]) {
    std::cout << "Everything is fine!" << std::endl;
    return EXIT_SUCCESS;
}
