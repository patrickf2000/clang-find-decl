#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceManager.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  explicit FindNamedClassVisitor(ASTContext *Context)
    : Context(Context) {}
  
  //
  // Format: file,name,isStruct,isClass,isUnion,isEnum,
  //         isThisDeclarationADefinition,isCompleteDefinition,isBeingDefined,isEmbeddedInDeclarator,
  //         isFreeStanding,isDependentType
  //        \n
  //
  bool VisitTagDecl(TagDecl *Declaration) {
    SourceManager *sources = &Context->getSourceManager();
    FileID main_id = sources->getMainFileID();
    const FileEntry *entry = sources->getFileEntryForID(main_id);
    
    llvm::outs() << entry->getName() << ",";
    
    FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
    if (FullLocation.isValid()) llvm::outs() << FullLocation.getSpellingLineNumber() << ",";
    
    llvm::outs() << Declaration->getNameAsString() << ",";
    llvm::outs() << Declaration->isStruct() << ",";
    llvm::outs() << Declaration->isClass() << ",";
    llvm::outs() << Declaration->isUnion() << ",";
    llvm::outs() << Declaration->isEnum() << ",";
    llvm::outs() << Declaration->isThisDeclarationADefinition() << ",";
    llvm::outs() << Declaration->isCompleteDefinition() << ",";
    llvm::outs() << Declaration->isBeingDefined() << ",";
    llvm::outs() << Declaration->isEmbeddedInDeclarator() << ",";
    llvm::outs() << Declaration->isFreeStanding() << ",";
    llvm::outs() << Declaration->isDependentType();
    //llvm::outs() << Declaration->isThisDeclarationADemotedDefinition();
    llvm::outs() << "\n";
    
    //llvm::outs() << "Tag declaration: " << Declaration->getNameAsString() << "\n";
    return true;
  }

private:
  ASTContext *Context;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
  explicit FindNamedClassConsumer(ASTContext *Context)
    : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  FindNamedClassVisitor Visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<FindNamedClassConsumer>(&Compiler.getASTContext());
  }
};

static cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  return Tool.run(newFrontendActionFactory<FindNamedClassAction>().get());
}
