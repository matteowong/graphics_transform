#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 move: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");

  struct matrix * temp;
  
  while ( fgets(line, 255, f) != NULL ) {
    if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
    printf(":%s:\n",line);


    if (!strcmp(line,"line")) {
      fgets(line,255,f);//gets next line w/ six commands
      double x0=0;
      double y0=0;
      double z0=0;
      double x1=0;
      double y1=0;
      double z1=0;
      if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
      sscanf(line,"%lf %lf %lf %lf %lf %lf",&x0,&y0,&z0,&x1,&y1,&z1);
      add_edge(edges,x0,y0,z0,x1,y1,z1);

    } else if (!strcmp(line,"ident")) {
      ident(transform);
    } else if (!strcmp(line,"scale")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
      double sx=0;
      double sy=0;
      double sz=0;
      sscanf(line,"%lf %lf %lf",&sx,&sy,&sz);
      temp = make_scale(sx,sy,sz);
      matrix_mult(temp,transform);
      free_matrix(temp);

    } else if (!strcmp(line,"move")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';      
      double tx=0;
      double ty=0;
      double tz=0;
      sscanf(line,"%lf %lf %lf",&tx,&ty,&tz);
      temp=make_translate(tx,ty,tz);
      matrix_mult(temp,transform);
      free_matrix(temp);
    } else if (!strcmp(line,"rotate")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';      
      char axis[2];
      double theta;
      sscanf(line,"%s %lf",axis,&theta);
      if (axis[0]=='x') {
	temp=make_rotX(theta);
      }
      else if (axis[0]=='y') {
	temp=make_rotY(theta);
      }
      else if (axis[0]=='z') {
	temp=make_rotZ(theta);
      }
      matrix_mult(temp,transform);
      free_matrix(temp);
    } else if (!strcmp(line,"apply")) {
      matrix_mult(transform,edges);
    } else if (!strcmp(line,"display")) {
      clear_screen(s);
      color c;
      c.green=255;
      c.red=0;
      c.blue=0;
      draw_lines(edges,s,c);
      display(s);
    } else if (!strcmp(line,"save")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';      
      clear_screen(s);
      color c;
      c.green=255;
      c.red=0;
      c.blue=0;
      draw_lines(edges,s,c);
      save_extension(s,line);
    } else if (!strcmp(line,"quit")) {

      exit(0);
    }

  }
}
  


