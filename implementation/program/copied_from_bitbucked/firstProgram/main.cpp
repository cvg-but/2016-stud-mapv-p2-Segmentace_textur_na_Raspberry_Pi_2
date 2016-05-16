#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <iomanip>

#include "GaborTextureSegmenter.h"
using namespace cv;
using namespace std;

#define kNormalizationGaborKernelConst 100


int main(int argc, char** argv)
{
	
	Mat image;
	
	


	//image = imread("./databaze/text13.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	//image = imread("./dog.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	//image = imread("./texture3.png", CV_LOAD_IMAGE_COLOR);   // Read the file

	for (int i = 3; i < 14; i++) {

		std::ostringstream stringStream;
		stringStream << "./databaze/text" << setfill('0') << setw(2) << i << ".png";
		std::string imagePath = stringStream.str();

		image = imread(imagePath, CV_LOAD_IMAGE_COLOR);   // Read the file

		if (!image.data)                              // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			break;
		}

		Mat clustered = GaborTextureSegmenter().segmentImageByNumberOfTextures(image, 4);
		clustered.convertTo(clustered, CV_8U);

		imshow("asdf", clustered); waitKey();
		
		std::ostringstream stringStream2;
		stringStream2 << "./databaze/text" << setfill('0') << setw(2) << i << "_clustered.png";
		std::string savePath = stringStream2.str();
		imwrite("./databaze/1234132.png", image);
	}

	return 0;
}


