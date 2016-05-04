//
//  main.cpp
//  TeaPot
//
//  Created by Pujan Dave on 3/19/16.
//  Copyright © 2016 Pujan Dave. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "process_obj.hpp"

vector<vert> vertices, normals;
vector<GLfloat> T_s, T_t;
vector<GLuint> faces;
const char* fname = "/Users/pujandave/cs418/MP3/TeaPot/TeaPot/teapot_0.obj";

/* Need to Provide correct path */
// Image for texture
const char* tex_img1 = "/Users/pujandave/cs418/MP3/TeaPot/TeaPot/flower.ppm";
// Image for Environment
const char* tex_img2 = "/Users/pujandave/cs418/MP3/TeaPot/TeaPot/stpeters.ppm";

int nFPS = 30;
clock_t startClock=0,curClock;
long long int prevF=0,curF=0;
int fRotateAngle = 0;

int e = 1;

// For texture coordinates
GLchar teximage1[256][256][3];
GLchar teximage2[256][256][3];

void init_texture(){
    /* First, read in the image file */
    FILE * fp1;
    
    int i,j,k;
    
    fp1 = fopen(tex_img1,"rb");
    fscanf(fp1,"%*s %*d %*d %*d%*c");
    for (i = 0 ; i < 256 ; i++)
        for (j = 0 ; j < 256 ; j++)
            for (k = 0 ; k < 3 ; k++){ // RGB
               fscanf(fp1,"%c",&(teximage1[i][j][k]));
            }
    fclose(fp1);
    
    /* Environment texture */
    fp1 = fopen(tex_img2,"rb");
    fscanf(fp1,"%*s %*d %*d %*d%*c");
    for (i = 0 ; i < 256 ; i++)
        for (j = 0 ; j < 256 ; j++)
            for (k = 0 ; k < 3 ; k++){ // RGB
                fscanf(fp1,"%c",&(teximage2[i][j][k]));
            }
    fclose(fp1);
    
}

void init(void)
{
    // init your data, setup OpenGL environment here
    
    //Texture
    GLuint tex[2];
    init_texture();  // Initialize the texture image buufer
    glGenTextures(2, tex);
    
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, teximage1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, teximage2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    
    // Activate first texture
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Activate Second texture
    glActiveTexture(GL_TEXTURE1);
//    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
    
    
    
//    glClearColor(0.9,0.9,0.9,1.0); // clear color is gray
    glClearColor(0.0,0.0,0.0,1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
    // GL_POINT / GL_LINE / GL_FILL (default)
    glPointSize(4.0);
    
    GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
    
    GLfloat mat_diffuse[] = { 0.8, 0.6, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 0.4, 1.0 };

//    GLfloat mat_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
//    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat mat_shininess[] = {80.0};
    
    GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {1.0, 0.0, 1.0, 0.0};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    //FLip light0 switch on
    glEnable(GL_LIGHT0);
    
    //Turn on Power
    glEnable(GL_LIGHTING);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Z-buffer

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
    // GL_POINT / GL_LINE / GL_FILL (default)
    glPointSize(4.0);
}


void display(void)
{
    curF++;
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // put your OpenGL display commands here
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // reset OpenGL transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // reset transformation matrix to identity
    
    // setup look at transformation so that
    // eye is at : (0,0,3)
    // look at center is at : (0,0,0)
    // up direction is +y axis
    gluLookAt(0.f,4.f,-6.f,0.f,1.f,0.f,0.f,1.f,0.f);
    
    glRotatef(fRotateAngle,0.f,1.f,0.f);
    
    // Comment the below line if you want the light to not rotate!
    //glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[]){0.0, 0.0, 1.0, 0.0});
    
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);

    if (e) {
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    //glColor3f(0.3,0.3,0.3);
    glColor3f(1.0,1.0,1.0);
    
    glBegin(GL_TRIANGLES);
    for (int i=0; i<faces.size(); i+=3) {
        vert v1 = vertices[faces[i]];
        vert v2 = vertices[faces[i+1]];
        vert v3 = vertices[faces[i+2]];
        
        vert n1,n2,n3;
        n1 = normals[faces[i]];
        n2 = normals[faces[i+1]];
        n3 = normals[faces[i+2]];
        
        glNormal3f(n1.x, n1.y, n1.z);
        glMultiTexCoord2f(GL_TEXTURE0, T_s[faces[i]], T_t[faces[i]]);
        glVertex3f(v1.x, v1.y, v1.z);
        
        glNormal3f(n2.x, n2.y, n2.z);
        glMultiTexCoord2f(GL_TEXTURE0, T_s[faces[i+1]], T_t[faces[i+1]]);
        glVertex3f(v2.x, v2.y, v2.z);
        
        glNormal3f(n3.x, n3.y, n3.z);
        glMultiTexCoord2f(GL_TEXTURE0, T_s[faces[i+2]], T_t[faces[i+2]]);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
    
    if (e==0) {
       glDisable(GL_TEXTURE_GEN_S);
       glDisable(GL_TEXTURE_GEN_T);
    }
    glDisable(GL_TEXTURE_2D);
    
    glFlush();
    glutSwapBuffers();	// swap front/back framebuffer to avoid flickering
    
    
    
}


void reshape (int w, int h)
{
    // reset viewport ( drawing screen ) size
    glViewport(0, 0, w, h);
    float fAspect = ((float)w)/h;
    // reset OpenGL projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Perspective Distortion
    gluPerspective(70.f,fAspect,0.001f,30.f);
}


//KeyBoard controls
void keyboard(unsigned char key, int x, int y)
{
    
    if (key == 27)
    {
        // ESC hit, so quit
        printf("demonstration finished.\n");
        exit(0);
    }
    
    if (key == 'e'){
        e = 1 - e;
    }
    
}

void timer(int v)
{
    fRotateAngle = fRotateAngle%360 + 1;
    glutPostRedisplay(); // trigger display function by sending redraw into message queue
    glutTimerFunc(v,timer,v); // restart timer again
}


int main(int argc, char* argv[])
{
    // Initialize sines and cosines for integral angles
    
    glutInit(&argc, (char**)argv);
    
    // set up for double-buffering & RGB color buffer & depth test
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1920, 1080);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ((const char*)"TeaPot");
    
    // Get the data ready
    process_file(fname, vertices, normals, faces, T_s, T_t);
    
    init(); // setting up user data & OpenGL environment
    
    // set up the call-back functions
    glutDisplayFunc(display);  // called when drawing
    glutReshapeFunc(reshape);  // called when change window size
    
    glutKeyboardFunc(keyboard); // called when received keyboard interaction
    
    glutTimerFunc(nFPS,timer,nFPS); // a periodic timer. Usually used for updating animation
    
    startClock=clock();
    
    glutMainLoop(); // start the main message-callback loop
    
    return 0;
}
