#ifndef _HEADER_BMPBASIC_H_
#define _HEADER_BMPBASIC_H_
#include <memory>
#include "../header/bitmap_struct.h"



class BmpBasic {    // 24bit bitmap
 public:
    friend class BmpGray;
    BmpBasic();
    BmpBasic(const BmpBasic& a);
    BmpBasic(BmpBasic&& a);
    ~BmpBasic();
    BmpBasic& operator = (const BmpBasic& a);
    BmpBasic& operator = (BmpBasic&& a);
    void bitmap_read(const char* FileName);
    void bitmap_write(const char* FileName) const;
 private:
    BITMAPFILEHEADER Bf;
    BITMAPINFOHEADER Bi;
    BYTE* Imgdata;    // data of bitmap
};

#endif    // _HEADER_BMPBASIC_H_
