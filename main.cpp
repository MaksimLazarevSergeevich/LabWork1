#include "bmpimage.h"

int main() {
    const char* filename = "imag.bmp";
    BmpImage amg;
    amg.loadFile(filename);
    amg.rotateOclock();
    amg.writeFile("outFile_ro1.bmp");
    amg.rotateUnOclock();
    amg.rotateUnOclock();
    amg.writeFile("out_2.bmp");
}