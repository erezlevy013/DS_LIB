#include <stdlib.h>
#include <math.h>

#include "HashMap.h"
#include "GenericDoubleList.h"
#include "ListItr.h"
#include "ListFunc.h"

#define MIN_SIZE  5

struct HashMap
{
    List **m_data;
    HashFunction m_hashFunction;
    EqualityFunction m_equalityFunction;
    size_t m_hashSize;            
    size_t m_capacity;            
    size_t m_numOfItems;           
};

typedef struct Bucket
{
    void* m_key;
    void* m_val;
}Bucket;

typedef struct InfoFindFirst
{
    void* m_userContext;
    EqualityFunction m_userFunc;
}InfoFindFirst;

typedef struct Info
{
    void* m_userContext;
    KeyValueActionFunction m_userFunc;
    size_t m_count;
}Info;


typedef struct DestroyFuncs
{
    KeyDestroy m_key;
    ValDestroy m_val;
}DestroyFuncs;

static size_t CalcSize(size_t _size);
static int IsPrime (size_t _num);
static Bucket* BucketCreate(const void *_key, const void *_val);
static Map_Result ListCreateForMap(HashMap *_map, size_t _indx);
static int IsDuplicate(EqualityFunction _keysEqualFunc, List* _list, const void* _key, ListItr* _itr);
static int ForEachAction (void* _element, void* _info);
static int FindFirstAction (void* _element, void* _info);
static int ForEachDestroy(void* _element, void* _info);


