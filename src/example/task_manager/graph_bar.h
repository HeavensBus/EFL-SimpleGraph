/* Jin Sol Park
 * HeavensBus@gmail.com
 * 2014. 7. 2
 * graph_bar.h 
 * build gcc -o task_manager task_manager.c `pkg-config --cflags --libs elementary ecore`
 */ 

#include <stdio.h>
#include <string.h>
#include <Elementary.h>
#include <stdlib.h>

#include <time.h>

struct graph_bar
{
   int size_x;
   int size_y;
   int position_x;
   int position_y;
   int floor_count;
   int floor_strong;
   int floor_value;
   int gap_strong;

   Evas_Object **rect;
   Ecore_Timer *timer;

   Evas_Object *table;
   Evas_Object *layout;
   char *layout_part_name;

   //void *floor_value_get_cb;
   int (*floor_value_get_cb)(int per_total);

   struct graph_bar_color *bar_color;
};

struct graph_bar_color
{
   int floor_bright_r;
   int floor_bright_g;
   int floor_bright_b;
   int floor_bright_a;
   
   int floor_dark_r;
   int floor_dark_g;
   int floor_dark_b;
   int floor_dark_a;

   int gap_r;
   int gap_g;
   int gap_b;
   int gap_a;
};

static void graph_bar_color_init(struct graph_bar *bar)
{
   struct graph_bar_color *bar_color = bar->bar_color;
   bar_color->floor_bright_r = 0;
   bar_color->floor_bright_g = 221;
   bar_color->floor_bright_b = 0;
   bar_color->floor_bright_a = 255; 

   bar_color->floor_dark_r = 0; 
   bar_color->floor_dark_g = 69; 
   bar_color->floor_dark_b = 0; 
   bar_color->floor_dark_a = 255; 

   bar_color->gap_r = 0;
   bar_color->gap_g = 0;
   bar_color->gap_b = 0;
   bar_color->gap_a = 255;
}

void graph_bar_color_reset(struct graph_bar *bar, int r, int g, int b, int a)
{
   struct graph_bar_color *bar_color = bar->bar_color;
   bar_color->floor_bright_r = r;
   bar_color->floor_bright_g = g;
   bar_color->floor_bright_b = b;
   bar_color->floor_bright_a = a; 

   if (r > g && r > b)
   {
      bar_color->floor_dark_r = r - 180;
      bar_color->floor_dark_g = 0;
      bar_color->floor_dark_b = 0;
   }
   else if (g > r && g > b)
   {
      bar_color->floor_dark_g = g - 180;
      bar_color->floor_dark_r = 0;
      bar_color->floor_dark_b = 0;
   }
   else if (b > g && b >r)
   {
      bar_color->floor_dark_b = b - 180;
      bar_color->floor_dark_g = 0;
      bar_color->floor_dark_r = 0;
   }

   bar_color->floor_dark_a = a;

   graph_bar_draw(bar);
}



static Evas_Object **graph_bar_malloc(int floor_count)
{
   Evas_Object **rect;

   int i = 0;
   rect = (Evas_Object**)malloc(sizeof(Evas_Object*) * floor_count);

   for(i = 0; i < floor_count; i++)
   {
      rect[i] = (Evas_Object*)malloc(sizeof(Evas_Object*));
   }

   return rect;
}


static void graph_bar_table_pack(struct graph_bar *bar)
{
   struct graph_bar_color *bar_color = bar->bar_color;
   int tic = 0;

   int i = 0;
   for (i = 0; i < bar->floor_count; i++)
   {
      bar->rect[i] = evas_object_rectangle_add(bar->table);
      if (tic == 0)
      {
         evas_object_color_set(bar->rect[i], 0, 69, 0, 255);
         evas_object_size_hint_weight_set(bar->rect[i], 1.0, 0.0);
         evas_object_size_hint_align_set(bar->rect[i], -1.0, -1.0);
         evas_object_size_hint_min_set(bar->rect[i], -1, 2);
         evas_object_size_hint_max_set(bar->rect[i], -1, 2);
         elm_table_pack(bar->table, bar->rect[i], 0,0 + i,2, 1);
         tic = 1;
      }
      else{
         evas_object_color_set(bar->rect[i], 0, 0, 0, 255);
         evas_object_size_hint_weight_set(bar->rect[i], 1.0, 0.0);
         evas_object_size_hint_align_set(bar->rect[i], -1.0, -1.0);
         evas_object_size_hint_max_set(bar->rect[i], -1, 1);
         evas_object_size_hint_min_set(bar->rect[i], -1, 1);
         elm_table_pack(bar->table, bar->rect[i], 0,0 + i,2, 1);
         tic = 0;
      }
      evas_object_show(bar->rect[i]);
   }
}


