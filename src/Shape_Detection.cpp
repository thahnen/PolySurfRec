//
// Created by thahnen on 12.02.20.
//

#include "SurfRec.h"


/// Efficient RANSAC for shape detection
// TODO: maybe add plane regularization (https://cgal.geometryfactory.com/CGAL/doc/master/Shape_detection/Shape_detection_2efficient_RANSAC_and_plane_regularization_8cpp-example.html)
ECODE SurfRec::Shape_Detection::ransac(std::vector<PNI>& points) {
    Efficient_ransac ransac;
    ransac.set_input(points);

    // The only shape useful with city models are planes
    ransac.add_shape_factory<Plane>();

    // Detects the planes
    if (!ransac.detect()) {
        return SD_RANSAC_DETECT;
    }

    // Stores the plane index of each point as third element to the tuple
    Point_to_shape_index_map sim(points, ransac.planes());
    for (int i = 0; i < points.size(); ++i) {
        points[i].get<2>() = get(sim, i);
    }

    return SUCCESS;
}


/// Region growing for shape detection using file specific parameter
ECODE SurfRec::Shape_Detection::region_growing(std::vector<PNI>& points, struct SurfRec::rg_params& parameter) {
    Neighbor_query nq(points, parameter.par1);
    Region_type rt(points, parameter.par2, parameter.par3, parameter.par4);

    Region_growing rg(points, nq, rt);
    std::vector<std::vector<std::size_t>> regions;

    // Detects regions
    rg.detect(std::back_inserter(regions));

    // Stores the plane index of each point as third element to the tuple
    Index_map index_map(points, regions);
    for (int i = 0; i < points.size(); ++i) {
        points[i].get<2>() = get(index_map, i);
    }

    return SUCCESS;
}