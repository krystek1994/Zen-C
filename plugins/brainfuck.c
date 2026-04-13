#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __has_builtin
#if __has_builtin(__builtin_pow)
#define _zc_pow __builtin_pow
#endif
#endif
#ifndef _zc_pow
extern double pow(double, double);
#define _zc_pow pow
#endif
#include <unistd.h>
#include <fcntl.h>
#if defined(_WIN32) || defined(_WIN64)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif
#define ZC_SIMD(T, N) T __attribute__((vector_size(N * sizeof(T))))
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO auto
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO __auto_type
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#define ZC_CAST(T, x) ((T)(x))
#ifdef __TINYC__
#undef ZC_AUTO
#define ZC_AUTO __auto_type
#undef ZC_AUTO_INIT
#define ZC_AUTO_INIT(var, init) __typeof__((init)) var = (init)

#ifndef __builtin_expect
#define __builtin_expect(x, v) (x)
#endif

#ifndef __builtin_unreachable
#define __builtin_unreachable()
#endif
#else
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202300L
#define ZC_AUTO_INIT(var, init) auto var = (init)
#else
#define ZC_AUTO_INIT(var, init) __auto_type var = (init)
#endif
#endif
static inline const char* _z_bool_str(_Bool b) { return b ? "true" : "false"; }
#ifdef __SIZEOF_INT128__
static inline const char *_z_u128_str(unsigned __int128 val) {
    static _Thread_local char buf[40];
    if (val == 0) return "0";
    int i = 38;
    buf[39] = 0;
    while (val > 0) { buf[i--] = (char)((val % 10) + '0'); val /= 10; }
    return &buf[i + 1];
}
static inline const char *_z_i128_str(__int128 val) {
    static _Thread_local char buf[41];
    if (val == 0) return "0";
    int neg = val < 0;
    unsigned __int128 uval = neg ? -val : val;
    int i = 39;
    buf[40] = 0;
    while (uval > 0) { buf[i--] = (char)((uval % 10) + '0'); uval /= 10; }
    if (neg) buf[i--] = '-';
    return &buf[i + 1];
}
#define _z_128_map ,__int128: "%s", unsigned __int128: "%s"
#else
#define _z_128_map
#endif
#ifdef __OBJC__
#define _z_objc_map ,id: "%s", Class: "%s", SEL: "%s"
#define _z_objc_arg_map(x) ,id: [(id)(x) description].UTF8String, Class: class_getName((Class)(x)), SEL: sel_getName((SEL)(x))
#else
#define _z_objc_map
#define _z_objc_arg_map(x)
#endif

