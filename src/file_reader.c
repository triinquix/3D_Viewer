#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_handler.h"

int store_vertex(file_data *data, char *line_with_vertex) {
  int error = 0;
  char *token = NULL;
  token = strtok(line_with_vertex, " ");
  if (token) {
    double vec[3];
    for (int i = 0; i < 3 && !error; i++) {
      token = strtok(NULL, " ");
      char *ptr;
      if (token) {
        vec[i] = strtod(token, &ptr);
        if (strcmp(token, ptr) == 0)
          error = 1;
      } else
        error = 1;
    }
    if (!error)
      add_point(data, vec);
  } else
    error = 1;
  return error;
}

int store_polygons(file_data *data, char *line_with_polygons) {
  int error = 0;
  char *token = strtok(line_with_polygons, " ");
  int counter = 0;
  int *polygons = NULL;
  while (token && !error) {
    token = strtok(NULL, " ");
    char *ptr;
    if (token) {
      int vertex_num = (int)strtol(token, &ptr, 10);
      if (vertex_num == 0) {
        if (counter < 3)
          error = 1;
      } else {
        counter++;
        if (counter > 1)
          polygons = (int *)realloc(polygons, sizeof(int) * counter);
        else
          polygons = (int *)malloc(sizeof(int));
        polygons[counter - 1] = vertex_num;
      }
    }
  }
  if (!error)
    add_face_elements(data, polygons, counter);
  return error;
}

file_data *get_data_from_file(const char *file_name) {
  file_data *data = NULL;
  int error = 0;
  FILE *file_to_read = fopen(file_name, "r");
  if (file_to_read) {
    data = init_file_data();
    char line[1000];
    while (fgets(line, 1000, file_to_read) && !error) {
      if (line[0] == 'v' && line[1] == ' ')
        error = store_vertex(data, line);
      else if (line[0] == 'f' && line[1] == ' ')
        error = store_polygons(data, line);
    }
    fclose(file_to_read);
  }
  // if error
  if (error)
    free_file_data(data);
  return data;
}
