#ifndef __LIST_FUNC_H__
#define __LIST_FUNC_H__


#include "ListItr.h"

typedef int (*GenFunc) (void * _element, void* _context);



/** 
 * @brief Predicate function returns a none zero value if predicate holds for element
 *
 * @param _element : element to test
 * @param _context : context to be used
 * @return none zero if predicate holds
 */
typedef int (*PredicateFunction)(void * _element, void* _context);

/** 
 * @brief Action function to operate on an element
 *
 * @param _element : element to test
 * @param _context : context to be used
 * @return zero if action fails
 */
typedef int (*ListActionFunction)(void* _element, void* _context);


/** 
 * @brief Finds the first element in a range satsifying a predicate
 * @details find the element for which the predicate reuturns a non zero value
 *  in the half open range [begin..end)
 *
 * @param _begin : start search from here
 * @param _end : end search here - not included
 * @param _predicate : Predicate function
 * @param _context : context to be passed to the predicate
 * @return an iterator pointing to the first node with data found or to _end if not
 * @return NULL if (_begin or _end  or _predicate ) = NULL
 */
ListItr ListItrFindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context);

/** 
 * @brief count elements in a sub list satisfying predicate
 * @details find the element for which the predicate reuturns a non zero value
 *  in the half open range [begin..end)
 *
 * @param _begin : start search from here
 * @param _end : end search here - not included
 * @param _predicate : Predicate function
 * @param _context : context to be passed to the predicate
 * @return count of all elements e where _predicate(e, _context) != 0
 * @return 0 if (_begin or _end  or _predicate  or  _context ) = NULL
 */
size_t ListItrCountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context);

/** 
 * @brief execute an action for all elements in a half open range
 * @details call -action for each element in the range [begin..end)
 * iteration will stop if Action function returns 0 for an element
 *
 * @param _begin: A list iterator to start operations from
 * @param _end : A list iterator to end operations on
 * @param _Action : user provided action function
 * @param _context : Parameters for the function
 * @return ListItr to the element where the iteration stoped. this might be end iterator
*  @return NULL if (_begin or _end  or _predicate ) = NULL
 */
ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context);



#endif /*  __LIST_FUNC_H__ */

