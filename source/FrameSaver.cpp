#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include "opengl.h"

#include <iostream>

#include "FrameSaver.h"

using namespace std;

FrameSaver::FrameSaver()
{
	SetRecording(0) ;
	m_pixels = NULL ;
	m_frameCount = 0 ;
    
    m_dir = "" ;
}

void FrameSaver::SetDir(const string dir)
{
    m_dir = dir ;
}
void FrameSaver::Toggle(void)
{
	if( GetRecording() == 0 ) 
		StartRecord() ;
	else 
		StopRecord() ;
}

void FrameSaver::StopRecord(void)
{
		SetRecording(0) ;
}

void FrameSaver::StartRecord(void)
{
	SetRecording(1) ; 
	m_frameCount = 0 ;
}


int FrameSaver::DumpPPM(int width, int height)
{
	if( m_pixels ) delete [] m_pixels ;
	m_pixels = new unsigned char [3*width];
    if(	m_pixels == NULL )
    {
		cerr << "Cannot allocate enough memory" << endl;
		return  -1;
    }

    std::string fname, baseName ;
    char fn[50], bn[50];
	if( GetRecording() == 0 ) // one time save
	{
        fname = m_dir + "scene.ppm" ;
        baseName = "scene" ;
	}
	else				// animation recording
	{
       
        sprintf(fn,"frame%d.ppm", m_frameCount) ;
        fname = m_dir +fn;
		sprintf(bn,"frame%d", m_frameCount) ;
        baseName = m_dir + bn ;
        
		m_frameCount++ ;
	}
	FILE *fp = fopen(fname.c_str(),"wb") ;
	if( fp == NULL ) 
	{
		cerr << "Cannot open file " << fname << endl;

		return -1 ;
	}
    printf("Saving %s\n", fname.c_str()) ;
	DumpPPM(fp,width,height) ;
	fclose(fp) ;
	//ConvertToPng(baseName.c_str()) ;
	return 1 ;
}
void FrameSaver::ConvertToPng(const char *baseName)
{
	char com[256] ;
	sprintf(com,"convert %s.ppm %s.png\n", baseName, baseName) ;
	system(com) ;
	sprintf(com,"/bin/rm -f %s.ppm\n",  baseName) ;
	system(com) ;
}

// dumps a PPM raw (P6) file on an already allocated memory array
void FrameSaver::DumpPPM(FILE *fp, int width, int height)
{
    const int maxVal=255;
    int y;
    
    fprintf(fp,	"P6 ");
    fprintf(fp,	"%d %d ", width, height);
    fprintf(fp,	"%d\n",	maxVal);

   	glReadBuffer(GL_FRONT) ; 
	for	( y = height-1;	y>=0; y-- ) 
	{
		glReadPixels(0,y,width,1,GL_RGB,GL_UNSIGNED_BYTE, 
			(GLvoid *) m_pixels);
		fwrite(m_pixels, 3, width, fp);
	}
}

