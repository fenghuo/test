#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "fifo.h"	/* for taboo list */


#define MAXSIZE (512)

#define TABOOSIZE (50)
#define BIGCOUNT (9999999)

/***
 *** example of very simple search for R(5,5) counter examples
 ***
 *** starts with a small randomized graph and works its way up to successively
 *** larger graphs one at a time
 ***
 *** uses a taboo list of size #TABOOSIZE# to hold and encoding of and edge
 *** (i,j)+clique_count
 ***/

/*
 * PrintGraph
 *
 * prints in the right format for the read routine
 */
void PrintGraph(int *g, int gsize)
{
	int i;
	int j;
    
	fprintf(stdout,"%d\n",gsize);
    
	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			fprintf(stdout,"%d ",g[i*gsize+j]);
		}
		fprintf(stdout,"\n");
	}
    
	return;
}

/*
 * print out the graph to the file.
 */
void
OutputGraph(int *g, int gsize, int seed)
{
	int i,j;
	int g_length = gsize*gsize;
	char fn_output[50];
	FILE *fp;
    
	sprintf(fn_output, "Ramsey_%d_%d.txt", gsize, seed);
	fp = fopen(fn_output, "w");
    
	for(i=0; i < gsize; i++)
	{
		for(j=0; j < gsize; j++)
		{
			fprintf(fp,"%d",g[i*gsize+j]);
		}
		//fprintf(stdout,"\n");
	}
	fprintf(fp, "\n");
	fclose(fp);
    
}


/*
 * CopyGraph
 *
 * copys the contents of old_g to corresponding locations in new_g
 * leaving other locations in new_g alone
 * that is
 * 	new_g[i,j] = old_g[i,j]
 */
void CopyGraph(int *old_g, int o_gsize, int *new_g, int n_gsize)
{
	int i;
	int j;
    
	/*
	 * new g must be bigger
	 */
	if(n_gsize < o_gsize)
    return;
    
	for(i=0; i < o_gsize; i++)
	{
		for(j=0; j < o_gsize; j++)
		{
			new_g[i*n_gsize+j] = old_g[i*o_gsize+j];
		}
	}
    
	return;
}


/*
 ***
 *** returns the number of monochromatic cliques in the graph presented to
 *** it
 ***
 *** graph is stored in row-major order
 *** only checks values above diagonal
 */

