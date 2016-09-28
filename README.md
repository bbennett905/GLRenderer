# Summary #

A work-in-progress OpenGL renderer, partially based on code in and using assets from [LearnOpenGL](http://learnopengl.com/). Can take any model format supported by ASSIMP.


# What you need #

All listed files are included in /include/ or /lib/ directory.
### Requires ###

* [Assimp](http://www.assimp.org/) - Model loader

* [GLM](http://glm.g-truc.net/) - Matrix header-only library, intended for OpenGL use

* [GLFW3](http://www.glfw.org/) - Window and input library, intended for OpenGL use

* [GLEW](http://glew.sourceforge.net/) - OpenGL Extension library

* [SOIL](http://www.lonesock.net/soil.html) - Image loader

### Recommended ###

* [GLIntercept](https://github.com/dtrebilco/glintercept) - Extremely useful for debugging OpenGL errors (can be found in /glintercept/ directory)


# More Information #
### Shaders ###

This uses typical GLSL shaders (currently no geometry shader support) with a few extras used for dynamically creating the shaders as needed by the renderer:

* *#insert version* - Inserts the GLSL macro *#version TYPE* which is specified by the application. Use this on the first line of any shader file.

* *#insert num_point_lights* - Inserts the number of point lights in the scene.

* *#insert num_spot_lights* - Inserts the number of spot lights in the scene.

* *#insert num_materials* - Inserts the number of materials for the object being rendered.

Suffix of *_nomat* on a shader file name means that it does not take materials; *_unlit* means no lights.


### ToDo List ###

* Fix FPS counter

* Remove unnecessary includes, use forward declaration

* Check that everything is being properly destroyed

* Create more primitives, possibly generating vertices & indices

* Input abstraction

* Load materials from file

* More shaders and shader naming changes

* Add more advanced OpenGL things, such as FBOs, geometry shaders, etc.

* See in-file TODOs for others...