#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<GL/glut.h>
#include <GL/gl.h>
#include<windows.h>

#define SPEED 30.0
#define CAR_SPEED 60.0
using namespace std;

int l, score=0;
char str[100];
float cloud=-100.0, car=0.0, dinosaur=0.0;
bool isForward = true;
bool isMouseClicked = false;
bool isGameStarted = true;
bool isCrushed = false;

void draw_text(int x, int y, float r, float g, float b, void * font, string message){
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = message.size();
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, message[i]);
    }
    glutSwapBuffers();
}

void draw_pixel(GLint cx, GLint cy){
    glBegin(GL_POINTS);
	glVertex2i(cx,cy);
	glEnd();
}

void plotpixels(GLint h,GLint k, GLint x,GLint y){
	draw_pixel(x+h,y+k);
	draw_pixel(-x+h,y+k);
	draw_pixel(x+h,-y+k);
	draw_pixel(-x+h,-y+k);
	draw_pixel(y+h,x+k);
	draw_pixel(-y+h,x+k);
	draw_pixel(y+h,-x+k);
	draw_pixel(-y+h,-x+k);
}

void help_text(){
    draw_text(50.0, 80.0, 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15, ">> JUMP(MOUSE LEFT CLICK) OR PRESS 'J'");
}

void draw_circle(GLint h, GLint k, GLint r){
	GLint d=1-r, x=0, y=r;
	while(y>x){
		plotpixels(h,k,x,y);
		if(d<0) d+=2*x+3;
		else{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plotpixels(h,k,x,y);
}

void draw_car(){
    //car-shade
    glColor3f(0.06, 0.15, 0.40);
	glBegin(GL_POLYGON);
	glVertex2f(1020-car,195);
	glVertex2f(1010-car,165);
    glVertex2f(1080-car,165);
    glVertex2f(1080-car,195);
    glEnd();
    //car-bottom
    glColor3f(0.24, 0.46, 1.00);
	glBegin(GL_POLYGON);
	glVertex2f(1010-car,165);
	glVertex2f(1010-car,150);
    glVertex2f(1080-car,150);
    glVertex2f(1080-car,165);
    glEnd();
    //car-wheel
    for(l=0;l<=10;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(1030-car,150,l);
	}
	for(l=0;l<=10;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(1060-car,150,l);
	}
}

void drawing_tree(){
    //Tree
    glColor3f(0.9,0.2,0.0);
    glBegin(GL_POLYGON);
    glVertex2f(900,140);
    glVertex2f(900,285);
    glVertex2f(940,285);
    glVertex2f(940,140);
    glEnd();
    for(l=0;l<=40;l++){
		glColor3f(0.0,0.5,0.0);
		//middle-circle
		draw_circle(965,340,l);
		draw_circle(875,340,l);
        //bottom-circle
		draw_circle(850,280,l);
		draw_circle(990,280,l);
		//bottom-middle-circle
		draw_circle(900,280,l);
		draw_circle(940,280,l);
	}
	for(l=0;l<=55;l++){
		glColor3f(0.0,0.5,0.0);
		draw_circle(915,360,l);
	}
}

void draw_cloud(){
    //Cloud1
    for(l=0;l<=20;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(160+cloud,625,l);
	}
	for(l=0;l<=35;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(200+cloud,625,l);
		draw_circle(225+cloud,625,l);
	}
	for(l=0;l<=20;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(265+cloud,625,l);
	}
	//Cloud2
    for(l=0;l<=20;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(370+cloud,615,l);
    }
	for(l=0;l<=35;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(410+cloud,615,l);
		draw_circle(435+cloud,615,l);
		draw_circle(470+cloud,615,l);
	}
    for(l=0;l<=20;l++){
		glColor3f(1.0,1.0,1.0);
		draw_circle(500+cloud,615,l);
    }
}

void draw_dinosaur(){
    //Mouth
    glColor3f(0.06, 0.15, 0.40);
	glBegin(GL_TRIANGLES);
    glVertex2f(280, 250+dinosaur);
    glVertex2f(300, 230+dinosaur);
    glVertex2f(320, 280+dinosaur);
    glEnd();

    //gola
    glColor3f(0.06, 0.15, 0.40);
	glBegin(GL_POLYGON);
	glVertex2f(280,250+dinosaur);
	glVertex2f(280,180+dinosaur);
    glVertex2f(300,180+dinosaur);
    glVertex2f(300,250+dinosaur);
    glEnd();

    //bottom belly
    glColor3f(1.00, 0.44, 0.32);
	glBegin(GL_POLYGON);
	glVertex2f(160,160+dinosaur);
	glVertex2f(300,230+dinosaur);
    glVertex2f(300,230+dinosaur);
    glVertex2f(260,160+dinosaur);
    glEnd();

    //top belly
    glColor3f(1.00, 0.32, 0.32);
	glBegin(GL_POLYGON);
	glVertex2f(300,160+dinosaur);
	glVertex2f(160,230+dinosaur);
    glVertex2f(300,230+dinosaur);
    glVertex2f(260,160+dinosaur);
    glEnd();

    //leg-right
    glColor3f(0.06, 0.15, 0.40);
	glBegin(GL_POLYGON);
	glVertex2f(265,160+dinosaur);
	glVertex2f(265,140+dinosaur);
    glVertex2f(270,140+dinosaur);
    glVertex2f(280,160+dinosaur);
    glEnd();

    //leg-left
    glColor3f(0.06, 0.15, 0.40);
	glBegin(GL_POLYGON);
	glVertex2f(235,160+dinosaur);
	glVertex2f(235,140+dinosaur);
    glVertex2f(240,140+dinosaur);
    glVertex2f(250,160+dinosaur);
    glEnd();
}

void draw_object(){
    //Skay
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_POLYGON);
    glVertex2f(0,140);
    glVertex2f(0,700);
    glVertex2f(1100,700);
    glVertex2f(1100,140);
    glEnd();

    //Road --------------
	glColor3f(0.40, 0.16, 0.06);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(0,140);
    glVertex2f(1100,140);
    glVertex2f(1100,0);
    glEnd();

    //Drawing tree
    drawing_tree();

    //Drawing cloud
    draw_cloud();

    //Drawing car
    draw_car();

    //Drawing dinosaur
    draw_dinosaur();


}


void click_event(){
    isMouseClicked = true;
    if(dinosaur <= 60 && (car >= 710 && car <= 850)){
        isCrushed = true;
        draw_text(50.0, 100.0, 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15, ">>CRUSHED: PRESS 'J' TO RESET.");
        score = 0;
    }else{
        score+=1;
    }
}

void mouse(int btn,int state,int x,int y){
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP){
        click_event();
    }
}

