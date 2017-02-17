#ifndef _HEADER_BMPOPT_H_
#define _HEADER_BMPOPT_H_
#include"../header/bitmap_struct.h"
#include"../header/BmpGray.h"


class BmpOpt : private BmpGray {
 public:
    BmpOpt();
    BmpOpt(const BmpGray& a);
    BmpOpt(BmpGray&& a);
    BmpOpt(const BmpOpt& a);
    BmpOpt(BmpOpt&& a);
    ~BmpOpt();
    BmpOpt& operator = (const BmpOpt& a);
    BmpOpt& operator = (BmpOpt&& a);
    BmpOpt& ConvertFromGray(const BmpGray& a);
    BmpGray ConvertToGray() const;
    void smooth(const double& sigma = 1);
    void Canny(const double& sigma = 1, double EP_MAX = 0, double EP_MIN = 0);
 private:
    BmpOpt& operator = (const BmpGray& a);
    BmpOpt& operator = (BmpGray&& a);
    void GetTemp(const double& sigma = 1);
    BYTE** BmpMat;    // bitmap matrix
    double* Temp;    // template of gaussian faction
    uint32_t TempSize;    // size of template
    double TempSigma;    // sigma of template
};

#endif    // _HEADER_BMPOPT_H_
