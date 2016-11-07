# Summary #

My pet project over the last few months, a work-in-progress OpenGL renderer/engine, partially based on code in and using assets from [LearnOpenGL](http://learnopengl.com/).

## Features ##

* Lighting using Blinn-Phong model
* 3 types of light objects - point, spot, directional
* Texture loading using SOIL, including cubemap texture support
* Skyboxes
* Dynamic shader creation to create shaders tailored to a specific object in a scene
* Window and input abstraction layers over SDL
* Model loading using ASSIMP
* Simple profiling tool
* Logging tools, supporting logging to file, to console, and to screen as a UI element
* (Fairly) straightforward base classes for any entity/object, drawable object, UI elements to inherit from

# What you need #

### Requires ###
All listed files are included in /include/ or /lib/ directory.

* [Assimp](http://www.assimp.org/) ([License](http://www.assimp.org/main_license.html)) - Model loader
* [GLM](http://glm.g-truc.net/) ([License](http://glm.g-truc.net/copying.txt)) - Matrix header-only library, intended for OpenGL use
* [GLEW](http://glew.sourceforge.net/) ([License](https://github.com/nigels-com/glew#copyright-and-licensing)) - OpenGL Extension library
* [SOIL](http://www.lonesock.net/soil.html) ([License](http://www.lonesock.net/soil.html)) - Image loader
* [SDL2](https://www.libsdl.org/) ([License](https://www.libsdl.org/license.php)) - Window management and input, also SDL2_ttf (replaced GLFW3 in this project)

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

Suffix *_unlit* on shaders means that it doesn't support lighting


### Planned Features / ToDo List ###

* Normal map support
* Optimizations - using UBOs for Draw methods, load time optimizations, etc.
* Add shadows support
* Improved memory management system
* Create more primitives (possibly generating vertices & indices)
* Load materials/scenes from file
* See in-file TODOs for others...
* And hopefully lots more, later on

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