#define _z_str(x) _Generic((x), _Bool: "%s", char: "%c", signed char: "%c", unsigned char: "%u", short: "%d", unsigned short: "%u", int: "%d", unsigned int: "%u", long: "%ld", unsigned long: "%lu", long long: "%lld", unsigned long long: "%llu", float: "%f", double: "%f", char*: "%s", const char*: "%s", void*: "%p" _z_128_map _z_objc_map)
#ifdef __SIZEOF_INT128__
#define _z_safe_i128(x) _Generic((x), __int128: (x), default: (__int128)0)
#define _z_safe_u128(x) _Generic((x), unsigned __int128: (x), default: (unsigned __int128)0)
#define _z_128_arg_map(x) ,__int128: _z_i128_str(_z_safe_i128(x)), unsigned __int128: _z_u128_str(_z_safe_u128(x))
#else
#define _z_128_arg_map(x)
#endif
#define _z_safe_bool(x) _Generic((x), _Bool: (x), default: (_Bool)0)
#define _z_arg(x) _Generic((x), _Bool: _z_bool_str(_z_safe_bool(x)) _z_128_arg_map(x) _z_objc_arg_map(x), default: (x))
typedef size_t usize;
typedef char* string;
typedef struct { void *func; void *ctx; void (*drop)(void*); } z_closure_T;
static void *_z_closure_ctx_stash[256];
typedef void U0;
typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;
#define F32 float
#define F64 double
#define z_malloc malloc
#define z_realloc realloc
#define z_free free
#define z_print printf
void __zenc_panic(const char* msg) { fprintf(stderr, "Panic: %s\n", msg); exit(1); }
#if defined(__APPLE__)
#define _ZC_SEC __attribute__((used,section("__DATA,__zarch")))
#elif defined(_WIN32)
#define _ZC_SEC __attribute__((used))
#else
#define _ZC_SEC __attribute__((used,section(".note.zarch")))
#endif
static const unsigned char _zc_abi_v1[] _ZC_SEC = {0x07,0xd5,0x59,0x30,0x7c,0x7f,0x66,0x75,0x30,0x69,0x7f,0x65,0x3c,0x30,0x59,0x7c,0x79,0x7e,0x73,0x71};
void _z_autofree_impl(void *p) { void **pp = (void**)p; if(*pp) { z_free(*pp); *pp = NULL; } }
#define __zenc_assert(cond, ...) if (!(cond)) { fprintf(stderr, "\"Assertion failed: \" " __VA_ARGS__); exit(1); }
string _z_readln_raw() { size_t cap = 64; size_t len = 0; char *line = z_malloc(cap); if(!line) return NULL; int c; while((c = fgetc(stdin)) != EOF) { if(c == '\n') break; if(len + 1 >= cap) { cap *= 2; char *n = z_realloc(line, cap); if(!n) { z_free(line); return NULL; } line = n; } line[len++] = c; } if(len == 0 && c == EOF) { z_free(line); return NULL; } line[len] = 0; return line; }
int _z_scan_helper(const char *fmt, ...) { char *l = _z_readln_raw(); if(!l) return 0; va_list ap; va_start(ap, fmt); int r = vsscanf(l, fmt, ap); va_end(ap); z_free(l); return r; }
int _z_orig_stdout = -1;
void _z_suppress_stdout() {
    fflush(stdout);
    if (_z_orig_stdout == -1) _z_orig_stdout = dup(STDOUT_FILENO);
    int nullfd = open("/dev/null", O_WRONLY);
    dup2(nullfd, STDOUT_FILENO);
    close(nullfd);
}
void _z_restore_stdout() {
    fflush(stdout);
    if (_z_orig_stdout != -1) {
        dup2(_z_orig_stdout, STDOUT_FILENO);
        close(_z_orig_stdout);
        _z_orig_stdout = -1;
    }
}
#ifndef ZC_CFG_linux
#define ZC_CFG_linux 1
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio.h>
typedef char* string;
typedef struct { void **data; int len; int cap; } Vec;
#define Vec_new() (Vec){.data=0, .len=0, .cap=0}
void _z_vec_push(Vec *v, void *item) { if(v->len >= v->cap) { v->cap = v->cap?v->cap*2:8; v->data = z_realloc(v->data, v->cap * sizeof(void*)); } v->data[v->len++] = item; }
static inline Vec _z_make_vec(int count, ...) { Vec v = {0}; v.cap = count > 8 ? count : 8; v.data = z_malloc(v.cap * sizeof(void*)); v.len = 0; va_list args; va_start(args, count); for(int i=0; i<count; i++) { v.data[v.len++] = va_arg(args, void*); } va_end(args); return v; }
#define Vec_push(v, i) _z_vec_push(&(v), (void*)(uintptr_t)(i))
static inline long _z_check_bounds(long index, long limit) { if(index < 0 || index >= limit) { fprintf(stderr, "Index out of bounds: %ld (limit %ld)\n", index, limit); exit(1); } return index; }
typedef struct ZPlugin ZPlugin;
typedef struct ZConfig ZConfig;
typedef struct ZApi ZApi;


struct ZPlugin {
    char name[256];
    void (*handler)(char*, ZApi*);
    char* (*hover_handler)(char*, int32_t, int32_t);
};

struct ZConfig {
    int32_t is_debug;
    int32_t verbose;
    char* target;
    char* cc;
};

struct ZApi {
    uint32_t api_version;
    char* filename;
    int32_t current_line;
    void* out;
    void* hoist_out;
    void (*error)(ZApi*, char*, ...);
    void (*warn)(ZApi*, char*, ...);
    void (*note)(ZApi*, char*, ...);
    ZConfig config;
    void* user_data;
};

