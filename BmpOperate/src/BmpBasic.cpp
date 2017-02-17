#include "../header/BmpBasic.h"
#include <string>
#include <cstring>
#include <utility>





BmpBasic::BmpBasic() {
    Imgdata = nullptr;
}

BmpBasic::BmpBasic(const BmpBasic& a) {
    *this = a;
}

BmpBasic::BmpBasic(BmpBasic&& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
}

BmpBasic::~BmpBasic() {
    delete[] Imgdata;
    Imgdata = nullptr;
}

BmpBasic& BmpBasic::operator = (const BmpBasic& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    const uint32_t Size = ((Bi.biWidth * 3 + 3) / 4) * 4 * Bi.biHeight;
    delete[] Imgdata;
    Imgdata = new BYTE[Size];
    memcpy(Imgdata, a.Imgdata, Size);
    return *this;
}

BmpBasic& BmpBasic::operator = (BmpBasic&& a) {
    Bf = a.Bf;
    Bi = a.Bi;
    delete[] Imgdata;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    return *this;
}

void BmpBasic::bitmap_read(const char* FileName) {
    FILE* fp = fopen(FileName, "rb");
    if (fp == NULL)    exit(1);
    fread(&Bf, sizeof(Bf), 1, fp);
    fread(&Bi, sizeof(Bi), 1, fp);
    const uint32_t Size = ((Bi.biWidth * 3+ 3) / 4) * 4 * Bi.biHeight;
    Imgdata = new BYTE[Size];
    fread(Imgdata, 1, Size, fp);
    fclose(fp);
}

void BmpBasic::bitmap_write(const char* FileName) const {
    FILE* fp = fopen(FileName, "wb");
    if (fp == NULL || Imgdata == nullptr)    exit(1);
    fwrite(&Bf, sizeof(Bf), 1, fp);
    fwrite(&Bi, sizeof(Bi), 1, fp);
    const uint32_t Size = ((Bi.biWidth * 3 + 3) / 4) * 4 * Bi.biHeight;
    fwrite(Imgdata, 1, Size, fp);
    fclose(fp);
}
