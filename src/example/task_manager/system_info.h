/* Jin Sol Park
 * HeavensBus@gmail.com
 * 2014. 7. 2
 * system_info.c 
 * build gcc -o task_manager task_manager.c `pkg-config --cflags --libs elementary ecore`
 */ 

#include <stdio.h>
#include <string.h>

/*
   pid - 1
   user - 2
   cpu - 9
   mem - 10
   command - 12
   char *pid;
   char *user;
   char *cpu;
   char *mem;
   char *command;

   
 */
int cpu_used_per_get(int per_total)
{

   FILE *fp;
   char buff[2048];
   int toogle = 1;

//   double out;
   int out;
   char temp[10][256];

//   printf("__LINE__ : %d, __func__: %s \n", __LINE__, __func__);
   fp = popen("top -n 2 -d 0.01 | grep Cpu", "r");
   if (fp == NULL)
   {
      perror("erro : ");
      return 0;
   }
   while(fgets(buff, 2048, fp) != NULL)
   {
      sscanf(buff, "%s %s %s %s %s %s %s %s", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
      if(toogle == 0)
      {
         if(strstr(temp[7], "id"))
         {
          //  return 0;

            out = 100;

         }
         else
         {
           // return (atoi(temp[7]) - 100)/2;
           out = (atoi(temp[7]));
         }
      }
      else
         toogle = 0;
   } 


   int out_value = out;
   //printf("cpu idle : %d, per_total %d, per_total/100 %f,setnum : %d\n", out_value, per_total, (double)per_total/(double)100, (int)((double)out_value * (double)((double)per_total / (double)100)));
   pclose(fp);
   return (int)((double)out_value * (double)((double)per_total / (double)100));
}



int memory_used_per_get(int per_total)
{
   FILE *fp;
   char buff[1024];
   char mem_s[256];
   char total_s[256];
   char used_s[256];
   char free_s[256];
   char shared_s[256];
   char buffers_s[256];
   char cached_s[256];
   int total;
   int shared;
   int free;
   int cached;
   int used;
   int used_real;


   //printf("__LINE__ : %d, __func__: %s, pertoal : %d \n", __LINE__, __func__, per_total);
   fp = popen("free | grep Mem:", "r");
   if (fp == NULL)
   {
      perror("erro : ");
      return 0;
   }
   while(fgets(buff, 1024, fp) != NULL);

   sscanf(buff, "%s %s %s %s %s %s %s", mem_s, total_s, used_s, free_s, shared_s, buffers_s, cached_s);
   total = atoi(total_s);
   shared = atoi(shared_s);
   cached = atoi(cached_s);
   used = atoi(used_s);
   used_real = used - shared - cached;
   used_real = total - used_real;


   int change_per = used_real / (total / per_total);
   printf("real used : %d\n", used_real);
   printf("real used per : %d\n", change_per);
   pclose(fp);

   return change_per;
}


