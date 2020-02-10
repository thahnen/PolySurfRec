#include "SurfRec.h"


/// Runs polygonal surface reconstruction from given file and outputs it to new file
void SurfRec::polygonalReconstruction(std::string& path, options& algOptions) {
    ECODE status;

    // 1) Check if options are well formatted!
    if ((
            // Shape detection options should be given if no shapes in file
            !algOptions.shapesGiven && !algOptions.shapeDet
        ) || (
            // Detail options should be given if no detail level given
            algOptions.detail.level == DETAIL::USER && !algOptions.detail.details
        )) return;

    // 2) Load input from file
    std::vector<PNI> points;
    if ((status = loadPointsFromFile(points, path, algOptions.inputFormat)) != ECODE::SUCCESS) {
        return status;
    }

    // 3) Shape detection (if needed)
    if (!algOptions.shapesGiven) {
        if (algOptions.shapeDet->ransac) {
            // TODO: change return type
            SurfRec::Shape_Detection::ransac(points);
        } else {
            // TODO: change return type
            SurfRec::Shape_Detection::region_growing(points, algOptions.shapeDet->regGrow);
        }
    }

    // 4) Surface reconstruction
    CGAL::Surface_mesh<Point> model
    polygonalReconstruction(points, model, algOptions.detail);

    // 5) Save output to file
    if ((status = writeModelToFile(model, path, algOptions.outputFormat)) != ECODE::SUCCESS) {
        return status;
    }

    return status;
}


/// Runs polygonal surface reconstruction from given points and outputs to given model
void SurfRec::polygonalReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model,
                                        struct sr_options& level) {
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

    return ret;
}