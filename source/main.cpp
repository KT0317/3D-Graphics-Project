//========================================================================
// This is a small test application for GLFW.
// The program opens a window (640x480), and renders a spinning colored
// triangle (it is controlled with both the GLFW timer and the mouse).
// Author: Petros Faloutsos
// Template supplied by: Petros Faloutsos
// Editted by: Kevin Tang
//========================================================================

#include <stdio.h>
#include <stdlib.h>

#include "opengl.h"
#include "global.h"
#include "drawScene.h"

#include "FrameSaver.h"

void instructions(void) ; // drawScene.cpp


using namespace std ;


// global variables -- Normally they would be in a singlenton

static FrameSaver gFrameSaver;
static int gAnimate = 0;
static int gShader = 0;
static int gRecording = 0;
static int gWidth = 640 ;
static int gHeight = 480 ;
static double gTime = 0.0 ;
static double lastTime = 0.0;

static int nbFrames = 0;
static int gLighting = 0;

// Initialize OopenGL
void initOpenGL(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
    
    glEnable(GL_DEPTH_TEST) ;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
}


// Function that prints OpenGL context information
void printOpenGLVersion(GLFWwindow *window)
{
    // obtain the OpenGL version information
	const GLubyte* openGLversion = glGetString(GL_VERSION);
	const GLubyte* openGLvendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION) ;
    
    int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) ;
    int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) ;
        
	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    
    
    printf("OpenGL Vendor: %s\n", openGLvendor ) ;
    printf("Renderer: %s\n",  renderer) ;
    printf("OpenGL Version (string): %s\n",  openGLversion) ;
    printf("OpenGL Version (integer) : %d.%d\n", major, minor) ;
    printf("GLSL Version: %s\n", glslVersion) ;
    printf("Maximum Texture Size %d\n", texSize) ;
}



// Used during initialization of GLFW
static void error_callback(int error, const char* description)
{
    std::cerr << description;

}

// Callback that handles mouse motion interaction
static void mouseMotion_callback(GLFWwindow* window, double x, double y)
{
    //cerr << "Curson is now at " << x << " " << y << endl ;
    return ;
        
}

// Callback that handles mouse button interaction
static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        cerr << "Left Button Pressed\n" ;
        
    }
    
}

// Callback that handles keyboard interaction
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return ;
    }
    
    if( action == GLFW_PRESS )
    {
        switch (key) {
            case GLFW_KEY_V :
                gFrameSaver.DumpPPM(gWidth,gHeight) ;
                break;
            case GLFW_KEY_A: // toggle animation

                gAnimate = 1 - gAnimate ;
                cerr << "Animate = " << gAnimate << endl ;
                // reset the timer to point to the current time
                glfwSetTime(gTime) ;

                break ;

            case GLFW_KEY_M:
              
                if( gRecording == 1 )
                {
                    cerr << "Frame recording disabled." << endl;
                    gRecording = 0 ;
                    gFrameSaver.StopRecord() ;
                    glfwSetTime(gTime) ;

                }
                else
                {
                    cerr << "Frame recording enabled." << endl;
                    gRecording = 1;
                    gFrameSaver.StartRecord() ;
                }
                break;
			// Texture only used when gShader = 0
            case GLFW_KEY_1:
                useTexture(1);
                break ;
            case GLFW_KEY_2:
                useTexture(2);
                break ;
            case GLFW_KEY_3:
                useTexture(3);
                break ;
            case GLFW_KEY_4:
                useTexture(12);
                break ;
            case GLFW_KEY_5:
                useTexture(13);
                break ;
            case GLFW_KEY_6:
                useTexture(23);
                break ;
            case GLFW_KEY_7:
                useTexture(123);
                break ;
            case GLFW_KEY_8:
                useTexture(100);
                break ;
            case GLFW_KEY_L:
                if( GLFW_MOD_SHIFT & mods ) useLighting(0) ;
                else useLighting(1) ;
                break ;
                
            case GLFW_KEY_H:
                instructions();
                break;

			case GLFW_KEY_P:
				gShader = (2 + gShader) % 4;
				// gShader = 0		Per Fragment
				// gShader = 1		Per Vertex		(CURRENTLY NOT USED)
				// gShader = 2		Per Fragment with Geometry shader
				cerr << "Shader = " << gShader << endl;
				useShader(gShader);
				break;

                        
			// illuminate sphere A with cube map reflection only
			case GLFW_KEY_R:
				if (GLFW_MOD_SHIFT & mods) useCubeMapADS(0);
				else useCubeMapADS(1);
				break;

			// illuminate sphere B with image based diffuse lighting only.
			case GLFW_KEY_D:
				if (GLFW_MOD_SHIFT & mods) useCubeMapLighting(0);
				else useCubeMapLighting(1);
				break;

			// illuminate sphere C with image based specular lighting only
			case GLFW_KEY_S:
				if (GLFW_MOD_SHIFT & mods) useCubeMapLighting(0);
				else useCubeMapLighting(2);
				break;

			// illuminate sphere C with a combination of diffuse and specular image based lighting.
            case GLFW_KEY_T:
				if (GLFW_MOD_SHIFT & mods) useCubeMapLighting(0);
				else useCubeMapLighting(12);
                break ;
        }
    }
    
}

