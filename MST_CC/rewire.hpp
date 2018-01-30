#ifndef REWIRE_HPP
#define REWIRE_HPP

#include "SP_structure.hpp"
#include "tree_info.hpp"
using namespace std;




/**** Updating the maxE of children  ***/
/** Someof the nodes may have false parents or neutral edges **/
void update_children_maxE(A_Network *X, vector<RT_Vertex> *MST, int v)
{
    
    list<int> NodesQ;
    NodesQ.clear();
    //Add v to NodesQ
    NodesQ.push_back(v);
    
    int nodes=MST->size();
    Edge mye;
    
    vector<int> visited;
    visited.clear();
    visited.resize(nodes,false);
    
    
    //Only visited nodes can be parents;
    visited[v]=true;
    visited[MST->at(v).Parent]=true;
    
    
    while(!NodesQ.empty())
    {
        int thisn=NodesQ.front();
        NodesQ.pop_front();
        
          //printf("%d... \n", thisn);
        
        for(int i=0;i<X->at(thisn).ListW.size();i++)
        {
            int myn=X->at(thisn).ListW[i].first;
            
            //If visited continue
            if(visited[myn]){continue;}
            
            //If child alread has maxE as the deleted edge,
            //then its children will also have that
            //We do not need to update further down the line
           if(MST->at(myn).maxE==MST->at(thisn).maxE){continue;}
            
            //If a child --then update maxE
            if(MST->at(myn).Parent==thisn)
            {
                MST->at(myn).maxE=MST->at(thisn).maxE;
                visited[myn]=true;
                
                //Put in Q
                if(MST->at(myn).degree>1)
                {NodesQ.push_front(myn);}
            }
        } //end of for i--going thru Neighbors
        
    }//end of while for BFS
    
    
    return;
}
/**** END OF FUNCTION ***/

/**** Find true parent of v and its children ***/
/** Some of the nodes may have false parents or neutral edges--this is to fix that **/
void set_parent(int v, vector<bool> *isroot, A_Network *X, vector<RT_Vertex> *MST)
{
    
    list<int> NodesQ;
    NodesQ.clear();
    //Add v to NodesQ
    NodesQ.push_back(v);
    
    int nodes=MST->size();
    vector<int> visited;
    visited.clear();
    visited.resize(nodes,false);
    
    bool found_p=false;
    while(!NodesQ.empty())
    {
        int thisn=NodesQ.front();
        NodesQ.pop_front();
        
        visited[thisn]=true;
         //printf("%d...%d  \n", thisn, MST->at(thisn).Level);
        
        for(int i=0;i<X->at(thisn).ListW.size();i++)
        {
            int myn=X->at(thisn).ListW[i].first;
            double mywt=X->at(thisn).ListW[i].second;
            
            //If the edge is marked as deleted--continue
            if(mywt==-1){continue;}
            //If the edge is visited--continue
            if(visited[myn]){continue;}
            
            //If marked as child continue
            if(MST->at(myn).Parent==thisn) {continue;}
            
            //==any neighbor found after this is unmarked or possible parent
            //If true parent found
            // set current node as parent of neighbor
            // put node to be processed later
             if(found_p)
             {  MST->at(myn).Parent=thisn;
                 MST->at(myn).EDGwt=mywt;
                 NodesQ.push_back(myn);
                 continue;
             }
            
            //Othherwise check if parent is true parent
            //should have a route to root
            int lp=route_to_root(myn,isroot,X, MST);
            
               //If lp>-1 true parent;
               //set as myn as parent and mark parent found
               if(lp>-1)
               {   MST->at(thisn).Parent=myn;
                   MST->at(thisn).EDGwt=mywt;
                   isroot->at(thisn)=true;
                    found_p=true;}
    
             // no use checking further down if isroot is false
            
        } //end of for i--going thru Neighbors
        
    }//end of while for BFS
    
    
    return;
}

/**** END OF FUNCTION ***/


//Update the Parent,Level and maxE after new edges inserted in tree
void rewire_Parent(vector<Edge> *deleted_edges, vector<int> *updated_nodes,vector<RT_Vertex> *MST, A_Network *X, int p)
{
    
    //First delete the edges in MST by marking parents as -1
    for(int i=0;i<deleted_edges->size();i++)
    {
        Edge delE=deleted_edges->at(i);
        
        //Delete the parent relation
          if(MST->at(delE.node2).Parent==delE.node1)
         {MST->at(delE.node2).Parent=-1;}
         if(MST->at(delE.node1).Parent==delE.node2)
         {MST->at(delE.node1).Parent=-1;}
        
        
    }
    
    
    
   // printf("Inside rewire \n");
    //Find unique set of updated nodes
    sort(updated_nodes);
    *updated_nodes=unique(*updated_nodes);
    
    int nodes=MST->size();
    vector<bool> isroot;
    isroot.resize(nodes, false);
    
    //Find the correct parent of affected nodes
    //do this by finding the path to the root
    bool change=false;
    while(1)
    {
        change=false;
#pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int i=0;i<updated_nodes->size();i++)
        {
            if(isroot[i]){continue;}
            
            change=true;
            int x=updated_nodes->at(i);
            
           // printf("%d --\n",x);
            set_parent(x, &isroot, X, MST);
        }//end of for
        
        if(!change){break;}
    }
    printf("tree done \n");
    
    

    return;
}
/**** End of Function*****/


#endif

    
