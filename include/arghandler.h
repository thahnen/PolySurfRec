//
// Created by thahnen on 10.01.20.
//

#ifndef POLYSURFREC_ARGHANDLER_H
#define POLYSURFREC_ARGHANDLER_H

#include <regex>
#include <string>

#include "filehandler.h"
#include "surface_reconstruction_methods.h"


/// Different shape detection methods
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
    bool planes;                            // planes already given (false equals points)
    SHAPE_DETECTION_METHOD meth;            // shape detection method when points are given
    DETAIL level;                           // level of detail for surface reconstruction
};


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
 */
ECODE parseArguments(arguments& args, int argc, char* argv[]);


#endif //POLYSURFREC_ARGHANDLER_H
