#include "filters.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

	
	//����������� ������ Filters
CreateFilters::CreateFilters(Uint16 rows, Uint16 columns)
{
		this->rows_image = rows;
		this->columns_image = columns;
}

	//�������, ����������� ������ ������ � �����������
void CreateFilters::GetGaussianFinishedPixels(vector<vector<short>> PixelsData)
{
		//��� ����� ������� ������� ������
	static vector<vector<double>>GaussKernel = GetGaussianKernel();

	//�������������� ����� �������� �������� � �����
	Uint16 NewColumns = this->columns_image + SizeOfKernel / 2;
	Uint16 NewRows = this->rows_image + SizeOfKernel / 2;

	//������ ����� ������, � �������� �������� ������ ������
	vector<vector<short>> NewPixelsData(NewColumns);
	
	//��������� ����
	for (size_t i = 0; i < NewColumns; ++i)
	{
		NewPixelsData[i].resize(NewColumns);
	}

	//��������� ����� ������� �������
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = 0; j < SizeOfKernel / 2; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j];
		}
	}

	//��������� ������ ������� �������
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = (NewColumns - SizeOfKernel / 2); j < NewColumns; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j - SizeOfKernel / 2];
		}
	}
	
	//��������� ����� ������ �������
	for (size_t i = (NewRows - SizeOfKernel / 2); i < NewRows; ++i)
	{
		for (size_t j = 0; j < SizeOfKernel / 2; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i - SizeOfKernel / 2][j];
		}
	}

	//��������� ����� ������ �������
	for (size_t i = (NewRows - SizeOfKernel / 2); i < NewRows; ++i)
	{
		for (size_t j = (NewColumns - SizeOfKernel / 2); j < NewColumns; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i - SizeOfKernel / 2][j - SizeOfKernel / 2];
		}
	}

	//��������� ������� �� ������ �� ������� ������� ���������
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = SizeOfKernel / 2; j < (NewColumns - SizeOfKernel / 2); ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j - SizeOfKernel / 2];
		}
	}
}

	//�������, ����������� ��������� ������ � �����������
void CreateFilters::GetMedianFinishedPixels()
{
}

	//�������, ������������ ������� ���������������� ����������
double CreateFilters::GetValueSigmaSquare()
{
	double sigma;
	cout << "Enter the sigma value: ";
	cin >> sigma;
	double sigma_square = sigma*sigma;
	return sigma_square;
}

	//�������, ����������� ������� ������
vector<vector<double>> CreateFilters::GetGaussianKernel()
{
	//������ ����������� ������� ������
	cout << "Enter the size of kernel: ";
	cin >> this->SizeOfKernel;

	//�������� �� ���������� � �����������������
	try
	{
		if (SizeOfKernel % 2 == 0 || SizeOfKernel < 0)
			throw false;
	}
	catch (bool)
	{
		cout << "Size is wrong!";
		abort();
	}

	//�������� ���������� ������������� ������� ��� �������� ������� ������
	vector<vector<double>>kernel(SizeOfKernel);

	//������������� ��������� ��������� ������� ������ � ������������ ������������
	double x = 0, y = 0, sum = 0;

	//�������������� ���������
	double SigmaSquare = GetValueSigmaSquare();

	//���������� �������� ��������� � ��������� �������� ��������� �������
	for (int i = 0; i < SizeOfKernel; ++i)
	{
		for (int j = 0; j < SizeOfKernel; ++j)
		{
			x = -SizeOfKernel / 2 + i;
			y = -SizeOfKernel / 2 + j;
			kernel[i].resize(SizeOfKernel);
			kernel[i].push_back((1.0 / 2.0 * M_PI*SigmaSquare)*exp(-(x*x + y*y) / 2.0 * SigmaSquare));
			sum += kernel[i][j];
		}
	}
	//��������� ������� ������
	for (int i = 0; i < SizeOfKernel; ++i)
	{
		for (int j = 0; j < SizeOfKernel; ++j)
		{
			kernel[i][j] /= sum;
		}
	}
	return kernel;
}
