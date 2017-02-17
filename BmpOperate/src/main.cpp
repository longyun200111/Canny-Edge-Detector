#include"../header/BmpOpt.h"
#include<ctime>
#include<iostream>

int main() {
    clock_t st, fi;
    st = clock();
    BmpBasic bmp;
    bmp.bitmap_read("C:\\Users\\lenovo\\Desktop\\程序\\BmpOperate\\BmpOperate\\b.bmp");
    BmpGray gray;
    gray.ConvertFrom24Bit(bmp);
    BmpOpt opt;
    opt.ConvertFromGray(gray);
    opt.Canny();
    gray = opt.ConvertToGray();
    gray.bitmap_write("edge.bmp");
    fi = clock();
    std::cout << static_cast<double>(fi - st)/CLOCKS_PER_SEC;
    system("pause");
    return 0;
}
