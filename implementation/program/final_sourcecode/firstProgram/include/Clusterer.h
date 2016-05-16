#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Clusterer
{
public:
	Clusterer();
	~Clusterer();
	Mat clusterArrayOfImages(vector<Mat> imageArray, int numberOfClusters, double* squareErr);

private:
	Mat mat2gray(const Mat& src);
};

