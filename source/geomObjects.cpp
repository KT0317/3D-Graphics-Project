///////////////////////////////////////////////////////////////////////////////
// 
// testing vertex array (glDrawElements, glDrawArrays)
//
//  Original AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
//  Changes and additions: Petros Faloutsos 2013
// CREATED: 2005-10-04
// UPDATED: 2012-07-11
///////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#endif

#include "opengl.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "glm/glm.hpp"

#include "image.h"

#include "program.h"

#include "geomObjects.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;

using namespace glm ;

#ifndef M_PI
#define M_PI 3.14159
#endif




//GLuint porcupineTexture ;


// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (36 * 3 = 108).
GLfloat vertices1[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
                       -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

                        1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
                        1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

                        1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
                       -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

                       -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
                       -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

                       -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
                        1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

                        1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
                       -1, 1,-1,   1, 1,-1,   1,-1,-1 };    // v6-v5-v4

// normal array
GLfloat normals1[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
                        0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

                        1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
                        1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
                       -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

                        0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
                        0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

                        0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
                        0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

// color array
GLfloat colors1[]   = { 1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v1-v2 (front)
                        1, 0, 0,   1, 0, 0,   1, 0, 0,      // v2-v3-v0

                        0, 0, 0,   0, 0, 0,   0, 0, 0,      // v0-v3-v4 (right)
                        0, 0, 0,   0, 0, 0,   0, 0, 0,      // v4-v5-v0

                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
                        0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

                        1, 1, 0,   1, 1, 0,   1, 1, 0,      // v1-v6-v7 (left)
                        1, 1, 0,   1, 1, 0,   1, 1, 0,      // v7-v2-v1

                        0, 1, 1,   0, 1, 1,   0, 1, 1,      // v7-v4-v3 (bottom)
                        0, 1, 1,   0, 1, 1,   0, 1, 1,      // v3-v2-v7

                        0, 0, 1,   0, 0, 1,   0, 0, 1,      // v4-v7-v6 (back)
                        0, 0, 1,   0, 0, 1,   0, 0, 1 };    // v6-v5-v4

GLfloat cubeTexCoord[] = { 1,1,  0,1, 0,0,      // v0-v1-v2 (front)
    0,0,   1,0,   1, 1,      // v2-v3-v0
    
    0, 1,   0,0,  1,0,      // v0-v3-v4 (right)
    1,0,   1, 1,   0,1,      // v4-v5-v0
    
    1, 0,   1, 1,  0, 1,      // v0-v5-v6 (top)
    0, 1,   0, 0,  1, 0,      // v6-v1-v0
    
    1, 1,   0, 1,  0,0,     // v1-v6-v7 (left)
    0,0,    1,0,   1, 1,      // v7-v2-v1
    
    0,1,   0,0,   1,0,      // v7-v4-v3 (bottom)
    1,0,   1,1,    0,1,  // v3-v2-v7
    
    0,0, 1,0, 1,1,     // v4-v7-v6 (back)
    1,1, 0,1, 0,0 };    // v6-v5-v4

// color array
GLfloat colors2[]   = { 
	1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
    1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0
    
    1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
    0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0
    
    1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
    0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0
    
    1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
    0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1
    
    0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
    1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7
    
    0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
    0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4


// pyramid ///////////////////////////////////////////////////////////////////////
//	   v0\
//     /|\ \
//    / | \  \
//   //v4--\--/v3
//  //      \/
// v1-------v2

// vertex coords array for glDrawArrays() =====================================
// A square-based pyramid has 5 sides and top side has 1 triangle while the square base has 2 triangles, 
// of 18 vertices (6 triangles * 3 vertices = 18 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (18 * 3 = 54).

GLfloat verticesPyramid[] = {
	0, 1, 0,  -1,-0.5,-1,  1,-0.5,-1,      // v0-v1-v2 (front face)
	1,-0.5,-1,  1,-0.5, 1,  0, 1, 0,      // v2-v3-v0 (right face)

	0, 1, 0,   1,-0.5,1, -1,-0.5, 1,      // v0-v3-v4 (back face)
	-1,-0.5, 1,  -1,-0.5,-1, 1,-0.5,-1,	  // v4-v1-v2 (bottom first half)

	1,-0.5,-1,	1,-0.5,1,	-1,-0.5, 1,	  // v2-v3-v4 (bottom second half)
	-1,-0.5, 1, -1,-0.5,-1,  0, 1, 0 };     // v4-v1-v0 (left face)

 // normal array
GLfloat normalsPyramid[] = { 
	// note to self: y = 0 is the middle of the pyramid

	0, 0, 1,   0, 0, 1,   0, 0, 1,		// v0-v1-v2 (front)
	-1, 0, 0,   -1, 0, 0,   -1, 0, 0,  // v2-v3-v0 (right)

	0, 0,-1,   0, 0,-1,   0, 0,-1,		// v0-v3-v4 (back face)
	0, -1, 0,   0, -1, 0,  0, -1, 0,     // v4-v1-v2 (bottom first half)

	0, -1, 0,   0, -1, 0,  0, -1, 0,      // v2-v3-v4 (bottom second half)
	1, 0, 0,   1, 0, 0,   1, 0, 0};      // v4-v1-v0 (left)

// color array
GLfloat colorsPyramid[] = { 
	1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v1-v2 (front)
	0, 0, 0,   0, 0, 0,   0, 0, 0,      // v2-v3-v0 (right)

	0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v3-v4 (back)
	0, 1, 1,   0, 1, 1,   0, 1, 1,      // v4-v1-v2 (bottom)

	0, 1, 1,   0, 1, 1,   0, 1, 1,      // v2-v3-v4 (bottom)
	1, 1, 0,   1, 1, 0,   1, 1, 0 };    // v4-v1-v0 (left)
    
GLfloat PyramidTexCoord[] = {
1,1,  0,1,	  0,0,      // v0-v1-v2 (front)
0,0,   1,0,   1,1,     // v2-v3-v0 (right)

0,0,   1,0,  1,1,      // v0-v3-v4 (back)
0,1,   0,0,   1,0,      // v4-v1-v2 (bottom first half)

1,0,   1,1,    0,1,		// v2-v3-v4 (bottom second half)
1, 1,   0,1,  0,0 };    // v4-v1-v0 (left)

float side = 50.0f;
float side2 = side / 2.0f;


// From Cookbook
SkyBox::SkyBox()
{
	float side = 50.0f;
	float side2 = side / 2.0f;
	float v[24 * 3] = {
		// Front
	   -side2, -side2, side2,
		side2, -side2, side2,
		side2,  side2, side2,
	   -side2,  side2, side2,
		// Right
		side2, -side2, side2,
		side2, -side2, -side2,
		side2,  side2, -side2,
		side2,  side2, side2,
		// Back
	   -side2, -side2, -side2,
	   -side2,  side2, -side2,
		side2,  side2, -side2,
		side2, -side2, -side2,
		// Left
		-side2, -side2, side2,
		-side2,  side2, side2,
		-side2,  side2, -side2,
		-side2, -side2, -side2,
		// Bottom
		-side2, -side2, side2,
		-side2, -side2, -side2,
		side2, -side2, -side2,
		side2, -side2, side2,
		// Top
		-side2, side2, side2,
		side2, side2, side2,
		side2, side2, -side2,
		-side2, side2, -side2
	};

	GLuint el[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), el, GL_STATIC_DRAW);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);

	glBindVertexArray(0);
}

