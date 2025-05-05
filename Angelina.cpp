// Angelina.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

typedef unsigned char BYTE;

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


int main()
{
    unsigned short int BfType;
    unsigned int BfSize;
    unsigned short int BfReserved1, BfReserved2;
    unsigned int BfOffBits;

    BITMAPINFOHEADER BitMapInfo;

    std::string name;

    //for (int s = 1; s < 32; ++s) {
        
        std::ifstream f_in("Ярослав Сергеевич.bmp", std::ios::binary);
        std::ofstream f_out("Ярослав Сергеевич жёлто-голубой 3.bmp", std::ios::binary);

        f_in.read(reinterpret_cast<char*>(&BfType), sizeof(unsigned short int));

        //std::cout.setf(std::ios::hex, std::ios::basefield);
        //std::cout << BfType << std::endl;

        f_out.write(reinterpret_cast<char*>(&BfType), sizeof(unsigned short int));

        f_in.read(reinterpret_cast<char*>(&BfSize), sizeof(unsigned int));

        std::cout.setf(std::ios::dec, std::ios::basefield);
        std::cout << BfSize << std::endl;

        f_out.write(reinterpret_cast<char*>(&BfSize), sizeof(unsigned int));

        f_in.read(reinterpret_cast<char*>(&BfReserved1), sizeof(unsigned short int));
        f_in.read(reinterpret_cast<char*>(&BfReserved2), sizeof(unsigned short int));
        f_in.read(reinterpret_cast<char*>(&BfOffBits), sizeof(unsigned int));

        f_out.write(reinterpret_cast<char*>(&BfReserved1), sizeof(unsigned short int));
        f_out.write(reinterpret_cast<char*>(&BfReserved2), sizeof(unsigned short int));
        f_out.write(reinterpret_cast<char*>(&BfOffBits), sizeof(unsigned int));

        f_in.read(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFOHEADER));

        //int EWidth = BitMapInfo.biWidth * sizeof(RGBTRIPLE);
        //int tail = 0;
        //while (EWidth % 4 != 0) {
        //    ++EWidth;
        //    ++tail;
        //}
        //BitMapInfo.biSizeImage = BitMapInfo.biHeight * EWidth;

        f_out.write(reinterpret_cast<char*>(&BitMapInfo), sizeof(BITMAPINFOHEADER));

        RGBTRIPLE pix;
        BYTE buf;
        BYTE dummy = 0;

        std::cout << BitMapInfo.biHeight << std::endl;
        std::cout << BitMapInfo.biWidth << std::endl;

        for (int i = 0; i < BitMapInfo.biHeight; ++i) {
            for (int j = 0; j < BitMapInfo.biWidth; ++j) {
                f_in.read(reinterpret_cast<char*>(&pix), 3);
                buf = (pix.rgbtRed + pix.rgbtGreen + pix.rgbtBlue) / 3;
                pix.rgbtBlue = buf;
                pix.rgbtGreen = 128 + buf / 2;
                pix.rgbtRed = 255 - buf;
                f_out.write(reinterpret_cast<char*>(&pix), 3);
            }
            //for (int j = 0; j < tail; ++j) {
            //    f_out.write(reinterpret_cast<char*>(&dummy), 1);
            //}
       }

        f_in.close();
        f_out.close();

 //   }

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
