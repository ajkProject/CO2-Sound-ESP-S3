
#include "ProgressBar.h"
#include <stdio.h>
#include <string.h>

ProgressBar::ProgressBar(unsigned char* image, int width, int height, bool grayScale) : Paint( image, width, height, grayScale)
{

}

void ProgressBar::Bar(int x, int y, int width, int height, int progress, int maxProgress, int minValue, int maxValue, int colored)
{

int textY = y + height - 8;
float barScale = width / (maxValue - minValue);

DrawRectangle(x, y, x + width -1, y + height - 10, colored);
DrawFilledRectangle(x, y, x + (progress - minValue) * barScale - 1, y + height - 10, colored);
DrawVerticalLine(x + (maxProgress - minValue) * barScale, y, height - 10, colored);

char buffer[16];
sprintf(buffer, "%d", minValue);

DrawStringAt(x, textY, buffer, &Font8, colored);

sprintf(buffer, "%d", maxValue);
DrawStringAt(x + width - Font8.Width * strlen(buffer), textY, buffer, &Font8, colored);


}


/* END OF FILE */