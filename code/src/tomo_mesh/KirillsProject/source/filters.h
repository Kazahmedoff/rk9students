#pragma once

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <vector>

class CreateFilters 
{
	private:
		Uint16 rows_image;
		Uint16 columns_image;
		short SizeOfKernel;
	public:
		CreateFilters(Uint16, Uint16);
		void GetGaussianFinishedPixels(std::vector<std::vector<short>>);
		void GetMedianFinishedPixels();
		double GetValueSigmaSquare();
		std::vector<std::vector<double>> GetGaussianKernel();
};
