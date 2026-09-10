#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct JesterRegistry JesterRegistry;
typedef char *(*JesterBuiltin)(const char *,const char **,const char **,size_t,void *);
extern void jester_registry_add(JesterRegistry *,const char *,JesterBuiltin);
static char *log_call(const char *path,const char **names,const char **values,size_t count,void *ctx){(void)path;(void)names;(void)ctx;for(size_t i=0;i<count;i++)fputs(values[i],stdout);fputc('\n',stdout);const char *v=count?values[0]:""; char *out=malloc(strlen(v)+1); strcpy(out,v); return out;}
void jester_builtin_register(JesterRegistry *registry){jester_registry_add(registry,"sys.log",log_call);}
