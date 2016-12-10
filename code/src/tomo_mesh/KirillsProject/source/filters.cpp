#include "filters.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

	
	//Конструктор класса Filters
CreateFilters::CreateFilters(Uint16 rows, Uint16 columns)
{
		this->rows_image = rows;
		this->columns_image = columns;
}

	//Функция, применяющая фильтр Гаусса к изображению
void CreateFilters::GetGaussianFinishedPixels(vector<vector<short>> PixelsData)
{
		//Тут будем хранить матрицу свёртки
	static vector<vector<double>>GaussKernel = GetGaussianKernel();

	//Инициализируем новые значения столбцов и строк
	Uint16 NewColumns = this->columns_image + SizeOfKernel / 2;
	Uint16 NewRows = this->rows_image + SizeOfKernel / 2;

	//Создаём новый массив, к которому применим фильтр Гаусса
	vector<vector<short>> NewPixelsData(NewColumns);
	
	//Формируем окно
	for (size_t i = 0; i < NewColumns; ++i)
	{
		NewPixelsData[i].resize(NewColumns);
	}

	//Заполняем левый верхний квадрат
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = 0; j < SizeOfKernel / 2; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j];
		}
	}

	//Заполняем правый верхний квадрат
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = (NewColumns - SizeOfKernel / 2); j < NewColumns; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j - SizeOfKernel / 2];
		}
	}
	
	//Заполняем левый нижний квадрат
	for (size_t i = (NewRows - SizeOfKernel / 2); i < NewRows; ++i)
	{
		for (size_t j = 0; j < SizeOfKernel / 2; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i - SizeOfKernel / 2][j];
		}
	}

	//Заполняем левый нижний квадрат
	for (size_t i = (NewRows - SizeOfKernel / 2); i < NewRows; ++i)
	{
		for (size_t j = (NewColumns - SizeOfKernel / 2); j < NewColumns; ++j)
		{
			NewPixelsData[i][j] = PixelsData[i - SizeOfKernel / 2][j - SizeOfKernel / 2];
		}
	}

	//Заполняем границы от левого до правого верхних квадратов
	for (size_t i = 0; i < SizeOfKernel / 2; ++i)
	{
		for (size_t j = SizeOfKernel / 2; j < (NewColumns - SizeOfKernel / 2); ++j)
		{
			NewPixelsData[i][j] = PixelsData[i][j - SizeOfKernel / 2];
		}
	}
}

	//Функция, применяющая медианный фильтр к изображению
void CreateFilters::GetMedianFinishedPixels()
{
}

	//Функция, возвращающая квадрат среднеквадичного отклонения
double CreateFilters::GetValueSigmaSquare()
{
	double sigma;
	cout << "Enter the sigma value: ";
	cin >> sigma;
	double sigma_square = sigma*sigma;
	return sigma_square;
}

	//Функция, формирующая матрицу свёртки
vector<vector<double>> CreateFilters::GetGaussianKernel()
{
	//Вводим размерность матрицы свёртки
	cout << "Enter the size of kernel: ";
	cin >> this->SizeOfKernel;

	//Проверка на нечетность и неотрицательность
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

	//Создание двумерного динамического массива для хранения матрицы свёртки
	vector<vector<double>>kernel(SizeOfKernel);

	//Инициализация координат элементов матрицы свёртки и коэффициента нормирования
	double x = 0, y = 0, sum = 0;

	//Инициализируем дисперсию
	double SigmaSquare = GetValueSigmaSquare();

	//Заполнение значений координат и получение значений элементов матрицы
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
	//Нормируем матрицу свёртки
	for (int i = 0; i < SizeOfKernel; ++i)
	{
		for (int j = 0; j < SizeOfKernel; ++j)
		{
			kernel[i][j] /= sum;
		}
	}
	return kernel;
}
