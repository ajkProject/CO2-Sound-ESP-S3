#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "epdpaint.h"

class ProgressBar : public Paint 
{

public:
    ProgressBar(unsigned char* image, int width, int height, bool grayScale = false);
    
    void Bar(int x, int y, int width, int height, int progress, int maxProgress, int minValue, int maxValue, int colored);

};

#endif // GRAPH_H