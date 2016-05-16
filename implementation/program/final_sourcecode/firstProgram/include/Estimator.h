#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class TextuxeCountEstimator
{
public:
	int EstimateCount(Mat imageLuv);
private:
	int FindPeaks(Mat hist);
};
