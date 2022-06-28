#include "SLL.h"

Node * sllGetPreLastNodeInSeq( Node * firstNode )
{
    Node * currentNode = firstNode;
    for ( ; currentNode != NULL &&
            currentNode->next != NULL ; )
    {
        if( currentNode->next->next != NULL )
        {
            currentNode = currentNode->next;
        } else
        {
            break;
        }
    }

    return currentNode;
};

Node * sllPush( Node * firstNode, void * dataPtr )
{
    Node * currentNode = firstNode;
    
    if( currentNode != NULL )
    {
        if( currentNode->data == NULL )
        {
            currentNode->data = dataPtr;
            return currentNode;
        } else
        {
            Node * preLastNode = sllGetPreLastNodeInSeq( currentNode );

            if( preLastNode->next != NULL )
            {
                preLastNode->next->next = createNode();

                preLastNode->next->next->data = dataPtr;

                return preLastNode->next->next;
            } else
            {
                preLastNode->next = createNode();
                preLastNode->next->data = dataPtr;
            }
        }
    } else
    {
        return NULL;
    }
};

void * sllPop( Node * firstNode )
{
    Node * currentNode = sllGetPreLastNodeInSeq( firstNode );

    void * data = currentNode->data;

    free(currentNode);

    return data;
};

Node * sllGetNodeByIndex( Node * firstNode, size_t index )
{
    Node * currentNode = firstNode;
    size_t currentIndex = 0;
    for( ; currentNode != NULL && currentIndex < index ; ++currentIndex )
    {
        if( currentNode->next == NULL ) return NULL;
        currentNode = currentNode->next;
    }

    return currentNode;
};

size_t sllGetListSize( Node * firstNode )
{
    Node * currentNode = firstNode;
    size_t i = 0;
    for( ; currentNode != NULL; ++i, currentNode = currentNode->next );

    return i;
}

void * sllToArray( Node * firstNode, size_t payloadSize )
{
    size_t sllSize = sllGetListSize(firstNode);

    void * array = malloc( payloadSize * sllSize );

    for (size_t i = 0; i < sllSize; i++)
    {
        Node * node = sllGetNodeByIndex(firstNode, i);
        memcpy( (void *)( array + i * payloadSize),
                node->data,
                payloadSize
               );
    }
        
    return array;
};

Node * createNode()
{
    Node * node = malloc( sizeof( Node ) );

    node->data = NULL;
    node->next = NULL;

    return node;
}

Node * createSll()
{
    Node * node = malloc( sizeof( Node ) );

    node->data = NULL;
    node->next = NULL;

    return node;
};