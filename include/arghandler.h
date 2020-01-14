//
// Created by thahnen on 10.01.20.
//

#ifndef POLYSURFREC_ARGHANDLER_H
#define POLYSURFREC_ARGHANDLER_H

#include <regex>
#include <string>
#include <sys/stat.h>
#include <filesystem>

#include "filehandler.h"


enum SHAPE_DETECTION_METHOD {
    NONE = 0,       // None, used when planes given
    RANSAC,         // uses RANSAC algorithm, default value
    REGION_GROWING  // uses region growing
};


/**
 *  Structure holding all necessary options extracted from command line arguments
 */
struct arguments {
    FORMAT input_format, output_format;     // format for input and output
    std::string input_file, output_file;    // path for input and output
    bool planes;                            // planes already given (points if not)
    SHAPE_DETECTION_METHOD meth;            // shape detection method when points are given
};


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


/**
 *  Runs a regular expression on given string with given flags
 *
 *  @param str              the string to test on
 *  @param expr             which test should be done
 *  @param flags            which flags should be used, defaults to std::regex_constants::icase
 *  @return                 true if test was successfull, false otherwise
 */
inline bool regex(const char* str, const char* expr, std::regex::flag_type flags = std::regex_constants::icase) {
    return std::regex_search(str, std::regex(expr, flags));
}


/**
 *  Parses the command line arguments and returns arguments for the program
 *
 *  @param args             the program arguments for later use
 *  @param argc             the length of command line arguments
 *  @param argv             the array of command line arguments
 *  @return                 SUCCESS, a error code otherwise
 *
 *  TODO: test if output folder exists / output path is file and is correct, even though it does not exist yet
 */
int parseArguments(arguments& args, int argc, char* argv[]) {
    /// Items given (File name + "-informat" + Format + "-infile" + Path + "-given" + Planes/ Points) => 7 minimum
    if (argc < 6) {
        // not enough arguments given!
        return 1;
    }

    // Input format
    if (!regex(argv[1], "-informat")) {
        // No "-informat" given
        return 1;
    }

    if (!regex(argv[2], "(ply|xyz|off", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
        // No input format given
        return 2;
    }

    if (regex(argv[2], "ply")) {
        args.input_format = FORMAT::PLY;
    } else if (regex(argv[2], "xyz")) {
        args.input_format = FORMAT::XYZ;
    } else {
        args.input_format = FORMAT::OFF;
    }

    // Input file path
    if (!regex(argv[3], "-infile")) {
        // No "-infile" given
        return 1;
    }

    if (!isFile(argv[4])) {
        // No input file given
        return 3;
    }

    args.input_file = std::string(argv[3]);

    // Given input type
    if (!regex(argv[5], "-given")) {
        // No "-given" given
        return 10;
    }

    if (!regex(argv[6], "(planes|points)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
        // No information about planes/ points given
        return 11;
    }

    args.planes = regex(argv[6], "planes");

    // Check if more is given (shape detection and output format)
    if (argc > 10) {
        // Shape detection method
        if(!regex(argv[7], "-shdetection")) {
            // No "-shdetection" given
        }

        if (!regex(argv[8], "(ransac|region_growing|none)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
            // No shape detection method given
            return 6;
        }

        if (regex(argv[8], "ransac")) {
            args.meth = SHAPE_DETECTION_METHOD::RANSAC;
        } else if (regex(argv[8], "region_growing")) {
            args.meth = SHAPE_DETECTION_METHOD::REGION_GROWING;
        } else {
            if (!args.planes) {
                // None given but that is wrong! Only allowed when planes are already given!
                return 11;
            }

            args.meth = SHAPE_DETECTION_METHOD::NONE;
        }

        // Output format
        if (!regex(argv[9], "-outformat")) {
            // No "-output" given
            return 4;
        }

        if (!regex(argv[10], "(ply|xyz|off)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
            // No output format given
            return 6;
        }

        if (regex(argv[10], "ply")) {
            args.output_format = FORMAT::PLY;
        } else if (regex(argv[10], "xyz")) {
            args.output_format = FORMAT::XYZ;
        } else {
            args.output_format = FORMAT::OFF;
        }
    } else {
        // Default values for output!
        args.output_format = FORMAT::OFF;
    }

    args.output_file = args.input_file + ".out.off";

    return 0;
}


#endif //POLYSURFREC_ARGHANDLER_H
