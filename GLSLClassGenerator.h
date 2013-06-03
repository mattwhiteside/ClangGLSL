//
//  ShaderClassGenerator.h
//  ClangGLSL
//
//  Created by Matthew Whiteside on 5/27/13.
//
//

#ifndef __ClangGLSL__ShaderClassGenerator__
#define __ClangGLSL__ShaderClassGenerator__

#include <iostream>
#include <vector>
#include <map>
#include <OpenGL/gl3.h>
#include "ShaderCompilers/ShaderCompiler.h"

using namespace std;

class GLSLClassGenerator : public ShaderCompiler{
  void extractUniforms();
protected:
  vector<string> uniformNames, uniformTypes;


  
public:
  GLSLClassGenerator(const string& baseShaderPath);
  virtual const string genShaderClassHeader();
  virtual const string genShaderClassImpl();
};

#endif /* defined(__ClangGLSL__ShaderClassGenerator__) */
