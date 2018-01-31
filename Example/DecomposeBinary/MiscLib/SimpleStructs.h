/*!*****************************************************************************
********************************************************************************
**
** @copyright Copyright (C) 2018 James Till Matta
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** 
********************************************************************************
*******************************************************************************/
#ifndef DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SIMPLESTRUCTS_H
#define DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SIMPLESTRUCTS_H

struct OneDMetaData
{
public:
    int numBins = 0;
    int startBin = 0;
    int stopBin = 0;
    double firstBinLoEdge = 0.0;
    double finalBinHiEdge = 0.0;
};

struct TwoDMetaData
{
public:
    int numXbins = 0;
    int startXbin = 0;
    int stopXbin = 0;
    double firstXbinLoEdge = 0.0;
    double finalXbinHiEdge = 0.0;
    int numYbins = 0;
    int startYbin = 0;
    int stopYbin = 0;
    double firstYbinLoEdge = 0.0;
    double finalYbinHiEdge = 0.0;
};

#endif //DECOMPLIB_EXAMPLE_DECOMPOSEBINARY_SIMPLESTRUCTS_H
