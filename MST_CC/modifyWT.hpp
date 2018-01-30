#ifndef MODIFYWT_HPP
#define MODIFYWT_HPP

#include "SP_structure.hpp"
#include "tree_info.hpp"
#include "rewire.hpp"

//Comment out the ones not used
//TBA: Learn to use macros
//#include "Insert_Delete_MST.hpp"
#include "Insert_Delete_ConC.hpp"


using namespace std;


//Process  Changed Edges
void WT_updates (vector<xEdge> *allChange, A_Network *X,vector<RT_Vertex> *CRT, vector<Edge> *reBs, map_Edge_int *Edgemap,vector<Edge> *deleted_edges, vector<int> *updated_nodes, int highV,double max_val, double maxW, bool *dels, int root, int p)
{
    double startx, endx, start1, end1;
    
    
    int max_val_pnt=0;
    int myins=0;
    int mydel=0; //number of edges deleted from key edges
    vector<Edge> delEdges; //key edges that are deleted
    delEdges.clear();
    
    
    
     //  print_Parent(CRT);
#ifdef POWER_PROFILING_WT
    power_rapl_t ps;
    power_rapl_init(&ps);
    printf("Starting Classify and Insert\n");
    power_rapl_start(&ps);
#endif
    startx=omp_get_wtime();
    classify_insert (allChange,X,CRT, reBs, Edgemap, deleted_edges, updated_nodes, highV, max_val, maxW, dels,root, p);
    endx=omp_get_wtime();
#ifdef POWER_PROFILING_WT
    power_rapl_end(&ps);
    power_rapl_print(&ps);
#endif
    printf("Time for Classify and insert  %f \n", endx-startx);

    
    //This is to count the number of deleted edges
    //int del_edge=0;
    //del_edge_count(CRT, &del_edge);
    
   // print_network(*X);
    
    
#pragma omp parallel for num_threads(p)
    for(int x=0;x<CRT->size();x++)
    {
       // int maxEw=CRT[x].EDGwt;
        //if((maxwt<maxEw)&& (maxEw<delW))
        //{maxwt=maxEw;}
        CRT->at(x).marked=-1;
        
    }//end of pragma for
    
#ifdef POWER_PROFILING_WT
    power_rapl_init(&ps);
    printf("Starting process_deletions\n");
    power_rapl_start(&ps);
#endif
    startx=omp_get_wtime();
    //continue only if any active deleted edges
   if(*dels)
    { process_deletions(X,CRT,reBs, Edgemap, deleted_edges, updated_nodes, highV,max_val, maxW, root,p);}
    endx=omp_get_wtime();
#ifdef POWER_PROFILING_WT
    power_rapl_end(&ps);
    power_rapl_print(&ps);
#endif
    printf("Time for Deletion  %f \n", endx-startx);
    
    
    
}//end of updates
/**** End of Function*****/

#endif

    
