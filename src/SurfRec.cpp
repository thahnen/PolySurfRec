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


/// Efficient RANSAC for shape detection
ECODE SurfRec::Shape_Detection::ransac(std::vector<PNI>& points) {
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

    return SUCCESS;
}


/// Region growing for shape detection using file specific parameter
// TODO: minimze, delete useless stuff
ECODE SurfRec::Shape_Detection::region_growing(std::vector<PNI>& points, struct SurfRec::rg_params& parameter) {
    Kernel::FT search_sphere_radius = parameter.par1;
    Kernel::FT max_distance_to_plane = parameter.par2;
    Kernel::FT max_accepted_angle = parameter.par3;
    std::size_t min_region_size = parameter.par4;

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

    return SUCCESS;
}