/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "times.h"
#include "sorting.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "permutations.h"

/***************************************************/
/* Function: average_sorting_time Date:            */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short average_sorting_time(pfunc_sort metodo, 
                              int n_perms,
                              int N, 
                              PTIME_AA ptime)
{
int* unsortedArray;
  int ret, i, totalOBs = 0;
  clock_t timeBefore, timeAfter;
  ptime->N = N;
  ptime->time = 0;
  if (n_perms < 0) {
    printf(" Number of perms is less or equal than 0");
    return ERR;
  }
  for(i = 0; i < n_perms; i++) {
    /* Generate a permutation */
    unsortedArray = generate_perm(N);
    if(!unsortedArray) return ERR;
    
    /* Check the time before the method is run */
    timeBefore = clock();
    if (timeBefore == -1) {
      return ERR;
    }
    ret = metodo(unsortedArray, 0, N-1);
    /* Fetch the time after the method is run */
    timeAfter = clock();

    /* Safely add the time without interfering with the execution time of the sorting algorithm */
    ptime->time += ((double) (timeAfter - timeBefore) / (double) CLOCKS_PER_SEC);
    free(unsortedArray);
    
    /* Check if there was an error */
    if(ret == ERR) return ERR;
    totalOBs += ret;
    
    /* Check if the returned count of OB's is a maximum, a minimum or both */
    if(i == 0){
      ptime->max_ob = ret;
      ptime->min_ob = ret;
    }
    else {
      if(ret > ptime->max_ob) ptime->max_ob = ret;
      if(ret < ptime->min_ob) ptime->min_ob = ret;
    }
  }

  ptime->average_ob = (double) totalOBs / (double) n_perms;
  return OK;
}

/***************************************************/
/* Function: generate_sorting_times Date:          */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short generate_sorting_times(pfunc_sort method, char* file, 
                                int num_min, int num_max, 
                                int incr, int n_perms)
{
  int N, n_time = 0, n_times = (num_max - num_min + 1) / incr;
  PTIME_AA timeArray;
  timeArray = (PTIME_AA) malloc(sizeof(TIME_AA) * num_max);

  for (N = num_min; N <= num_max; N += incr, n_time++) {
    if(average_sorting_time(method, n_perms, N, &timeArray[n_time]) == ERR) {
      free(timeArray);
      return ERR;
    } 
  }

  if(save_time_table(file, timeArray, n_times) == ERR) {
    free(timeArray);
    return ERR;
  }
  free(timeArray);
  return OK;
}

/***************************************************/
/* Function: save_time_table Date:                 */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short save_time_table(char* file, PTIME_AA ptime, int n_times)
{
  FILE *pf;
  int i;

  pf = fopen (file, "w");
    if (pf == NULL) return ERR;

  fprintf(pf, "Size\t Time\t Avg.\t Max.\t Min.\t ");

  for(i = 0; i < n_times; i++) {
    fprintf(pf, "\n%d\t%e\t%e\t%d\t%d\t\n",
    ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
  }

  fclose(pf);
  return OK;
}


