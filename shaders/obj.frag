#version 400 core

uniform float time ;
uniform sampler2D RenderTex ;
uniform samplerCube cubemap ;

uniform int useLighting ;
uniform int DrawSkyBox ;
uniform int useRenderTexture;

out vec4 fragColour ;

in vec4 GPosition ;
in vec3 GNormal ;
in vec2 GTexCoord ;
in vec3 GColour;
in vec3 GReflectDir;
flat in int gl_ViewportIndex ;


struct LightInfo {
    vec4 Position ; // in eye coordinates
    vec3 Ia ; // ambient intensity
    vec3 Id ; // diffuse intensity
    vec3 Is ; // specular intensity
} ;

uniform LightInfo Light ;

struct MaterialInfo {
    vec3 Ka ; // ambient intensity
    vec3 Kd ; // diffuse intensity
    vec3 Ks ; // specular intensity
    float Shininess ;
} ;

uniform MaterialInfo Material ;

// gl_FrontFacing returns true both for gl_FrontFacing and
// !gl_FrontFacing by some drives when accessed multiple times
// in the same shader
// The following code seems to work though


bool isFrontFacing(void) {
    vec2 f = vec2(gl_FrontFacing,0.0) ;
    if( f.x == 1.0) {
        return true ;
    }
    else {
        return false ;
    }
}


void ads(vec4 position, vec3 N, out vec3 ambientDiffuse, out vec3 specular)
{
    vec3 L = vec3(normalize(Light.Position-position)) ;
    vec3 V = normalize(-position.xyz) ;
    vec3 R = normalize(reflect(-L, N)) ;
    
    vec3 ambient = Light.Ia * Material.Ka;
    float LdotN = max(dot(N, L),0) ;
    vec3 diffuse = Light.Id*Material.Kd*LdotN ;
    ambientDiffuse = ambient+diffuse ;

    specular = vec3(0.0) ;
    if( LdotN > 0.0)
      specular = Light.Is*Material.Ks*pow(max(dot(R,V), 0.0), Material.Shininess) ;
}


void main(void)
{
    vec3 AmbientDiffuseIntensity ;
    vec3 SpecularIntensity ;
	vec4 objectColour = vec4(GColour,1.0);
	
	vec4 skyboxcolourReflect = texture(cubemap, GReflectDir);
	vec4 renderTexColour = texture(RenderTex, GTexCoord);
   
    if( gl_FrontFacing) {
        ads(GPosition, GNormal, AmbientDiffuseIntensity, SpecularIntensity) ;
    }
    else {
        ads(GPosition, -GNormal, AmbientDiffuseIntensity, SpecularIntensity) ;
    }
 
	if(DrawSkyBox == 1)
		fragColour = skyboxcolourReflect;
	else if (useRenderTexture == 1)
		fragColour = renderTexColour*vec4(AmbientDiffuseIntensity, 1.f)+vec4(SpecularIntensity,0.0f);
	else{
		if( useLighting == 1 ) fragColour = objectColour*vec4(AmbientDiffuseIntensity, 1.f)+vec4(SpecularIntensity,0.0f) ;
		else fragColour = objectColour*vec4(Material.Kd,1.0f) ;
	}
	
}

