//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Compute Nash equilibria via Lemke-Howson algorithm
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include <unistd.h>
#include <iostream>

#include "libgambit/libgambit.h"
#include "lhtab.h"

extern int g_numDecimals;

void PrintProfile(std::ostream &p_stream,
		  const std::string &p_label,
		  const Gambit::MixedStrategyProfile<double> &p_profile)
{
  p_stream << p_label;
  for (int i = 1; i <= p_profile.Length(); i++) {
    p_stream << "," << p_profile[i];
  }

  p_stream << std::endl;
}

void PrintProfile(std::ostream &p_stream,
		  const std::string &p_label,
		  const Gambit::MixedStrategyProfile<Gambit::Rational> &p_profile)
{
  p_stream << p_label;
  for (int i = 1; i <= p_profile.Length(); i++) {
    p_stream.setf(std::ios::fixed);
    p_stream << "," << std::setprecision(g_numDecimals) << p_profile[i];
  }

  p_stream << std::endl;
}


template <class T> Gambit::Matrix<T> Make_A1(const Gambit::StrategySupport &, 
				      const T &);
template <class T> Gambit::Vector<T> Make_b1(const Gambit::StrategySupport &, 
				      const T &);
template <class T> Gambit::Matrix<T> Make_A2(const Gambit::StrategySupport &,
				      const T &);
template <class T> Gambit::Vector<T> Make_b2(const Gambit::StrategySupport &,
				      const T &);


template <class T> class nfgLcp {
private:
  int m_stopAfter, m_maxDepth;

  int AddBfs(LHTableau<T> &, Gambit::List<BFS<T> > &);
  void AddSolutions(const Gambit::StrategySupport &,
		    const Gambit::List<BFS<T> > &, const T &);
  void AllLemke(const Gambit::StrategySupport &, int, LHTableau<T> &B, Gambit::List<BFS<T> > &,
		int depth);
  
public:
  nfgLcp(void);
  virtual ~nfgLcp() { }
  
  int StopAfter(void) const { return m_stopAfter; }
  void SetStopAfter(int p_stopAfter) { m_stopAfter = p_stopAfter; }

  int MaxDepth(void) const { return m_maxDepth; }
  void SetMaxDepth(int p_maxDepth) { m_maxDepth = p_maxDepth; }

  void Solve(const Gambit::StrategySupport &);
};

//---------------------------------------------------------------------------
//                        nfgLcp: member functions
//---------------------------------------------------------------------------

template <class T> 
void nfgLcp<T>::Solve(const Gambit::StrategySupport &p_support)
{
  BFS<T> cbfs((T) 0);

  if (p_support.GetGame()->NumPlayers() != 2) {
    return;
  }

  Gambit::List<BFS<T> > bfsList;

  try {
    Gambit::Matrix<T> A1 = Make_A1(p_support, (T) 0);
    Gambit::Vector<T> b1 = Make_b1(p_support, (T) 0);
    Gambit::Matrix<T> A2 = Make_A2(p_support, (T) 0);
    Gambit::Vector<T> b2 = Make_b2(p_support, (T) 0);
    LHTableau<T> B(A1,A2,b1,b2);
    if (m_stopAfter != 1) {
      AllLemke(p_support,0,B,bfsList,m_maxDepth);
    }
    else  {
      B.LemkePath(1);
      AddBfs(B, bfsList);
    }

    return AddSolutions(p_support, bfsList, B.Epsilon());
  }
  catch (...) {
    // for now, we won't give *any* solutions -- but we should list
    // any solutions found!
    throw;
  }
  // any other exceptions will propagate out.
}

template <class T> int nfgLcp<T>::AddBfs(LHTableau<T> &p_tableau,
					      Gambit::List<BFS<T> > &p_list)
{
  BFS<T> cbfs((T) 0);
  cbfs = p_tableau.GetBFS();
  if ((m_stopAfter > 0 && p_list.Length() > m_stopAfter) ||
      p_list.Contains(cbfs)) {  
    return 0;
  }
  p_list.Append(cbfs);
  return 1;
}

//
// AllLemke finds all accessible Nash equilibria by recursively 
// calling itself.  List maintains the list of basic variables 
// for the equilibria that have already been found.  
// From each new accessible equilibrium, it follows
// all possible paths, adding any new equilibria to the List.  
//
template <class T> void nfgLcp<T>::AllLemke(const Gambit::StrategySupport &p_support,
					    int j, LHTableau<T> &B,
					    Gambit::List<BFS<T> > &p_list,
					    int depth)
{
  if (m_maxDepth != 0 && depth > m_maxDepth) {
    return;
  }

  if (!AddBfs(B, p_list)) {
    return;
  }
  
  for (int i = B.MinCol(); 
       i <= B.MaxCol() &&
       (m_stopAfter==0 || (p_list.Length()-1) < m_stopAfter);
       i++) {
    if (i != j)  {
      LHTableau<T> Bcopy(B);
      Bcopy.LemkePath(i);
      AllLemke(p_support,i,Bcopy, p_list, depth+1);
    }
  }
  return;
}

template <class T>
void nfgLcp<T>::AddSolutions(const Gambit::StrategySupport &p_support,
			     const Gambit::List<BFS<T> > &p_list,
			     const T &epsilon)
{
  int i,j;
  int n1 = p_support.NumStrategies(1);
  int n2 = p_support.NumStrategies(2);

  for (i = 1; i <= p_list.Length(); i++)    {
    Gambit::MixedStrategyProfile<T> profile(p_support);
    T sum = (T) 0;

    for (j = 1; j <= n1; j++)
      if (p_list[i].IsDefined(j))   sum += p_list[i](j);

    if (sum == (T) 0)  continue;

    for (j = 1; j <= n1; j++) {
      if (p_list[i].IsDefined(j)) {
	profile[p_support.GetStrategy(1, j)] = p_list[i](j) / sum;
      }
      else {
	profile[p_support.GetStrategy(1, j)] = (T) 0;
      }
    }

    sum = (T) 0;

    for (j = 1; j <= n2; j++)
      if (p_list[i].IsDefined(n1 + j))  
	sum += p_list[i](n1 + j);

    if (sum == (T) 0)  continue;

    for (j = 1; j <= n2; j++) {
      if (p_list[i].IsDefined(n1 + j)) {
	profile[p_support.GetStrategy(2, j)] = p_list[i](n1 + j) / sum;
      }
      else {
	profile[p_support.GetStrategy(2, j)] = (T) 0;
      }
    }
    PrintProfile(std::cout, "NE", profile);
  }
}

//-------------------------------------------------------------------------
//                    nfgLcp<T>: Member functions
//-------------------------------------------------------------------------

template <class T>
nfgLcp<T>::nfgLcp(void)
  : m_stopAfter(0), m_maxDepth(0)
{ }


template <class T>
void SolveStrategic(const Gambit::Game &p_game)
{
  nfgLcp<T> algorithm;
  algorithm.Solve(p_game);
}

template void SolveStrategic<double>(const Gambit::Game &);
template void SolveStrategic<Gambit::Rational>(const Gambit::Game &);

