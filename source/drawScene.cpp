
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/glm.hpp"
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
// glm::value_ptr
#include "glm/gtc/type_ptr.hpp"

#include <string>

#include "opengl.h"
#include "global.h"
#include "image.h"
#include "drawScene.h"

#include "program.h"
#include "geomObjects.h"
#include "vbomesh.h"
#include "vboplane.h"

#include "matStack.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;

using glm::vec3 ;
using glm::vec4 ;
using glm::mat4 ;

// -------- global variables -- Normally they should be in a singleton or namespace

// The shader list
static Shaders gShaders ;

// The canonical objects (primitives) ;
static Triangle *gTriangle;
static Square *gSquare;
static Pyramid *gPyramid;
static Cube *gCube ;
static Cylinder *gCylinder ;
static Cone *gCone ;
static Sphere *gSphere ;
static BezierPatch3 *gB3_1, *gB3_2 ;

// OBJ files
static VBOMesh *gMug;
static VBOMesh *gDonut;
static VBOMesh *gPlate;

static SkyBox *gSkyBox;
static VBOPlane *gPlane;

// Projection matrix and Model-View Matrix
static glm::mat4 gProjMat(1.0f) ;
static glm::mat4 gMVP(1.0f) ;

// projection matrix for each viewport
static glm::mat4 gProjMat_bottomLeft(1.0f);
static glm::mat4 gProjMat_bottomRight(1.0f);
static glm::mat4 gProjMat_topLeft(1.0f);
static glm::mat4 gProjMat_topRight(1.0f);

GLuint fboHandle;				// Used for RenderToTex
GLuint fboLayeredRender;		// used for LayeredRendering (currently not used)
static float viewportWidth;		// equal width for all viewports i.e. half the height
static float viewportHeight;	// equal height for all viewports i.e. half the height

// The modeling matrix stack
static ModelviewStack gMS(MAX_STACK_SIZE) ;

const GLuint gNumTex = 3;
GLuint gTexIDs[gNumTex] ; // Three textures
GLuint gskyboxID[4];	// load skybox textures;

// Default to using ADSPerPixel shaders		
// obj = 2, ADSPerPixel = 0 
// ADSPerVertex = 1 (we won't be using this one)
int shaderFlag;	

void loadTextures(void) ;
void setTextures(void) ;
void initUniforms(void) ;
void setupFBORenderToTex(void);

void renderToTexture(VBOMesh *obj);
void renderPlane(VBOPlane *plane);

GLfloat ctrlpoints1[4][4][3] = {
    {{-1.5, -1.5, 0.0}, {-0.5, -1.5, 0.0},
        {0.5, -1.5, 0.0}, {1.5, -1.5, 0.0}},
    {{-1.5, -0.5, 0.0}, {-0.5, -0.5, 0.0},
        {0.5, -0.5, 0.0}, {1.5, -0.5, 0.0}},
    {{-1.5, 0.5, 0.0}, {-0.5, 0.5, 0.0},
        {0.5, 0.5, 0.0}, {1.5, 0.5, 0.0}},
    {{-1.5, 1.5, 0.0}, {-0.5, 1.5, 0.0},
        {0.5, 1.5, 0.0}, {1.5, 1.5, 0.0}}
};
GLfloat ctrlpoints2[4][4][3] = {
    {{-2.5, -2.5, 0.0}, {-0.5, -1.5, 0.0},
        {0.5, -1.5, 0.0}, {1.5, -1.5, 0.0}},
    {{-1.5, -0.5, 0.0}, {-0.5, -0.5, 3.0},
        {0.5, -0.5, 3.0}, {1.5, -0.5, 0.0}},
    {{-1.5, 0.5, 0.0}, {-0.5, 0.5, 3.0},
        {0.5, 0.5, 3.0}, {1.5, 0.5, 0.0}},
    {{-1.5, 1.5, 0.0}, {-0.5, 1.5, 0.0},
        {0.5, 1.5, 0.0}, {1.5, 1.5, 0.0}}
};


