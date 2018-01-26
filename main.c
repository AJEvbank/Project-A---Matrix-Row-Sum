#include "main.h"

int main(int argc, char ** argv)
{
  MPI_Init(&argc,&argv);
  int world_rank;
  MPI_Comm_rank(MCW, &world_rank);
  int world_size;
  MPI_Comm_size(MCW, &world_size);
  MPI_Status status_1;

  printf("worldsize = %d, worldrank = %d\n",world_size,world_rank);





  MPI_Finalize();
  return 0;
}
