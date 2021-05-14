#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include<sys/time.h>
#include<mpi.h>

typedef struct Pos{
	float x,y,z;
}Pos;

float ran2(long *);
long iseed=-9;

double potential(Pos *r, int np){
	int i,j;
	double potent = 0;
	for(j=0;j<np;j++){
		for(i=j+1;i<np;i++){
			float x= r[i].x-r[j].x;
			float y= r[i].y-r[j].y;
			float z= r[i].z-r[j].z;
			float dist = sqrtf(x*x+y*y+z*z);
			if(dist > 0.) potent += 1./dist;
		}
	}
	return potent;
}

int getrandomnp(int istep, int niter){
	if(istep < niter/10) {
		return (int)(3000+5000*(ran2(&iseed)));
	}
	else {
		return (int)(100*(ran2(&iseed)));
	}
}

struct timeval tv;
float gettime(){
	static int startflag = 1;
	static double tsecs0, tsecs1;
	if(startflag) {
		(void ) gettimeofday(&tv, NULL);
		tsecs0 = tv.tv_sec + tv.tv_usec*1.0E-6;
		startflag = 0;
	}
	(void) gettimeofday(&tv, NULL);
	tsecs1 = tv.tv_sec + tv.tv_usec*1.0e-6;
	return (float) (tsecs1 - tsecs0);
}



int main(int argc, char **argv){
	int rank, size;
	int i, j;
	int np,niter,maxnp=5000000;
	Pos *r;
	double totpotent=0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	niter = 50;
	r = (Pos*)malloc(sizeof(Pos)*maxnp);
	ran2(&iseed);

	float time1, time2;
	time1 = gettime();
	for(i=0;i<niter;i++){
		np =  getrandomnp(i,niter);
		for(j=0;j<np;j++){
			r[j].x = 2.*(ran2(&iseed))-1.;
			r[j].y = 2.*(ran2(&iseed))-1.;
			r[j].z = 2.*(ran2(&iseed))-1.;
		}
		totpotent += potential(r, np);
	}
	time2 = gettime();
	MPI_Finalize();
	printf("Total potential is %20.10g in wallclock time = %g second\n",totpotent, (time2-time1));
}
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran2(long *idum)
{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if (*idum < 0) *idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX
/* (C) Copr. 1986-92 Numerical Recipes Software 71.+I0>+. */
