//
// Created by thahnen on 09.01.20.
//

#ifndef POLYSURFREC_FILEHANDLER_H
#define POLYSURFREC_FILEHANDLER_H

#include <utility>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/IO/read_ply_points.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/read_off_points.h>
#include <CGAL/IO/Writer_OFF.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef Kernel::Point_3                                         Point;
typedef Kernel::Vector_3                                        Vector;
typedef boost::tuple<Point, Vector, int>                        PNI;
typedef CGAL::Nth_of_tuple_property_map<0, PNI>                 Point_map;
typedef CGAL::Nth_of_tuple_property_map<1, PNI>                 Normal_map;
typedef CGAL::Nth_of_tuple_property_map<2, PNI>                 Plane_index_map;


/**
 *  Different file formats
 */
enum FORMAT {
    PLY = 0,    // format with user defined planes
    XYZ,        // point cloud format
    OFF,        // point cloud format
};


/**
 *  Loads points (with properties) from a file in PLY or XYZ / OFF format
 *
 *  @param points           where to store the points
 *  @param filepath         path to the file to load from
 *  @param format           input format: PLY (user defined planes), XYZ / OFF (point cloud)
 *  @return                 SUCCESS, a error code otherwise
 */
int loadPointsFromFile(std::vector<PNI>& points, const std::string& filepath, FORMAT format) {
    std::ifstream input(filepath.c_str());
    if (input.fail()) {
        // File cannot be opened
        return 1;
    }

    switch (format) {
        case PLY:
            if (!input || !CGAL::read_ply_points_with_properties(input, std::back_inserter(points),
                    CGAL::make_ply_point_reader(Point_map()),
                    CGAL::make_ply_normal_reader(Normal_map()),
                    std::make_pair(
                            Plane_index_map(),
                            CGAL::PLY_property<int>("segment_index")))) {
                // Cannot read file!
                return 2;
            }
            break;
        case XYZ:
            if (!input || !CGAL::read_xyz_points(input, std::back_inserter(points),
                    CGAL::parameters::point_map(Point_map()).normal_map(Normal_map()))) {
                // Cannot read file!
                return 3;
            }
            break;
        case OFF:
            // OFF format not supported yet!
            return 4;
    }

    return 0;
}


/**
 *  Writes a generated surface model to a file in PLY or OFF format
 *
 *  @param model            the model to store in a file
 *  @param filepath         path to the file to save to
 *  @param format           output format: PLY (user defined planes), OFF (point cloud)
 *  @return                 SUCCESS, a error code otherwise
 */
int writeModelToFile(const CGAL::Surface_mesh<Point>& model, const std::string& filepath, FORMAT format) {
    std::ofstream output(filepath.c_str());
    if (output.fail()) {
        // File cannot be opened
        return 1;
    }

    switch (format) {
        case PLY:
            if (!output || !CGAL::write_ply(output, model)) {
                // Cannot write file
                return 2;
            }
            break;
        case OFF:
            if (!output || !CGAL::write_off(output, model)) {
                // Cannot write file
                return 3;
            }
            break;
        default:
            // XYZ format not supported yet!
            return 4;
    }

    return 0;
}

#endif //POLYSURFREC_FILEHANDLER_H
