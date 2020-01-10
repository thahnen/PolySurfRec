#ifndef CGAL_USE_SCIP
#   error "SCIP not found, use alternatively GLPK!"
#endif


#include <iostream>

#include <CGAL/property_map.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygonal_surface_reconstruction.h>
#include <CGAL/SCIP_mixed_integer_program_traits.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_point_set.h>

#include "arghandler.h"

typedef CGAL::SCIP_mixed_integer_program_traits<double>         MIP_Solver;

typedef Kernel::FT                                              FT;
typedef CGAL::Polygonal_surface_reconstruction<Kernel>          Polygonal_surface_reconstruction;

typedef CGAL::Shape_detection::Point_set::Sphere_neighbor_query<Kernel, std::vector<PNI>, Point_map>                        Neighbor_query;
typedef CGAL::Shape_detection::Point_set::Least_squares_plane_fit_region<Kernel, std::vector<PNI>, Point_map, Normal_map>   Region_type;
typedef CGAL::Shape_detection::Region_growing<std::vector<PNI>, Neighbor_query, Region_type>                                Region_growing;

typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, std::vector<PNI>, Point_map, Normal_map>     Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits>                                                     Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>                                                                Plane;
typedef CGAL::Shape_detection::Point_to_shape_index_map<Traits>                                             Point_to_shape_index_map;


class Index_map {
public:
    Index_map() { }

    template<typename PointRange>
    Index_map(const PointRange& points, const std::vector<std::vector<std::size_t>>& regions) :
            m_indices(new std::vector<int>(points.size(), -1)) {
        for (std::size_t i = 0; i < regions.size(); ++i) {
            for (const std::size_t idx : regions[i]) {
                (*m_indices)[idx] = static_cast<int>(i);
            }
        }
    }

    inline friend int get(const Index_map& index_map, const std::size_t key) {
        const auto& indices = *(index_map.m_indices);
        return indices[key];
    }
private:
    std::shared_ptr<std::vector<int>> m_indices;
};



/**
 *  The main routine, running an polygonal surface reconstruction algorithm on given data
 *
 *  Usage: ./PolySurfRec -input [PLY | XYZ | OFF] <input file> -output [PLY | XYZ | OFF] {output file}
 *          => Ouput defaults to OFF format / input file name + ".out.off"
 *
 *
 *  @param argc             length of the arguments
 *  @param argv             list of all given arguments
 *  @return                 EXIT_SUCCESS on success, otherwise EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
    /// 1) Check arguments (input file, input format, output file, output format
    arguments args;
    if (parseArguments(args, argc, argv) != 0) {
        // There was an error parsing the arguments!
        std::cerr << "There was an error parsing the arguments!" << std::endl;
        return EXIT_FAILURE;
    }

    /// 2) Handle different
    /// 2) Load Points
    std::vector<PNI> points;
    if (loadPointsFromFile(points, args.input_file, args.input_format) != 0) {
        // There was an error loading from file!
        std::cerr << "There was an error loading from file!" << std::endl;
        return EXIT_FAILURE;
    }

    CGAL::Surface_mesh<Point> model;

    /// X) Save points
    if (writeModelToFile(model, args.output_file, args.output_format) != 0) {
        // There was an error writing to file!
        std::cerr << "There was an error writing to file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Everything is fine!" << std::endl;
    return EXIT_SUCCESS;
}
