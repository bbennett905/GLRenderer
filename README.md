# Description #

A work-in-progress OpenGL renderer, partially based on code and using assets from [LearnOpenGL](http://learnopengl.com/). Can take any model format supported by ASSIMP.


# Requires: #

* [Assimp](http://www.assimp.org/)

* [GLM](http://glm.g-truc.net/)

* [GLFW3](http://www.glfw.org/)

* [GLEW](http://glew.sourceforge.net/)

* [SOIL](http://www.lonesock.net/soil.html)


# Shaders #

This uses typical GLSL shaders (currently no geometry shader support) with a few extras used for dynamically creating the shaders as needed by the renderer:

* *#insert version* - Inserts the GLSL macro *#version TYPE* which is specified by the application. Use this on the first line of any shader file.

* *#insert num_point_lights* - Inserts the number of point lights in the scene.

* *#insert num_spot_lights* - Inserts the number of spot lights in the scene.

* *#insert num_materials* - Inserts the number of materials for the object being rendered.

Suffix of _nomat on a shader name means that it does not take materials; _unlit means no lights.