#version 400 core

// Implement OBJs with ADS per pixel lighting with a single light

// Vertex Attributes
layout(location = 0)in vec3 VertexPosition ;
layout(location = 1)in vec3 VertexNormal ;
layout(location = 2)in vec2 VertexTexCoord ;

// Output variables to be sent to Geometry shader
out vec4 VPosition ;
out vec3 VNormal ;
out vec2 VTexCoord ;
out vec3 VReflectDir ;

// Uniforms from main program
uniform float time;
uniform mat4 modelviewMat ;
uniform mat4 normalMat ;
uniform mat4 MVP ;

uniform int DrawSkyBox ;

void main(void)
{
	// sky box position
	if( DrawSkyBox == 1)
        VReflectDir = VertexPosition.xyz;
		
    //Position in object coordinate and normal in eye coordinate
	VPosition = vec4(VertexPosition, 1.0) ;
	VNormal = normalize(vec3(normalMat*vec4(VertexNormal,0.0)));
	VTexCoord = VertexTexCoord ;
	
    // Transform the vertex position from object to clipping coordinates
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	
}

