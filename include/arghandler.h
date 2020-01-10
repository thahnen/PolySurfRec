//
// Created by thahnen on 10.01.20.
//

#ifndef POLYSURFREC_ARGHANDLER_H
#define POLYSURFREC_ARGHANDLER_H

#include <regex>
#include <string>
#include <filesystem>

#include "filehandler.h"


/**
 *  Structure holding all necessary options extracted from command line arguments
 */
struct arguments {
    FORMAT input_format, output_format;
    std::string input_file, output_file;
};


/**
 *  Checks if given path is an existing file
 *
 *  @param path             the path to test on existence
 *  @return                 whether the file exists
 */
bool isFile(const char* path) {
    namespace fs = std::filesystem;
    std::string spath(path);

    try {
        fs::path pathObj(spath);
        if (fs::exists(pathObj) && fs::is_regular_file(pathObj))    return true;
    } catch (fs::filesystem_error& e) { }

    return false;
}


/**
 *  Parses the command line arguments and returns arguments for the program
 *
 *  @param args             the program arguments for later use
 *  @param argc             the length of command line arguments
 *  @param argv             the array of command line arguments
 *  @return                 SUCCESS, a error code otherwise
 */
int parseArguments(arguments& args, int argc, char* argv[]) {
    // There must atleast be an input given
    if (argc < 4) {
        // not enough arguments given!
        return 1;
    }

    std::regex input("-input", std::regex_constants::icase);
    if (!std::regex_search(argv[1], input)) {
        // No "-input" given
        return 1;
    }

    std::regex format("(ply|xyz|off)", std::regex_constants::ECMAScript | std::regex_constants::icase);
    if (!std::regex_search(argv[2], format)) {
        // No input format given
        return 2;
    }

    if (std::regex_search(argv[2], std::regex("ply", std::regex_constants::icase))) {
        args.input_format = FORMAT::PLY;
    } else if (std::regex_search(argv[2], std::regex("xyz", std::regex_constants::icase))) {
        args.input_format = FORMAT::XYZ;
    } else {
        args.input_format = FORMAT::OFF;
    }

    if (!isFile(argv[3])) {
        // No input file given
        return 3;
    }

    args.input_file = std::string(argv[3]);

    if (argc > 5) {
        // At least output format given!
        std::regex output("-output", std::regex_constants::icase);
        if (!std::regex_search(argv[4], input)) {
            // No "-output" given
            return 4;
        }

        std::regex format2("(ply|xyz|off)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        if (!std::regex_search(argv[5], format2)) {
            // No output format given
            return 5;
        }

        if (std::regex_search(argv[5], std::regex("ply", std::regex_constants::icase))) {
            args.output_format = FORMAT::PLY;
        } else if (std::regex_search(argv[5], std::regex("xyz", std::regex_constants::icase))) {
            args.output_format = FORMAT::XYZ;
        } else {
            args.output_format = FORMAT::OFF;
        }

        // TODO: check if last argument is a valid path!
        args.output_file = args.input_file + ".out.off";
    } else {
        // Default values for output!
        args.output_format = FORMAT::OFF;
        args.output_file = args.input_file + ".out.off";
    }

    return 0;
}


#endif //POLYSURFREC_ARGHANDLER_H
