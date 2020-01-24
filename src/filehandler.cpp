//
// Created by thahnen on 24.01.20.
//

#include "filehandler.h"


/// Checks if given path exists
bool exists(const char* path) {
    struct stat buf;
    return (stat(path, &buf) == 0);
}


/// Checks if given path is an existing file
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
ECODE loadPointsFromFile(std::vector<PNI>& points, const std::string& filepath, FORMAT format) {
    std::ifstream input(filepath);
    if (input.fail()) {
        // Input file cannot be opened!
        return FH_LOAD_OPEN_FAIL;
    }

    switch (format) {
        case PLY:
            if (!CGAL::read_ply_points_with_properties(input, std::back_inserter(points),
                    CGAL::make_ply_point_reader(Point_map()),
                    CGAL::make_ply_normal_reader(Normal_map()),
                    std::make_pair(
                            Plane_index_map(),
                            CGAL::PLY_property<int>("segment_index")))) {
                // Cannot read file!
                return FH_LOAD_PLY_FAIL;
            }
            break;
        case XYZ:
            if (!CGAL::read_xyz_points(input, std::back_inserter(points),
                    CGAL::parameters::point_map(Point_map()).normal_map(Normal_map()))) {
                // Cannot read file!
                return FH_LOAD_XYZ_FAIL;
            }
            break;
        case OFF:
            // OFF format not supported yet!
            return FH_LOAD_OFF_FAIL;
    }

    return SUCCESS;
}


/// Writes a generated surface model to a file in PLY or OFF format
ECODE writeModelToFile(const CGAL::Surface_mesh<Point>& model, const std::string& filepath, FORMAT format) {
    std::ofstream output(filepath.c_str());
    if (output.fail()) {
        // File cannot be opened
        return FH_SAVE_OPEN_FAIL;
    }

    switch (format) {
        case PLY:
            if (!output || !CGAL::write_ply(output, model)) {
                // Cannot write file
                return FH_SAVE_PLY_FAIL;
            }
            break;
        case XYZ:
            // XYZ format not supported yet!
            return FH_SAVE_XYZ_FAIL;
        case OFF:
            if (!output || !CGAL::write_off(output, model)) {
                // Cannot write file
                return FH_SAVE_OFF_FAIL;
            }
    }

    return SUCCESS;
}