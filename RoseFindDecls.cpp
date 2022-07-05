#include <iostream>
#include <string>

#include "rose.h"

//
// Does the actual traversing
//
class InnerStructSearch : public AstSimpleProcessing {
public:
    InnerStructSearch(std::string fileName) { this->fileName = fileName; }
    void outputDeclaration(SgDeclarationStatement *stmt);
    virtual void visit(SgNode *n);
private:
    std::string fileName = "";
};

void InnerStructSearch::outputDeclaration(SgDeclarationStatement *st) {
    std::cout << st->isNameOnly() << ",";
    std::cout << st->hasAssociatedSymbol() << ",";
    std::cout << st->get_nameOnly() << ",";
}

// Output syntax:
// file, name
// SgDeclarationStatement
// SgClassDeclaration
// SgEnumDeclaration
//
void InnerStructSearch::visit(SgNode *n) {
    switch (n->variantT()) {
        //
        // Enums
        //
        case V_SgEnumDeclaration: {
            SgEnumDeclaration *en = static_cast<SgEnumDeclaration *>(n);
            
            // Output everything
            std::cout << fileName << "," << en->get_name() << ",";      // File name, record name
            std::cout << "ENUM,";
            outputDeclaration(en);
            for (int i = 0; i<14; i++) std::cout << ",";                    // Generate SgClass buffer
            std::cout << en->hasExplicitScope() << ",";                     // 1
            std::cout << en->get_type_elaboration_required() << ",";        // 2
            std::cout << en->get_global_qualification_required() << ",";    // 3
            std::cout << en->get_embedded() << ",";                         // 4
            std::cout << en->get_isUnNamed() << ",";                        // 5
            std::cout << en->get_isAutonomousDeclaration() << ",";          // 6
            std::cout << en->get_isScopedEnum() << ",";                     // 7
            std::cout << std::endl;
        } break;
        
        //
        // Structures, unions, classes, etc
        //
        case V_SgClassDeclaration: {
            SgClassDeclaration *st = static_cast<SgClassDeclaration *>(n);
            
            // Output everything
            std::cout << fileName << "," << st->get_name() << ",";      // File name, record name   // 1,2
            std::cout << "CLASS,";
            outputDeclaration(st);
            std::cout << st->hasExplicitScope() << ",";                     // 1
            std::cout << st->get_type_elaboration_required() << ",";        // 2
            std::cout << st->get_global_qualification_required() << ",";    // 3
            std::cout << st->isSpecialization() << ",";                     // 4
            std::cout << st->isPartialSpecialization() << ",";              // 5
            std::cout << st->get_from_template() << ",";                    // 6
            std::cout << st->get_fixupScope() << ",";                       // 7
            std::cout << st->get_isUnNamed() << ",";                        // 8
            std::cout << st->get_explicit_annotation_interface() << ",";    // 9
            std::cout << st->get_explicit_interface() << ",";               // 10
            std::cout << st->get_explicit_enum() << ",";                    // 11
            std::cout << st->get_explicit_anonymous() << ",";               // 12
            std::cout << st->get_isAutonomousDeclaration() << ",";          // 13
            std::cout << st->isForward() << ",";                            // 14
            for (int i = 0; i<7; i++) std::cout << ",";                     // Generate SgEnum buffer
            std::cout << std::endl;
        } break;
        
        default: {}
    }
}

//
// In charge of finding the right file to traverse
//
class StructSearch : public AstSimpleProcessing {
public:
    virtual void visit(SgNode *n);
};

void StructSearch::visit(SgNode *n) {
    switch (n->variantT()) {
        case V_SgSourceFile: {
            SgFile *file = isSgFile(n);
           // std::cout << "FILE: " << file->getFileName() << std::endl;
            InnerStructSearch s2(file->getFileName());
            s2.traverseWithinFile(file, preorder);
        } break;
        
        default: {}
    }
}

int main(int argc, char *argv[]) {
    ROSE_INITIALIZE;
    
    SgProject *project = frontend(argc, argv);
    if (project == nullptr) return 1;
    //std::cout << "------------------------------" << std::endl;
    //SageInterface::printAST(project);
    //std::cout << "==============================" << std::endl;
    
    StructSearch search;
    search.traverseInputFiles(project, preorder);
    
    return 0;
}

