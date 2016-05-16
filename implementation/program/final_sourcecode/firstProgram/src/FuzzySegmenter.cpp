#include "FuzzySegmenter.h"

#include <iostream>

#define winSize 11
#define fuzziness 2
#define epsilon 5
#define imgSize 128

using std::cout;
using std::endl;



Mat FuzzySegmenter::SegmentImage(Mat I, int clusterCount)
{
	double scale;
	if (I.cols > I.rows)
		scale = (double)imgSize / I.rows;
	else
		scale = (double)imgSize / I.cols;

	Mat img;
	resize(I, img, Size(), scale, scale, CV_INTER_LINEAR);

	/*Mat I_small;
	resize(I, I_small, Size(), scale, scale, CV_INTER_LINEAR);

	Mat img;
	img.create(I_small.size(), CV_8UC3);
	cvtColor(I_small, img, CV_RGB2Luv);*/

	int x, y, n, k, i, j;
	int fromX, widX, fromY, widY;

	int X = img.cols;
	int Y = img.rows;
	int XY = X*Y;
	int winHalf = winSize / 2;

	Mat result;

	Mat F;
	F.create(Y, X, CV_32FC(6));
	Scalar mea, dev;

	float sum, sumDenominator, uPowered, u, uMax;
	Vec<float, 6> sumNumerator;

	Vec<float, 6> *ptF, *ptV;
	float *ptU;
	uchar *ptR;

	double m = 2.0 / (fuzziness - 1);

	Mat window;
	window.create(winSize, winSize, CV_8UC3);

	double minMe[3] = { 255 };
	double maxMe[3] = { 0 };
	double minDe[3] = { 255 };
	double maxDe[3] = { 0 };

	for (y = 0; y < Y; y++)
	{
		ptF = F.ptr<Vec<float, 6>>(y);

		for (x = 0; x < X; x++)
		{
			fromY = (y >= winHalf) ? (y - winHalf) : 0;
			widY = (y < (Y - winHalf - 1)) ? winSize : (Y - y);
			fromX = (x >= winHalf) ? (x - winHalf) : 0;
			widX = (x < (X - winHalf - 1)) ? winSize : (X - x);

			img(Rect(fromX, fromY, widX, widY)).copyTo(window);
			meanStdDev(window, mea, dev);

			mea[1] = mea[1] * 2;
			mea[2] = mea[2] * 2;
			dev[0] = dev[0] * 2;

			for (n = 0; n < 3; n++)
			{
				ptF[x][n * 2] = mea[n];
				ptF[x][n * 2 + 1] = dev[n];

				if (minMe[n] > mea[n])
					minMe[n] = mea[n];
				if (maxMe[n] < mea[n])
					maxMe[n] = mea[n];

				if (minDe[n] > dev[n])
					minDe[n] = dev[n];
				if (maxDe[n] < dev[n])
					maxDe[n] = dev[n];
			}
		}
	}

	srand(time(NULL));

	Mat V;
	V.create(1, clusterCount, CV_32FC(6));
	Mat Vprev;
	Vprev.create(1, clusterCount, CV_32FC(6));

	Mat U;
	U.create(clusterCount, XY, CV_32F);

	Mat res;
	res.create(Y, X, CV_8U);

	Mat colors;
	colors.create(1, clusterCount, CV_8U);

	ptV = V.ptr<Vec<float, 6>>(0);
	for (n = 0; n < 3; n++)
	{
		for (k = 0; k < clusterCount; k++)
		{
			ptV[k][n * 2] = fRand(minMe[n], maxMe[n]);
			ptV[k][n * 2 + 1] = fRand(minDe[n], maxDe[n]);
		}
	}

	while (norm(V, Vprev, NORM_L2) > epsilon)
	{
		V.copyTo(Vprev);

		for (k = 0; k < clusterCount; k++)
		{
			ptU = U.ptr<float>(k);

			sumNumerator = { 0, 0 };
			sumDenominator = 0;

			for (y = 0; y < Y; y++)
			{
				ptF = F.ptr<Vec<float, 6>>(y);
				i = y * X;

				for (x = 0; x < X; x++)
				{
					sum = 0;

					for (j = 0; j < clusterCount; j++)
					{
						sum += pow(norm(ptF[x], ptV[k], NORM_L2) / norm(ptF[x], ptV[j]), m);
					}

					ptU[i + x] = 1 / sum;
				}
			}

			for (y = 0; y < Y; y++)
			{
				ptF = F.ptr<Vec<float, 6>>(y);
				i = y * X;

				for (x = 0; x < X; x++)
				{
					uPowered = pow(ptU[i + x], m);
					sumNumerator += ptF[x] * uPowered;
					sumDenominator += uPowered;
				}
			}

			ptV[k] = sumNumerator / sumDenominator;
		}
	}
	
	uchar *ptC = colors.ptr<uchar>(0);
	uchar dc = 255 / (clusterCount - 1);
	for (k = 0; k < clusterCount; k++)
		ptC[k] = k*dc;

	for (y = 0; y < Y; y++)
	{
		i = y * X;
		ptR = res.ptr<uchar>(y);

		for (x = 0; x < X; x++)
		{
			uMax = 0;
			for (k = 0; k < clusterCount; k++)
			{
				u = U.ptr<float>(k)[i + x];
				if (uMax < u)
				{
					j = k;
					uMax = u;
				}
			}
			ptR[x] = ptC[j];
		}
	}

	
	resize(res, result, Size(), 1 / scale, 1 / scale, CV_INTER_NN);

	return result;
}

float FuzzySegmenter::fRand(float fMin, float fMax)
{
	float f = (float)rand() / RAND_MAX;
	return (fMin + f * (fMax - fMin));
}
