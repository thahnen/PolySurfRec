//
// Created by thahnen on 14.01.20.
//

#ifndef POLYSURFREC_SHAPE_DETECTION_METHODS_H
#define POLYSURFREC_SHAPE_DETECTION_METHODS_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>

#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_point_set.h>

#include "filehandler.h"

#define dbg(x) std::cout << x << std::endl


/// Typedefs for RANSAC
typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, std::vector<PNI>, Point_map, Normal_map>                     Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits>                                                                     Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>                                                                                Plane;
typedef CGAL::Shape_detection::Point_to_shape_index_map<Traits>                                                             Point_to_shape_index_map;

/// Typedefs for Region Growing
typedef CGAL::Shape_detection::Point_set::Sphere_neighbor_query<Kernel, std::vector<PNI>, Point_map>                        Neighbor_query;
typedef CGAL::Shape_detection::Point_set::Least_squares_plane_fit_region<Kernel, std::vector<PNI>, Point_map, Normal_map>   Region_type;
typedef CGAL::Shape_detection::Region_growing<std::vector<PNI>, Neighbor_query, Region_type>                                Region_growing;


/// Stores an index mapping of regions found by region growing on given points
class Index_map {
public:
    /// Default constructor
    Index_map() = default;

    /// Basic constructor for mapping indizes
    template<typename PointRange>
    Index_map(const PointRange& points, const std::vector<std::vector<std::size_t>>& regions) :
            m_indices(new std::vector<int>(points.size(), -1)) {
        for (std::size_t i = 0; i < regions.size(); ++i) {
            for (const std::size_t idx : regions[i]) {
                (*m_indices)[idx] = static_cast<int>(i);
            }
        }
    }

    /// Used as friend function for informational purposes
    inline friend int get(const Index_map& index_map, const std::size_t key) {
        const auto& indices = *(index_map.m_indices);
        return indices[key];
    }
private:
    std::shared_ptr<std::vector<int>> m_indices;
};


/**
 *  Runs the RANSAC algorithm on the given points to find planes and store them
 *
 *  @param points           the points used in the calculation / where the planes are stored to
 */
void ransac(std::vector<PNI>& points);


/**
 *  Runs an region growing algorithm on the given points to find planes and store them
 *
 *  @param points           the points used in the calculation / where the planes are stored to
 *  @param file_parameters  the file parameters for the input file used in region growing
 */
void region_growing(std::vector<PNI>& points, const std::tuple<Kernel::FT, Kernel::FT, Kernel::FT, std::size_t>& file_parameters);


#endif //POLYSURFREC_SHAPE_DETECTION_METHODS_H
