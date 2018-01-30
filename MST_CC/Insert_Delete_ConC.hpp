#ifndef INSERTDELETE_CONC_HPP
#define INSERTDELETE_CONC_HPP

#include "SP_structure.hpp"
#include "tree_info.hpp"
#include "rewire.hpp"
using namespace std;

//Classify the Edges and Process the Insertions
void classify_insert(vector<xEdge> *allChange, A_Network *X,vector<RT_Vertex> *ConC, vector<Edge> *reBs, map_Edge_int *Edgemap, vector<Edge> *deleted_edges, vector<int> *updated_nodes, int highV, double max_val, double maxW, bool *dels, int root, int p)
{
    
    //Mark how the edge is processed
    vector<int> Edgedone;
    Edgedone.clear();
    Edgedone.resize(allChange->size(),0);
    
    //Mark the edge that replaces corresponding edge
    Edge dummyE;
    dummy_edge(&dummyE);
    vector<Edge> maxEdges;
    maxEdges.clear();
    maxEdges.resize(allChange->size(),dummyE);
    
    printf("Classify %d %d  \n", allChange->size(), Edgedone.size());
    
    
#pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int xe=0; xe<allChange->size(); xe++)
    {
        xEdge myxE=allChange->at(xe);
        Edge mye=myxE.theEdge; //get the edge
        

        int ins=myxE.inst; //check whether it is add or delete
       // printf("%d %d %d %f %f===\n", ins, mye.node1, mye.node2, mye.edge_wt, max_val);
        //printf("%d %d %d %d ===\n", mye.node1, mye.node2, ConC->at(mye.node1).root, ConC->at(mye.node2).root);
        //If Insert
        if(ins==1)
        {
            //Check if edge exists in the tree
            //this will happen if node1 is parentof node or vice-versa
            if( (ConC->at(mye.node1).Parent==mye.node2)|| (ConC->at(mye.node2).Parent==mye.node1))
            {
                Edgedone[xe]=4; //mark to not add--edge exists
                continue;}
            
            //insert if nodes are in different trees
            if(ConC->at(mye.node1).root != ConC->at(mye.node2).root)
            { Edgedone[xe]=1; //mark edge to be added
                
                continue;}
            
        } // end of if Insert
        
        
        //Else Delete
        else
        {
            //Check if edge exists in the tree
            //this will happen if node1 is parentof node or vice-versa
            if( (ConC->at(mye.node1).Parent==mye.node2)|| (ConC->at(mye.node2).Parent==mye.node1))
            {
                *dels=true;
                int delW=maxW+1;
                Edge maxEdge;
                Edgedone[xe]=3;
                
              // printf("delete %d %d %d\n", mye.node1, mye.node2, delW);
                
                //mark deleted edges with high value
                if(ConC->at(mye.node1).Parent==mye.node2)
                {ConC->at(mye.node1).EDGwt=delW;
                    maxEdge.node1=mye.node2;
                    maxEdge.node2=mye.node1;
                    maxEdge.edge_wt=delW;
                    ConC->at(mye.node1).maxE=maxEdge;
                 // update_children_maxE(X,ConC, mye.node1);
                }
                
                if(ConC->at(mye.node2).Parent==mye.node1)
                {ConC->at(mye.node2).EDGwt=delW;
                    maxEdge.node1=mye.node1;
                    maxEdge.node2=mye.node2;
                    maxEdge.edge_wt=delW;
                    ConC->at(mye.node2).maxE=maxEdge;
                   // update_children_maxE(X, ConC, mye.node2);
                }
                continue;
            }//end of if
            else
            {
                //Not part of Key Edges
                //Mark to be added to deleted other edges
                Edgedone[xe]=5;
                
                continue;
            }//end of else
            
        }//end of Else Delete
        
        
    }//end of for--going through the inputs
    
    
    vector<xEdge> newChange;
    newChange.clear();
    
    
    int del_edges=0;
    int ins_edges=0;
    int del_remainder=0; //number of remainder deleted edges mapped
    std::pair<std::map<Edge,int>::iterator,bool> ret;
    
    //TBA: Probably a switch would be better here
    for(int xe=0; xe<allChange->size(); xe++)
    {
        if(Edgedone[xe]==4) {continue;}//This edge already exists
        if(Edgedone[xe]==3) {del_edges++; continue;} //Edge has been deleted
        
        
        if(Edgedone[xe]==5) //Store remainder deleted edge in map
        {
            // printf("Mapping deleted edges \n");
            Edge myedge=allChange->at(xe).theEdge;
            ret=Edgemap->insert ( std::pair<Edge,int>(myedge,del_remainder) );
            if(ret.second==true){del_remainder++;}
            continue;
        }
        
        if(Edgedone[xe]==2) //Add to Remainder Edges in Appropriate Bucket
        {
            Edge myedge=allChange->at(xe).theEdge;
            
            //Have lower id node as node1
            Edge MyE;
            if(myedge.node1 <myedge.node2)
            {MyE.node1=myedge.node1;
                MyE.node2=myedge.node2;}
            else
            {MyE.node1=myedge.node2;
                MyE.node2=myedge.node1;}
            MyE.edge_wt=myedge.edge_wt;
            
            reBs->push_back(MyE);
            
            continue;
        }
        
        if(Edgedone[xe]==1) //Add edge and replace with another edge
        {
            ins_edges++;
                //Increase degree
                Edge myedge=allChange->at(xe).theEdge;
                ConC->at(myedge.node1).degree++;
                ConC->at(myedge.node2).degree++;
                updated_nodes->push_back(myedge.node1);
                updated_nodes->push_back(myedge.node2);
                
                //Mark added myedge.node1--myedge.node2
                int_double cVal;
                cVal.first=myedge.node2;
                cVal.second=myedge.edge_wt;
                X->at(myedge.node1).ListW.push_back(cVal);
                
                
                cVal.first=myedge.node1;
                cVal.second=myedge.edge_wt;
                X->at(myedge.node2).ListW.push_back(cVal);
                //End of Adding Edges*/
            
            continue;
        }//end of if Edgedone[xe]==1
        
    }//end of for--going through the changed edges
    
   
    
    printf("Deleted Edges: %d \n", del_edges);
     printf("Processed Inserts: %d\n ", ins_edges);
    
    return;
}
/**** End of Function*****/



