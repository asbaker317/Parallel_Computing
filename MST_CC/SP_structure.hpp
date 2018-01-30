#ifndef SP_STRUCTURE_HPP
#define SP_STRUCTURE_HPP


/*** All Headers Required From ESSENS **/
//INPUT HEADERS
#include "input_to_network.hpp"
#include <set>
#include<map>
#include "ADJ/create_networkP.hpp"

//OUTPUT HEADERS
#include "printout_network.hpp"
//#include "printout_others.hpp"

//SET_OPERATIONS
#include "unary_set_operations.hpp"
#include "binary_set_operations.hpp"

//NETWORK TRAVERSAL
#include "ADJ/traversal.hpp"

//ADD and DELETE EDGE
#include "ADJ/add_edge.hpp"
#include "ADJ/del_edge.hpp"

#include <omp.h>


//Power Profiling
//Comment out if not measuring power
/*
#ifdef POWER_PROFILING
// #define POWER_PROFILING_MAIN 1
#define POWER_PROFILING_WT 0
#include "power_rapl.h"
extern power_rapl_t ps;
#endif
*/



using namespace std;


//========================|
//Structure to indicate whether Edge is to be inserted/deleted
struct xEdge {
    Edge theEdge;
    int inst;
};

//===================|
//Comparing endpoints of two edges
bool operator == (const Edge &mye1, const Edge &mye2 )
{
    if((mye1.node1==mye2.node1) && (mye1.node2==mye2.node2) && (mye1.edge_wt==mye2.edge_wt)) {  return true;}
    else
    {
     if((mye1.node1==mye2.node2) && (mye1.node2==mye2.node1) && (mye1.edge_wt==mye2.edge_wt)) {  return true;}
    else
    {
    return false;
    }
    }
};


//bool wt_sort(Edge e1, Edge e2) {return (e1.edge_wt<e2.edge_wt);}

//Creating dummy edge
void dummy_edge(Edge *mye)
{
    mye->node1=-1;
    mye->node2=-1;
    mye->edge_wt=-1;
    return;
}




//===================|


// Data Structure for each vertex in the rooted tree
struct RT_Vertex
{
    int Parent; //mark the parent in the tree
    double EDGwt; //weight of edge between self and parent
    int Level; //Distance from the root
    int degree; //Degree of the vertex in the rooted tree
    int root; //The root of the tree
    Edge maxE;  // Maximum Edge from root to the vertex
    int marked; //whether the vertex and the edge connecting its parent ..
                //..exists(-1); has been deleted(-2); is marked for replacement (+ve value index of changed edge)
    //Constructor
    RT_Vertex()
    {
        Parent=-1;
        EDGwt=0.0;
        Level=-1;
        root=-1;
        degree=-1;
        marked=-1;
        dummy_edge(&maxE);
    }
    
    //Destructor
    void clear()
    {}
    
};
//The Rooted tree is a vector of structure RT_Vertex;


//Structure of Edge with level
struct lEdge {
    Edge theEdge;
    int Level;
};


//Define compare for edge
struct EdgeCompare
{
    bool operator() (const Edge& edge1, const Edge& edge2) const
    {
        return edge1.edge_wt< edge2.edge_wt;
    }
};

//Map of Edge to Position Index
typedef map<Edge,int, EdgeCompare> map_Edge_int;

////Finds the head (component id) of a given node
// Needed for union join
int find_head ( vector<int> *myV, int node)
{
    
    int f1=node;
    int i1;
    int iter=0;
    
    while(1)
    {
        i1=myV->at(f1);
        if(f1==i1) {break;}
        
        f1=i1;
        
        iter++;
    }
    
    return f1;
}
//====


#endif
