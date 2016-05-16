#include "ImageNormalizer.h"


#define kMaxImageValue 255

ImageNormalizer::ImageNormalizer()
{
}


ImageNormalizer::~ImageNormalizer()
{
}

Mat ImageNormalizer::normalizeImage(Mat inputImage)
{
	Mat normalizedImage;
	cv::Point min_loc, max_loc;
	double min, max;
	cv::minMaxLoc(inputImage, &min, &max, &min_loc, &max_loc);

	double filteredImageRangeUsage = max / kMaxImageValue;
	normalizedImage = inputImage * (1 / filteredImageRangeUsage);

	return normalizedImage;
}

Mat ImageNormalizer::equalizeImageHistogram(Mat inputImage)
{
	Mat equalizedImage;
	Mat inputImageCopy;
	inputImage.copyTo(inputImageCopy);

	cv::Point min_loc, max_loc;
	double min, max;
	cv::minMaxLoc(inputImage, &min, &max, &min_loc, &max_loc);

	equalizedImage = normalizeImage(inputImageCopy - min);

	return equalizedImage;
}


void ImageNormalizer::getMaxMix(double *max, double *min) {

}