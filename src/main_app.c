#include "data_handler.h"

GtkWidget *create_window(char *file_name) {
  GtkWidget *window = gtk_window_new();
  gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);
  gtk_window_set_title(GTK_WINDOW(window), file_name);
  return window;
}

closure_struct *init_tmp_struct(GtkWidget *window, file_data *data,
                                GtkWidget *grid) {
  closure_struct *data_to_pass =
      (closure_struct *)malloc(sizeof(closure_struct));
  data_to_pass->data = data;
  data_to_pass->grid = grid;
  data_to_pass->gif = (gif_struct *)malloc(sizeof(gif_struct));
  data_to_pass->gif->count = 0;
  g_signal_connect_swapped(window, "destroy", G_CALLBACK(on_window_close),
                           data_to_pass);
  return data_to_pass;
}

void init_transformations_ui(GtkWidget *transform_entries[9],
                             GtkWidget *column_labels[4],
                             GtkWidget *row_labels[3], GtkWidget *grid) {
  // Initializing column_labels
  column_labels[0] = gtk_label_new("Transformation\\Axes");
  gtk_widget_set_halign(column_labels[0], GTK_ALIGN_END);
  column_labels[1] = gtk_label_new("X");
  column_labels[2] = gtk_label_new("Y");
  column_labels[3] = gtk_label_new("Z");
  for (int i = 0; i < 4; i++)
    gtk_grid_attach(GTK_GRID(grid), column_labels[i], 3 + i, 0, 1, 1);

  // Initializing row labels
  row_labels[0] = gtk_label_new("Translation");
  row_labels[1] = gtk_label_new("Rotation");
  row_labels[2] = gtk_label_new("Scale");
  for (int i = 0; i < 3; i++) {
    gtk_grid_attach(GTK_GRID(grid), row_labels[i], 3, 1 + i, 1, 1);
    gtk_widget_set_halign(row_labels[i], GTK_ALIGN_END);
  }

  // Initializing entries
  int row = 1;
  int column = 4;
  for (int i = 0; i < 9; i++) {
    transform_entries[i] = gtk_entry_new();
    if (i < 6)
      gtk_editable_set_text(GTK_EDITABLE(transform_entries[i]), "0");
    else
      gtk_editable_set_text(GTK_EDITABLE(transform_entries[i]), "1");
    gtk_grid_attach(GTK_GRID(grid), transform_entries[i], column, row, 1, 1);
    column++;
    if (column == 7) {
      column = 4;
      row++;
    }
  }
}

GtkWidget *create_drawing_area(GtkWidget *window,
                               closure_struct *data_to_pass) {
  GtkWidget *d_area = gtk_drawing_area_new();
  gtk_window_set_child(GTK_WINDOW(window), d_area);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(d_area), draw_func,
                                 data_to_pass, NULL);
  return d_area;
}

void init_background_colour(GtkWidget *grid, GtkWidget *d_area,
                            settings *current_settings) {
  GtkWidget *background_colour = gtk_label_new("Background colour");
  gtk_grid_attach(GTK_GRID(grid), background_colour, 3, 6, 1, 1);
  GtkWidget *colour_entries[3];
  for (int i = 0; i < 3; i++) {
    colour_entries[i] = gtk_entry_new();
    char buffer[10];
    sprintf(buffer, "%d", current_settings->background_colour[i]);
    gtk_editable_set_text(GTK_EDITABLE(colour_entries[i]), buffer);
    gtk_grid_attach(GTK_GRID(grid), colour_entries[i], 4 + i, 6, 1, 1);
  }

  // Initializing background colour button
  GtkWidget *apply_colour_button =
      gtk_button_new_with_label("Change background colour");
  gtk_grid_attach(GTK_GRID(grid), apply_colour_button, 7, 6, 1, 1);
  g_signal_connect(apply_colour_button, "clicked",
                   G_CALLBACK(on_background_change), d_area);
}

void init_vrtx_disp_method(GtkWidget *grid, settings *current_settings,
                           GtkWidget *d_area) {
  GtkWidget *disp_label = gtk_label_new("Display method");
  gtk_grid_attach(GTK_GRID(grid), disp_label, 3, 8, 1, 1);
  GtkWidget *disp_switcher = gtk_drop_down_new_from_strings(
      (const char *[]){"None", "Circle", "Square", NULL});
  if (current_settings->vertex_method == NONE)
    gtk_drop_down_set_selected(GTK_DROP_DOWN(disp_switcher), 0);
  else if (current_settings->vertex_method == CIRCLE)
    gtk_drop_down_set_selected(GTK_DROP_DOWN(disp_switcher), 1);
  else
    gtk_drop_down_set_selected(GTK_DROP_DOWN(disp_switcher), 2);
  gtk_grid_attach(GTK_GRID(grid), disp_switcher, 4, 8, 1, 1);
  GtkWidget *apply_method = gtk_button_new_with_label("Apply");
  gtk_grid_attach(GTK_GRID(grid), apply_method, 5, 8, 1, 1);
  g_signal_connect(apply_method, "clicked", G_CALLBACK(on_method_changed),
                   d_area);
}

