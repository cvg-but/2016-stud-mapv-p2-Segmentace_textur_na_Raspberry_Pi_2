#include "GaborBankGenerator.h"
#define kMaxNumberOfWavelength 99
#define kOptimalizeNumberOfFiltersForRaspberryPi true

GaborBankGenerator::GaborBankGenerator()
{
}


GaborBankGenerator::~GaborBankGenerator()
{
}

GaborFilterCell * GaborBankGenerator::getGaborBank(Size imageSize, int oreintationStep)
{
	GaborFilterCell* gaborBank;
	float wavelengthMin = 4.0f / (float)sqrt(2);
	float wavelengthMax = hypot((float)imageSize.height, (float)imageSize.width);
	int n = (int)floor(log2(wavelengthMax / wavelengthMin));
	float *wavelength = (float*)malloc((n - 1)*sizeof(float));
	int numberOfOrientations = 180 / oreintationStep;

	cout << "Start Generating Gabor Filters";

	if (numberOfOrientations < 1){
		cout << "Error when generating gabor bank: bad orientation step" << endl;
		return nullptr;
	}
	
	float *orientation = (float*)malloc(numberOfOrientations*sizeof(float));


	if (n > kMaxNumberOfWavelength) //done to limit number of wawelength
		n = kMaxNumberOfWavelength;
	if(kOptimalizeNumberOfFiltersForRaspberryPi){
		n--;
	}
	

	for (int i = 0; i < n - 1; i++) {
		wavelength[i] = (float)pow(2, i) * wavelengthMin;
	}

	for (int i = 0; i < numberOfOrientations; i++) {
		orientation[i] = (float)(oreintationStep * i);
	}


	int totalNumberOfGaborFilters = (n - 1) * numberOfOrientations;
	GaborFilterCell *lastGaborFilterCell;

	cout << "Generating " << totalNumberOfGaborFilters << " Filters";

	for (int j = 0; j < (n-1); j++)
	{
		for (int i = 0 ; i < numberOfOrientations ; i++)
		{
			GaborFilterCell *newGaborFilterCell = genGaborFilter(wavelength[j], orientation[i] / 180 * CV_PI);
			
			if (i != 0 || j != 0) // not first gabor filter cell
			{
				lastGaborFilterCell->nextFilterCell = newGaborFilterCell;
			}
			else {
				gaborBank = newGaborFilterCell;
				
			}
			lastGaborFilterCell = newGaborFilterCell;
		}
	}

	return gaborBank;
}

int GaborBankGenerator::getNumberOfGaborFiltersInBank(GaborFilterCell * gaborBank)
{
	int numberOfGaborFilters = 0;
	GaborFilterCell *lastGaborCell = gaborBank;

	for (numberOfGaborFilters = 0; lastGaborCell->nextFilterCell != nullptr; numberOfGaborFilters++) {
		lastGaborCell = lastGaborCell->nextFilterCell;
	}

	return numberOfGaborFilters+1;
}


float GaborBankGenerator::hypot(float a, float b) {
	return sqrt( pow(a,2) + pow(b,2) );
}



GaborFilterCell *GaborBankGenerator::genGaborFilter(float wavelength, float orientation) {

	GaborFilterCell *newGaborCell = new GaborFilterCell;
	newGaborCell->nextFilterCell = nullptr;

	int kernelWidth = 5 * wavelength;
	Size size = Size(kernelWidth, kernelWidth);
	double aspectRatio = CV_PI * 0.5;
	double phaseOffset = 0;
	double gaussianStdDev = wavelength;

	newGaborCell->gaborKernel = getGaborKernel(size, gaussianStdDev, orientation, wavelength, aspectRatio, phaseOffset, CV_64F);
	newGaborCell->wavelength = wavelength;
	newGaborCell->gaborKernel = newGaborCell->gaborKernel;// / (size.width * size.height);

	return newGaborCell;
}
