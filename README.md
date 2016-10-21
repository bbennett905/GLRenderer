# Summary #

A work-in-progress OpenGL renderer/engine, partially based on code in and using assets from [LearnOpenGL](http://learnopengl.com/). Currently can take any model format supported by ASSIMP.


# What you need #

All listed files are included in /include/ or /lib/ directory.
### Requires ###

* [Assimp](http://www.assimp.org/) ([License](http://www.assimp.org/main_license.html)) - Model loader

* [GLM](http://glm.g-truc.net/) ([License](http://glm.g-truc.net/copying.txt)) - Matrix header-only library, intended for OpenGL use

* [GLEW](http://glew.sourceforge.net/) ([License](https://github.com/nigels-com/glew#copyright-and-licensing)) - OpenGL Extension library

* [SOIL](http://www.lonesock.net/soil.html) ([License](http://www.lonesock.net/soil.html)) - Image loader

* [SDL2](https://www.libsdl.org/) ([License](https://www.libsdl.org/license.php)) - Window management and input (replaced GLFW3 in this project)

### Recommended ###

* [GLIntercept](https://github.com/dtrebilco/glintercept) ([License](https://github.com/dtrebilco/glintercept/blob/master/Docs/license.txt)) - Extremely useful for debugging OpenGL errors and memory leaks (can be found in /glintercept/ directory)

* [VLD](https://vld.codeplex.com/) ([License](https://vld.codeplex.com/SourceControl/latest#COPYING.txt)) - Automatically identifies memory leaks on application shutdown (not included)


# More Information #
### Shaders ###

This uses typical GLSL shaders (currently no geometry shader support) with a few extras used for dynamically creating the shaders as needed by the renderer:

* *#insert version* - Inserts the GLSL macro *#version TYPE* which is specified by the application. Use this on the first line of any shader file.

* *#insert num_point_lights* - Inserts the number of point lights in the scene.

* *#insert num_spot_lights* - Inserts the number of spot lights in the scene.

* *MAX_MATERIALS* - Defined in Shader.h, sets the max materials that can be used for a single drawable object


### ToDo List ###

* 2D drawing using SDL (for UI)

* Improved memory management system

* Create more primitives (possibly generating vertices & indices)

* Load materials/scenes from file

* More shaders and shader naming changes

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