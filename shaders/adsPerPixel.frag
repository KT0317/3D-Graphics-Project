#version 400 core

uniform sampler2D texture1 ;
uniform sampler2D texture2 ;
uniform sampler2D texture3 ;

uniform samplerCube skybox ;
uniform samplerCube skyboxSpecular ;
uniform samplerCube skyboxDiffuse ;

uniform int DrawSkyBox ;
uniform int useCubeMap ;
uniform int CubeMapADS ;
uniform float BlendFactor;
uniform int CubeMapLighting ;		// used for diffuse cubemap and specular cubemap
uniform float eta ;

uniform float time ;
uniform int useTex ;

// Use ADS lighting
uniform int useLighting ;
uniform int WithADS ;

out vec4 fragColour ;


in vec4 VColour ;
in vec2 TexCoord ;
in vec3 NormalInEyeCoords ;
in vec4 PosInEyeCoords ;

in vec3 ReflectDir ;
in vec3 RefractDir ;


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
	vec4 skyboxcolourReflect = texture(skybox, ReflectDir);
	vec4 skyboxcolourSpecularReflect = texture(skyboxSpecular, ReflectDir);
	vec4 skyboxcolourDiffuseReflect = texture(skyboxDiffuse, ReflectDir);
	vec4 reflectColour;
	
	vec4 skyboxcolourRefract = texture(skybox, RefractDir);
	vec4 skyboxcolourSpecularRefract = texture(skyboxSpecular, RefractDir);
	vec4 skyboxcolourDiffuseRefract = texture(skyboxDiffuse, RefractDir);
	vec4 refractColour;

    vec4 texColour1 = texture(texture1,TexCoord);
    vec4 texColour2 = texture(texture2,TexCoord);
    vec4 texColour3 = texture(texture3,TexCoord);
    vec4 texColour ;
    
    vec3 AmbientDiffuseIntensity ;
    vec3 SpecularIntensity ;
    
    
    if( isFrontFacing()) {
        ads(PosInEyeCoords, NormalInEyeCoords, AmbientDiffuseIntensity, SpecularIntensity) ;
    }
    else {
        ads(PosInEyeCoords, -NormalInEyeCoords, AmbientDiffuseIntensity, SpecularIntensity) ;
    }
    switch( useTex ) {
        case 1: texColour = texColour1 ; break ;
        case 2: texColour = texColour2 ; break ;
        case 3: texColour = texColour3 ; break ;
        case 12: texColour = texColour1*texColour2 ; break ;
        case 13: texColour = texColour1*texColour3 ; break ;
        case 23: texColour = texColour2*texColour3 ; break ;
        case 123: texColour = texColour1*texColour2*texColour3 ; break ;
		case 100: texColour = VColour ;
        default: texColour = vec4(1.f) ; break;
    } ;

 
	if(DrawSkyBox == 1){
		fragColour = skyboxcolourReflect;
	}
	
	// Sphere B: cubemap reflection and ADS lighting
	else if (CubeMapADS == 1){
	
		if (WithADS == 1){
			// If blend factor is .7 or more then if will take 70% of the second variable
			// currently 0.5
			fragColour = mix(vec4(AmbientDiffuseIntensity,1.0) ,skyboxcolourReflect,  BlendFactor ) +vec4(SpecularIntensity,0.0f) ;
		}
		else fragColour = skyboxcolourReflect;
		
	}
	
	// Sphere C: cubemap for each reflect, diffuse and specular component
	else if (useCubeMap == 1 ){
	
		//Diffuse only
		if(CubeMapLighting == 1){
			reflectColour = mix(skyboxcolourDiffuseReflect ,skyboxcolourReflect,  BlendFactor );
			refractColour = mix(skyboxcolourDiffuseRefract ,skyboxcolourRefract,  BlendFactor );
			fragColour = mix(reflectColour, refractColour, BlendFactor);
		}
		
		//Specular only
		else if(CubeMapLighting == 2){
			reflectColour = skyboxcolourReflect + skyboxcolourSpecularReflect;
			refractColour = skyboxcolourRefract + skyboxcolourSpecularRefract; 
			fragColour = mix(reflectColour, refractColour, BlendFactor);
		}
		
		// Both diffuse and specular
		else if(CubeMapLighting == 12){
			reflectColour = mix(skyboxcolourDiffuseReflect ,skyboxcolourReflect,  BlendFactor) + skyboxcolourSpecularReflect;
			refractColour = mix(skyboxcolourDiffuseRefract ,skyboxcolourRefract,  BlendFactor) + skyboxcolourSpecularRefract;
			fragColour = mix(reflectColour, refractColour, BlendFactor);
		}
		
		// Neither
		else	fragColour = mix(skyboxcolourRefract, skyboxcolourReflect, BlendFactor);

	}
	
	// Sphere A:
	// used for anything that is not using a cube map
	else{
		if( useLighting == 1 ) fragColour = texColour*vec4(AmbientDiffuseIntensity,1.0)+vec4(SpecularIntensity,0.0f) ;
		else fragColour = texColour*vec4(Material.Kd,1.f) ;
	}
	
}

