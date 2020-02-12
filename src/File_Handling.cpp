//
// Created by thahnen on 24.01.20.
//

#include <utility>
#include <iostream>
#include <sys/stat.h>
#include <filesystem>

#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/read_ply_points.h>
#include <CGAL/IO/read_xyz_points.h>

#include "SurfRec.h"


/**
 *  Checks if given path exists
 *
 *  @param path             the path to test on existence
 *  @return                 whether the path exists
 */
bool exists(const char* path) {
    struct stat buf;
    return (stat(path, &buf) == 0);
}


/**
 *  Checks if given path is an existing file
 *
 *  @param path             the path to test on existence
 *  @return                 whether the file exists
 */
bool isFile(const char* path) {
    if (!exists(path))
        return false;

    namespace fs = std::filesystem;
    std::string spath(path);

    try {
        fs::path pathObj(spath);
        if (fs::is_regular_file(pathObj))
            return true;
    } catch (fs::filesystem_error& e) { }

    return false;
}


/// Loads points (with properties) from a file in PLY or XYZ / OFF format
ECODE SurfRec::File_Handling::readPointsFromFile(std::vector<PNI>& points, const std::string& filepath, SurfRec::FORMAT format) {
    if (!isFile(filepath.c_str())) {
        // File does not exist or is no file
        return ECODE::FH_LOAD_EXIST_FAIL;
    }

    std::ifstream input(filepath);
    if (input.fail()) {
        // Input file cannot be opened!
        return ECODE::FH_LOAD_OPEN_FAIL;
    }

    switch (format) {
        case FORMAT::PLY:
            if (!CGAL::read_ply_points_with_properties(input, std::back_inserter(points),
                    CGAL::make_ply_point_reader(Point_map()),
                    CGAL::make_ply_normal_reader(Normal_map()),
                    std::make_pair(
                            Plane_index_map(),
                            CGAL::PLY_property<int>("segment_index")))) {
                // Cannot read file!
                return ECODE::FH_LOAD_PLY_FAIL;
            }
            break;
        case FORMAT::XYZ:
            if (!CGAL::read_xyz_points(input, std::back_inserter(points),
                    CGAL::parameters::point_map(Point_map()).normal_map(Normal_map()))) {
                // Cannot read file!
                return ECODE::FH_LOAD_XYZ_FAIL;
            }
            break;
        case FORMAT::OFF:
            // OFF format not supported yet!
            return ECODE::FH_LOAD_OFF_FAIL;
    }

    return ECODE::SUCCESS;
}


/// Writes a generated surface model to a file in PLY or OFF format
ECODE SurfRec::File_Handling::writeModelToFile(const CGAL::Surface_mesh<Point>& model, const std::string& filepath, SurfRec::FORMAT format) {
    std::ofstream output(filepath);
    if (output.fail()) {
        // File cannot be opened
        return ECODE::FH_SAVE_OPEN_FAIL;
    }

    switch (format) {
        case FORMAT::PLY:
            if (!output || !CGAL::write_ply(output, model)) {
                // Cannot write file
                return ECODE::FH_SAVE_PLY_FAIL;
            }
            break;
        case FORMAT::XYZ:
            // XYZ format not supported yet!
            return ECODE::FH_SAVE_XYZ_FAIL;
        case FORMAT::OFF:
            if (!output || !CGAL::write_off(output, model)) {
                // Cannot write file
                return ECODE::FH_SAVE_OFF_FAIL;
            }
    }

    return SUCCESS;
}