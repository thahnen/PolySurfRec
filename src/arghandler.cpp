//
// Created by thahnen on 24.01.20.
//

#include "arghandler.h"


/// Parses the command line arguments and returns arguments for the program
/// TODO: test if output folder exists / output path is file and is correct, even though it does not exist yet
ECODE parseArguments(arguments& args, int argc, char* argv[]) {
    // Items given (File name + "-informat" + Format + "-infile" + Path + "-given" + Planes/ Points) => 7 minimum
    if (argc < 6) {
        // not enough arguments given!
        return AH_TO_LITTLE_PARAMS;
    }

    // Input format
    if (!regex(argv[1], "-informat")) {
        // No "-informat" given
        return AH_NO_INFORMAT;
    }

    if (!regex(argv[2], "(ply|xyz|off)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
        // No input format given
        return AH_WRONG_INFORMAT;
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
        return AH_NO_INFILE;
    }

    if (!isFile(argv[4])) {
        // No input file given
        return AH_WRONG_INFILE;
    }

    args.input_file = std::string(argv[4]);

    // Given input type
    if (!regex(argv[5], "-given")) {
        // No "-given" given
        return AH_NO_TYPE_GIVEN;
    }

    if (!regex(argv[6], "(planes|points)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
        // No information about planes/ points given
        return AH_WRONG_TYPE_GIVEN;
    }

    args.planes = regex(argv[6], "planes");

    // Check if more is given (shape detection, level of detail and output format)
    if (argc > 12) {
        // Shape detection method
        if(!regex(argv[7], "-shdetection")) {
            // No "-shdetection" given
            return AH_NO_SHDETECTION;
        }

        if (!regex(argv[8], "(ransac|region_growing|none)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
            // No shape detection method given
            return AH_WRONG_SHDETECTION;
        }

        if (regex(argv[8], "ransac")) {
            args.meth = SHAPE_DETECTION_METHOD::RANSAC;
        } else if (regex(argv[8], "region_growing")) {
            args.meth = SHAPE_DETECTION_METHOD::REGION_GROWING;
        } else {
            if (!args.planes) {
                // None given but that is wrong! Only allowed when planes are already given!
                return AH_MISS_SHDETECTION;
            }

            args.meth = SHAPE_DETECTION_METHOD::NONE;
        }

        // Level of detail
        if (!regex(argv[9], "-lod")) {
            // No "-lod" given
            return AH_NO_LOD;
        }

        if (!regex(argv[10], "(most|normal|least|less)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
            // No level of detail given
            return AH_WRONG_LOD;
        }

        if (regex(argv[10], "most")) {
            args.level = DETAIL::MOST;
        } else if (regex(argv[10], "normal")) {
            args.level = DETAIL::NORMAL;
        } else if (regex(argv[10], "least")) {
            args.level = DETAIL::LEAST;
        } else {
            args.level = DETAIL::LESS;
        }

        // Output format
        if (!regex(argv[11], "-outformat")) {
            // No "-outformat" given
            return AH_NO_OUTFORMAT;
        }

        if (!regex(argv[12], "(ply|xyz|off)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
            // No output format given
            return AH_WRONG_OUTFORMAT;
        }

        if (regex(argv[12], "ply")) {
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

    return SUCCESS;
}