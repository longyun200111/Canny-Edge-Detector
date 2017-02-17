#include "../header/BmpOpt.h"
#include <cmath>
#include <queue>
#include <cstring>



BmpOpt::BmpOpt() {
    BmpMat = nullptr;
    Temp = nullptr;
    Imgdata = nullptr;
    ipRGB = nullptr;
}


BmpOpt::BmpOpt(const BmpGray& a) {
    *this = a;
}


BmpOpt::BmpOpt(BmpGray&& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
}


BmpOpt::BmpOpt(const BmpOpt& a) {
    *this = a;
}


BmpOpt::BmpOpt(BmpOpt&& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    TempSigma = a.TempSigma;
    TempSize = a.TempSize;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    BmpMat = a.BmpMat;
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        BmpMat[i] = a.BmpMat[i];
        a.BmpMat[i] = nullptr;
    }
    a.BmpMat = nullptr;
    Temp = a.Temp;
    a.Temp = nullptr;
}


BmpOpt::~BmpOpt() {
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        delete[] BmpMat[i];
        BmpMat[i] = nullptr;
    }
    delete[] BmpMat;
    BmpMat = nullptr;
    delete[] Temp;
    Temp = nullptr;
}


BmpOpt& BmpOpt::operator = (const BmpOpt& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    TempSigma = a.TempSigma;
    TempSize = a.TempSize;
    if (ipRGB == nullptr) {
        ipRGB = new RGBQUAD[256];
        for (uint16_t i = 0; i < 256; ++i) {
            RGBQUAD& RGB = ipRGB[i];
            RGB.rgbBlue = RGB.rgbGreen = RGB.rgbRed = static_cast<BYTE>(i);
        }
    }
    const uint32_t Size = ((Bi.biWidth + 3) / 4) * 4 * Bi.biHeight;
    delete[] Imgdata;
    Imgdata = new BYTE[static_cast<size_t>(Size)];
    memcpy(Imgdata, a.Imgdata, static_cast<size_t>(Size));
    if (a.BmpMat != nullptr) {
        if (BmpMat != nullptr) {
            for (uint32_t i = 0; i < Bi.biHeight; ++i)
                delete[] BmpMat[i];
            delete[] BmpMat;
        }
        BmpMat = new BYTE*[Bi.biHeight];
        for (uint32_t i = 0; i < Bi.biHeight; ++i) {
            BmpMat[i] = new BYTE[Bi.biWidth];
            memcpy(BmpMat[i], a.BmpMat[i], Bi.biWidth);
        }
    }
    if (a.Temp != nullptr) {
        delete[] Temp;
        Temp = new double[TempSize];
        memcpy(Temp, a.Temp, TempSize);
    }
    return *this;
}


BmpOpt& BmpOpt::operator = (BmpOpt&& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    TempSigma = a.TempSigma;
    TempSize = a.TempSize;
    delete[] ipRGB;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
    delete[] Imgdata;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    if (BmpMat != nullptr) {
        for (uint32_t i = 0; i < Bi.biHeight; ++i)
            delete[] BmpMat[i];
        delete[] BmpMat;
    }
    BmpMat = a.BmpMat;
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        BmpMat[i] = a.BmpMat[i];
        a.BmpMat[i] = nullptr;
    }
    a.BmpMat = nullptr;
    delete[] Temp;
    Temp = a.Temp;
    a.Temp = nullptr;
    return *this;
}


BmpOpt& BmpOpt::operator = (const BmpGray& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    if (ipRGB == nullptr) {
        ipRGB = new RGBQUAD[256];
        for (uint16_t i = 0; i < 256; ++i) {
            RGBQUAD& RGB = ipRGB[i];
            RGB.rgbBlue = RGB.rgbGreen = RGB.rgbRed = static_cast<BYTE>(i);
        }
    }
    const uint32_t Size = ((Bi.biWidth + 3) / 4) * 4 * Bi.biHeight;
    delete[] Imgdata;
    Imgdata = new BYTE[Size];
    memcpy(Imgdata, a.Imgdata, Size);
    return *this;
}