void init_vrtx_colour(GtkWidget *grid, settings *current_settings,
                      GtkWidget *d_area) {
  GtkWidget *vrtx_colour_label = gtk_label_new("Vertex colour");
  gtk_grid_attach(GTK_GRID(grid), vrtx_colour_label, 3, 9, 1, 1);
  GtkWidget *vrtx_colours[3];
  for (int i = 0; i < 3; i++) {
    vrtx_colours[i] = gtk_entry_new();
    char buffer[10];
    sprintf(buffer, "%d", current_settings->vrtx_colour[i]);
    gtk_editable_set_text(GTK_EDITABLE(vrtx_colours[i]), buffer);
    gtk_grid_attach(GTK_GRID(grid), vrtx_colours[i], 4 + i, 9, 1, 1);
  }
  GtkWidget *apply_colour_vrtx =
      gtk_button_new_with_label("Change vertices colour");
  g_signal_connect(apply_colour_vrtx, "clicked",
                   G_CALLBACK(on_colour_vertex_changed), d_area);
  gtk_grid_attach(GTK_GRID(grid), apply_colour_vrtx, 7, 9, 1, 1);
}

void init_vrtx_size(GtkWidget *grid, settings *current_settings,
                    GtkWidget *d_area) {
  GtkWidget *size_label = gtk_label_new("Vertex size");
  gtk_grid_attach(GTK_GRID(grid), size_label, 3, 10, 1, 1);
  GtkWidget *size_entry = gtk_entry_new();
  char buffer[10];
  sprintf(buffer, "%lf", current_settings->vrtx_size);
  gtk_editable_set_text(GTK_EDITABLE(size_entry), buffer);
  gtk_grid_attach(GTK_GRID(grid), size_entry, 4, 10, 1, 1);
  GtkWidget *apply_size = gtk_button_new_with_label("Apply");
  gtk_grid_attach(GTK_GRID(grid), apply_size, 5, 10, 1, 1);
  g_signal_connect(apply_size, "clicked", G_CALLBACK(on_vrtx_size_changed),
                   d_area);
}

void init_vertex_settings(GtkWidget *grid, GtkWidget *d_area,
                          settings *current_settings) {
  // Initializing vertices display method
  init_vrtx_disp_method(grid, current_settings, d_area);

  // Initializing vertex colour ui
  init_vrtx_colour(grid, current_settings, d_area);

  // Initializing vertex size
  init_vrtx_size(grid, current_settings, d_area);
}

void init_edge_settings(GtkWidget *grid, GtkWidget *d_area,
                        settings *current_settings) {
  // Init dash settings
  GtkWidget *dash_button = gtk_check_button_new_with_label("Dashed");
  gtk_grid_attach(GTK_GRID(grid), dash_button, 3, 11, 1, 1);
  if (current_settings->is_dash_active)
    gtk_check_button_set_active(GTK_CHECK_BUTTON(dash_button), 1);
  else
    gtk_check_button_set_active(GTK_CHECK_BUTTON(dash_button), 0);
  g_signal_connect(dash_button, "toggled", G_CALLBACK(on_deshed), d_area);

  // Init edge colour
  GtkWidget *colour_label = gtk_label_new("Change edge colour");
  gtk_grid_attach(GTK_GRID(grid), colour_label, 3, 12, 1, 1);
  GtkWidget *colour_entries[3];
  for (int i = 0; i < 3; i++) {
    colour_entries[i] = gtk_entry_new();
    char buffer[10];
    sprintf(buffer, "%d", current_settings->line_colour[i]);
    gtk_editable_set_text(GTK_EDITABLE(colour_entries[i]), buffer);
    gtk_grid_attach(GTK_GRID(grid), colour_entries[i], 4 + i, 12, 1, 1);
  }
  GtkWidget *apply_edge_colour = gtk_button_new_with_label("Apply");
  gtk_grid_attach(GTK_GRID(grid), apply_edge_colour, 7, 12, 1, 1);
  g_signal_connect(apply_edge_colour, "clicked",
                   G_CALLBACK(on_edge_colour_changed), d_area);

  // Init line width
  GtkWidget *line_thickness = gtk_label_new("Line thickness");
  gtk_grid_attach(GTK_GRID(grid), line_thickness, 3, 13, 1, 1);
  GtkWidget *thickness_entry = gtk_entry_new();
  char buffer[10];
  sprintf(buffer, "%lf", current_settings->line_width);
  gtk_editable_set_text(GTK_EDITABLE(thickness_entry), buffer);
  gtk_grid_attach(GTK_GRID(grid), thickness_entry, 4, 13, 1, 1);
  GtkWidget *apply_thickness = gtk_button_new_with_label("Apply");
  gtk_grid_attach(GTK_GRID(grid), apply_thickness, 5, 13, 1, 1);
  g_signal_connect(apply_thickness, "clicked", G_CALLBACK(on_width_changed),
                   d_area);
}

