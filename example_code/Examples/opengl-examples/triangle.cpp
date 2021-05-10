// $Id: triangle.cpp,v 1.11 2020-08-22 18:49:37-07 - - $

#include <GL/freeglut.h>
#include <libgen.h>

bool flipflop = true;

void draw_rgb_triangle() {
   glBegin(GL_TRIANGLES);
   glColor3ub (0xFF, 0x00, 0x00);
   glVertex2f (0, 1);
   glColor3ub (0x00, 0xFF, 0x00);
   glVertex2f (-1, -1);
   glColor3ub (0x00, 0x00, 0xFF);
   glVertex2f (1, -1);
   glEnd();
}

void draw_cmy_triangle() {
   glBegin(GL_TRIANGLES);
   glColor3ub (0x00, 0xFF, 0xFF);
   glVertex2f (0, -1);
   glColor3ub (0xFF, 0x00, 0xFF);
   glVertex2f (-1, 1);
   glColor3ub (0xFF, 0xFF, 0x00);
   glVertex2f (1, 1);
   glEnd();
}

void display() {
   glClearColor (0.5, 0.5, 0.5, 1.0);
   glClear (GL_COLOR_BUFFER_BIT);
   if (flipflop) draw_rgb_triangle();
            else draw_cmy_triangle();
   glFlush();
}

void mouse (int, int state, int, int) {
   if (state == GLUT_DOWN) flipflop =  not flipflop;
   glutPostRedisplay();
}

int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitWindowSize (640, 480);
   glutCreateWindow (basename (argv[0]));
   glutDisplayFunc (display);
   glutMouseFunc (mouse);
   glutMainLoop();
   return 0;
}

//TEST// mkpspdf triangle.ps triangle.cpp*

