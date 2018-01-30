//
//  maxEdgefind.hpp
//  
//
//  Created by Sanjukta Bhowmick on 1/19/17.
//
//

#ifndef CREATETREE_HPP
#define CREATETREE_HPP

#include "SP_structure.hpp"
#include  "tree_info.hpp"
using namespace std;


/**** Rooting the Tree Sequential Code ***/
void set_parents(int root, int v, A_Network *X, vector<RT_Vertex> *CRT)
{
    
    list<int> NodesQ;
    NodesQ.clear();
    //Add v to NodesQ
    NodesQ.push_back(v);
    
    int nodes=CRT->size();
    Edge mye;
    
    while(!NodesQ.empty())
    {
        int thisn=NodesQ.front();
        NodesQ.pop_front();
        
        for(int i=0;i<X->at(thisn).ListW.size();i++)
        {
            int myn=X->at(thisn).ListW[i].first;
            
            //printf("%d ...%d %d\n", thisn, myn, CRT->at(myn).degree);
            double mywt=X->at(thisn).ListW[i].second;
            if(mywt==-1){continue;} //invalid edge
            if(CRT->at(myn).Parent>-1){continue;} //if parent marked then visited
            
            
                //mark the parent
                CRT->at(myn).Parent=thisn; //mark the parent
                CRT->at(myn).EDGwt=mywt; //mark the edgewt
                CRT->at(myn).Level=CRT->at(thisn).Level+1; //mark the Level
                CRT->at(myn).root=root;
                
                //Mark maxE if it is higher
                if(CRT->at(thisn).maxE.edge_wt < mywt)
                {
                    mye.node1=thisn;
                    mye.node2=myn;
                    mye.edge_wt=mywt;
                    CRT->at(myn).maxE=mye;
                }
                else
                { CRT->at(myn).maxE=CRT->at(thisn).maxE;}
                
                //Put in Q
                if(CRT->at(myn).degree>1)
                {NodesQ.push_front(myn);}
            
            
        } //end of for i--going thru Neighbors
        
        
    }//end of while for BFS
    
    return;
}

/**** END OF FUNCTION ***/



/**** Rooting the Tree for Minimum Height  ***/
 void set_parents( A_Network *X, vector<RT_Vertex> *CRT, vector< vector<int> > *degPs,  vector<int> *temp_deg, vector<int> *rootC)
 {
 
     //Process all degree1 nodes
     list<int> NodeQ;
     NodeQ.clear();
     
     rootC->clear();
     
     for(int k=0; k<degPs->size(); k++)
     {
     for(int d=0;d<degPs->at(k).size();d++)
     {
         int x=degPs->at(k)[d];
         if(temp_deg->at(x)>1) {continue;}
         
         for(int i=0;i<X->at(x).ListW.size();i++)
         {
             int y=X->at(x).ListW[i].first;
             double mywt=X->at(x).ListW[i].second;
             
             if(mywt==-1){continue;} //invalid edge
             
             if(CRT->at(y).Parent!=x)
             {
                 CRT->at(x).Parent=y; //this node is parent of degree 1 neighbors
                 CRT->at(x).EDGwt=mywt; //update edge weight
             }
             
             temp_deg->at(y)=temp_deg->at(y)-1;
             
             //If degree reduced to 1 then send to NodeQ for processing
             if(temp_deg->at(y)==1) {NodeQ.push_back(y);}
             
             //If degree reduced to 0 then mark as root
             if(temp_deg->at(y)==0) {rootC->push_back(y);}
         }
     }//end of d
     
     }//end of for k;
     
   
     
     while(!NodeQ.empty())
     {
         int thisn=NodeQ.front();
         NodeQ.pop_front();
         
       //  printf("%d --\n", thisn);
         bool gotP=false;
         
         for(int i=0;i<X->at(thisn).ListW.size();i++)
         {
             int y=X->at(thisn).ListW[i].first;
             double mywt=X->at(thisn).ListW[i].second;
             
              if(mywt==-1){continue;} //invalid edge
         
             if(CRT->at(y).Parent!=thisn)
             {
             CRT->at(thisn).Parent=y; //this node is parent of degree 1 neighbors
             CRT->at(thisn).EDGwt=mywt; //update edge weight
                 gotP=true;
             
             temp_deg->at(y)=temp_deg->at(y)-1;
             
             if(temp_deg->at(y)==1) {NodeQ.push_back(y);}
             }
         }//end of for
     
         //If parent not found this is parent
         if(!gotP){rootC->push_back(thisn);}
     
     }//end of while
 
 }
/**** END OF FUNCTION ***/






/**** Creating the Rooted Tree **/
//Options: Parallel:0; Sequential BFS:1; Sequential optimalheight:2

