/* Runtime, registry, and common definitions for Jester. Included by LPIM.c. */
#ifndef JESTER_RRC_C
#define JESTER_RRC_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>

typedef enum { J_STRING, J_BLOCK, J_FUNCTION, J_BOOL } JType;
typedef struct { char *name; char *value; JType type; } JVar;
typedef struct JesterRegistry JesterRegistry;
typedef char *(*JesterBuiltin)(const char *path, const char **names, const char **values, size_t count, void *context);
typedef struct { char *path; JesterBuiltin fn; } JBuiltin;
struct JesterRegistry { JBuiltin *items; size_t count, cap; };
typedef struct { JVar *vars; size_t count, cap; JesterRegistry registry; } Runtime;

static char *j_dup(const char *s) { size_t n=strlen(s); char *p=malloc(n+1); if(!p) exit(2); memcpy(p,s,n+1); return p; }
static char *j_trim(char *s) { while(isspace((unsigned char)*s)) s++; char *e=s+strlen(s); while(e>s && isspace((unsigned char)e[-1])) *--e=0; return s; }
void jester_registry_add(JesterRegistry *r,const char *p,JesterBuiltin f) { if(r->count==r->cap){r->cap=r->cap?r->cap*2:8;r->items=realloc(r->items,r->cap*sizeof *r->items);} r->items[r->count++]=(JBuiltin){j_dup(p),f}; }
static JesterBuiltin jester_registry_find(JesterRegistry*r,const char*p) { for(size_t i=0;i<r->count;i++) if(!strcmp(r->items[i].path,p)) return r->items[i].fn; return NULL; }
static JVar *rt_var(Runtime *r,const char *n) { for(size_t i=0;i<r->count;i++) if(!strcmp(r->vars[i].name,n)) return &r->vars[i]; return NULL; }
static void rt_set(Runtime*r,const char*n,const char*v,JType t){JVar*x=rt_var(r,n);if(!x){if(r->count==r->cap){r->cap=r->cap?r->cap*2:16;r->vars=realloc(r->vars,r->cap*sizeof*r->vars);}x=&r->vars[r->count++];x->name=j_dup(n);x->value=NULL;}free(x->value);x->value=j_dup(v);x->type=t;}
static void rt_free(Runtime*r){for(size_t i=0;i<r->count;i++){free(r->vars[i].name);free(r->vars[i].value);}free(r->vars);for(size_t i=0;i<r->registry.count;i++)free(r->registry.items[i].path);free(r->registry.items);}
/* A plugin exports: void jester_builtin_register(JesterRegistry *). */
static void rt_load_builtins(Runtime*r,const char *dir){DIR*d=opendir(dir);if(!d)return;struct dirent*e;while((e=readdir(d))){if(!strstr(e->d_name,".so"))continue;char p[4096];snprintf(p,sizeof p,"%s/%s",dir,e->d_name);void*h=dlopen(p,RTLD_NOW|RTLD_GLOBAL);if(!h)continue;void(*reg)(JesterRegistry*)=dlsym(h,"jester_builtin_register");if(reg)reg(&r->registry);}closedir(d);}
#endif
