#include "simple.h"
#include "../grph.h"
#include "merge.h"
#include <sys/types.h>
#include <values.h>
#include "my_malloc.h"

int Boruvka_sparse_merge(ent_t *grph_list, int n_vertices,THREADED)
{

  hrtime_t start,end,s1,t1;
  double interval1=0,interval2=0,interval3=0,interval;
  double ti_findmin=0,ti_cc=0,ti_compact=0;
  ele_t * tmp_arr;

  int newly_dead=0,*p_alive,*p_total,*D,*dead_matrix,*total_matrix;
  int min,min_ind,k,l,w,i,j,total=0;


  D = node_malloc(sizeof(int)*n_vertices,TH);
  dead_matrix = node_malloc(sizeof(int)*THREADS,TH);
  total_matrix = node_malloc(sizeof(int)*THREADS,TH);
  p_alive = node_malloc(sizeof(int),TH);
  p_total = node_malloc(sizeof(int),TH);

  tmp_arr = malloc(sizeof(ele_t)*(n_vertices));
  on_one_thread {
	*p_alive = n_vertices;
	*p_total = 0;

  }

  #pragma omp barrier();

  while(1) {

  	start = gethrtime();
	on_one_thread printf(" Alive vertices:%d, total weights:%d\n",n_vertices,*p_total);
	newly_dead=0;
	total=0;

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size){
	 D[i]=i;
    }
	#pragma omp barrier();

	s1 = gethrtime();
	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
	{
		min=MAXINT;
		for(k=0;k<grph_list[i].n_neighbors;k++)
          	{
            		j=grph_list[i].my_neighbors[k].v;
            		if(i!=j && min>grph_list[i].my_neighbors[k].w ){
                 		min = grph_list[i].my_neighbors[k].w;
                 		min_ind=j;
					}
            }
		D[i]=min_ind;
    }

	#pragma omp barrier();
	t1 = gethrtime();
	interval = t1 - s1;
	interval = interval ;
	ti_findmin += interval;

	s1 = gethrtime();
	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
		if(i==D[D[i]] && i>D[i]) D[i]=i;
	#pragma omp barrier();

	on_one printf("before pj\n");

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
		while(D[i]!=D[D[i]]){
			D[D[i]]=D[D[D[i]]];
		 	D[i]=D[D[i]];
		}
	#pragma omp barrier();
	on_one printf("after pj\n");

	#pragma omp parallel for num_threads(p) schedule(dynamic, batch_size)
	{
		grph_list[i].wk_space1=D[i];
		for(j=0;j<grph_list[i].n_neighbors;j++)
			grph_list[i].my_neighbors[j].v=D[grph_list[i].my_neighbors[j].v];
	}

	#pragma omp barrier();
	t1 = gethrtime();
	interval = t1-s1;
	interval = interval ;
	ti_cc += interval;
	s1 = gethrtime();

	grph_list=compact_adj_list_arr(grph_list,&n_vertices,tmp_arr,TH);
	if(n_vertices<=1) break;

	t1 = gethrtime();
	interval=t1-s1;
	interval=interval;
	ti_compact+=interval;
	on_one printf("Time used for compacting list is %f s\n",interval);
	on_one_thread printf("**** compact_adj_list_arr: n_vertices is now %d\n",n_vertices);

	#pragma omp barrier();
	end = gethrtime();
	interval1=end-start;
	on_one_thread printf("=== Time for this round: %f s\n\n", interval1);

  } /*while*/

  on_one printf("The total weight is %d\n",*p_total);
  on_one printf("METRICS: Time used for find_min %f s, cc %f s, compact %f s\n",ti_findmin, ti_cc, ti_compact);
  #pragma omp barrier();
  //free(tmp_arr);
  node_free(p_alive,TH);
  node_free(p_total,TH);
  node_free(dead_matrix,TH);
  node_free(total_matrix,TH);
  node_free(D,TH);
}
