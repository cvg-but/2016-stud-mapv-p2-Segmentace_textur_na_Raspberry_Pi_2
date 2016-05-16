#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <iomanip>

#include "GaborTextureSegmenter.h"
#include "FuzzySegmenter.h"
#include "Estimator.h"

using namespace cv;
using namespace std;

#define kNormalizationGaborKernelConst 100
#define kHelpParamStr "--help"
#define kHelpParamStrShort "-h"
#define kGaborParameter "--gabor"
#define kFuzzyParameter "--fuzzy"
#define kContinousParameter "--continous"
#define kCameraParameter "--camera"
#define kPrevWindow "Preview Window"


int helpParameterHandler(int argc, char **argv);

void printHelp();

Mat cameraParameterHandler(int argc, char **argv);

Mat filtrationParameterHandler(int argc, char **argv, Mat image);

void infiniteCameraLoop(int argc, char **argv);

int segmentationSection(Mat image, int argc, char **argv);




int main(int argc, char** argv)
{
	
	Mat image;
	Size originalImageSize;
	
	if(helpParameterHandler(argc, argv) == 1){
		return 0;
	}
	

	if( argc != 3 && argc != 4 )
    {
		printHelp();
		return -1;
    }
    
	image = cameraParameterHandler(argc, argv);
	int output = segmentationSection(image, argc, argv);
	
	waitKey(0);
	return output;
}



int helpParameterHandler(int argc, char **argv){

	if(argc >= 2){
		if( strcmp(argv[1],kHelpParamStrShort) == 0 || strcmp(argv[1],kHelpParamStr) == 0 ){
			printHelp();
			return 1;
		}
	}
	return 0;
}



void printHelp(){
		
	system("clear");
	printf("Be welcome USER !!\n");
	printf("This is \"Texture Segmentation Software 3000 Delux\" developed at Brno university of technology (BUT) in 2016\n");
	printf("\n");
	printf("Shotr tutorial: \n");
	printf("Software accept 3 parameters:\n");
	printf("1. Input parameter: use \"--camera\" for camera input still image, --continous for endless camera input segmentation or write path to image in file system\n");
	printf("2. Segmentation method: select \"--gabor\" for segmentation in frequency domain of use \"--fuzzy\" to segment in color domain\n");
	printf("3. Number of Clusters: write number of output segmented clusters\n");
	printf("\n");
	printf("Example:\n");
	printf("\t./TextSeg --camera --gabor 4\n");
	printf("\tor\n");
	printf("\t./TextSeg ~/myImages/image.jpg --fuzzy 2\n");
}


Mat cameraParameterHandler(int argc, char **argv){
	
	Mat image;
	if( strcmp(argv[1],kCameraParameter) == 0 ){
		system("raspistill -w 400 -h 300 -t 200 -n -hf -vf -o camera.jpg");
		image = imread("camera.jpg", CV_LOAD_IMAGE_COLOR);
		return image;
	}
	if( strcmp(argv[1],kContinousParameter) == 0 ){
		infiniteCameraLoop(argc, argv);
	}

	// load image from local file
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   
	return image;
}



Mat filtrationParameterHandler(int argc, char **argv, Mat image){
	
	Mat clustered;

	
	Mat imageLuv;
	imageLuv.create(image.size(), CV_8UC3);
	cvtColor(image, imageLuv, CV_RGB2Luv);
	
	int numberOfClusters = 2;
	
	if (argc == 4)
	{
		numberOfClusters = atoi( argv[3] );
	}
	else
	{
		numberOfClusters = TextuxeCountEstimator().EstimateCount(imageLuv);
		cout << "Estimating... " << numberOfClusters << " textures found" << endl;
	}
		
	
	if ( strcmp(argv[2],kGaborParameter) == 0 ) {
		clustered = GaborTextureSegmenter().segmentImageByNumberOfTextures(image, numberOfClusters);
		clustered.convertTo(clustered, CV_8U);
	}
	else if ( strcmp(argv[2],kFuzzyParameter) ==0 ) {
		clustered = FuzzySegmenter().SegmentImage(imageLuv, numberOfClusters);
	}
	
	return clustered;
}



void infiniteCameraLoop(int argc, char **argv){
	
	Mat image;
	while(true){
		system("raspistill -w 400 -h 300 -t 200 -n -hf -vf -o camera.jpg");
		image = imread("camera.jpg", CV_LOAD_IMAGE_COLOR);
		
		segmentationSection(image, argc, argv);
	}
}



int segmentationSection(Mat image, int argc, char **argv){
	
	
	
    if(!image.data ){
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    Size originalImageSize = image.size();
	resize(image,image, Size(400,300));
	namedWindow(kPrevWindow);
	
    cout << "Image loaded" << endl;

	Mat clustered = filtrationParameterHandler(argc, argv,image);


	if (!clustered.empty())
	{
		resize(clustered,clustered, originalImageSize, CV_INTER_NN);
		imwrite("segmentedImage.jpg", clustered);
		
		resize(clustered,clustered, Size(400, 300), CV_INTER_NN);
		imshow(kPrevWindow, clustered); 
		waitKey(30);
	}
	else
	{
		cout << "Could not segment image" << endl;
		return -1;
	}
	return 0;
}
