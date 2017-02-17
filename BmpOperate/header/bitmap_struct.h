#ifndef _HEADER_BITMAP_STRUCT_H_
#define _HEADER_BITMAP_STRUCT_H_
#include <cstdint>
#pragma pack(push)
#pragma pack(2)
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint32_t LONG;
typedef struct tagBITMAPFILEHEADER {    // 位图文件头
    WORD    bfType;             // 文件类型，必为0x4D42（“BM”）
    DWORD    bfSize;            // 文件大小，单位为字节
    WORD        bfReserved1;    // 保留，必为0
    WORD        bfReserved2;    // 保留，必为0
    DWORD     bfOffBits;        // 从文件头开始到实际图像数据间的字节偏移量
}BITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {    // 位图信息头
    DWORD    biSize;             // 结构所需字节数
    LONG    biWidth;             // 图像宽度，单位为像素
    LONG    biHeight;            // 图像高度，单位为像素（正值为倒向，负值为正向）
    WORD    biPlanes;            // 位面数，总为1
    WORD    biBitCount;          // 比特数/像素，值为1、4、8、16、24或32
    DWORD    biCompression;      // 图像数据压缩类型，值为BI_RGB、BI_RLE8、BI_RLE4或BI_BITFIELDS
    DWORD    biSizeImage;        // 图像大小，单位为字节。当用BI_RGB格式时可为0
    LONG    biXPelsPerMeter;     // 水平分辨率，单位为像素/米
    LONG    biYPelsPerMeter;     // 垂直分辨率，单位为像素/米
    DWORD    biClrUsed;          // 实际使用的彩色表的颜色索引数，如果为0则说明使用所有调色板项
    DWORD    biClrImportant;     // 对图像显示有重要影响的颜色索引的数目，如果为0则表示都重要
}BITMAPINFOHEADER;


typedef struct tagRGBQUAD {    // 颜色表
    BYTE    rgbBlue;    // 蓝色强度
    BYTE    rgbGreen;    // 绿色强度
    BYTE    rgbRed;    // 红色强度
    BYTE    rgbReserved;    // 保留，必为0
}RGBQUAD;

#pragma pack(pop)
#endif    // _HEADER_BITMAP_STRUCT_H_