#ifndef __SEMANTIC_H
#define __SEMANTIC_H

#include "symbol_table.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Program(Tnode *node);
void ExtDefList(Tnode *node);
void ExtDef(Tnode *node);
Type Specifier(Tnode *node);
void ExtDecList(Tnode *node, Type type);
enum BASIC_TYPE TYPE(Tnode *node);
// FieldList StructSpecifier(Tnode *node, Type type);
FieldList VarDec(Tnode *node, Type type, enum VarDec_flag flag);
void DefList(Tnode *node);
void Def(Tnode *node);
void DecList(Tnode *node, Type type);
void Dec(Tnode *node, Type type);
void FunDec(Tnode *node, Type rtnType);
void VarList(Tnode *node, Type type);
void ParamDec(Tnode *node, Type type);
void CompSt(Tnode *node);
void StmtList(Tnode *node);

#endif