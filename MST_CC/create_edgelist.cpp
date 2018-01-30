
// HEADERS
#include "SP_structure.hpp"


using namespace std;

/*** All Headers Required From ESSENS **/


int main(int argc, char *argv[])
{       
	
	
	/***** Preprocessing to Graph (GUI) ***********/

    //Assume Processed Input
    //Form node node weight
    //Nodes are numbered contiguously
    //Starts from zero
    //Edges are undirected (both a-->b and b-->a present)
	//Check if valid input is given
	if ( argc < 2 ) { cout << "INPUT ERROR:: Three inputs required. First: filename. Second: number of changed edges. Third: Maximum Weight of new edges. Fourth: Percentage of Inserted Edges (in values 0 to 100) \n" ; return 0;}
	//Check to see if file opening succeeded
	ifstream the_file ( argv[1] ); if (!the_file.is_open() ) { cout<<"INPUT ERROR:: Could not open main file\n";}
/*** Create DataStructure Sparsifictaion Tree **/
    
    
    /******* Read Graph to EdgeList****************/
    vector<Edge> allEdges;
    allEdges.clear();
    int nodes;
    readin_networkE(argv[1],&allEdges, &nodes);
  //  printf("done reading graph\n");
    /******* Read Graph to EdgeList****************/
    
    /**** Create Set of Edges to Modify ****/
    srand (time(NULL));
    
    int numE=atoi(argv[2]);
    int max_wt= atoi(argv[3]);
    int ins_per=atoi(argv[4]);
    
    
    double numF=(double)numE*((double)ins_per/(double)100);
    int numI=(int)numF;
    int numD=numE-numI;
    
    int iI=0;//number of inserts
    int iD=0;//number of deletes
    int k;
    
    while(1)
    {
        k=rand()%2;
        
    //Edges to Insert
    if(k==1 && iI<numI)
    {
    
        int nx=rand()%nodes;
        int ny=rand()%nodes;
        if(nx==ny){continue;}
        
        int n1, n2;
        if(nx<ny) {n1=nx; n2=ny;}
        else
        {n1=ny; n2=nx;}
        
        double mywt=rand()%max_wt;
        printf("%d  %d %f 1 \n", n1, n2, mywt);
        iI++;
   
        
    }//end of if
    
        
        // Edge to Delete
        if(k==0 && iD<numD)
        {   
            int nz=rand()%(allEdges.size());
            Edge mye=allEdges[nz];
            printf("%d  %d  %f 0 \n", mye.node1, mye.node2, mye.edge_wt);
            iD++;
            continue;
        } //end of if
        
        if(iI==numI && iD==numD){break;}
    }//end of while
    
return 0;	
}//end of main
	
	//==========================================//
	
	

