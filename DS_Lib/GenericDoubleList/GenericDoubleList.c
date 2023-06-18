#include <stdlib.h>
#include "GenericDoubleList.h"
#include "Internal.h"

void insert( Node *_node1, Node *_node2);
void pop( Node *_node);
static ListResult CheckPushParameters (List *_list, void* _item);
static ListResult CheckPopParameters(List *_list, void** _pitem);

List* ListCreate(void)
{
    List *list;
	if((list = (List*)malloc(sizeof(List))) == NULL )
	{
		return NULL;
	}
	list->m_head.m_next = &list->m_tail;
	list->m_head.m_prev = &list->m_head;
	list->m_tail.m_next = &list->m_tail;
	list->m_tail.m_prev = &list->m_head;

	return list;
}
void ListDestroy(List** _pList, ElementDestroy _elementDestroy)
{
    Node *temp, *temp2;
	if( _pList == NULL || *(_pList) == NULL)
	{
		return;
	}
	temp = (*(_pList))->m_head.m_next;
	while(temp != &(*(_pList))->m_tail)
	{
        if(_elementDestroy != NULL)
        {
            (*_elementDestroy)(temp->m_data);
        }
		temp2 = temp->m_next;
		free(temp);
		temp = temp2;
	}
	free(*(_pList));
    *(_pList) = NULL;
}
ListResult ListPushHead(List* _list, void* _item)
{
    Node *newNode;
    ListResult res;
	if( (res=CheckPushParameters(_list, _item)) != LIST_SUCCESS)
    {
        return res;
    }
	if((newNode = (Node*)malloc(sizeof(Node))) == NULL)
	{
		return LIST_ALLOCATION_ERROR;
	}
	newNode->m_data = _item;
	insert(newNode, _list->m_head.m_next);
	return LIST_SUCCESS;
}
ListResult ListPushTail(List* _list, void* _item)
{
    Node *newNode;
    ListResult res;
	if( (res=CheckPushParameters(_list, _item)) != LIST_SUCCESS)
    {
        return res;
    }
	if((newNode = (Node*)malloc(sizeof(Node))) == NULL)
	{
		return LIST_ALLOCATION_ERROR;
	}
	newNode->m_data = _item;
	insert(newNode,&_list->m_tail);
	return LIST_SUCCESS;
}
ListResult ListPopHead(List* _list, void** _pItem)
{
    Node * base;
    ListResult res;
	if( (res=CheckPopParameters(_list, _pItem)) != LIST_SUCCESS)
    {
        return res;
    }
    if( _list->m_head.m_next == &_list->m_tail )
    {
        return LIST_EMPTY;
    }
    base = _list->m_head.m_next;
	*(_pItem) = base->m_data;
	pop(base); /* _list->m_head.m_next  || base->m_next*/
	free(base);
	return LIST_SUCCESS;
}
ListResult ListPopTail(List* _list, void** _pItem)
{
    Node * base;
    ListResult res;
	if( (res=CheckPopParameters(_list, _pItem)) != LIST_SUCCESS)
    {
        return res;
    }
    if( _list->m_head.m_next == &_list->m_tail )
    {
        return LIST_EMPTY;
    }
    base =  _list->m_tail.m_prev;
	*(_pItem) = base->m_data;
	pop(base); /*  _list->m_tail.m_prev  || base->m_next*/
	free(base);
	return LIST_SUCCESS;
}
size_t ListSize(const List* _list)
{
    Node *temp;
    size_t count = 0;
	if( _list == NULL )
	{
		return count;
	}
	temp = _list->m_head.m_next;
	while(temp != &_list->m_tail)
	{
        count++;
		temp = temp->m_next;
	}
    return count;
}
void insert( Node *_node1, Node *_node2)
{
	_node1->m_prev = _node2->m_prev;
	_node1->m_next = _node2 ;
	_node2->m_prev->m_next = _node1;
	_node2->m_prev = _node1;
}
void pop( Node *_node)
{
	_node->m_next->m_prev = _node->m_prev;
	_node->m_prev->m_next = _node->m_next;
}
static ListResult CheckPushParameters(List *_list, void* _item)
{
    if(_list == NULL)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
    if(_item == NULL)
    {
        return LIST_NULL_ELEMENT_ERROR;
    }
    return LIST_SUCCESS;
}
static ListResult CheckPopParameters(List *_list, void** _pitem)
{
    if(_list == NULL || _pitem == NULL)
    {
        return LIST_UNINITIALIZED_ERROR;
    }
     return LIST_SUCCESS;
}


