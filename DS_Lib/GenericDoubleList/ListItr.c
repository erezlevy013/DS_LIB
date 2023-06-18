#include <stdlib.h>
#include "GenericDoubleList.h"
#include "ListItr.h"
#include "Internal.h"

#define TRUE 1
#define FALSE 0

ListItr ListItrBegin(const List* _list)
{
	if( _list == NULL)
	{
		return NULL;
	}
	return _list->m_head.m_next;
}
ListItr ListItrEnd(const List* _list)
{
	if( _list == NULL)
	{
		return NULL;
	}
	return (void*)&_list->m_tail;
}
int ListItrEquals(const ListItr _a, const ListItr _b)
{
	return (_a == _b);
}
ListItr ListItrNext(ListItr _itr)
{
	if(_itr == NULL)
	{
		return NULL;
	}
	return ((Node*)_itr)->m_next; 
}
ListItr ListItrPrev(ListItr _itr)
{
	if(_itr == NULL)
	{
		return NULL;
	}
	if(((Node*)_itr)->m_prev->m_prev ==  ((Node*)_itr)->m_prev )
	{
		return ((Node*)_itr);
	}
	return ((Node*)_itr)->m_prev; 
}
void* ListItrGet(ListItr _itr)
{
	if(_itr == NULL)
	{
		return NULL;
	}
	if(((Node*)_itr)->m_next ==((Node*)_itr))
	{
		return NULL;
	}
	return ((Node*)_itr)->m_data;
}
void* ListItrSet(ListItr _itr, void* _element)
{
	void * temp;
	if(_itr == NULL || _element == NULL || ((Node*)_itr)->m_next ==((Node*)_itr))
	{
		return NULL;
	}
	temp = ((Node*)_itr)->m_data; 
	((Node*)_itr)->m_data = _element;
	return temp;
}
ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
	Node *newNode;
	if(_itr == NULL || _element == NULL)
	{
		return NULL;
	}
	if((newNode = (Node*)malloc(sizeof(Node))) == NULL)
	{
		return NULL;
	}
	newNode->m_data = _element;
	insert(newNode,((Node*)_itr));
	return newNode;
}
void* ListItrRemove(ListItr _itr)
{
	void *data;

	if(_itr == NULL || ((Node*)_itr)->m_next ==((Node*)_itr))
	{
		return NULL;
	}
	data = ((Node*)_itr)->m_data;
	pop(((Node*)_itr));
	free(_itr);
	return data;
}



