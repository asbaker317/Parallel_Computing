#ifndef KRUSKAL_MST_HPP
#define KRUSKAL_MST_HPP

//INPUT HEADERS
#include "translate_from_input.hpp"
#include "input_to_network.hpp"
#include"structure_defs.hpp"

#include "ADJ/traversal.hpp"

//OUTPUT HEADERS
#include "printout_network.hpp"
#include "printout_others.hpp"

//PERMUTE
#include "permute_elements.hpp"
#include "ADJ/permute_vertex.hpp"
#include "SP_structure.hpp"


//NOTES: Assuming that the graph is undirected
using namespace std;



void kruskal_MST(A_Network *Y, A_Network *Y1)
{       
	//Components for vertices
    vector<int> component;
    component.resize(Y->size(),-1);
    
    //List of Edges
    vector<Edge> allEdges;
    allEdges.clear();
    
    Edge mye;
    ADJ_Bundle AList;;
    
   //Preprocessing
    for(int i=0;i<Y->size();i++)
    { component[i]=i; //initialize each edge in its component
     
        
        //Put the edges in a vector
        for(int j=0;j<Y->at(i).ListW.size();j++)
        {
            
            //Only process in one direction
            if(i> Y->at(i).ListW[j].first) {continue;}
            
            mye.node1=i;
            mye.node2=Y->at(i).ListW[j].first;
            mye.edge_wt=Y->at(i).ListW[j].second;
            allEdges.push_back(mye);
        }
        
        //Create Rows for Y1;
        AList.Row=i;
        AList.ListW.clear();
        Y1->push_back(AList);
    }//end of for
    
    
    
    //Sort the Edges
    sort(&allEdges,inc_wt);
   
    
    
  //Kruskal's Algorithn
    int join=1; //number of connected nodes;
    int_double myval;
    
    int x=0;
    int old_join=0;
  
    while (x<allEdges.size())
    {
        
     //Find lowest weight edge
        
        mye=allEdges[x];
        x++;
       
        
     
        //Check if the nodes are already joined in the MST
        int f1=find_head(&component, mye.node1);
        int f2=find_head(&component, mye.node2);
        
        
        if(f1!=f2)
        {
            join++; //increase join
            
            //Update Component
            if(f1<f2)
            { component[f2]=f1;}
            
            else
            { component[f1]=f2;}
            
        
            //Add to Y1
            myval.first=mye.node2;
            myval.second=mye.edge_wt;
            Y1->at(mye.node1).ListW.push_back(myval);
            
            
            myval.first=mye.node1;
            myval.second=mye.edge_wt;
            Y1->at(mye.node2).ListW.push_back(myval);
            
        }//end of if
    
         f1=find_head(&component, mye.node1);
        f2=find_head(&component, mye.node2);
        
        old_join=join;
    }//end of while
    
    
    //Sort the edge lists in  Y1
    for(int i=0;i<Y1->size();i++)
    {
        sort(&Y1->at(i).ListW);
        
    }//end of for
    
    
    return;
}//end of function
	
#endif

