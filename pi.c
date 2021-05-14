#include<stdio.h>
#include<cstdlib>
#include<sys/time.h>
#include<math.h>
#include<ctime>
#include"mpi.h"
#define MAX 1000000

struct timeval tv;
float gettime()
{
        static int startflag=1;
        static double tsecs0, tsecs1;
        if( startflag ) {
                (void ) gettimeofday(&tv,0);
                tsecs0 = tv.tv_sec + tv.tv_usec*1.0e-6;
                startflag = 0;
        }
        (void) gettimeofday(&tv,0);
        tsecs1 = tv.tv_sec + tv.tv_usec*1.0e-6;
        return ((float) (tsecs1-tsecs0));
}

int main(int argc, char *argv[]){
   int rank, size;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   double time_start,time_end;
   double integ=0., integ_rev=0;

   time_start = gettime();

   for (int n=0; n<MAX; n++){
      if (n%4==0 && rank==0) integ += (double)((double)4/(8*n+1)-(double)2/(8*n+4)-(double)1/(8*n+5)-(double)1/(8*n+6))/pow(16,n);
      if (n%4==1 && rank==1) integ += (double)((double)4/(8*n+1)-(double)2/(8*n+4)-(double)1/(8*n+5)-(double)1/(8*n+6))/pow(16,n);
      if (n%4==2 && rank==2) integ += (double)((double)4/(8*n+1)-(double)2/(8*n+4)-(double)1/(8*n+5)-(double)1/(8*n+6))/pow(16,n);
      if (n%4==3 && rank==3) integ += (double)((double)4/(8*n+1)-(double)2/(8*n+4)-(double)1/(8*n+5)-(double)1/(8*n+6))/pow(16,n);
   }
   MPI_Reduce(&integ,&integ_rev,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

   time_end = gettime();
   if (rank == 0){
      printf("%.30f\n",integ_rev);
      printf("Wallclock time %g\n",time_end-time_start);
   }
   MPI_Finalize();
}