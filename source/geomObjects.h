//
//  geomObjects.h
//  cse3431Template
//
//  Created by Petros Faloutsos on 9/23/12.
//  Copyright (c) 2012 Petros Faloutsos. All rights reserved.
//

#ifndef cse3431Template_geomObjects_h
#define cse3431Template_geomObjects_h

#include "glm/glm.hpp"
using glm::vec4;
using glm::vec3 ;
using glm::vec2 ;

typedef struct VertexData {
    vec4 position ;
    vec4 colour ;
    vec3 normal ;
    vec2 texCoord ;
} Vertexdata;

class Object {
public:
    Object() {_name = "" ;} ;
    Object(std::string name) { _name = name ;} ;
	~Object() { glDeleteBuffers(1, &_vbo); glDeleteBuffers(1, &_vbo2); glDeleteBuffers(1, &_vao);};
    virtual void Draw(void) {return;}  ;
    std::string GetName() { return _name;} ;
    void getAttributeLocations(GLint prog, GLint &pa,
                               GLint &na, GLint &ca,
                               GLint &ta);
    void SetBuffers(GLuint prog) ;
    virtual void SetData(GLuint prog){ exit(-1) ; std::cerr << "Object not child\n" ;};
    void binVAO(void) { glBindVertexArray(_vao) ;} ;
    
protected:
    std::string _name ;
	GLuint _vbo ;
	GLuint _vbo2 ;		// for element arrays
    GLuint _vao ;

	// for separate arrays
	GLuint _vaoHandle;
	GLuint _vertexBuffer;
	GLuint _colorBuffer;
	GLuint _normalBuffer;
	GLuint _textureBuffer;
   
};

class Triangle : public Object {
public:
	Triangle() { return; };
	Triangle(std::string name, GLuint prog);
	void Draw(void);
	void SetData(GLuint prog);
};


class Sphere : public Object {
public:
    Sphere() { _n = 0 ; return ;}
    Sphere(std::string name, int nSub, GLuint prog);
    void Draw(void) ;
    
    void SetData(GLuint prog) ;
    int getN(void) {return _n;} ;
private:
    int _n ; // number of subdivision in each paramater
};

class Square : public Object {
public:
    Square() { return ;} ;
    Square(std::string name, GLuint prog) ;
    void Draw(void) ;
    void SetData(GLuint prog) ;
} ;

class Pyramid : public Object {
public:
	Pyramid() { return; };
	Pyramid(std::string name, GLuint prog);
	void Draw(void);
	void SetData(GLuint prog);
};

class Cube : public Object {
public:
    Cube() { return ;} ;
    Cube(std::string name, GLuint prog) ;
    void Draw(void) ;
    void SetData(GLuint prog) ;
} ;

// From COOKBOOK
class SkyBox : public Object {
public:
	SkyBox();
	void Draw(void);
};


class Cylinder : public Object {
public:
    Cylinder() { _n = 0 ;return ;};
    Cylinder(std::string name, int nSub, GLuint prog) ;
    void Draw(void) ;
    void SetData(GLuint prog) ;
    int getN(void) {return _n;} ;
private:
    int _n ; // number of subdivision in each paramater
};

class Cone : public Object {
public:
    Cone() { _n = 0 ;return ;};
    Cone(std::string name, int nSub, GLuint prog) ;
    void Draw(void) ;
    void SetData(GLuint prog) ;
    int getN(void) {return _n;} ;
private:
    int _n ; // number of subdivision in each paramater
};

    
class BezierPatch3 : public Object {
public:
    BezierPatch3() {_n = 0 ; return ;}
    BezierPatch3(std::string name, int nSub, GLfloat p[4][4][3], GLuint prog) ;
    void Draw(void) ;
    void SetData(GLuint prog) ;
    int getN(void) {return _n;} ;
    void GetVertex(VertexData &vd, float u, float v) ;
    void SetControlPoints(GLfloat p[4][4][3]) ;

private:
    int _n ;
    glm::vec3 _cpoints[4][4] ;
};



#endif
