//
// FILE: efstrat.h -- Supports and stuff for the extensive form games
//
// $Id$
//

#ifndef EFSTRAT_H
#define EFSTRAT_H

#include "gstring.h"
#include "gblock.h"
#include "efg.h"
#include "gpvector.h"

class Action;
class EFPlayer;

class EFActionArrays   {
  friend class EFActionSet;
protected:
  gBlock<Action *> acts;

public:
  EFActionArrays ( const gArray <Action *> &a);
  EFActionArrays ( const EFActionArrays &a);
  virtual ~EFActionArrays();
  EFActionArrays &operator=( const EFActionArrays &a);
  bool operator==( const EFActionArrays &a);

};

class EFActionSet{

protected:
  EFPlayer *efp;
  gArray < EFActionArrays *> infosets;
public:
  
  //----------------------------------------
  // Constructors, Destructor, operators
  //----------------------------------------

//  EFActionSet();
  EFActionSet(const EFActionSet &);
  EFActionSet(EFPlayer &);
  virtual ~EFActionSet();

  EFActionSet &operator=(const EFActionSet &);
  bool operator==(const EFActionSet &s);

  //--------------------
  // Member Functions
  //--------------------

  // Append an action to an infoset;
  void AddAction(int iset, Action *);

  // Insert an action in a particular place in an infoset;
  void AddAction(int iset, Action *, int index);


  // Remove an action at int i, returns the removed action pointer
  Action *RemoveAction(int iset, int i);

  // Remove an action from an infoset . 
  // Returns true if the action was successfully removed, false otherwise.
  bool RemoveAction(int iset, Action *);

  // Get a garray of the actions in an Infoset
  const gArray<Action *> &ActionList(int iset) const
     { return infosets[iset]->acts; }
  
  // Get an Action
  Action *GetAction(int iset, int index);

  // returns the index of the action if it is in the ActionSet
  int IsActionInActionSet(int iset, Action *);
 
  // returns the index of the action in the original ActionSet
  int OriNumber (int iset, Action *);

  // Number of Infosets 
  int NumInfosets(void) const;

  // Number of Actions in a particular infoset
  int NumActions(int iset) const;

  // return the EFPlayer of the EFActionSet
  EFPlayer &GetPlayer(void) const;

  // checks for a valid EFActionSet
  bool IsValid(void) const;

};

class BaseEfg;  


class EFSupport {


protected:

  gString name;
  const BaseEfg *befg;
  gArray <EFActionSet *> sets;

public:

  //--------------------------------------
  // Constructors, Destructors, operators
  //--------------------------------------

  EFSupport ( const BaseEfg &);
  EFSupport ( const EFSupport &);
  virtual ~EFSupport();
  EFSupport &operator=(const EFSupport &);
  bool operator==(const EFSupport &);

  //-------------
  // Members
  //-------------

  // Return the number of players, infosets, actions in a support 
  int NumActions(int pl, int iset) const;

  // Returns the position of the action in the support.  Returns zero
  // if it is not there.
  int Contains(Action *) const;

  const gArray<Action *> &ActionList(int pl, int iset) const
    { return sets[pl]->ActionList(iset); }

  // Returns the original position of the action before any changes were made
  // in the support.
  int OriNumber(int pl, int iset, Action *);

  // Action editing functions
  Action * RemoveAction( int, int, int );
  bool RemoveAction( int, int, Action *);
  void AddAction (int , int, Action *);
  void AddAction (int , int, Action *, int);

  // Returns the BaseEfg associated with this Support.
  const BaseEfg &BelongsTo(void) const;

  // Checks to see that every infoset in the support has at least one
  // action in it.
  bool IsValid(void) const;

  // Returns a gPVector representing the Dimensionality of the support.
  gPVector<int> Dimensionality(bool trunc) const;

  // Number of Sequences for the player
  int NumSequences(int pl) const;

  void Dump(gOutput& s) const;
};

gOutput &operator<<(gOutput &f, const EFSupport &);


#endif  //# EFSTRAT_H














