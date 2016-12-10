// STL including
#include <iostream>
#include <string>

#include <conio.h>

//filters.h including
#include "filters.h"

// DCMTK including
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diregist.h"
#include "dcmtk/dcmimgle/dcmimage.h"

// Boost including
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

	// передаем функции аргументы
int main(int argc, char *argv[]) 
	{ 

		//строки и столбцы изображения
	Uint16 rows;
	Uint16 columns;

	//ofstream DataPixels("DataPixels.txt");

		//количество снимков
	int image_num = 0; 

		// первому файлу присваиваем true, остальным - false
	bool first_file = true; 
	bool second_file = false;

		// пробегаем циклом по всем файлам дирректории
	for (fs::recursive_directory_iterator it(argv[1]), end; it != end; it++) { 
				// если первый файл, то
		DcmFileFormat fileformat;
		string name_of_file;

			//я хочу присвоить name_of_file полный путь к файлу
		name_of_file = it->path().string(); 

			// проверяем, загрузился ли файл
		OFCondition status = fileformat.loadFile(name_of_file.c_str());

		if (first_file) {
			first_file = false;
			second_file = true;

			if (status.good()) {

					//получаем число строк и столбцов изображения
				if (fileformat.getDataset()->findAndGetUint16(DCM_Rows, rows).good()) {
					cout << "Rows: " << rows << endl;
				}
				if (fileformat.getDataset()->findAndGetUint16(DCM_Columns, columns).good()) {
					cout << "Columns: " << columns << endl;
				}
			}
		}

			//Создаём объект для передачи количества столбцов и строк конструктору
		CreateFilters *filter = new CreateFilters(rows, columns);

			// считывание pixel data, ссылка на пример:
			// http://forum.dcmtk.org/viewtopic.php?f=1&t=4001

				// количество бит
			unsigned long numByte = 0;

			 // создаём объект DicomImage и ассоциируем его с именем файла
			DicomImage *img = new DicomImage(name_of_file.c_str());

				// проверяем, открылось ли
			if (img->getStatus() == EIS_Normal) {
					// считываем пиксельные данные
				const DiPixel *inter = img->getInterData();

				if (inter != NULL) {
						// считаем количество бит
					numByte = inter->getCount();

					short *raw_pixel_data = (short *)inter->getData();
																						//Uint8 *raw_pixel_data = (Uint8 *)(img->getOutputData(8 /* bits */));
						// если указатель пустой - ошибка
					if (raw_pixel_data == nullptr) { 
						cout << "Couldn't acces pixel data!\n";
						return (1);
					}

						// создаем массив pixel data
						// читаем значения в pixel data,
						// где первый аргумент в скобках
						// - указатель на первый элемент
						// массива, а второй аргумент -
						// на последний элемент массива
					vector<short> pixel_data(
											 raw_pixel_data,
											 raw_pixel_data + rows * columns); 

				vector<vector<short>> slice;

					for (int i = 0; i < rows; ++i) {
						slice.resize(slice.size() + 1);
						slice[i].resize(columns);

						for (int j = 0; j < columns; ++j) {
							slice[i][j] = pixel_data[i*columns + j];
							//DataPixels << slice[i][j];
						}
					} 

					//Перадаём в функцию пиксельные данные
					filter->GetGaussianFinishedPixels(slice);

					image_num++;
				}
			}
			delete img;
			delete filter;
		}
	//DataPixels.close();
	std::cout << "Images was processed: " << image_num <<"\n";
	getch();
	return 0;
	}