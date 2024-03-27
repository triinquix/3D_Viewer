#include "s21_tests.h"

// First 5 point XYZ coordinates of man.obj
double **get_points(void) {
  double points_array[][3] = {{111.831268, 194.642273, -9.411493},
                              {114.463310, 195.502716, -8.250872},
                              {115.588570, 195.756531, -5.245904},
                              {111.820358, 194.749420, -5.151659},
                              {114.483192, 195.598404, -2.144564}};
  double **points = (double **)malloc(sizeof(double *) * 5);
  for (int i = 0; i < 5; i++)
    points[i] = (double *)malloc(sizeof(double) * 3);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++)
      points[i][j] = points_array[i][j];
  }
  return points;
}

// double **mult_points(double** points_array, int x, int y, int z) {
//     for (int i = 0; i < 5; i++) {
//         for (int j = 0; j < 3; j++) {
//             if (j == 0) points_array[i][j] *= x;
//             else if (j == 1) points_array[i][j] *= y;
//             else points_array[i][j] *= z;
//         }
//     }
//     return points_array;
// }

point *init_points(point *points, double **points_array) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      if (j == 0)
        points[i].x = points_array[i][j];
      else if (j == 1)
        points[i].y = points_array[i][j];
      else
        points[i].z = points_array[i][j];
    }
  }
  return points;
}

void clean_points(double **points_array) {
  for (int i = 0; i < 5; i++) {
    free(points_array[i]);
  }
  free(points_array);
}

