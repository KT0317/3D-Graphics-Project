#ifndef __drawScene__
#define __drawScene__

void drawScene(float time) ;
void initScene(int width, int height) ;

/// functions called from interaction callbacks
void useLighting(int t) ;
void useCubeMapADS(int t);
void useCubeMapLighting(int t);
void useShader(int indx) ;
void useTexture(int t) ;
void framebuffer_resize(int width, int height);
GLint loadCubeMap(const char * baseFileName, GLint reolution);

void useObjShader(float time);
void useADSPerPixelShader(float time);
#endif
