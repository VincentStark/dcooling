#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>

int graph_len;
int max_pos;
int bad_nodes   = 0;
int stop_vertex = -1;
int numpaths    = 0;

void checkPath(int **graph, int path[], int pos) {
 
  // Check if we came to a desired position
  if (path[pos-1] == stop_vertex) {
    if (pos == max_pos) {
      numpaths++;
      printf("Paths found so far: %d\n", numpaths);
    }
    return;
  }

  int v;
  for (v = 0; v < graph_len; v++) {
  
    // If this element is adjancent to the previous one
    // ...go this path recursively
    if (graph[ path[pos-1] ][v] == 1) {

      // Check if vertex is already added
      int i;
      int cont = 0;
      for (i = 0; i < pos; i++) {
        if (path[i] == v) {
          cont = 1;
          break;
        }
      }
      if (cont == 1) continue;

      path[pos] = v;
      checkPath(graph, path, pos+1);
      path[pos] = -1;
    }
  }

}

int main(void) {

  int width, height;

  // Ask for field's width and height
  printf("Width and height? ");
  scanf("%1i %1i", &width, &height);

  if ((width < 1 || width > 10) ||
      (height < 1 || height > 10)) {
    printf("ERROR: Width/height are out of range\n");
    return -1;
  }

  // Create playing field
  char field[width][height];
  int i, j;

  char *line;

  line = (char *)malloc(width*2 * sizeof *line);

  for (i = 0; i < height; i++) {
    printf("Line %2d? ", i);

    scanf(" %20[^\n]s", line);
    
    if ((int)strlen(line) != width*2 - 1) {
      printf("ERROR: You should enter exact number of values");
      free(line);
      return -1;
    }

    for (j = 0; j < width*2; j++) {
      field[i][j] = line[j*2] - '0';
    }
  }

  free(line);

  // Initialize graph
  graph_len = width*height;
  int **graph;

  // Allocate an array
  graph = (int **)malloc(graph_len * sizeof *graph);
  for (i = 0; i < graph_len; i++) {
    graph[i] = (int *)malloc(graph_len * sizeof *graph[i]);
  }

  // Fill-in default values
  for (i = 0; i < graph_len; i++) {
    for (j = 0; j < graph_len; j++) {
      graph[i][j] = 0;
    }
  }

  // Build graph
  int start_vertex   = -1;
  int current_vertex = -1;

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
 
      if (field[i][j] != 1) {
 
        // Calculate vertex number
        current_vertex = i*width + j;

        // Find start and stop vertices
        if (field[i][j] == 2) start_vertex = current_vertex;
        if (field[i][j] == 3) stop_vertex = current_vertex;

        //graph[current_vertex][current_vertex] = 1;

        if (i+1 < height && (field[i+1][j] == 0 || field[i+1][j] == 3)) {
          graph[current_vertex][(i+1)*width+j] = 1;
        }

        if (i-1 >= 0 && (field[i-1][j] == 0 || field[i-1][j] == 3)) {
          graph[current_vertex][(i-1)*width+j] = 1;
        }

        if (j+1 < width && (field[i][j+1] == 0 || field[i][j+1] == 3)) {
          graph[current_vertex][i*width+(j+1)] = 1;
        }

        if (j-1 >= 0 && (field[i][j-1] == 0 || field[i][j-1] == 3)) {
          graph[current_vertex][i*width+(j-1)] = 1;
        }
      
      }
      else {
        bad_nodes++;
      }
    }
  }


  // DEBUG: print graph matrix and additional info
  /*
  printf("    ");
  for (j = 0; j < graph_len; j++) {
    printf("%0.2d ", j);
  }
  printf("\n");
  for (i = 0; i < graph_len; i++) {
    printf("%0.2d: ", i);
    for (j = 0; j < graph_len; j++) {
      printf("%2d ", graph[i][j]);
    }
    printf("\n");
  }

  printf("Bad Nodes: %d\n", bad_nodes);
  printf("Start Vertex: %d\n", start_vertex);
  printf("Stop Vertex: %d\n", stop_vertex);
  */  

  // Initialize path
  int path[graph_len];

  for (i = 0; i < graph_len; i++) {
    path[i] = -1;
  }
  
  // Add initial vertex
  path[0] = start_vertex;

  // Max position
  max_pos = graph_len - bad_nodes;

  // Backtracing
  checkPath(graph, path, 1);

  printf("Paths: %d\n", numpaths);

  // Deallocate an array
  for (i = 0; i < graph_len; i++) {
    free(graph[i]);
  }
  free(graph);

  return 0;

}
