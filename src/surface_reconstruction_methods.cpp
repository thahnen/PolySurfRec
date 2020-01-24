//
// Created by thahnen on 24.01.20.
//

#include "surface_reconstruction_methods.h"


/// Reconstructs surfaces from given points and stores them in given model
bool reconstructSurfaces(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model, DETAIL level) {
    Polygonal_surface_reconstruction algorithm(
            points, Point_map(), Normal_map(), Plane_index_map()
    );

    bool ret = false;
    switch (level) {
        case MOST:
            ret = algorithm.reconstruct<MIP_Solver>(model, 0.8, 0.15, 0.05);
            break;
        case NORMAL:
            ret = algorithm.reconstruct<MIP_Solver>(model);
            break;
        case LESS:
            ret = algorithm.reconstruct<MIP_Solver>(model, 0.3, 0.2, 0.5);
            break;
        case LEAST:
            ret = algorithm.reconstruct<MIP_Solver>(model, 0.2, 0.1, 0.7);
    }

    if (!ret) {
        std::cerr << "[reconstructSurfaces] Error: " << algorithm.error_message() << std::endl;
    }

    return ret;
}