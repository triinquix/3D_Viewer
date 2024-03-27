#include "data_handler.h"

void init_gif_delta(GtkWidget *grid, gif_struct *gif) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4 + j, 1 + i);
      char *input = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
      char *ptr;
      double number = strtod(input, &ptr);
      if (i == 2)
        gif->delta[i][j] = (number - 1) / 10.;
      else
        gif->delta[i][j] = number / 10.;
    }
  }
}

void init_file_info(GtkWidget *grid, file_data *data) {
  GtkWidget *labels[4];
  labels[0] = gtk_label_new("Number of vertices");
  labels[1] = gtk_label_new("Number of edges");
  gtk_grid_attach(GTK_GRID(grid), labels[0], 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), labels[1], 1, 6, 1, 1);
  char buffer1[100];
  sprintf(buffer1, "%d", data->num_of_points);
  labels[2] = gtk_label_new(buffer1);
  gtk_grid_attach(GTK_GRID(grid), labels[2], 0, 7, 1, 1);
  char buffer2[100];
  sprintf(buffer2, "%d", data->num_of_face_elements);
  labels[3] = gtk_label_new(buffer2);
  gtk_grid_attach(GTK_GRID(grid), labels[3], 1, 7, 1, 1);
}

void save_jpeg(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);
  if (!gtk_grid_get_child_at(GTK_GRID(grid), 2, 3))
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new_with_mnemonic(".jpeg saved!"),
                    2, 3, 1, 1);
  gtk_widget_queue_draw(d_area);
}

void save_bmp(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);
  if (!gtk_grid_get_child_at(GTK_GRID(grid), 2, 3))
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new_with_mnemonic(".bmp saved!"),
                    2, 3, 1, 1);
  gtk_widget_queue_draw(d_area);
}

void record_gif(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);
  if (!gtk_grid_get_child_at(GTK_GRID(grid), 0, 5))
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new_with_mnemonic("Recording..."),
                    0, 5, 1, 1);
  gtk_widget_queue_draw(d_area);
}

void change_line(int line_num, char *line_to_insert) {
  // Copying file
  FILE *settings_file = fopen("settings.conf", "r");
  FILE *buffer_file = fopen("tmp.txt", "w");
  char line[100];
  while (fgets(line, 100, settings_file))
    fprintf(buffer_file, "%s", line);
  fclose(settings_file);
  fclose(buffer_file);
  // Rewriting file
  settings_file = fopen("settings.conf", "w");
  buffer_file = fopen("tmp.txt", "r");
  int line_counter = 1;
  while (fgets(line, 100, buffer_file)) {
    if (line_counter == line_num)
      fprintf(settings_file, "%s\n", line_to_insert);
    else
      fprintf(settings_file, "%s", line);
    line_counter++;
  }
  fclose(settings_file);
  fclose(buffer_file);
  remove("tmp.txt");
}

void on_deshed(GtkWidget *button, GtkWidget *d_area) {
  int is_active = gtk_check_button_get_active(GTK_CHECK_BUTTON(button));
  char buffer[10];
  sprintf(buffer, "%d", is_active);
  change_line(5, buffer);
  gtk_widget_queue_draw(d_area);
}

void on_width_changed(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);

  // Gettings line width
  GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4, 13);
  char *tmp_str = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
  change_line(7, tmp_str);
  gtk_widget_queue_draw(d_area);
}

void on_edge_colour_changed(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);

  // Getting edge colour
  int colour[3];
  for (int i = 0; i < 3; i++) {
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4 + i, 12);
    char *tmp_str = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
    colour[i] = (int)strtol(tmp_str, NULL, 10);
  }
  char buffer[50];
  sprintf(buffer, "%d %d %d", colour[0], colour[1], colour[2]);
  change_line(6, buffer);
  gtk_widget_queue_draw(d_area);
}

void on_vrtx_size_changed(GtkWidget *button, GtkWidget *d_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);

  // Getting size
  GtkWidget *size_entry = gtk_grid_get_child_at(GTK_GRID(grid), 4, 10);
  char *tmp_str = (char *)gtk_editable_get_text(GTK_EDITABLE(size_entry));
  char buffer[20];
  sprintf(buffer, "%lf", strtod(tmp_str, NULL));
  change_line(4, buffer);
  gtk_widget_queue_draw(d_area);
}

// Is executed when window is being closed
void on_window_close(closure_struct *passed_data) {
  GtkWidget *grid = passed_data->grid;

  // Activating the button
  GtkWidget *button = gtk_grid_get_child_at(GTK_GRID(grid), 0, 1);
  gtk_widget_set_sensitive(button, TRUE);

  // Deleting transformation ui
  for (int i = 3; i < 8; i++) {
    for (int j = 0; j < 14; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), i, j);
      if (child)
        gtk_grid_remove(GTK_GRID(grid), child);
    }
  }
  for (int i = 2; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), j, i);
      if (child)
        gtk_grid_remove(GTK_GRID(grid), child);
    }
  }
  free_file_data(passed_data->data);
  free(passed_data->gif);
  free(passed_data);
  passed_data = NULL;
}

// Executed when background colour is being changed
void on_background_change(GtkWidget *button, GtkWidget *drawing_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);
  // Getting colours
  double vec[3];
  for (int i = 0; i < 3; i++) {
    GtkWidget *field = gtk_grid_get_child_at(GTK_GRID(grid), 4 + i, 6);
    char *tmp_str = (char *)gtk_editable_get_text(GTK_EDITABLE(field));
    vec[i] = strtod(tmp_str, NULL);
  }
  char buffer[100];
  sprintf(buffer, "background_colour %lf %lf %lf", vec[0], vec[1], vec[2]);
  change_line(1, buffer);
  gtk_widget_queue_draw(drawing_area);
}

void on_method_changed(GtkWidget *button, GtkWidget *drawing_area) {
  // Getting grid
  GtkWidget *grid = gtk_widget_get_parent(button);

  // Getting disp method
  GtkWidget *method_switcher = gtk_grid_get_child_at(GTK_GRID(grid), 4, 8);
  int selected_pos = gtk_drop_down_get_selected(GTK_DROP_DOWN(method_switcher));
  char *method;
  if (selected_pos == 0)
    method = "NONE";
  else if (selected_pos == 1)
    method = "CIRCLE";
  else
    method = "SQUARE";
  change_line(2, method);
  gtk_widget_queue_draw(drawing_area);
}

void on_colour_vertex_changed(GtkWidget *button, GtkWidget *drawing_area) {
  GtkWidget *grid = gtk_widget_get_parent(button);

  // Getting vertex colour
  int colour[3];
  for (int i = 0; i < 3; i++) {
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 4 + i, 9);
    char *tmp_str = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
    colour[i] = (int)strtol(tmp_str, NULL, 10);
  }
  char buffer[100];
  sprintf(buffer, "%d %d %d", colour[0], colour[1], colour[2]);
  change_line(3, buffer);
  gtk_widget_queue_draw(drawing_area);
}