GLfloat ctrlpoints3[4][4][3] = {
    {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
        {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
    {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0},
        {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
    {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0},
        {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
    {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
        {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};


// Used to print out the value of a 4x4 matrix
void printMat4(glm::mat4 &m)
{
    printf("------------------\n") ;
    for( int i = 0 ; i < 4; i++)
    {
        for( int j = 0 ; j < 4; j++)
            printf("%lf ", m[i][j]) ;
        printf("\n") ;
    }
}

// Setting Ambient, Diffuse, Specular, and shininess coefficients for objects
void setMaterial(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float shininess)
{
    gShaders.setFloat3("Material.Ka", Ka[0], Ka[1], Ka[2]);
    gShaders.setFloat3("Material.Kd", Kd[0], Kd[1], Kd[2]);
    gShaders.setFloat3("Material.Ks", Ks[0], Ks[1], Ks[2]);
    gShaders.setFloat("Material.Shininess", shininess);
}

// Setting Ambient, Diffuse, Specular, and shininess coefficients for lights
void setLight(glm::vec4 Position, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is)
{
    gShaders.setFloat3("Light.Ia", Ia[0], Ia[1], Ia[2]);
    gShaders.setFloat3("Light.Id", Id[0], Id[1], Id[2]);
    gShaders.setFloat3("Light.Is", Is[0], Is[1], Is[2]);
    gShaders.setFloat4("Light.Position", Position[0], Position[1], Position[2], Position[3]);
}

void setColour(float c1,float c2,float c3)
{
	// Ks always one for non metalic materials
	vec3 ks = vec3(1.0f) ;
    vec3 c(c1,c2,c3) ;
	setMaterial(c,c,ks,40.0f) ;
}


// Used for string token
void trimString(string & str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}

// Use the import material info (computed by blender3D) through .mtl files
void setObjMaterial(const char * fileName)
{
	ifstream objMaterialStream(fileName, std::ios::in);

	if (!objMaterialStream)
		cerr << "Unable to open OBJ file: " << fileName << endl;
	else {
		string line, token;
		getline(objMaterialStream, line);
		while (!objMaterialStream.eof()) {
			trimString(line);

			if (line.length() > 0 && line.at(0) != '#') {
				istringstream lineStream(line);

				lineStream >> token;
				
				if (token == "Ns") {		// Shininess
					float shininess;
					lineStream >> shininess;
					gShaders.setFloat("Material.Shininess", shininess);
				}
				else if (token == "Ka") {	// Ambient
					float x, y, z;
					lineStream >> x >> y >> z;
					gShaders.setFloat3("Material.Ka", x, y, z);
				}
				else if (token == "Kd") {	// Diffuse
					float x, y, z;
					lineStream >> x >> y >> z;
					gShaders.setFloat3("Material.Kd", x, y, z);
				}
				else if (token == "Ks") {	// Specular
					float x, y, z;
					lineStream >> x >> y >> z;
					gShaders.setFloat3("Material.Ks", x, y, z);
				}
			}
			getline(objMaterialStream, line);
		}
	}
}


// Tells the fragment shader which of the 3 textures to use:
// 0 means no texture
// 1,2,3 specifies the respective texture
// 12 means 1 and 2
// 23 means 2 and 3
// 13 means 1 and 3
// 123 means all three
void useTexture(int v)
{
    gShaders.setInt("useTex", v) ;
}


void useLighting(int t) // should be 0 or 1 (off or on)
{
    gShaders.setInt("useLighting", t) ;
}

void useCubeMapADS(int t) // should be 0 or 1 (off or on)
{
	gShaders.setInt("WithADS", t);
}

// Tell the fragment shader
// 1 means diffuse lighting
// 2 means specular lighting
// 12 means both
// 0 means neither
void useCubeMapLighting(int t) 
{
	gShaders.setInt("CubeMapLighting", t);
}

// Sets the Modelview, the normal, and the MVP matrices from the stack given
void setMatricesFromStack(ModelviewStack & mvs)
{
    glm::mat4 m = mvs.Top() ;
    gShaders.setMatrix4f("modelviewMat", m);
    checkError();

    gMVP = gProjMat*m ;
    gShaders.setMatrix4f("MVP", gMVP) ;
	checkError();
    
    glm::mat4 nm  = glm::inverseTranspose(m);
    gShaders.setMatrix4f("normalMat", nm);
    checkError();

}



// Draw unit triangle centered at the origin
// Sets the modelview matrix and the normal matrix of gProg1
void drawTriangle(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gTriangle->Draw();
}

// Draw unit square-based pyramid centered at the origin
// Sets the modelview matrix and the normal matrix of gProg1
void drawPyramid(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gPyramid->Draw();
}

// Draws a unit sphere centered at the origin of radius 1.0 ;
// Sets the modelview matrix and the normal matrix of gProg1
void drawSphere(ModelviewStack & mvs)
{
    setMatricesFromStack(mvs) ;
    gSphere->Draw() ;
    
}


// Utility function that draws a 2x2x2 cube center at the origin
// Sets the modelview matrix at the "modelviewMat" uniform of gProg1
// and the corresponding "normalMat"
void drawCube(ModelviewStack & mvs)
{
        setMatricesFromStack(mvs) ;
        gCube->Draw() ;
}


// Utility function that draws a 1x1 quad center at the origin
// Sets the modelview matrix and the normal matrix of gProg1
void drawSquare(ModelviewStack & mvs)
{
    setMatricesFromStack(mvs) ;
    gSquare->Draw() ;
    
}

// Utility function that draws a cylinder along z of height 1 centered at the origin
// and radius 0.5 ;
// Sets the modelview matrix and the normal matrix of gProg1
void drawCylinder(ModelviewStack & mvs)
{
    setMatricesFromStack(mvs) ;
    gCylinder->Draw() ;
    
}

// Utility function that draws a cone along z of height 1 centered at the origin
// and base radius 1.0 ;
// Sets the modelview matrix and the normal matrix of gProg1
void drawCone(ModelviewStack & mvs)
{
    setMatricesFromStack(mvs) ;
    gCone->Draw() ;
    
}


// Draws a cubic bezier patch;
// Sets the modelview matrix and the normal matrix of gProg1
void drawBezierPatch(BezierPatch3 *b, ModelviewStack & mvs)
{
    setMatricesFromStack(mvs) ;
    b->Draw() ;
}


// Draws a skybox
// Sets the modelview matrix and the normal matrix of gProg1
void drawSkyBox(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gSkyBox->Draw();
}

// Draw the plane
// Sets the modelview matrix and the normal matrix of gProg1
void drawPlane(ModelviewStack & mvs) 
{
	setMatricesFromStack(mvs);
	gPlane->render();
}


// Draw all the specific OBJ models
// Sets the modelview matrix and the normal matrix of gProg1
void drawMug(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gMug->render();
}
void drawPlate(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gPlate->render();
}
void drawDonut(ModelviewStack & mvs)
{
	setMatricesFromStack(mvs);
	gDonut->render();
}



// Take current modelviewstack
// render obj into a texture map then render a plane using that specific texture
void drawRenderToTexToRender(ModelviewStack & mvs, VBOMesh *obj, VBOPlane *plane)
{
	gShaders.setInt("useRenderTexture", 1);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	setMatricesFromStack(mvs);
	renderToTexture(obj);
	glFlush();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	renderPlane(plane);
	gShaders.setInt("useRenderTexture", 0);
}

void renderToTexture(VBOMesh* obj)
{
	gShaders.setInt("RenderTex", 7);
	glViewport(0, 0, 512, 512);			// NOTE: Remember to change viewport back to original
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	obj->render();
}

void renderPlane(VBOPlane * plane)
{
	gShaders.setInt("RenderTex", 6);
	glViewportIndexedf(0, 0.0, 0.0, viewportWidth, viewportHeight);
	glViewportIndexedf(1, viewportWidth, 0.0, viewportWidth, viewportHeight);
	glViewportIndexedf(2, 0.0, viewportHeight, viewportWidth, viewportHeight);
	glViewportIndexedf(3, viewportWidth, viewportHeight, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	plane->render();
}



// Scene initialization -- Put here things that need to be done only once
// before the scene is drawn
void initScene(int width, int height)
{
    checkError() ;
    framebuffer_resize(width,height) ;
    
    // Clear color buffer to greenish yellow
    glClearColor( 0.5f, 0.5f, 0.2f, 1.0f );
    checkError() ;
   
    loadTextures() ;  // load textures before loading the shaders
    
    // Load the shader pairs/group -- MUST BE DONE before the objects are loaded
    
    gShaders.setShaderDir(gShaderDir) ;
    if( !gShaders.loadShaderPair("adsPerPixel") ) exit(-1) ;
    if( !gShaders.loadShaderPair("adsPerVertex") ) exit(-1) ;
	if (!gShaders.loadShaderPairWithGeom("obj")) exit(-1);
    // chose which shader to use and set their initial uniforms
	useShader(1);
	useShader(2);
	useShader(0);

	checkError();

    // Load canonical primitives (objects)
    GLuint pid = gShaders.getActiveID() ;
   
    checkError() ;
	gTriangle = new Triangle("triangle1", pid);
	checkError();
    gSquare = new Square("square1", pid) ;
    checkError() ;
	gPyramid = new Pyramid("pyramid1", pid);
	checkError();
    gCube = new Cube("cube1", pid) ;
    checkError() ;
    gCylinder = new Cylinder("cylinder1", 20, pid) ;
    checkError() ;
    gCone = new Cone("cone1", 20,  pid) ;
    checkError() ;
    gSphere = new Sphere("sphere1", 20, pid) ;
    checkError() ;
    gB3_1 = new BezierPatch3("b3-1", 20, ctrlpoints1, pid) ;
    checkError() ;
    gB3_2 = new BezierPatch3("b3-2", 20, ctrlpoints2, pid) ;
    checkError() ;
	gSkyBox = new SkyBox();
	checkError();
	gPlane = new VBOPlane(15.0f, 15.0f, 1, 1);
	checkError();


	// Load Obj objects using VBOMesh.cpp
	std::string fname = gProjectPath + "media/" + "mug.obj";
	char fname_char[200];

	strcpy(fname_char, fname.c_str());
	gMug = new VBOMesh(fname_char, true, true, true);

	fname = gProjectPath + "media/" + "plate.obj";
	strcpy(fname_char, fname.c_str());
	gPlate = new VBOMesh(fname_char, true, true, true);

	fname = gProjectPath + "media/" + "donut.obj";
	strcpy(fname_char, fname.c_str());
	gDonut = new VBOMesh(fname_char, true, true, true);
    
    useLighting(1) ;
	useCubeMapADS(0);
    useTexture(3) ;

	setupFBORenderToTex();
    
	// One pixel white texture (used to take a render and convert it into a texture map)
	GLuint whiteTexHandle;
	GLubyte whiteTex[] = { 255, 255, 255, 255 };
	glActiveTexture(GL_TEXTURE7);					// Texture unit 7
	glGenTextures(1, &whiteTexHandle);
	glBindTexture(GL_TEXTURE_2D, whiteTexHandle);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, whiteTex);
}



// The main display function
// Put your drawing code here  - Feel free to add functions as you see fit
void drawScene(float time)
{
    //gShaders.setFloat("time", time);
    
    //printf("Time = %f\n", time) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // Load an identity matrix on the modeling stack
    gMS.LoadIdentity();
    
    //------------ Set up a view --------------
    
    gMS.SetViewMatrix(vec3(0,5,10),vec3(0,0,0), vec3(0,1,0));
  

	//------------- Load Models based on the shaders used -----------------
	if (shaderFlag == 2)
		useObjShader(time);
	else if (shaderFlag == 0)
		useADSPerPixelShader(time);


	//----------------Example on how to draw the scene--------------------------------

	// Pushing a matrix onto the top will make all marix tranformations be applied to only the matrix on top
	// once popped, revert back to first/orignal matrix (
	// good way to keep track of matrices

    /*  Here is an example
    gMS.Push() ;
    {
        gMS.Rotate(20*time, vec3(0.0f,1.0f,0.0f)) ;
        gMS.Push() ; {
            gMS.Translate(vec3(-2.0f,0.0f,0.0f)) ;
            gMS.Scale(vec3(2.0f,2.0f,2.0f)) ;
            drawSquare(gMS) ;
            drawBezierPatch(gB3_1,gMS) ;
        }
        gMS.Pop() ;
        
        setColour(1.0,0.0,0.0) ;
        gMS.Push() ; {
            gMS.Translate(vec3(2.0f,0.0f,0.0f)) ;
            gMS.Scale(vec3(2.0f,2.0f,2.0f)) ;
            drawBezierPatch(gB3_2,gMS) ;
        }
        gMS.Pop();
        
    }
    gMS.Pop() ;
    */
}

// Used in drawScene function
void useObjShader(float time)
{
	// NOTE TO SELF: W AND H NOT POSITION ON THE SCREEN. IT IS SPECIFYING THE WIDTH/HEIGHT OF THE VIEWPORT
	// If any of the function changes the Viewport (i.e. glViewport(...); is used), use the 4 lines below to change it back
	// PARAM: index, x,	y, width, height
	glViewportIndexedf(0, 0.0, 0.0, viewportWidth, viewportHeight);							//bottom left corner
	glViewportIndexedf(1, viewportWidth, 0.0, viewportWidth, viewportHeight);				//bottom right corner
	glViewportIndexedf(2, 0.0, viewportHeight, viewportWidth, viewportHeight);				//top left corner
	glViewportIndexedf(3, viewportWidth, viewportHeight, viewportWidth, viewportHeight);	//top right corner

	char fname_char[200];

	gMS.Rotate(20 * time, vec3(0.0f, 1.0f, 0.0f));

	//------------- Draw your from your OBJ files ----------
	//--------------------------Plane-----------------------
	setMaterial(vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.2f, 0.1f), vec3(0.95f, 0.95f, 0.95f), 100.0f);
	gMS.Push();
	{
		drawRenderToTexToRender(gMS, gDonut, gPlane);
	} gMS.Pop();


	//--------------------------Mug-------------------------
	std::string fname = gProjectPath + "media/" + "mug.mtl";
	strcpy(fname_char, fname.c_str());
	setObjMaterial(fname_char);
	gMS.Push();
	{
		gMS.Translate(vec3(6.0f, 1.5f, 0.0f));

		drawMug(gMS);
	} gMS.Pop();


	//--------------------------Plate-----------------------
	fname = gProjectPath + "media/" + "plate.mtl";
	strcpy(fname_char, fname.c_str());
	setObjMaterial(fname_char);
	gMS.Push();
	{
		gMS.Translate(vec3(0.0f, 0.4f, 0.0f));

		drawPlate(gMS);
	} gMS.Pop();


	//--------------------------Donut-----------------------
	fname = gProjectPath + "media/" + "donut.mtl";
	strcpy(fname_char, fname.c_str());
	setObjMaterial(fname_char);
	gMS.Push();
	{
		gMS.Translate(vec3(0.0f, 1.0f, 0.0f));

		gShaders.setInt("donutIcing", 1);
		drawDonut(gMS);
		gShaders.setInt("donutIcing", 0);
	} gMS.Pop();


	//------------- Draw your skybox ----------
	gMS.Push();
	{

		gShaders.setInt("DrawSkyBox", 1);
		drawSkyBox(gMS);
		gShaders.setInt("DrawSkyBox", 0);
	} gMS.Pop();
}

// Used in drawScene function
void useADSPerPixelShader(float time)
{
	// NOTE: viewportWidth and viewportHeight is set to HALF the window width and height
	// This is just to reduce the number of "width/2" and "height/2" calls

	glViewport(0, 0, viewportWidth * 2, viewportHeight * 2);	// This shader uses a single viewport

	//------------- Draw your objects ----------
	//-----------------SPHERE A-----------------
	setMaterial(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f), vec3(0.7f, 0.6f, 0.6f), 20);
	gMS.Push();
	{
		gMS.Rotate(20 * time, vec3(0.0f, 1.0f, 0.0f));
		gMS.Translate(vec3(-3.0f, 0.0f, 0.0f));
		drawSphere(gMS);
	} gMS.Pop();


	//----------------SPHERE B------------------
	gShaders.setInt("CubeMapADS", 1);
	setMaterial(vec3(0.25f, 0.25f, 0.25f), vec3(0.4f, 0.4f, 0.4f), vec3(0.774597f, 0.774597f, 0.774597f), 20.0);
	gMS.Push();
	{
		gMS.Rotate(20 * time, vec3(0.0f, 1.0f, 0.0f));
		//gMS.Translate(vec3(0.0f, 0.0f, 0.0f));
		drawSphere(gMS);
		gShaders.setInt("CubeMapADS", 0);
	} gMS.Pop();


	//----------------SPHERE C------------------
	gShaders.setInt("useCubeMap", 1);
	setMaterial(vec3(0.25f, 0.25f, 0.25f), vec3(0.4f, 0.4f, 0.4f), vec3(0.774597f, 0.774597f, 0.774597f), 20);
	gMS.Push();
	{
		gMS.Rotate(20 * time, vec3(0.0f, 1.0f, 0.0f));
		gMS.Translate(vec3(3.0f, 0.0f, 0.0f));
		drawSphere(gMS);
		gShaders.setInt("useCubeMap", 0);
	} gMS.Pop();


	//------------- Draw your skybox ----------
	gMS.Push();
	{
		gMS.Rotate(20 * time, vec3(0.0f, 1.0f, 0.0f));
		gShaders.setInt("DrawSkyBox", 1);
		drawSkyBox(gMS);
		gShaders.setInt("DrawSkyBox", 0);

		gShaders.use(0);
	} gMS.Pop();
}

////////////////////////////////////////////////////////
//-------------- Texture Processing ------------------//
////////////////////////////////////////////////////////

void initTexture()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    if( 1 )
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }else {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
}


void setTexture(GLuint prog, std::string uname, GLuint activeTexID)
{
    initTexture() ;
    GLuint texUniform = glGetUniformLocation(prog, uname.c_str()) ;
    if ((int) texUniform >=0) {
        glUniform1i(texUniform, activeTexID) ;
    }
    else {
        std::cerr << "Shader did not contain the uniform " << uname  << std::endl;
	}
}


void setTextures(void)
{
	// image texture
    glActiveTexture(GL_TEXTURE0) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[0]) ;
    setTexture(gShaders.getActiveID(), "texture1", 0) ;
    glActiveTexture(GL_TEXTURE1) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[1]) ;
    setTexture(gShaders.getActiveID(), "texture2", 1) ;
    glActiveTexture(GL_TEXTURE2) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[2]) ;
    setTexture(gShaders.getActiveID(), "texture3", 2) ;
	

	// cubemap textures
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gskyboxID[0]);
	setTexture(gShaders.getActiveID(), "skybox", 3);

	// cubemap specular textures
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gskyboxID[1]);
	setTexture(gShaders.getActiveID(), "skyboxSpecular", 4);
	
	// cubemap irrandiance textures
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gskyboxID[2]);
	setTexture(gShaders.getActiveID(), "skyboxDiffuse", 5);

	// separate cubemap textures
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gskyboxID[3]);
	setTexture(gShaders.getActiveID(), "cubemap", 8);
}