//Process deletions to add edges
void process_deletions(A_Network *X,vector<RT_Vertex> *ConC, vector<Edge> *reBs, map_Edge_int *Edgemap, vector<Edge> *deleted_edges, vector<int> *updated_nodes, int highV, double max_val, double maxW, int root,int p)
{
 
    
   // printf("stsrrst \n");
    //First process the maxE for the nodes we will check for rejoining
    vector<bool> maxEdone;
    maxEdone.resize(ConC->size(),false);
    
    #pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int xe=0; xe<reBs->size(); xe++)
    {
        Edge mye=reBs->at(xe); //get the edge
        
        if(!maxEdone[mye.node1])
        {
           find_maxE_lvl(mye.node1, ConC);
            maxEdone[mye.node1]=true;}
        
        if(!maxEdone[mye.node2])
        {find_maxE_lvl(mye.node2, ConC);
            maxEdone[mye.node2]=true;}
    }
    
  //  printf("done updating maXE \n");
    
    //Mark how the edge is processed
    vector<int> Edgedone;
    Edgedone.clear();
    Edgedone.resize(reBs->size(),0);
    
    //Mark the edge that replaces corresponding edge
    Edge dummyE;
    dummy_edge(&dummyE);
    vector<Edge> maxEdges;
    maxEdges.clear();
    maxEdges.resize(reBs->size(),dummyE);

    double delW=(double)(maxW+1); //weight with which deleted edges are marked
    
   // printf("%d--\n",reBs->size());
    
    //Go over the remainder edges
#pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int xe=0; xe<reBs->size(); xe++)
        {
        
            
            Edge mye=reBs->at(xe); //get the edge
        
           // printf("%d %d %f ===\n",  mye.node1, mye.node2, mye.edge_wt);
            if(mye.node1> mye.node2)
            {Edgedone[xe]=2;
                continue;} //no need to process duplicate edges
            //We are only looking at possible insertions
            
            
            //Cancel the singleton vertices
            if((ConC->at(mye.node1).Level==-1)||(ConC->at(mye.node2).Level==-1))
            { Edgedone[xe]=2;
                continue;}
            
            //Start of Code for finding maxE
            //Find maximum weighted edge
            Edge maxE, maxEh1, maxEh2;
            maxE=dummyE;
            maxEh1=dummyE;
            maxEh2=dummyE;
            
            
            
            //Find maxE in path from node1 and node2
            //For node1 --find maxEh1 in path from node1 to root
            //If root --then already set to dummyE
           // if(mye.node1!=root)
                if(ConC->at(mye.node1).Parent!=mye.node1)
            { maxEh1=ConC->at(mye.node1).maxE;
                
                //If maxEh1 not set find it
                if(maxEh1.edge_wt==-1)
                { find_maxE_lvl(mye.node1, ConC);}
                maxEh1=ConC->at(mye.node1).maxE;
                
               // printf("kkk %d %d %f %d\n", maxEh1.node1, maxEh1.node2, maxEh1.edge_wt,ConC->size());
               
                //check that maxEh1 is current
                if(maxEh1.node1>-1 && maxEh1.node2>-1)
                {
                if( (ConC->at(maxEh1.node1).Parent!=maxEh1.node2) && (ConC->at(maxEh1.node2).Parent!=maxEh1.node1))
                {find_maxE_lvl(mye.node1, ConC);}
                //printf("kkkmmm %d %d %f %d\n", maxEh1.node1, maxEh1.node2, maxEh1.edge_wt,ConC->size());
                maxEh1=ConC->at(mye.node1).maxE;
                }
            }
          //  printf("hhhrrX \n");
            //For node2--find maxEh2 in path from node1 to root
            //If root --then already set to dummyE
           // if(mye.node2!=root)
            if(ConC->at(mye.node2).Parent!=mye.node2)
            { maxEh2=ConC->at(mye.node2).maxE;
                
                //If maxEh2 not set find it
                if(maxEh2.edge_wt==-1)
                {find_maxE_lvl(mye.node2, ConC);}
                 maxEh2=ConC->at(mye.node2).maxE;
                
                if(maxEh2.node1>-1 && maxEh2.node2>-1)
                {
                //check that maxEh2 is current
                if( (ConC->at(maxEh2.node1).Parent!=maxEh2.node2) && (ConC->at(maxEh2.node2).Parent!=maxEh2.node1))
                {find_maxE_lvl(mye.node2, ConC);}
                
                maxEh2=ConC->at(mye.node2).maxE;
                }
            }
            
           // printf("hhhrrY \n");
            //If neither of the edges are maxW+1, then continue
            if((maxEh1.edge_wt<delW)&&(maxEh2.edge_wt<delW))
            { Edgedone[xe]=2;
                continue;}
            
            //If edges are different--the maxedges will give ultimate maxE. No need to find path
            //If edges are same the path between node1 and node2 does not contain delW
            if(maxEh1==maxEh2)
            { Edgedone[xe]=2;
                continue;}
           // printf("camama \n");
            //===At this point both max edges should exist and one should be a deleted edge===
            
            //Tie-break if values are same to avoid race condition 2
            if(maxEh1.edge_wt==maxEh2.edge_wt)
            {tie_break_edge(ConC,&maxEh1,&maxEh2, &maxE);}
            else
            {
                if(maxEh1.edge_wt>maxEh2.edge_wt)
                {maxE=maxEh1;}
                else
                {maxE=maxEh2;}
                
            }//end of else
            
            //printf("here \n");
            //====At this point node1 and node2 are connected and maxE is known
            
            //Check that the remainder edge is not deleted. If deleted cannot be used
            bool got_deleted=false;
            std::map<Edge,int>::iterator it;
            it=Edgemap->find(maxE);
            if(it!=Edgemap->end())
            { got_deleted=true;}
            if(got_deleted) //this edge cannot be used to connect
            {Edgedone[xe]=2;
                continue; }
            
            
            // printf("To be replaced %d %d %f \n", maxE.node1, maxE.node2, maxE.edge_wt);
            //If we reach this point then node1 and node2 are connected and maxE is known
            //This max edge wt will always be greater than edge to be inserted beacuse it is delW
            Edgedone[xe]=1; //mark edge to be added
            
            //not marked
            if(ConC->at(maxE.node2).marked==-1)
            {
                ConC->at(maxE.node2).marked=xe; //mark the edge replacing it
                maxEdges[xe]=maxE;
            }
            else //already marked
            {
                //check if marked edge has higher weight
                int marked_index=ConC->at(maxE.node2).marked;
                Edge marked_edge=reBs->at(marked_index);
                
                //if marked edge is heavier--replace with current edge
                if(marked_edge.edge_wt>mye.edge_wt)
                {
                    ConC->at(maxE.node2).marked=xe; //mark the edge replacing it
                    maxEdges[xe]=maxE;
                    Edgedone[marked_index]=2; //do not add
                }
                else{
                    Edgedone[xe]=2; //mark to not add--edge exists
                }
            }//end of else

        }
    
    
    //TBA: Probably a switch would be better here
        for(int xe=0; xe<reBs->size(); xe++)
        {
           
            if(Edgedone[xe]==1) //Add edge and replace with another edge
            {
                
                //Get the edge
                Edge maxE=maxEdges[xe];
                
                // printf("%d %d %d .....\n", xe, maxE.node1, maxE.node2);
                
                //Check if maxE marked by the current change edge
                if(ConC->at(maxE.node2).marked==xe)
                {
                    
                    deleted_edges->push_back(maxE);
                    
                    //Delete the parent relation for maxE.node2
                    //Reduce degree
                    ConC->at(maxE.node1).degree--;
                    ConC->at(maxE.node2).degree--;
                    
                    //Mark deleted maxE.node1--maxE.node2 --set edge_wt to -1
                    for(int i=0;i<X->at(maxE.node1).ListW.size();i++)
                    {
                        int y=X->at(maxE.node1).ListW[i].first;
                        if(y==maxE.node2)
                        {
                            //Edge_wt set to -1 to mark as deleted
                            X->at(maxE.node1).ListW[i].second=-1;
                            break;
                        }
                    }//end of for
                    
                    for(int i=0;i<X->at(maxE.node2).ListW.size();i++)
                    {
                        int y=X->at(maxE.node2).ListW[i].first;
                        if(y==maxE.node1)
                        {
                            //Edge_wt set to -1 to mark as deleted
                            X->at(maxE.node2).ListW[i].second=-1;
                            break;
                        }
                    }//end of for
                    //End of Deleting Edges
                    
                    //Store end points ofupdated nodes
                    //Increase degree
                    Edge myedge=reBs->at(xe);
                    ConC->at(myedge.node1).degree++;
                    ConC->at(myedge.node2).degree++;
                    updated_nodes->push_back(myedge.node1);
                    updated_nodes->push_back(myedge.node2);
                    
                    // printf("Replaced EdgeZZZZZ %d %d %f\n", myedge.node1, myedge.node2, myedge.edge_wt);
                    
                    //Mark added myedge.node1--myedge.node2 --set edge_wt to -1
                    int_double cVal;
                    cVal.first=myedge.node2;
                    cVal.second=myedge.edge_wt;
                    X->at(myedge.node1).ListW.push_back(cVal);
                    
                    
                    cVal.first=myedge.node1;
                    cVal.second=myedge.edge_wt;
                    X->at(myedge.node2).ListW.push_back(cVal);
                    //End of Adding Edges
                }
            }//end of if Edgedone[xe]==1
            
        }//end of for--going through the remainder edges
        
        
    
    return;
}
/**** End of Function*****/



#endif

    