void SkyBox::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}


using namespace std;




// Retrieves the locations of the folloing attributes:
// "position", "colour", "normal", "VertexTexCoord"
void Object::getAttributeLocations(GLint prog,
                                   GLint &positionAttribute,
                                   GLint &normalAttribute,
                                   GLint &colorAttribute,
                                   GLint &texCoordAttribute)
{
    // Get attribute locations
   
	positionAttribute = glGetAttribLocation(prog, "VertexPosition");
	if (positionAttribute < 0) {
		cerr << "Shader did not contain the 'VertexPosition' attribute." << endl;
	}
    
    colorAttribute = glGetAttribLocation(prog, "VertexColour");
	if (colorAttribute < 0) {
		cerr << "Object::getAttributeLocations: Shader did not contain the 'VertexColour' attribute." << endl;
	}
    
    normalAttribute = glGetAttribLocation(prog, "VertexNormal");
	if ( normalAttribute < 0) {
		cerr << "Shader did not contain the 'VertexNormal' attribute." << endl;
	}
    
    texCoordAttribute = glGetAttribLocation(prog, "VertexTexCoord");
	if (texCoordAttribute < 0) {
		cerr << "Shader did not contain the 'VertexTexCoord' attribute." << endl;
	}
    
}

void Object::SetBuffers(GLuint prog)
{
    
    GLint positionAttribute, colourAttribute, normalAttribute, texCoordAttribute;
    
    glGenVertexArrays(1, &_vao);
    checkError() ;
    glBindVertexArray(_vao);
    checkError() ;
    
    // You need to make another buffer to use both GL_ARRAY_BUFFER
	// and GL_ELEMENT_ARRAY_BUFFER
	checkError() ;
    glGenBuffers(1, &_vbo);
	checkError() ;
    
    // bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    //populate the vertex buffer
    SetData(prog) ;
    checkError() ;
    
    getAttributeLocations(prog, positionAttribute, normalAttribute, colourAttribute, texCoordAttribute) ;
	checkError() ;
    glEnableVertexAttribArray((GLuint)positionAttribute);
    glEnableVertexAttribArray((GLuint)colourAttribute);
    glEnableVertexAttribArray((GLuint)normalAttribute) ;
    glEnableVertexAttribArray((GLuint)texCoordAttribute) ;
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo) ; // bind the vdo that contains the attributes 
    glVertexAttribPointer((GLuint) positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid *)0);
    glVertexAttribPointer((GLuint) colourAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid *) sizeof(vec4));
    glVertexAttribPointer((GLuint) normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid *) (2*sizeof(vec4)));
    glVertexAttribPointer((GLuint) texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid *) (2*sizeof(vec4) + sizeof(vec3)));
    

    glBindVertexArray(0) ; // unbind the vertex array
    
}


