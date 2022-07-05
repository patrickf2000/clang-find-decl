#!/bin/bash

echo "Running tests..."

# Generate test/compile_commands.json
JSON=test/compile_commands.json
echo "[" > $JSON
for f in test/{*.c,*.cpp}
do
    echo "{"  >> $JSON
    echo "\"directory\": \"`pwd`/test\","  >> $JSON
    if [[ ${f#*.} == "cpp" ]] ; then
        echo "\"command\": \"clang++ `pwd`/test/$f\","  >> $JSON
    else
        echo "\"command\": \"clang `pwd`/test/$f\","  >> $JSON
    fi
    echo "\"file\": \"`pwd`/test/$f\""  >> $JSON
    echo "},"  >> $JSON
done
echo "{" >> $JSON
    echo "\"directory\": \"`pwd`\"," >> $JSON
    echo "\"command\": \"clang `pwd`/test.c\","  >> $JSON
    echo "\"file\": \"`pwd`/test.c\""  >> $JSON
echo "}" >> $JSON
echo "]" >> $JSON

##
## Run the Clang test
##

# Setup the CSV
OUTPUT=output.csv
printf "file,line,name,isStruct,isClass,isUnion,isEnum," > $OUTPUT
printf "isThisDeclarationADefinition,isCompleteDefinition,isEmbeddedInDeclarator," >> $OUTPUT
printf "isFreeStanding,hasNameForLinkage," >> $OUTPUT
printf "(ENUM) isScoped,(ENUM) isScopedUsingClassTag,(ENUM) isFixed,(ENUM) isComplete," >> $OUTPUT
printf "(ENUM) isClosed,(ENUM) isClosedFlag,(ENUM) isClosedNonFlag," >> $OUTPUT
printf "(RECORD) isAnonymousStructOrUnion,(RECORD) isNonTrivialToPrimitiveDefaultInitialize," >> $OUTPUT
printf "(RECORD) isNonTrivialToPrimitiveCopy,(RECORD) isNonTrivialToPrimitiveDestroy," >> $OUTPUT
printf "(RECORD) isParamDestroyedInCallee,(RECORD) isInjectedClassName,(RECORD) isLambda,(RECORD) isCapturedRecord," >> $OUTPUT
printf "(RECORD) hasObjectMember,(RECORD) hasVolatileMember,(RECORD) hasLoadedFieldsFromExternalStorage,(RECORD) hasNonTrivialToPrimitiveDefaultInitializeCUnion," >> $OUTPUT
printf "(RECORD) hasNonTrivialToPrimitiveDestructCUnion,(RECORD) hasNonTrivialToPrimitiveCopyCUnion,(RECORD) canPassInRegisters,(RECORD) field_empty" >> $OUTPUT
echo "" >> $OUTPUT

# The loop
for f in test/{*.c,*.cpp}
do
    echo ""
    echo $f
    
    build/find-decls $f >> $OUTPUT
done

##
## Run the Rose tests
##
OUTPUT=rose_output.csv
printf "file,name,type," > $OUTPUT
printf "isNameOnly,hasAssociatedSymbol,get_nameOnly," >> $OUTPUT
printf "hasExplicitScope,get_type_elaboration_required,get_global_qualification_required," >> $OUTPUT
printf "get_embedded,get_isUnNamed,get_isAutonomousDeclaration,get_isScopedEnum," >> $OUTPUT
printf "hasExplicitScope,get_type_elaboration_required,get_global_qualification_required," >> $OUTPUT
printf "isSpecialization,isPartialSpecialization,get_from_template,get_fixupScope,get_isUnNamed," >> $OUTPUT
printf "get_explicit_annotation_interface,get_explicit_interface,get_explicit_enum," >> $OUTPUT
printf "get_explicit_anonymous,get_isAutonomousDeclaration,isForward," >> $OUTPUT
echo "" >> $OUTPUT

# The loop
for f in test/{*.c,*.cpp}
do
    echo ""
    echo $f
    
    #build/rose-find-decls $f >> $OUTPUT
    build/rose-find-decls $f >> /dev/null
    if [[ $? == 0 ]] ; then
        build/rose-find-decls $f >> $OUTPUT
    fi
done

echo ""
echo "Done"

