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

  int i,j,k,m,r = 0,stride = (rows/4);

  // Allocate space for arrays.
    // MPI_Request array for matrix.
    MPI_Request * req = (MPI_Request *)calloc(stride/gran,sizeof(MPI_Request));

  MPI_Barrier(MCW);

  if(world_rank == 0)
  {
    // Allocate space for final sum array.
    int * finalSums = (int *)calloc(rows,sizeof(int));
    // Allocate space for transport buffers.
    int * transporter = (int *)calloc(stride * 4 * cols,sizeof(int));
    // MPI_Request array for sums.
    MPI_Request * sumReq = (MPI_Request *)calloc(3,sizeof(MPI_Request));

    int walker = 0, offset = 0;

    // Loop for ((rows/4)/gran) iterations.
    for (i = 0; i < (stride/gran); i++)
    {
      // Generate 3 * gran lines.
      for (j = 0; j < gran; j++)
      {
        for (k = 0; k < cols; k++)
        {
          transporter[(stride * 1) + (walker * cols) + k] = QUAN;
          transporter[(stride * 2) + (walker * cols) + k] = QUAN;
          transporter[(stride * 3) + (walker * cols) + k] = QUAN;
        }
        walker++;
      }
      // Send generated lines to other processes.
      for (m = 1; m <= 3; m++)
      {
        MPI_Isend(
                  &transporter[(stride * m) + (offset * cols)],
                  cols * gran,
                  MPI_INT,
                  m,
                  i,
                  MCW,
                  &req[r]
        );
        r++;
      }
      offset += gran;
    }
    // Initiate Irecvs for line sums from other processes.
    for (m = 1; m <= 3; m++)
    {
      MPI_Irecv(
                &finalSums[stride * m],
                stride,
                MPI_INT,
                m,
                m,
                MCW,
                &sumReq[m-1];
      );
    }
    // Generate native lines for rank 0 and calculate sums.

    // Wait until all Ircevs are completed and then display results.

    free(finalSums);
    free(transporter);
  }
  else
  {
    // Allocate space for arrays.
        // Native matrix array
        int * matrix = (int *)calloc(stride * cols,sizeof(int));
        // Sum array
        int * lineSums = (int *)calloc(stride,sizeof(int));

    // Initiate 2500/gran Irecvs of buffer size (line_size * gran).
    // Initiate wait for each Irecv in succession and calculate the sum
    // associated with each as it is completed.

    // Initiate Isend when line totals are completed.
    free(matrix);
    free(lineSums);
  }



  MPI_Finalize();
  return 0;
}
