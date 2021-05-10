// $Id: glclock.cpp,v 1.78 2020-08-03 14:37:13-07 - - $

// Show a real-time analog clock.

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include <libgen.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static const GLubyte FOREGROUND[] = {0x00, 0xFF, 0x00};
static constexpr float RADIUS = 0.9;

struct {
   string program_name;
   int width = 256;
   int height = 256;
   bool minutes_only = false;
   bool smooth_ticks = false;
   bool fractional_seconds = false;
   unsigned int frequency_msec() {
      return smooth_ticks ? 50 : 500;
   }
} window;

struct calendar {
   time_t clock;
   struct tm localtime;
   struct timeval timevalue;
   double seconds;
   char sdate[64];
   char stime[64];
   void set() {
      clock = time (nullptr);
      localtime_r (&clock, &localtime);
      strftime (sdate, sizeof sdate, "%a %b %e", &localtime);
      if (window.fractional_seconds) {
         strftime (stime, sizeof stime, "%T.", &localtime);
         char frac[16];
         snprintf (frac, sizeof frac, "%06zd", timevalue.tv_usec);
         frac[1] = '\0';
         strncat (stime, frac, sizeof stime);
      }else {
         const char* timefmt = window.minutes_only
                             ?  "%I:%M %p" : "%I:%M:%S %p";
         strftime (stime, sizeof stime, timefmt, &localtime);
         if (stime[0] == '0') stime[0] = ' ';
         for (char* chr = stime; *chr != '\0'; ++chr) {
            if (isupper(*chr)) *chr = tolower (*chr);
         }
      }
      gettimeofday (&timevalue, nullptr);
      seconds = double (timevalue.tv_sec % 60)
              + double (timevalue.tv_usec) / 1.0e6;
   }
} calendar;

const GLubyte* ubytes (const char* cstring) {
   return reinterpret_cast<const GLubyte*> (cstring);
}

void draw_time() {
   void* font = GLUT_BITMAP_HELVETICA_18;
   float text_xmargin = 0.95;
   float text_ymargin = 0.925;
   float font_height = glutBitmapHeight (font);
   float time_ypos = text_ymargin - font_height / window.height;
   glRasterPos2f (-text_xmargin, time_ypos);
   glutBitmapString (font, ubytes (calendar.sdate));
   float time_width = glutBitmapLength (font, ubytes (calendar.stime));
   float time_xpos = text_xmargin - 2 * time_width / window.width;
   glRasterPos2f (time_xpos, time_ypos);
   glutBitmapString (font, ubytes (calendar.stime));
}

void draw_dots (int points, int count) {
   glEnable (GL_POINT_SMOOTH);
   glPointSize (points);
   glBegin(GL_POINTS);
   glColor3ubv (FOREGROUND);
   for (float theta = 0; theta < 2 * M_PI; theta += 2 * M_PI / count) {
      float xdot = 0.9 * RADIUS * cos (theta);
      float ydot = 0.9 * RADIUS * sin (theta);
      glVertex2f (xdot, ydot);
   }
   glEnd();
}

void draw_hand (GLfloat width, GLfloat length, GLfloat clock) {
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_POLYGON_SMOOTH);
   glPushMatrix();
   glRotatef (-clock * 6, 0, 0, 1);
   glColor3ubv (FOREGROUND);
   glBegin (GL_POLYGON);
   glVertex2f (-width / 2 * RADIUS, -0.05);
   glVertex2f (+width / 2 * RADIUS, -0.05);
   glVertex2f (+width / 4, length * RADIUS);
   glVertex2f (-width / 4, length * RADIUS);
   glEnd();
   glPopMatrix();
}

void display() {
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3ubv (FOREGROUND);
   draw_dots (4, 60);
   draw_dots (10, 12);
   calendar.set();
   float second = calendar.localtime.tm_sec;
   float minute = calendar.localtime.tm_min + second / 60;
   float hour = calendar.localtime.tm_hour + minute / 60;
   draw_hand (0.1, 0.6, hour * 5);
   draw_hand (0.075, 0.8, minute);
   if (not window.minutes_only) {
      if (window.smooth_ticks) draw_hand (0.05, 1.0, calendar.seconds);
                          else draw_hand (0.05, 1.0, second);
   }
   draw_time();
   glutSwapBuffers();
}

void timer (int) {
   glutTimerFunc (window.frequency_msec(), timer, 0);
   glutPostRedisplay();
}

void reshape (int width, int height) {
   window.width = width;
   window.height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (-1, +1, -1, +1);
   glMatrixMode (GL_MODELVIEW);
   glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glViewport (0, 0, window.width, window.height);
   glClearColor (0x1Fp0/255, 0x1Fp0/255, 0x1Fp0/255, 1.0);
}

void usage_exit() {
   cerr << "Usage: " << window.program_name
                     << " [-mps] [-w size]" << endl;
   cerr << "-f fractional seconds" << endl;
   cerr << "-m minutes only" << endl;
   cerr << "-s smooth second hand" << endl;
   cerr << "-w window size (pixels)" << endl;
   exit (EXIT_FAILURE);
}

void scan_options (int argc, char** argv) {
   bool invalid_opt = false;
   for (;;) {
      int opt = getopt (argc, argv, "fmsw:");
      if (opt == EOF) break;
      switch (opt) {
         case 'f': window.fractional_seconds = true;
                   window.smooth_ticks = true;
                   break;
         case 'm': window.minutes_only = true;
                   break;
         case 's': window.smooth_ticks = true;
                   break;
         case 'w': window.width = window.height = atoi (optarg);
                   break;
         default:  invalid_opt = true;
      }
   }
   if (invalid_opt) usage_exit();
   if ((window.fractional_seconds || window.smooth_ticks)
       && window.minutes_only) {
      cerr << window.program_name
           << ": -m option is incompatible with -f or -s" << endl;
      usage_exit();
   }
}

int main (int argc, char** argv) {
   window.program_name = basename (argv[0]);
   scan_options (argc, argv);
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window.width, window.height);
   glutCreateWindow (window.program_name.c_str());
   glutDisplayFunc (display);
   glutReshapeFunc (reshape);
   glutTimerFunc (window.frequency_msec(), timer, 0);
   glutMainLoop();
   return EXIT_SUCCESS;
}

//TEST// mkpspdf glclock.ps glclock.cpp*

