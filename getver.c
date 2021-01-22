
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char
main                    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cmd       [200] = "";
   FILE       *f           = NULL;
   char        x_recd      [200] = "";
   char       *p           = NULL;
   char       *r           = NULL;
   /*---(extract version)----------------*/
   snprintf (x_cmd, 200, "grep \"P_VERNUM\" y*.h > version.txt");
   rc = system (x_cmd);
   --rce;  if (rc != 0)         return rce;
   /*---(read in)------------------------*/
   f = fopen ("version.txt", "rt");
   --rce;  if (f == NULL)       return rce;
   fgets (x_recd, 200, f);
   --rce;  if (strstr (x_recd, "P_VERNUM") == NULL)  return rce;
   fclose (f);
   /*---(parse)--------------------------*/
   p = strtok_r (x_recd, "\"", &r);
   --rce;  if (p == NULL)       return rce;
   p = strtok_r (NULL  , "\"", &r);
   --rce;  if (p == NULL)       return rce;
   --rce;  if (strlen (p) != 4) return rce;
   /*---(save back)----------------------*/
   printf ("%c %c %c\n", p [0], p [2], p [3]);
   /*---(complete)-----------------------*/
   return 0;
}