bool flip(const VertexData &vd1,const VertexData &vd2, const VertexData &vd3){
    
    // compute average normal
    vec3 an = 1.0f/3.0f*(vd1.normal + vd2.normal + vd3.normal) ;
    // compute from triangle
    vec3 va = vd2.normal - vd1.normal ;
    vec3 vb = vd3.normal - vd1.normal ;
    vec3 tn = glm::cross(vb,va) ;
    if( dot(an,tn) < 0) return true ;
    
    return false ;
}




Triangle::Triangle(std::string name, GLuint prog)
{
	SetBuffers(prog);
	return;
}

void Triangle::Draw(void)
{
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	checkError();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkError();
	glBindVertexArray(0);
	checkError();
}

void Triangle::SetData(GLuint prog)
{
	VertexData vertexData[3] = {
		//			position					colour						   normal			texture data
		{ vec4(-0.5f, -0.5f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec3(0.0f,0.0f,1.0f), vec2(0.0f,0.0f) },
		{ vec4(0.5f, -0.5f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), vec3(0.0f,0.0f,1.0f), vec2(1.0f,0.0f) },
		{ vec4(0.0f,  0.5f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), vec3(0.0f,0.0f,1.0f), vec2(1.0f,1.0f) }
	};

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
	checkError();
}



// pyramid ///////////////////////////////////////////////////////////////////////
//	   v0\
//     /|\ \
//    / | \  \
//   //v4--\--/v3
//  //      \/
// v1-------v2

Pyramid::Pyramid(std::string name, GLuint prog) 
{
	SetBuffers(prog);
	return;
}

void Pyramid::Draw(void)
{
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(_vaoHandle);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	checkError();

	glDrawArrays(GL_TRIANGLES, 0, 18);
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (GLubyte *)NULL);
	checkError();
	glBindVertexArray(0);
	checkError();
}

