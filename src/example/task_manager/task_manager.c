/* Jin Sol Park
 * HeavensBus@gmail.com
 * 2014. 7. 2
 * task manager.c
 * build gcc -o task_manager task_manager.c `pkg-config --cflags --libs elementary ecore`
 */ 

#include<Elementary.h>
#include"system_info.h"
#include"graph_broken.h"
#include"graph_bar.h"

struct graph_broken_line *broken_line_cpu;
struct graph_broken_line *broken_line_mem;
struct graph_bar *bar_cpu;
struct graph_bar *bar_mem;

Evas_Object *win;

Evas_Object *layout;
Evas_Object *layout_graph;
Evas_Object *layout_list;

void layout_init_default();
void layout_set_list();
void layout_set_graph();

//void toolbar_init(Evas_Object *box);
//void toolbar_create(Evas_Object *box);

void _toolbar_list_cb(void *data, Evas_Object *obj, void *event_info);
void _toolbar_graph_cb(void *data, Evas_Object *obj, void *event_info);


void color_select_win(void *);
void control_box_create();
Evas_Object* box_create(Evas_Object *win);
//graph broken cpu function
void graph_broken_cpu_init();
void graph_broken_mem_init();

void graph_bar_cpu_init();
void graph_bar_mem_init();

//Genernel graph function
void graph_show();
void graph_hide();

char *edje_path = "./graph.edj";

void layout_init(Evas_Object *layout, Evas_Object *win, char *path, char *group)
{

/*
   layout = elm_layout_add(win);

   elm_layout_file_set(layout, path, group);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, layout);

   evas_object_show(layout);

   */
}

void layout_init_default()
{

   layout = elm_layout_add(win);

   elm_layout_file_set(layout, edje_path, "layout");
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, layout);

   evas_object_show(layout);
}

static Eina_Bool
_timer_reset_cb(void *data, Evas_Object *obj, void *event_info)
{
   double value = elm_slider_value_get(obj);
   printf("slider value %f\n", value);

   graph_bar_timer_reset(bar_cpu, value);
   graph_bar_timer_reset(bar_mem, value);
   graph_broken_timer_reset(broken_line_cpu, value);
   graph_broken_timer_reset(broken_line_mem, value);
}


static void
_change_color(void *data, Evas_Object *obj, void *event_info)
{
   if(obj == NULL || data == NULL)
      return;

   int r, g, b, a;
   elm_colorselector_color_get(obj, &r, &g, &b, &a);

   if(data == broken_line_cpu || data == broken_line_mem)
      graph_broken_color_reset(data, r, g, b, a);
   if(data == bar_cpu || data == bar_mem)
      graph_bar_color_reset(data, r,g,b,a);

}



static void
_button_graph_bar_color_reset_cb(void *data, Evas_Object *obj, void *event_info)
{
   Evas_Object *temp = (Evas_Object*)data;
   if(obj == NULL)
      return;
// int r, g, b, a;
// elm_colorselector_color_get(obj, &r, &g, &b, &a);
// evas_object_color_set(temp, r, g, b, a);
// printf("bar_color button click \n");
   color_select_win(data);
}

