#include "main.h"

int main(int argc, char ** argv)
{
  MPI_Init(&argc,&argv);
  int world_rank;
  MPI_Comm_rank(MCW, &world_rank);
  int world_size;
  MPI_Comm_size(MCW, &world_size);
  int rows,cols,seed,gran,print;

  CommLineArgs(argc,argv,&rows,&cols,&seed,&gran,&print);

  int i,j,k,m,r = 0,stride = (rows/4),lineStart;

  // Allocate space for arrays.


  MPI_Barrier(MCW);

  if(world_rank == 0)
  {
    srand(seed);
    // Allocate space for final sum array.
    int * finalSums = (int *)calloc(rows,sizeof(int));
    // Allocate space for transport buffers.
    int * transporter = (int *)calloc(rows * cols,sizeof(int));
    // MPI_Request array for sums.
    MPI_Request * sumReq = (MPI_Request *)calloc(3,sizeof(MPI_Request));
    // MPI_Status array for incoming sums.
    MPI_Status * status = (MPI_Status *)calloc(3,sizeof(MPI_Status));
    // MPI_Request array for matrix.
    MPI_Request * req = (MPI_Request *)calloc((3 * stride)/gran,sizeof(MPI_Request));

    printf("world_size = %d, world_rank = %d, print = %d, gran = %d, rows = %d, cols = %d\n",world_size,world_rank,print,gran,rows,cols);
    int walker = 0, offset = 0, checkSumA = 0, checkSumB = 0, checkSumC = 0, incorrect = 0;


    // Loop for (stride/gran) iterations.
    for (i = 0; i < (stride/gran); i++)
    {
      // Generate 3 * gran lines.
      for (j = 0; j < gran; j++)
      {
        for (k = 0; k < cols; k++)
        {
          transporter[(((stride * 1) + walker) * cols) + k] = QUAN;
          transporter[(((stride * 2) + walker) * cols) + k] = QUAN;
          transporter[(((stride * 3) + walker) * cols) + k] = QUAN;
        }
        walker++;
      }

      // Send generated lines to other processes.
      for (m = 1; m <= 3; m++)
      {
        MPI_Isend(
                  &transporter[((stride * m) + offset) * cols],
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
    if (print) printf("Results: \n");
    for (i = 0; i < stride; i++)
    {
      lineStart = i * cols;
      for(j = 0; j < cols; j++)
      {
        transporter[lineStart + j] = QUAN;
        finalSums[i] += transporter[lineStart + j];
      }
      if (print) printf("line sum for %d = %d,\n",i,finalSums[i]);
    }

    // Wait until all Ircevs are completed and display results.
    for (m = 1; m <= 3; m++)
    {
      MPI_Wait(
                &sumReq[m-1],
                &status[m-1]
              );
      if (print)
      {
        for (i = (stride * m); i < (stride * (m+1)); i++)
        {
          printf("line sum for %d = %d,\n",i,finalSums[i]);
        }
      }
    }
    printf("Calculations complete.\n");

    // Check to make sure finalSums matches the result of brute force
    // calculation in the transporter array.
    if (CHECKSUM)
    {
      printf("Checking results...\n");
      walker = 0;
      for (i = 0; i < (stride/gran); i++)
      {
        // Calculate the sum of each line and compare it to finalSums.
        for (j = 0; j < gran; j++)
        {
          checkSumA = 0, checkSumB = 0, checkSumC = 0;
          for (k = 0; k < cols; k++)
          {
            checkSumA += transporter[(((stride * 1) + walker) * cols) + k];
            checkSumB += transporter[(((stride * 2) + walker) * cols) + k];
            checkSumC += transporter[(((stride * 3) + walker) * cols) + k];
          }
          if (checkSumA != finalSums[(stride * 1) + walker])
          {
            printf("checkSumA != finalSums[%d] where %d != %d\n",(stride * 1) + walker,checkSumA,finalSums[(stride * 1) + walker]);
            incorrect++;
          }
          if (checkSumB != finalSums[(stride * 2) + walker])
          {
            printf("checkSumB != finalSums[%d] where %d != %d\n",(stride * 2) + walker,checkSumB,finalSums[(stride * 2) + walker]);
            incorrect++;
          }
          if (checkSumC != finalSums[(stride * 3) + walker])
          {
            printf("checkSumC != finalSums[%d] where %d != %d\n",(stride * 3) + walker,checkSumC,finalSums[(stride * 3) + walker]);
            incorrect++;
          }
          walker++;
        }
      }
      printf("Result check complete.\n");
      if (incorrect == 0)
      {
        printf("There were no errors in the results.\n");
      }
      else
      {
        printf("There were %d errors in the results.\n",incorrect);
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
        MPI_Status * status = (MPI_Status *)calloc(stride/gran,sizeof(MPI_Status));
        // MPI_Request array for matrix.
        MPI_Request * req = (MPI_Request *)calloc(stride/gran,sizeof(MPI_Request));
    MPI_Request rqs;
    MPI_Status stt;


    // Initiate stride/gran Irecvs of buffer size (line_size * gran).
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
    for (i = 0; i < stride/gran; i++)
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
              0,
              world_rank,
              MCW,
              &rqs
             );

    // Wait to deallocate memory until Isend completes.
    MPI_Wait(
              &rqs,
              &stt
            );

    free(matrix);
    free(lineSums);
    free(status);
  }

  MPI_Finalize();
  return 0;
}
