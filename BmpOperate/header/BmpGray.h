#ifndef _HEADER_BMPGRAY_H_
#define _HEADER_BMPGRAY_H_
#include <memory>
#include "../header/bitmap_struct.h"
#include "../header/BmpBasic.h"


class BmpGray {    // 8bit gray bitmap
 public:
    friend class BmpOpt;
    BmpGray();
    BmpGray(const BmpGray& a);
    BmpGray(BmpGray&& a);
    ~BmpGray();
    BmpGray& operator = (const BmpGray& a);
    BmpGray& operator = (BmpGray&& a);
    void bitmap_read(const char* FileName);
    void bitmap_write(const char* FileName) const;
    BmpGray& ConvertFrom24Bit(const BmpBasic& a);
 private:
    BITMAPFILEHEADER Bf;
    BITMAPINFOHEADER Bi;
    RGBQUAD* ipRGB;
    BYTE* Imgdata;    // data of bitmap
};

#endif    // _HEADER_BMPBASIC_H_
