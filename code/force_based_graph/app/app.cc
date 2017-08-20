#include <cairo.h>
#include <gtk/gtk.h>
#include <iostream>
#include "Graph.h"
#include "ForceBasedSimulator.h"

const float PI = 3.1415927;


static gboolean
on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{
  ForceBasedSimulator *simulator = 
    reinterpret_cast<ForceBasedSimulator *>(user_data);
  Graph *g = simulator->getGraph();

  for (int i = 0; i < g->size(); i++)
  {
    cairo_set_source_rgb(cr, 255, 0, 0);
    Vertex &v = g->node(i);
    cairo_arc(cr, v.posX, v.posY, 5, 0, 2*PI);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, 255, 0, 0, 0.4);
    std::vector<int> &adjacency = g->adjacency(i);
    for (int j : adjacency)
    {
      Vertex &u = g->node(j);
      cairo_move_to(cr, v.posX, v.posY);
      cairo_line_to(cr, u.posX, u.posY);
      cairo_stroke(cr);
    }
  }

  simulator->update(gtk_widget_get_allocated_width(widget),
                    gtk_widget_get_allocated_height(widget));
  gtk_widget_queue_draw(widget);
  return FALSE;
}


int
main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  Graph graph;
  ForceBasedSimulator simulator(&graph, 0.8f, 0.08f, 0.9f);

  graph.addNode(); // 0
  graph.addNode(); // 1
  graph.addNode(); // 2
  graph.addNode(); // 3
  graph.addNode(); // 4
  graph.addNode(); // 5
  graph.addNode(); // 6
  graph.addNode(); // 7
  graph.addNode(); // 8
  graph.addNode(); // 9
  graph.addNode(); // 10
  graph.addNode(); // 11
  graph.addNode(); // 12
  graph.addNode(); // 13
  graph.addNode(); // 14

  graph.addEdge(0, 1);
  graph.addEdge(0, 2);
  graph.addEdge(0, 3);
  graph.addEdge(0, 4);
  graph.addEdge(0, 5);
  graph.addEdge(5, 6);
  graph.addEdge(5, 7);
  graph.addEdge(5, 8);
  graph.addEdge(0, 8);
  graph.addEdge(8, 5);
  graph.addEdge(8, 3);
  graph.addEdge(3, 7);
  graph.addEdge(0, 9);
  graph.addEdge(9, 5);
  graph.addEdge(9, 3);
  graph.addEdge(2, 9);
  graph.addEdge(5, 10);
  graph.addEdge(5, 11);
  graph.addEdge(0, 12);
  graph.addEdge(8, 13);
  graph.addEdge(2, 14);

  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), &simulator); 
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 1000, 680); 
  gtk_window_set_title(GTK_WINDOW(window), "what the heeeeeeck?");
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
