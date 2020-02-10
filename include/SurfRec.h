#pragma once
#ifndef POLYSURFREC_SURFREC_H
#define POLYSURFREC_SURFREC_H

#if defined (__GNUC__)
#   define DLL __attribute__ ((visibility("default")))
#else
#   error "No suitable Compiler found!"
#endif


#include <string>
#include "surface_reconstruction_methods.h"


namespace SurfRec {
    /// 1) Polygonal Surface Reconstruction

    /**
     *  Structure to hold region detection parameters for file (shape detection)
     */
    struct rg_params {
        Kernel::FT par1;    // search sphere radius ???
        Kernel::FT par2;    // maximum distance to plane ???
        Kernel::FT par3;    // maximum angle between points ???
        std::size_t par4;   // minimum region size ???
    }

    /**
     *  Structure to hold shape detection options
     */
    struct sd_options {
        bool ransac;                // ransac given or not
        struct rg_params* regGrow;  // region growing info if not ransac
    };

    /**
     *  Structure to hold user defined options for the solver ?
     */
    struct usr_detail {
        double fitting;     // weight for the data fitting
        double coverage;    // weight for the point coverage
        double complexity;  // weight for the model complexity
    }

    /**
     *  Structure to hold informations about the level of detail for reconstruction
     */
    struct sr_options {
        DETAIL level;               // indicates the level or a user given one
        struct usr_detail* details; // optional user given detail information (level == DETAIL::USER)
    }

    /**
     *  Structure to hold information on whole reconstruction process including shape detection
     */
    struct options {
        FORMAT inputFormat;             // input file format
        FORMAT outputFormat;            // output file format

        bool shapesGiven;               // are shapes already given in input
        struct sd_options* shapeDet;    // options for shape detection (if no shapes given directly)

        struct sr_options detail;       // information about the level of detail;
    }

    /**
     *  Runs polygonal surface reconstruction from given file and outputs it to new file
     *  => used when shapes given in input file
     * 
     *  @param path             path to file
     *  @param algOptions       the options used in the whole reconstruction process, start to finish
     */
    DLL void polygonalReconstruction(std::string& path, options& algOptions);

    /**
     *  Runs polygonal surface reconstruction from given points and outputs to given model
     *  => used when shapes already detected (due to shape detection or given in input file)
     * 
     *  @param points           input points for reconstruction (after shape detection)
     *  @param model            output surface mesh
     *  @param level            level of detail, the reconstruction should be
     */
    DLL void polygonalReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model,
                                        struct sr_options& level);


    namespace Shape_Detection {
        /**
         *  Efficient RANSAC for shape detection
         * 
         *  @param points       points used to find/ store shapes
         */
        DLL void ransac(std::vector<PNI>& points);

        /**
         *  Region growing for shape detection using file specific parameter
         * 
         *  @param points       points used to find/ store shapes
         *  @param parameter    file specific parameters
         */
        DLL void region_growing(std::vector<PNI>& points, struct rg_params& parameter);
    }
}


#endif //POLYSURFREC_SURFREC_H