//
// Created by Alex Baker on 10/17/17.
//
#include <stdio.h>
#include "SP_structure.hpp"
#include "tree_info.hpp"
#include "rewire.hpp"
#include "input_to_network.hpp"
#include"structure_defs.hpp"
#include "ADJ/permute_vertex.hpp"
#include "ADJ/traversal.hpp"
#include "permute_elements.hpp"
#include "Insert_Delete_ConC.hpp"


// structure to represent a subset for union-find
struct subset
{
    int parent;
    int rank;
};


int find(struct subset subsets[], int i);
void Union(struct subset subsets[], int x, int y);

void boruvkaMST(vector<xEdge> *allChange, A_Network *X,vector<RT_Vertex> *CRT, vector<Edge> *reBs, map_Edge_int *Edgemap,vector<Edge> *deleted_edges, vector<int> *updated_nodes, int highV,double max_val, double maxW, bool *dels, int root, int p, int nodes)
{

    struct Graph {
        int V, E;
        Edge* mye;
    };
    Graph* graph = new Graph;
    //E = graph->E;
   // Edge *edge = graph->edge;
    vector<Edge> allEdges;
    allEdges.clear();
    Edge *mye = graph->mye;
    A_Network *Y1;


//


    graph->V = nodes;
    graph->E = allEdges.size();
//
    int V = graph->V;
    int E = graph->E;


    struct subset *subsets = new subset[V];
    //vector<int> component;
    //component.resize(X->size(),-1);

    // array to store index of the cheapest edge
    int *min_weight = new int[V];

    // create V subsets with single elements
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
        min_weight[v] = -1;
    }

    // V number of different trees.
    // by the end there will be one MST
    int numTrees = V;
    int MSTweight = 0;

    // keep combining until all are not combined into single MST.
    while (numTrees > 1)
    {
        // update cheapest of every component
        for (int i=0; i<X->size(); i++)
        {
            // Find components of two corners
            int f1 = find(subsets, mye->node1); // subsets, edge[i].src
            int f2 = find(subsets, mye->node2); // subsets, edge[i].dest

            if (f1 == f2)
                continue;

                // check if current edge is closer to previous cheapest edges
            else
            {
               // if(f1<f2)
               // { subsets[f2]=f1;}

               // else
               // { subsets[f1]=f2;}

                mye=allEdges[i];

                if (min_weight[f1] == -1 ||
                    mye[min_weight[f1]].edge_wt > mye->edge_wt)
                    min_weight[f1] = mye->edge_wt;

                if (min_weight[f1] == -1 ||
                    mye[min_weight[f2]].edge_wt > mye->edge_wt)
                    min_weight[f2] = mye->edge_wt;
            }
        }

        // add edges to MST
        for (int i=0; i<V; i++)
        {
            if (min_weight[i] != -1)
            {
                int f1 = find(subsets, mye[min_weight[i]].node1);
                int f2 = find(subsets, mye[min_weight[i]].node2);

                if (f1 == f2)
                    continue;
                Y1 += mye[min_weight[i]].edge_wt;
                Union(subsets, f1, f2);
                numTrees--;
            }
        }
    }

    printf("Weight of MST is %d\n", Y1);
    return;
}

int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent =
                find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// union of sets (by rank)
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // attach smaller rank tree under root of high rank tree
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

        // if ranks are same, make one root and increment rank
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}
