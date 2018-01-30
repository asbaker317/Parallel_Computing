#ifndef READIN_DATA_HPP
#define READIN_DATA_HPP


#include "SP_structure.hpp"
//#include "minimum_spanTX.hpp"
using namespace std;


void readin_changes(char *myfile,  vector<xEdge> *allChange)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    xEdge myedge;
    int ins;

    graph_file=fopen(myfile, "r");
    while(fgets(line,128,graph_file) != NULL)
    {
        //Read line
        sscanf(line,"%d %d %lf %d",&myedge.theEdge.node1,&myedge.theEdge.node2,&myedge.theEdge.edge_wt, &ins);
        if(ins==1)
        {myedge.inst=true;}
        else
        {myedge.inst=false;}
        
        // myedge.theEdge.edge_wt=((int)(ceil(myedge.theEdge.edge_wt))%max_val+1);
        allChange->push_back(myedge);

          }//end of while
    fclose(graph_file);
    
    return ;
}//end of function




void readin_remainder(char *myfile,  int highV, vector<Edge> *reBs)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    Edge myedge;
    int ins;
    
    graph_file=fopen(myfile, "r");
    while(fgets(line,128,graph_file) != NULL)
    {
        //Read line
        sscanf(line,"%d %d %lf",&myedge.node1,&myedge.node2,&myedge.edge_wt);
        
       
        
        if(myedge.node1>myedge.node2) {continue;}
        if(myedge.edge_wt> double(highV)){continue;}
       
         //printf("%d %d ===\n", myedge.node1, myedge.node2);
        reBs->push_back(myedge);
        
        
    }//end of while
    fclose(graph_file);
    
    
    
    
    return ;
}//end of function*/

	//==========================================//
	
	
#endif
