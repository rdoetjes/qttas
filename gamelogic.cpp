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
    //when beginning we need to be very cary to merge into traffic
    if (pos == 4) if (obstacles[2] == 0) return 1 - pos; 

    //We have a bias for the fasdt lane :P
    if (obstacles[0] == 0) return 0 - pos; 

    if (obstacles[1] == 0) return 1 - pos;

    if (obstacles[2] == 0) return 2 - pos ;

    return 0;
}

int avoidObstacles(int *myPosition, int *obstacles){
   int pos = 0;
   
   //find the car's curremt position on screen
   while(!myPosition[pos]) { pos++; }

   //we have nothing to fear in start position or the hitchhiker position   
  return movesToSafePlace(pos, obstacles);

}

void sendKeyCode(Display* X11Handle, int key){
    unsigned int keycode = XKeysymToKeycode(X11Handle, key);
    XTestFakeKeyEvent(X11Handle, keycode, True, 0);
    XTestFakeKeyEvent(X11Handle, keycode, False, 0);
    XFlush(X11Handle);
}

void gameLogic(QPixmap *screen){
   static Display *X11Handle = NULL;

   if (X11Handle==NULL)
      X11Handle = XOpenDisplay(NULL);

   int obstacles[] {0, 0, 0} ; 
   int myPosition[] {0, 0, 0, 0, 0};

   QImage img = screen->toImage().convertToFormat(QImage::Format_Grayscale8);

   getObstacles(&img, obstacles);

   getMyPosition(&img, myPosition);

   int move = avoidObstacles(myPosition, obstacles);
   
   if (move < 0)
     for(int i=0; i<abs(move); i++) sendKeyCode(X11Handle, XK_Left);
   
   if( move > 0)
     for(int i=0; i<move; i++) sendKeyCode(X11Handle, XK_Right);

   return ;
}