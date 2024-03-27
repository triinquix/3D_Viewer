#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

#include "data_handler.h"

double find_min_x(file_data *data) {
  double x_min = data->points[0].x;
  for (int i = 1; i < data->num_of_points; i++) {
    double tmp_x = data->points[i].x;
    if (tmp_x < x_min)
      x_min = tmp_x;
  }
  return x_min;
}

double find_max_x(file_data *data) {
  double x_max = data->points[0].x;
  for (int i = 1; i < data->num_of_points; i++) {
    double tmp_x = data->points[i].x;
    if (tmp_x > x_max)
      x_max = tmp_x;
  }
  return x_max;
}

double find_min_y(file_data *data) {
  double y_min = data->points[0].y;
  for (int i = 1; i < data->num_of_points; i++) {
    double tmp_y = data->points[i].y;
    if (tmp_y < y_min)
      y_min = tmp_y;
  }
  return y_min;
}

double find_max_y(file_data *data) {
  double y_max = data->points[0].y;
  for (int i = 1; i < data->num_of_points; i++) {
    double tmp_y = data->points[i].y;
    if (tmp_y > y_max)
      y_max = tmp_y;
  }
  return y_max;
}

double map_x(int width, double x, double x_min, double x_max) {
  double tmp_x = width * (x - x_min) / (x_max - x_min);
  return 20 + (width - 40) * tmp_x / width;
}

double map_y(int height, double y, double y_min, double y_max) {
  double tmp_y = height * (1 - (y - y_min) / (y_max - y_min));
  return 20 + (height - 40) * tmp_y / height;
}

point get_point_by_vertex_num(file_data *data, int vertex_num) {
  point point_to_return;
  if (vertex_num > 0)
    point_to_return = data->points[vertex_num - 1];
  else if (vertex_num < 0)
    point_to_return = data->points[data->num_of_points + vertex_num];
  return point_to_return;
}

void copy_file_data(file_data *source, file_data *destination) {
  destination->num_of_points = source->num_of_points;
  destination->num_of_face_elements = source->num_of_face_elements;
  // Copying points
  destination->points =
      (point *)malloc(sizeof(point) * destination->num_of_points);
  for (int i = 0; i < destination->num_of_points; i++)
    destination->points[i] = source->points[i];

  // Copying face elements
  destination->num_of_vertices_in_face_elem =
      (int *)malloc(sizeof(int) * destination->num_of_face_elements);
  destination->face_elements =
      (int **)malloc(sizeof(int *) * destination->num_of_face_elements);
  for (int i = 0; i < destination->num_of_face_elements; i++) {
    destination->num_of_vertices_in_face_elem[i] =
        source->num_of_vertices_in_face_elem[i];
    destination->face_elements[i] = (int *)malloc(
        sizeof(int) * destination->num_of_vertices_in_face_elem[i]);
    for (int j = 0; j < destination->num_of_vertices_in_face_elem[i]; j++)
      destination->face_elements[i][j] = source->face_elements[i][j];
  }
}

void init_rotation_matrix(double angles[3], double rotation_matrix[3][3]) {
  double alpha = angles[0] * M_PI / 180;
  double beta = angles[1] * M_PI / 180;
  double gamma = angles[2] * M_PI / 180;
  rotation_matrix[0][0] = cos(beta) * cos(gamma);
  rotation_matrix[0][1] =
      sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);
  rotation_matrix[0][2] =
      cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);
  rotation_matrix[1][0] = cos(beta) * sin(gamma);
  rotation_matrix[1][1] =
      sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
  rotation_matrix[1][2] =
      cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
  rotation_matrix[2][0] = -sin(beta);
  rotation_matrix[2][1] = sin(alpha) * cos(beta);
  rotation_matrix[2][2] = cos(alpha) * cos(beta);
}

void translate_data(file_data *data, double transform_matrix[3][3]) {
  double rotation_matrix[3][3];
  init_rotation_matrix(transform_matrix[1], rotation_matrix);
  double tmp_x = rotation_matrix[0][0] * transform_matrix[0][0] +
                 rotation_matrix[0][1] * transform_matrix[0][1] +
                 rotation_matrix[0][2] * transform_matrix[0][2];
  double tmp_y = rotation_matrix[1][0] * transform_matrix[0][0] +
                 rotation_matrix[1][1] * transform_matrix[0][1] +
                 rotation_matrix[1][2] * transform_matrix[0][2];
  double tmp_z = rotation_matrix[2][0] * transform_matrix[0][0] +
                 rotation_matrix[2][1] * transform_matrix[0][1] +
                 rotation_matrix[2][2] * transform_matrix[0][2];
  for (int i = 0; i < data->num_of_points; i++) {
    data->points[i].x += tmp_x;
    data->points[i].y += tmp_y;
    data->points[i].z += tmp_z;
  }
}

void scale_data(file_data *data, double scale_params[3]) {
  for (int i = 0; i < 10; i++) {
    data->points[i].x *= scale_params[0];
    data->points[i].y *= scale_params[1];
    data->points[i].z *= scale_params[2];
  }
}

void rotate_data(file_data *data, double angles[3]) {
  double rotation_matrix[3][3];
  init_rotation_matrix(angles, rotation_matrix);
  for (int i = 0; i < data->num_of_points; i++) {
    double init_x = data->points[i].x;
    double init_y = data->points[i].y;
    double init_z = data->points[i].z;
    data->points[i].x = rotation_matrix[0][0] * init_x +
                        rotation_matrix[0][1] * init_y +
                        rotation_matrix[0][2] * init_z;
    data->points[i].y = rotation_matrix[1][0] * init_x +
                        rotation_matrix[1][1] * init_y +
                        rotation_matrix[1][2] * init_z;
    data->points[i].z = rotation_matrix[2][0] * init_x +
                        rotation_matrix[2][1] * init_y +
                        rotation_matrix[2][2] * init_z;
  }
}

file_data *get_transformed_data(file_data *data, double transformation[3][3]) {
  file_data *transformed_data = (file_data *)malloc(sizeof(file_data));
  // Copy data
  copy_file_data(data, transformed_data);
  // Transform data

  // Scale data
  scale_data(transformed_data, transformation[2]);

  // Rotate data
  rotate_data(transformed_data, transformation[1]);

  // Translate data
  translate_data(transformed_data, transformation);

  return transformed_data;
}