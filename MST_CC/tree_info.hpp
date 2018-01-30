//
//  maxEdgefind.hpp
//  
//
//  Created by Sanjukta Bhowmick on 1/19/17.
//
//

#ifndef TREEINFO_HPP
#define TREEINFO_HPP

#include "SP_structure.hpp"
using namespace std;
/***Printing the Tree via Parent Relations ***/

void print_Parent(vector<RT_Vertex> *CRT)
{
    for(int x=0;x<CRT->size();x++)
    {printf("%d is child of %d, at Level %d, Edgewt %f with maxWt (%d %d %f ) and root %d\n ", x, CRT->at(x).Parent, CRT->at(x).Level, CRT->at(x).EDGwt, CRT->at(x).maxE.node1, CRT->at(x).maxE.node2, CRT->at(x).maxE.edge_wt, CRT->at(x).root);}
    
    return;
}
/*** END of Function ***/


/***Get Last Parents ***/
int route_to_root (int x, vector<bool> *isroot, A_Network *X, vector<RT_Vertex> *CRT)
{
    
    int lp=x;
    while(1)
    {
     
       // printf("%d...%d \n", lp,CRT->at(lp).Parent);
        if(isroot->at(lp)==true){break;}
        if(CRT->at(lp).Parent==lp) {break;}
        lp=CRT->at(lp).Parent;
        if(lp==-1){break;}
    }
    
    
    return lp;
}
/** End of Function ****/


/***Find maxE and Level of node from root **/
void find_maxE_lvl(int v,  vector<RT_Vertex> *CRT)
{
   // printf("%d --\n",v);
    int last_node=v;
    int myp, thisn,thisp;
    double mywt;
    Edge mye;
    
    vector<int> visited_nodes;
    visited_nodes.clear();
    visited_nodes.push_back(last_node);
    
    //Path ends if we reach root
    int root=-1;
    while(1)
    {
       // printf("%d--::%d %d...\n", last_node, myp, CRT->at(last_node).Parent);
        myp=CRT->at(last_node).Parent;
        mywt=CRT->at(last_node).EDGwt;
        
       // printf("xxx \n");
        
        //Store Parents until we find one with all info
        visited_nodes.push_back(myp);
        
       // printf("yyy %d %d\n", myp, CRT->at(myp).Level);
        
        //break when found node with valid Level
        if(CRT->at(myp).Level!=-1)
        {
            //printf("zzzzyy \n");
            root=CRT->at(myp).root;
            break;}
        
        //printf("zzzz %d\n", root);

        last_node=myp;
        
    }//end of while
    
    //printf("ttt %d\n",root);
    
    //Update levels of all ancestors until root
    for(int i=visited_nodes.size()-1;i>0;i--)
    {
        thisn=visited_nodes[i-1];
        thisp=visited_nodes[i];
        
       //printf("%d  %d \n", thisn, thisp);
        
        CRT->at(thisn).Level=CRT->at(thisp).Level+1; //set level
        CRT->at(thisn).root=root; //set root
       
        
        //Mark maxE if it is higher
        if(CRT->at(thisn).EDGwt > CRT->at(thisp).maxE.edge_wt)
        {
            mye.node1=thisp;
            mye.node2=thisn;
            mye.edge_wt=CRT->at(thisn).EDGwt;
            
            CRT->at(thisn).maxE=mye;
        }
        
        else
        {
            //printf("here \n");
            CRT->at(thisn).maxE=CRT->at(thisp).maxE;}
        
    }//end of for
    //printf("lll %d %d \n", CRT->at(v).maxE.node1,CRT->at(v).maxE.node2 );
    
    //print_Parent(CRT);
    
    return;
}
/**** END OF FUNCTION ***/