int CliqueCount(int *g,
                int gsize)
{
    int i;
    int j;
    int k;
    int l;
    int m;
    int n;
    int count=0;
    int sgsize = 6;
    
    for(i=0;i < gsize-sgsize+1; i++)
    {
        for(j=i+1;j < gsize-sgsize+2; j++)
        {
            for(k=j+1;k < gsize-sgsize+3; k++)
            {
                if((g[i*gsize+j] == g[i*gsize+k]) &&
                   (g[i*gsize+j] == g[j*gsize+k]))
                {
                    for(l=k+1;l < gsize-sgsize+4; l++)
                    {
                        if((g[i*gsize+j] == g[i*gsize+l]) &&
                           (g[i*gsize+j] == g[j*gsize+l]) &&
                           (g[i*gsize+j] == g[k*gsize+l]))
                        {
                            for(m=l+1;m < gsize-sgsize+5; m++)
                            {
                                if((g[i*gsize+j] == g[i*gsize+m]) &&
                                   (g[i*gsize+j] == g[j*gsize+m]) &&
                                   (g[i*gsize+j] == g[k*gsize+m]) &&
                                   (g[i*gsize+j] == g[l*gsize+m])) {
                                    for(n=m+1; n < gsize-sgsize+6; n++)
                                    {
                                        if((g[i*gsize+j]
                                            == g[i*gsize+n]) &&
                                           (g[i*gsize+j]
                                            == g[j*gsize+n]) &&
                                           (g[i*gsize+j]
                                            == g[k*gsize+n]) &&
                                           (g[i*gsize+j]
                                            == g[l*gsize+n]) &&
                                           (g[i*gsize+j]
                                            == g[m*gsize+n])) {
                                               count++;
//                                               if(count > 500)
//                                                   return count;
                                           }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return(count);
}

int isPrime(int target)
{
    int i;
    for (i = 2; i <= pow(target, 0.5); i++)
    {
        if ((target%i) == 0)
        {
            return 0;
        }
    }
    return 1;
}


void generateSymGraph(int* g, int gsize){
    int largestDis = gsize/2;
    int i, j, d;
    for( d = 1; d <= largestDis; d++){
        int one = rand()%100>50?1:0;
        if(one == 1){
            // if we see one, assign all edges with distance of d to be 1
            for(i = 0; i < gsize; i++){
                j = (i+d)%gsize;
                if(i < j)
                    g[i*gsize+j] = 1;
                else
                    g[j*gsize+i] = 1;
            }
        }
        
    }
}

void enlargeSymGraph(int* g, int gsize, int *new_g, int newsize){
    int largestDis = gsize/2;
    int i, j, d;
    int counter = 0;
    for( d = 1; d <= largestDis; d++){
        int one = g[d];

            // copy the same distance
            for(i = 0; i < newsize; i++){
                j = (i+d)%newsize;
                if(i < j)
                    new_g[i*newsize+j] = one;
                else
                    new_g[j*newsize+i] = one;
            }
        
    }
    for(d = largestDis+1; d <= newsize/2; d++){
        int one = rand()%100>50?1:0;
            // assign all edges with distance of d to be either 1 or 0
            for(i = 0; i < newsize; i++){
                j = (i+d)%newsize;
                if(i < j)
                    new_g[i*newsize+j] = one;
                else
                    new_g[j*newsize+i] = one;
            }
        
    }
}

void flipOneDis(int* g, int gsize, int d){
    //int m = 0;
    //int n = rand()%(gsize/2);
    //int d = n-m;
    int i, j;
    
    for(i = 0; i < gsize; i++){
        j = (i+d)%gsize;
        if(i < j)
            g[i*gsize+j] = 1 - g[i*gsize+j];
        else
            g[j*gsize+i] = 1 - g[j*gsize+i];
    }
    
}

int
main(int argc,char *argv[])
{
	int *g;
	int *new_g;
	int gsize;
    int oldsize;
	int count;
    int best_count;
    int best_count_pre;
    int best_d;
	int i;
	int j;
	void *taboo_list;
    
    int if_break;
    int seed = 2;
    
    srand(seed);
	/*
	 * start with graph of size 8
	 */
	gsize = 53;
    best_count = BIGCOUNT;
    best_count_pre = best_count;
    
    printf("gsize: %d\n", gsize);
	g = (int *)malloc(gsize*gsize*sizeof(int));
	if(g == NULL) {
		exit(1);
	}
    
	/*
	 * make a fifo to use as the taboo list
	 */
	taboo_list = FIFOInitEdge(TABOOSIZE);
	if(taboo_list == NULL) {
		exit(1);
	}
    
	/*
	 * start out with all zeros
	 */
	memset(g,0,gsize*gsize*sizeof(int));
    
    /*
     * start to construct symmetric graph
     */
    generateSymGraph(g, gsize);
    
    
    
    
    //jump(g, gsize);
//    char ch;
//    FILE *fp;
//    if((fp=fopen("Ramsey_57_13.txt","r"))==NULL) {
//        printf("not open");
//        exit(0);
//    }
//    for(i = 0; i < gsize*gsize; i++){
//        if((ch=fgetc(fp))!=EOF){
//            g[i] = ch-48;
//            printf("%d", g[i]);
//            //ch=fgetc(fp);
//        }
//    }
//    fclose(fp);
    
    //min_count = 0;
    
	/*
	 * while we do not have a publishable result
	 */
    
    while(gsize < 102)
	{
		/*
		 * find out how we are doing
		 */
		count = CliqueCount(g,gsize);
        
		/*
		 * if we have a counter example
		 */
		if(count == 0)
		{
			printf("Eureka!  Counter-example found!\n");
			PrintGraph(g,gsize);
            OutputGraph(g, gsize, seed);
            
            int newsize = gsize+1;
            while (isPrime(newsize)==0) {
                newsize++;
            }
			new_g = (int *)malloc((newsize)*(newsize)*sizeof(int));
			if(new_g == NULL)
            exit(1);
            memset(new_g,0,newsize*newsize*sizeof(int));
			/*
			 * copy the old graph into the new graph leaving the
			 * last row and last column alone
			 */
//			CopyGraph(g,gsize,new_g,gsize+1);
            enlargeSymGraph(g, gsize, new_g, newsize);

			/*
			 * throw away the old graph and make new one the
			 * graph
			 */
           
			free(g);
			g = new_g;
            oldsize = gsize;
			gsize = newsize;
            printf("Start with new graph, size: %d.\n", newsize);
            //PrintGraph(g, gsize);
            /*
             * reset the taboo list for the new graph
             */
			taboo_list = FIFOResetEdge(taboo_list);
            /*
             * keep going
             */
            
            best_count = BIGCOUNT;
            best_count_pre = best_count;
            continue;
		}
        
		/*
		 * otherwise, we need to consider flipping an edge
		 *
		 * let's speculative flip each edge, record the new count,
		 * and unflip the edge.  We'll then remember the best flip and
		 * keep it next time around
		 *
		 * only need to work with upper triangle of matrix =>
		 * notice the indices
		 */
//		if_break = 0;
        //best_count = BIGCOUNT;
		for(i=1; i <= gsize/2; i++)
		{
            flipOneDis(g, gsize, i);
            count = CliqueCount(g,gsize);
            if((count < best_count)&&!FIFOFindEdgeCountSym(taboo_list,i,count)){
                best_count = count;
                best_d = i;
            }
            flipOneDis(g, gsize, i);
        }

        
        if(best_count == best_count_pre){
            // it means that we have explored every possibility of flipping one bit
            // but no optimization found
            // we would like to either swap or jump this time
            printf("Best count: %d. Have to jump here!\n", best_count);
            int i = 0;
            while(i<10){
                int d = rand()%(gsize/2+1);
                //printf("jump distance: %d\n", d);
                flipOneDis(g, gsize, d);
                i++;
            }
            best_count = BIGCOUNT;
            best_count_pre = best_count;
            continue;
            
        }
		
		/*
		 * keep the best flip we saw
		 */
        
        printf("gsize: %d, best count here: %d\n", gsize, best_count);
		//g[best_i*gsize+best_j] = 1 - g[best_i*gsize+best_j];
        flipOneDis(g, gsize, best_d);
        
		/*
		 * taboo g graph configuration so that we don't visit
		 * it again
		 */
		//count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
        FIFOInsertEdgeCountSym(taboo_list,best_d,best_count);
        best_count_pre = best_count;
		/*
		 * rinse and repeat
		 */
	}
    
	FIFODeleteGraph(taboo_list);
    
    
	return(0);
    
}
