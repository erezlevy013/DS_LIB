
#include "GenericVector.h"

static VectorResult Increase(Vector *_vector);
static VectorResult reduce(Vector *_vector);


struct Vector
{
	void** m_items;	
	size_t m_originalSize;
	size_t m_size;
	size_t m_nItems;
	size_t m_blockSize;
};

Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
	Vector *vector;
	if(_blockSize == 0 && _initialCapacity == 0 )
	{
		return NULL;
	}
	if((vector = (Vector*)malloc(sizeof(Vector))) == NULL)
	{
		return NULL;
	}
	if((vector->m_items = (void**)malloc( _initialCapacity * sizeof(void*) )) == NULL)
	{
		free(vector);
		return NULL;
	}
	vector->m_originalSize = _initialCapacity;
	vector->m_size = _initialCapacity;
	vector->m_nItems = 0;
	vector->m_blockSize = _blockSize;
	return vector;
}

void VectorDestroy(Vector** _vector, ElementDestroy _elementDestroy)
{
	size_t i, size;
	if( _vector == NULL || *(_vector) == NULL )
	{
		return;
	}
	if(_elementDestroy != NULL)
	{
		size = (*_vector)->m_nItems;
		for(i=0; i<size; i++)
		{	
			_elementDestroy((*_vector)->m_items[i]);
		}
		
	}
	
	free((*_vector)->m_items);
	free(*(_vector));
	*(_vector) = NULL;
}

VectorResult VectorAppend(Vector* _vector, void* _item)
{
	Vector *tempPtr;
	if(_vector == NULL || _item == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	tempPtr = _vector;
	if( tempPtr->m_size == tempPtr->m_nItems + 1 )
	{
		if(tempPtr->m_blockSize == 0)
		{
			return VECTOR_OVERFLOW_ERROR;
		}
		if(Increase(tempPtr) != VECTOR_SUCCESS)
		{
			return VECTOR_REALLOCATION_ERROR;
		}
	}
	*(_vector->m_items + _vector->m_nItems) = _item;
	_vector->m_nItems++;

	return VECTOR_SUCCESS;
}

VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
	if(_vector == NULL || _pValue == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if(_vector->m_nItems == 0)
	{
		return VECTOR_UNDERFLOW_ERROR;
	}
	_vector->m_nItems--;
	*(_pValue) = *(_vector->m_items +_vector->m_nItems);
	
	if(2*(_vector->m_size - _vector->m_nItems) == (_vector->m_size))
	{
		reduce(_vector);	
	}
	return VECTOR_SUCCESS;
}

VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue)
{
	if(_vector == NULL || _pValue == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if(_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	*(_pValue) = *(_vector->m_items + _index);
	return VECTOR_SUCCESS;
}
VectorResult VectorSet(Vector* _vector, size_t _index, void*  _value)
{
	if(_vector == NULL || _value == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if(_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	*(_vector->m_items + _index) = _value;
	return VECTOR_SUCCESS;
}

size_t VectorSize(const Vector* _vector)
{	
	if(_vector == NULL)
	{
		return 0;
	}
	return _vector->m_nItems;
	
}

size_t VectorCapacity(const Vector* _vector)
{
	if(_vector == NULL)
	{
		return 0;
	}
	return _vector->m_size;
}
size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	size_t i, numOfItems = _vector->m_nItems;
	if(_vector == NULL)
	{
		return 0;
	}
	for(i = 0; i < numOfItems; i++)
	{   
     	if(_action(*(_vector->m_items+i), i, _context) == 0)
     	{
     		break;	
     	}			
    }
	return i;
}

size_t GetNumOfItems(const Vector* _vector)
{
	return _vector->m_nItems;
}

static VectorResult Increase(Vector *_vector)
{
	void **new_addres;
	if((new_addres = (void**)realloc(_vector->m_items,(_vector->m_size + _vector->m_blockSize)*sizeof(void*))) == NULL )
	{
		return VECTOR_REALLOCATION_ERROR;
	}
	_vector->m_size = _vector->m_size + _vector->m_blockSize;
	_vector->m_items = new_addres;
	
	return VECTOR_SUCCESS;
}
static VectorResult reduce(Vector *_vector)
{
	void **new_addres;
	if((new_addres = (void **)realloc(_vector->m_items,(_vector->m_size - _vector->m_blockSize)*sizeof(void *))) == NULL )
	{
		return VECTOR_REALLOCATION_ERROR;
	}
	_vector->m_size = _vector->m_size - _vector->m_blockSize;
	_vector->m_items = new_addres;
	
	return VECTOR_SUCCESS;
}



