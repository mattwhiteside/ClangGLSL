//
//  ShaderClassGenerator.cpp
//  ClangGLSL
//
//  Created by Matthew Whiteside on 5/27/13.
//
//

#include "GLSLClassGenerator.h"
#include "glfw/include/GLFW/glfw3.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
////need to figure this out for linux
#endif

GLSLClassGenerator::GLSLClassGenerator(const string& basePath) : ShaderCompiler(basePath){}


void GLSLClassGenerator::extractUniforms(){
  vector<string> shaderSrcs;
  const char* extensions[] = {".vsh",".gsh",".fsh"};
  for (unsigned i = 0; i < 3; i++) {
    using namespace std;
    const string filename = baseShaderPath + extensions[i];
    ifstream file(filename.c_str());
    stringstream buffer;
    buffer << file.rdbuf();
    shaderSrcs.push_back(buffer.str());
  }
  
//  If you wanted to use cocoa to create the opengl context...
//  but this has the drawback that the plugin has to be compiled as Objective-C++

  
//  NSOpenGLPixelFormatAttribute attrs[] =
//  {
//    NSOpenGLPFADoubleBuffer,
//    NSOpenGLPFADepthSize, 24,
//    // Must specify the 3.2 Core Profile to use OpenGL 3.2
//    NSOpenGLPFAOpenGLProfile,
//    NSOpenGLProfileVersion3_2Core,
//    0
//  };
//
//  NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
//  NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
//  [context makeCurrentContext];


  
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }
//


  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.1
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  glfwWindowHint(GLFW_FOCUSED, GL_FALSE);
  glfwWindowHint(GLFW_ICONIFIED, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window;


  window = glfwCreateWindow( 300, 300, "Gears", NULL, NULL );
  if (!window)
  {
    fprintf( stderr, "Failed to open GLFW window\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  glfwMakeContextCurrent(window);

  cout << "Vendor:   " <<  glGetString(GL_VENDOR) << endl;
  cout << "Renderer: " <<  glGetString(GL_RENDERER) << endl;
  cout << "Version:  " <<  glGetString(GL_VERSION) << endl;
  cout << "GLSL:     " <<  glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  
  
  GLuint program = glCreateProgram();
  if (program == 0) {
    cout << "Failed to create shader program." << endl;
    exit(1);
  }

                      
  
  if (!compile(&program, false)) {
    cout << "Failed to compile shader program" << endl;
    exit(1);
  }
  
  GLint nUniforms, maxLen;
  glGetProgramiv( program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                 &maxLen);

  glGetProgramiv( program, GL_ACTIVE_UNIFORMS,
                 &nUniforms);

  GLchar * name = (GLchar *) malloc( maxLen );
  GLint size;
  GLsizei written;
  GLenum type;
  for(int i = 0; i < nUniforms; i++) {
    glGetActiveUniform( program, i, maxLen, &written, &size, &type, name );
    //printf(" %s\n", name);
    string _name(name);
    size_t index = _name.find(".");
    if (index != string::npos) {
      _name.replace(index, 1, "");
      _name.insert(index, "__");
    }
    uniformNames.push_back(_name);

    switch (type) {
      case GL_FLOAT_VEC3:
        uniformTypes.push_back("Vector<3>");
        break;
      case GL_FLOAT_VEC4:
        uniformTypes.push_back("Vector<4>");
        break;
      case GL_FLOAT_MAT3:
        uniformTypes.push_back("Matrix<3>");
        break;
      case GL_FLOAT_MAT4:
        uniformTypes.push_back("Matrix<4>");
        break;
      case GL_FLOAT:
        uniformTypes.push_back("float");
        break;
      case GL_BOOL:
        uniformTypes.push_back("bool");
        break;
      default:
        break;
    }
    
  }
  free(name);
  //free(window);
  glfwTerminate();

//  if using cocoa...
//  [context release];
//  [pf release];

}


const std::string GLSLClassGenerator::genShaderClassHeader(){
  const string baseShaderName = getName() + "Shader";
  
  extractUniforms();
  std::stringstream ss;
  
  //the contents of the GLSL Wrapper class header file should be generated here;
  //this is really going to be specific to your framework and needs
  
  return ss.str();
  
}

const std::string GLSLClassGenerator::genShaderClassImpl(){
  const string baseShaderName = getName();
  
  stringstream implBuff;
  //this is not currently used;
  // the whole implementation goes in the header file, since that is
  // seemingly the only way to make this work, without delving deeper
  // into the AST
  return implBuff.str();
}