void Pyramid::SetData(GLuint prog)
{
	// Separate Array
	glGenVertexArrays(1, &_vaoHandle);
	checkError();
	glBindVertexArray(_vaoHandle);
	checkError();

	// Create the buffer objects
	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_colorBuffer);
	glGenBuffers(1, &_normalBuffer);
	glGenBuffers(1, &_textureBuffer);

	glEnableVertexAttribArray(0);	// vertex position
	glEnableVertexAttribArray(1);	// vertex color
	glEnableVertexAttribArray(2);	// vertex normal
	glEnableVertexAttribArray(3);	// texture coordinates

	// populate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * 3* sizeof(float), verticesPyramid, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	checkError();

	// populate the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * 3 * sizeof(float), colorsPyramid, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	checkError();

	// populate the normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * 3 * sizeof(float), normalsPyramid, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	checkError();

	// populate the texture buffer
	glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * 2 * sizeof(float), PyramidTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	checkError();

	glBindVertexArray(0); // disable the vao

	/*
	//////////////// Element Arrays////////////////////////

	VertexData vertexData[5] = {
		//			position						colour						 normal			texture data
		// REMEMBER TO CALCULATE THE NORMALS
		{ vec4(0.0f,  1.0f, 0.0f, 1.0f),	vec4(1.0f, 0.0f, 0.0f, 1.0f), vec3(0.0f,1.0f,0.0f), vec2(1.0f,1.0f) },
		{ vec4(-1.0f, -0.5f, -1.0f, 1.0f),	vec4(1.0f, 1.0f, 0.0f, 1.0f), vec3(-1.0f,-0.5f,-1.0f), vec2(0.0f,0.0f) },
		{ vec4(1.0f, -0.5f, -1.0f, 1.0f),	vec4(0.0f, 1.0f, 0.0f, 1.0f), vec3(1.0f,-0.5f,-1.0f), vec2(1.0f,0.0f) },
		{ vec4(1.0f, -0.5f, 1.0f, 1.0f),	vec4(0.0f, 1.0f, 1.0f, 1.0f), vec3(1.0f,-0.5f,1.0f), vec2(1.0f,1.0f) },
		{ vec4(-1.0f, -0.5f, 1.0f, 1.0f),	vec4(0.0f, 0.0f, 1.0f, 1.0f), vec3(-1.0f,-0.5f,1.0f), vec2(1.0f,1.0f) }
	};


	// Add indice for ordering of vertices
	GLuint faces[18] = {
			0,1,2, 0,2,3, 0,3,4, 0,4,1,			// Triangle faces
			1,2,3, 3,4,1						// Square base
	};
	checkError();
	glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
	checkError();


	glGenBuffers(1, &_vbo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(GLuint), faces, GL_STATIC_DRAW);
	checkError();
	*/
}

Sphere::Sphere(std::string name, int n, GLuint prog)
{
    if( n < 1) return ;
    _n = n ;
    SetBuffers(prog) ;
    return ;
}

void Sphere::Draw() {
    
    glFrontFace(GL_CW) ;
    glEnable(GL_CULL_FACE) ;
    //glDisable(GL_CULL_FACE) ;
    
    glBindVertexArray(_vao);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0,_n*_n*6 );
    checkError() ;
    glBindVertexArray(0) ;  
}



void GetVertexSphere(VertexData &vd, float uu, float vv)
{
    //float v = 2*M_PI*vv - M_PI;
    //float u = 2*M_PI*uu  ;
    
    // epsilon
    //float e = 0.05; // in parametric space
    
    //float v = M_PI*(vv*(1-e) + e) ;
    float v = M_PI*vv ;
    float u = 2*M_PI* uu ;
    vd.position[0] = cos(u)*sin(v);
    vd.position[1] = sin(u)*sin(v) ;
    vd.position[2] = cos(v) ;
    vd.position[3] = 1.0 ;
    
    vd.normal[0] = vd.position[0] ;
    vd.normal[1] = vd.position[1] ;
    vd.normal[2] = vd.position[2] ;
    
    vd.colour = vec4(1.0,0.0,0.0,1.0) ;
    
    vd.texCoord[0] = uu ;
    vd.texCoord[1] = vv ;
    //std::cerr << vv << ", " << uu << std::endl ;
    
    return;
}



