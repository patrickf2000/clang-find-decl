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

echo ""
echo "Done"

