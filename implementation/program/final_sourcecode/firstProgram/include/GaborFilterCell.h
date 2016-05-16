#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

class GaborFilterCell
{
public:
	GaborFilterCell();
	~GaborFilterCell();

	Mat gaborKernel;
	GaborFilterCell *nextFilterCell;
	float wavelength;
};