void Square::SetData(GLuint prog){
    
    VertexData vertexData[4] = {
        { vec4(-0.5f, -0.5f, 0.0f, 1.0f ), vec4( 1.0f, 0.0f, 0.0f, 1.0f ), vec3(0.0f,0.0f,1.0f), vec2(0.0f,0.0f) },
        { vec4( 0.5f, -0.5f, 0.0f, 1.0f ), vec4( 0.0f, 1.0f, 0.0f, 1.0f ), vec3(0.0f,0.0f,1.0f), vec2(1.0f,0.0f) },
        { vec4( 0.5f,  0.5f, 0.0f, 1.0f ), vec4( 0.0f, 0.0f, 1.0f, 1.0f ), vec3(0.0f,0.0f,1.0f), vec2(1.0f,1.0f) },
        { vec4(-0.5f,  0.5f, 0.0f, 1.0f ), vec4( 1.0f, 1.0f, 1.0f, 1.0f ), vec3(0.0f,0.0f,1.0f), vec2(0.0f,1.0f) }
    };
 
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertexData, GL_STATIC_DRAW);
    checkError() ;
}




Square::Square(std::string name, GLuint prog)
{
    SetBuffers(prog) ;
    return ;
}

void Square::Draw(void) {

    glFrontFace(GL_CCW) ;
    glDisable(GL_CULL_FACE) ;
    glBindVertexArray(_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    checkError() ;
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    checkError() ;
    glBindVertexArray(0) ;
    checkError() ;
}

Cube::Cube(std::string name, GLuint prog)
{
    SetBuffers(prog) ;
    return ;
}

void Cube::SetData(GLuint prog)
{
    
    checkError() ;
    
    // put all elements in one array

    
    VertexData vertexDataArray[36] ;
    
    int count = 0, texCount = 0 ;
    for( int i = 0 ; i < 36; i++){
        vertexDataArray[i].position[0] = vertices1[count] ;
        vertexDataArray[i].position[1] = vertices1[count+1] ;
        vertexDataArray[i].position[2] = vertices1[count+2] ;
        vertexDataArray[i].position[3] = 1.0 ;
        
        
        vertexDataArray[i].normal[0] = normals1[count] ;
        vertexDataArray[i].normal[1] = normals1[count + 1] ;
        vertexDataArray[i].normal[2] = normals1[count + 2] ;
        
        vertexDataArray[i].colour[0] = colors1[count] ;
        vertexDataArray[i].colour[1] = colors1[count + 1] ;
        vertexDataArray[i].colour[2] = colors1[count + 2] ;
        vertexDataArray[i].colour[3] = 1.0 ;
        
        
        vertexDataArray[i].texCoord[0] = cubeTexCoord[texCount] ;
        vertexDataArray[i].texCoord[1] = cubeTexCoord[texCount+1] ; ;
        
        
        //cout << vertexDataArray[i].position << " --- " << vertices1[count] << ","
        //    << vertices1[count+1] << "," << vertices1[count+2] << endl ;
        //cout << vertexDataArray[i].position << "-- "
        //<< vertexDataArray[i].colour << endl ;
        count = count + 3 ;
        texCount = texCount + 2 ;
        
    }
    
    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(VertexData), vertexDataArray, GL_STATIC_DRAW);
    checkError() ;
    
}

void Cube::Draw(void) {

    glFrontFace(GL_CCW) ;
    //glCullFace(GL_FRONT);
    
    //glEnable(GL_CULL_FACE) ;
    glDisable(GL_CULL_FACE) ;
    
    glBindVertexArray(_vao);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0) ;
    
}

Cylinder::Cylinder(std::string name, int n, GLuint prog)
{
    if( n < 1) return ;
    _n = n ;
    SetBuffers(prog) ;
    return ;
}





