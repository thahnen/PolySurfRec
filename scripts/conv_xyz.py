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


import os
import sys


if __name__ == "__main__":
    args = sys.argv[:]
    if len(args) != 2:
        print("No file/ folder to convert was given!")
        exit(1)

    # Check if path is file/ folder that exist!
    path = args[1]
    files = []

    if os.path.isdir(path):
        for file in os.listdir(path):
            if file.split(".")[-1] == "xyz":
                files.append(os.path.join(path, file))
    elif os.path.isfile(path):
        if path.split(".")[-1] != "xyz":
            print("Wrong file given! Only files ending with .xyz allowed!")
            exit(2)

        files.append(path)
    else:
        print("Given parameter is neather a existing file nor folder!")
        exit(3)

    # Converts file
    for file in files:
        contents = []

        # Read input
        with open(file, "rt") as input:
            for line in input:
                arr = line.split(" ")
                if len(arr) < 6:
                    print("The lines are not formatted correctly! There should be 'X Y Z [...] NX NY NZ' given!")
                    exit(4)
                
                # First 3 elements equals position vector
                xyz = arr[:3]
                for coord in xyz:
                    if not "." in coord:
                        print("Position vector must contain floats! They have to look like 1.23456789...")
                        exit(5)
                
                # Last 3 elements equals normal vector
                nxnynz = arr[-3:]
                for norm in nxnynz:
                    if not "." in norm:
                        print("Normal vector must contain floats! They have to look like 1.23456789...")
                        exit(6)
                
                contents.append(" ".join(xyz + nxnynz))
        
        # Write output
        path = file + ".out"
        print("Pfad: " + path)

        with open(path, "w") as output:
            for elem in contents:
                output.write(elem)
    
    print("Everything done!")