void
control_box_create()
{
   Evas_Object *box = box_create(layout_graph);
   elm_object_part_content_set(layout_graph, "graph_control_box", box);

   Evas_Object *sl = elm_slider_add(box);
   elm_slider_unit_format_set(sl, "%1.3f sec");
   elm_slider_min_max_set(sl, 0, 3.0);
   elm_slider_value_set(sl, 1.0);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, 0.5);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(sl);
   elm_box_pack_end(box, sl);
   evas_object_smart_callback_add(sl, "delay,changed", _timer_reset_cb, NULL);

   Evas_Object *table = elm_table_add(box);

   evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);

   Evas_Object *button ;

   //button
   button = elm_button_add(table);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(button, "Bar Cpu");
   evas_object_show(button);
   evas_object_smart_callback_add(button, "clicked", _button_graph_bar_color_reset_cb, bar_cpu);

   elm_table_homogeneous_set(table, EINA_TRUE);

   elm_table_pack(table, button, 0, 0, 1, 1);

   button = elm_button_add(table);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_text_set(button, "Bar Mem");
   evas_object_show(button);
   evas_object_smart_callback_add(button, "clicked", _button_graph_bar_color_reset_cb, bar_mem);

   elm_table_pack(table, button, 1, 0, 1, 1);

   button = elm_button_add(table);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_text_set(button, "Broken Line Cpu");
   evas_object_show(button);
   evas_object_smart_callback_add(button, "clicked", _button_graph_bar_color_reset_cb, broken_line_cpu);

   elm_table_pack(table, button, 2, 0, 1, 1);

   button = elm_button_add(table);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_text_set(button, "Broken Line Mem");
   evas_object_smart_callback_add(button, "clicked", _button_graph_bar_color_reset_cb, broken_line_mem);
   evas_object_show(button);

   elm_table_pack(table, button, 3, 0, 1, 1);
   evas_object_show(table);
   elm_box_pack_end(box, table);
}


void
color_select_win(void *data)
{
   Evas_Object *color_win, *fr, *cs;
   Evas_Object *box;
   color_win = elm_win_util_standard_add("Color Selector", "JS Color Selector");

   elm_win_autodel_set(color_win, EINA_TRUE);
   box = elm_box_add(color_win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(color_win, box);
   evas_object_show(box);

   fr = elm_frame_add(color_win);
   evas_object_size_hint_weight_set(fr, 1.0, 0.5);
   evas_object_size_hint_align_set(fr, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(fr, "Color Selector");
   elm_box_pack_end(box, fr);
   evas_object_show(fr);
   cs = elm_colorselector_add(color_win);

   elm_colorselector_mode_set(cs, ELM_COLORSELECTOR_ALL);

   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, 1.0);
   evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, -10.0);
   elm_colorselector_color_set(cs, 0, 255, 18, 255);
   evas_object_show(cs);

   evas_object_smart_callback_add(cs, "changed", _change_color, data);

   elm_object_content_set(fr, cs);
   evas_object_resize(color_win, 330, 480);
   evas_object_show(color_win);
}

void
layout_set_graph()
{
   if(layout_graph == NULL)
   {

      layout_graph = elm_layout_add(layout);
      elm_layout_file_set(layout_graph, edje_path, "graph");
      evas_object_size_hint_weight_set(layout_graph, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);


   //elm_object_part_content_set(layout, "part_graph_bar_cpu_spacer", layout_graph);

// evas_object_del(layout_list);

   }

   elm_object_part_content_unset(layout, "part_layout");
   evas_object_hide(layout_list);
   elm_object_part_content_set(layout, "part_layout", layout_graph);
   evas_object_show(layout_graph);

   if(broken_line_cpu == NULL)
      graph_broken_cpu_init();
   if(bar_cpu == NULL)
      graph_bar_cpu_init();
   if(broken_line_mem == NULL)
      graph_broken_mem_init();
   if(bar_mem == NULL)
      graph_bar_mem_init();

   control_box_create();
   graph_show();
}

