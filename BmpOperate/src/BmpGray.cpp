#include "../header/BmpGray.h"
#include <string>
#include <cstring>





BmpGray::BmpGray() {
    Imgdata = nullptr;
    ipRGB = nullptr;
}

BmpGray::BmpGray(const BmpGray& a) {
    *this = a;
}

BmpGray::BmpGray(BmpGray&& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
}

BmpGray::~BmpGray() {
    delete[] Imgdata;
    Imgdata = nullptr;
    delete[] ipRGB;
    ipRGB = nullptr;
}

BmpGray& BmpGray::operator = (const BmpGray& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    const uint32_t Size = ((Bi.biWidth * 3 + 3) / 4) * 4 * Bi.biHeight;
    delete[] Imgdata;
    Imgdata = new BYTE[Size];
    memcpy(Imgdata, a.Imgdata, Size);
    if (ipRGB == nullptr) {
        ipRGB = new RGBQUAD[256];
        for (uint16_t i = 0; i < 256; ++i) {
            RGBQUAD& RGB = ipRGB[i];
            RGB.rgbBlue = RGB.rgbGreen = RGB.rgbRed = static_cast<BYTE>(i);
        }
    }
    return *this;
}

BmpGray& BmpGray::operator = (BmpGray&& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    delete[] Imgdata;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    delete[] ipRGB;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
    return *this;
}

void BmpGray::bitmap_read(const char* FileName) {
    FILE* fp = fopen(FileName, "rb");
    if (fp == NULL)    exit(1);
    fread(&Bf, sizeof(Bf), 1, fp);
    fread(&Bi, sizeof(Bi), 1, fp);
    ipRGB = new RGBQUAD[256];
    fread(ipRGB, sizeof(ipRGB), 256, fp);
    const uint32_t Size = ((Bi.biWidth + 3) / 4) * 4 * Bi.biHeight;
    Imgdata = new BYTE[Size];
    fread(Imgdata, 1, Size, fp);
    fclose(fp);
}

void BmpGray::bitmap_write(const char* FileName) const {
    FILE* fp = fopen(FileName, "wb");
    if (fp == NULL || ipRGB == nullptr || Imgdata == nullptr)    exit(1);
    fwrite(&Bf, sizeof(Bf), 1, fp);
    fwrite(&Bi, sizeof(Bi), 1, fp);
    fwrite(ipRGB, sizeof(*ipRGB), 256, fp);
    const uint32_t Size = ((Bi.biWidth + 3) / 4) * 4 * Bi.biHeight;
    fwrite(Imgdata, Size, 1, fp);
    fclose(fp);
}

BmpGray& BmpGray::ConvertFrom24Bit(const BmpBasic& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    Bi.biBitCount = 8;
    Bi.biSizeImage = ((a.Bi.biWidth + 3) / 4) * 4 * a.Bi.biHeight;
    Bf.bfOffBits = sizeof(Bf) + sizeof(Bi) + 256 * sizeof(*ipRGB);
    Bf.bfSize = Bf.bfOffBits + Bi.biSizeImage;
    ipRGB = new RGBQUAD[256];
    for (uint16_t i = 0; i < 256; ++i)
        ipRGB[i].rgbBlue = ipRGB[i].rgbGreen = ipRGB[i].rgbRed = (uint8_t)i;
    const uint32_t nBytePerLine = ((a.Bi.biWidth + 3) / 4) * 4;
    const uint32_t nBytePerLine_a = ((a.Bi.biWidth * 3 + 3) / 4) * 4;
    Imgdata = new BYTE[nBytePerLine * Bi.biHeight];
    for (uint32_t i = 0; i < (uint32_t)Bi.biHeight; ++i) {
        for (uint32_t j = 0; j < (uint32_t)Bi.biWidth; ++j) {
            const BYTE& B = a.Imgdata[i*nBytePerLine_a + j * 3];
            const BYTE& G = a.Imgdata[i*nBytePerLine_a + j * 3 + 1];
            const BYTE& R = a.Imgdata[i*nBytePerLine_a + j * 3 + 2];
            Imgdata[i*nBytePerLine + j] = (R * 38 + G * 75 + B * 15) >> 7;
        }
    }
    return *this;
}
