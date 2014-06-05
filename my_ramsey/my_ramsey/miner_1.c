
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "fifo.h"	/* for taboo list */


#define MAXSIZE (512)

#define TABOOSIZE (500)
#define BIGCOUNT (9999999)

/***
 *** example of very simple search for R(6,6) counter examples
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

char* getValue(){
    
}


int
main(int argc,char *argv[])
{
//	int *g;
//	int *new_g;
//	int gsize;
//	int count;
//    int totalnumber;
//    int mynumber;
	int i;
	int j;
//	int best_count;
//	int best_i;
//	int best_j;
	void *taboo_list;
    int MINER = 1;

	/*
	 * start with graph of size 8
	 */
//	gsize = 8;
//	g = (int *)malloc(gsize*gsize*sizeof(int));
//	if(g == NULL) {
//		exit(1);
//	}

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
//	memset(g,0,gsize*gsize*sizeof(int));

	/*
	 * while we do not have a publishable result
	 */
    
	while(1)
	{
		/*
		 * find out how we are doing
		 */
        while(access( "../graph.txt", F_OK ) == -1){
            // polling
        }
        
        int *g;
        int *new_g;
        int gsize;
        int count;
        int totalnumber;
        int mynumber;
        int best_count;
        int totalsize;
        int mysize;
        int best_i;
        int best_j;
        
        char str[999];
        char str_count[100], str_size[100], str_totalnumber[100], str_mynumber[100], str_graph[12000];
        
        char* graphfile = "../graph.txt";
        FILE * file;
        file = fopen( graphfile , "r");
        char * pch;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        
        if (file) {
//            while (fscanf(file, "%s", str)!=EOF)
//                printf("%s",str);
//            fscanf(file, "%s%s%s%s%s", str_count, str_size, str_totalnumber, str_mynumber, str_graph);
//            if((read = getline(&line, &len, file)) != -1) {
//                printf("Retrieved line of length %zu :\n", read);
//                printf("%s", line);
//               
//            }
            printf("Received Graph!");
            if(!feof(file)) {
                if (fgets(str_count,100,file)) {
                    pch = strtok (str_count,"=");
                    pch = strtok (NULL, "\n");
                    count = atoi(pch);
                    printf("count: %d\n", count);

                }
            }
            
            if(!feof(file)) {
                if (fgets(str_size,100,file)) {
                    pch = strtok (str_size,"=");
                    pch = strtok (NULL, "\n");
                    gsize = atoi(pch);
                    printf("gsize: %d\n", gsize);
                    
                }
            }
            
            if(!feof(file)) {
                if (fgets(str_totalnumber,100,file)) {
                    pch = strtok (str_totalnumber,"=");
                    pch = strtok (NULL, "\n");
                    totalnumber = atoi(pch);
                    printf("total number: %d\n", totalnumber);
                    
                }
            }
            
            if(!feof(file)) {
                if (fgets(str_mynumber,100,file)) {
                    pch = strtok (str_mynumber,"=");
                    pch = strtok (NULL, "\n");
                    mynumber = atoi(pch);
                    printf("my number: %d\n", mynumber);
                    
                }
            }
            
            if(!feof(file)) {
                if (fgets(str_graph,12000,file)) {
                    pch = strtok (str_graph,"=");
                    pch = strtok (NULL, "=");
//                    count = atoi(pch);
//                    printf("count: %d\n", count);
                    
                }
            }
            
            fclose(file);
            remove(graphfile);
            
            
            
            
            g = (int *)malloc(gsize*gsize*sizeof(int));
            if(g == NULL) {
                exit(1);
            }
            memset(g,0,gsize*gsize*sizeof(int));
            
            for(i = 0; i < gsize*gsize; i++){
                // convert char to int
                g[i] = pch[i]-48;
            }
            
            
            best_count = count;
            totalsize = (gsize-1)*(gsize-1)/2;
            
            int eachsize = totalsize/totalnumber;
            
            int mystart = eachsize*(mynumber-1)+1;
            int myend = (mynumber==totalnumber)? totalsize : eachsize*mynumber;
            
            // to get to my start
            int counter = 1;
            int flag = 0;
            for(i = 0; i < gsize; i++){
                for(j = i+1; j < gsize; j++){
                    if(counter < mystart)
                        counter++;
                    else{
                        flag = 1;
                        break;
                    }
                }
                if(flag==1)
                    break;
            }
            
            // until now the i and j are exactly position we want to start
            int m, n;
            m = i;
            n = j;
            flag = 0;
            for(; m < gsize; m++)
            {
                for(; n < gsize; n++)
                {
                    
                    /*
                     * flip it
                     */
                    g[m*gsize+n] = 1 - g[m*gsize+n];
                    count = CliqueCount(g,gsize);
                    
                    /*
                     * is it better and the i,j,count not taboo?
                     */
                    if((count < best_count) && !FIFOFindEdgeCount(taboo_list,m,n,count)) //!FIFOFindEdge(taboo_list,i,j))
                                        
                    {
                        best_count = count;
                        best_i = m;
                        best_j = n;
                        
                        if(best_count == 0){
                            flag = 1;
                            break;
                        }
                    }
                    
                    /*
                     * flip it back
                     */
                    g[m*gsize+n] = 1 - g[m*gsize+n];
                    counter++;
                    if(counter > myend){
                        flag = 1;
                        break;
                    }
                    
                }
                
                if(flag == 1)
                    break;
                
                n = m+2;
            }
            
            const char* status;
            
            if(best_count == 0){
                printf("Congratulations! \nSize: %d \nGraph:\n", gsize);
                for(i = 0; i < gsize*gsize; i++){
                    printf("%d", g[i]);
                }
                printf("\n");
                taboo_list = FIFOResetEdge(taboo_list);
                status = "complete";
            }else{
                printf("Local minimum. \nSize: %d \nCount: %d\n", gsize, best_count);
                g[best_i*gsize+best_j] = 1 - g[best_i*gsize+best_j];
                
                //count = CliqueCount(g,gsize);
                //FIFOInsertEdge(taboo_list,best_i,best_j);
                FIFOInsertEdgeCount(taboo_list,best_i,best_j,count);
                status = "minimum";

            }
            
            FILE *result_file = fopen("../result.txt", "w");
            if (result_file == NULL)
            {
                printf("Error opening file!\n");
                exit(1);
            }
            
            /* print some text */
            fprintf(result_file, "miner=%d\n", MINER);
            fprintf(result_file, "status=%s\n", status);
            fprintf(result_file, "count=%d\n", best_count);
            fprintf(result_file, "size=%d\n", gsize);
            fprintf(result_file, "graph=");
            
            for(i = 0; i < gsize*gsize; i++){
                fprintf(result_file, "%d", g[i]);
            }
            
            fclose(result_file);
                        
            free(g);
            
        }
        
		/*
		 * if we have a counter example
//		 */
//		if(count == 0)
//		{
//			printf("Eureka!  Counter-example found!\n");
//			PrintGraph(g,gsize);
//			/*
//			 * make a new graph one size bigger
//			 */
//			new_g = (int *)malloc((gsize+1)*(gsize+1)*sizeof(int));
//			if(new_g == NULL)
//				exit(1);
//			/*
//			 * copy the old graph into the new graph leaving the
//			 * last row and last column alone
//			 */
//			CopyGraph(g,gsize,new_g,gsize+1);
//
//			/*
//			 * zero out the last column and last row
//			 */
//			for(i=0; i < (gsize+1); i++)
//			{
//				new_g[i*(gsize+1) + gsize] = 0; // last column
//				new_g[gsize*(gsize+1) + i] = 0; // last row
//			}
//
//			/*
//			 * throw away the old graph and make new one the
//			 * graph
//			 */
//			free(g);
//			g = new_g;
//			gsize = gsize+1;
//
//			/*
//			 * reset the taboo list for the new graph
//			 */
//			taboo_list = FIFOResetEdge(taboo_list);
//
//			/*
//			 * keep going
//			 */
//			continue;
//		}
//
//		/*
//		 * otherwise, we need to consider flipping an edge
//		 *
//		 * let's speculative flip each edge, record the new count,
//		 * and unflip the edge.  We'll then remember the best flip and
//		 * keep it next time around
//		 *
//		 * only need to work with upper triangle of matrix =>
//		 * notice the indices
//		 */
//		best_count = BIGCOUNT;
//		for(i=0; i < gsize; i++)
//		{
//			for(j=i+1; j < gsize; j++)
//			{
//				/*
//				 * flip it
//				 */
//				g[i*gsize+j] = 1 - g[i*gsize+j];
//				count = CliqueCount(g,gsize);
//
//				/*
//				 * is it better and the i,j,count not taboo?
//				 */
//				if((count < best_count) && 
//					!FIFOFindEdge(taboo_list,i,j))
////					!FIFOFindEdgeCount(taboo_list,i,j,count))
//				{
//					best_count = count;
//					best_i = i;
//					best_j = j;
//				}
//
//				/*
//				 * flip it back
//				 */
//				g[i*gsize+j] = 1 - g[i*gsize+j];
//			}
//		}
//
//		if(best_count == BIGCOUNT) {
//			printf("no best edge found, terminating\n");
//			exit(1);
//		}
//		
//		/*
//		 * keep the best flip we saw
//		 */
//		g[best_i*gsize+best_j] = 1 - g[best_i*gsize+best_j];
//
//		/*
//		 * taboo this graph configuration so that we don't visit
//		 * it again
//		 */
//		count = CliqueCount(g,gsize);
//		FIFOInsertEdge(taboo_list,best_i,best_j);
////		FIFOInsertEdgeCount(taboo_list,best_i,best_j,count);
//
//		printf("ce size: %d, best_count: %d, best edge: (%d,%d), new color: %d\n",
//			gsize,
//			best_count,
//			best_i,
//			best_j,
//			g[best_i*gsize+best_j]);
//
//		/*
//		 * rinse and repeat
//		 */
//	}
//
//	FIFODeleteGraph(taboo_list);

    }
	return(0);

}
