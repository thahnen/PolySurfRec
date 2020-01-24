//
// Created by thahnen on 24.01.20.
//

#include "shape_detection_methods.h"


/// Runs the RANSAC algorithm on the given points to find planes and store them
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


/// Runs an region growing algorithm on the given points to find planes and store them
///  TODO: add file parameters to simple struct
void region_growing(std::vector<PNI>& points, const std::tuple<Kernel::FT, Kernel::FT, Kernel::FT, std::size_t>& file_parameters) {
    typedef Kernel::FT FT;

    FT search_sphere_radius, max_distance_to_plane, max_accepted_angle;
    std::size_t min_region_size;
    std::tie(search_sphere_radius, max_distance_to_plane, max_accepted_angle, min_region_size) = file_parameters;

    Neighbor_query nq(points, search_sphere_radius);
    Region_type rt(points, max_distance_to_plane, max_accepted_angle, min_region_size);
    dbg("Neighbor_query / Region_type created!");

    Region_growing rg(points, nq, rt);
    std::vector<std::vector<std::size_t>> regions;
    dbg("Region_growing/ regions created!");

    // Detects regions
    rg.detect(std::back_inserter(regions));
    dbg("Regions detected!");

    // Stores the plane index of each point as third element to the tuple
    Index_map index_map(points, regions);
    for (int i = 0; i < points.size(); ++i) {
        points[i].get<2>() = get(index_map, i);
    }
}