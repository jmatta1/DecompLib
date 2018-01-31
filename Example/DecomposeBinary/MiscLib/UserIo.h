#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H

#include<tuple>
#include<string>
#include"SimpleStructs.h"

//get the type of initial spectrum to feed the decomposition
int getStartingValType();
//get the scaling factor for the spectrum if that was chose
double getScalingFactor();
//get the constant value if starting with a constant
double getStartingConstant();
//get the minimum value below which a value will not be tested for convergence
double getMinThreshold();
//that the maximum fractional change that a value can have between iterations before a check is declared unconverged
double getConvergence();
//used to get the bin boundaries of axes of a response matrix from the user
void get2dAxisBounds(TwoDMetaData& meta);
//used to get the bin boundaries of the axis of a spectrum
void get1dAxisBounds(OneDMetaData& meta);

namespace Detail
{

int getAxisLoBound(const std::string& axis, int numBins, double loEdge, double perBin);
int getAxisHiBound(const std::string& axis, int numBins, double hiEdge, double perBin);

}
#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H
