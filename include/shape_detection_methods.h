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

/// Typedefs for RANSAC
typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, std::vector<PNI>, Point_map, Normal_map>                     Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits>                                                                     Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>                                                                                Plane;
typedef CGAL::Shape_detection::Point_to_shape_index_map<Traits>                                                             Point_to_shape_index_map;

/// Typedefs for Region Growing
typedef CGAL::Shape_detection::Point_set::Sphere_neighbor_query<Kernel, std::vector<PNI>, Point_map>                        Neighbor_query;
typedef CGAL::Shape_detection::Point_set::Least_squares_plane_fit_region<Kernel, std::vector<PNI>, Point_map, Normal_map>   Region_type;
typedef CGAL::Shape_detection::Region_growing<std::vector<PNI>, Neighbor_query, Region_type>                                Region_growing;


/**
 *  Stores an index mapping of regions found by region growing on given points
 */
class Index_map {
public:
    Index_map() = default;

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


/**
 *  Runs the RANSAC algorithm on the given points to find planes and store them
 *
 *  @param points           the points used in the calculation / where the planes are stored to
 */
void ransac(std::vector<PNI>& points) {
    Efficient_ransac ransac;
    ransac.set_input(points);
    ransac.add_shape_factory<Plane>();

    // Detects the planes
    ransac.detect();

    // Stores the plane index of each point as third element to the tuple
    Point_to_shape_index_map sim(points, ransac.planes());
    for (int i = 0; i < points.size(); ++i) {
        points[i].get<2>() = get(sim, i);
    }
}


/**
 *  Runs an region growing algorithm on the given points to find planes and store them
 *
 *  @param points           the points used in the calculation / where the planes are stored to
 *
 *  TODO: what is the meaning of the constant values copied from the example?
 */
void region_growing(std::vector<PNI>& points) {
    typedef Kernel::FT FT;

    const FT search_sphere_radius       = FT(2) / FT(100);
    const FT max_distance_to_plane      = FT(2) / FT(1000);
    const FT max_accepted_angle         = FT(25);
    const std::size_t min_region_size   = 200;

    Neighbor_query nq(points, search_sphere_radius);
    Region_type rt(points, max_distance_to_plane, max_accepted_angle, min_region_size);

    Region_growing rg(points, nq, rt);
    std::vector<std::vector<std::size_t>> regions;

    // Detects regions
    rg.detect(std::back_inserter(regions));

    // Stores the plane index of each point as third element to the tuple
    Index_map index_map(points, regions);
    for (int i = 0; i < points.size(); ++i) {
        points[i].get<2>() = get(index_map, i);
    }
}


#endif //POLYSURFREC_SHAPE_DETECTION_METHODS_H
