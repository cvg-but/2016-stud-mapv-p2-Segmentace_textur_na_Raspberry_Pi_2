#include "Estimator.h"

#define peakDistance 15
#define blurSize 7

using namespace std;

int TextuxeCountEstimator::EstimateCount(Mat imageLuv)
{
	int n = 0;

	vector<Mat> luv_planes;
	split(imageLuv, luv_planes);

	int histSize = 256;
	float range[] = { 0, 255 };
	const float* histRange = { range };
	Mat u_hist, v_hist;

	calcHist(&luv_planes[1], 1, 0, Mat(), u_hist, 1, &histSize, &histRange);
	calcHist(&luv_planes[2], 1, 0, Mat(), v_hist, 1, &histSize, &histRange);

	n += FindPeaks(u_hist);
	n += FindPeaks(v_hist);

	if (n >= 2)
		return n;
	else
		return 2;
}

int TextuxeCountEstimator::FindPeaks(Mat hist)
{
	blur(hist, hist, Size(1, blurSize));

	double min, max;
	minMaxIdx(hist, &min, &max);

	int i, prev_peak = 0;
	float *pt = hist.ptr<float>(0);
	float tresh = (float)max / 3;
	int peaks = 0;

	for (i = 1; i < (hist.rows - 1); i++)
	{
		if (pt[i] > pt[i - 1] && pt[i] >= pt[i + 1])
		{
			if ((i - prev_peak) > peakDistance && pt[i] > tresh)
			{
				prev_peak = i;
				peaks++;
			}
		}
	}

	return peaks;
}
