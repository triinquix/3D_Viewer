#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <gtk/gtk.h>
#include <time.h>
#include <wand/MagickWand.h>

typedef struct point {
  double x;
  double y;
  double z;
} point;

typedef struct file_data {
  point *points;
  int num_of_points;
  int **face_elements;
  int num_of_face_elements;
  int *num_of_vertices_in_face_elem;
} file_data;

typedef struct gif_struct {
  int count;
  double delta[3][3];
  char file_names[10][23];
} gif_struct;

typedef struct closure_struct {
  file_data *data;
  gif_struct *gif;
  GtkWidget *grid;
} closure_struct;

enum disp_method { NONE, CIRCLE, SQUARE };

typedef struct settings {
  int background_colour[3];
  enum disp_method vertex_method;
  int vrtx_colour[3];
  double vrtx_size;
  int is_dash_active;
  int line_colour[3];
  double line_width;
} settings;

// Signals
void save_jpeg(GtkWidget *button, GtkWidget *d_area);

void save_bmp(GtkWidget *button, GtkWidget *d_area);

void record_gif(GtkWidget *button, GtkWidget *d_area);

void init_file_info(GtkWidget *grid, file_data *data);

void init_gif_delta(GtkWidget *grid, gif_struct *gif);

int get_gif_transformation(GtkWidget *grid, double transformation[3][3],
                           gif_struct *gif);

void on_edge_colour_changed(GtkWidget *button, GtkWidget *d_area);

void on_width_changed(GtkWidget *button, GtkWidget *d_area);

void on_deshed(GtkWidget *button, GtkWidget *d_area);

void on_vrtx_size_changed(GtkWidget *button, GtkWidget *d_area);

void on_window_close(closure_struct *passed_data);

void on_background_change(GtkWidget *button, GtkWidget *drawing_area);

void on_method_changed(GtkWidget *button, GtkWidget *drawing_area);

void on_colour_vertex_changed(GtkWidget *button, GtkWidget *drawing_area);

void draw_func(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height,
               gpointer data);

// Adds point to the data
void add_point(file_data *data, double vec[3]);

// Adds face elements to the data
void add_face_elements(file_data *data, int *polygons, int num_of_vertices);

// Frees file data
void free_file_data(file_data *data);

// Inits data struct
file_data *init_file_data();

// Read data from file
file_data *get_data_from_file(const char *file_name);

// Gets settings grom conf file
settings *get_settings();

// Finds minimal x in data
double find_min_x(file_data *data);

// Finds maximal x in data
double find_max_x(file_data *data);

// Finds minimal y in data
double find_min_y(file_data *data);

// Finds maximal y in data
double find_max_y(file_data *data);

// Finds x coordinate in window
double map_x(int width, double x, double x_min, double x_max);

// Finds y coordinate in window
double map_y(int height, double y, double y_min, double y_max);

// Gets a point by vertex number
point get_point_by_vertex_num(file_data *data, int vertex_num);

// Copyies data
void copy_file_data(file_data *source, file_data *destination);

// Transformes given data
file_data *get_transformed_data(file_data *data, double transformation[3][3]);

#endif // DATA_HANDLER_H
