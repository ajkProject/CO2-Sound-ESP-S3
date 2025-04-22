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
    
    int co2, sound, maxIntSound, hour, minute;
    int indexer = dataBuffer->CreateIndexer(); // Create an indexer for the data buffer
    int recordCount = dataBuffer->GetRecordCount(); // Get the number of records in the buffer

    //Clcukate co2 range
    int maxCO2 = (dataBuffer->GetMaxCO2() /200 + 1)*200; // Adjust the maxCO2 value to fit within the graph height
    int minCO2 = (dataBuffer->GetMinCO2() /200) * 200; // Adjust the minCO2 value to fit within the graph height

    int maxSound = (dataBuffer->GetMaxSound() / 10 + 1) *10; // Adjust the maxSound value to fit within the graph height
    int minSound = (dataBuffer->GetMinSound() / 10)*10; // Adjust the minSound value to fit within the graph height

    int graphHeight = height/2 - 5; // Leave space for labels and axes
    int graphWidth = width - 21; // Leave space for labels and axes
    int graphX = x + 21;
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
    paint->DrawStringAt( x, y , buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", (maxCO2 + minCO2)/2  ); 
    paint->DrawStringAt( x, y + graphHeight/ 2 - 3 , buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", minCO2  ); 
    paint->DrawStringAt( x, graphCo2Y - 5 , buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", maxSound  ); 
    paint->DrawStringAt( x , graphSoundY - graphHeight, buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", (minSound + maxSound)/2  ); 
    paint->DrawStringAt( x , graphSoundY - graphHeight / 2 - 3, buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%4d", minSound  ); 
    paint->DrawStringAt( x, graphSoundY - 5 , buffer, &Font8, lineColour); // Draw CO2 label

    sprintf(buffer, "%02d:%02d", currentHour, currentMinute);
    paint->DrawStringAt( x + width - 24, graphCo2Y + 3 , buffer, &Font8, lineColour); // Draw CO2 label

    paint->DrawStringAt( graphX + 5 , y + 2, "CO", &Font8, lineColour); // Draw CO2 label
    paint->DrawStringAt( graphX + 15 , y + 5, "2", &Font8, lineColour); // Draw CO2 label

    paint->DrawStringAt( x , y + graphHeight / 4 - 3, "ppm", &Font8, lineColour); // Draw CO2 label
 
    paint->DrawStringAt( graphX + 5 , graphSoundY - graphHeight + 2, "Noise", &Font8, lineColour); // Draw CO2 label
    paint->DrawStringAt( x , graphSoundY - graphHeight * 3 / 4 - 3, "dB", &Font8, lineColour); // Draw CO2 label
    
    // Draw the graph here using the paint object and data from the dataBuffer
    // You can use paint->DrawPixel() or other methods to draw the graph on the screen

    int lastx, lastCO2y, lastSoundy;
    bool firstTime = true;
    int i=0;
    int initialHour =0, initialMinute = 0;
    bool newPixel = true;
    int currentX = 0;
    int pixelMaxSoundY = 0;
    
    while (dataBuffer->GetNextRecord(co2, sound, maxIntSound, hour, minute, indexer)) 
    {

        int xPos = graphX + (i * graphWidth) / recordCount; // Calculate the X position for the current data point
        // Use co2 and sound values to plot points on the graph

        int co2Y = graphCo2Y - ((co2 - minCO2) * graphHeight) / co2Scale; // Calculate the Y position for CO2
        int soundY = graphSoundY - ((sound - minSound) * graphHeight) / soundScale; // Calculate the Y position for sound
        int maxSoundY = graphSoundY - ((maxIntSound - minSound) * graphHeight) / soundScale; // Calculate the Y position for sound
        
       

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

        if( currentX != xPos)
        {
            paint->DrawPixel(currentX, pixelMaxSoundY, lineColour); // Draw pixel for CO2
            currentX = xPos; // Update the current X position
            pixelMaxSoundY = maxSoundY;
        }
        else
        {
            if( pixelMaxSoundY < maxSoundY)
            {
                pixelMaxSoundY = maxSoundY; // Update the maximum Y position for sound
            }
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
        paint->DrawStringAt( graphX, graphCo2Y + 3 , buffer, &Font8, lineColour); // Draw CO2 label

        //Mid point

        if( currentHour < initialHour )
        {
            currentHour += 24; // Adjust for midnight crossing
        }
        int midTime  = ((currentHour - initialHour) * 60 + (currentMinute - initialMinute)) / 2; // Calculate the mid time in minutes
        int midHour = initialHour + (midTime / 60); // Calculate the mid hour
        int midMinute = initialMinute + (midTime % 60); // Calculate the mid minute
          
        if( midHour >= 24 )
        {
            midHour -= 24; // Adjust for overflow in hours
        }

        sprintf(buffer, "%02d:%02d", midHour, midMinute);
        paint->DrawStringAt( graphX + graphWidth/2 - 12, graphCo2Y + 3 , buffer, &Font8, lineColour); // Draw CO2 label

    }


}
