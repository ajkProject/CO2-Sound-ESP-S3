#include "dataBuffer.h"
#include "epdpaint.h"

class Graph
{

    public:

        Graph(Paint* paint, DataBuffer* dataBuffer, int lineColour );
        ~Graph();

        void DrawDoubleGraph(int x, int y, int width, int height, int currentHour, int currentMinute );
    
    private:

        Paint* paint;
        DataBuffer* dataBuffer;
        int lineColour;
       

};