BmpOpt& BmpOpt::operator = (BmpGray&& a) {
    Bi = a.Bi;
    Bf = a.Bf;
    delete[] ipRGB;
    ipRGB = a.ipRGB;
    a.ipRGB = nullptr;
    delete[] Imgdata;
    Imgdata = a.Imgdata;
    a.Imgdata = nullptr;
    return *this;
}


BmpOpt& BmpOpt::ConvertFromGray(const BmpGray& a) {
    *this = a;
    BmpMat = new BYTE*[Bi.biHeight];
    const uint32_t nBytePerLine = ((Bi.biWidth + 3) / 4) * 4;
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        BmpMat[i]= new BYTE[Bi.biWidth];
        memcpy(BmpMat[i], &Imgdata[i*nBytePerLine], Bi.biWidth);
    }
    return *this;
}


void BmpOpt::GetTemp(const double& sigma) {
    TempSigma = sigma;
    TempSize = ((static_cast<uint32_t>(ceil(sigma * 3))) << 1) + 1;
    Temp = new double[TempSize];
    if (sigma == 0) {
        Temp[0] = 1;
        return;
    }
    double add = 0;
    const uint32_t radius = TempSize >> 1;
    const double SigmaPow = sigma * sigma * 2;
    for (uint32_t i = 0; i < TempSize; ++i) {
        int x = i - radius;
        Temp[i] = exp(-1 * x * x /SigmaPow);
        add += Temp[i];
    }
    for (uint32_t i = 0; i < TempSize; ++i)
        Temp[i] /= add;
}


BmpGray BmpOpt::ConvertToGray() const {
    BmpGray gray;
    const uint32_t nBytePerLine = ((Bi.biWidth + 3) / 4) * 4;
    gray.Imgdata = new BYTE[nBytePerLine*Bi.biHeight];
    gray.ipRGB = new RGBQUAD[256];
    const size_t Size = sizeof(Imgdata[0])*nBytePerLine*Bi.biHeight;
    memset(Imgdata, 0, Size);
    for (uint32_t i = 0; i < Bi.biHeight; ++i)
        memcpy(gray.Imgdata + i*nBytePerLine, BmpMat[i], Bi.biWidth);
    for (uint16_t i = 0; i < 256; ++i) {
        RGBQUAD& RGB = gray.ipRGB[i];
        RGB.rgbBlue = RGB.rgbGreen = RGB.rgbRed = static_cast<BYTE>(i);
    }
    gray.Bf = Bf;
    gray.Bi = Bi;
    return gray;
}


void BmpOpt::smooth(const double& sigma) {
    GetTemp(sigma);
    BYTE** ans = new BYTE*[Bi.biHeight];
    for (uint32_t i = 0; i < Bi.biHeight; ++i)
        ans[i] = new BYTE[Bi.biWidth];
    const int HalfLen = TempSize >> 1;
    for (int32_t y = 0; y < static_cast<int32_t>(Bi.biHeight); ++y) {
        for (int32_t x = 0; x < static_cast<int32_t>(Bi.biWidth); ++x) {
            double center = 0;
            for (int32_t i = -HalfLen; i <= HalfLen; ++i)
                if (x + i >= 0 && x + i < static_cast<int32_t>(Bi.biWidth))
                    center += BmpMat[y][x + i] * Temp[i + HalfLen];
            ans[y][x] = static_cast<BYTE>(center + 0.5);
        }
    }
    for (int32_t x = 0; x < static_cast<int32_t>(Bi.biWidth); ++x) {
        for (int32_t y = 0; y < static_cast<int32_t>(Bi.biHeight); ++y) {
            double center = 0;
            for (int32_t i = -HalfLen; i <= HalfLen; ++i)
                if (y + i >= 0 && y + i < static_cast<int32_t>(Bi.biHeight))
                    center += ans[y + i][x] * Temp[i + HalfLen];
            BmpMat[y][x] = static_cast<BYTE>(center + 0.5);
        }
    }
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        delete[] ans[i];
        ans[i] = nullptr;
    }
    delete[] ans;
    ans = nullptr;
}


