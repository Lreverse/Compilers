#ifndef __SEMANTIC_H
#define __SEMANTIC_H

#include "symbol_table.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Program(Tnode *node);
void ExcDecList(Tnode *node);
void ExtDef(Tnode *node);
Type Specifier(Tnode *node);
void ExtDecList(Tnode *node, Type type);
enum BASIC_TYPE TYPE(Tnode *node);
// FieldList StructSpecifier(Tnode *node, Type type);
void VarDec(Tnode *node, Type type);


#endif