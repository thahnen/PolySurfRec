#include <CGAL/poisson_surface_reconstruction.h>

#include "SurfRec.h"
#include "filehandler.h"


/// Runs polygonal surface reconstruction from given file and outputs it to new file
ECODE SurfRec::polygonalReconstruction(std::string& path, struct SurfRec::options& algOptions) {
    ECODE status;

    // 1) Check if options are well formatted!
    if ((
            // Shape detection options should be given if no shapes in file
            !algOptions.shapesGiven && !algOptions.shapeDet
        ) || (
            // Detail options should be given if no detail level given
            algOptions.detail.level == DETAIL::USER && !algOptions.detail.details
        )) return status;

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
            SurfRec::Shape_Detection::region_growing(points,
                                                     reinterpret_cast<rg_params&>(algOptions.shapeDet->regGrow));
        }
    }

    // 4) Surface reconstruction
    CGAL::Surface_mesh<Point> model;
    polygonalReconstruction(points, model, algOptions.detail);

    // 5) Save output to file
    return writeModelToFile(model, path, algOptions.outputFormat);
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

    if (ret)    return SUCCESS;
    else        return SR_RECONSTRUCT_FAIL;
}


/// Runs poisson surface reconstruction from given points and outputs to given model
// TODO: maybe add indicator for level of detail like for polygon surface reconstruction
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