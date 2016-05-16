#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>

#include "GaborFilterCell.h"

using namespace cv;
using namespace std;


class GaborBankGenerator
{
public:
	GaborBankGenerator();
	~GaborBankGenerator();

	GaborFilterCell* getGaborBank(Size imageSize, int oreintationStep);

	int getNumberOfGaborFiltersInBank(GaborFilterCell* gaborBank);

private:

	float hypot(float a, float b);

	GaborFilterCell *genGaborFilter(float wavelength, float orientation);
};

