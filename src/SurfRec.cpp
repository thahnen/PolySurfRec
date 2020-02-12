//
// Created by thahnen on 05.02.20.
//

#include <CGAL/poisson_surface_reconstruction.h>

#include "SurfRec.h"


/// Runs polygonal surface reconstruction from given file and outputs it to new file
ECODE SurfRec::polygonalReconstruction(std::string& path, struct SurfRec::options& algOptions) {
    // 1) Check if options are well formatted!
    if ((
            // Shape detection options should be given if no shapes in file
            !algOptions.shapesGiven && !algOptions.shapeDet
        ) || (
            // Detail options should be given if no detail level given
            algOptions.detail.level == DETAIL::USER && !algOptions.detail.details
        )) return ECODE::SR_WRONG_OPTIONS;

    // 2) Load input from file
    std::vector<PNI> points;

    ECODE status;
    if ((status = File_Handling::readPointsFromFile(points, path, algOptions.inputFormat)) != ECODE::SUCCESS) {
        return status;
    }

    // 3) Shape detection (if needed)
    if (!algOptions.shapesGiven) {
        if (algOptions.shapeDet->ransac) {
            // TODO: change return type
            SurfRec::Shape_Detection::ransac(points);
        } else {
            // TODO: change return type
            SurfRec::Shape_Detection::region_growing(points,
                                                     reinterpret_cast<rg_params&>(algOptions.shapeDet->regGrow));
        }
    }

    // 4) Surface reconstruction
    CGAL::Surface_mesh<Point> model;
    if ((status = polygonalReconstruction(points, model, algOptions.detail)) != ECODE::SUCCESS) {
        return status;
    }

    // 5) Save output to file
    return File_Handling::writeModelToFile(model, path, algOptions.outputFormat);
}


/// Runs polygonal surface reconstruction from given points and outputs to given model
ECODE SurfRec::polygonalReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model,
                                        struct SurfRec::sr_options& level) {
    Polygonal_surface_reconstruction algorithm(
        points, Point_map(), Normal_map(), Plane_index_map()
    );

    bool ret = false;
    if (level.level == DETAIL::MOST) {
        ret = algorithm.reconstruct<MIP_Solver>(model, 0.8, 0.15, 0.05);
    } else if (level.level == DETAIL::NORMAL) {
        ret = algorithm.reconstruct<MIP_Solver>(model);
    } else if (level.level == DETAIL::LESS) {
        ret = algorithm.reconstruct<MIP_Solver>(model, 0.3, 0.2, 0.5);
    } else if (level.level == DETAIL::LEAST) {
        ret = algorithm.reconstruct<MIP_Solver>(model, 0.2, 0.1, 0.7);
    } else {
        // Check if level.details points to valid
        if (level.details) {
            ret = algorithm.reconstruct<MIP_Solver>(
                model,
                level.details->fitting,
                level.details->coverage,
                level.details->complexity
            );
        }
    }

    if (ret) return SUCCESS;

    std::cerr << "[SurfRec::polygonalReconstruction] Solver error: " << algorithm.error_message() << std::endl;
    return SR_POLY_RECON_FAIL;
}


/// Runs poisson surface reconstruction from given points and outputs to given model
// TODO: maybe add indicator for level of detail like for polygon surface reconstruction
// TODO: evaluate return value of "CGAL::poisson_surface_reconstruction_delaunay" function
ECODE SurfRec::poissonReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model) {
    double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>(
            points,
            6,
            CGAL::parameters::point_map(Point_map())
    );

    if (CGAL::poisson_surface_reconstruction_delaunay(
            points.begin(), points.end(),
            Point_map(), Normal_map(),
            model, average_spacing)) {
        return SR_POISSON_FAIL;
    }

    return SUCCESS;
}