//
//  process_obj.hpp
//  TeaPot
//
//  Created by Pujan Dave on 3/19/16.
//  Copyright © 2016 Pujan Dave. All rights reserved.
//

#ifndef process_obj_hpp
#define process_obj_hpp

#include "GLUT/glut.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


typedef struct vert {
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat z = 0.0;
    GLfloat w = 1.0;
}vert;

void process_file (const char* fname, vector<vert> &vertices, vector<vert> &normals, vector<GLuint> &faces, vector<GLfloat> &T_s, vector<GLfloat> &T_t);

#endif /* process_obj_hpp */