void BmpOpt::Canny(const double& sigma, double EP_MAX, double EP_MIN) {
    smooth(sigma);
    struct Node {
        uint32_t x; uint32_t y;
        Node(const uint32_t& i, const uint32_t& j) :x(i), y(j) {}
    };
    int **dx = new int*[Bi.biHeight], **dy = new int*[Bi.biHeight];
    BYTE **edge = new BYTE*[Bi.biHeight];
    double **d = new double*[Bi.biHeight];
    double **temp = new double*[Bi.biHeight];
    double **EdgeLarge = new double*[Bi.biHeight];
    double **EdgeLittle = new double*[Bi.biHeight];
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        dx[i] = new int[Bi.biWidth], dy[i] = new int[Bi.biWidth];
        temp[i] = new double[Bi.biWidth], EdgeLarge[i] = new double[Bi.biWidth];
        EdgeLittle[i] = new double[Bi.biWidth], d[i] = new double[Bi.biWidth];
        edge[i] = new BYTE[Bi.biWidth];
    }
    for (uint32_t i = 0; i < Bi.biHeight - 1; ++i) {
        for (uint32_t j = 0; j < Bi.biWidth - 1; ++j) {
            dx[i][j] = BmpMat[i][j + 1] - BmpMat[i][j];
            dy[i][j] = BmpMat[i + 1][j] - BmpMat[i][j];
            d[i][j] = sqrt(dx[i][j] * dx[i][j] + dy[i][j] * dy[i][j]);
        }
    }
    for (uint32_t i = 0; i < Bi.biHeight; ++i)
        memset(temp[i], 0, sizeof(temp[0][0])*Bi.biWidth);
    for (uint32_t i = 1; i < Bi.biHeight - 1; ++i) {
        for (uint32_t j = 1; j < Bi.biWidth - 1; ++j) {
            if (d[i][j] == 0)
                continue;
            int gradX = dx[i][j], gradY = dy[i][j];
            double grad = d[i][j];
            double weight, grad1, grad2, grad3, grad4;
            if (abs(gradY) > abs(gradX)) {
                weight = abs(gradX) / abs(gradY);
                grad2 = d[i - 1][j];
                grad4 = d[i + 1][j];
                if (gradX*gradY > 0) {
                    /*    G1        G2
                                    G
                                    G4        G3*/
                    grad1 = d[i - 1][j - 1];
                    grad3 = d[i + 1][j + 1];
                } else {
                    /*            G2        G1
                                  G
                        G3        G4            */
                    grad1 = d[i - 1][j + 1];
                    grad3 = d[i + 1][j - 1];
                }
            } else {
                weight = abs(gradY) / abs(gradX);
                grad2 = d[i][j + 1];
                grad4 = d[i][j - 1];
                if (gradX*gradY > 0) {
                    /* G3
                       G4        G        G2
                                          G1*/
                    grad1 = d[i + 1][j + 1];
                    grad3 = d[i - 1][j - 1];
                } else {
                    /*                     G1
                        G4        G        G2
                        G3                    */
                    grad1 = d[i - 1][j + 1];
                    grad3 = d[i + 1][j - 1];
                }
            }
            double GradTemp1, GradTemp2;
            GradTemp1 = weight*grad1 + (1 - weight)*grad2;
            GradTemp2 = weight*grad3 + (1 - weight)*grad4;
            if (grad >= GradTemp1&&grad >= GradTemp2)
                temp[i][j] = grad;
        }
    }
    if (EP_MAX == 0) {
        uint32_t Hist[512];
        uint32_t EdgeNum = 0, Max;
        memset(Hist, 0, sizeof(Hist[0]) * 512);
        for (uint32_t i = 0; i < Bi.biHeight; ++i)
            for (uint32_t j = 0; j < Bi.biWidth; ++j)
                if (temp[i][j] > 0)
                    ++Hist[static_cast<size_t>(temp[i][j] + 0.5)];
        for (uint32_t i = 1; i < 512; ++i) {
            if (Hist[i] > 0) {
                Max = i;
                EdgeNum += Hist[i];
            }
        }
        uint32_t HighCount = (uint32_t)(EdgeNum*0.7 + 0.5), count = 1;
        EdgeNum = Hist[1];
        while (count < Max - 1 && EdgeNum <= HighCount) {
            ++count;
            EdgeNum += Hist[count];
        }
        EP_MAX = static_cast<double>(count);
        EP_MIN = EP_MAX*0.4;
    } else if (EP_MIN == 0 || EP_MAX < EP_MIN)
        EP_MIN = EP_MAX*0.4;
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        for (uint32_t j = 0; j < Bi.biWidth; ++j) {
            EdgeLarge[i][j] = EdgeLittle[i][j] = 0;
            if (temp[i][j] >= EP_MAX)
                EdgeLarge[i][j] = temp[i][j];
            else if (temp[i][j] >= EP_MIN)
                EdgeLittle[i][j] = temp[i][j];
        }
    }
    std::queue<Node> q;
    for (uint32_t i = 0; i < Bi.biHeight; ++i)
        memset(edge[i], 255, sizeof(edge[0][0])*Bi.biWidth);
    for (uint32_t i = 1; i < Bi.biHeight - 1; ++i) {
        for (uint32_t j = 1; j < Bi.biWidth - 1; ++j) {
            if (EdgeLarge[i][j]) {
                q.push(Node(i, j));
                edge[i][j] = 0;
                EdgeLarge[i][j] = 0;
            }
            while (!q.empty()) {
                uint32_t x = q.front().x, y = q.front().y;
                q.pop();
                if (EdgeLittle[x - 1][y - 1]) {
                    edge[x - 1][y - 1] = 128;
                    EdgeLittle[x - 1][y - 1] = 0;
                    q.push(Node(x - 1, y - 1));
                }
                if (EdgeLittle[x - 1][y]) {
                    edge[x - 1][y] = 128;
                    EdgeLittle[x - 1][y] = 0;
                    q.push(Node(x - 1, y));
                }
                if (EdgeLittle[x - 1][y + 1]) {
                    edge[x - 1][y + 1] = 128;
                    EdgeLittle[x - 1][y + 1] = 0;
                    q.push(Node(x - 1, y + 1));
                }
                if (EdgeLittle[x][y - 1]) {
                    edge[x][y - 1] = 128;
                    EdgeLittle[x][y - 1] = 0;
                    q.push(Node(x, y - 1));
                }
                if (EdgeLittle[x][y + 1]) {
                    edge[x][y + 1] = 128;
                    EdgeLittle[x][y + 1] = 0;
                    q.push(Node(x, y + 1));
                }
                if (EdgeLittle[x + 1][y - 1]) {
                    edge[x + 1][y - 1] = 128;
                    EdgeLittle[x + 1][y - 1] = 0;
                    q.push(Node(x + 1, y - 1));
                }
                if (EdgeLittle[x + 1][y]) {
                    edge[x + 1][y] = 128;
                    EdgeLittle[x + 1][y] = 0;
                    q.push(Node(x + 1, y));
                }
                if (EdgeLittle[x + 1][y + 1]) {
                    edge[x + 1][y + 1] = 128;
                    EdgeLittle[x + 1][y + 1] = 0;
                    q.push(Node(x + 1, y + 1));
                }
            }
        }
    }
    for (uint32_t i = 0; i < Bi.biHeight; ++i)
        memcpy(BmpMat[i], edge[i], Bi.biWidth);
    for (uint32_t i = 0; i < Bi.biHeight; ++i) {
        delete[] dx[i];
        delete[] dy[i];
        delete[] d[i];
        delete[] temp[i];
        delete[] EdgeLarge[i];
        delete[] EdgeLittle[i];
        delete[] edge[i];
        edge[i] = nullptr;
        dx[i] = dy[i] = nullptr;
        d[i] = temp[i] = EdgeLarge[i] = EdgeLittle[i] = nullptr;
    }
    delete[] dx;
    delete[] dy;
    delete[] d;
    delete[] temp;
    delete[] EdgeLarge;
    delete[] EdgeLittle;
    delete[] edge;
    edge = nullptr;
    dx = dy = nullptr;
    d = temp = EdgeLarge = EdgeLittle = nullptr;
}