//Find the maximum weighted edge in the path between node1 and node2, that have a common path to the root that passes thru vertex commonN
void findmaxEinfork (int node1, int node2,  vector<RT_Vertex> *CRT, Edge *maxE1, Edge *maxE2)
{
    
   //max weighted edge for node1
    maxE1->edge_wt=-1;
     //max weighted edge for node2
    maxE2->edge_wt=-1;
    
    //find the minimum common level
    int min_level;
    if(CRT->at(node1).Level<=CRT->at(node2).Level)
    {min_level=CRT->at(node1).Level;}
    else
    {min_level=CRT->at(node2).Level;}
    //printf("%d \n", min_level);
    
    //Traverse each node until we reach min_level
    //For node1
    int ln1=node1;
    while(1)
    {
        //Update Level
        int Level1=CRT->at(ln1).Level;
        if(Level1<=min_level) {break;}
        
        //Update max weight
        if(maxE1->edge_wt<CRT->at(ln1).EDGwt)
           {maxE1->node1=CRT->at(ln1).Parent;
            maxE1->node2=ln1;
            maxE1->edge_wt=CRT->at(ln1).EDGwt;}
        
        //Go up to parent
        ln1=CRT->at(ln1).Parent;
        
        if(ln1==node2){break;} //break if found other end
    }
   // printf("klllZZ \n");
    //For node2
    int ln2=node2;
    while(1)
    {
        int Level2=CRT->at(ln2).Level;
        if(Level2<=min_level) {break;}
        

        //Update max weight
        if(maxE2->edge_wt<CRT->at(ln2).EDGwt)
           {maxE2->node1=CRT->at(ln2).Parent;
            maxE2->node2=ln2;
            maxE2->edge_wt=CRT->at(ln2).EDGwt;}
      
        //Go up to parent
        ln2=CRT->at(ln2).Parent;
        
     
        
        if(ln2==node1){break;} //break if found other end
    }
    
   // printf("klll \n");
    //now nodes are at the same level --decrease them simultaneously
    while(1)
    {
        //break when found common ancestor
        if(ln1==ln2) {break;}
        
        //=====Move up for node1
        //Update max weight
        if(maxE1->edge_wt<CRT->at(ln1).EDGwt)
        {maxE1->node1=CRT->at(ln1).Parent;
            maxE1->node2=ln1;
            maxE1->edge_wt=CRT->at(ln1).EDGwt;}
        
        
        //Go up to parent
        ln1=CRT->at(ln1).Parent;
        
        
        //=====Move up for node2
        //Update max weight
        if(maxE2->edge_wt<CRT->at(ln2).EDGwt)
        {maxE2->node1=CRT->at(ln2).Parent;
            maxE2->node2=ln2;
            maxE2->edge_wt=CRT->at(ln2).EDGwt;}
        
        //Go up to parent
        ln2=CRT->at(ln2).Parent;
        
        if(ln2==CRT->at(ln2).Parent){break;}
    }
    // printf("klllXX \n");
        return;
}
/**** End of Function*****/

//If two edges have same weight--use tiebreaker
//Edges(a1 b1) and (a2 b2). Select if a1+i*b1 < a2+i*b2; i=0,1...
void tie_break_edge(vector<RT_Vertex> *CRT,Edge *maxEh1,Edge *maxEh2, Edge *maxE)
{
    
    if(CRT->at(maxEh1->node1).Level > CRT->at(maxEh2->node1).Level)
    { *maxE=*maxEh1;
        return;}
    
    
    if(CRT->at(maxEh1->node1).Level < CRT->at(maxEh2->node1).Level)
    {*maxE=*maxEh2;
        return;
    } //end of if
    
    //If levels are equal
    int a1=maxEh1->node1;
    int b1=maxEh1->node2;
    
    int a2=maxEh2->node1;
    int b2=maxEh2->node2;
    
    int c1=0;
    int c2=0;
    
    int i=1;
    while(1)
    {
        c1=a1+i*b1;
        c2=a2+i*b2;
        
        if(c1==c2){i++;}
        if(c1<c2)
        {*maxE=*maxEh1; break;}
        if(c1>c2)
        {*maxE=*maxEh2; break;}
        
    }
    
    return;
}


#endif
