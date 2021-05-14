/*
   (C) 2001 by Argonne National Laboratory.
       See COPYRIGHT in top-level directory.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpe.h"
#include "mpe_graphics.h"

int makelimit(int sum);
int main( int argc, char** argv )
{
    MPE_XGraph graph;
    int ierr, mp_size, my_rank, x_size, y_size;
    MPE_Color my_color;
    char ckey;
    
    char displayname[30] = "";
    

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &mp_size );
    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );

    x_size=1280;
    y_size=1024;
    
    if ( my_rank == 0 ) {
        strcpy( displayname, getenv( "DISPLAY" ) );
        //strcpy( displayname, getenv( "localhost:11.0" ) );
    }

    MPI_Bcast( displayname, 30, MPI_CHAR, 0, MPI_COMM_WORLD );
    fprintf( stdout, "%d : $DISPLAY at process 0 = %s\n", my_rank, displayname );
    fflush( stdout );

    ierr = MPE_Open_graphics( &graph, MPI_COMM_WORLD, displayname,
                              -1, -1, x_size, y_size, 0 );
    

    if ( ierr != MPE_SUCCESS ) {
        //fprintf( stderr, "%d : MPE_Open_graphics() fails\n", my_rank );
        ierr = MPI_Abort( MPI_COMM_WORLD, 1 );
        exit(1);
    }
    my_color = (MPE_Color) (my_rank + 1);

	int i, j;
    if ( my_rank == 0 ){
	for( i = 0; i<300; i++){
		for( j = 0; j<= i/3; j++){

			MPE_Add_RGB_color( graph, makelimit(1024*i+16*j+(4096-1024*i-16*j)*0.8), makelimit(1024*i+16*j+(8960-1024*i-16*j)*0.8), makelimit(1024*i+16*j+(34304-1024*i-16*j)*0.8), &my_color);
	        	ierr = MPE_Draw_point( graph, i, j, (MPE_Color)my_color);
	        	ierr = MPE_Draw_point( graph, 300-i, 100-j, (MPE_Color)my_color);
		}
	}
    }
    else if ( my_rank > 0 && my_rank < 3){
	for( i = my_rank%2; i<300; i+=2){
		for( j = 0; j<=i/3; j++){
			
			MPE_Add_RGB_color( graph, makelimit(1024*i+16*j+(4096-1024*i-16*j)*0.8), makelimit(1024*i+16*j+(8960-1024*i-16*j)*0.8), makelimit(1024*i+16*j+(34304-1024*i-16*j)*0.8), &my_color);
	        	ierr = MPE_Draw_point( graph, i+350, j, (MPE_Color)my_color);
	        	ierr = MPE_Draw_point( graph, 650-i, 100-j, (MPE_Color)my_color);
		}
	}
    }
    ierr = MPE_Update( graph );

    if ( my_rank == 0 ) {
        
        fprintf( stdout, "Hit any key then return to continue  " );
        fscanf( stdin, "%s", &ckey );
        fprintf( stdout, "\n" );
        
        sleep(1);
    }
    MPI_Barrier( MPI_COMM_WORLD );

    ierr = MPE_Close_graphics( &graph );

    MPI_Finalize();
    
    return 0;
}

int makelimit(int sum){
	if (sum < 0)sum=0;
	if (sum > 60000)sum=60000;
	return sum;
}
