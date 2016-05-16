#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class ImageNormalizer
{
public:
	ImageNormalizer();
	~ImageNormalizer();

	Mat normalizeImage(Mat inputImage);

	Mat equalizeImageHistogram(Mat inputImage);

private:

	void getMaxMix(double *max, double *min);
};