void
layout_set_list()
{
   if(layout_list == NULL) {
      //evas_object_grid_size_get

      layout_list = elm_layout_add(layout);
      elm_layout_file_set(layout_list, edje_path, "list");
      evas_object_size_hint_weight_set(layout_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

      //evas_object_del(layout_graph);
      //evas_object_hide(layout_graph);

   }

   elm_object_part_content_unset(layout, "part_layout");
   evas_object_hide(layout_graph);
   elm_object_part_content_set(layout, "part_layout", layout_list);

   evas_object_show(layout_list);

   graph_hide();
}

void
graph_hide()
{
   if(broken_line_cpu != NULL)
      graph_broken_hide(broken_line_cpu);
   if(broken_line_mem != NULL)
      graph_broken_hide(broken_line_mem);
}

void
graph_show()
{
   if(broken_line_cpu != NULL)
      graph_broken_show(broken_line_cpu);
   if(broken_line_mem != NULL)
      graph_broken_show(broken_line_mem);
}

Evas_Object*
box_create(Evas_Object *obj)
{
	Evas_Object *box;
	box = elm_box_add(obj);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(box);

	return box;
}

	void
list_init(Evas_Object *box)
{


}


void panes_cb(void *data, Evas_Object *obj, void *event_info)
{
	printf("click");
}


Evas_Object *panes_init(Evas_Object *obj)
{
	Evas_Object *panes, *panes_h, *bt;

	panes = elm_panes_add(obj);
	evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
//	elm_obj_resize_object_add(obj, panes);
	evas_object_show(panes);

	bt = elm_button_add(obj);
	elm_object_text_set(bt, "Left");
	evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(bt);
	elm_object_part_content_set(panes, "left", bt);
/*
	panes_h = elm_panes_add(obj);
	elm_panes_horizontal_set(panes_h, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND,
			EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(panes_h);
	elm_object_part_content_set(panes, "right", panes_h);

	bt = elm_button_add(obj);
	elm_object_text_set(bt, "Up");
	evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(bt);
	elm_object_part_content_set(panes_h, "left", bt);
	*/
/*
	bt = elm_button_add(obj);
	elm_object_text_set(bt, "Down");
	evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(bt);
	elm_object_part_content_set(panes_h, "right", bt);
	*/

	evas_object_smart_callback_add(panes, "clicked",panes_cb, panes);
	evas_object_smart_callback_add(panes, "clicked,double", panes_cb,
			panes);
	evas_object_smart_callback_add(panes, "press", panes_cb, panes);
	evas_object_smart_callback_add(panes, "unpress", panes_cb, panes);
	
	return panes;

}



	Elm_Genlist_Item_Class *itc;

char* process_list_data_get_cb(void *data, Evas_Object *obj EINA_UNUSED, const char *part EINA_UNUSED)
{

	return "This is TESET!!!!!!!!!!!!!!!!!!!!!!!";
}

Evas_Object*
process_list_init(Evas_Object *obj)
{
	Evas_Object *gl;



	gl = elm_genlist_add(obj);
	evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(gl);

	itc = elm_genlist_item_class_new();
	itc->item_style = "default";
	itc->func.text_get = process_list_data_get_cb;

	elm_genlist_item_append(gl, itc, (void *)1, NULL, ELM_GENLIST_ITEM_TREE, NULL, NULL);



	printf("sjdkflsjdklfjalsfkdjaksdlfjksadlfjalfjaflsadfjlasdjflasdfjlfd");



	return gl;


}

 void
_toolbar_list_cb(void *data, Evas_Object *obj, void *event_info)
{

   printf("toolbar one \n");
   layout_set_list();
   Evas_Object *box;
   Evas_Object *process_list;
   box = box_create(win);
	 /*
   // temp rect
   Evas_Object *rect;
   rect = evas_object_rectangle_add(box);
   evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(rect, -1.0, -1.0);
   evas_object_color_set(rect, 5,52,151,255);
   //evas_object_color_set(rect, 255,255,255,255);
   evas_object_show(rect);
   elm_box_pack_end(box, rect);
	 */

//	 process_list = process_list_init(box);
//	 elm_box_pack_end(box, process_list);
	 //elm_box_pack_end(box, panes_init(box));

   elm_object_part_content_set(layout_list, "list_test", box);
   printf("call list cb \n");
}

Evas_Object *rect;
void
_toolbar_graph_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("toolbar two\n");

   layout_set_graph();


   Evas_Object *box;
   box = box_create(win);
   // temp rect
   rect = evas_object_rectangle_add(box);
   evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(rect, -1.0, -1.0);
   //evas_object_color_set(rect, 100,100,200,255);
   evas_object_color_set(rect, 0,0,0,255);
   evas_object_show(rect);
   elm_box_pack_end(box, rect);

   elm_object_part_content_set(layout_graph, "graph_broken_cpu", box);
}