HashMap *HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
    HashMap* hashMap;
    List** list;
    size_t newSize;
    if ( _capacity <  MIN_SIZE|| _hashFunc == NULL || _keysEqualFunc == NULL )
    {
        return NULL;
    }
    if ((hashMap = (HashMap*)malloc(sizeof(HashMap))) == NULL)
    {
        return NULL;
    }
    newSize = CalcSize(_capacity);
    if ((list = (List**)calloc(sizeof(List*), newSize)) == NULL)
    {
        free(hashMap);
        return NULL;
    }
    hashMap->m_data = list;
    hashMap->m_hashFunction = _hashFunc;
    hashMap->m_equalityFunction = _keysEqualFunc;
    hashMap->m_hashSize = _capacity;
    hashMap->m_capacity = newSize;
    hashMap->m_numOfItems = 0;
    return hashMap;
}
Map_Result HashMap_Insert(HashMap *_map, const void *_key, const void *_value)
{
    size_t indx;
    Bucket* bucket;
    ListItr itr;
    if (_map == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }

    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if ((bucket = BucketCreate(_key, _value)) == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }
    indx = ((*_map->m_hashFunction)(bucket->m_key)) % _map->m_capacity;
    if(_map->m_data[indx] == NULL)
    {
        if ((ListCreateForMap(_map, indx)) != MAP_SUCCESS)
        {
            return MAP_ALLOCATION_ERROR;
        }
    }
    if(IsDuplicate(_map->m_equalityFunction, _map->m_data[indx], _key, &itr))
    {
        return MAP_KEY_DUPLICATE_ERROR;
    }
    if ( ListItrInsertBefore(itr, bucket) == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }
    _map->m_numOfItems ++;
    
    return MAP_SUCCESS;
}
void HashMap_Destroy(HashMap **_map, KeyDestroy _keyDestroy, ValDestroy _valDestroy)
{
    size_t i, sizeMap;
    ListItr begin, end ;
    DestroyFuncs destroyFuncs;
    if (_map ==NULL || *_map == NULL)
    {
        return; 
    }
    destroyFuncs.m_key = _keyDestroy;
    destroyFuncs.m_val = _valDestroy;
    sizeMap = (*_map)->m_capacity;
    for (i = 0; i < sizeMap; i++)
    {
        if ((*_map)->m_data[i] != NULL)
        {
            begin = ListItrBegin((*_map)->m_data[i]);
            end = ListItrEnd((*_map)->m_data[i]);
            ListItrForEach(begin, end, ForEachDestroy, &destroyFuncs);
            ListDestroy(&(*_map)->m_data[i], NULL);
        }
    }
    free(*_map);
    *_map = NULL;
}
Map_Result HashMap_Remove(HashMap *_map, const void *_searchKey, void **_pKey, void **_pValue)
{
    size_t indx;
    ListItr itr;
    Bucket* bucket;
    if (_map == NULL || _pKey == NULL || _pValue == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_searchKey == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    indx = ((*_map->m_hashFunction)( _searchKey)) % _map->m_capacity;
    if (_map->m_data[indx] == NULL || !IsDuplicate(_map->m_equalityFunction, _map->m_data[indx], _searchKey, &itr))
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    bucket =(Bucket*)ListItrRemove(itr);
    *_pKey = bucket->m_key;
    *_pValue = bucket->m_val;
    _map->m_numOfItems --;
    
    return MAP_SUCCESS;
}
Map_Result HashMap_Find(const HashMap *_map, const void *_key, void **_pValue)
{
    size_t indx;
    ListItr itr;
    Bucket* bucket;
    if (_map == NULL || _pValue == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }

    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    indx = ((*_map->m_hashFunction)(_key)) % _map->m_capacity;
    if (!IsDuplicate(_map->m_equalityFunction, _map->m_data[indx], _key, &itr))
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    bucket = (Bucket*)ListItrGet(itr);
    *_pValue = bucket->m_val;
    return MAP_SUCCESS;
}
size_t HashMap_Size(const HashMap *_map)
{
   if ( _map == NULL) 
   {
        return 0;
   }
   return _map->m_numOfItems;
}
size_t HashMap_ForEach(const HashMap *_map, KeyValueActionFunction _action, void *_context)
{
    size_t i, sizeMap;
    ListItr begin, end , itr;
    Info info;
    if (_map == NULL || _action == NULL)
    {
        return 0;
    }
    info.m_count = 0;
    info.m_userContext = _context;
    info.m_userFunc = _action;
    sizeMap = _map->m_capacity;
    for (i = 0; i < sizeMap; i++)
    {
        if(_map->m_data[i] != NULL)
        {
            begin = ListItrBegin(_map->m_data[i]);
            end = ListItrEnd(_map->m_data[i]);
            itr = ListItrForEach(begin, end, ForEachAction, &info);
            if (itr != end)
            {
                break;
            }
        }     
    }
    return info.m_count;
}

static size_t CalcSize (size_t _size)
{
    while (!IsPrime(_size))
    {
        _size++;
    }
    return _size;
}
static int IsPrime (size_t _num)
{
    size_t i, sq;
    if (_num % 2 == 0)
    {
        return FALSE;
    }
    sq = sqrt(_num);
    for (i = 3; i < sq; i += 2)
    {
        if (_num % i == 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}
static Bucket* BucketCreate(const void *_key, const void *_val)
{
    Bucket* bucket;
    if ((bucket = (Bucket*)malloc(sizeof(Bucket))) == NULL)
    {
        return NULL;
    }
    bucket->m_key = (void*)_key;
    bucket->m_val = (void*)_val;
    return bucket;
}
static Map_Result ListCreateForMap(HashMap *_map, size_t _indx)
{
    List* list;
    if ((list = ListCreate()) == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }
    _map->m_data[_indx] = list;
    return MAP_SUCCESS;
}
static int IsDuplicate(EqualityFunction _keysEqualFunc, List* _list, const void* _key, ListItr* _itr)
{
    ListItr begin,  end, checkedItr;
    InfoFindFirst info;
    begin = ListItrBegin(_list);
    end = ListItrEnd(_list);
    if (ListItrEquals(begin, end))
    {
        *_itr = end;
        return FALSE;
    }
    info.m_userContext = (void*)_key;
    info.m_userFunc = _keysEqualFunc;
    checkedItr = ListItrFindFirst(begin, end, FindFirstAction, &info);
    if (ListItrEquals(checkedItr, end))
    {
        *_itr= end;
        return FALSE;
    }
    *_itr = checkedItr;
    return TRUE;
}
static int ForEachAction (void* _element, void* _info)
{
    Bucket* bucket;
    Info* info;
    int res;
    info = (Info*)_info;
    bucket = (Bucket*)_element;
    res = (info->m_userFunc)(bucket->m_key,bucket->m_val,info->m_userContext);
    info->m_count ++;
    return res;
}
static int FindFirstAction (void* _element, void* _info)
{
    Bucket* bucket;
    InfoFindFirst* info;
    int res;
    info = (InfoFindFirst*)_info;
    bucket = (Bucket*)_element;
    res = (info->m_userFunc)(bucket->m_key,info->m_userContext);
    return res;
}
static int ForEachDestroy(void* _element, void* _info)
{
    DestroyFuncs* destroyFuncs;
    Bucket* bucket;
    destroyFuncs = (DestroyFuncs*)_info;
    bucket = (Bucket*)_element;
    if (destroyFuncs->m_key != NULL)
    {
      (*destroyFuncs->m_key)(bucket->m_key);
    }
    if (destroyFuncs->m_val != NULL)
    {
      (*destroyFuncs->m_val)(bucket->m_val);
    }
    return 1;
}


