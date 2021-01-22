/*===[[ START ]]==============================================================*/
#include  "_lib.h"

char
error_fatal             (cchar *a_name, cchar *a_message)
{
   printf("FATAL  : %.50s\n", a_message);
   printf("object : %2d[%.50s]\n", strlen (a_name), a_name);
   printf("usage  :\n");
   printf("\n");
   printf("   _lib -l         list all installed libraries (.so)\n");
   printf("   _lib -v         list all installed libraries (.*)\n");
   printf("\n");
   printf("   _lib -d <name>  delete a archive/shared library and header\n");
   printf("   _lib -D <name>  delete a archive/shared library only\n");
   printf("\n");
   printf("   _lib -h <name>  install just the header\n");
   printf("   _lib -H <name>  install just the header\n");
   printf("   _lib -a <name>  install static library and header\n");
   printf("   _lib -A <name>  install static library only\n");
   printf("   _lib -s <name>  install shared library and header\n");
   printf("   _lib -S <name>  install shared library only\n");
   printf("   _lib -i <name>  install static/shared library and header\n");
   printf("\n");
   exit(-1);
}

char
check_file              (cchar *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =  '-';
   tSTAT       r;
   /*---(defense)------------------------*/
   --rce;  if (a_file == NULL)  return rce;
   /*---(check existance)----------------*/
   rc = lstat (a_file, &r);
   --rce;  if (rc < 0)   return rce;
   /*---(check type)---------------------*/
   --rce; if (S_ISLNK (r.st_mode))     rc = 'l';
   else if   (S_ISREG (r.st_mode))     rc = 'r';
   else                                return rce;
   /*---(complete)-----------------------*/
   return rc;
}

char
get_version             (char *a_name, char *a_vernum)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_name      [LEN_FULL] = "";
   char        x_cmd       [LEN_FULL] = "";
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_FULL] = "";
   char       *p           = NULL;
   char       *r           = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL)  return rce;
   /*---(extract version)----------------*/
   strcpy (x_name, a_name);
   p = strstr (x_name, "_debug");
   if (p != NULL)  p [0] = '\0';
   /*> printf ("%-20.20s  %-10p  %s\n", a_name, p, x_name);                           <*/
   snprintf (x_cmd, LEN_FULL, "grep \"P_VERNUM\" %s*.h > version.txt", x_name);
   rc = system (x_cmd);
   --rce;  if (rc != 0)         return rce;
   /*---(read in)------------------------*/
   f = fopen ("version.txt", "rt");
   --rce;  if (f == NULL)       return rce;
   fgets (x_recd, LEN_FULL, f);
   --rce;  if (strstr (x_recd, "P_VERNUM") == NULL)  return rce;
   fclose (f);
   /*---(parse)--------------------------*/
   p = strtok_r (x_recd, "\"", &r);
   --rce;  if (p == NULL)       return rce;
   p = strtok_r (NULL  , "\"", &r);
   --rce;  if (p == NULL)       return rce;
   --rce;  if (strlen (p) != 4) return rce;
   /*---(save back)----------------------*/
   sprintf (a_vernum, "%c %c %c", p [0], p [2], p [3]);
   /*---(complete)-----------------------*/
   return 0;
}

