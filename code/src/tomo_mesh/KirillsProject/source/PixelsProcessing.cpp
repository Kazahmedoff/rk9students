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

	// �������� ������� ���������
int main(int argc, char *argv[]) 
	{ 

		//������ � ������� �����������
	Uint16 rows;
	Uint16 columns;

	//ofstream DataPixels("DataPixels.txt");

		//���������� �������
	int image_num = 0; 

		// ������� ����� ����������� true, ��������� - false
	bool first_file = true; 
	bool second_file = false;

		// ��������� ������ �� ���� ������ �����������
	for (fs::recursive_directory_iterator it(argv[1]), end; it != end; it++) { 
				// ���� ������ ����, ��
		DcmFileFormat fileformat;
		string name_of_file;

			//� ���� ��������� name_of_file ������ ���� � �����
		name_of_file = it->path().string(); 

			// ���������, ���������� �� ����
		OFCondition status = fileformat.loadFile(name_of_file.c_str());

		if (first_file) {
			first_file = false;
			second_file = true;

			if (status.good()) {

					//�������� ����� ����� � �������� �����������
				if (fileformat.getDataset()->findAndGetUint16(DCM_Rows, rows).good()) {
					cout << "Rows: " << rows << endl;
				}
				if (fileformat.getDataset()->findAndGetUint16(DCM_Columns, columns).good()) {
					cout << "Columns: " << columns << endl;
				}
			}
		}

			//������ ������ ��� �������� ���������� �������� � ����� ������������
		CreateFilters *filter = new CreateFilters(rows, columns);

			// ���������� pixel data, ������ �� ������:
			// http://forum.dcmtk.org/viewtopic.php?f=1&t=4001

				// ���������� ���
			unsigned long numByte = 0;

			 // ������ ������ DicomImage � ����������� ��� � ������ �����
			DicomImage *img = new DicomImage(name_of_file.c_str());

				// ���������, ��������� ��
			if (img->getStatus() == EIS_Normal) {
					// ��������� ���������� ������
				const DiPixel *inter = img->getInterData();

				if (inter != NULL) {
						// ������� ���������� ���
					numByte = inter->getCount();

					short *raw_pixel_data = (short *)inter->getData();
																						//Uint8 *raw_pixel_data = (Uint8 *)(img->getOutputData(8 /* bits */));
						// ���� ��������� ������ - ������
					if (raw_pixel_data == nullptr) { 
						cout << "Couldn't acces pixel data!\n";
						return (1);
					}

						// ������� ������ pixel data
						// ������ �������� � pixel data,
						// ��� ������ �������� � �������
						// - ��������� �� ������ �������
						// �������, � ������ �������� -
						// �� ��������� ������� �������
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

					//������� � ������� ���������� ������
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