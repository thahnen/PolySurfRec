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
    FH_LOAD_OPEN_FAIL,      // filehandler: input file cannot be opened
    FH_LOAD_PLY_FAIL,       // filehandler: cannot read PLY file
    FH_LOAD_XYZ_FAIL,       // filehandler: cannot read XYZ file
    FH_LOAD_OFF_FAIL,       // filehandler: cannot read OFF file

    FH_SAVE_OPEN_FAIL,      // filehandler: output file cannot be opened
    FH_SAVE_PLY_FAIL,       // filehandler: cannot write PLY file
    FH_SAVE_XYZ_FAIL,       // filehandler: cannot write XYZ file
    FH_SAVE_OFF_FAIL,       // filehandler: cannot write OFF file

    SD_RANSAC_DETECT,       // shape detection: RANSAC could not detect shapes

    SR_RECONSTRUCT_FAIL,    // surface reconstruction: reconstruction using solver failed
    SR_POISSON_FAIL,        // surface reconstruction: poisson reconstruction failed

    /// argument handler error codes
    AH_TO_LITTLE_PARAMS,    // argument handler: not enough parameter given
    AH_NO_INFORMAT,         // argument handler: no "-informat" given
    AH_WRONG_INFORMAT,      // argument handler: wrong input format
    AH_NO_INFILE,           // argument handler: no "-infile" given
    AH_WRONG_INFILE,        // argument handler: wrong input path
    AH_NO_TYPE_GIVEN,       // argument handler: no "-given" given
    AH_WRONG_TYPE_GIVEN,    // argument handler: wrong input data type
    AH_NO_SHDETECTION,      // argument handler: no "-shdetection" given
    AH_WRONG_SHDETECTION,   // argument handler: wrong shape detection method
    AH_MISS_SHDETECTION,    // argument handler: no shape detection given when using points
    AH_NO_LOD,              // argument handler: no "-lod" given
    AH_WRONG_LOD,           // argument handler: wrong level of detail
    AH_NO_OUTFORMAT,        // argument handler: no "-outformat" given
    AH_WRONG_OUTFORMAT,     // argument handler: wrong output format
};


#endif //POLYSURFREC_ERRORHANDLER_H
