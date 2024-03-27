#include "data_handler.h"

// Changes background colour
void change_background_colour(settings *settings_to_apply, int width,
                              int height, cairo_t *cr) {
  cairo_set_source_rgb(cr, settings_to_apply->background_colour[0],
                       settings_to_apply->background_colour[1],
                       settings_to_apply->background_colour[2]);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);
}

// Gets transformation matrix from main window
int get_transformation(GtkWidget *grid, double transformation[3][3]) {
  int error = 0;
  // Getting translsation
  for (int i = 0; i < 3 && !error; i++) {
    for (int j = 0; j < 3 && !error; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4 + j, 1 + i);
      char *input = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
      char *ptr;
      double number = strtod(input, &ptr);
      if (strcmp(input, ptr) == 0)
        error++;
      else
        transformation[i][j] = number;
    }
  }
  return error;
}

// Draws face elements
void draw_face_elements(file_data *data, file_data *transformed_data,
                        cairo_t *cr, int width, int height,
                        settings *current_settings) {
  cairo_set_source_rgb(cr, current_settings->line_colour[0],
                       current_settings->line_colour[1],
                       current_settings->line_colour[2]);
  cairo_set_line_width(cr, current_settings->line_width);
  cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 13);
  if (current_settings->is_dash_active) {
    double tmp[2] = {1, 1};
    cairo_set_dash(cr, tmp, 1, 1);
  }
  double x_min = find_min_x(data);
  double x_max = find_max_x(data);
  double y_min = find_min_y(data);
  double y_max = find_max_y(data);
  for (int i = 0; i < transformed_data->num_of_face_elements; i++) {
    for (int j = 1; j < transformed_data->num_of_vertices_in_face_elem[i];
         j++) {
      int vertex_1 = transformed_data->face_elements[i][j - 1];
      int vertex_2 = transformed_data->face_elements[i][j];
      point point_1 = get_point_by_vertex_num(transformed_data, vertex_1);
      point point_2 = get_point_by_vertex_num(transformed_data, vertex_2);
      cairo_move_to(cr, map_x(width, point_1.x, x_min, x_max),
                    map_y(height, point_1.y, y_min, y_max));
      cairo_line_to(cr, map_x(width, point_2.x, x_min, x_max),
                    map_y(height, point_2.y, y_min, y_max));
    }
    // Connecting last and first points
    int vertex_start = transformed_data->face_elements[i][0];
    int vertex_end =
        transformed_data->face_elements
            [i][transformed_data->num_of_vertices_in_face_elem[i] - 1];
    point point_1 = get_point_by_vertex_num(transformed_data, vertex_start);
    point point_2 = get_point_by_vertex_num(transformed_data, vertex_end);
    cairo_move_to(cr, map_x(width, point_1.x, x_min, x_max),
                  map_y(height, point_1.y, y_min, y_max));
    cairo_line_to(cr, map_x(width, point_2.x, x_min, x_max),
                  map_y(height, point_2.y, y_min, y_max));
  }
  cairo_stroke(cr);
}

// Draws vertices
void draw_vertices(file_data *data, file_data *transformed_data, cairo_t *cr,
                   int width, int height, settings *current_settings) {
  cairo_set_source_rgb(cr, current_settings->vrtx_colour[0],
                       current_settings->vrtx_colour[1],
                       current_settings->vrtx_colour[2]);
  cairo_set_line_width(cr, 0.5);
  cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, current_settings->vrtx_size);
  // Getting display method
  char *char_to_print;
  if (current_settings->vertex_method == NONE)
    char_to_print = "";
  else if (current_settings->vertex_method == CIRCLE)
    char_to_print = "o";
  else
    char_to_print = "▫";
  double x_min = find_min_x(data);
  double x_max = find_max_x(data);
  double y_min = find_min_y(data);
  double y_max = find_max_y(data);
  for (int i = 0; i < transformed_data->num_of_points; i++) {
    double x = map_x(width, transformed_data->points[i].x, x_min, x_max);
    double y = map_y(height, transformed_data->points[i].y, y_min, y_max);
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, char_to_print);
  }
  cairo_stroke(cr);
}

int get_gif_transformation(GtkWidget *grid, double transformation[3][3],
                           gif_struct *gif) {
  int error = 0;
  for (int i = 0; i < 3 && !error; i++) {
    for (int j = 0; j < 3 && !error; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4 + j, 1 + i);
      char *input = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
      char *ptr;
      double number = strtod(input, &ptr);
      if (strcmp(input, ptr) == 0)
        error++;
      else if (i != 2 || (i == 2 && number != 1)) {
        if (number > 0) {
          transformation[i][j] = number - gif->delta[i][j] * gif->count;
        } else
          transformation[i][j] = number - gif->delta[i][j] * gif->count;
      } else {
        transformation[i][j] = number;
      }
    }
  }
  return error;
}

