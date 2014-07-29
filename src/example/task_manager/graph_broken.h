/* Jin Sol Park
 * HeavensBus@gmail.com
 * 2014. 7. 2
 * graph_broken.c
 * build gcc -o task_manager task_manager.c `pkg-config --cflags --libs elementary ecore`
 */ 

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <Elementary.h>

#include <stdlib.h>
#define EINA_LIST_FOREACH_FOR_GRAPH_BROKEN(list, l, data, count)\
	for (l = list,                  		  \
			count = 0,                        \
			data = eina_list_data_get(l);     \
			l;                                \
			l = eina_list_next(l),            \
			data = eina_list_data_get(l),     \
			count++)                          \

struct graph_broken_line
{
   int size_x;
   int size_y;
   int position_x;
   int position_y;
   int space_gap;
   int line_strong;
   int vertex_count;
   int first_value;
   int prev_data;

   Evas_Object *win;
   Evas_Object **polygon;
   Ecore_Timer *timer;

   Eina_List *list;
   int (*broken_value_get_cb)(int per_total);
};

struct graph_broken_vertex
{
   int data;
};

void polygon_line_add(Evas_Object *po, int strong, int x1, int y1, int x2, int y2);
int random_number();
int _push_graph(int data, int node_count, void*);
static Eina_Bool _call_graph_func(void *data);
void line_set_polygon(unsigned data, int count, unsigned prev_data, struct graph_broken_line *broken_line);

void polygon_line_add(Evas_Object *po, int strong, int x1, int y1, int x2, int y2) 
{
   evas_object_polygon_point_add(po, x1, y1 + strong);
   evas_object_polygon_point_add(po, x1, y1);
   evas_object_polygon_point_add(po, x2, y2);
   evas_object_polygon_point_add(po, x2, y2 + strong);
}

int random_number(int range)
{
   int temp = (unsigned int)rand() % range;
#ifdef DEBUG
   printf("rand = %d\n", temp);
#endif
   return temp;
}

int _push_graph(int data, int node_count, void *pointer)
{
   struct graph_broken_line *broken_line = (struct graph_broken_line*)pointer;
   evas_object_polygon_points_clear(broken_line->polygon[node_count]);
   line_set_polygon(data, node_count, broken_line->prev_data, broken_line);
   broken_line->prev_data = data;

   return 0;
}
static struct graph_broken_vertex* graph_broken_data_create(int data)
{
   struct graph_broken_vertex *num = (struct graph_broken_vertex*)malloc(sizeof(struct graph_broken_vertex));

   num->data = data;

   return num;
}

void graph_broken_value_get_cb_reset(struct graph_broken_line *broken_line, int (*broken_value_get_cb)(int per_total))
{
      broken_line->broken_value_get_cb = broken_value_get_cb;
}

static Eina_Bool
_call_graph_func(void *data)
{
   struct graph_broken_line *broken_line = (struct graph_broken_line*)data;

   if ( broken_line->broken_value_get_cb == NULL)
   {
      printf("broken_value_get_cb is RUN!!\n");
   }

   struct graph_broken_vertex *num = graph_broken_data_create(broken_line->broken_value_get_cb(100));
   //struct graph_broken_vertex *num = graph_broken_data_create(random_number(100));
   struct graph_broken_vertex *num_temp = NULL;

   //insert tail
   broken_line->list = eina_list_append(broken_line->list, num);

   //get head data
   num_temp = eina_list_nth(broken_line->list, 0);
   broken_line->prev_data = num_temp->data;

   //delete_head
   Eina_List *temp_list = NULL;
   num_temp = eina_list_nth(broken_line->list, 0);
   free(num_temp);
   temp_list = eina_list_nth_list(broken_line->list, 0);
   broken_line->list = eina_list_remove_list(broken_line->list, temp_list);
   
   //push graph
   int count = 0;
  EINA_LIST_FOREACH_FOR_GRAPH_BROKEN(broken_line->list, temp_list, num, count)
      _push_graph(num->data, count, broken_line);

   return EINA_TRUE;
}






void line_set_polygon(unsigned data, int count, unsigned prev_data, struct graph_broken_line *broken_line)
{

// Evas_Object *polygon[] = broken_line->polygon[count];

#ifdef DEBUG
   printf("data : %d, count : %d, prev_data: %d, VERTEX_SPACE_GAP*(count+2):%d, %d\n", data, count, prev_data, VERTEX_SPACE_GAP*(count+2), VERTEX_SPACE_GAP*(count+1));

#endif
   polygon_line_add(broken_line->polygon[count], broken_line->line_strong, broken_line->space_gap * (count + 2) + broken_line->position_x, data + broken_line->position_y, broken_line->space_gap * (count + 1) + broken_line->position_x, prev_data + broken_line->position_y);


   //evas_object_show(broken_line->polygon[count]);
}

void graph_broken_color_random(struct graph_broken_line *broken_line)
{
   int vertex_count = broken_line->vertex_count;
      
   int i = 0;
   for(i = 0; i < vertex_count; i++)
   {
      evas_object_color_set(broken_line->polygon[i],random_number(255),random_number(255),random_number(255),255);
   }

}

int graph_broken_color_reset(struct graph_broken_line *broken_line, int r, int g, int b, int a)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return -1;
   }

   int vertex_count = broken_line->vertex_count;
   int i = 0;
   for(i = 0; i < vertex_count; i++)
   {
      evas_object_color_set(broken_line->polygon[i],r,g,b,a);
   }

   return 0;
}