void GetVertexCylinder(VertexData &vd, float u, float v)
{
    
    vd.position[0] = 0.5*cos(u*2*M_PI);
    vd.position[1] = 0.5*sin(u*2*M_PI) ;
    vd.position[2] = (v-0.5) ;
    vd.position[3] = 1.0 ;
    
    vd.normal[0] = cos(u*2*M_PI) ;
    vd.normal[1] = sin(u*2*M_PI) ;
    vd.normal[2] = 0.0 ;
    vd.colour = vec4(1.0,0.0,0.0,1.0) ;
    
    vd.texCoord[0] = u ;
    vd.texCoord[1] = v ;
    
    return;
}


void Cylinder::SetData(GLuint prog)
{
    
    if( _n < 1) return;
    
    VertexData *vertexDataArray = new VertexData[_n*_n*6] ;
    
    checkError() ;
    
    float du = 1.0 / (float) _n ;
    float dv = du ;
    // do it by quads made up of two triangles
    int count = 0 ;
    for( float u = 0 ; u < 1.0 ; u += du) {
        for( float v = 0 ; v < 1.0  ; v += dv) {
            //cerr << "----------------------------\n" ;
            //cerr << "(" << u << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v+dv << ")" << endl ;
            //cerr << "(" << u << "," << v+dv << ")" << endl ;
           
            // make them into triangles
            VertexData vd1,vd2,vd3,vd4 ;
            GetVertexCylinder(vd1, u,v) ;
            GetVertexCylinder(vd2, u+du,v) ;
            GetVertexCylinder(vd3, u+du,v+dv) ;
            GetVertexCylinder(vd4, u,v+dv) ;
            
            
            vertexDataArray[count++] = vd1 ;
            vertexDataArray[count++] = vd2 ;
            vertexDataArray[count++] = vd3 ;
            
           
            vertexDataArray[count++] = vd3 ;
            vertexDataArray[count++] = vd4 ;
            vertexDataArray[count++] = vd1 ;
            
        }
           
    }
    glBufferData(GL_ARRAY_BUFFER, _n*_n*6*sizeof(VertexData), vertexDataArray, GL_STATIC_DRAW);
    checkError() ;
    delete [] vertexDataArray ;
}



void Cylinder::Draw() {
    
    glFrontFace(GL_CCW) ;
    //glEnable(GL_CULL_FACE) ;
    //glDisable(GL_CULL_FACE) ;
    
    glBindVertexArray(_vao);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0,_n*_n*6 );
    checkError() ;
    glBindVertexArray(0) ;
    
}

Cone::Cone(std::string name, int n, GLuint prog)
{
    if( n < 1) return ;
    _n = n ;
    SetBuffers(prog) ;
    return ;
}

void Cone::Draw() {
    
    glFrontFace(GL_CCW) ;
    //glEnable(GL_CULL_FACE) ;
    //glDisable(GL_CULL_FACE) ;
    
    glBindVertexArray(_vao);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0,_n*_n*6 );
    checkError() ;
    glBindVertexArray(0) ;
    
}


void GetVertexCone(VertexData &vd, float u, float v)
{
    float radius = 1.0 - v ;
    
    vd.position[0] = radius*cos(u*2*M_PI);
    vd.position[1] = radius*sin(u*2*M_PI) ;
    vd.position[2] = (v-0.5) ;
    vd.position[3] = 1.0 ;
    
    glm::vec3 n(cos(u*2*M_PI), sin(u*2*M_PI), 1.0) ;
    vd.normal = glm::normalize(n) ;
    
    vd.colour = vec4(1.0,0.0,0.0,1.0) ;
    
    vd.texCoord[0] = u ;
    vd.texCoord[1] = v ;
    
    return;
}

