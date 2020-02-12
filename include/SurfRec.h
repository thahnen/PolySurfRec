//
// Created by thahnen on 05.02.20.
//

#pragma once
#ifndef POLYSURFREC_SURFREC_H
#define POLYSURFREC_SURFREC_H

#if defined (__GNUC__)
#   define DLL __attribute__ ((visibility("default")))
#else
#   error "No suitable Compiler found!"
#endif


#include <string>
#include "definitions.h"


namespace SurfRec {
    /*******************************************************************************************************************
     *
     *      1) POLYGONAL SURFACE RECONSTRUCTION
     *
     ******************************************************************************************************************/

    /**
     *  Runs polygonal surface reconstruction from given file and outputs it to new file
     *  => used when shapes given in input file
     * 
     *  @param path             path to file (output path := path + ".out")
     *  @param algOptions       the options used in the whole reconstruction process, start to finish
     *  @return                 SUCCESS if reconstruction was successful, an error otherwise
     */
    DLL ECODE polygonalReconstruction(std::string& path, struct SurfRec::options& algOptions);

    /**
     *  Runs polygonal surface reconstruction from given points and outputs to given model
     *  => used when shapes already detected (due to shape detection or given in input file)
     * 
     *  @param points           input points for reconstruction (after shape detection)
     *  @param model            output surface mesh
     *  @param level            level of detail, the reconstruction should be
     *  @return                 SUCCESS if reconstruction was successful, an error otherwise
     */
    DLL ECODE polygonalReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model,
                                        struct SurfRec::sr_options& level);

    /*******************************************************************************************************************
     *
     *      2) POISSON SURFACE RECONSTRUCTION
     *
     ******************************************************************************************************************/

    /**
     *  Runs poisson surface reconstruction from given points and outputs to given model
     *
     *  @param points           input points for reconstruction
     *  @param model            output surface mesh
     *  @return                 SUCCESS if reconstruction was successful, an error otherwise
     */
    DLL ECODE poissonReconstruction(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model);


    /*******************************************************************************************************************
     *
     *      3) SHAPE DETECTION ALGORITHM
     *
     ******************************************************************************************************************/
    namespace Shape_Detection {
        /**
         *  Efficient RANSAC for shape detection
         * 
         *  @param points       points used to find/ store shapes
         *  @return             SUCCESS if RANSAC ran successful, an error otherwise
         */
        DLL ECODE ransac(std::vector<PNI>& points);

        /**
         *  Region growing for shape detection using file specific parameter
         * 
         *  @param points       points used to find/ store shapes
         *  @param parameter    SUCCESS if Region Growing finished successful, an error otherwise
         */
        DLL ECODE region_growing(std::vector<PNI>& points, struct SurfRec::rg_params& parameter);
    }


    /*******************************************************************************************************************
     *
     *      4) FILE HANDLING
     *
     ******************************************************************************************************************/
    namespace File_Handling {
        /**
         *  Reads points (with properties) from a file in PLY or XYZ / OFF format
         *
         *  @param points           where to store the points
         *  @param filepath         path to the file to load from
         *  @param format           input format: PLY (user defined planes), XYZ / OFF (point cloud)
         *  @return                 SUCCESS, a error code otherwise
         */
        DLL ECODE readPointsFromFile(std::vector<PNI>& points, const std::string& filepath, SurfRec::FORMAT format);

        /**
         *  Writes a generated surface model to a file in PLY or XYZ / OFF format
         *
         *  @param model            the model to store in a file
         *  @param filepath         path to the file to save to
         *  @param format           output format: PLY (user defined planes), XYZ / OFF (point cloud)
         *  @return                 SUCCESS, a error code otherwise
         */
        DLL ECODE writeModelToFile(const CGAL::Surface_mesh<Point>& model, const std::string& filepath,
                                    SurfRec::FORMAT format);
    }
}


#endif //POLYSURFREC_SURFREC_H