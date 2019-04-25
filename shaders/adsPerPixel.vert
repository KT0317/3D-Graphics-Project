#version 400 core

// Implement ADS per vertex lighting with a single light

// Vertex Attributes
in vec4 VertexPosition ;
in vec4 VertexColour ;
in vec3 VertexNormal ;
in vec2 VertexTexCoord ;

// Output variables to be interpolated by the rasterizer
out vec2 TexCoord ;
out vec3 NormalInEyeCoords ;
out vec4 PosInEyeCoords ;
out vec4 VColour ;

out vec3 ReflectDir ;
out vec3 RefractDir ;

// Uniforms from main program
uniform float time;
uniform mat4 modelviewMat ;
uniform mat4 normalMat ;
uniform mat4 MVP ;

uniform int DrawSkyBox ;
uniform int useLighting ;
uniform int CubeMapLighting ;
uniform float eta ;

void main(void)
{
    if( DrawSkyBox == 1) {
        ReflectDir = VertexPosition.xyz;
	}else{
		VColour = VertexColour ; 
		
		PosInEyeCoords = modelviewMat*VertexPosition ;
		NormalInEyeCoords = normalize(vec3(normalMat*vec4(VertexNormal,0.0))) ;
		
	   
		// sent out the vertex coord
		TexCoord = VertexTexCoord ;
		
		// eta is set to 0.8
		ReflectDir = reflect(-VertexPosition.xyz, VertexNormal);
		RefractDir = refract(-VertexPosition.xyz, VertexNormal, eta);
	}
    
    // Transform the vertex position from object to clip coordinates
	gl_Position = MVP * VertexPosition;
	
}