void my_keyboard(unsigned char key, int x, int y){
    if(key == 'j' || key == 'J'){
        click_event();
    }
}

void idle(){

    if(!isCrushed){
        //Cloud-animation
        if(isForward){
            cloud+=SPEED/150;
            if(cloud > 500)
                isForward = false;
        }else if(!isForward){
            cloud-=SPEED/150;
            if(cloud < -100)
                isForward = true;
        }
        //Car-animation
        if(car < 1100){
            car+=CAR_SPEED/20;
        }else{
            car = 0;
        }
        //Game play
        if(isMouseClicked){
            dinosaur+=40/12;
            if(dinosaur>200){
                dinosaur=0;
                isMouseClicked = false;
            }
        }
        if(!isMouseClicked && isGameStarted){
            if(dinosaur <= 60 && (car >= 710 && car <= 850)){
                isGameStarted = false;
                isCrushed = true;
                score = 0;
            }
        }
    }else{

        if(isMouseClicked){
            score=0;cloud=-100.0; car=0.0; dinosaur=0.0;
            isForward = true;
            isMouseClicked = false;
            isGameStarted = true;
            isCrushed = false;
        }
    }

    if(!isGameStarted){
        draw_text(50.0, 100.0, 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15, ">>CRUSHED: PRESS 'J' TO RESET.");
    }else{
        string str_score = "SCORE: " + to_string(score);
        draw_text(50.0, 100.0, 1.0, 1.0, 1.0, GLUT_BITMAP_HELVETICA_18, str_score);
    }
    glutPostRedisplay();
}

void myinit(){
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(0.0,0.0,1.0);
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1100.0,0.0,700.0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    draw_object();
    glFlush();
    help_text();
}

int main(int argc,char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1100.0,700.0);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Dinosaur Game");
	glutDisplayFunc(display);
    glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutKeyboardFunc(my_keyboard);
	myinit();
	glutMainLoop();
	return 0;
}
