/**
 * Project: Compiler IFJ21
 *
 * @file expression.h
 *
 * @brief
 *
 * @author
 */

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <stdbool.h>


typedef struct DLLElement {
    char data[3];
    struct DLLElement *previousElement;
    struct DLLElement *nextElement;
} *DLLElementPtr;


typedef struct {
    DLLElementPtr firstElement;
    DLLElementPtr lastElement;
} DLList;

bool expression();
void DLL_Init( DLList * );
void DLL_Dispose( DLList * );
void DLL_InsertFirst( DLList *, char * );
void DLL_InsertLast( DLList *, char * );
void DLL_GetFirst( DLList *, int * );
void DLL_GetLast( DLList *, int * );
#endif // _EXPRESSION_H
