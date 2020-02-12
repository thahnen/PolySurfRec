#ifndef CGAL_USE_SCIP
#   error "SCIP not found, use alternatively GLPK (which is not implemented yet)!"
#endif


#include <regex>
#include <iostream>
#include <SurfRec.h>


/**
 *  Runs a regular expression on given string with given flags
 *
 *  @param str              the string to test on
 *  @param expr             which test should be done
 *  @param flags            which flags should be used, defaults to std::regex_constants::icase
 *  @return                 true if test was successfull, false otherwise
 */
inline bool regex(const char* str, const char* expr, std::regex::flag_type flags = std::regex_constants::icase) {
    return std::regex_search(str, std::regex(expr, flags));
}


/**
 *  The main routine, running RANSAC shape detection and polygonal surface reconstruction on given file
 *
 *  Usage: ./PolySurfRec <Input file name> <Poly | Poisson> <Output file name>
 *
 *  @param argc             length of the arguments
 *  @param argv             list of all given arguments
 *  @return                 EXIT_SUCCESS on success, otherwise EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
    ECODE status;

    /// 1) Check arguments (input/ output file name)
    if (argc != 4) {
        std::cerr << "Not enough arguments given!"
                     << "Use: ./PolySurfRec <Input file name> <Poly | Poisson> <Output file name>" << std::endl;
        return EXIT_FAILURE;
    }

    if (!regex(argv[2], "(poly|poisson)", std::regex_constants::ECMAScript | std::regex_constants::icase)) {
        std::cerr << "Wrong surface reconstruction algorithm given!"
                    << "Use: ./PolySurfRec <Input file name> <Poly | Poisson> <Output file name>" << std::endl;
        return EXIT_FAILURE;
    }

    bool use_poly = regex(argv[2], "poly");

    std::string input(argv[1]);
    std::string output(argv[3]);


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


    /// 4) Surface reconstruction
    CGAL::Surface_mesh<Point> model;

    if (use_poly) {
        /// 4.1) Polygonal algorithm
        SurfRec::sr_options level_options(SurfRec::DETAIL::MOST);

        if ((status = SurfRec::polygonalReconstruction(points, model, level_options)) != ECODE::SUCCESS) {
            std::cerr << "There was an error using polygonal surface reconstruction: " << status << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Polygonal surface reconstruction done correctly!" << std::endl;
    } else {
        /// 4.2) Poisson algorithm
        if ((status = SurfRec::poissonReconstruction(points, model)) != ECODE::SUCCESS) {
            std::cerr << "There was an error using poisson surface reconstruction: " << status << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Poisson surface reconstruction done correctly!" << std::endl;
    }


    /// 5) Write model to file
    if ((status = SurfRec::File_Handling::writeModelToFile(model, output, SurfRec::FORMAT::OFF)) != ECODE::SUCCESS) {
        std::cerr << "There was an error saving to output file: " << status << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Model writing done correctly!" << std::endl;

    return EXIT_SUCCESS;
}
