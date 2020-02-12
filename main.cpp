#ifndef CGAL_USE_SCIP
#   error "SCIP not found, use alternatively GLPK!"
#endif


#include <iostream>
#include <SurfRec.h>


/**
 *  The main routine, running RANSAC shape detection and polygonal surface reconstruction on given file
 *
 *  Usage: ./PolySurfRec <Input file name> <Output file name>
 *
 *  @param argc             length of the arguments
 *  @param argv             list of all given arguments
 *  @return                 EXIT_SUCCESS on success, otherwise EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
    ECODE status;

    /// 1) Check arguments (input/ output file name)
    if (argc != 3) {
        std::cerr << "Not enough arguments given. Use: ./PolySurfRec <Input file name> <Output file name>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input(argv[1]);
    std::string output(argv[2]);

    /// 2) Read points from file
    std::vector<PNI> points;
    if ((status = SurfRec::File_Handling::readPointsFromFile(points, input, SurfRec::FORMAT::XYZ)) != ECODE::SUCCESS) {
        std::cerr << "There was an error reading from input file: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Points reading done correctly!" << std::endl;

    /// 3) Shape detection using RANSAC
    if ((status = SurfRec::Shape_Detection::ransac(points)) != ECODE::SUCCESS) {
        std::cerr << "There was an error using RANSAC for shape detection: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "RANSAC shape detection done correctly!" << std::endl;

    /// 4) Surface reconstruction using polygonal algorithm
    CGAL::Surface_mesh<Point> model;
    SurfRec::sr_options level_options(SurfRec::DETAIL::MOST);

    if ((status = SurfRec::polygonalReconstruction(points, model, level_options)) != ECODE::SUCCESS) {
        std::cerr << "There was an error using polygonal surface reconstruction: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Polygonal surface reconstruction done correctly!" << std::endl;

    /// 5) Write model to file
    if ((status = SurfRec::File_Handling::writeModelToFile(model, output, SurfRec::FORMAT::OFF)) != ECODE::SUCCESS) {
        std::cerr << "There was an error saving to output file: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Model writing done correctly!" << std::endl;

    return EXIT_SUCCESS;
}
