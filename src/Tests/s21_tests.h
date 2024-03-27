#ifndef S21_TESTS_H
#define S21_TESTS_H

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.0000000001
#define M_PI 3.1415926535897932384626433

typedef struct point {
  double x;
  double y;
  double z;
} point;

void scale_data(point *points, double scale_params[3]);

void init_rotation_matrix(double angles[3], double rotation_matrix[3][3]);

void rotate_data(point *points, double angles[3]);

void translate_data(point *points, double transform_matrix[3][3]);

Suite *suite_scale_1(void);
Suite *suite_scale_2(void);
Suite *suite_scale_3(void);
Suite *suite_rotate_1(void);
Suite *suite_rotate_2(void);
Suite *suite_rotate_3(void);
Suite *suite_translate_1(void);
Suite *suite_translate_2(void);
Suite *suite_translate_3(void);

#endif