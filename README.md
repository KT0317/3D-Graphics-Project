# 3D-Graphics-Project
Rendering visual effects using advanced features of OpenGL 4.x

This project is complied and ran in Visual Studios 2019 (also works with Visual Studios 2010 and up)
Programming language: C/C++ and OpenGL Shader Language (GLSL)

All the source files are located in the "Source" folder
The Visual Studios Solution file (cse3431Template.sln located in the cse3431Template folder) is the file you will be running
- You must have Visual Studios installed

If you are using an older version of VS:
- In Visual Studios under the Solution Explorer, Right-click "cse3431Template" and click on options
- Under Configuration Properties -> General -> Platform Toolset, change it to your version of Visual Studios


NOTE: The libraries are linked using absolute pathing; therefore, only working on my specific machine (instructions below)
NOTE: Relative pathing didn't work for libraries but worked for include files for some reason

Here are instructions on how to get it working on your machine:
- Right-click "cse3431Template" and click on options
- Under Configuration Properties -> Linker -> Additional Library Directories, click on the arrow drop down box (this should open a window)
	- click on the first item which should be "C:\Users\kevin\OneDrive\Desktop\cse4431_3D_Project\cse3431Template-glfw3-v22-3textures\opengl\glfw\lib-vc2019"
	- A button on the right of it with "..." written should appear; click it
	- Look in ..\opengl\glfw folder and choose lib-vc<your_vs_version_here>
- Once this is complete, right-click "Solution 'cse3431Template' (1 project)" and build your solution
- If there are no errors, run it on Visual Studios


While running:
- Press 'a' to start the animation
- Press 'v' to save the image (should be saved in the "frames" folder
- Press 't' to reset time and objects
- Press 'a' to toggle the animation
- Press 'm' to toggle frame dumping
- Press 'l/L' to turn lighting on/off
- Press '1-7' to use different textures
- Press 'p' to toggle between two different shaders
Default Shader = 0
- Press 'r/R' to illuminate/unilluminate sphere A with cube map reflection only (only when Shader = 0)
- Press 'd/D' to illuminate/unilluminate sphere B with image based diffuse lighting only (only when Shader = 0)
- Press 's/S' to illuminate/unilluminate sphere C with image based specular lighting only (only when Shader = 0)
- Press 't/T' to illuminate/unilluminate sphere C with a combination of diffuse and specular image based lighting. (only when Shader = 0)
- Press 'ESC' to quit


Bugs (currently working on it):
- Whichever shader runs first, runs perfectly but swapping to next shader messes up rendering 
	- possibly due to the fact that its drawing different objects
	- issue with gl_frontfacing when swapping
	- not drawing plane when swapping
	- You can edit the code to choose which shader to start with.
		- In drawScene.cpp, CTRL+F "useShader". You should see useShader(2) then useShader(0); swap the ordering 


What it's suppose to look like:
- image files located in ..\media folder labelled "Spheres.png" and "Donuts.png"
