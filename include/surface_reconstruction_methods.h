//
// Created by thahnen on 22.01.20.
//

#ifndef POLYSURFREC_SURFACE_RECONSTRUCTION_METHODS_H
#define POLYSURFREC_SURFACE_RECONSTRUCTION_METHODS_H

#include <CGAL/Polygonal_surface_reconstruction.h>
#include <CGAL/SCIP_mixed_integer_program_traits.h>

#include "filehandler.h"


/// Typedefs for polygonal surface reconstruction
typedef CGAL::SCIP_mixed_integer_program_traits<double>         MIP_Solver;
typedef CGAL::Polygonal_surface_reconstruction<Kernel>          Polygonal_surface_reconstruction;


/// Different levels of detail
enum DETAIL {
    MOST = 0,   // highest level of detail
    NORMAL,     // normal level of detail (standard values given by the method)
    LESS,       // slightly less than normal level of detail
    LEAST       // lowest level of detail
};


/**
 *  Reconstructs surfaces from given points and stores them in given model
 *
 *  @param points           the points used for reconstruction
 *  @param model            the output surface mesh
 *  @param level            the level of detail, the reconstruction should be
 *  @return                 true if reconstruction was successfull
 */
bool reconstructSurfaces(std::vector<PNI>& points, CGAL::Surface_mesh<Point>& model, DETAIL level);


#endif //POLYSURFREC_SURFACE_RECONSTRUCTION_METHODS_H
