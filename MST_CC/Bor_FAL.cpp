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

#include "simple.h"
#include "../grph.h"
#include "append.h"
#include <sys/types.h>
#include <values.h>
#include "my_malloc.h"

//nt Boruvka_sparse_nocompact(ent1_t *grph_list, int n_vertices,THREADED)
void boruvka(A_Network *Y, A_Network *Y1, int p, int batch_size)
{

  clock_t start,end,s1,t1;
  double interval,findmin_t=0,append_t=0,housekeep_t=0;

  int newly_dead=0,*p_alive,*p_total,*D,*Label,*Min;
  int min,min_ind,k,l,w,i,j,total=0;
  int n_all_vertices;
  adj_t * pTmp;

  n_vertices = Y->size() //added
  n_all_vertices = n_vertices;
  D = node_malloc(sizeof(int)*n_vertices,TH);
  Label = node_malloc(sizeof(int)*n_vertices,TH);

  Min = node_malloc(sizeof(int)*n_vertices,TH);

  #pragma omp parallel for num_threads(p) schedule(dynamic, batch_size) { //n_all_vertices
    Label[i]=i;
  }
  #pragma omp barrier();

  while(1) {

  	start = omp_get_wtime();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){
	 D[i]=i;
	}
	#pragma omp barrier();

	s1 = omp_get_wtime();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
	{
		min=MAXINT;
		min_ind=-1;
		pTmp = grph_list[i].head;

    // = while (numTrees > 1)
    while(pTmp!=NULL){
			for(k=0;k<pTmp->n_neighbors;k++)
          	{
            		j=pTmp->my_neighbors[k].v;
            		if(min>pTmp->my_neighbors[k].w && i!=Label[j]){
                 		min = pTmp->my_neighbors[k].w;
                 		min_ind=j;
					}
            }
      // if (set1 == set2)
			pTmp=pTmp->next;
		}
    // else
		if(min_ind!=-1) D[i]=Label[min_ind];
		if(min_ind!=-1) Min[i]=min;
    }

	#pragma omp barrier();
	t1 = omp_get_wtime();
	interval = t1 - s1;
	//interval = interval /NANO;
	on_one printf(" Time used for find-min is %f s\n",interval);

	findmin_t += interval;

	s1 = omp_get_wtime();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size) {
          if(D[D[i]]!=i|| (D[D[i]]==i && i<D[i]) ){
                 total+=Min[i];
          }
  }
	#pragma omp barrier();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){
		if(i==D[D[i]] && i<D[i]) D[i]=i;
  }
	#pragma omp barrier();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){
		while(D[i]!=D[D[i]]){
		 D[D[i]]=D[D[D[i]]];
		 D[i]=D[D[i]];
   }
 }
	#pragma omp barrier();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){ //n_all_vertices
		Label[i]=D[Label[i]];
  }

	#pragma omp barrier();

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){
		grph_list[i].wk_space1=D[i];
  }
	#pragma omp barrier();

	t1 = omp_get_wtime();
	interval = t1 - s1;
	//interval = interval /NANO;
	on_one printf("Time used for housekeeping is %f s\n",interval);
	housekeep_t+=interval;

	s1 = omp_get_wtime();
	grph_list=append_adj_list(grph_list,&n_vertices,Label,n_all_vertices,TH);
	if(n_vertices<=1) break;

	t1 = omp_get_wtime();
	interval=t1-s1;
	//interval=interval/NANO;
	on_one printf("Time used for appending  is %f s\n",interval);
	append_t += interval;
	#pragma omp barrier();
	end = omp_get_wtime();
	interval=end-start;
	on_one_thread printf("Time for this round: %f s\n\n", interval); // /NANO

  } /*while*/

  total=node_Reduce_i(total,SUM,TH);
  on_one printf("total weight is %d \n",total);
  on_one printf("METRICS: time used for find_min, housekeeping, append is %f, %f %f \n",findmin_t,housekeep_t,append_t);
  on_one printf("==================================================\n\n");

  #pragma omp barrier();
  node_free(D,TH);
  return(total);
}