// Callback that handles resizing of the OpenGL window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    framebuffer_resize(width, height) ;
}



// The main entry point of the program
int main( void )
{

    glfwSetErrorCallback(error_callback) ;

    // Initialise GLFW
    if( !glfwInit() )
    {
        std::cerr <<  "Failed to initialize GLFW\n" ;
        exit( EXIT_FAILURE );
    }

    // Open a window and create its OpenGL context
    //glfwDefaultWindowHints() ;
#ifdef __APPLE__ // 3.2 for the current OSX
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3) ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2) ;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE) ;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) ;
#else // 4.3 for windows and linux
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4) ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3) ;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE) ;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) ;
#endif
    
    GLFWwindow* window = glfwCreateWindow(gWidth, gHeight, "CSE 4431, FPS: 0" , NULL, NULL);
    if( !window )
    {
        std::cerr << "Failed to open GLFW window\n";

        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    glfwMakeContextCurrent(window);

	// Load the OpenGL functions.
	if (!gladLoadGL()) { exit(-1); }
    
    //glfwSwapInterval(0) ;
    checkError() ;
    
    // Load the OpenGL function pointers

	/* 
	NOTE: USING GLAD INSTEAD OF GLEW

    glewExperimental = GL_TRUE ;
    GLenum errGlew = glewInit();
    if (GLEW_OK != errGlew)
    {
        std::cerr << "ERROR: Cannot initialize glew\n" ;
        fprintf(stderr, "Error: %s\n", glewGetErrorString(errGlew));
        glfwDestroyWindow(window);
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    */

    // Set event callbacks so we can capture and handle user interaction
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback) ;
    glfwSetMouseButtonCallback(window, mouseButton_callback) ;
    glfwSetCursorPosCallback(window, mouseMotion_callback) ;
    
    checkError() ;
    
    printOpenGLVersion(window) ;
    
    // Initialize OpenGL
    initOpenGL() ;
    
    // Initialize the scene (e.g. load primitives etc)
    initScene(gWidth, gHeight) ;
    
    // Set the recording directory
    gFrameSaver.SetDir(gFrameDir) ;
    
    // global variable that keems the master clock
    gTime = 0.0 ;
    // Start the main loop until an event closes the window
    
    while (!glfwWindowShouldClose(window))
    {

        //cerr << gTime << endl ;
    
        // draw the scene
        drawScene(gTime) ;
           
        // Swap buffers
        glfwSwapBuffers(window);
        
        // store time in a global variable
        if( gRecording && gAnimate )
        {
            glFinish() ;
            gFrameSaver.DumpPPM(gWidth,gHeight) ;
            gTime += 0.033 ; // 30 FPS
        }
        else if( gAnimate)
            gTime = glfwGetTime();
        
        if( gAnimate) {
            
            // Handle events
            glfwPollEvents() ;

			// put FPS counter here
			// Measure speed
			gTime = glfwGetTime();
			nbFrames++;

			if (gTime - lastTime >= 2) {
				// print
				//cerr << "FPS = " << (int)(nbFrames / (gTime - lastTime)) << endl;

				// Change FPS into char *
				char buffer[5];
				sprintf(buffer, "%d", (int)(nbFrames / (gTime - lastTime)));

				// append FPS to title and cast as const char*
				char Result[30] = "CSE 4431, FPS = ";
				strcat(Result, buffer);
				const char *title = (const char *)Result;

				// this function only takes const char*
				glfwSetWindowTitle(window, title);

				
				nbFrames = 0;
				lastTime = gTime;
			}
			
        }
        else {
			glfwSetWindowTitle(window, "CSE 4431, FPS = 0");
            glfwWaitEvents() ;
        }
       
    }

    // Destroy the window to free resources
    glfwDestroyWindow(window);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    exit( EXIT_SUCCESS );
}

void instructions(void)
{
	cerr << "Keyboard:" << endl;
	cerr << "  v to save the image." << endl;
	cerr << "  t to reset time and objects." << endl;
	cerr << "  a to toggle the animation." << endl;
	cerr << "  m to toggle frame dumping." << endl;
    cerr << " l/L to turn lighting on/off." << endl ;
	cerr << " 1-7 to use different textures." << endl;
	cerr << " p to toggle between two different shaders." << endl;
	cerr << " r to illuminate sphere A with cube map reflection only (only when Shader = 0)." << endl;
	cerr << " d to illuminate sphere C with image based diffuse lighting only (only when Shader = 0)." << endl;
	cerr << " s to illuminate sphere C with image based specular lighting only (only when Shader = 0)." << endl;
	cerr << " t to illuminate sphere C with a combination of diffuse and specular image based lighting. (only when Shader = 0)." << endl;
	cerr << "  ESC to quit" << endl;
    
	cerr << endl;
    
}

