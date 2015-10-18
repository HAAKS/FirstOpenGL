//  Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <GLUT/glut.h>
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
//-----------------

//  Global Variables
int selectedBar = 0; // used to determine which bar has the mouse currently over it
int bar1Y = 0;       // holds the Y translation value of bar 1
int bar2Y = 0;       // holds the Y translation value of bar 2
int bar3Y = 0;       // holds the Y translation value of bar 3
int ballY = 0;       // holds the value of the Y coordinate of the ball
//-----------------

int main(int argc, char** argr) {
    glutInit(&argc, argr);
    
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(150, 150);
    
    glutCreateWindow("Robotic Arm");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
    glutKeyboardUpFunc(KeyUp);  // sets the KeyboardUp handler function; called when a key is released
    glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
    glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);
    
    glutMainLoop();
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric *quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

// Keyboard handler function
//   key: the key pressed on the keyboard
//   x  : the X coordinate of the mouse cursor at the moment of pressing the key
//   y  : the Y coordinate of the mouse cursor at the moment of pressing the key
void Key(unsigned char key, int x, int y) {
    // pick a bar based on the X position of the mouse, assuming the window is divided into 3 vertical slices
    if (x < 100)
        selectedBar = 1;
    else if (x < 250)
        selectedBar = 2;
    else
        selectedBar = 3;
    
    // check the selected bar, to decide which variable to control: bar1Y, bar2Y or bar3Y
    switch (selectedBar) {
        case 1:
            // if the key 'a' is pressed, increment bar1Y until it reaches the top limit of its support
            if (key == 'a' && bar1Y < 270)
                bar1Y++;
            // if the key 'z' is pressed, decrement bar1Y until it reaches the bottom limit of its support
            if (key == 'z' && bar1Y > -270)
                bar1Y--;
            break;
        case 2:
            // if the key 'a' is pressed, increment bar2Y until it reaches the top limit of its support
            if (key == 'a' && bar2Y < 90)
                bar2Y++;
            // if the key 'z' is pressed, decrement bar2Y until it reaches the bottom limit of its support
            if (key == 'z' && bar2Y > -90)
                bar2Y--;
            break;
        case 3:
            // if the key 'a' is pressed, increment bar3Y until it reaches the top limit of its support
            if (key == 'a' && bar3Y < 20)
                bar3Y++;
            // if the key 'z' is pressed, decrement bar3Y until it reaches the bottom limit of its support
            if (key == 'z' && bar3Y > -20)
                bar3Y--;
            break;
    }
    
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}

// KeyboardUp handler function
//   similar to the Keyboard handler function except that it is called only when the key is released
void KeyUp(unsigned char key, int x, int y) {
    // reset the selectedBar value to 0 to unselect the selected bar
    selectedBar = 0;
    
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}

// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button
void Mouse(int button, int state, int x, int y) {
    // calculate the Y coordinate of the tip of the arm
    int barTipY = bar1Y + bar2Y + bar3Y + 400;
    
    // if the tip of the bar is inside the ball's circumference
    if (barTipY <= ballY + 15 && barTipY >= ballY - 15)
        // color the background green
        glClearColor(0, 1, 0, 0);
    else
        // otherwise, color it red
        glClearColor(1, 0, 0, 0);
    
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}

void Timer(int value) {
    // set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)
    ballY = rand() % 780 + 10;
    
    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
    
    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(20 * 1000, Timer, 0);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    // if the mouse is over the 1st bar, color it yellow, otherwise black
    if (selectedBar == 1)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    // draw the 1st vertical rectangle, which is fixed and won't move
    drawRect(10, 120, 20, 560); // rectangle 1
    
    /**
     * note:
     *   read the next comments from down to up, since we are dealing with stacks to handle transformations (LIFO).
     *   the comments are numbered to help you read them in the correct order
     *   3 nested stacks are used to handle the 3 parts of the arm, which look like that:
     *
     *
     *    Draw Rect 1
     *
     *    Stack 3 Push <-------------------.
     *      Translate Bar 1                |
     *      Draw Rect 2                    |
     *                                     |
     *      Stack 2 Push <-------------.   |
     *        Translate Bar 2          |   |
     *        Draw Rect 3 and 4        |   |
     *                                 |   |
     *        Stack 1 Push <-------.   |   |
     *          Translate Bar 3    |   |   |
     *          Draw Rect 5 and 6  |   |   |
     *                             |   |   |
     *        Stack 1 Pop  <------'    |   |
     *      Stack 2 Pop  <------------'    |
     *    Stack 3 Pop  <------------------'
     *
     * order of execution:
     *  1. Draw Rect 5 and 6
     *  2. Translate Bar 3
     *  3. Draw Rect 3 and 4
     *  4. Translate Bar 2
     *  5. Draw Rect 2
     *  6. Translate Bar 1
     */
    
    //===== 3. create a new stack to control the 1st part of the arm (rectangles 2 and 3)
    //         since this is a stack, rectangles 4, 5 and 6 will be already drawn and will translate with rectangles 2 and 3
    glPushMatrix();
    glTranslatef(0, bar1Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(30, 390, 150, 20); // rectangle 2
    
    // if the mouse is over the 2nd bar, color it yellow, otherwise black
    if (selectedBar == 2)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    drawRect(180, 300, 20, 200); // rectangle 3
    
    //===== 2. create a new stack to control the 2nd part of the arm (rectangles 4 and 5)
    //         since this is a stack, rectangle 6 will be already drawn and will translate with rectangles 4 and 5
    glPushMatrix();
    glTranslatef(0, bar2Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(200, 390, 100, 20); // rectangle 4
    
    // if the mouse is over the 3rd bar, color it yellow, otherwise black
    if (selectedBar == 3)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    drawRect(300, 370, 20, 60); // rectangle 5
    
    //===== 1. create a new stack to control the 3rd part of the arm (rectangle 6)
    glPushMatrix();
    glTranslatef(0, bar3Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(320, 390, 60, 20); // rectangle 6
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
    
    // draw the ball; its Y coordinate is controlled by `ballY`
    glColor3f(0, 0, 1);
    drawCircle(380, ballY, 15);
    
    glFlush();
}