void graph_broken_position_reset(struct graph_broken_line *broken_line, int x, int y)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }

   if (broken_line->space_gap == 0)
   {
      printf("space_gap is 0, You need graph_broken_space_gap_reset() before call\n");
   }

   broken_line->position_x = (x - broken_line->space_gap);
   //broken_line->position_x = x;
   broken_line->position_y = (y );
   //broken_line->position_y = y;
}

void graph_broken_space_gap_reset(struct graph_broken_line *broken_line, int space_gap)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
   broken_line->space_gap = space_gap;
}

void graph_broken_strong_reset(struct graph_broken_line *broken_line, int line_strong)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
   broken_line->line_strong = line_strong;
}

/*
void graph_broken_vertex_count_reset(struct graph_broken_line *broken_line, int vertex_count)
{
   //TODO : vertex count change process

   _graph_broken_reset(broken_line, vertex_count);

}
*/

//static void 
//_graph_broken_reset(struct graph_borken_line *broken_line, int vertex_count_change)
void graph_broken_vertex_count_reset(struct graph_broken_line *broken_line, int vertex_count_change)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
   Evas_Object *win = broken_line->win;
   int vertex_count = broken_line->vertex_count;

   int i = 0;
   if (vertex_count == 0)
   {
      for (i = 0; i < vertex_count_change; i++)
      {
         broken_line->polygon[i] = evas_object_polygon_add(win);

         //struct graph_broken_vertex *num = graph_broken_data_create(100);
         broken_line->list = eina_list_append(broken_line->list, graph_broken_data_create(100));
         //broken_line->list = eina_list_append(broken_line->list, graph_broken_data_create(100));
         //list_insert_tail(100);
      }
         printf("polygon create, polygon count : %d\n", vertex_count_change);
   }
   else if (vertex_count_change > vertex_count)
   {
      for (i = vertex_count; i < vertex_count_change; i++)
      {
         broken_line->polygon[i] = evas_object_polygon_add(win);

         broken_line->list = eina_list_prepend(broken_line->list, graph_broken_data_create(100));
         //broken_line->list = eina_list_prepend(broken_line->list, graph_broken_data_create(100));
         //list_insert_head(100);

      }
         printf("polygon add, polygon count : %d\n", vertex_count_change);
   }
   else if (vertex_count_change < vertex_count)
   {
      for (i = vertex_count_change; i < vertex_count; i++)
      {
         evas_object_del(broken_line->polygon[i]);
         
         //list_delete_head();
         //delete_head
         Eina_List *temp_list = NULL;
         free(eina_list_nth(broken_line->list, 0));
         temp_list = eina_list_nth_list(broken_line->list, 0);
         broken_line->list = eina_list_remove_list(broken_line->list, temp_list);
      }
         printf("polygon del, polygon count : %d\n", vertex_count_change);
   }

   broken_line->vertex_count = vertex_count_change;
   for (i = 0; i < vertex_count_change; i++)
   {
      //list_insert_tail(broken_line->first_value);
   }

}

void graph_broken_timer_reset(struct graph_broken_line *broken_line, double sec)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
// broken_line->timer = ecore_timer_add(sec, _call_graph_func, broken_line);

   //TODO : timer reset : complete


   if(broken_line->timer == NULL)
   {
      broken_line->timer = ecore_timer_add(sec, _call_graph_func, broken_line);
   }
   else
   {
      ecore_timer_del(broken_line->timer);
      broken_line->timer = ecore_timer_add(sec, _call_graph_func, broken_line);
   }


}

void graph_broken_hide(struct graph_broken_line *broken_line)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
   int i = 0;

   for(i = 0; i < broken_line->vertex_count; i++)
   {
      evas_object_hide(broken_line->polygon[i]);
   }
}
void graph_broken_show(struct graph_broken_line *broken_line)
{
   if (broken_line == NULL)
   {
      printf("graph_broken_line is NULL!!!\n");
      return ;
   }
   int i = 0;

   for(i = 0; i < broken_line->vertex_count; i++)
   {
      evas_object_show(broken_line->polygon[i]);
   }
}

Evas_Object** graph_polygon_malloc(int vertex_count)
{

   Evas_Object **polygon;

   int i = 0;
   polygon = (Evas_Object**)malloc(sizeof(Evas_Object*) * vertex_count);

   for(i = 0; i < vertex_count; i++)
   {
      polygon[i] = (Evas_Object*)malloc(sizeof(Evas_Object*));
   }


   return polygon;
}

void graph_polygon_free(Evas_Object **polygon, int vertex_count)
{
   if (polygon == NULL)
   {
      printf("Polygon is NULL!!!\n");
   }

   int i = 0;
   for(i = 0; i < vertex_count; i++)
   {
      free(polygon[i]);
   }
   free(polygon);
} 

struct graph_broken_line* graph_broken_add(Evas_Object *win, int vertex_count)
{
   if (win == NULL)
   {
      printf("Evas_Object is NULL!!!\n");
      return;
   }
   if (vertex_count < 3)
   {
      printf("vertex count is error!!!\n");
      return;
   }

   //TODO : cpu data get after random number function delete
   srand(time(NULL));

   struct graph_broken_line *broken_line = (struct graph_broken_line*)malloc(sizeof(struct graph_broken_line));
   memset(broken_line, 0, sizeof(struct graph_broken_line));
   broken_line->polygon = graph_polygon_malloc(vertex_count);

   broken_line->win = win;

   eina_init();

   graph_broken_vertex_count_reset(broken_line, vertex_count);

   return broken_line;
}