ZPlugin* z_plugin_init(void);
char* bf_hover(char* body, int32_t line, int32_t col);
void bf_transpile(char* body, ZApi* api);
void __zenc_todo_impl(const char* file, int32_t line, const char* func, const char* msg);
void __zenc_panic_impl(const char* file, int32_t line, const char* func, const char* msg);
void ZApi__report_error(ZApi* self, char* msg);
void ZApi__report_warn(ZApi* self, char* msg);
void ZApi__report_note(ZApi* self, char* msg);
const int32_t ZEN_PLUGIN_API_VERSION = 2;
const int32_t ZEN_PLUGIN_MAX_NAME = 256;
ZPlugin bf_plugin = (struct ZPlugin){.name = "brainfuck", .handler = bf_transpile, .hover_handler = bf_hover};
// Global Generic Drop Dispatch
#define _z_drop(x) _Generic((x), \
    default: (void)0)


#line 69 "plugins/brainfuck.zc"

__attribute__((visibility("default"))) ZPlugin* z_plugin_init(void)
{
    {

#line 70 "plugins/brainfuck.zc"
    return (&bf_plugin);
    }
}

#line 34 "plugins/brainfuck.zc"

char* bf_hover(char* body, int32_t line, int32_t col)
{
    {

#line 35 "plugins/brainfuck.zc"
    char* p = body;

#line 36 "plugins/brainfuck.zc"
    int32_t r = 0;
    int32_t c = 0;

#line 38 "plugins/brainfuck.zc"
while ((p[0] != 0))     {

#line 39 "plugins/brainfuck.zc"
if (((r == line) && (c == col)))     {

#line 40 "plugins/brainfuck.zc"
{ int32_t _m_0 = ((int32_t)(p[0])); 
#line 41 "plugins/brainfuck.zc"
if (_m_0 == '>') {     return "**:ptr++**: Increment the data pointer.";
 } else 
#line 42 "plugins/brainfuck.zc"
if (_m_0 == '<') {     return "**:ptr--**: Decrement the data pointer.";
 } else 
#line 43 "plugins/brainfuck.zc"
if (_m_0 == '+') {     return "**(*ptr)++**: Increment the byte at the data pointer.";
 } else 
#line 44 "plugins/brainfuck.zc"
if (_m_0 == '-') {     return "**(*ptr)--**: Decrement the byte at the data pointer.";
 } else 
#line 45 "plugins/brainfuck.zc"
if (_m_0 == '.') {     return "**:putchar**: Output the byte at the data pointer.";
 } else 
#line 46 "plugins/brainfuck.zc"
if (_m_0 == ',') {     return "**:getchar**: Accept one byte of input, storing its value in the byte at the data pointer.";
 } else 
#line 47 "plugins/brainfuck.zc"
if (_m_0 == '[') {     return "**:while(*ptr)**: If the byte at the data pointer is zero, jump forward to the command after the corresponding `]` command.";
 } else 
#line 48 "plugins/brainfuck.zc"
if (_m_0 == ']') {     return "**:end-while**: If the byte at the data pointer is nonzero, jump back to the command after the corresponding `[` command.";
 } else 
#line 49 "plugins/brainfuck.zc"
if (1) {     return NULL;
 } };
    }

#line 52 "plugins/brainfuck.zc"
if ((p[0] == 10))     {
(r++);

#line 53 "plugins/brainfuck.zc"
(c = 0);
    }
 else     {
(c++);
    }

#line 57 "plugins/brainfuck.zc"
(p = (&p[1]));
    }

#line 59 "plugins/brainfuck.zc"
    return NULL;
    }
}

#line 6 "plugins/brainfuck.zc"

