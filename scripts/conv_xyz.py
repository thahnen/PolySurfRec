#!/usr/bin/env python3
# -*- coding: utf-8 -*-


# ================================================================================
#
#                   Converts valid XYZ files to work with CGAL
#
#       => normal files contain information in following order per line:
#               X Y Z [...] NX NY NZ, where
#           - (X|Y|Z) is the point position,
#           - [...] may contain the color (R|G|B) and the scalar field,
#           - (NX|NY|NZ) is the normal vector of the position
#
#       => output should look like this:
#               X Y Z NX NY NZ
# ================================================================================


import re
import os
import sys




# ================================================================================
#                       Error codes returned by the program
# ================================================================================
ERR_NO_PARAM = 1
ERR_PARAM_WRONG = 2
ERR_NO_XYZ_FILES = 3
ERR_LINES_WRONG = 4
ERR_POS_NO_FLOATS = 5
ERR_NORM_NO_FLOATS = 6



# ================================================================================
#                   Checks if given path ends with ".xyz"
# ================================================================================
def checkXYZFile(path):
    return bool(
        re.search(r'(?i).xyz$', path)
    )



# ================================================================================
#                                   MAIN-ROUTINE:
#
#   1) Check for file/ folder
#   2) For every file remove unnecessary information
#   3) Write (X|Y|Z) position vector + (NX|NY|NZ) normal vector to file
#
# ================================================================================
if __name__ == "__main__":
    args = sys.argv[:]
    if len(args) != 2:
        print("No file/ folder to convert was given!")
        exit(ERR_NO_PARAM)

    # Check if path is file/ folder that exist!
    path = args[1]
    files = []

    if os.path.isdir(path):
        for file in os.listdir(path):
            if checkXYZFile(file):
                files.append(os.path.join(path, file))
    elif os.path.isfile(path):
        if checkXYZFile(path):
            files.append(path)
    else:
        print("Given parameter is neather a existing file nor folder!")
        exit(ERR_PARAM_WRONG)

    # Min. one correct file given
    if len(files) == 0:
        print("Wrong file given! Only files ending with .xyz allowed!")
        exit(ERR_NO_XYZ_FILES)

    # Converts file
    for file in files:
        print("Input path:\t" + file)

        # Read input
        contents = []
        with open(file, "rt") as input:
            for line in input:
                arr = line.split(" ")
                if len(arr) < 6:
                    print("The lines are not formatted correctly! There should be 'X Y Z [...] NX NY NZ' given!")
                    exit(ERR_LINES_WRONG)
                
                # First 3 elements should equal position vector
                xyz = []
                for c in arr[:3]:
                    try:
                        xyz.append(str(
                            float(c)        # try/except aims at this conversion!
                        ))
                    except ValueError:
                        print("Position vector must contain floats! They have to look like [-]1[.23456789]...")
                        exit(ERR_POS_NO_FLOATS)
                

                # Last 3 elements should equal normal vector
                nxnynz = []
                for nc in arr[-3:]:
                    try:
                        nxnynz.append(str(
                            float(nc)       # try/except aims at this conversion!
                        ))
                    except ValueError:
                        print("Normal vector must contain floats! They have to look like [-]1[.23456789]...")
                        exit(ERR_NORM_NO_FLOATS)
                

                contents.append(" ".join(xyz) + " " + " ".join(nxnynz) + "\n")
        
        # Write output
        path = file + ".out"
        print("Output path:\t" + path + "\n")

        with open(path, "w") as output:
            for elem in contents:
                output.write(elem)
    
    print("Conversion done!")