static void graph_bar_table_create(struct graph_bar *bar)
{
   bar->table = elm_table_add(bar->layout);
   //elm_win_resize_object_add(win, table);
   evas_object_size_hint_weight_set(bar->table, 1.0, 1.0);
   evas_object_size_hint_align_set(bar->table, -1.0, -1.0);
   evas_object_show(bar->table);
   elm_table_homogeneous_set(bar->table, EINA_FALSE);
   elm_object_part_content_set(bar->layout, bar->layout_part_name, bar->table);
}

void graph_bar_draw(struct graph_bar *bar)
{
   struct graph_bar_color *bar_color = bar->bar_color;
   int tic = 0;

   int i = 0;
   int j = 0;

   if(bar->floor_value < 0)
   {
      printf("floor value is NULL!!\n"); 
   }


   printf("floor_value : %d, count : %d\n", bar->floor_value, bar->floor_count);



   //bar->floor_value = graph_bar_random_number(bar->floor_count/2);
   ///bar->floor_value = graph_bar_random_number(bar->floor_count/2);
   for (i = 0, j = 0; i < bar->floor_count; i++)
   {
      if (tic == 0)
      {
         if (j > (bar->floor_value / 2))
         {
            evas_object_color_set(bar->rect[i], bar_color->floor_bright_r, bar_color->floor_bright_g, bar_color->floor_bright_b, bar_color->floor_bright_a);
         }
         else
         {
            evas_object_color_set(bar->rect[i], bar_color->floor_dark_r, bar_color->floor_dark_g, bar_color->floor_dark_b, bar_color->floor_dark_a);
         }
         tic = 1;
         j++;
      }
      else{
         evas_object_color_set(bar->rect[i], 0, 0, 0, 255);
         tic = 0;
      }
      evas_object_show(bar->rect[i]);
   }
}

static Eina_Bool
_graph_bar_draw_cb(void *data)
{
   struct graph_bar *bar = (struct graph_bar*)data;
   if(bar  == NULL)
   {
      printf("bar is NULL!!\n");
      return EINA_FALSE;
   }

   if(bar->floor_value_get_cb == NULL)
   {
      printf("floor_value_get_cb is NULL!!\n");
      return EINA_FALSE;
   }
   bar->floor_value = bar->floor_value_get_cb(bar->floor_count);

   graph_bar_draw(bar);
}
void graph_bar_floor_value_get_cb_reset(struct graph_bar *bar, int (*floor_value_get_cb)(int per_total))
{
   bar->floor_value_get_cb = floor_value_get_cb;
}

void graph_bar_timer_reset(struct graph_bar *bar, double sec)
{
   if (bar == NULL)
   {
      printf("bar is NULL!!!\n");
      return;
   }

   if(bar->timer == NULL)
   {
      bar->timer = ecore_timer_add(sec, _graph_bar_draw_cb, bar);
   }
   else
   {
      ecore_timer_del(bar->timer);
      bar->timer = ecore_timer_add(sec, _graph_bar_draw_cb, bar);
   }
}

int graph_bar_random_number(int range)
{
   int temp = (unsigned int)rand() % range;
#ifdef DEBUG
   printf("rand = %d\n", temp);
#endif
   return temp;
}

void graph_bar_floor_count_reset(struct graph_bar *bar, int floor_count)
{
}



struct graph_bar* graph_bar_add(Evas_Object *layout, char *layout_part_name, int floor_count)
{
   if (layout == NULL)
   {
      printf("Evas_Object is NULL!!!\n");
      return;
   }
   if (floor_count < 3)
   {
      printf("bar count is error!!!\n");
      return;
   }

   //TODO : cpu data get after random number function delete
   srand(time(NULL));

   struct graph_bar *bar = (struct graph_bar*)malloc(sizeof(struct graph_bar));
   struct graph_bar_color *bar_color = (struct graph_bar_color*)malloc(sizeof(struct graph_bar_color));
   memset(bar, 0, sizeof(struct graph_bar));
   memset(bar_color, 0, sizeof(struct graph_bar_color));


   //TODO 
   bar->bar_color = bar_color;
   bar->rect = graph_bar_malloc(floor_count);
   bar->layout = layout;
   bar->table = elm_table_add(layout);
   bar->layout_part_name = layout_part_name;

// graph_bar_floor_count_reset(bar, floor_count);
   bar->floor_count = floor_count;
 
   graph_bar_color_init(bar);

   graph_bar_table_create(bar);
   graph_bar_table_pack(bar);

   return bar;
}


