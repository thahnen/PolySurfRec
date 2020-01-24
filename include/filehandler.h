//
// Created by thahnen on 09.01.20.
//

#ifndef POLYSURFREC_FILEHANDLER_H
#define POLYSURFREC_FILEHANDLER_H

#include <utility>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <filesystem>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/read_ply_points.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/read_off_points.h>
#include <CGAL/IO/Writer_OFF.h>

#include "errorhandler.h"


/// Typedefs for data handling
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef Kernel::Point_3                                         Point;
typedef Kernel::Vector_3                                        Vector;
typedef boost::tuple<Point, Vector, int>                        PNI;
typedef CGAL::Nth_of_tuple_property_map<0, PNI>                 Point_map;
typedef CGAL::Nth_of_tuple_property_map<1, PNI>                 Normal_map;
typedef CGAL::Nth_of_tuple_property_map<2, PNI>                 Plane_index_map;


/// Different file formats
enum FORMAT {
    PLY = 0,    // format with user defined planes
    XYZ,        // point cloud format
    OFF,        // point cloud format
};


/**
 *  Checks if given path exists
 *
 *  @param path             the path to test on existence
 *  @return                 whether the path exists
 */
bool exists(const char* path);


/**
 *  Checks if given path is an existing file
 *
 *  @param path             the path to test on existence
 *  @return                 whether the file exists
 */
bool isFile(const char* path);


/**
 *  Loads points (with properties) from a file in PLY or XYZ / OFF format
 *
 *  @param points           where to store the points
 *  @param filepath         path to the file to load from
 *  @param format           input format: PLY (user defined planes), XYZ / OFF (point cloud)
 *  @return                 SUCCESS, a error code otherwise
 */
ECODE loadPointsFromFile(std::vector<PNI>& points, const std::string& filepath, FORMAT format);


/**
 *  Writes a generated surface model to a file in PLY or OFF format
 *
 *  @param model            the model to store in a file
 *  @param filepath         path to the file to save to
 *  @param format           output format: PLY (user defined planes), OFF (point cloud)
 *  @return                 SUCCESS, a error code otherwise
 */
ECODE writeModelToFile(const CGAL::Surface_mesh<Point>& model, const std::string& filepath, FORMAT format);


#endif //POLYSURFREC_FILEHANDLER_H
