#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H

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

#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_USERIO_H