void init_settings_ui(GtkWidget *grid, GtkWidget *d_area) {
  // Creating settings ui
  GtkWidget *settings_label = gtk_label_new("Settings");
  gtk_grid_attach(GTK_GRID(grid), settings_label, 3, 5, 5, 1);

  // Getting current settings from file
  settings *current_settings = get_settings();

  // Initializing background colour

  init_background_colour(grid, d_area, current_settings);

  init_vertex_settings(grid, d_area, current_settings);

  init_edge_settings(grid, d_area, current_settings);

  free(current_settings);
}

// Opens another window (viewport)
void open_viewport(GtkWidget *open_button, GtkWidget *grid) {
  // Getting data from file
  GtkWidget *file_name_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
  char *file_name =
      (char *)gtk_editable_get_text(GTK_EDITABLE(file_name_entry));
  file_data *data = get_data_from_file(file_name);
  if (data) {
    // Deactivating button
    gtk_widget_set_sensitive(open_button, FALSE);

    init_file_info(grid, data);

    // Creating another window
    GtkWidget *window = create_window(file_name);

    // Creating struct to pass
    closure_struct *data_to_pass = init_tmp_struct(window, data, grid);

    // Initializing drawing area
    GtkWidget *d_area = create_drawing_area(window, data_to_pass);

    // Creating transformation ui
    GtkWidget *transform_entries[9];
    GtkWidget *column_labels[4];
    GtkWidget *row_labels[3];
    GtkWidget *apply_button;

    init_transformations_ui(transform_entries, column_labels, row_labels, grid);

    // Settings
    init_settings_ui(grid, d_area);

    // Initializing apply button
    apply_button = gtk_button_new_with_label("Apply");
    g_signal_connect_swapped(apply_button, "clicked",
                             (GCallback)gtk_widget_queue_draw, d_area);
    gtk_grid_attach(GTK_GRID(grid), apply_button, 3, 4, 4, 1);

    // Declaring ui for pictures
    GtkWidget *picture_label = gtk_label_new_with_mnemonic("Picture menu");
    gtk_grid_attach(GTK_GRID(grid), picture_label, 0, 2, 2, 1);

    GtkWidget *save_file_1 = gtk_button_new_with_label("Save file (.jpeg)");
    gtk_grid_attach(GTK_GRID(grid), save_file_1, 0, 3, 1, 1);
    GtkWidget *save_file_2 = gtk_button_new_with_label("Save file (.bmp)");
    gtk_grid_attach(GTK_GRID(grid), save_file_2, 1, 3, 1, 1);

    GtkWidget *record = gtk_button_new_with_label("Record");
    gtk_grid_attach(GTK_GRID(grid), record, 0, 4, 2, 1);

    // Buttons for pictures
    g_signal_connect(save_file_1, "clicked", G_CALLBACK(save_jpeg), d_area);

    g_signal_connect(save_file_2, "clicked", G_CALLBACK(save_bmp), d_area);

    g_signal_connect(record, "clicked", G_CALLBACK(record_gif), d_area);

    gtk_widget_show(window);
  }
}

void activate_app(GtkApplication *app) {
  // Variables
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *file_name_label;
  GtkWidget *file_name_entry;
  GtkWidget *open_button;

  // Creating main window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "3D_Viewer");
  gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);

  // Creating grid
  grid = gtk_grid_new();
  gtk_widget_set_halign(grid, GTK_ALIGN_START);
  gtk_widget_set_valign(grid, GTK_ALIGN_START);
  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

  // Creating ui for opening a model
  file_name_label = gtk_label_new("File name");
  gtk_grid_attach(GTK_GRID(grid), file_name_label, 0, 0, 1, 1);
  file_name_entry = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), file_name_entry, 1, 0, 1, 1);
  open_button = gtk_button_new_with_label("Open file");
  gtk_grid_attach(GTK_GRID(grid), open_button, 0, 1, 2, 1);
  g_signal_connect(open_button, "clicked", G_CALLBACK(open_viewport), grid);

  gtk_widget_show(window);
}

int main(int argc, char **argv) {
  // Variables
  GtkApplication *app;
  int status;

  // Running main application
  app = gtk_application_new("viewer.app", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate_app), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
