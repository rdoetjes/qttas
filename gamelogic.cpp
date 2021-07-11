#include "gamelogic.h"

using namespace std;
typedef vector< vector<int> > Matrix;


/*
* Checks the X, Y pixel value for a RED value < threshold and then sets index to value
*/
void processImage(QImage *img, Matrix &decisionMatrix, int *gameObjectsToTrack){
   QColor pix;
   for (long unsigned int i=0; i < decisionMatrix.size(); i++){
       pix = img->pixelColor( decisionMatrix[i][0], decisionMatrix[i][1]);
       if (pix.red() < decisionMatrix[i][2]) gameObjectsToTrack[ decisionMatrix[i][3] ] = decisionMatrix[i][4];
   }  

}

/*
* Sets the position in myPosition array to 1 where the car is currently located on the screen
*/
void getMyPosition(QImage *img, int *myPosition){
    /*
    * Y, Y, RED VALUE LESS THAN, INDEX of MyPosition, Value to set it to
    */
    Matrix findCurrentCarPosition = {
        {858, 798, 40, 0, 1},
        {984, 798, 40, 1, 1},
        {1106, 798, 40, 2, 1},
        {1233, 798, 40, 4, 1},
        {733, 798, 40, 3, 1}

    };

    processImage(img, findCurrentCarPosition, myPosition);

 /*
  * This was the code as it was shown in the video
    As to easily describe the logic
    The above code does the smae but is cleaner
    (not faster though!)

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
   */
}

void getObstacles(QImage *img, int *obstacles){
   /*
   * X, Y, Red Pixel value less than, obstacle, value to set obstacle to if found
   */
   Matrix findCurrentObstacles = { 
       {852, 735, 40, 0, 1},
       {960, 712, 40, 1, 1},
       {1030, 726, 40, 2, 1} 
   }; 
    
    processImage(img, findCurrentObstacles, obstacles);

  /*
  * This was the code as it was shown in the video
    As to easily describe the logic
    The above code does the smae but is cleaner
    (not faster though!)

   QColor pix = img->pixelColor(852, 735);
   if (pix.red() <40) obstacles[0] = 1; 

   pix = img->pixelColor(960,712);
   if (pix.red() <40) obstacles[1] = 1; 

   pix = img->pixelColor(1030,726);
   if (pix.red() <40) obstacles[2] = 1; 
   */
}

/*
* Moves the car into the lane where there is no obstacle currently
* It returns the number of positions to move from current position
* Where a negative number means the number of positions to move the left
* Where a positive number means the number of positions to move to the right 
*/
int movesToSafePlace(int pos, int *obstacles){
    //when beginning we need to be very cary to merge into traffic
    if (pos == 4) if (obstacles[2] == 0) return -1; 

    //We have a bias for the fast lane :P
    if (obstacles[0] == 0) return 0 - pos; 

    if (obstacles[1] == 0) return 1 - pos;

    if (obstacles[2] == 0) return 2 - pos ;

    return 0;
}

/*
* avoidOnbstacles is a convenienve wrapper function that first gets our own current position
* and passes it on to movesToSafePlace.
*
* Since we always need to move our car in relation to our current position, a wrapper function makes sense.
*/
int avoidObstacles(int *myPosition, int *obstacles){
   int pos = 0;
   
   //find the car's curremt position on screen
   while(!myPosition[pos]) { pos++; }

   //we have nothing to fear in start position or the hitchhiker position   
  return movesToSafePlace(pos, obstacles);

}

/*
* Send a keystroke to the X11 environment
*/
void sendKeyCode(Display* X11Handle, int key){
    unsigned int keycode = XKeysymToKeycode(X11Handle, key);
    XTestFakeKeyEvent(X11Handle, keycode, True, 0);
    XTestFakeKeyEvent(X11Handle, keycode, False, 0);
    XFlush(X11Handle);
}

/*
* Game logic will create a handle to X11 (to send keystrokes to play the game)
* Turn the Pixmap into a greyscale QImage
* Then determines where the obstacles are on the road
* And the determines how to avoid those obstacles
* And sends these movement instructions in the form of keypresses to X11
*/
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