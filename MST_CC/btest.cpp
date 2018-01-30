#ifndef btest_cpp
#define btest_cpp

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

#include<omp.h>


//NOTES: Assuming that the graph is undirected
using namespace std;

void boruvka(A_Network *Y, A_Network *Y1, int p, int batch_size)
{
    //Components for vertices
    vector<int> component;
    component.resize(Y->size(),-1);

    //List of Edges
    vector<Edge> allEdges;
    allEdges.clear();

    Edge mye;
    ADJ_Bundle AList;;
    int i;
    int j;
    int k;
    int l;
    int m;


    int *min_weight = new int[Y->size()];

    //Preprocessing
//#pragma omp parallel for num_threads(p) schedule(dynamic) private(i)
    for(i=0;i<Y->size();i++)
    {
        component[i]=i; //initialize each edge in its component

        min_weight[i] = -1;
        //Put the edges in a vector
       // #pragma omp parallel for num_threads(p) schedule(dynamic) shared(mye, Y) private (j)
        for(j=0;j<Y->at(i).ListW.size();j++)
        {
            //Only process in one direction
            if(i> Y->at(i).ListW[j].first) {continue;}
                mye.node1 = i;
                mye.node2 = Y->at(i).ListW[j].first;
                mye.edge_wt = Y->at(i).ListW[j].second;
                allEdges.push_back(mye);
        }

        //Create Rows for Y1;
        AList.Row=i;
        AList.ListW.clear();
        Y1->push_back(AList);
    }//end of for

    //Sort the Edges
    sort(&allEdges,inc_wt);

    //Boruvka's Algorithn
    int join=1; //number of connected nodes;
    int_double myval;

    int x=0;
    int old_join=0;

    int numTrees = Y->size();
    int MSTweight = 0;

    while (numTrees > 1) {

        //Find lowest weight edge
        mye = allEdges[x];
        //x++;
        //int f1;
        //int f2;
#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
        for (k = 0; k < allEdges.size(); k++)
        {

            //mye.node1=i;
            //mye.node2=Y->at(i).ListW[j].first;
            //mye.edge_wt=Y->at(i).ListW[j].second;



            //#pragma omp critical
            //Check if the nodes are already joined in the MST
            int f1 = find_head(&component, mye.node1);
            int f2 = find_head(&component, mye.node2);

            if (f1 == f2)
                continue;

            else
            {
                join++; //increase join

                //Update Component
                // if (f1 < f2) { component[f2] = f1; }
                // else { component[f1] = f2; }

                if (min_weight[f1] == -1 || allEdges[min_weight[f1]].edge_wt > mye.edge_wt)
                    min_weight[f1] = mye.edge_wt;

                if (min_weight[f1] == -1 || allEdges[min_weight[f2]].edge_wt > mye.edge_wt)
                    min_weight[f2] = mye.edge_wt;
            }
        }
        // add edges to MST
//#pragma omp parallel for num_threads(p) schedule(dynamic)

        for (l=0; l < Y->at(l).ListW.size(); l++)
        {
            if (min_weight[l] != -1)
            {

                int f1 = find_head(&component, allEdges[min_weight[l]].node1);
                int f2 = find_head(&component, allEdges[min_weight[l]].node2);

                if (f1 == f2)
                    continue;
                //Y1 += allEdges[min_weight[i]].edge_wt;

                //Add to Y1
                myval.first = mye.node2;
                myval.second = mye.edge_wt;
                Y1->at(mye.node1).ListW.push_back(myval);

                myval.first = mye.node1;
                myval.second = mye.edge_wt;
                Y1->at(mye.node2).ListW.push_back(myval);

                numTrees--;
            }
        }

        x++;
        int f1=find_head(&component, mye.node1);
        int f2=find_head(&component, mye.node2);

        old_join=join;
    }//end of while


//#pragma omp parallel for num_threads(p) schedule(dynamic)
    //Sort the edge lists in  Y1
    for(m=0;m<Y1->size();m++)
    {
        sort(&Y1->at(m).ListW);
    }//end of for

    return;
}//end of function

#endif