void bf_transpile(char* body, ZApi* api)
{
    {

#line 7 "plugins/brainfuck.zc"
    void* out = api->out;

#line 9 "plugins/brainfuck.zc"
fputs(({ 
#line 9 "plugins/brainfuck.zc"
    static char _fs_buf_0[4096]; _fs_buf_0[0]=0;
    char _fs_t_0[128];
    strcat(_fs_buf_0, "{");
    strcat(_fs_buf_0, "\n    static unsigned char tape[30000] = ");
    strcat(_fs_buf_0, "{");
    strcat(_fs_buf_0, "0");
    strcat(_fs_buf_0, "}");
    strcat(_fs_buf_0, ";\n    unsigned char *ptr = tape;\n");
    _fs_buf_0
 }), out);

#line 14 "plugins/brainfuck.zc"
    int32_t idx = 0;

#line 15 "plugins/brainfuck.zc"
while ((body[idx] != 0))     {

#line 16 "plugins/brainfuck.zc"
    char ch = body[idx];

#line 17 "plugins/brainfuck.zc"
{ char _m_1 = ch; 
#line 18 "plugins/brainfuck.zc"
if (_m_1 == '>') { 
#line 18 "plugins/brainfuck.zc"
fputs("    ++ptr;\n", out);
 } else 
#line 19 "plugins/brainfuck.zc"
if (_m_1 == '<') { 
#line 19 "plugins/brainfuck.zc"
fputs("    --ptr;\n", out);
 } else 
#line 20 "plugins/brainfuck.zc"
if (_m_1 == '+') { 
#line 20 "plugins/brainfuck.zc"
fputs("    ++*ptr;\n", out);
 } else 
#line 21 "plugins/brainfuck.zc"
if (_m_1 == '-') { 
#line 21 "plugins/brainfuck.zc"
fputs("    --*ptr;\n", out);
 } else 
#line 22 "plugins/brainfuck.zc"
if (_m_1 == '.') { 
#line 22 "plugins/brainfuck.zc"
fputs("    putchar(*ptr);\n", out);
 } else 
#line 23 "plugins/brainfuck.zc"
if (_m_1 == ',') { 
#line 23 "plugins/brainfuck.zc"
fputs("    *ptr = getchar();\n", out);
 } else 
#line 24 "plugins/brainfuck.zc"
if (_m_1 == '[') { 
#line 24 "plugins/brainfuck.zc"
fputs(({ 
#line 24 "plugins/brainfuck.zc"
    static char _fs_buf_1[4096]; _fs_buf_1[0]=0;
    char _fs_t_1[128];
    strcat(_fs_buf_1, "    while (*ptr) ");
    strcat(_fs_buf_1, "{");
    strcat(_fs_buf_1, " \n");
    _fs_buf_1
 }), out);
 } else 
#line 25 "plugins/brainfuck.zc"
if (_m_1 == ']') { 
#line 25 "plugins/brainfuck.zc"
fputs(({ 
#line 25 "plugins/brainfuck.zc"
    static char _fs_buf_2[4096]; _fs_buf_2[0]=0;
    char _fs_t_2[128];
    strcat(_fs_buf_2, "    ");
    strcat(_fs_buf_2, "}");
    strcat(_fs_buf_2, " \n");
    _fs_buf_2
 }), out);
 } else 
#line 26 "plugins/brainfuck.zc"
if (1) { ((void)(0));
 } };

#line 28 "plugins/brainfuck.zc"
(idx = (idx + 1));
    }

#line 31 "plugins/brainfuck.zc"
fputs(({ 
#line 31 "plugins/brainfuck.zc"
    static char _fs_buf_3[4096]; _fs_buf_3[0]=0;
    char _fs_t_3[128];
    strcat(_fs_buf_3, "}");
    strcat(_fs_buf_3, " \n");
    _fs_buf_3
 }), out);
    }
}

#line 13 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"

void __zenc_todo_impl(const char* file, int32_t line, const char* func, const char* msg)
{
    {

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"
fprintf(stderr, "todo: %s\n  at %s:%d in %s()\n", msg, file, line, func);

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"
exit(1);
    }
}

#line 8 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"

void __zenc_panic_impl(const char* file, int32_t line, const char* func, const char* msg)
{
    {

#line 9 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"
fprintf(stderr, "panic: %s\n  at %s:%d in %s()\n", msg, file, line, func);

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/core.zc"
exit(1);
    }
}

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"

void ZApi__report_error(ZApi* self, char* msg)
{
    {

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"
self->error(self, "%s", msg);
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"

void ZApi__report_warn(ZApi* self, char* msg)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"
self->warn(self, "%s", msg);
    }
}

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"

void ZApi__report_note(ZApi* self, char* msg)
{
    {

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/plugin.zc"
self->note(self, "%s", msg);
    }
}
