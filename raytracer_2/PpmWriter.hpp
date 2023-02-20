//
//  PpmWriter.hpp
//  raytracer_2
//
//  Created by Sofia Iannicelli on 2/15/23.
//

#ifndef PpmWriter_hpp
#define PpmWriter_hpp

#include <stdio.h>
#include <fstream>

class PpmWriter {
private:
    static std::ofstream setUpFile(std::string outputFilePath, std::string magicNumber, int numCols, int numRows, int maxColorVal) {
        // opening file to write output to
        std::ofstream outputFile;
        outputFile.open(outputFilePath);

        // write file headers
        outputFile << magicNumber << std::endl; // magic number
        outputFile << numCols << " " << numRows << std::endl; // set num cols and rows
        outputFile << maxColorVal << std::endl; // maximum color value

        return outputFile;
    }
public:
    static std::ofstream writePpm(std::string outputFilePath, std::string magicNumber, int numCols, int numRows, int maxColorVal, int*** pixelColors) {
        std::ofstream outputFile = setUpFile(outputFilePath, magicNumber, numCols, numRows, maxColorVal);
        
        for(int i = 0; i < numRows; ++i) {
            for(int j = 0; j < numCols; ++j) {
                outputFile << pixelColors[i][j][0] << " ";
                outputFile << pixelColors[i][j][1] << " ";
                outputFile << pixelColors[i][j][2] << " ";
            }
        }
        
        
        return outputFile;
    }
};

#endif /* PpmWriter_hpp */