void loadTextures(void)
{
    std::string fname = gProjectPath + "media/" + "wood.bmp" ;
    char fname_char[200] ;
    strcpy(fname_char, fname.c_str()) ;
    GL_Image2D Img1(fname_char) ;
    fname = gProjectPath + "media/" + "floor.bmp" ;
    strcpy(fname_char, fname.c_str()) ;
    GL_Image2D Img2(fname_char) ;
   
    glGenTextures(gNumTex,gTexIDs) ;
    
    glActiveTexture(GL_TEXTURE0) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[0]) ;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Img1.m_width,
                 Img1.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 Img1.m_data);
    
    glActiveTexture(GL_TEXTURE1) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[1]) ;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Img2.m_width,
                 Img2.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 Img2.m_data);
   // setTexture(gShaders.getActiveID(),"texture2", 1) ;

    
    glActiveTexture(GL_TEXTURE2) ;
    glBindTexture(GL_TEXTURE_2D,gTexIDs[2]) ;
    int i, j, c;
    GLubyte checkImage[64][64][4] ;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            c = ((((i&0x8)==0) ^ (((j&0x8))==0)))*255;
            if( c == 0) c = 50 ;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64,
                 64, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 checkImage);
	
	gskyboxID[0] = loadCubeMap("cubemap", 512);
	gskyboxID[1] = loadCubeMap("cubemapSpecular", 512);
	gskyboxID[2] = loadCubeMap("cubemapDiffuse", 512);
	gskyboxID[3] = loadCubeMap("Yokohama", 512);
	
}

