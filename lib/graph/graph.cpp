#include "graph.h"
#include <stdio.h>

Graph::Graph(Paint* paint, DataBuffer* dataBuffer, int lineColour ) : paint(paint), dataBuffer(dataBuffer), lineColour(lineColour)
{
    // Constructor implementation
}

Graph::~Graph()
{
    // Destructor implementation
}

void Graph::DrawDoubleGraph(int x, int y, int width, int height, int currentHour, int currentMinute )
{
    
    int co2, sound, hour, minute;
    int indexer = dataBuffer->CreateIndexer(); // Create an indexer for the data buffer
    int recordCount = dataBuffer->GetRecordCount(); // Get the number of records in the buffer

    //Clcukate co2 range
    int maxCO2 = (dataBuffer->GetMaxCO2() /200 + 1)*200; // Adjust the maxCO2 value to fit within the graph height
    int minCO2 = (dataBuffer->GetMinCO2() /200) * 200; // Adjust the minCO2 value to fit within the graph height

    int maxSound = (dataBuffer->GetMaxSound() / 10 + 1) *10; // Adjust the maxSound value to fit within the graph height
    int minSound = (dataBuffer->GetMinSound() / 10)*10; // Adjust the minSound value to fit within the graph height

    int graphHeight = height/2 - 8; // Leave space for labels and axes
    int graphWidth = width - 30; // Leave space for labels and axes
    int graphX = x + 30;
    int graphCo2Y = y+graphHeight; // Starting Y position for the graph
    int graphSoundY = y+height; // Starting Y position for the graph
    int xScaleY = y + graphHeight + 1; // Y position for the X-axis labels


    

    // Draw the outline of the graph with text *****

    paint ->DrawRectangle(graphX, y, graphX + graphWidth, y + graphHeight, lineColour); // Draw the outline of the  CO2 graph
    paint ->DrawRectangle(graphX, graphSoundY, graphX + graphWidth, graphSoundY - graphHeight, lineColour); // Draw the outline of the  CO2 graph

    //Draw tics and axis labels


    int co2Scale = (maxCO2 - minCO2); // Calculate the scale for CO2
    if(co2Scale == 0) co2Scale = 1; // Avoid division by zero
    int soundScale = (maxSound - minSound); // Calculate the scale for sound
    if(soundScale == 0) soundScale = 1; // Avoid division by zero


    char buffer[20];
    sprintf(buffer, "%4d", maxCO2  ); 
    paint->DrawStringAt( x, y , buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", (maxCO2 + minCO2)/2  ); 
    paint->DrawStringAt( x, y + graphHeight/ 2 - 5 , buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", minCO2  ); 
    paint->DrawStringAt( x, graphCo2Y - 9 , buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", maxSound  ); 
    paint->DrawStringAt( x , graphSoundY - graphHeight, buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", (minSound + maxSound)/2  ); 
    paint->DrawStringAt( x , graphSoundY - graphHeight / 2 - 5, buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", minSound  ); 
    paint->DrawStringAt( x, graphSoundY - 9 , buffer, &Font12, lineColour); // Draw CO2 label

    sprintf(buffer, "%02d:%02d", currentHour, currentMinute);
    paint->DrawStringAt( x + width - 34, graphCo2Y + 4 , buffer, &Font12, lineColour); // Draw CO2 label

    
    // Draw the graph here using the paint object and data from the dataBuffer
    // You can use paint->DrawPixel() or other methods to draw the graph on the screen

    int lastx, lastCO2y, lastSoundy;
    bool firstTime = true;
    int i=0;
    int initialHour =0, initialMinute = 0;
    
    while (dataBuffer->GetNextRecord(co2, sound, hour, minute, indexer)) 
    {

        int xPos = graphX + (i * graphWidth) / recordCount; // Calculate the X position for the current data point
        // Use co2 and sound values to plot points on the graph

        int co2Y = graphCo2Y - ((co2 - minCO2) * graphHeight) / co2Scale; // Calculate the Y position for CO2
        int soundY = graphSoundY - ((sound - minSound) * graphHeight) / soundScale; // Calculate the Y position for sound

        if( firstTime)
        {
                firstTime = false; // Set the flag to false after the first point
                initialHour = hour; // Store the initial hour and minute for the first point
                initialMinute = minute; // Store the initial hour and minute for the first point
        }
        else
        {
            // Draw lines between points for CO2 and sound levels
            paint->DrawLine(lastx, lastCO2y, xPos, co2Y, lineColour); // Draw line for CO2
            paint->DrawLine(lastx, lastSoundy, xPos, soundY, lineColour); // Draw line for sound
            
        }

        lastx = xPos;
        lastCO2y = co2Y;
        lastSoundy = soundY;
        i++;

    //     paint->DrawPixel(xPos, graphCo2Y - (( co2 - minCO2) * graphHeight )/ co2Scale, lineColour); // Adjust scaling as needed
    //     paint->DrawPixel(xPos, graphSoundY - (( sound - minSound) * graphHeight )/ soundScale, lineColour); // Adjust scaling as needed
    }

    if( firstTime == false)
    {
        // Draw the last point for CO2 and sound levels    
        sprintf(buffer, "%02d:%02d", initialHour, initialMinute);
        paint->DrawStringAt( graphX, graphCo2Y + 4 , buffer, &Font12, lineColour); // Draw CO2 label

    }


}
