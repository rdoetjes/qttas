#include "gamelogic.h"
#include <iostream>

void getMyPosition(QImage *img, int *myPosition){
   QColor pix = img->pixelColor(858, 798);
   if (pix.red() <40) myPosition[0] = 1;                    //demo for video

   pix = img->pixelColor(984, 798);
   if (pix.red() <40) myPosition[1] = 1;                    //demo for video

   pix = img->pixelColor(1106, 798);
   if (pix.red() <40) myPosition[2] = 1;                    //demo for video

   pix = img->pixelColor(1233, 798);                        //begin position car
   if (pix.red() <40) myPosition[4] = 1; 

   pix = img->pixelColor(733, 798);                         //hitchiker position car
   if (pix.red() <40) myPosition[3] = 1; 
}

void getObstacles(QImage *img, int *obstacles){
   QColor pix = img->pixelColor(852, 735);
   if (pix.red() <40) obstacles[0] = 1; 

   pix = img->pixelColor(960,712);
   if (pix.red() <40) obstacles[1] = 1; 

   pix = img->pixelColor(1030,726);
   if (pix.red() <40) obstacles[2] = 1; 
}

int movesToSafePlace(int pos, int *obstacles){
    //we have a bias for the center position
    if (obstacles[1] == 0) return 1 - pos;

    if (obstacles[0] == 0) return 0 - pos; 

    if (obstacles[2] == 0) return 2 - pos ;

    return 0;
}

void avoidObstacles(int *myPosition, int *obstacles){
   int pos = 0;
   
   //find the car's curremt position on screen
   while(!myPosition[pos]) { pos++; }

   //we have nothing to fear in start position or the hitchhiker position   
   std::cout << movesToSafePlace(pos, obstacles) << "\n";          

}

void gameLogic(QPixmap *screen){
   int obstacles[] {0, 0, 0} ; 
   int myPosition[] {0, 0, 0, 0, 0};

   QImage img = screen->toImage().convertToFormat(QImage::Format_Grayscale8);

   getObstacles(&img, obstacles);

   getMyPosition(&img, myPosition);

   avoidObstacles(myPosition, obstacles);

   return ;
}