char
header_install          (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_cmd       [LEN_FULL] = "";
   char        x_name      [LEN_FULL] = "";
   struct stat s;
   /*---(try regular header)-------------*/
   snprintf (x_name, LEN_FULL, "%s.h", a_name);
   rc = stat (x_name, &s);
   /*---(install)------------------------*/
   if (rc == 0) {
      /*---(copy)------------------------*/
      snprintf (x_cmd, LEN_FULL, "cp -f %.40s %s &> /dev/null", x_name, HEADERS);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not copy to shared folder");
      /*---(fix permissions)-------------*/
      snprintf (x_cmd, LEN_FULL, "chmod 0755 %s/%.40s &> /dev/null", HEADERS, x_name);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not change permissions");
   }
   /*---(try solo header)----------------*/
   snprintf (x_name, LEN_FULL, "%s_solo.h", a_name);
   rc = stat (x_name, &s);
   /*---(install)------------------------*/
   if (rc == 0) {
      /*---(copy)------------------------*/
      snprintf (x_cmd, LEN_FULL, "cp -f %.40s %s &> /dev/null", x_name, HEADERS);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not copy to shared folder");
      /*---(fix permissions)-------------*/
      snprintf (x_cmd, LEN_FULL, "chmod 0755 %s/%.40s &> /dev/null", HEADERS, x_name);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not change permissions");
   }
   /*---(try unit header)----------------*/
   snprintf (x_name, LEN_FULL, "%s_unit.h", a_name);
   rc = stat (x_name, &s);
   /*---(install)------------------------*/
   if (rc == 0) {
      /*---(copy)------------------------*/
      snprintf (x_cmd, LEN_FULL, "cp -f %.40s %s &> /dev/null", x_name, HEADERS);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not copy to shared folder");
      /*---(fix permissions)-------------*/
      snprintf (x_cmd, LEN_FULL, "chmod 0755 %s/%.40s &> /dev/null", HEADERS, x_name);
      rc = system (x_cmd);
      if (rc != 0)  error_fatal (x_name, "can not change permissions");
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
static_install(char *a_name)
{
   int   x_rc        = 0;
   char  x_cmd[LEN_FULL]  = "";
   char  x_name[LEN_FULL] = "";
   snprintf(x_name, LEN_FULL, "%s.a", a_name);
   /*---(check for existance)--------------*/
   snprintf(x_cmd, LEN_FULL, "ls lib%.40s &> /dev/null", x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "no existing local archive");
   /*---(install the library)--------------*/
   snprintf(x_cmd, LEN_FULL, "cp -f lib%.40s %s &> /dev/null", x_name, LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not copy to shared folder");
   /*---(fix permissions)------------------*/
   snprintf(x_cmd, LEN_FULL, "chmod 0755 %s/lib%.40s &> /dev/null", LIBRARIES, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not change permissions");
   /*---(update cache)---------------------*/
   snprintf(x_cmd, LEN_FULL, "/sbin/ldconfig -n %s &> /dev/null", LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not update cache");
   /*---(complete)-------------------------*/
   return 0;
}

char
shared_install          (cchar *a_name, cchar *a_vernum)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_base      [LEN_FULL]   = "";
   char        x_full      [LEN_FULL]   = "";
   char        x_soname    [LEN_FULL]   = "";
   char        x_generic   [LEN_FULL]   = "";
   char        x_cmd       [LEN_FULL]   = "";
   int         i           =    0;
   /*---(defense)------------------------*/
   if (a_name == NULL)         error_fatal ("NULL", "library main name is null");
   if (strlen (a_name) < 3)    error_fatal (a_name, "library main name is too short");
   /*---(make base name)-------------------*/
   snprintf (x_base   , LEN_FULL, "%s.so", a_name);
   snprintf (x_full   , LEN_FULL, "lib%.40s.%c.%c.%c", x_base, a_vernum [0], a_vernum [2], a_vernum [4]);
   snprintf (x_soname , LEN_FULL, "lib%.40s.%c", x_base, a_vernum [0]);
   snprintf (x_generic, LEN_FULL, "lib%.40s", x_base);
   /*---(check for local existance)--------*/
   rc = check_file  (x_full);
   if (rc != 'r')              error_fatal (x_full, "no existing local lib");
   /*---(unlink old libraries)-------------*/
   snprintf (x_cmd , LEN_FULL, "rm -f %s/%s   &> /dev/null", LIBRARIES, x_generic);
   rc = system(x_cmd);
   if (rc != 0)                error_fatal (x_base, "can not unlink lib from generic");
   for (i = 0; i <= 9; ++i) {
      snprintf (x_cmd , LEN_FULL, "rm -f %s/%s%d  &> /dev/null", LIBRARIES, x_generic, i);
      rc = system(x_cmd);
   }
   snprintf (x_cmd , LEN_FULL, "rm -f %s/%s*  &> /dev/null", LIBRARIES, x_generic);
   rc = system(x_cmd);
   if (rc != 0)                error_fatal (x_base, "can not unlink lib from generic");
   /*---(install new library)--------------*/
   snprintf (x_cmd , LEN_FULL, "cp -f %s %s &> /dev/null", x_full, LIBRARIES);
   rc = system (x_cmd);
   if (rc != 0)                error_fatal (x_full, "can not copy to shared folder");
   /*---(fix permissions)------------------*/
   snprintf (x_cmd , LEN_FULL, "chmod 0755 %s/%s   &> /dev/null", LIBRARIES, x_full);
   rc = system (x_cmd);
   if (rc != 0)                error_fatal (x_full, "can not change permissions");
   /*---(link to major)--------------------*/
   snprintf (x_cmd , LEN_FULL, "ln -sf %s/%s %s/%s &> /dev/null", LIBRARIES, x_full, LIBRARIES, x_soname);
   rc = system(x_cmd);
   if (rc != 0)                error_fatal (x_base, "can not link lib to soname");
   snprintf (x_cmd , LEN_FULL, "ln -sf %s/%s %s/%s &> /dev/null", LIBRARIES, x_full, LIBRARIES, x_generic);
   rc = system(x_cmd);
   if (rc != 0)                error_fatal (x_base, "can not link lib to generic");
   /*---(update cache)---------------------*/
   snprintf (x_cmd , LEN_FULL, "/sbin/ldconfig -n %s &> /dev/null", LIBRARIES);
   rc = system (x_cmd);
   if (rc != 0)                error_fatal (x_full, "can not update cache");
   /*---(complete)-------------------------*/
   return 0;
}

int
main               (int argc, const char *argv[])
{
   char        rc          =    0;
   char        x_vernum    [LEN_TERSE]  = "";
   /*---(defense argc)---------------------*/
   if (argc <= 1)               error_fatal("n/a", "no arguments");
   /*> printf("option   : <<%s>>\n", argv[1]);                                        <*/
   /*---(locals)---------------------------*/
   int   x_rc        = 0;
   char  x_cmd[LEN_FULL]  = "";
   char  x_base[LEN_FULL] = "";
   char  x_option   = ' ';
   /*---(defense option)-------------------*/
   if (strlen(argv[1]) != 2)    error_fatal("n/a", "option malformed");
   if (argv[1][0]      != '-')  error_fatal("n/a", "option in wrong format");
   x_option = argv[1][1];
   if ((x_option < 'a' || x_option > 'z') &&
         (x_option < 'A' || x_option > 'Z')) {
      error_fatal ("n/a", "option not [A-Za-z]");
   }
   /*---(change uid/permissions)-----------*/
   /*> printf("user   (real)       = %d\n", getuid());                                <* 
    *> printf("user   (effective)  = %d\n", geteuid());                               <* 
    *> printf("group  (real)       = %d\n", getgid());                                <* 
    *> printf("group  (effective)  = %d\n", getegid());                               <* 
    *> x_rc = system("echo $UID");                                                    <* 
    *> x_rc = system("echo $EUID");                                                   <*/
   x_rc = setuid(0);
   if (x_rc != 0)               error_fatal("n/a", "could not setuid(0)");
   /*> x_rc = system("echo $UID");                                                    <* 
    *> x_rc = system("echo $EUID");                                                   <*/
   /*---(handle no arg options)-----------*/
   switch (x_option) {
   case 'l':
      printf ("#---shared libs----------------\n");
      snprintf(x_cmd, LEN_FULL, "ls -1 %s/liby*.so 2> /dev/null", LIBRARIES);
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no shared objects\n");
      snprintf(x_cmd, LEN_FULL, "ls -1 %s/liby*.a 2> /dev/null", LIBRARIES);
      printf ("#---static libs----------------\n");
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no archive objects\n");
      return 0;
      break;
   case 'v':
      snprintf(x_cmd, LEN_FULL, "ls -l %s/liby*    2> /dev/null", LIBRARIES);
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no shared/archive objects\n");
      return 0;
      break;
   }
   /*---(defense on name)------------------*/
   if (argc <= 2)               error_fatal("n/a", "no argument to act on");
   /*> printf("arg      : <<%s>>\n", argv[2]);                                        <*/
   strncpy(x_base, argv[2], LEN_FULL);
   if (strlen(x_base) < 1)       error_fatal("n/a", "argument is empty");
   /*---(get version)----------------------*/
   rc = get_version (x_base, x_vernum);
   if (rc < 0)     error_fatal (x_base, "can not find version");
   /*---(handle specific options)---------*/
   switch (x_option) {
   case 'V':
      printf ("%s, %s\n", x_base, x_vernum);
      break;
   case 'd':
      snprintf(x_cmd, LEN_FULL, "rm -f %s/lib%.40s.*", LIBRARIES, x_base);
      x_rc = system(x_cmd);
      if (x_rc != 0)            error_fatal(x_base, "can not delete shared lib");
      snprintf(x_cmd, LEN_FULL, "rm -f %s/%.40s.h*", HEADERS, x_base);
      x_rc = system(x_cmd);
      snprintf(x_cmd, LEN_FULL, "rm -f %s/%.40s_solo.h*", HEADERS, x_base);
      x_rc = system(x_cmd);
      return 0;
      break;
   case 'D':
      snprintf(x_cmd, LEN_FULL, "rm -f %s/lib%.40s.*", LIBRARIES, x_base);
      x_rc = system(x_cmd);
      if (x_rc != 0)            error_fatal(x_base, "can not delete shared lib");
      return 0;
      break;
   case 'h': case 'H':
      header_install (x_base);
      return 0;
      break;
   case 'a':
      static_install (x_base);
      header_install (x_base);
      return 0;
      break;
   case 'A':
      static_install (x_base);
      return 0;
      break;
   case 's':
      shared_install (x_base, x_vernum);
      header_install (x_base);
      return 0;
      break;
   case 'S':
      shared_install (x_base, x_vernum);
      return 0;
      break;
   case 'i':
   case 'b':
      shared_install (x_base, x_vernum);
      static_install (x_base);
      header_install (x_base);
      return 0;
      break;
   default :
      error_fatal("n/a", "option not understood");
      break;
   }
   /*---(complete)-------------------------*/
   printf("done\n");
   return -99;
}

