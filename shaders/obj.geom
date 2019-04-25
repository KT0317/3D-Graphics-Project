#version 400 core

// For cube map arrays: 4 viewport * 6 cube map faces = 24 invocations
// If you are using layered rendering, remember to use gl_Layer
layout( triangles, invocations = 4 ) in;
layout( triangle_strip, max_vertices = 3 ) out;

in vec4 VPosition[] ;			// object coordinate system
in vec3 VNormal[] ;				// eye coordinate system
in vec2 VTexCoord[] ;
in vec3 VReflectDir[] ;

uniform mat4 modelviewMat;
uniform mat4 ProjectionMatrices[4];
uniform int donutIcing; 

out vec4 GPosition;
out vec3 GNormal;
out vec2 GTexCoord;
out vec3 GColour;
out vec3 GReflectDir ;

// out int gl_Layer;
out int gl_ViewportIndex ;

const vec3 diffViewpointColour[4] = vec3[4]
(
		vec3(0.637255/2, 0.260784/2, 0.460784/2),		// kinda brown colour
		vec3(1, 0, 0),
		vec3(0, 1, 0),
		vec3(0, 0, 1)
);

void main()
{	
	// Determine current viewports MVP 
	mat4 Current_MVP_Viewport = ProjectionMatrices[gl_InvocationID] * modelviewMat;

	//gl_Position is in clipping coordinate system
	vec4 p0 = Current_MVP_Viewport * VPosition[0] ;
	vec4 p1 = Current_MVP_Viewport * VPosition[1] ;
	vec4 p2 = Current_MVP_Viewport * VPosition[2] ;

	GColour = vec3(1.0f);
	
	
	if(donutIcing == 1)
	{
		if (VPosition[0].y >= 0.15){
			p0 = Current_MVP_Viewport * (VPosition[0] * vec4(1.05, 1.05, 1.05, 1.0));
			GColour = diffViewpointColour[gl_InvocationID];			
		}
		
		if (VPosition[1].y >= 0.15){
			p1 = Current_MVP_Viewport * (VPosition[1] * vec4(1.05, 1.05, 1.05, 1.0));
			GColour = diffViewpointColour[gl_InvocationID];
		}
		
		if (VPosition[2].y >= 0.15){
			p2 = Current_MVP_Viewport * (VPosition[2] * vec4(1.05, 1.05, 1.05, 1.0));
			GColour = diffViewpointColour[gl_InvocationID];
		}
	}
	
	GReflectDir  = VReflectDir[0];
	GTexCoord = VTexCoord[0];
	GNormal = VNormal[0];
	GPosition = modelviewMat*VPosition[0];
	gl_Position = p0;
	// gl_Layer = gl_InvocationID;
	gl_ViewportIndex = gl_InvocationID;
	EmitVertex();

	GReflectDir  = VReflectDir[1];
	GTexCoord = VTexCoord[1];
	GNormal = VNormal[1];
	GPosition = modelviewMat*VPosition[1];
	gl_Position = p1; 
	// gl_Layer = gl_InvocationID;
	gl_ViewportIndex = gl_InvocationID;
	EmitVertex();

	GReflectDir  = VReflectDir[2];
	GTexCoord = VTexCoord[2];
	GNormal = VNormal[2];
	GPosition = modelviewMat*VPosition[2];
	gl_Position = p2; 
	// gl_Layer = gl_InvocationID;
	gl_ViewportIndex = gl_InvocationID;
	EmitVertex();

	EndPrimitive();

}
