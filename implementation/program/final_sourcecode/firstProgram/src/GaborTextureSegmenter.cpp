#include "GaborTextureSegmenter.h"

#define kMaxNumberOfClusters 10

GaborTextureSegmenter::GaborTextureSegmenter()
{
}


GaborTextureSegmenter::~GaborTextureSegmenter()
{
}




Mat GaborTextureSegmenter::segmentImageByNumberOfTextures(Mat image, int numberOfTextures) {

	Size originalImageSize = image.size();
	double *err = new double;
	*err = 0;
	
	vector<Mat> arrayOfgaborFilteredImages = prepareFeatureArray(image);
	Mat clustered = Clusterer().clusterArrayOfImages(arrayOfgaborFilteredImages, numberOfTextures, err);

	return postprocessClusteredImage(clustered,originalImageSize);
}



Mat GaborTextureSegmenter::segmentImageByUnknownNumberOfTextures(Mat image) {


	Size originalImageSize = image.size();
	Mat resizedImage;
	resize(image, resizedImage, Size(400, 300));
	vector<Mat> arrayOfgaborFilteredImages = prepareFeatureArray(resizedImage);

	int selectedIndex = 0;

	Mat clustered[kMaxNumberOfClusters];
	double squareErr[kMaxNumberOfClusters];




	for (int i = 1; i <= kMaxNumberOfClusters; i++) {
		clustered[i-1] = Clusterer().clusterArrayOfImages(arrayOfgaborFilteredImages, i, squareErr+i-1);
	}



	return postprocessClusteredImage(clustered[3], originalImageSize);
}



vector<Mat> GaborTextureSegmenter::prepareFeatureArray(Mat image) {

	vector<Mat> arrayOfgaborFilteredImages;
	GaborFilterCell *gaborBank;
	Mat filteredImage;
	Mat filteredGaussianImage;
	Mat grayImage;
	int borderPoliticy = BORDER_REFLECT101;


	cvtColor(image, grayImage, CV_BGR2GRAY, CV_16U);
	gaborBank = GaborBankGenerator().getGaborBank(Size(image.cols, image.rows), 30);
	int numberOfFiltersInGaborBank = GaborBankGenerator().getNumberOfGaborFiltersInBank(gaborBank);
	GaborFilterCell *currentGaborFilterCell = gaborBank;

	int i = 1;

	do
	{
		cout << "Filtering by " << i << ". filter" << endl;

		filter2D(grayImage, filteredImage, -1, currentGaborFilterCell->gaborKernel, Point(-1, -1), 0, borderPoliticy);

		double wavelength = currentGaborFilterCell->wavelength;
		int wavelength3 = (int)(5 * wavelength);
		if (wavelength3 % 2 == 0) {
			wavelength3 += 1;
		}


		GaussianBlur(filteredImage, filteredGaussianImage, Size(wavelength3, wavelength3), 0, 0, borderPoliticy);
		filteredGaussianImage = ImageNormalizer().normalizeImage(filteredGaussianImage);
		arrayOfgaborFilteredImages.push_back(filteredGaussianImage);

		currentGaborFilterCell = currentGaborFilterCell->nextFilterCell;
		i++;
	} while (currentGaborFilterCell != nullptr);

	return arrayOfgaborFilteredImages;
}


Mat GaborTextureSegmenter::postprocessClusteredImage(Mat image, Size originalSize) {

	image.convertTo(image, CV_8U);
	resize(image, image, originalSize);
	image = ImageNormalizer().normalizeImage(image);

	return image;
}

int GaborTextureSegmenter::selectClusterNumberByElbowMethod(double *stdErr, Size clusteredImageSize) {
	
	double normalizedSquareErr[kMaxNumberOfClusters];

	int selectedIndex = 0;
	double totalImageArea = clusteredImageSize.height * clusteredImageSize.width;

	return selectedIndex;
}
