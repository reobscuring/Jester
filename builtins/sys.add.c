#include <stdlib.h>
#include <string.h>
typedef struct JesterRegistry JesterRegistry;
typedef char *(*JesterBuiltin)(const char *,const char **,const char **,size_t,void *);
extern void jester_registry_add(JesterRegistry *,const char *,JesterBuiltin);
static char *add_call(const char *path,const char **names,const char **values,size_t count,void *ctx){(void)path;(void)names;(void)ctx;size_t n=1;for(size_t i=0;i<count;i++)n+=strlen(values[i]);char*out=malloc(n),*p=out;for(size_t i=0;i<count;i++){size_t z=strlen(values[i]);memcpy(p,values[i],z);p+=z;}*p=0;return out;}
void jester_builtin_register(JesterRegistry *registry){jester_registry_add(registry,"sys.add",add_call);}
