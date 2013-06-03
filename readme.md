### Build & Setup Instructions


1. Download & build clang/llvm from source: http://clang.llvm.org/get_started.html
2. Install this Xcode plugin, https://github.com/Jean-Daniel/clang-xcode, which will allow you to choose your freshly built clang as the compiler for Xcode to use.
3. Clone this repo.
4. Modify the implementation of GLSLClassGenerator::genShaderClassHeader to suit your needs.
5. Build your plugin.
6. You are now ready to use your plugin.  To do this, go to your other OpenGL project containing shader programs, make sure you select the clang executable you built in step 1 as the compiler, and then pass the following compiler flags when compiling a file that references the in-memory 'virtual classes' for your shader programs:

`-Xclang -load -Xclang /path/to/your/binary/libClangGLSL.dylib -Xclang -add-plugin -Xclang clang-glsl 
-Xclang -plugin-arg-clang-glsl -Xclang '/Users/matt/code/SomeOpenGLProj/shaders/FlatColor' 
-Xclang -plugin-arg-clang-glsl -Xclang '/Users/matt/code/SomeOpenGLProj/shaders/Wireframe'`