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
void StructSpecifier(Tnode *node, Type type);
FieldList VarDec(Tnode *node, Type type, enum VarDec_flag flag);
void DefList(Tnode *node, Type struc_type);
void Def(Tnode *node, Type struc_type);
void DecList(Tnode *node, Type type, Type struc_type);
void Dec(Tnode *node, Type type, Type struc_type);
void FunDec(Tnode *node, Type rtnType);
void VarList(Tnode *node, Type type);
void ParamDec(Tnode *node, Type type);
void CompSt(Tnode *node, Type rtn_type);
void StmtList(Tnode *node, Type rtn_type);
void Stmt(Tnode *node, Type rtn_type);
Type Exp(Tnode *node);
void Args(Tnode *node, FieldList argv);


Type get_array_type(Type type);
Type get_function_rtnType(Type type);
int type_cmp(Type type1, Type type2);
void print_type_name(Type type);

#endif