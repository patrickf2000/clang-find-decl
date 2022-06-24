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
  //         isThisDeclarationADefinition,isCompleteDefinition,isEmbeddedInDeclarator,
  //         isFreeStanding
  //        ----- ENUM -----
  //        isScoped,isScopedUsingClassTag,isFixed,isComplete,isClosed,isClosedFlag,isClosedNonFlag
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
    llvm::outs() << Declaration->isEmbeddedInDeclarator() << ",";
    llvm::outs() << Declaration->isFreeStanding() << ",";
    llvm::outs() << Declaration->hasNameForLinkage() << ",";
    
    if (Declaration->isEnum()) {
        EnumDecl *Dec2 = static_cast<EnumDecl *>(Declaration);
        llvm::outs() << Dec2->isScoped() << ",";
        llvm::outs() << Dec2->isScopedUsingClassTag() << ",";
        llvm::outs() << Dec2->isFixed() << ",";
        llvm::outs() << Dec2->isComplete() << ",";
        llvm::outs() << Dec2->isClosed() << ",";
        llvm::outs() << Dec2->isClosedFlag() << ",";
        llvm::outs() << Dec2->isClosedNonFlag();
    } else {
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ";
    }
    
    // If it's not an enum, it's probably anything else
    // isAnonymousStructOrUnion,isNonTrivialToPrimitiveDefaultInitialize,
    // isNonTrivialToPrimitiveCopy,isNonTrivialToPrimitiveDestroy,
    // isParamDestroyedInCallee,isInjectedClassName,isLambda,isCapturedRecord,
    // hasObjectMember,hasVolatileMember,hasLoadedFieldsFromExternalStorage,hasNonTrivialToPrimitiveDefaultInitializeCUnion,
    // hasNonTrivialToPrimitiveDestructCUnion,hasNonTrivialToPrimitiveCopyCUnion,canPassInRegisters,field_empty
    if (!Declaration->isEnum()) {
        RecordDecl *record = static_cast<RecordDecl *>(Declaration);
        llvm::outs() << record->isAnonymousStructOrUnion() << ",";
        llvm::outs() << record->isNonTrivialToPrimitiveDefaultInitialize() << ",";
        llvm::outs() << record->isNonTrivialToPrimitiveCopy() << ",";
        llvm::outs() << record->isNonTrivialToPrimitiveDestroy() << ",";
        llvm::outs() << record->isParamDestroyedInCallee() << ",";
        llvm::outs() << record->isInjectedClassName() << ",";
        llvm::outs() << record->isLambda() << ",";
        llvm::outs() << record->isCapturedRecord() << ",";
        llvm::outs() << record->hasObjectMember() << ",";
        llvm::outs() << record->hasVolatileMember() << ",";
        llvm::outs() << record->hasLoadedFieldsFromExternalStorage() << ",";
        llvm::outs() << record->hasNonTrivialToPrimitiveDefaultInitializeCUnion() << ",";
        llvm::outs() << record->hasNonTrivialToPrimitiveDestructCUnion() << ",";
        llvm::outs() << record->hasNonTrivialToPrimitiveCopyCUnion() << ",";
        llvm::outs() << record->canPassInRegisters() << ",";
        llvm::outs() << record->field_empty();
    } else {
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ,";
        llvm::outs() << " ";
    }
    
    llvm::outs() << "\n";
    
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
