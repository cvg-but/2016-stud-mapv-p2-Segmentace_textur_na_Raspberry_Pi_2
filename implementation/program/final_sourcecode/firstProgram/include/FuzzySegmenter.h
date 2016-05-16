#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>

using namespace cv;

class FuzzySegmenter
{
public:
	Mat SegmentImage(Mat I, int clusterCount);
private:
	float fRand(float fMin, float fMax);
};
