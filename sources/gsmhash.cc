//
// FILE: gsmhash.h -- definitions of descendent HashTables used by
//                    GSM and its accompanying modules
//                    companion to GSM
//
// $Id$
//



#include "gstring.h"
class Portion;
class FuncDescObj;
class Instruction;
#ifdef __GNUG__
#define TEMPLATE template
#elif defined __BORLANDC__
#define TEMPLATE
template <class T> class gList;
template <class T> class gNode;

TEMPLATE class gList< gString >;
TEMPLATE class gNode< gString >;

#pragma option -Jgd
#endif   // __GNUG__, __BORLANDC__

#include "glist.imp"

TEMPLATE class gList< Portion* >;
TEMPLATE class gNode< Portion* >;

TEMPLATE class gList< FuncDescObj* >;
TEMPLATE class gNode< FuncDescObj* >;

TEMPLATE class gList< gList< Instruction* >* >;
TEMPLATE class gNode< gList< Instruction* >* >;

/* 
	 already instantiated in readefg.y
	 TEMPLATE class gList< gString >;
	 TEMPLATE class gNode< gString >;
	 */

#include "hash.imp"


TEMPLATE class HashTable< gList< Instruction* >*, int >;

TEMPLATE class HashTable< gString, Portion* >;
TEMPLATE class HashTable< gString, FuncDescObj* >;

#include "gsmhash.h"

#ifdef __BORLANDC__
TEMPLATE class RefCountHashTable< gList< Instruction* >* >;
#endif

#pragma option -Jgx
#include "portion.h"
#include "gsmfunc.h"







int RefHashTable::NumBuckets( void ) const
{ return 26; }

int RefHashTable::Hash( const gString& ref ) const
{ return (int)( ref[0] % 26 ); }

void RefHashTable::DeleteAction( Portion* value )
{ delete value; }

RefHashTable::RefHashTable()
{ Init(); }

RefHashTable::~RefHashTable()
{ Flush(); }





int FunctionHashTable::NumBuckets() const
{ return 26; }

int FunctionHashTable::Hash( const gString& funcname ) const 
{ return (int)( funcname[0] ) % 26; }

void FunctionHashTable::DeleteAction( FuncDescObj* func ) 
{ delete func; }

FunctionHashTable::FunctionHashTable() 
{ Init(); }

FunctionHashTable::~FunctionHashTable() 
{ Flush(); }  





template <class T> int RefCountHashTable<T>::NumBuckets( void ) const 
{ return 10; }

template <class T> int RefCountHashTable<T>::Hash( const T& ptr ) const 
{ return ( (unsigned int) ptr / 4 ) % 10; }

template <class T> void RefCountHashTable<T>::DeleteAction( int /*value*/ ) 
{ }

template <class T> RefCountHashTable<T>::RefCountHashTable() 
{ Init(); }

template <class T> RefCountHashTable<T>::~RefCountHashTable()
{ Flush(); }


TEMPLATE class RefCountHashTable< gList< Instruction* >* >;
