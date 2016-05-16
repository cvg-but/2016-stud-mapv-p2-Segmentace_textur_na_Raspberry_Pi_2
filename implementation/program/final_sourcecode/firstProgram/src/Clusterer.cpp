#include "Clusterer.h"
#include <iostream>
#include "ImageNormalizer.h"

using namespace std;

Clusterer::Clusterer()
{
}


Clusterer::~Clusterer()
{
}



Mat Clusterer::clusterArrayOfImages(vector<Mat> imageArray, int numberOfClusters, double* squareErr) {
	
	Mat src;
	src = imageArray[0];
	ImageNormalizer normalizator = ImageNormalizer();

	cout << "Clustering into " << numberOfClusters << " clusters by K-mean" << endl;

	Mat samples(src.rows * src.cols, imageArray.size(), CV_32F);
	for (int z = 0; z < imageArray.size(); z++) {
		imageArray[z].convertTo(src, CV_32F);
		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				samples.at<float>(y + x*src.rows, z) = (float)src.at<float>(y, x);
			}
		}
	}


	int attempts = 1;
	
	Mat centers, labels;
	*squareErr = kmeans(samples, numberOfClusters, labels, TermCriteria(CV_TERMCRIT_ITER, 10, 0.0), attempts, KMEANS_RANDOM_CENTERS, centers);
	
	cout << "Compactness: " << *squareErr << " for " << numberOfClusters << " clusters" << endl;

	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x*src.rows, 0);
			new_image.at<float>(y, x) = (int)cluster_idx;
		};

	return mat2gray(new_image);
}



Mat Clusterer::mat2gray(const Mat& src)
{
	Mat dst;
	normalize(src, dst, 0.0, 255.0, NORM_MINMAX);
	return dst;
}
