#!/bin/sh

ANTLR=/usr/local/bin/antlr
OUTPUT_DIR=${BUILD_DIR}/${BUILD_STYLE}/antlr
SRCDIR=${SRCROOT}/../librets/src

GRAMMAR=${SRCDIR}/rets-sql.g
DMQL_TREE=${SRCDIR}/dmql-tree.g
GET_OBJECT_TREE=${SRCDIR}/get-object-tree.g
TRIGGER=${OUTPUT_DIR}/up-to-date

#if [ ${GRAMMAR} -nt ${TRIGGER} ] ||
#    [ ${TREE} -nt ${TRIGGER} ]
#then
    ${ANTLR} -o $OUTPUT_DIR ${GRAMMAR}
    ${ANTLR} -o $OUTPUT_DIR -glib ${GRAMMAR} ${DMQL_TREE}
    ${ANTLR} -o $OUTPUT_DIR -glib ${GRAMMAR} ${GET_OBJECT_TREE}
    touch ${OUTPUT_DIR}/*.cpp
    touch ${OUTPUT_DIR}/*.hpp
    touch ${OUTPUT_DIR}/*.txt
    touch ${TRIGGER}
#fi