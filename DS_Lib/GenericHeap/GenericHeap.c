#include "GenericHeap.h"
#include "GenericVector.h"

#define IS_LEAF(x, y) (((x) >= (y)) ? (1) : (0))
#define F(i) ((i-1)/2)
#define RS(i) ((i+1)*2)
#define LS(i) ((i*2)+1)

static void Heapify(Heap* _heap, size_t _indx);
static size_t Compare(Heap* _heap, size_t _indxFather);
static void swap(Heap* _heap, size_t _indxFather, size_t _indx);
static void bubbleUp(Heap *_heap);
static HeapResultCode VectorError( VectorResult vectorResult);

struct Heap
{
    Vector* m_vec;
	size_t m_heapSize; 
    LessThanComparator m_comp;
};

Heap* HeapBuild(Vector* _vector, LessThanComparator _pfLess)
{
    Heap *heap;
	size_t i, father;
	if( _vector == NULL || _pfLess == NULL)
	{
		return NULL;
	}
	if((heap = (Heap*)malloc(sizeof(Heap))) == NULL )
	{
		return NULL;
	}
	heap->m_vec = _vector;
	heap->m_heapSize = VectorSize(_vector);
    heap->m_comp = _pfLess;
	if(heap->m_heapSize > 1)
	{	
		father = F(heap->m_heapSize-1);
		for(i=father; i>0; i--)
		{
			Heapify(heap, i);
		}
		Heapify(heap, 0); /* size_t >= 0*/
	}
	return heap;
}
Vector* HeapDestroy(Heap** _heap)
{
    Vector *vec = NULL;
    if ( _heap == NULL || *(_heap) == NULL)
	{
		return NULL;
	}
	vec = (*(_heap))->m_vec;
	free(*_heap);
    *(_heap) = NULL;
    return vec;
}
HeapResultCode HeapInsert(Heap* _heap, void* _element)
{
	VectorResult res;
	if(_heap == NULL || _element == NULL)
	{
		return HEAP_NOT_INITIALIZED;
	}
	if((res = VectorAppend(_heap->m_vec, _element)) != VECTOR_SUCCESS)
	{
		return VectorError( res);	
	}
    _heap->m_heapSize++;
	bubbleUp(_heap);
	return VECTOR_SUCCESS;	
}
const void* HeapPeek(const Heap* _heap)
{
    void *item;
	if( _heap == NULL || _heap->m_heapSize == 0 )
	{
		return NULL;
	}
	VectorGet(_heap->m_vec, 0, &item);
	return item;
}
void* HeapExtract(Heap* _heap)
{
	void *item, *data;
	VectorResult res;
	if( _heap == NULL || _heap->m_heapSize == 0)
	{
		return NULL;
	}
	VectorGet(_heap->m_vec, 0, &data);
	if((res = VectorRemove(_heap->m_vec, &item)) != VECTOR_SUCCESS)
	{
		return NULL;
	}
	VectorSet(_heap->m_vec, 0, item);
	_heap->m_heapSize--;
	Heapify(_heap, 0);
	return data;
}
size_t HeapSize(const Heap* _heap)
{
    if( _heap == NULL)
	{
		return 0;
	}
	return _heap->m_heapSize;
}
size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
    size_t i; 
    void *item ;
	if( _heap == NULL || _act == NULL )
	{
		return 0;
	}
    
	for(i = 0; i < _heap->m_heapSize; i++)
	{   
        VectorGet(_heap->m_vec, i, &item);
     	if((*_act)(item, _context) == 0)
     	{
     		break;	
     	}			
    }
	return i;
}

static void Heapify(Heap* _heap, size_t _indx)
{
	size_t temp;
	if(IS_LEAF(_indx, _heap->m_heapSize))
	{
		return;
	}
	temp = Compare(_heap, _indx);
	if(temp == _indx)
	{
		return;
	}
	swap(_heap, _indx, temp);
	Heapify(_heap, _indx);
}
static size_t Compare(Heap* _heap, size_t _indxFather)
{
	void  *left, *right, *father;
    VectorGet(_heap->m_vec, _indxFather, &father);
    VectorGet(_heap->m_vec, LS(_indxFather), &left);
	if(RS(_indxFather) < _heap->m_heapSize)
	{
		VectorGet(_heap->m_vec, RS(_indxFather), &right);
		if((*_heap->m_comp)( right , father) &&(*_heap->m_comp)(right ,left))
		{
			return RS(_indxFather);
		}
	}
	if( (*_heap->m_comp)(left , father))
	{
		return LS(_indxFather);
	}
	return _indxFather;
}
static void swap(Heap* _heap, size_t _indxFather, size_t _indx)
{
	void  *a, *b;
	VectorGet(_heap->m_vec, _indxFather, &a);
	VectorGet(_heap->m_vec, _indx, &b);
	VectorSet(_heap->m_vec, _indxFather, b);
	VectorSet(_heap->m_vec, _indx, a);
}
static void bubbleUp(Heap *_heap)
{
	size_t indx,father;
	void  *val, *valFater;
	indx = _heap->m_heapSize-1;
	father = F(indx);
	VectorGet(_heap->m_vec, indx, &val);
	while( father > 0)
	{
		VectorGet(_heap->m_vec, father, &valFater);
		if((*_heap->m_comp)(valFater , val))
		{
			return;
		}
		swap(_heap, father, indx);
		indx = father;
		father = F(father);
	}
	VectorGet(_heap->m_vec, father, &valFater);
	if((*_heap->m_comp)(val , valFater))
	{
		swap(_heap,father, indx);
	}
}
static HeapResultCode VectorError( VectorResult vectorResult)
{
    HeapResultCode res;
    switch (vectorResult)
    {
    case VECTOR_SUCCESS:
        res = HEAP_SUCCESS;
        break;
    case VECTOR_UNITIALIZED_ERROR:
         res = HEAP_NOT_INITIALIZED;
        break;
    case VECTOR_ALLOCATION_ERROR:
         res = HEAP_VECTOR_ALLOCATION_FAILED;
        break;
    case VECTOR_REALLOCATION_ERROR:
         res = HEAP_VECTOR_REALLOCATION_FAILED;
        break;
    case VECTOR_UNDERFLOW_ERROR:
         res = HEAP_VECTOR_UNDERFLOW_ERROR;
        break;
    case VECTOR_OVERFLOW_ERROR:
        res = HEAP_VECTOR_OVERFLOW_ERROR;
        break; 
    case VECTOR_INDEX_OUT_OF_BOUNDS_ERROR:
        res = HEAP_VECTOR_OVERFLOW_ERROR;
        break;  
    }
    return res;

} 
