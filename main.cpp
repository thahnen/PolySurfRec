#ifndef CGAL_USE_SCIP
#   error "SCIP not found, use alternatively GLPK!"
#endif


#include <iostream>

#include "arghandler.h"
#include "shape_detection_methods.h"
#include "surface_reconstruction_methods.h"


/**
 *  The main routine, running an polygonal surface reconstruction algorithm on given data
 *
 *  Usage: ./PolySurfRec -informat [PLY | XYZ | OFF] -infile <path> -given [planes | points] -shdetection [ransac | region_growing] -lod [MOST | NORMAL | LEAST | LESS] -outformat [PLY | XYZ | OFF]
 *          => shape detection defaults to the RANSAC algorithm => not implemented yet!
 *          => output format defaults to OFF                    => not implemented yet!
 *
 *  TODO: when using region growing default parameter values for file must be given!
 *
 *  @param argc             length of the arguments
 *  @param argv             list of all given arguments
 *  @return                 EXIT_SUCCESS on success, otherwise EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
    int status;

    /// 1) Check arguments (input file, input format, output file, output format
    arguments args;
    if ((status = parseArguments(args, argc, argv)) != 0) {
        // There was an error parsing the arguments!
        std::cerr << "There was an error parsing the arguments: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Arguments parsed correctly!" << std::endl;

    /// 2) Handle different input formats
    if (args.input_format == FORMAT::OFF) {
        // Not supported yet!
        return EXIT_FAILURE;
    }
    std::cout << "Input format correct!" << std::endl;

    /// 3) Load from file
    std::vector<PNI> points;
    if ((status = loadPointsFromFile(points, args.input_file, args.input_format)) != 0) {
        std::cerr << "There was an error loading from file: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Points loaded correctly!" << std::endl;

    /// 4) Handle points if given
    if (!args.planes) {
        try {
            switch (args.meth) {
                case SHAPE_DETECTION_METHOD::RANSAC:
                    // RANSAC algorithm
                    ransac(points);
                    break;
                case SHAPE_DETECTION_METHOD::REGION_GROWING:
                    // region growing algorithm (using example parameter values)
                    std::tuple<Kernel::FT, Kernel::FT, Kernel::FT, std::size_t> example(
                            Kernel::FT(2) / Kernel::FT(100),
                            Kernel::FT(2) / Kernel::FT(1000),
                            Kernel::FT(25),
                            200
                    );

                    region_growing(points, example);
            }
        } catch (...) {
            std::exception_ptr p = std::current_exception();
            std::cerr << "Shape detection error: " << (p ? p.__cxa_exception_type()->name() : "null") << std::endl;
            return EXIT_FAILURE;
        }


        std::cout << "Shape detection on points done correctly!" << std::endl;
    }

    /// 5) Surface reconstruction
    CGAL::Surface_mesh<Point> model;
    if (!reconstructSurfaces(points, model, args.level)) {
        // Reconstruction failed
        std::cerr << "Surface reconstruction failed!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Surface reconstruction done correctly" << std::endl;

    /// 6) Save points
    if (writeModelToFile(model, args.output_file, args.output_format) != 0) {
        // There was an error writing to file!
        std::cerr << "There was an error writing to file!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Output points saved correctly!" << std::endl;

    return EXIT_SUCCESS;
}
