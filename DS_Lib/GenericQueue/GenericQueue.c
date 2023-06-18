#include "GenericQueue.h"

#define INC 1 /* increase */


static QueueResult cheakInsert(Queue *_queue);
static QueueResult cheakRemove(Queue *_queue);
static QueueResult cheakPrametersInsert (Queue *_queue, void *_item);
static QueueResult cheakPrametersRemove(Queue *_queue, void **_item);

struct Queue
{
	void** m_que;		
 	size_t m_size;	/* Queue size */
 	size_t m_head; /* Index of head in m_que. */
 	size_t m_tail; /* Index of tail in m_que. */
 	size_t m_nItems; /* num of element */
};
Queue* QueueCreate(size_t _size)
{
	Queue *queue;
	if( _size == 0)   /* ??? size === 1 ??? */
	{
		return NULL;
	}
	if((queue = (Queue*)malloc(sizeof(Queue))) == NULL)
	{
		return NULL;
	}
	if((queue->m_que = (void**)malloc(_size * sizeof(void*))) == NULL)
	{
		free(queue);
		return NULL;
	}
	queue->m_size = _size;
	queue->m_head = 0;
	queue->m_tail = 0;
	queue->m_nItems = 0;

	return queue;
}
void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy)
{
	size_t i , size; 
	if( _queue == NULL ||   *(_queue) == NULL)
	{	
		return;
	}
	
	if( _itemDestroy != NULL)
	{
		size = (*_queue)->m_nItems;
		for(i=0; i<size; i++)
		{	
			_itemDestroy((*_queue)->m_que[((*_queue)->m_head + i) % (*_queue)->m_size] );
		}
		
	}
	free((*(_queue))->m_que);
	free(*(_queue));
	*(_queue) = NULL;
}
QueueResult QueueInsert(Queue* _queue, void* _item)
{
	QueueResult res;
	if((res = cheakPrametersInsert(_queue, _item)) != QUEUE_SUCCESS)
	{
		return res;
	}
	if(cheakInsert(_queue) != QUEUE_SUCCESS)
	{
		return QUEUE_FULL;
	}
	_queue->m_que[_queue->m_tail] = _item;
	_queue->m_tail = (_queue->m_tail + INC ) % _queue->m_size;
	_queue->m_nItems++;
	return QUEUE_SUCCESS;
}
QueueResult QueueRemove(Queue* _queue,void** _item)
{
	QueueResult res;
	if((res = cheakPrametersRemove(_queue, _item)) != QUEUE_SUCCESS)
	{
		return res;
	}
	if(cheakRemove(_queue) != QUEUE_SUCCESS)
	{
		return QUEUE_EMPTY;
	}
	*(_item) = _queue->m_que[_queue->m_head]; 
	_queue->m_head = (_queue->m_head + INC ) % _queue->m_size;
	_queue->m_nItems--;
	return QUEUE_SUCCESS;

}
size_t QueueIsEmpty(Queue* _queue)
{
	if( _queue == NULL )
	{
		return TRUE; 
	} 
	if( _queue->m_nItems == 0)
	{
		return TRUE;
	}
	return FALSE;
}
size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context)
{
    size_t i, numOfItems, j; 
    void *item ;
	if( _queue == NULL || _action == NULL )
	{
		return 0;
	}
    numOfItems = _queue->m_nItems;
	for(i = 0; i < numOfItems; i++)
	{   
		j = (_queue->m_head + i) % _queue->m_size;
		item = _queue->m_que[j];
     	if((*_action)(item, _context) == 0)
     	{
     		break;	
     	}		
    }
	return j;
}

static QueueResult cheakInsert(Queue *_queue)
{
	if( _queue->m_nItems == _queue->m_size )
	{
		return QUEUE_FULL;
	} 
	return QUEUE_SUCCESS;
}
static QueueResult cheakRemove(Queue *_queue)
{	
	if( _queue->m_nItems == 0 )
	{
		return QUEUE_EMPTY;
	}
	return QUEUE_SUCCESS;
}
static QueueResult cheakPrametersInsert (Queue *_queue, void *_item)
{
	if( _queue == NULL)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}
	if( _item == NULL)
	{
		return QUEUE_DATA_UNINITIALIZED_ERROR;
	}
	return QUEUE_SUCCESS;
}
static QueueResult cheakPrametersRemove(Queue *_queue, void **_item)
{
	if( _queue == NULL || _item == NULL)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}
	return QUEUE_SUCCESS;
}
