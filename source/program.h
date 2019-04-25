//
//  program.h
//  cse3431Template
//
//  Created by Petros Faloutsos on 13-09-26.
//  Copyright (c) 2013 Petros Faloutsos. All rights reserved.
//

#ifndef __cse3431Template__program__
#define __cse3431Template__program__


#include "opengl.h"
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/glm.hpp"
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/matrix_transform.hpp"
// glm::value_ptr
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <vector>
#include <string>


GLuint InitShader(const char* vShaderFile, const char* fShaderFile, const char* outputAttributeName) ;

class Program {
public:
    Program(){_prog = 1 ; return ;}
    ~Program() ;
    GLuint loadShaderPair(std::string vertShader, std::string fragShader);
	GLuint loadShaderPairWithGeom(std::string vertShader, std::string fragShader, std::string geomShader);
    GLuint id(void) {return _prog;} ;
    
    void setMatrix4f(std::string name, glm::mat4 &m);
    void setFloat(const std::string name, float v) ;
    void setFloat4(const std::string name, float v1, float v2, float v3, float v4) ;
    void setFloat3(const std::string name, float v1, float v2, float v3) ;
    void setInt(const std::string name, int v);
    
  

    
	void printActiveUniforms() ;
	void printActiveAttributes() ;
private:
    GLuint _prog; // program id
   
   
    
};

class Shaders {
public:
    Shaders() {_n = 0; _activeIndex = -1; _activeProgram = NULL ; _dir = "" ; };
    
    GLuint loadShaderPair(std::string name) ;
	GLuint loadShaderPairWithGeom(std::string name);
    GLuint use(int index) ;
    void setShaderDir(std::string dir) { _dir = dir;} ;
    GLuint getActiveID(void) { return _progs.at(_activeIndex)->id();}
    Program * getActive(void) { return _progs.at(_activeIndex);}
    
    // functions wrapping those of the active program
    void setMatrix4f(std::string name, glm::mat4 &m) { _activeProgram->setMatrix4f(name, m);};
    void setFloat(const std::string name, float v) 
		{_activeProgram->setFloat(name, v);} ;
    void setFloat3(const std::string name, float v1, float v2, float v3)
        { _activeProgram->setFloat3(name, v1, v2, v3);} ;
    void setFloat4(const std::string name, float v1, float v2, float v3, float v4)
        { _activeProgram->setFloat4(name, v1, v2, v3, v4);} ;
    void setInt(const std::string name, int v) {_activeProgram->setInt(name, v);};
    
    void printActiveUniforms(){_activeProgram->printActiveUniforms();} ;
    void printActiveAttributes() {_activeProgram->printActiveAttributes();} ;
    
    int _n ; // number of shaders
    int _activeIndex; // the index in the list of shaders that is active
    Program *_activeProgram ;
    std::vector<Program *> _progs ;
    std::string _dir ; // shader directory
};



#endif /* defined(__cse3431Template__program__) */
