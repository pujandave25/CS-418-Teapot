//
//  process_obj.cpp
//  TeaPot
//
//  Created by Pujan Dave on 3/19/16.
//  Copyright © 2016 Pujan Dave. All rights reserved.
//

#include "process_obj.hpp"

GLdouble theta;
GLfloat max_y = INT64_MIN;


vert normalize_vert(vert a){
    vert n;
    GLfloat mag = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    n.x = a.x/mag;
    n.y = a.y/mag;
    n.z = a.z/mag;
    
    return n;
}

vert cross_prod(vert a, vert b, vert c) {
    vert cp;
    GLfloat mag;
    
    GLfloat x1, y1, z1, x2, y2, z2;
    x1 = a.x - b.x;
    y1 = a.y - b.y;
    z1 = a.z - b.z;
    
    x2 = a.x - c.x;
    y2 = a.y - c.y;
    z2 = a.z - c.z;
    
    cp.x = y1*z2 - z1*y2;
    cp.y = z1*x2 - x1*z2;
    cp.z = x1*y2 - y1*x2;
    
    mag = sqrt(cp.x*cp.x + cp.y*cp.y + cp.z*cp.z);
    cp.x = cp.x/mag;
    cp.y = cp.y/mag;
    cp.z = cp.z/mag;
    
    return cp;
}


void process_file (const char* fname, vector<vert> &vertices, vector<vert> &normals, vector<GLuint> &faces, vector<GLfloat> &T_s, vector<GLfloat> &T_t) {
    
    ifstream f (fname, ios::in);
    
    if (!f){
        cout << "Error loading file";
    }
    
    string line;
    vector<int> repeats;
    vert v,n;
    
    // Parse the data from the file
    while(getline(f, line)){
        
        string label = line.substr(0,2);
        
        if(label == "v "){
            istringstream s(line.substr(2));
            s >> v.x;                           // Coordinates
            s >> v.y;
            s >> v.z;
            
            if (max_y < v.y) {
                max_y = v.y;
            }
            
            vertices.push_back(v);
            theta = atan2(v.z, v.x);
            T_s.push_back((theta + M_PI)/(2*M_PI));
            T_t.push_back(v.y);
            repeats.push_back(0);
        }
        else if(label == "f ") {
            istringstream s(line.substr(2));
            GLuint v1,v2,v3;                    // Vertex indices
            s >> v1;
            s >> v2;
            s >> v3;
            faces.push_back(v1 - 1);
            repeats[v1 - 1]++;
            faces.push_back(v2 - 1);
            repeats[v2 - 1]++;
            faces.push_back(v3 - 1);
            repeats[v3 - 1]++;
        }
        else {
            // Ignore the comments!
        }
    }
    
    // Per vertex normals
    normals.resize(vertices.size(), n);
    for (int i=0; i<faces.size(); i+=3){
        GLuint a,b,c;
        a = faces[i];
        b = faces[i+1];
        c = faces[i+2];
        vert n = cross_prod(vertices[a],vertices[b],vertices[c]);
        for(int j=0; j<3; j++){
            repeats[faces[i+j]]++;
            if(repeats[faces[i+j]]==1){
                normals[faces[i+j]] = n;
            }
            
            else{
                // Avg. the normals
                normals[faces[i+j]].x = normals[faces[i+j]].x * (1.0 - 1.0/repeats[faces[i+j]]) + (n.x/repeats[faces[i+j]]);
                normals[faces[i+j]].y = normals[faces[i+j]].y * (1.0 - 1.0/repeats[faces[i+j]]) + (n.y/repeats[faces[i+j]]);
                normals[faces[i+j]].z = normals[faces[i+j]].z * (1.0 - 1.0/repeats[faces[i+j]]) + (n.z/repeats[faces[i+j]]);
                
                // Normalize
                normals[faces[i+j]] = normalize_vert(normals[faces[i+j]]);
            }
        }
    }
    
    // Divide t coordinates by max_y
    for (int i=0; i<T_t.size(); i++)
        T_t[i] = (max_y-T_t[i])/max_y;

}
