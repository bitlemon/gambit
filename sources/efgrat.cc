//
// FILE: efgrat.cc -- Instantiation of rational-precision extensive forms
//
// $Id$
//

#include "rational.h"
#include "glist.h"

#ifdef __GNUG__
#define TEMPLATE template
#elif defined __BORLANDC__
template <class T> class TypedNode;
class TypedNode<double>;
template <class T> class BehavProfile;
class BehavProfile<double>;
template <class T> class Efg;
class Efg<double>;
template <class T> class OutcomeVector;
class OutcomeVector<double>;
template <class T> class ChanceInfoset;
class ChanceInfoset<double>;
class Node;
class gString;
class gList<gString>;
class gList<gRational>;
class gList<Node *>;

template <class T> class BehavSolution;
gOutput &operator<<(gOutput &, const BehavSolution<gRational> &);

#define TEMPLATE
#pragma option -Jgd
#endif   // __GNUG__, __BORLANDC__

#include "efg.imp"

TEMPLATE class Efg<gRational>;
DataType Efg<gRational>::Type(void) const   { return RATIONAL; }

TEMPLATE class TypedNode<gRational>;
TEMPLATE class ChanceInfoset<gRational>;
TEMPLATE class OutcomeVector<gRational>;
TEMPLATE class BehavProfile<gRational>;
TEMPLATE gOutput &operator<<(gOutput &, const BehavProfile<gRational> &);
//TEMPLATE bool operator==(const gArray<gRational> &, const gArray<gRational> &);
//TEMPLATE bool operator!=(const gArray<gRational> &, const gArray<gRational> &);

#include "efgiter.imp"

TEMPLATE class EfgIter<gRational>;

#include "efgciter.imp"

TEMPLATE class EfgContIter<gRational>;

#include "readefg.imp"

TEMPLATE class EfgFile<gRational>;
TEMPLATE int ReadEfgFile(gInput &, Efg<gRational> *&);

#include "glist.imp"

TEMPLATE class gList<BehavProfile<gRational> >;
TEMPLATE class gNode<BehavProfile<gRational> >;

#include "behavsol.h"

TEMPLATE class gList<BehavSolution<gRational> >;
TEMPLATE class gNode<BehavSolution<gRational> >;
