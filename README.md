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

* *MAX_MATERIALS* - Defined in Shader.h, sets the max materials that can be used for a single drawable object

Suffix of *_nomat* on a shader file name means that it does not take materials; *_unlit* means no lights.


### ToDo List ###

* Remove unnecessary includes, use forward declaration whenever possible

* Ensure that everything is being properly destroyed (hint: its not)

* Create more primitives (possibly generating vertices & indices)

* Switch from GLFW to SDL for window management

* Load materials/scenes from file

* More shaders and shader naming changes

* Implement transparency support

* Add logging system

* See in-file TODOs for others...

# License #

MIT License

Copyright (c) 2016 Brett Bennett

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.