//
//  ClangGLSL.cpp
//  ClangGLSL
//
//  Created by Matthew Whiteside on 5/18/13.
//
//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include "GLSLClassGenerator.h"


using namespace clang;

namespace {
  
  
  
  
  class ClangGLSL : public ASTConsumer {
  private:
    
  //this is a stub
  public:
  };
  
  
  
  class ClangGLSLAction : public PluginASTAction {
  private:
    std::vector<string> baseShaderPaths;
    FileID _createInMemoryFile(StringRef FileName, const llvm::MemoryBuffer *Source,
                               SourceManager &Sources, FileManager &Files) {
      const FileEntry *Entry = Files.getVirtualFile(FileName == "-" ? "<stdin>" :
                                                    FileName,
                                                    Source->getBufferSize()+1, 0);
      Sources.overrideFileContents(Entry, Source, true);
      return Sources.createFileID(Entry, SourceLocation(), SrcMgr::C_User);
    }
    
    
    
  protected:
    ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
      return new ClangGLSL();
    }
    
    
    
    
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string>& args) {
      
      for (unsigned long i = 0, e = args.size(); i != e; ++i) {
        llvm::errs() << "ClangGLSL arg = " << args[i] << "\n";
        baseShaderPaths.push_back(args[i]);
        // Example error handling.
        if (args[i] == "-an-error") {
          DiagnosticsEngine &D = CI.getDiagnostics();
          unsigned DiagID = D.getCustomDiagID(
                                              DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
          D.Report(DiagID);
          return false;
        }
      }
      auto& srcMgr = CI.getSourceManager();
      auto& fileMgr = srcMgr.getFileManager();
      //auto file = fileMgr.getFile("main.cpp");
      //auto origBuffer = fileMgr.getBufferForFile("main.cpp");
      //auto contents = origBuffer->getBuffer();
      //auto newcontents = (contents + "//dogfood\n").str();
      //auto buff = llvm::MemoryBuffer::getMemBuffer(newcontents, "something",false);
      //srcMgr.overrideFileContents(file, buff);
      
      //time_t modTime;
      //FileManager::modifyFileEntry(file, newcontents.size(), modTime);
      //
      //srcMgr.overrideFileContents(file,
      //                          const FileEntry *NewFile);
      
      //void overrideFileContents(const FileEntry *SourceFile,
      //                          const llvm::MemoryBuffer *Buffer,
      //                          bool DoNotFree = false);
      
      for (unsigned i = 0; i < baseShaderPaths.size(); i++) {
        const string& baseShaderPath = baseShaderPaths[i];
        
        
        GLSLClassGenerator converter(baseShaderPath);
        auto tmp = converter.genShaderClassHeader();
        llvm::errs() << tmp << "\n\n";
        char* header = (char*)malloc((tmp.size()+1)*sizeof(char));
        for (unsigned i = 0; i < tmp.size(); i++) {
          header[i] = tmp[i];
        }
        header[tmp.size()] = '\0';
        
        
        _createInMemoryFile( baseShaderPath + "Shader.h",
                            llvm::MemoryBuffer::getMemBuffer(llvm::StringRef(header), "something",false),
                            srcMgr, fileMgr);
      }
      
      if (args.size() && args[0] == "help")
        PrintHelp(llvm::errs());
      
      return true;
    }
    void PrintHelp(llvm::raw_ostream& ros) {
      ros << "Help for PrintFunctionNames plugin goes here\n";
    }
    
    
    
  };
  
  
  
  
  
}

static FrontendPluginRegistry::Add<ClangGLSLAction>
X("clang-glsl", "generate cpp classes for OpenGL shaders");

