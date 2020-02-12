//
// Created by thahnen on 24.01.20.
//

#ifndef POLYSURFREC_ERROR_HANDLING_H
#define POLYSURFREC_ERROR_HANDLING_H


/// Different error codes
enum ECODE{
    SUCCESS = 0,            // there was no error

    /// file handler error codes
    FH_LOAD_EXIST_FAIL,     // File Handling: input file does not exist or is no file
    FH_LOAD_OPEN_FAIL,      // File Handling: input file cannot be opened
    FH_LOAD_PLY_FAIL,       // File Handling: cannot read PLY file
    FH_LOAD_XYZ_FAIL,       // File Handling: cannot read XYZ file
    FH_LOAD_OFF_FAIL,       // File Handling: cannot read OFF file

    FH_SAVE_OPEN_FAIL,      // File Handling: output file cannot be opened
    FH_SAVE_PLY_FAIL,       // File Handling: cannot write PLY file
    FH_SAVE_XYZ_FAIL,       // File Handling: cannot write XYZ file
    FH_SAVE_OFF_FAIL,       // File Handling: cannot write OFF file

    SD_RANSAC_DETECT,       // Shape Detection: RANSAC could not detect shapes

    SR_WRONG_OPTIONS,       // Surface Reconstruction (Polygonal): wrong options given
    SR_POLY_RECON_FAIL,     // Surface Reconstruction (Polygonal): reconstruction using solver failed
    SR_POISSON_FAIL,        // Surface Reconstruction (Poisson): reconstruction failed
};


#endif //POLYSURFREC_ERROR_HANDLING_H