void scale_data(point *points, double scale_params[3]) {
  for (int i = 0; i < 5; i++) {
    points[i].x *= scale_params[0];
    points[i].y *= scale_params[1];
    points[i].z *= scale_params[2];
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

void rotate_data(point *points, double angles[3]) {
  double rotation_matrix[3][3];
  init_rotation_matrix(angles, rotation_matrix);
  for (int i = 0; i < 5; i++) {
    double init_x = points[i].x;
    double init_y = points[i].y;
    double init_z = points[i].z;
    points[i].x = rotation_matrix[0][0] * init_x +
                  rotation_matrix[0][1] * init_y +
                  rotation_matrix[0][2] * init_z;
    points[i].y = rotation_matrix[1][0] * init_x +
                  rotation_matrix[1][1] * init_y +
                  rotation_matrix[1][2] * init_z;
    points[i].z = rotation_matrix[2][0] * init_x +
                  rotation_matrix[2][1] * init_y +
                  rotation_matrix[2][2] * init_z;
  }
}

void translate_data(point *points, double transform_matrix[3][3]) {
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
  for (int i = 0; i < 5; i++) {
    points[i].x += tmp_x;
    points[i].y += tmp_y;
    points[i].z += tmp_z;
  }
}

START_TEST(test_scale_1) {
  double result_array[][3] = {{111.831268, 389.284546, -28.234479},
                              {114.463310, 391.005432, -24.752616},
                              {115.588570, 391.513062, -15.737712},
                              {111.820358, 389.49884, -15.454977},
                              {114.483192, 391.196808, -6.433692}};

  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double scale_params[3] = {1, 2, 3};
  scale_data(points, scale_params);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }

  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_scale_2) {

  double result_array[][3] = {{335.493804, 389.284546, -9.411493},
                              {343.389930, 391.005432, -8.250872},
                              {346.765710, 391.513062, -5.245904},
                              {335.461074, 389.498840, -5.151659},
                              {343.449576, 391.196808, -2.144564}};

  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double scale_params[3] = {3, 2, 1};
  scale_data(points, scale_params);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_scale_3) {
  double result_array[][3] = {{111.831268, 194.642273, -9.411493},
                              {114.463310, 195.502716, -8.250872},
                              {115.588570, 195.756531, -5.245904},
                              {111.820358, 194.749420, -5.151659},
                              {114.483192, 195.598404, -2.144564}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double scale_params[3] = {1, 1, 1};
  scale_data(points, scale_params);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_rotate_1) {
  double result_array[][3] = {{111.831268, 9.411493, 194.642273},
                              {114.463310, 8.250872, 195.502716},
                              {115.588570, 5.245904, 195.756531},
                              {111.820358, 5.151659, 194.749420},
                              {114.483192, 2.144564, 195.598404}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double angles[3] = {90, 0, 0};
  rotate_data(points, angles);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_rotate_2) {
  double result_array[][3] = {{-9.411493, 194.642273, -111.831268},
                              {-8.250872, 195.502716, -114.463310},
                              {-5.245904, 195.756531, -115.588570},
                              {-5.151659, 194.749420, -111.820358},
                              {-2.144564, 195.598404, -114.483192}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double angles[3] = {0, 90, 0};
  rotate_data(points, angles);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_rotate_3) {
  double result_array[][3] = {{-194.642273, 111.831268, -9.411493},
                              {-195.502716, 114.463310, -8.250872},
                              {-195.756531, 115.588570, -5.245904},
                              {-194.749420, 111.820358, -5.151659},
                              {-195.598404, 114.483192, -2.144564}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double angles[3] = {0, 0, 90};
  rotate_data(points, angles);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_translate_1) {
  double result_array[][3] = {{116.831268, 194.642273, -9.411493},
                              {119.463310, 195.502716, -8.250872},
                              {120.588570, 195.756531, -5.245904},
                              {116.820358, 194.749420, -5.151659},
                              {119.483192, 195.598404, -2.144564}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double transform_matrix[][3] = {{5, 0, 0}, {0, 0, 0}, {1, 1, 1}};
  translate_data(points, transform_matrix);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_translate_2) {
  double result_array[][3] = {{111.831268, 199.642273, -9.411493},
                              {114.463310, 200.502716, -8.250872},
                              {115.588570, 200.756531, -5.245904},
                              {111.820358, 199.749420, -5.151659},
                              {114.483192, 200.598404, -2.144564}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double transform_matrix[][3] = {{0, 5, 0}, {0, 0, 0}, {1, 1, 1}};
  translate_data(points, transform_matrix);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

START_TEST(test_translate_3) {
  double result_array[][3] = {{111.831268, 194.642273, -4.411493},
                              {114.463310, 195.502716, -3.250872},
                              {115.588570, 195.756531, -0.245904},
                              {111.820358, 194.749420, -0.151659},
                              {114.483192, 195.598404, 2.855436}};
  point *points = (point *)malloc(sizeof(point) * 5);
  double **points_array = get_points();
  points = init_points(points, points_array);

  double transform_matrix[][3] = {{0, 0, 5}, {0, 0, 0}, {1, 1, 1}};
  translate_data(points, transform_matrix);
  char equal = 1;

  for (int i = 0; i < 5 && equal; i++) {
    for (int j = 0; j < 3 && equal; j++) {
      switch (j) {
      case 0: {
        if (points[i].x - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 1: {
        if (points[i].y - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      case 2: {
        if (points[i].z - result_array[i][j] > EPSILON)
          equal = 0;
        break;
      }
      }
    }
  }
  ck_assert_int_eq(1, equal);

  clean_points(points_array);
  free(points);
}
END_TEST

Suite *suite_scale_1(void) {
  Suite *s;
  TCase *tc_scale_1;

  s = suite_create("scale_1");
  tc_scale_1 = tcase_create("scale_1 tcase");

  tcase_add_test(tc_scale_1, test_scale_1);
  suite_add_tcase(s, tc_scale_1);
  tcase_set_timeout(tc_scale_1, 100);

  return s;
}

Suite *suite_scale_2(void) {
  Suite *s;
  TCase *tc_scale_2;

  s = suite_create("scale_2");
  tc_scale_2 = tcase_create("scale_2 tcase");

  tcase_add_test(tc_scale_2, test_scale_2);
  suite_add_tcase(s, tc_scale_2);
  tcase_set_timeout(tc_scale_2, 100);

  return s;
}

Suite *suite_scale_3(void) {
  Suite *s;
  TCase *tc_scale_3;

  s = suite_create("scale_3");
  tc_scale_3 = tcase_create("scale_3 tcase");

  tcase_add_test(tc_scale_3, test_scale_3);
  suite_add_tcase(s, tc_scale_3);
  tcase_set_timeout(tc_scale_3, 100);

  return s;
}

Suite *suite_rotate_1(void) {
  Suite *s;
  TCase *tc_rotate_1;

  s = suite_create("rotate_1");
  tc_rotate_1 = tcase_create("rotate_1 tcase");

  tcase_add_test(tc_rotate_1, test_rotate_1);
  suite_add_tcase(s, tc_rotate_1);
  tcase_set_timeout(tc_rotate_1, 100);

  return s;
}

Suite *suite_rotate_2(void) {
  Suite *s;
  TCase *tc_rotate_2;

  s = suite_create("rotate_2");
  tc_rotate_2 = tcase_create("rotate_2 tcase");

  tcase_add_test(tc_rotate_2, test_rotate_2);
  suite_add_tcase(s, tc_rotate_2);
  tcase_set_timeout(tc_rotate_2, 100);

  return s;
}

Suite *suite_rotate_3(void) {
  Suite *s;
  TCase *tc_rotate_3;

  s = suite_create("rotate_3");
  tc_rotate_3 = tcase_create("rotate_3 tcase");

  tcase_add_test(tc_rotate_3, test_rotate_3);
  suite_add_tcase(s, tc_rotate_3);
  tcase_set_timeout(tc_rotate_3, 100);

  return s;
}

Suite *suite_translate_1(void) {
  Suite *s;
  TCase *tc_translate_1;

  s = suite_create("translate_1");
  tc_translate_1 = tcase_create("translate_1 tcase");

  tcase_add_test(tc_translate_1, test_translate_1);
  suite_add_tcase(s, tc_translate_1);
  tcase_set_timeout(tc_translate_1, 100);

  return s;
}

Suite *suite_translate_2(void) {
  Suite *s;
  TCase *tc_translate_2;

  s = suite_create("translate_2");
  tc_translate_2 = tcase_create("translate_2 tcase");

  tcase_add_test(tc_translate_2, test_translate_2);
  suite_add_tcase(s, tc_translate_2);
  tcase_set_timeout(tc_translate_2, 100);

  return s;
}

Suite *suite_translate_3(void) {
  Suite *s;
  TCase *tc_translate_3;

  s = suite_create("translate_3");
  tc_translate_3 = tcase_create("translate_3 tcase");

  tcase_add_test(tc_translate_3, test_translate_3);
  suite_add_tcase(s, tc_translate_3);
  tcase_set_timeout(tc_translate_3, 100);

  return s;
}
