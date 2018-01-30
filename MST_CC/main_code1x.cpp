// HEADERS
#include "SP_structure.hpp"
#include "create_tree.hpp"
#include "readin_data.hpp"
#include "btest.cpp"

//AUXILLIARY OPS
#include  "modifyWT.hpp"
#include<queue>
#include<stack>
#include<list>

/*** All Headers Required From ESSENS **/

using namespace std;

int main(int argc, char *argv[])
{
	clock_t q, q1, q2,t;
    double startx, endx, startx1, endx1;
#ifdef POWER_PROFILING_MAIN
    power_rapl_t ps;
    power_rapl_init(&ps);
#endif

	/***** Preprocessing to Graph (GUI) ***********/
	t=clock();
	q=clock();    //Assume Processed Input
    //Form node node weight
    //Nodes are numbered contiguously
    //Starts from zero
    //Edges are undirected (both a-->b and b-->a present)
	//Check if valid input is given
	if ( argc < 6 ) { cout << "INPUT ERROR:: SEVEN inputs required. First: filename without tree. Second: Graph Certificate. Third: List of Edges to be Updated. Fourth: Upper Bound on Weight Fifth: number of nodes. Sixth: Size of batch. Seventh: number of threads\n"; return 0;}
	//Check to see if file opening succeeded
	ifstream the_file ( argv[1] ); if (!the_file.is_open() ) { cout<<"INPUT ERROR:: Could not open main file\n";}
    ifstream the_file1 ( argv[2] ); if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open certificate file\n";}
    ifstream the_file2 ( argv[3] ); if (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open update file\n";}


/* Set Max weight and the number of Threads and Vertices**/
int maxW=atoi(argv[4]);   //maximum wieght of an edge
int nodes=atoi(argv[5]); //total number of vertices
int p=atoi(argv[7]);  //total number of threads per core
int batch=atoi(argv[6]); //the number of changes processed per point
/** Set the number of Threads and Vertices**/

   /***
    Initializing Memory ***/ /** TBD: Change to Array and Malloc**/
    startx=omp_get_wtime();

    //Initializing  Rooted Tree
    RT_Vertex RTV;
    vector<RT_Vertex> CRT;
    CRT.clear();
    CRT.resize(nodes,RTV);

    //List of Changes
    vector<xEdge> allChange;
    allChange.clear();


    endx=omp_get_wtime();
    printf("Time Taken for Initializing Memory %f \n", endx-startx);
    /*** Initializing Memory ***/

    /**** Read Remainder Edges ***/
    startx=omp_get_wtime();
       //List of Remainder Edges
    vector<Edge> reBs;
    int highV=10; //highest weight to consider
    reBs.clear();
    printf("Highest Edge Value %d --\n", highV);
    readin_remainder(argv[1], highV, &reBs);
    endx=omp_get_wtime();
    printf("Total Time for Reading Remainder Edges %f \n", endx-startx);
    /*** Finished Reading Remainder Edges **/

    /*** Read the Certificate ***/
    startx=omp_get_wtime();
    A_Network X;
    readin_network(&X, argv[2],-1);
    endx=omp_get_wtime();
    printf("Total Time for Reading Certificate Network %f \n", endx-startx);
    /*** Finished Reading CRT Tree **/

    /*** Read set of Changed Edges ***/
    startx=omp_get_wtime();
    readin_changes(argv[3], &allChange);
    endx=omp_get_wtime();
    printf("Total Time for Reading Update List %f \n", endx-startx);
    /*** Finished Reading Changed Edges **/



    printf("==Update Starts =======\n");

#ifdef POWER_PROFILING_MAIN
    power_rapl_start(&ps);
#endif


    /*** Find Degree of Each Vertex ***/
    startx=omp_get_wtime();

    //Get degrees
/*#pragma omp parallel for num_threads(p)
    for(int x=0;x<nodes;x++)
    {CRT[x].degree=X[x].ListW.size();}
    endx=omp_get_wtime();
    printf("Total Time for Obtaining Degrees of Vertices  %f \n", endx-startx);

    /*************************/

    /*** Create the Rooted Tree **/
     startx=omp_get_wtime();

     double maxwt=-1.0; //maximum edge weight in tree
     int maxV=0; //root of the tree

    //Initial Creation of rooted tree
    //options:
    //From given root:0
    //Find node with highest degree and set as root:1;
    //Find root that gives optimal hieght:2
    //All options use a form of BFS. Option 2 needs to do BFS twice--therefore twice the time
    int tree_type=0; //was 1
    //print_network(X);
     create_tree(&X, &CRT, &maxwt, &maxV, tree_type, p);

    //print_Parent(&CRT);
    endx=omp_get_wtime();
    /*************************/
    printf("Total Time for Initial Tree Creation  %f \n", endx-startx);

    /*** Update the Edges by bathces***/


    //Using Batch updates
        vector<xEdge> bChange; //contains the Change edges for this batch

    //Nodes whose CRT entires have been updated

    int st, end;
    int iter=0;
    bool dels=false;

    vector<int> updated_nodes;
    vector<Edge> deleted_edges;
      map_Edge_int Edgemap;

    A_Network Y1; // added this line

    startx1=omp_get_wtime();
    double checkx1;
    for(int i=0; i<allChange.size(); i=i+batch)
    {
        //check to see if updating is taking too long
        // if so, switch to recomputation
        double checkx2 = omp_get_wtime();
        //printf("checkx2 = %f", checkx2);
        checkx1 += checkx2-startx1;
        double timeallowed = .00000000000001; //setting to small value to initiate recomputation
        if(checkx1 >= timeallowed)
        {
            printf("Total time for Updating (%f) has exceeded maximum alotted time (%f), switching to recomputation\n", checkx1, timeallowed);
            printf("==Recomputation Starts =======\n");
            double checkx3 = omp_get_wtime();
            boruvka(&X, &Y1, p, batch);
            // boruvka(&bChange, &X, &CRT, &reBs, &Edgemap, &deleted_edges, &updated_nodes, highV,maxwt, maxW, &dels, maxV, p, nodes);
            double endz=omp_get_wtime();
            printf("Total Time for Recomputing was %f \n", endz-checkx3);
            break;
        }

        iter++;

        if (i + batch >= allChange.size()){
            batch = allChange.size() - i;
        }

     //Updating Certificate
     st=i;
     end=st+batch-1;
     dels=false;
     bChange.assign(allChange.begin()+st, allChange.begin()+end);

     //reset deleted edges
     deleted_edges.clear();
     //reset updated nodes
     updated_nodes.clear();

     startx=omp_get_wtime();
     WT_updates (&bChange, &X, &CRT, &reBs, &Edgemap, &deleted_edges, &updated_nodes, highV,maxwt, maxW, &dels, maxV, p);
     endx=omp_get_wtime();
     printf("Total Time for Updating at Batch %d is %f \n", iter, endx-startx);

     //  print_network(X);

     startx=omp_get_wtime();
     bool rewire=0;
     if(!rewire)
     {
         CRT.clear();
         CRT.resize(nodes,RTV);
     create_tree(&X, &CRT, &maxwt, &maxV, 1, p);
     }

     else
     {

     rewire_Parent(&deleted_edges,&updated_nodes,&CRT,&X,p);
     }
     endx=omp_get_wtime();
     printf("Total Time for Updating Rooted Tree at iter %d is %f with Rewire:%d\n", iter, endx-startx,rewire);

       // print_Parent(&CRT);

        //Get Maximum Weighted Edge of Tree
        //Set marked to -1
        maxwt=-1.0;
        int delW=maxW+1;
#pragma omp parallel for num_threads(p)
        for(int x=0;x<nodes;x++)
        {
            int maxEw=CRT[x].EDGwt;
            if((maxwt<maxEw)&& (maxEw<delW))
            {maxwt=maxEw;}
            CRT[x].marked=-1;

        }//end of pragma for

    }//end of batch


#ifdef POWER_PROFILING_MAIN
    power_rapl_end(&ps);
    power_rapl_print(&ps);
#endif
    endx1=omp_get_wtime();
    printf("Total Time for Updating  %f \n", endx1-startx1);
    /*** Update the Edges **/

return 0;
}//end of main

	//==========================================//
