#ifndef VBOMESH_H
#define VBOMESH_H

//#include "drawable.h"

#include <vector>
using std::vector;
//#include <glm/glm.hpp>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/glm.hpp"
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
// glm::value_ptr
#include "glm/gtc/type_ptr.hpp"
using glm::vec3;
using glm::vec2;
using glm::vec4;



#include <string>
using std::string;

#include "opengl.h"

class VBOMesh
{
private:
    GLuint faces;
    GLuint vaoHandle;

    bool reCenterMesh, loadTex, genTang;

    void trimString( string & str );
    void storeVBO( const vector<vec3> & points,
                    const vector<vec3> & normals,
                    const vector<vec2> &texCoords,
                    const vector<vec4> &tangents,
                    const vector<vec2> &elements );
    void generateAveragedNormals(
            const vector<vec3> & points,
            vector<vec3> & normals,
            const vector<vec2> & faces );
    void generateTangents(
            const vector<vec3> & points,
            const vector<vec3> & normals,
            const vector<GLuint> & faces,
            const vector<vec2> & texCoords,
            vector<vec4> & tangents);
    void center(vector<vec3> &);

public:
    VBOMesh( const char * fileName, bool reCenterMesh = false, bool loadTc = false, bool genTangents = false );

    void render() const;

    void loadOBJ( const char * fileName );

};

#endif // VBOMESH_H