void Cone::SetData(GLuint prog){
    
    if( _n < 1) return;
    
    VertexData *vertexDataArray = new VertexData[_n*_n*6] ;
    
    checkError() ;
    
    float du = 1.0 / (float) _n ;
    float dv = du ;
    // do it by quads made up of two triangles
    int count = 0 ;
    for( float u = 0 ; u < 1.0 ; u += du) {
        for( float v = 0 ; v < 1.0  ; v += dv) {
            //cerr << "----------------------------\n" ;
            //cerr << "(" << u << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v+dv << ")" << endl ;
            //cerr << "(" << u << "," << v+dv << ")" << endl ;
            
            // make them into triangles
            VertexData vd1,vd2,vd3,vd4 ;
            GetVertexCone(vd1, u,v) ;
            GetVertexCone(vd2, u+du,v) ;
            GetVertexCone(vd3, u+du,v+dv) ;
            GetVertexCone(vd4, u,v+dv) ;
            
            vertexDataArray[count++] = vd1 ;
            vertexDataArray[count++] = vd2 ;
            vertexDataArray[count++] = vd3 ;
            
            vertexDataArray[count++] = vd3 ;
            vertexDataArray[count++] = vd4 ;
            vertexDataArray[count++] = vd1 ;
            
        }
    }
    
    // load the attribute data into the vbo
    glBufferData(GL_ARRAY_BUFFER, _n*_n*6*sizeof(VertexData), vertexDataArray, GL_STATIC_DRAW);
    checkError() ;
    delete [] vertexDataArray ;
}


float B3(int i, float t)
{
    float t1 ;
    switch ( i ) {
  
        case 0:
            t1 = 1-t ;
            return t1*t1*t1 ;
            break ;
        case 1:
            return 3*(1-t)*(1-t)*t ;
            break ;
        case 2:
            return 3*(1-t)*t*t ;
            break ;
        case 3:
            return t*t*t ;
            break ;
        default:
            cerr << "ERROR: B3: index must be 0,1,2,3 \n" ;
            return 0 ;
    }
}


float dB3(int i, float t)
{
    float t1 ;
    switch ( i ) {
        case 0:
            t1 = 1-t ;
            return -3*t1*t1 ;
            break ;
        case 1:
            return 3+(9*t-12)*t ;
            break ;
        case 2:
            return 3*t*(2-3*t) ;
            break ;
        case 3:
            return 3*t*t ;
            break ;
        default:
            cerr << "ERROR: dB3: index must be 0,1,2,3 \n" ;
            return 0 ;
    }
}

BezierPatch3::BezierPatch3(std::string name, int n, GLfloat p[4][4][3], GLuint prog)
{
    if( n < 1) return ;
    _n = n ;
    
    SetControlPoints(p);
    SetBuffers(prog) ;
    return ;
}

void BezierPatch3::Draw() {
    
    glFrontFace(GL_CW) ;
    //glEnable(GL_CULL_FACE) ;
    //glDisable(GL_CULL_FACE) ;
    
    glBindVertexArray(_vao);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPointSize(5) ;
    glDrawArrays(GL_TRIANGLES, 0,_n*_n*6 );
    checkError() ;
    glBindVertexArray(0) ;
    
}

void BezierPatch3::GetVertex(VertexData &vd, float u, float v)
{
    
    vec3 dPu = vec3(0.f) ;
    vec3 dPv = vec3(0.f) ;
    vec3 position = vec3(0.f) ;
    
    for( int i = 0 ; i < 4 ; i++ )
    {
        float bi = B3(i,u) ;
        float dbi = dB3(i,u) ;
        for( int j = 0 ; j < 4 ; j++ )
        {
            position = position + bi*B3(j,v)*_cpoints[i][j] ;
            dPu += dbi*B3(j,v)*_cpoints[i][j] ;
            dPv += bi*dB3(j,v)*_cpoints[i][j] ;
        }
    }
    vd.position = vec4(position, 1.0f) ;
    vd.normal = glm::normalize(glm::cross(dPv,dPu)) ;
    
    vd.colour = vec4(1.0,0.0,0.0,1.0) ;
    
    vd.texCoord[0] = u ;
    vd.texCoord[1] = v ;
    //std::cerr << vv << ", " << uu << std::endl ;
    
    return;
}

