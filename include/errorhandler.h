//
// Created by thahnen on 24.01.20.
//

#ifndef POLYSURFREC_ERRORHANDLER_H
#define POLYSURFREC_ERRORHANDLER_H


/// Different error codes
// TODO: rename surface reconstruction errors to distinguish polygonal/ poisson
enum ECODE{
    SUCCESS = 0,            // there was no error

    /// file handler error codes
    FH_LOAD_EXIST_FAIL,     // File Handling: input file does not exist or is no file
    FH_LOAD_OPEN_FAIL,      // filehandler: input file cannot be opened
    FH_LOAD_PLY_FAIL,       // filehandler: cannot read PLY file
    FH_LOAD_XYZ_FAIL,       // filehandler: cannot read XYZ file
    FH_LOAD_OFF_FAIL,       // filehandler: cannot read OFF file

    FH_SAVE_OPEN_FAIL,      // filehandler: output file cannot be opened
    FH_SAVE_PLY_FAIL,       // filehandler: cannot write PLY file
    FH_SAVE_XYZ_FAIL,       // filehandler: cannot write XYZ file
    FH_SAVE_OFF_FAIL,       // filehandler: cannot write OFF file

    SD_RANSAC_DETECT,       // shape detection: RANSAC could not detect shapes

    SR_WRONG_OPTIONS,       // surface reconstruction: wrong options given for polygonal
    SR_RECONSTRUCT_FAIL,    // surface reconstruction: reconstruction using solver failed
    SR_POISSON_FAIL,        // surface reconstruction: poisson reconstruction failed
};


#endif //POLYSURFREC_ERRORHANDLER_H
