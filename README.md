# opengl-terrain-demo
Modern OpenGL terrain rendering for learning purposes. Dependencies: Assimp, DevIL, glew, glfw, glm.

### Controls:

O, P -> Terrain height scale

K, L -> Target screen space triangle area (control tesselation)

G -> Toggle wireframe

F -> Toggle fog


1, 2, 3 -> Choose terrain (heightmap and normalmap)

0 -> Choose texturing mode (height/slope based and slope-only based)

Left Alt -> Toggle vsync

### [Youtube Video](https://www.youtube.com/watch?v=jADa3OFTLCQ)

### [Windows Executable](http://www.bmlourenco.com/downloads/opengl-terrain-demo.zip)
Requires driver support for OpenGL 4.5.

# Screenshots
![](http://bmlourenco.com/public/images/opengl-terrain-demo/1.JPG)
![](http://bmlourenco.com/public/images/opengl-terrain-demo/4.JPG)
![](http://bmlourenco.com/public/images/opengl-terrain-demo/5.JPG)

#Details:
- Developed with C++, OpenGL and GLSL 4.5.
- Base framework abstracting and simplifying the use of OpenGL objects: buffers, textures, VAOs, FBOs and shaders. Abstracts mesh and texture loading. Also helps with the management of scene objects and their respective meshes and materials.
- The terrain is defined through a height map and a normal map.
- Terrain has adaptative hardware tessellation (trying to generate triangles with the same screen-space area). Based on nVidia's Paper DirectX 11 Terrain Tesselation.
- Multitexturing based on two criteria: altitude and slope.
- GPU frustum culling is applied to the terrain on a patch level, on the tesselation control shader.
- Single pass wireframe is implemented using nVidia's technique described on Solid Wireframe.
- A basic shadow mapping algorithm is used to cast shadows on the terrain. Some percentage closer filtering (PCF) is applied to soften the aliased edges.
- Fog is generated on the fragment shader as a function of distance to camera and altitude.

# License
Licensed under the MIT license, see [LICENSE](https://github.com/MadEqua/opengl-terrain-demo/blob/master/LICENSE) for details.