void BezierPatch3::SetControlPoints(GLfloat p[4][4][3])
{
    for( int i = 0 ; i < 4 ; i++ )
        for( int j = 0 ; j < 4; j ++ )
        {
            _cpoints[i][j][0] = p[i][j][0] ;
            _cpoints[i][j][1] = p[i][j][1] ;
            _cpoints[i][j][2] = p[i][j][2] ;
            
        }
    
}

void BezierPatch3::SetData(GLuint prog){
    

    if( _n < 1) return;
    
    
    VertexData *vertexDataArray = new VertexData[_n*_n*6] ;
    
    checkError() ;
    
    float du = 1.0 / (float) _n ;
    float dv = du ;
    // do it by quads made up of two triangles
    int count = 0 ;
    for( float u = 0 ; u < 1.0 ; u += du) {
        for( float v = 0 ; v < 1.0  ; v += dv) {
            //cerr << "----------------------------\n" ;
            //cerr << "(" << u << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v+dv << ")" << endl ;
            //cerr << "(" << u << "," << v+dv << ")" << endl ;
            
            // make them into triangles
            VertexData vd1,vd2,vd3,vd4 ;
            GetVertex(vd1, u,v) ;
            GetVertex(vd2, u+du,v) ;
            GetVertex(vd3, u+du,v+dv) ;
            GetVertex(vd4, u,v+dv) ;
            
            vertexDataArray[count++] = vd1 ;
            vertexDataArray[count++] = vd2 ;
            vertexDataArray[count++] = vd3 ;
            
            vertexDataArray[count++] = vd3 ;
            vertexDataArray[count++] = vd4 ;
            vertexDataArray[count++] = vd1 ;
        }
    }
        
    // load the attribute data into the vbo
    glBufferData(GL_ARRAY_BUFFER, _n*_n*6*sizeof(VertexData), vertexDataArray, GL_STATIC_DRAW);
    checkError() ;
    delete [] vertexDataArray ;
}



void Sphere::SetData(GLuint prog){
    
    if( _n < 1) return;
    
    VertexData *vertexDataArray = new VertexData[_n*_n*6] ;
    
    checkError() ;
    
    float du = 1.0 / (float) _n ;
    float dv = du ;
    // do it by quads made up of two triangles
    int count = 0 ;
    for( float u = 0 ; u < 1.0 ; u += du) {
        for( float v = 0 ; v < 1.0  ; v += dv) {
            //cerr << "----------------------------\n" ;
            //cerr << "(" << u << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v << ")" << endl ;
            //cerr << "(" << u+du << "," << v+dv << ")" << endl ;
            //cerr << "(" << u << "," << v+dv << ")" << endl ;
            
            // make them into triangles
            VertexData vd1,vd2,vd3,vd4 ;
            GetVertexSphere(vd1, u,v) ;
            GetVertexSphere(vd2, u+du,v) ;
            GetVertexSphere(vd3, u+du,v+dv) ;
            GetVertexSphere(vd4, u,v+dv) ;
            
            if( !flip(vd1,vd2,vd3) ) {
                vertexDataArray[count++] = vd1 ;
                vertexDataArray[count++] = vd2 ;
                vertexDataArray[count++] = vd3 ;
            }
            else {
                vertexDataArray[count++] = vd1 ;
                vertexDataArray[count++] = vd3 ;
                vertexDataArray[count++] = vd2 ;
            }
            
            
            if( !flip(vd3,vd4,vd1) ) {
                vertexDataArray[count++] = vd3 ;
                vertexDataArray[count++] = vd4 ;
                vertexDataArray[count++] = vd1 ;
            }
            else {
                vertexDataArray[count++] = vd3 ;
                vertexDataArray[count++] = vd1 ;
                vertexDataArray[count++] = vd4 ;
            }
            
        }
        
    }
    
    glBufferData(GL_ARRAY_BUFFER, _n*_n*6*sizeof(VertexData), vertexDataArray, GL_STATIC_DRAW);
    delete [] vertexDataArray ;
    checkError() ;
}