// Is executed when widget needs to be redrawn
void draw_func(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height,
               gpointer data) {
  closure_struct *strct = data;
  GtkWidget *grid = strct->grid;
  gif_struct *gif = strct->gif;
  file_data *f_data = strct->data;
  int error = 0;
  // Setting drawing area size
  gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(drawing_area), height);
  gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(drawing_area), width);
  double transformation[3][3];
  init_gif_delta(grid, gif);
  if (gtk_grid_get_child_at(GTK_GRID(grid), 0, 5)) {
    for (int i = 0; i < 10; i++) {
      error = get_gif_transformation(grid, transformation, gif);
      if (!error) {
        // Reading settings data
        settings *new_settings = get_settings();
        // Changing background colour
        change_background_colour(new_settings, width, height, cr);
        // Get transformed data
        file_data *transformed_data =
            get_transformed_data(f_data, transformation);
        // Draw
        draw_vertices(f_data, transformed_data, cr, width, height,
                      new_settings);
        draw_face_elements(f_data, transformed_data, cr, width, height,
                           new_settings);
        // Freeing data
        free(new_settings);
        free_file_data(transformed_data);
        // For recording GIF
        if (gtk_grid_get_child_at(GTK_GRID(grid), 0, 5)) {
          srand(time(NULL));
          int ran_num = rand() % 1000000000;
          char ran_filename[23] = "\0";
          sprintf(ran_filename, "%d%s", ran_num, "drawing.jpeg");
          cairo_surface_t *surface = cairo_get_target(cr);
          GdkPixbuf *pixbuf =
              gdk_pixbuf_get_from_surface(surface, 0, 0, width, height);
          gdk_pixbuf_save(pixbuf, ran_filename, "jpeg", NULL, NULL);
          g_object_unref(pixbuf);
          sprintf(gif->file_names[gif->count], "%s", ran_filename);
          gif->count++;
          if (gif->count == 10) {
            MagickWandGenesis();
            MagickWand *wand;
            wand = NewMagickWand();
            for (int i = 0; i < 10; i++) {
              MagickReadImage(wand, gif->file_names[i]);
            }
            MagickWriteImages(wand, "output.gif", MagickTrue);
            MagickWandTerminus();
            gtk_grid_remove(GTK_GRID(grid),
                            gtk_grid_get_child_at(GTK_GRID(grid), 0, 5));
            for (int i = 0; i < 10; i++) {
              remove(gif->file_names[i]);
            }
            gif->count = 0;
          }
        }
      }
    }
  } else {
    error = get_transformation(grid, transformation);
    if (!error) {
      // Reading settings data
      settings *new_settings = get_settings();
      // Changing background colour
      change_background_colour(new_settings, width, height, cr);
      // Get transformed data
      file_data *transformed_data =
          get_transformed_data(f_data, transformation);
      // Draw
      draw_vertices(f_data, transformed_data, cr, width, height, new_settings);
      draw_face_elements(f_data, transformed_data, cr, width, height,
                         new_settings);
      // Freeing data
      free(new_settings);
      free_file_data(transformed_data);
    }
  }
  // For saving photos
  if (gtk_grid_get_child_at(GTK_GRID(grid), 2, 3)) {
    const char *text = gtk_label_get_text(
        GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(grid), 2, 3)));
    if (strcmp(text, ".jpeg saved!") == 0) {
      cairo_surface_t *surface = cairo_get_target(cr);
      GdkPixbuf *pixbuf =
          gdk_pixbuf_get_from_surface(surface, 0, 0, width, height);
      gdk_pixbuf_save(pixbuf, "drawing.jpeg", "jpeg", NULL, NULL);
      g_object_unref(pixbuf);
      gtk_grid_remove(GTK_GRID(grid),
                      gtk_grid_get_child_at(GTK_GRID(grid), 2, 3));
    } else {
      cairo_surface_t *surface = cairo_get_target(cr);
      GdkPixbuf *pixbuf =
          gdk_pixbuf_get_from_surface(surface, 0, 0, width, height);
      gdk_pixbuf_save(pixbuf, "drawing.bmp", "bmp", NULL, NULL);
      g_object_unref(pixbuf);
      gtk_grid_remove(GTK_GRID(grid),
                      gtk_grid_get_child_at(GTK_GRID(grid), 2, 3));
    }
  }
}