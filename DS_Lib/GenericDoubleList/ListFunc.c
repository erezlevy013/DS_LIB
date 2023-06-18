#include "ListFunc.h"
#define TRUE 1
#define FALSE 0

typedef int (*InnerFunc) (int _element, void* _context);

typedef struct GenInfo
{
    GenFunc m_genFunc;
    InnerFunc m_innerFunc;
    void *m_context;
    void *m_returnContext;
}Info;


static int ChangeForEach (int  _res , void* _context);
static int ChangeFindFirst (int _res , void* _context);
static int ChangeCountIf (int _res , void* _context);
static ListItr ListItrRun( ListItr _begin, ListItr _end, Info _info );



ListItr ListItrFindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    Info info ;
    if( _begin == NULL || _end == NULL ||_predicate == NULL )
    {
        return NULL;
    }
    info.m_genFunc = _predicate;
    info.m_innerFunc = ChangeFindFirst;
    info.m_context = _context;
    info.m_returnContext = NULL;
    return ListItrRun( _begin, _end, info );
}

size_t ListItrCountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    Info info;
    size_t count = 0;
    if( _begin == NULL || _end == NULL ||_predicate == NULL || _context == NULL)
    {
        return count;
    }
    info.m_genFunc = _predicate;
    info.m_innerFunc = ChangeCountIf;
    info.m_context = _context;
    info.m_returnContext = &count;
    ListItrRun( _begin, _end, info );
    return count;
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
    Info info;
    if( _begin == NULL || _end == NULL || _action == NULL )
    {
        return NULL;
    }
    info.m_genFunc = _action;
    info.m_innerFunc = ChangeForEach;
    info.m_context = _context;
    info.m_returnContext = NULL;
    return ListItrRun( _begin, _end, info );
}

static ListItr ListItrRun( ListItr _begin, ListItr _end, Info _info )
{
    ListItr start = _begin;
    int res;
    while( start != _end )
    {
        res = (*_info.m_genFunc)(start, _info.m_context );
        res = (*_info.m_innerFunc)(res, _info.m_returnContext);
        if( res == 0 )
        {
            break;
        }
        start = ListItrNext(start);
    }
    return start;
}
static int ChangeForEach (int _res , void* _context)
{
    return _res;
}
static int ChangeFindFirst (int _res , void* _context)
{
    return (!_res);
}
static int ChangeCountIf (int _res , void* _context)
{
    if(_res)
    {
        (*(size_t*)_context)++;
    }
    return TRUE;
}