Evas_Object*
toolbar_create(Evas_Object *box)
{

   Evas_Object *toolbar;

   toolbar = elm_toolbar_add(box);

   evas_object_size_hint_weight_set(toolbar,EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(toolbar);

   elm_toolbar_item_append(toolbar, "one", "            Process List            ", _toolbar_list_cb, NULL);
   elm_toolbar_item_append(toolbar, "two", "            Performance             ", _toolbar_graph_cb, NULL);

   return toolbar;
}

void graph_bar_cpu_init()
{
   bar_cpu = graph_bar_add(layout_graph, "graph_bar_cpu", 67);
 graph_bar_floor_value_get_cb_reset(bar_cpu, cpu_used_per_get);
 graph_bar_timer_reset(bar_cpu, 1.0);
}
void graph_bar_mem_init()
{
   bar_mem = graph_bar_add(layout_graph, "graph_bar_mem", 67);
   graph_bar_floor_value_get_cb_reset(bar_mem, memory_used_per_get);
   graph_bar_timer_reset(bar_mem, 1.0);
}



int color_set = 0;
void graph_broken_cpu_init()
{
   broken_line_cpu = graph_broken_add(win, 10);

   int x,y,w,h;

   edje_object_part_geometry_get(elm_layout_edje_get(layout_graph), "graph_broken_cpu", &x, &y, &w, &h);

   printf("x : %d, y: %d, w: %d, h: %d\n", x,y,w,h);

   graph_broken_space_gap_reset(broken_line_cpu, 30);

   graph_broken_position_reset(broken_line_cpu, x, y * 2);
   graph_broken_strong_reset(broken_line_cpu, 2);
   graph_broken_value_get_cb_reset(broken_line_cpu, cpu_used_per_get);

   graph_broken_timer_reset(broken_line_cpu, 1.0);

   if(color_set == 1)
      graph_broken_color_random(broken_line_cpu);
   else
      graph_broken_color_reset(broken_line_cpu, 255, 255, 0, 255);

   graph_broken_show(broken_line_cpu);
}

void graph_broken_mem_init()
{
   broken_line_mem = graph_broken_add(win, 10);

   int x,y,w,h;

   edje_object_part_geometry_get(elm_layout_edje_get(layout_graph), "graph_broken_mem", &x, &y, &w, &h);

   printf("broken graph mem - x : %d, y: %d, w: %d, h: %d\n", x,y,w,h);

   graph_broken_space_gap_reset(broken_line_mem, 30);

   graph_broken_position_reset(broken_line_mem, x, y + 50);
   graph_broken_strong_reset(broken_line_mem, 2);
   graph_broken_value_get_cb_reset(broken_line_mem, memory_used_per_get);

   graph_broken_timer_reset(broken_line_mem, 1.0);

   if(color_set == 1)
      graph_broken_color_random(broken_line_mem);
   else
      graph_broken_color_reset(broken_line_mem, 255, 255, 0, 255);

   graph_broken_show(broken_line_mem);
}




int main(int argc, char **argv)
{
   elm_init(argc, argv);
   Evas_Object *bg;

   win = elm_win_util_standard_add("js", "layout_test");

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);
   evas_object_resize(win, 450, 500);

   bg =  elm_bg_add(win);

   elm_win_resize_object_add(win, bg);
   //evas_object_color_set(bg, 0, 240, 240, 255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bg, -1.0, -1.0);
// evas_object_color_set(bg, 0, 0, 255, 255);
   elm_bg_color_set(bg, 50,50,50);
   evas_object_show(bg);
   evas_object_show(win);

   if (argc > 1) {
      color_set = atoi(argv[1]);
      if(argc == 3)
         edje_path = argv[2];
   }

   layout_init_default();
   Evas_Object *box_toolbar;
   Evas_Object *toolbar;
   box_toolbar = box_create(win);
   toolbar = toolbar_create(box_toolbar);
   elm_box_pack_end(box_toolbar, toolbar);
   elm_object_part_content_set(layout, "part_toolbar", box_toolbar);

   //temp set
   _toolbar_graph_cb(NULL,NULL,NULL);


   elm_run();

   elm_shutdown();

   return 0;
}
