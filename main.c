#include "main.h"

int main(int argc, char ** argv)
{
  MPI_Init(&argc,&argv);
  int world_rank;
  MPI_Comm_rank(MCW, &world_rank);
  int world_size;
  MPI_Comm_size(MCW, &world_size);
  MPI_Status status_1;
  unsigned long rows,cols,seed;

  CommLineArgs(argc,argv,&rows,&cols,&seed);
  printf("worldsize = %d, worldrank = %d, rows = %lu, cols = %lu, seed = %lu\n",world_size,world_rank,rows,cols,seed);

  MPI_Barrier(MCW);
  int message_id = 0;
  int sender,recipient;
  char message[20];
  if(world_rank == 0)
  {

  }
  else
  {

  }



  MPI_Finalize();
  return 0;
}
