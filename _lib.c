/*===[[ START ]]==============================================================*/
#include  "_lib.h"

char
error_fatal(char *a_name, char *a_message)
{
   printf("FATAL  : %.50s\n", a_message);
   printf("object : %.50s\n", a_name);
   printf("usage  :\n");
   printf("   _lib -l         list all installed libraries (.so)\n");
   printf("   _lib -v         list all installed libraries (.*)\n");
   printf("   _lib -d <name>  delete a specific archive/shared library\n");
   printf("   _lib -a <name>  install a specific archive/static library\n");
   printf("   _lib -A <name>  install only static library, no header\n");
   printf("   _lib -s <name>  install a specific shared library\n");
   printf("   _lib -S <name>  install only shared library, no header\n");
   printf("   _lib -i <name>  install both archive and shared library\n");
   exit(-1);
}

char
header_install(char *a_name)
{
   int   x_rc        = 0;
   char  x_cmd[200]  = "";
   char  x_name[200] = "";
   /*---(try regular header)---------------*/
   struct stat sb;
   snprintf(x_name, 200, "%s.h", a_name);
   x_rc = stat(x_name, &sb);
   if (x_rc == 0) {
      snprintf(x_cmd, 200, "cp -f %.40s %s &> /dev/null", x_name, HEADERS);
      x_rc = system(x_cmd);
      if (x_rc != 0)            error_fatal(x_name, "can not copy to shared folder");
   } else {
      snprintf(x_name, 200, "%s.hpp", a_name);
      x_rc = stat(x_name, &sb);
      if (x_rc == 0) {
         snprintf(x_cmd, 200, "cp -f %.40s %s &> /dev/null", x_name, HEADERS);
         x_rc = system(x_cmd);
         if (x_rc != 0)            error_fatal(x_name, "can not copy to shared folder");
      }
   }
   /*---(fix permissions)------------------*/
   snprintf(x_cmd, 200, "chmod 0755 %s/%.40s &> /dev/null", HEADERS, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not change permissions");
   /*---(complete)-------------------------*/
   return 0;
}

char
static_install(char *a_name)
{
   int   x_rc        = 0;
   char  x_cmd[200]  = "";
   char  x_name[200] = "";
   snprintf(x_name, 200, "%s.a", a_name);
   /*---(check for existance)--------------*/
   snprintf(x_cmd, 200, "ls lib%.40s &> /dev/null", x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "no existing local archive");
   /*---(install the library)--------------*/
   snprintf(x_cmd, 200, "cp -f lib%.40s %s &> /dev/null", x_name, LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not copy to shared folder");
   /*---(fix permissions)------------------*/
   snprintf(x_cmd, 200, "chmod 0755 %s/lib%.40s &> /dev/null", LIBRARIES, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not change permissions");
   /*---(update cache)---------------------*/
   snprintf(x_cmd, 200, "/sbin/ldconfig -n %s &> /dev/null", LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not update cache");
   /*---(complete)-------------------------*/
   return 0;
}

char
shared_install(char *a_name)
{
   int   x_rc        = 0;
   char  x_cmd[200]  = "";
   char  x_name[200] = "";
   snprintf(x_name, 200, "%s.so", a_name);
   /*---(check for existance)--------------*/
   snprintf(x_cmd, 200, "ls lib%.40s.1.0 &> /dev/null", x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "no existing local lib");
   /*---(install the library)--------------*/
   snprintf(x_cmd, 200, "cp -f lib%.40s.1.0 %s &> /dev/null", x_name, LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not copy to shared folder");
   /*---(fix permissions)------------------*/
   snprintf(x_cmd, 200, "chmod 0755 %s/lib%.40s.1.0 &> /dev/null", LIBRARIES, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not change permissions");
   /*---(update cache)---------------------*/
   snprintf(x_cmd, 200, "/sbin/ldconfig -n %s &> /dev/null", LIBRARIES);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not update cache");
   /*---(link library)---------------------*/
   snprintf(x_cmd, 200, "ln -sf %s/lib%.40s.1.0 %s/lib%.40s.1 &> /dev/null", LIBRARIES, x_name, LIBRARIES, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not link lib to so.1");
   snprintf(x_cmd, 200, "ln -sf %s/lib%.40s.1.0 %s/lib%.40s &> /dev/null", LIBRARIES, x_name, LIBRARIES, x_name);
   x_rc = system(x_cmd);
   if (x_rc != 0)            error_fatal(x_name, "can not link lib to so");
   /*---(complete)-------------------------*/
   return 0;
}


int
main               (int argc, const char *argv[])
{
   /*---(defense argc)---------------------*/
   if (argc <= 1)               error_fatal("n/a", "no arguments");
   /*> printf("option   : <<%s>>\n", argv[1]);                                        <*/
   /*---(locals)---------------------------*/
   int   x_rc        = 0;
   char  x_cmd[200]  = "";
   char  x_base[200] = "";
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
      snprintf(x_cmd, 200, "ls -1 %s/liby*.so 2> /dev/null", LIBRARIES);
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no shared objects\n");
      snprintf(x_cmd, 200, "ls -1 %s/liby*.a 2> /dev/null", LIBRARIES);
      printf ("#---static libs----------------\n");
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no archive objects\n");
      return 0;
      break;
   case 'v':
      snprintf(x_cmd, 200, "ls -l %s/liby*    2> /dev/null", LIBRARIES);
      x_rc = system(x_cmd);
      if (x_rc != 0)            printf("no shared/archive objects\n");
      return 0;
      break;
   }
   /*---(defense on name)------------------*/
   if (argc <= 2)               error_fatal("n/a", "no argument to act on");
   /*> printf("arg      : <<%s>>\n", argv[2]);                                        <*/
   strncpy(x_base, argv[2], 200);
   if (strlen(x_base) < 1)       error_fatal("n/a", "argument is empty");
   /*---(handle specific options)---------*/
   switch (x_option) {
   case 'd':
      snprintf(x_cmd, 200, "rm -f %s/lib%.40s.*", LIBRARIES, x_base);
      x_rc = system(x_cmd);
      if (x_rc != 0)            error_fatal(x_base, "can not delete shared lib");
      snprintf(x_cmd, 200, "rm -f %s/%.40s.h*", HEADERS, x_base);
      x_rc = system(x_cmd);
      return 0;
      break;
   case 'a':
      static_install(x_base);
      header_install(x_base);
      return 0;
      break;
   case 'A':
      static_install(x_base);
      return 0;
      break;
   case 's':
      shared_install(x_base);
      header_install(x_base);
      return 0;
      break;
   case 'S':
      shared_install(x_base);
      return 0;
      break;
   case 'i':
   case 'b':
      shared_install(x_base);
      static_install(x_base);
      header_install(x_base);
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

