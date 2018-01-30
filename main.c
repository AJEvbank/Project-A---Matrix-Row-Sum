#include "main.h"

int main(int argc, char ** argv)
{
  MPI_Init(&argc,&argv);
  int world_rank;
  MPI_Comm_rank(MCW, &world_rank);
  int world_size;
  MPI_Comm_size(MCW, &world_size);
  // MPI_Status status_1;
  unsigned long rows,cols,seed,gran;

  CommLineArgs(argc,argv,&rows,&cols,&seed,&gran);
  printf("worldsize = %d, worldrank = %d, rows = %lu, cols = %lu, seed = %lu, gran = %lu\n\n",world_size,world_rank,rows,cols,seed,gran);

  // Allocate space for arrays.
      // Native matrix array
      // Sum array

  MPI_Barrier(MCW);

  if(world_rank == 0)
  {
    // Allocate space for final sum array.
    // Allocate space for transport buffers.

    // Loop for (2500/gran) iterations.
      // Generate 3 * gran lines and send to the processes.

    // Initiate Irecvs for line sums from other processes.
    // Generate native lines for rank 0 and calculate sums.

    // Wait until all Ircevs are completed and then display results.
  }
  else
  {
    // Initiate 2500/gran Irecvs of buffer size (line_size * gran).
    // Initiate wait for each Irecv in succession and calculate the sum
    // associated with each as it is completed.

    // Initiate Isend when line totals are completed.
  }



  MPI_Finalize();
  return 0;
}
