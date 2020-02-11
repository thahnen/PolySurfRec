//
// Created by thahnen on 11.02.20.
//

#ifndef POLYSURFREC_DEFINITIONS_H
#define POLYSURFREC_DEFINITIONS_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/property_map.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_point_set.h>

#include <CGAL/SCIP_mixed_integer_program_traits.h>
#include <CGAL/Polygonal_surface_reconstruction.h>

#include "errorhandler.h"


/// 1) Typedefs for data handling
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef Kernel::Point_3                                         Point;
typedef Kernel::Vector_3                                        Vector;
typedef boost::tuple<Point, Vector, int>                        PNI;
typedef CGAL::Nth_of_tuple_property_map<0, PNI>                 Point_map;
typedef CGAL::Nth_of_tuple_property_map<1, PNI>                 Normal_map;
typedef CGAL::Nth_of_tuple_property_map<2, PNI>                 Plane_index_map;


/// 3.1) Shape detection: Typedefs for RANSAC
typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, std::vector<PNI>, Point_map, Normal_map>                     Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits>                                                                     Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>                                                                                Plane;
typedef CGAL::Shape_detection::Point_to_shape_index_map<Traits>                                                             Point_to_shape_index_map;

/// 3.2) Shape detection: Typedefs for Region Growing
typedef CGAL::Shape_detection::Point_set::Sphere_neighbor_query<Kernel, std::vector<PNI>, Point_map>                        Neighbor_query;
typedef CGAL::Shape_detection::Point_set::Least_squares_plane_fit_region<Kernel, std::vector<PNI>, Point_map, Normal_map>   Region_type;
typedef CGAL::Shape_detection::Region_growing<std::vector<PNI>, Neighbor_query, Region_type>                                Region_growing;

/// 3.3) Shape detection: Index map to store a mapping of regions found on given points
class Index_map {
public:
    // Default constructor
    Index_map() = default;

    // Basic constructor for mapping indizes
    template<typename PointRange>
    Index_map(const PointRange& points, const std::vector<std::vector<std::size_t>>& regions) :
            m_indices(new std::vector<int>(points.size(), -1)) {
        for (std::size_t i = 0; i < regions.size(); ++i) {
            for (const std::size_t idx : regions[i]) {
                (*m_indices)[idx] = static_cast<int>(i);
            }
        }
    }

    // Used as friend function for informational purposes
    inline friend int get(const Index_map& index_map, const std::size_t key) {
        const auto& indices = *(index_map.m_indices);
        return indices[key];
    }
private:
    std::shared_ptr<std::vector<int>> m_indices;
};


/// 4.1) Surface reconstruction: Typedefs for polygonal surface reconstruction
typedef CGAL::SCIP_mixed_integer_program_traits<double>         MIP_Solver;
typedef CGAL::Polygonal_surface_reconstruction<Kernel>          Polygonal_surface_reconstruction;


namespace SurfRec {
    /// 2) File handling: Different file formats
    enum FORMAT {
        PLY = 0,    // format with user defined planes
        XYZ,        // point cloud format
        OFF,        // point cloud format
    };


    /// 3.3) Structure to hold region detection parameters for file (shape detection)
    struct rg_params {
        Kernel::FT par1;    // search sphere radius ???
        Kernel::FT par2;    // maximum distance to plane ???
        Kernel::FT par3;    // maximum angle between points ???
        std::size_t par4;   // minimum region size ???
    };

    /// 3.4) Structure to hold shape detection options
    struct sd_options {
        bool ransac;                // ransac given or not
        struct rg_params* regGrow;  // region growing info if not ransac
    };


    /// 4.2) Surface reconstruction: Different levels of detail
    enum DETAIL {
        USER = 0,   // level of detail is defined directly by user
        MOST,       // highest level of detail
        NORMAL,     // normal level of detail (standard values given by the method)
        LESS,       // slightly less than normal level of detail
        LEAST       // lowest level of detail
    };

    /// 4.3) Structure to hold user defined options for the solver ?
    struct usr_detail {
        double fitting;     // weight for the data fitting
        double coverage;    // weight for the point coverage
        double complexity;  // weight for the model complexity
    };

    /// 4.4) Structure to hold informations about the level of detail for reconstruction
    struct sr_options {
        DETAIL level;               // indicates the level or a user given one
        struct usr_detail* details; // optional user given detail information (level == DETAIL::USER)
    };


    /// 5) Structure to hold information on whole reconstruction process including shape detection
    struct options {
        FORMAT inputFormat;             // input file format
        FORMAT outputFormat;            // output file format

        bool shapesGiven;               // are shapes already given in input
        struct sd_options* shapeDet;    // options for shape detection (if no shapes given directly)

        struct sr_options detail;       // information about the level of detail;
    };
}


#endif //POLYSURFREC_DEFINITIONS_H
