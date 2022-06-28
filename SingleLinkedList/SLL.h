#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include <stddef.h>
#include <memory.h>
#include <stdlib.h>

typedef struct Node
{
    void * data;
    struct Node * next;
} Node;

Node * sllPush( Node * firstNode, void * dataPtr );
void * sllPop( Node * firstNode );
Node * sllGetPreLastNodeInSeq( Node * firstNode );
Node * sllGetNodeByIndex( Node * firstNode, size_t index );
size_t sllGetListSize( Node * firstNode );
void * sllToArray( Node * firstNode, size_t payloadSize );
Node * createSll();
Node * createNode();

#endif