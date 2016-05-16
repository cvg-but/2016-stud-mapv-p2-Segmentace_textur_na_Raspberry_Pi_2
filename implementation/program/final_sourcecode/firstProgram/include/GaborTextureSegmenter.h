#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "GaborBankGenerator.h"
#include "ImageNormalizer.h"
#include "Clusterer.h"

using namespace std;
using namespace cv;

class GaborTextureSegmenter
{
public:
	GaborTextureSegmenter();
	~GaborTextureSegmenter();

	Mat segmentImageByNumberOfTextures(Mat image, int numberOfTextures);

	Mat segmentImageByUnknownNumberOfTextures(Mat image);


private:

	vector<Mat> prepareFeatureArray(Mat image);

	Mat postprocessClusteredImage(Mat image, Size originalSize);

	int selectClusterNumberByElbowMethod(double *stdErr, Size clusteredImageSize);
};