void create_tree(A_Network *X, vector<RT_Vertex> *CRT, double *maxwt, int *maxV, int type, int p)
{
    int nodes=CRT->size();
    
    //tree where root is given
    if(type==0)
    {
        //Set maxV as root and root the tree
        CRT->at(*maxV).Parent=*maxV;
        CRT->at(*maxV).root=*maxV;
        CRT->at(*maxV).Level=0;
        
        //This is the sequential version made parallel from Level 1
        #pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int i=0;i<X->at(*maxV).ListW.size();i++)
        {
            int y=X->at(*maxV).ListW[i].first;
            double mywt=X->at(*maxV).ListW[i].second;
            
            if(mywt==-1){continue;}
            
            CRT->at(y).Parent=*maxV;
            CRT->at(y).Level=1;
            CRT->at(y).EDGwt=mywt;
            CRT->at(y).root=*maxV;
            
            Edge mye;
            mye.node1=*maxV;
            mye.node2=y;
            mye.edge_wt=X->at(*maxV).ListW[i].second;
            CRT->at(y).maxE=mye;
            
            //Do a BFS for each y to find the parents, Level and maxE
            set_parents(*maxV, y, X, CRT);
        }
        
        }//end of if type 0
    
    
    
    //sequential BFS code-after finding root
    if(type==1)
    {
        
        //Allows for multiple trees
        while(1)
        {
            
            int maxD=-1; //highest degree
            *maxV=-1; //vertex with highest degree
        
            //First find the degree
           #pragma omp parallel for num_threads(p)
            for(int x=0;x<nodes;x++)
            {
                //Only check for nodes whose root is not given
               // printf("%d   %d \n", x, CRT->at(x).root, CRT->at(x).degree);
                if(CRT->at(x).root >-1) {continue;}
                
               CRT->at(x).degree=X->at(x).ListW.size();
                int degree=X->at(x).ListW.size();
                
                //Otherwise find maxDeg
                if(maxD<degree)
                { maxD=degree;
                    *maxV=x;}
                
            }//end of pragma for
            
            //printf("%d---\n",*maxV);
            
            //Done when all nodes have roots
           if(*maxV==-1){break;}
        
        //Set maxV as root and root the tree
        CRT->at(*maxV).Parent=*maxV;
        CRT->at(*maxV).root=*maxV;
        CRT->at(*maxV).Level=0;
        
        //This is the sequential version made parallel from Level 1
        #pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int i=0;i<X->at(*maxV).ListW.size();i++)
        {
            int y=X->at(*maxV).ListW[i].first;
            CRT->at(y).Parent=*maxV;
            CRT->at(y).Level=1;
            CRT->at(y).EDGwt=X->at(*maxV).ListW[i].second;
            CRT->at(y).root=*maxV;
            
            Edge mye;
            mye.node1=*maxV;
            mye.node2=y;
            mye.edge_wt=X->at(*maxV).ListW[i].second;
            CRT->at(y).maxE=mye;
           
            //Do a BFS for each y to find the parents, Level and maxE
            set_parents(*maxV, y, X, CRT);
        }
        
            
        
        }//end of while
        
    }//end of type 1
    
    
    //sequential optimal height
    if(type==2)
    {
        
        vector<int> temp_deg;
        temp_deg.resize(nodes, -1);
    
        vector<int> deg1;
        deg1.clear();
        
        vector< vector<int> > degPs;
        degPs.resize(p, deg1);
        
        vector<int> rootC;
        
#pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int x=0;x<nodes;x++)
        {  temp_deg[x]=CRT->at(x).degree;
        
            if(temp_deg[x]==1)
            {
               int th=omp_get_thread_num();
               degPs[th].push_back(x);
            }
        }
        set_parents(X, CRT, &degPs, &temp_deg, &rootC);
        
   
        //Getroots
        //Nodes with no parent will be roots except where they have a neighbor who also does not have parents
       #pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int j=0;j<rootC.size();j++)
        {
            int x=rootC[j];
            int root=-1;
        
            if(CRT->at(x).Parent==-1)
            {
                root=x;
              //Check if it has a neighbor withough parent
                for(int i=0;i<X->at(x).ListW.size();i++)
                {
                    int y=X->at(x).ListW[i].first;
                    double mywt=X->at(x).ListW[i].second;
                    //Check parent of neighbor
                    if(CRT->at(y).Parent==-1)
                    {//found2=true;
                        
                        if(y<x){root=y;}
                        else {CRT->at(y).Parent=x;
                            CRT->at(y).EDGwt=mywt;
                            CRT->at(x).root=root;
                        }
                        break;}
                }
          
            
            if(x==root)
            {
                CRT->at(x).Parent=root;
                CRT->at(x).Level=0;
                CRT->at(x).root=root;
            }//end of if
                
        }//end of if
            
        }//end of for
    
      
        
        //Get levels and maxE
#pragma omp parallel for num_threads(p) schedule(dynamic)
        for(int x=0;x<nodes;x++)
        {
            if(CRT->at(x).degree==0){continue;}
            
          if(CRT->at(x).Level==-1)
         {find_maxE_lvl(x, CRT);}
        }
 
    }//end of type 2
    
    
}
/**** END OF FUNCTION ***/


#endif