// Copied from GLCookbook
// Loads a cubemap and returns the texID of that cubemap
GLint loadCubeMap(const char * baseFileName,  GLint resolution)
{
	char fname_char[200];
	glActiveTexture(GL_TEXTURE3);

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	// swapped negy and posy position
	// for some reason they were rendered in opposite position
	const char * suffixes[] = { "posx", "negx", "posy", "negy",  "posz", "negz" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	checkError();

	GLint w = resolution, h = resolution;

	// Allocate storage GL_RGBA8
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGB8, w, h);

	// Load each cube-map face
	for (int i = 0; i < 6; i++) {
		std::string texName = gProjectPath + "media/" + std::string(baseFileName) + "/" + suffixes[i] + ".bmp";
		strcpy(fname_char, texName.c_str());
		GL_Image2D Img(fname_char);
		GLubyte * data = Img.m_data;
		glTexSubImage2D(targets[i], 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	checkError();
	return texID;
}



// Copied from GLCookbook
void setupFBORenderToTex(void) {
	// Generate and bind the framebuffer
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// Create the texture object
	GLuint renderTex;
	glGenTextures(1, &renderTex);
	glActiveTexture(GL_TEXTURE6);  // Use texture unit 6
	glBindTexture(GL_TEXTURE_2D, renderTex);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

	// Create the depth buffer
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

	// Bind the depth buffer to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuf);

	// Set the targets for the fragment output variables
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		cout << "Framebuffer is complete" << endl;
	}
	else {
		cout << "Framebuffer error: " << result << endl;
	}

	// Unbind the framebuffer, and revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



// set the uniforms' default value for the active shader
void initUniforms(void)
{
    setTextures() ;
    
    setMaterial(vec3(0.2f,0.2f,0.2f), vec3(1.0f,0.0f,0.0f),vec3(1.0f,1.0f,1.0f), 20.0)  ;
    setLight(vec4(0.0f,0.0f,10.0f,1.0f), vec3(0.2f,0.2f,0.2f), vec3(1.0f,1.0f,1.0f),vec3(1.0f,1.0f,1.0f))  ;
	useCubeMapLighting(0);
	gShaders.setFloat("DrawSkyBox", 0);

	// When using shader: adsPerPixel
	if (shaderFlag == 0)
	{
		gShaders.setFloat("BlendFactor", 0.5);
		gShaders.setFloat("eta", 0.85);
		gShaders.setFloat("useCubeMap", 0);
		gShaders.setFloat("CubeMapADS", 0);
	}

	// When using shader: obj
	// Used for multiple viewports with different projection Matrices
	if (shaderFlag == 2) 
	{
		gShaders.setMatrix4f("ProjectionMatrices[0]", gProjMat_bottomLeft);
		gShaders.setMatrix4f("ProjectionMatrices[1]", gProjMat_bottomRight);
		gShaders.setMatrix4f("ProjectionMatrices[2]", gProjMat_topLeft);
		gShaders.setMatrix4f("ProjectionMatrices[3]", gProjMat_topRight);
	}
}

// Used mainly in the main.cpp and to initialize values
void useShader(int index)
{
	shaderFlag = index ;
    gShaders.use(index) ;
    initUniforms() ;
}

// Used only in the main.cpp for resizing
void framebuffer_resize(int width, int height)
{
    //printf("Before\n") ;printMat4(gProjMat) ;

	// Using this to split the window into 4 viewports
	viewportWidth = width / 2.0f;
	viewportHeight = height / 2.0f;

	//glDepthRange(1.0, 100.0);

	// Aspect Ratio for a single viewport
    float aspectRatio = (float) width / (float) height ;
    if( aspectRatio <= 1 )
    {
		//						 left, right,		bottom,					top,					near,	far
		gProjMat = glm::frustum(-1.0f, 1.0f, -1.0f*(1.0f / aspectRatio), 1.0f*(1.0f / aspectRatio), 1.0f, 100.0f);
        //gProjMat = glm::frustum(-1.0f*aspectRatio, 1.0f*aspectRatio, -1.0f, 1.0f, 1.0f, 500.0f);
    }
    else
    {
		//MODIFY THIS FOR NEAR AND FAR and INITIAL VIEWPORT
		//								left,			right,		bottom,	top,  near,	 far
        gProjMat = glm::frustum(-1.0f*aspectRatio, 1.0f*aspectRatio, -1.0f, 1.0f, 1.0f, 100.0f);
    }


	// Second Aspect Ratio of each different viewport
	// 4 viewports meaning half the width and half the height for each viewport
	float aspectRatio2 = (float)viewportWidth / (float)viewportHeight;
	if (aspectRatio2 <= 1)
	{
		// Original Projection Matrix
		gProjMat_bottomLeft = glm::frustum(-1.0f, 1.0f, -1.0f*(1.0f / aspectRatio2), 1.0f*(1.0f / aspectRatio2), 1.0f, 100.0f);

		// close up of donut
		gProjMat_bottomRight = glm::ortho(-2.0f*aspectRatio2, 2.0f*aspectRatio2, -1.0f*aspectRatio2, 1.0f*aspectRatio2, 1.0f, 100.0f);

		// attempt of top-down view
		gProjMat_topLeft = glm::ortho(-10.0f*aspectRatio2, 10.0f*aspectRatio2, -5.0f*aspectRatio2, 3.0f*aspectRatio2, 1.0f, 100.0f);

		// outside cube map view
		gProjMat_topRight = glm::ortho(-30.0f*aspectRatio2, 30.0f*aspectRatio2, -30.0f*aspectRatio2, 30.0f*aspectRatio2, 1.0f, 100.0f);
	}
	else
	{
		gProjMat_bottomLeft = glm::frustum(-1.0f*aspectRatio2, 1.0f*aspectRatio2, -1.0f, 1.0f, 1.0f, 100.0f);
		gProjMat_bottomRight = glm::ortho(-2.0f*aspectRatio2, 2.0f*aspectRatio2, -1.0f*aspectRatio2, 1.0f*aspectRatio2,  1.0f, 100.0f);
		gProjMat_topLeft = glm::ortho(-10.0f*aspectRatio2, 10.0f*aspectRatio2, -5.0f*aspectRatio2, 3.0f*aspectRatio2, 1.0f, 100.0f);
		gProjMat_topRight = glm::ortho(-30.0f*aspectRatio2, 30.0f*aspectRatio2, -30.0f*aspectRatio2, 30.0f*aspectRatio2,  1.0f, 100.0f);
	}

}

