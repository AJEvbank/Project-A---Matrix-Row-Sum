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

  int i,j,k,m,r = 0,stride = (rows/4),lineStart;

  // Allocate space for arrays.
    // MPI_Request array for matrix.
    MPI_Request * req = (MPI_Request *)calloc(stride/gran,sizeof(MPI_Request));

  MPI_Barrier(MCW);

  if(world_rank == 0)
  {
    srand(seed);
    // Allocate space for final sum array.
    int * finalSums = (int *)calloc(rows,sizeof(int));
    // Allocate space for transport buffers.
    int * transporter = (int *)calloc(stride * 4 * cols,sizeof(int));
    // MPI_Request array for sums.
    MPI_Request * sumReq = (MPI_Request *)calloc(3,sizeof(MPI_Request));
    // MPI_Status array for incoming sums.
    MPI_Status * status = (MPI_Status *)calloc(3,sizeof(MPI_Status));

    int walker = 0, offset = 0;

    // Loop for (stride/gran) iterations.
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
                &sumReq[m-1]
              );
    }

    // Generate native lines for rank 0 and calculate sums.
    // Results are displayed as calculated.
    printf("Results: \n");
    for (i = 0; i < stride; i++)
    {
      lineStart = i * cols;
      for(j = 0; j < cols; j++)
      {
        transporter[lineStart + j] = QUAN;
        finalSums[i] += transporter[lineStart + j];
      }
      printf("line sum for %d = %d,\n",i,finalSums[i]);
    }

    // Wait until all Ircevs are completed and display results.
    for (m = 1; m <= 3; m++)
    {
      printf("Waiting...\n");
      MPI_Wait(
                &sumReq[m-1],
                &status[m-1]
              );
      for (i = (stride * m); i < stride; i++)
      {
        printf("line sum for %d = %d,\n",i,finalSums[i]);
      }
    }

    free(finalSums);
    free(transporter);
    free(sumReq);
    free(status);
  }
  else
  {
    // Allocate space for arrays.
        // Native matrix array
        int * matrix = (int *)calloc(stride * cols,sizeof(int));
        // Sum array
        int * lineSums = (int *)calloc(stride,sizeof(int));
        // MPI_Status array for incoming lines.
        MPI_Status * status = (MPI_Status *)calloc(stride,sizeof(MPI_Status));
    MPI_Request r;
    MPI_Status s;


    // Initiate stride/gran Irecvs of buffer size (line_size * gran).
    printf("Process %d initiating Irecvs\n",world_rank);
    for (i = 0; i < stride/gran; i++)
    {
      MPI_Irecv(
                &matrix[i * gran * cols],
                gran * cols,
                MPI_INT,
                0,
                i,
                MCW,
                &req[i]
              );
    }

    // Initiate wait for each Irecv in succession and calculate the sum
    // associated with each as it is completed.
    for (i = 0; i < stride; i++)
    {
      MPI_Wait(
                &req[i],
                &status[i]
              );
      for (j = 0; j < gran; j++)
      {
        lineSums[(i * gran) + j] = calcSum(&matrix[((i * gran) + j) * cols],cols);
      }
    }

    // Initiate Isend when line totals are completed.
    MPI_Isend(
              lineSums,
              stride,
              MPI_INT,
              world_rank,
              0,
              MCW,
              &r
             );

    // Wait to deallocate memory until Isend completes.
    MPI_Wait(
              &r,
              &s
            );
    free(matrix);
    free(lineSums);
    free(status);
  }



  MPI_Finalize();
  return 0;
}
