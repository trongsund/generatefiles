// ShiftedImages.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

typedef unsigned char BYTE;

typedef struct tagBITMAPFILEHEADER {
    unsigned short int BfType;
    unsigned int BfSize;
    unsigned short int BfReserved1, BfReserved2;
    unsigned int BfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    unsigned int biSize;                 // размер структуры BITMAPFILEHEADER
    int biWidth;                // ширина изображения в пикселах
    int biHeight;               // высота изображения в пикселах
    unsigned short int  biPlanes;               // количество плоскостей
    unsigned short int  biBitCount;             // количество бит на пиксел
    unsigned int biCompression;          // тип сжатия
    unsigned int biSizeImage;            // размер изображения в байтах
    int  biXPelsPerMeter;        // разрешение устройства вывода по X
    int  biYPelsPerMeter;        // разрешение устройства вывода по Y
    unsigned int biClrUsed;              // размер таблицы цветов
    unsigned int biClrImportant;         // необходимое количество цветов
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGBTRIPLE;

struct BMPFile {
    BITMAPFILEHEADER FileHeader;
    BITMAPINFOHEADER InfoHeader;
    std::vector<std::vector<RGBTRIPLE>> Info;
};

BMPFile ReadFile(const std::string& filename) {
    
    std::ifstream f_in(filename, std::ios::binary);
    
    BMPFile result_file;
    
    f_in.read(reinterpret_cast<char*>(&result_file.FileHeader.BfType), 2);
    f_in.read(reinterpret_cast<char*>(&result_file.FileHeader.BfSize), 4);
    f_in.read(reinterpret_cast<char*>(&result_file.FileHeader.BfReserved1), 2);
    f_in.read(reinterpret_cast<char*>(&result_file.FileHeader.BfReserved2), 2);
    f_in.read(reinterpret_cast<char*>(&result_file.FileHeader.BfOffBits), 4);

    f_in.read(reinterpret_cast<char*>(&result_file.InfoHeader), sizeof(BITMAPINFOHEADER));
    
    RGBTRIPLE pixel;

    for (int i = 0; i < result_file.InfoHeader.biHeight; ++i) {
        result_file.Info.push_back({});
        for (int j = 0; j < result_file.InfoHeader.biWidth; ++j) {
            f_in.read(reinterpret_cast<char*>(&pixel), 3);
            result_file.Info[i].push_back(pixel);
        }
    }

    f_in.close();
    return result_file;
}

void WriteFile(BMPFile& file, const std::string& filename) {

    std::ofstream f_out(filename, std::ios::binary);

    f_out.write(reinterpret_cast<char*>(&file.FileHeader.BfType), 2);
    f_out.write(reinterpret_cast<char*>(&file.FileHeader.BfSize), 4);
    f_out.write(reinterpret_cast<char*>(&file.FileHeader.BfReserved1), 2);
    f_out.write(reinterpret_cast<char*>(&file.FileHeader.BfReserved2), 2);
    f_out.write(reinterpret_cast<char*>(&file.FileHeader.BfOffBits), 4);

    f_out.write(reinterpret_cast<char*>(&file.InfoHeader), sizeof(BITMAPINFOHEADER));

    for (int i = 0; i < file.InfoHeader.biHeight; ++i) {
        for (int j = 0; j < file.InfoHeader.biWidth; ++j) {
            f_out.write(reinterpret_cast<char*>(&file.Info[i][j]), 3);
        }
    }

    f_out.close();
}

int main()
{
    for (int ind = 1; ind <= 1; ++ind) {

        BMPFile file = ReadFile("Nadejda.bmp");

        //std::cout << file.InfoHeader.biWidth << std::endl;
        //std::cout << file.InfoHeader.biHeight << std::endl;

        int hshift = 20, vshift = 12;

        BMPFile file1 = file;
        file1.FileHeader.BfSize += hshift * 3 * file1.InfoHeader.biHeight;
        file1.InfoHeader.biWidth += hshift;

        BYTE buf, bufp;

        for (int i = 0; i < file1.InfoHeader.biHeight; ++i) {
            file1.Info[i] = std::vector<RGBTRIPLE>(file1.InfoHeader.biWidth);
        }
        for (int i = 0; i < file1.InfoHeader.biHeight; ++i) {
            for (int j = 0; j < hshift; ++j) {
                buf = (file.Info[i][j].rgbtBlue + file.Info[i][j].rgbtGreen + file.Info[i][j].rgbtRed) / 3;
                file1.Info[i][j].rgbtBlue = buf / 2;
                file1.Info[i][j].rgbtGreen = 64 + 3 * buf / 8;
                file1.Info[i][j].rgbtRed = 128 + buf / 4;
            }
            for (int j = hshift; j < file.InfoHeader.biWidth; ++j) {
                buf = (file.Info[i][j].rgbtBlue + file.Info[i][j].rgbtGreen + file.Info[i][j].rgbtRed) / 3;
                bufp = (file.Info[i][j - hshift].rgbtBlue + file.Info[i][j - hshift].rgbtGreen + file.Info[i][j - hshift].rgbtRed) / 3;
                //buf = bufp + buf / 2;
                file1.Info[i][j].rgbtBlue = std::max(std::abs(buf - bufp), buf / 2);
                file1.Info[i][j].rgbtGreen = std::max(std::abs(buf - bufp), 64 + 3 * buf / 8);
                file1.Info[i][j].rgbtRed = std::max(std::abs(buf - bufp), 128 + buf / 4);
            }
            for (int j = file.InfoHeader.biWidth; j < file1.InfoHeader.biWidth; ++j) {
                buf = (file.Info[i][j - hshift].rgbtBlue + file.Info[i][j - hshift].rgbtGreen + file.Info[i][j - hshift].rgbtRed) / 3;
                file1.Info[i][j].rgbtBlue = buf / 2;
                file1.Info[i][j].rgbtGreen = 64 + 3 * buf / 8;
                file1.Info[i][j].rgbtRed = 128 + buf / 4;
            }
        }
        WriteFile(file1, "Nadejda_кофе_1.bmp");
    }
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
