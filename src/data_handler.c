#include "data_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

file_data *init_file_data() {
  file_data *data = (file_data *)malloc(sizeof(file_data));
  data->num_of_points = 0;
  data->points = NULL;
  data->face_elements = NULL;
  data->num_of_face_elements = 0;
  data->num_of_vertices_in_face_elem = NULL;
  return data;
}

void add_point(file_data *data, double vec[3]) {
  data->num_of_points++;
  if (data->num_of_points > 1)
    data->points =
        (point *)realloc(data->points, sizeof(point) * data->num_of_points);
  else
    data->points = (point *)malloc(sizeof(point));
  data->points[data->num_of_points - 1].x = vec[0];
  data->points[data->num_of_points - 1].y = vec[1];
  data->points[data->num_of_points - 1].z = vec[2];
}

void add_face_elements(file_data *data, int *polygons, int num_of_vertices) {
  data->num_of_face_elements++;
  if (data->num_of_face_elements > 1) {
    data->face_elements = (int **)realloc(
        data->face_elements, sizeof(int *) * data->num_of_face_elements);
    data->num_of_vertices_in_face_elem =
        (int *)realloc(data->num_of_vertices_in_face_elem,
                       sizeof(int) * data->num_of_face_elements);
  } else {
    data->face_elements = (int **)malloc(sizeof(int *));
    data->num_of_vertices_in_face_elem = (int *)malloc(sizeof(int));
  }
  data->face_elements[data->num_of_face_elements - 1] = polygons;
  data->num_of_vertices_in_face_elem[data->num_of_face_elements - 1] =
      num_of_vertices;
}

void free_file_data(file_data *data) {
  if (data->num_of_points) {
    free(data->points);
    data->points = NULL;
  }
  if (data->num_of_face_elements) {
    for (int i = 0; i < data->num_of_face_elements; i++) {
      free(data->face_elements[i]);
      data->face_elements[i] = NULL;
    }
    free(data->face_elements);
    data->face_elements = NULL;
    free(data->num_of_vertices_in_face_elem);
    data->num_of_vertices_in_face_elem = NULL;
  }
  free(data);
  data = NULL;
}

void get_background_colour(char *line, FILE *settings_file,
                           settings *new_settings) {
  fgets(line, 100, settings_file);
  char *token;
  token = strtok(line, " ");
  token = strtok(NULL, " ");
  new_settings->background_colour[0] = (int)strtol(token, NULL, 10);
  token = strtok(NULL, " ");
  new_settings->background_colour[1] = (int)strtol(token, NULL, 10);
  token = strtok(NULL, " ");
  new_settings->background_colour[2] = (int)strtol(token, NULL, 10);
}

void get_disp_method(char *line, FILE *settings_file, settings *new_settings) {
  fgets(line, 100, settings_file);
  char *token;
  if (strstr(line, "NONE"))
    new_settings->vertex_method = NONE;
  else if (strstr(line, "CIRCLE"))
    new_settings->vertex_method = CIRCLE;
  else
    new_settings->vertex_method = SQUARE;
  // Getting vertices colour
  fgets(line, 100, settings_file);
  token = strtok(line, " ");
  new_settings->vrtx_colour[0] = (int)strtol(token, NULL, 10);
  token = strtok(NULL, " ");
  new_settings->vrtx_colour[1] = (int)strtol(token, NULL, 10);
  token = strtok(NULL, " ");
  new_settings->vrtx_colour[2] = (int)strtol(token, NULL, 10);
}

void get_edge_colour(char *line, FILE *settings_file, settings *new_settings) {
  char *token;
  fgets(line, 100, settings_file);
  for (int i = 0; i < 3; i++) {
    if (i == 0)
      token = strtok(line, " ");
    else
      token = strtok(NULL, " ");
    new_settings->line_colour[i] = (int)strtol(token, NULL, 10);
  }
}

settings *get_settings() {
  settings *new_settings = NULL;
  FILE *settings_file = fopen("settings.conf", "r");
  if (settings_file) {
    char line[100];
    new_settings = (settings *)malloc(sizeof(settings));
    if (new_settings) {
      // Getting background colour
      get_background_colour(line, settings_file, new_settings);

      // Getting vertex display method
      get_disp_method(line, settings_file, new_settings);

      // Getting vertex size
      fgets(line, 100, settings_file);
      new_settings->vrtx_size = strtod(line, NULL);

      // Getting dashed status
      fgets(line, 100, settings_file);
      new_settings->is_dash_active = (int)strtol(line, NULL, 10);

      // Getting edge colour
      get_edge_colour(line, settings_file, new_settings);

      // Getting line width
      fgets(line, 100, settings_file);
      new_settings->line_width = strtod(line, NULL);
    }

    fclose(settings_file);
  }
  return new_settings;
}
