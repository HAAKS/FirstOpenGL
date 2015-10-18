#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
float pointone = 0;
float ydir =0;
float amplitude =98;
float ycircle;
float pushy;
int count=0;
float jumpy =10;
float ymin =0 ;
float ymax =3.0;
int counttime =0;
std::string result;
GLboolean turn ; //for horizontal
GLboolean jclicked=false; //to stop jumping
GLboolean doublejump = false;
GLboolean pressed = false; //not to jump while jumping
GLboolean notonground = false;
GLboolean havewon= false;
GLboolean jumping= false;


void glutBitmapCharacter(void *font, int character);
void drawtext(const char *text, int length, int x, int y);
void Display();
void DrawWall();
void Anim();
void Keyboard(unsigned char key, int x, int y);
void Counter(std::string string, int x, int y);
void EndGame(std::string string);
void Timer(int value);
int main(int argc, char** argr) {
    glutInit(&argc, argr);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Kbeer El Haramiya");
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(0, Timer, 0);
    glClearColor(0.89f, 0.89f, 0.89f, 0.89f);
    glPointSize(20.0);
    gluOrtho2D(0.0, 1000.0, 0.0, 600.0);
    glutMainLoop();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    DrawWall();
    glPushMatrix();
    glTranslatef(pointone, pushy, 0);
    glBegin(GL_POLYGON);
    glColor3f(0.372f,0.713f,0.780f);
    glVertex2i(0.0f, 0.0f);
    glVertex2i(50.0f, 0.0f);
    glVertex2i(50.0f, 50.0f);
    glVertex2i(0.0f, 50.0f);
    glEnd();
    glPopMatrix();
    Counter(result,90,550);
    EndGame("You are out !");
    glFlush();
    }


void DrawWall(){
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2i(999, 0);
    glVertex2i(999,600);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2i(900, 600);
    glVertex2i(900, 0);
    glEnd();
    //finish line
    glBegin(GL_POLYGON);
    glColor3b(1.0f, 1.0f, 1.0f);
    glVertex2i(0, 410);
    glVertex2i(20,410);
    glVertex2i(20, 275);
    glVertex2i(0,275);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2i(0, 210);
    glVertex2i(760,210);
    glVertex2i(760, 175);
    glVertex2i(0,175);
    glEnd();
}

void Keyboard(unsigned char key, int x, int y){
    if(key == 'j' && !pressed)  {
        ydir=ymin;
        pressed = true;
        jclicked =true;
    }
    
    if(pointone >=820 && key == 'j'){
        count++;
        if(count>=1){
        doublejump = true;
            jclicked =true;
            count=0;
        }
        
    }
    
}
void Anim(){
if((notonground && pointone <=21 && pushy>265) || havewon){
       havewon = true;
   }

    if(notonground && pointone>=743){
         if(pushy<=0){
            notonground = false;
             jumping = false;
            std::cout << "jumping 0 /n";
        }
            else if(jumping){
                ycircle =pushy;
                ycircle-=3.5;
                if(ycircle>0){
                    pushy=ycircle;
                    std::cout << "JJjumping: " << pushy;}
                else if(ycircle<0){
                    pushy=0;
                }}
            
         else if(!jumping){
             pushy-=5;
             std::cout << "NOTjumping: /n " << pushy;
         }

       
        ymin=0;
        ymax = 3.0;
        amplitude =98;
    }
      //parabolic for hitting the wall
    if(doublejump){
        if(jumpy<=16 && jclicked){
            pushy = jumpy*jumpy;
            jumpy+=0.2;

    }
        else{
            if(jumpy<=14.6){
                count=0;
                pushy = 180;
                doublejump = false;
                pressed = false;
                ymin = 0.79;
                ymax = 2.45;
                jumpy=10;
                ydir=1;
                amplitude = 290;

            }
        else{
            jumpy-=0.2;
            pushy = jumpy*jumpy;
            jclicked = false;


        }
           
        }
    }
    else{
        
    if(pointone <=100 && pushy>150){
            notonground = true;
        
        }
    if(ydir>=ymin && ydir<=ymax && jclicked){
               pushy=amplitude*sin(ydir);
                ydir+=0.050;
    
    }
       

    else if(ydir>ymax && pressed &&!jumping){
        if (pointone>740 && notonground){
            jumping = true;
            std::cout << "GGGjumping: " << pushy;
        }
        pressed = false;
        if(ymin ==0){
            ydir=0;}
               pushy=amplitude*sin(ydir);
        jclicked=false;
        std::cout << "HHHjumping: " << pushy;
    }
    }
    
    // horizontal movement
    
    if(pointone<=850 && turn ==true){
        pointone+=3;
        turn=true;
    }
    else if (pointone==0){
        turn=true;
    }
    else {
        turn = false;
        pointone-=3;
    }
    glutPostRedisplay();
}
void Counter(std::string string, int x, int y) {
     glRasterPos2d(x, y);
    std::string text = "Score: ";
    int tol =text.length();
    int len = string.length();
    int i;
    for(i=0;i<tol;i++){
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    for(i =0;i<len;i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
    glutPostRedisplay();
}

void EndGame(std::string string){
    if(havewon){
    glRasterPos2i(290,450);
    int len = string.length();
    int i;
    for(i =0;i<len;i++){
        glColor3b(1.0f, 0.9f,0.0f);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
    }
}

void Timer(int value){
    if(!havewon){
    counttime++;
    std::string string = std::to_string(counttime);
    result = string;
        
    glutTimerFunc(10*100, Timer, 0);
    glutPostRedisplay();

    }
}