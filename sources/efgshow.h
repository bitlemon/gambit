//
// FILE: efgshow.h -- Declarations of classes for extensive form display code
//
//  $Id$
//

#ifndef EFGSHOW_H
#define EFGSHOW_H

#include "efgnfgi.h"
#include "gambit.h"
#include "accels.h"
#include "efgsolng.h"
#include "bsolnsf.h"
#include "efgconst.h"

// For gui logging ------------
#include "guiobj.h"
#include "guirec.h"
#include "guirecdb.h"
#include "guipb.h"
// ----------------------------

class EfgSolnShow;
class guiEfgShowToolBar;
class EFSupportInspectDialog;
class NodeSolnShow;
class TreeWindow;

template <class T> class SolutionList: public gSortList<T>
{
private:
    unsigned int max_id;

public:
    SolutionList(void):gSortList<T>(), max_id(1) { }
    SolutionList(const gList<T> &l): gSortList<T>(l), max_id(1) { }
    virtual int Append(const T &a)
    { (*this)[gSortList<T>::Append(a)].SetId(max_id++); return Length(); }
};

typedef SolutionList<BehavSolution> BehavSolutionList;

class EfgShow: public wxFrame, public EfgNfgInterface, public EfgShowInterface, public GuiObject
{
private:
    wxFrame *parent;
    Efg &ef;

    // Solution routines
    BehavSolutionList solns;
    struct StartingPoints
    {
        BehavSolutionList profiles;
        int last;
        StartingPoints() : last(-1) { }
    } starting_points;
    int cur_soln;

    // We can display EF for one support, while working on a different support
    // disp_sup always corresponds to the support currently displayed.  cur_sup
    // corresponds to the support that will be operated upon by solution algs.
    gList<EFSupport *> supports;
    EFSupport *cur_sup, *disp_sup;
    EFSupportInspectDialog *support_dialog;

    // all_nodes must at all times refer to the prefix traversal order of the tree.
    // It is TreeWindow's job to call RemoveSolutions every time the nodes are
    // changes.  all_nodes is maintained in RemoveSolutions.
    gList<Node *> all_nodes;
    EfgSolnShow *soln_show;
    BSolnSortFilterOptions sf_options;
    gList<Accel>    accelerators;
    gText   filename;
    // checkable menu items 
    int m_inspectInfosetsItem;
    guiEfgShowToolBar *toolbar;

    // Private functions
    gArray<AccelEvent> MakeEventNames(void);
    NodeSolnShow *node_inspect;

    bool SolveNormalReduced(void);
    bool SolveNormalAgent(void);

    void SubgamesSetup(void);
    void NodeInspect(bool insp);

  struct es_features {
    Bool iset_hilight;
    es_features(void) : iset_hilight(FALSE) { }
  } features;

    void MakeMenus(void);

public:
  TreeWindow *tw;

  // CONSTRUCTOR AND DESTRUCTOR
  EfgShow(Efg &p_efg, EfgNfgInterface *p_nfg = 0, int p_subgame = 1, 
	  wxFrame *p_frame = 0, char *p_title = 0, 
	  int p_x = -1, int p_y = -1, int p_w = 600,
	  int p_h = 400, int p_type = wxDEFAULT_FRAME);
  ~EfgShow();

    // Event handlers
    Bool        OnClose(void);
    void        OnMenuCommand(int id);
    void        OnSize(int w, int h);
    void        OnSetFocus(void);

    // Solution routines
    void        Solve(int id);

    void        SolveStandard(void);
    void        InspectSolutions(int what);
    void        RemoveSolutions(void);
    void        ChangeSolution(int soln);
    void        OnSelectedMoved(const Node *n);
    BehavSolution CreateSolution(void);

    // Solution interface to the algorithms
    void        PickSolutions(const Efg &, gList<BehavSolution> &);
    BehavProfile<gNumber> CreateStartProfile(int how);
    void        RemoveStartProfiles(void);
    void        SetPickSubgame(const Node *n);

    // Solution interface to normal form
    void        SolutionToEfg(const BehavProfile<gNumber> &s, bool set = false);
    const  Efg *InterfaceObjectEfg(void) { return &ef; }
    wxFrame *Frame(void);

    // Solution access for TreeWindow
    gNumber BranchProb(const Node *n, int br);

    // Reset the supports.
    void GameChanged(void);

    // Interface for infoset hilighting between the tree and solution display
    void HilightInfoset(int pl, int iset, int who);

    // Accelerators allow for platform-indep handling of hotkeys
    int CheckAccelerators(wxKeyEvent &ev);

    // EFSupport support
    void        ChangeSupport(int what);
    EFSupport  *MakeSupport(void);
    void        SolveElimDom(void);

    // Used by TreeWindow
    virtual gText AsString(TypedSolnValues what, const Node *n, int br = 0) const;

    // Display some inherent game properties
    void ShowGameInfo(void);

    // Currently used support
    const EFSupport *GetSupport(int which);

  void UpdateMenus(Node *p_cursor, Node *p_markNode);

    // File name
    void         SetFileName(void);
    void         SetFileName(const gText &s);
    const gText &Filename(void) const;

	// Gui playback:
    void ExecuteLoggedCommand(const class gText &, const class gList<gText> &);
};


// Solution constants
typedef enum
{
    EFG_NO_SOLUTION = -1, EFG_QRE_SOLUTION, EFG_LIAP_SOLUTION,
    EFG_LCP_SOLUTION, EFG_PURENASH_SOLUTION, EFG_CSUM_SOLUTION,
    EFG_NUM_SOLUTIONS
} EfgSolutionT;

#endif // EFGSHOW_H

