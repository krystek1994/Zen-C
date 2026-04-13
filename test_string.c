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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "std/third-party/tre/include/tre.h"
#include <stdio.h>
#include <stdlib.h>
typedef char* string;
typedef struct { void **data; int len; int cap; } Vec;
#define Vec_new() (Vec){.data=0, .len=0, .cap=0}
void _z_vec_push(Vec *v, void *item) { if(v->len >= v->cap) { v->cap = v->cap?v->cap*2:8; v->data = z_realloc(v->data, v->cap * sizeof(void*)); } v->data[v->len++] = item; }
static inline Vec _z_make_vec(int count, ...) { Vec v = {0}; v.cap = count > 8 ? count : 8; v.data = z_malloc(v.cap * sizeof(void*)); v.len = 0; va_list args; va_start(args, count); for(int i=0; i<count; i++) { v.data[v.len++] = va_arg(args, void*); } va_end(args); return v; }
#define Vec_push(v, i) _z_vec_push(&(v), (void*)(uintptr_t)(i))
static inline long _z_check_bounds(long index, long limit) { if(index < 0 || index >= limit) { fprintf(stderr, "Index out of bounds: %ld (limit %ld)\n", index, limit); exit(1); } return index; }
typedef struct Option__string Option__string;
typedef struct Option__bool Option__bool;
typedef struct Option__double Option__double;
typedef struct Option__charPtr Option__charPtr;
typedef struct Result__JsonValuePtr Result__JsonValuePtr;
typedef struct MapEntry__JsonValuePtr MapEntry__JsonValuePtr;
typedef struct MapIter__JsonValuePtr MapIter__JsonValuePtr;
typedef struct Map__JsonValuePtr Map__JsonValuePtr;
typedef struct VecIterResult__JsonValuePtr VecIterResult__JsonValuePtr;
typedef struct VecIterRef__JsonValuePtr VecIterRef__JsonValuePtr;
typedef struct Option__JsonValuePtr Option__JsonValuePtr;
typedef struct VecIter__JsonValuePtr VecIter__JsonValuePtr;
typedef struct Vec__JsonValuePtr Vec__JsonValuePtr;
typedef struct VecIterResult__DirEntryPtr VecIterResult__DirEntryPtr;
typedef struct VecIterRef__DirEntryPtr VecIterRef__DirEntryPtr;
typedef struct Option__DirEntryPtr Option__DirEntryPtr;
typedef struct VecIter__DirEntryPtr VecIter__DirEntryPtr;
typedef struct Vec__DirEntryPtr Vec__DirEntryPtr;
typedef struct VecIterResult__DirEntry VecIterResult__DirEntry;
typedef struct VecIterRef__DirEntry VecIterRef__DirEntry;
typedef struct VecIter__DirEntry VecIter__DirEntry;
typedef struct Vec__DirEntry Vec__DirEntry;
typedef struct VecIterResult__StringPtr VecIterResult__StringPtr;
typedef struct VecIterRef__StringPtr VecIterRef__StringPtr;
typedef struct Option__StringPtr Option__StringPtr;
typedef struct VecIter__StringPtr VecIter__StringPtr;
typedef struct Vec__StringPtr Vec__StringPtr;
typedef struct Result__bool Result__bool;
typedef struct VecIterResult__String VecIterResult__String;
typedef struct VecIterRef__String VecIterRef__String;
typedef struct VecIter__String VecIter__String;
typedef struct Vec__String Vec__String;
typedef struct VecIterResult__int32_t VecIterResult__int32_t;
typedef struct VecIterRef__int32_t VecIterRef__int32_t;
typedef struct VecIter__int32_t VecIter__int32_t;
typedef struct Vec__int32_t Vec__int32_t;
typedef struct VecIterResult__size_t VecIterResult__size_t;
typedef struct VecIterRef__size_t VecIterRef__size_t;
typedef struct VecIter__size_t VecIter__size_t;
typedef struct Vec__size_t Vec__size_t;
typedef struct Option__size_t Option__size_t;
typedef struct Option__int32_t Option__int32_t;
typedef struct VecIterResult__char VecIterResult__char;
typedef struct VecIterRef__char VecIterRef__char;
typedef struct Option__char Option__char;
typedef struct VecIter__char VecIter__char;
typedef struct Vec__char Vec__char;
typedef struct Regex Regex;
typedef struct ZRegMatch ZRegMatch;
typedef struct Match Match;
typedef struct Env Env;
typedef struct Arena Arena;
typedef struct Time Time;
typedef struct Duration Duration;
typedef struct Metadata Metadata;
typedef struct File File;
typedef struct StringCharsIter StringCharsIter;
typedef struct Sort Sort;
typedef struct EnvRes EnvRes;
typedef struct JsonType JsonType;
typedef struct Option__Match Option__Match;
typedef struct MapIterResult__JsonValuePtr MapIterResult__JsonValuePtr;
typedef struct Result__Vec__DirEntry Result__Vec__DirEntry;
typedef struct Result__Metadata Result__Metadata;
typedef struct Result__Vec__String Result__Vec__String;
typedef struct Result__File Result__File;
typedef struct JsonValue JsonValue;
typedef struct String String;
typedef struct Result__JsonValue Result__JsonValue;
typedef struct Result__String Result__String;
typedef struct Option__String Option__String;
typedef struct Command Command;
typedef struct Output Output;
typedef struct Path Path;
typedef struct DirEntry DirEntry;
typedef struct Option__Path Option__Path;
typedef struct Option__DirEntry Option__DirEntry;


EnvRes EnvRes__ERR();
EnvRes EnvRes__OK();
JsonType JsonType__JSON_NULL();
JsonType JsonType__JSON_BOOL();
JsonType JsonType__JSON_NUMBER();
JsonType JsonType__JSON_STRING();
JsonType JsonType__JSON_ARRAY();
JsonType JsonType__JSON_OBJECT();
typedef tre_str_source tre_str_source;
typedef regamatch_t regamatch_t;
typedef regaparams_t regaparams_t;
typedef reg_errcode_t reg_errcode_t;
typedef regmatch_t regmatch_t;
typedef regex_t regex_t;
typedef JsonValue* JsonValuePtr;
typedef JsonValue* JsonValuePtr;
typedef struct Clone_VTable {
    void* (*clone)(void* self);
} Clone_VTable;
typedef struct Clone { void *self; Clone_VTable *vtable; } Clone;

typedef struct Copy_VTable {
} Copy_VTable;
typedef struct Copy { void *self; Copy_VTable *vtable; } Copy;

typedef struct Drop_VTable {
    void (*drop)(void* self);
} Drop_VTable;
typedef struct Drop { void *self; Drop_VTable *vtable; } Drop;


    #define ZC_IMPL_SORT(T) \
    static int _z_partition_##T(T* arr, int low, int high) { \
        T pivot = arr[high]; \
        int i = low - 1; \
        for (int j = low; j < high; j++) { \
            if (arr[j] < pivot) { \
                i++; \
                T temp = arr[i]; \
                arr[i] = arr[j]; \
                arr[j] = temp; \
            } \
        } \
        T temp2 = arr[i + 1]; \
        arr[i + 1] = arr[high]; \
        arr[high] = temp2; \
        return i + 1; \
    } \
    static void _z_quick_sort_##T(T* arr, int low, int high) { \
        if (low < high) { \
            int pivot_idx = _z_partition_##T(arr, low, high); \
            _z_quick_sort_##T(arr, low, pivot_idx - 1); \
            _z_quick_sort_##T(arr, pivot_idx + 1, high); \
        } \
    } \
    void sort_##T(T* arr, size_t len) { \
        if (len > 1) { \
            _z_quick_sort_##T(arr, 0, (int)len - 1); \
        } \
    }

    // Pre-declare standard library types
    ZC_IMPL_SORT(int)
    ZC_IMPL_SORT(long)
    ZC_IMPL_SORT(float)
    ZC_IMPL_SORT(double)

struct Option__string {
    bool is_some;
    string val;
};

struct Option__bool {
    bool is_some;
    bool val;
};

struct Option__double {
    bool is_some;
    double val;
};

struct Option__charPtr {
    bool is_some;
    char* val;
};

struct Result__JsonValuePtr {
    bool is_ok;
    JsonValue* val;
    char* err;
};

struct MapEntry__JsonValuePtr {
    char* key;
    struct JsonValue* val;
};

struct MapIter__JsonValuePtr {
    char** keys;
    struct JsonValue** vals;
    bool* occupied;
    bool* deleted;
    size_t cap;
    size_t idx;
};

struct Map__JsonValuePtr {
    char** keys;
    JsonValue** vals;
    bool* occupied;
    bool* deleted;
    size_t len;
    size_t cap;
};

struct VecIterResult__JsonValuePtr {
    struct JsonValue** ptr;
};

struct VecIterRef__JsonValuePtr {
    struct JsonValue** data;
    size_t count;
    size_t idx;
};

struct Option__JsonValuePtr {
    bool is_some;
    struct JsonValue* val;
};

struct VecIter__JsonValuePtr {
    struct JsonValue** data;
    size_t count;
    size_t idx;
};

struct Vec__JsonValuePtr {
    JsonValue** data;
    size_t len;
    size_t cap;
};

struct VecIterResult__DirEntryPtr {
    struct DirEntry** ptr;
};

struct VecIterRef__DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Option__DirEntryPtr {
    bool is_some;
    struct DirEntry* val;
};

struct VecIter__DirEntryPtr {
    struct DirEntry** data;
    size_t count;
    size_t idx;
};

struct Vec__DirEntryPtr {
    DirEntry** data;
    size_t len;
    size_t cap;
};

struct VecIterResult__DirEntry {
    DirEntry* ptr;
};

struct VecIterRef__DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct VecIter__DirEntry {
    DirEntry* data;
    size_t count;
    size_t idx;
};

struct Vec__DirEntry {
    DirEntry* data;
    size_t len;
    size_t cap;
};

struct VecIterResult__StringPtr {
    struct String** ptr;
};

struct VecIterRef__StringPtr {
    struct String** data;
    size_t count;
    size_t idx;
};

struct Option__StringPtr {
    bool is_some;
    struct String* val;
};

struct VecIter__StringPtr {
    struct String** data;
    size_t count;
    size_t idx;
};

struct Vec__StringPtr {
    String** data;
    size_t len;
    size_t cap;
};

struct Result__bool {
    bool is_ok;
    bool val;
    char* err;
};

struct VecIterResult__String {
    String* ptr;
};

struct VecIterRef__String {
    String* data;
    size_t count;
    size_t idx;
};

struct VecIter__String {
    String* data;
    size_t count;
    size_t idx;
};

struct Vec__String {
    String* data;
    size_t len;
    size_t cap;
};

struct VecIterResult__int32_t {
    int32_t* ptr;
};

struct VecIterRef__int32_t {
    int32_t* data;
    size_t count;
    size_t idx;
};

struct VecIter__int32_t {
    int32_t* data;
    size_t count;
    size_t idx;
};

struct Vec__int32_t {
    int32_t* data;
    size_t len;
    size_t cap;
};

struct VecIterResult__size_t {
    size_t* ptr;
};

struct VecIterRef__size_t {
    size_t* data;
    size_t count;
    size_t idx;
};

struct VecIter__size_t {
    size_t* data;
    size_t count;
    size_t idx;
};

struct Vec__size_t {
    size_t* data;
    size_t len;
    size_t cap;
};

struct Option__size_t {
    bool is_some;
    size_t val;
};

struct Option__int32_t {
    bool is_some;
    int32_t val;
};

struct VecIterResult__char {
    char* ptr;
};

struct VecIterRef__char {
    char* data;
    size_t count;
    size_t idx;
};

struct Option__char {
    bool is_some;
    char val;
};

struct VecIter__char {
    char* data;
    size_t count;
    size_t idx;
};

struct Vec__char {
    char* data;
    size_t len;
    size_t cap;
};

struct Regex {
    void* preg;
    char* pattern;
    int32_t flags;
};

struct ZRegMatch {
    int32_t rm_so;
    int32_t rm_eo;
};

struct Match {
    char* text;
    int32_t start;
    int32_t len;
};

struct Env {
    char _placeholder;
};

struct Arena {
    void* data;
    size_t capacity;
    size_t used;
};

struct Time {
    char _placeholder;
};

struct Duration {
    uint64_t millis;
};

struct Metadata {
    uint64_t size;
    bool is_dir;
    bool is_file;
};

struct File {
    void* handle;
};

struct StringCharsIter {
    char* data;
    size_t len;
    size_t pos;
};

struct Sort {
    char _placeholder;
};

typedef enum { EnvRes__ERR_Tag, EnvRes__OK_Tag, } EnvRes_Tag;
struct EnvRes { EnvRes_Tag tag; union { } data; };

EnvRes EnvRes__ERR() { return (EnvRes){.tag=EnvRes__ERR_Tag}; }
EnvRes EnvRes__OK() { return (EnvRes){.tag=EnvRes__OK_Tag}; }
typedef enum { JsonType__JSON_NULL_Tag, JsonType__JSON_BOOL_Tag, JsonType__JSON_NUMBER_Tag, JsonType__JSON_STRING_Tag, JsonType__JSON_ARRAY_Tag, JsonType__JSON_OBJECT_Tag, } JsonType_Tag;
struct JsonType { JsonType_Tag tag; union { } data; };

JsonType JsonType__JSON_NULL() { return (JsonType){.tag=JsonType__JSON_NULL_Tag}; }
JsonType JsonType__JSON_BOOL() { return (JsonType){.tag=JsonType__JSON_BOOL_Tag}; }
JsonType JsonType__JSON_NUMBER() { return (JsonType){.tag=JsonType__JSON_NUMBER_Tag}; }
JsonType JsonType__JSON_STRING() { return (JsonType){.tag=JsonType__JSON_STRING_Tag}; }
JsonType JsonType__JSON_ARRAY() { return (JsonType){.tag=JsonType__JSON_ARRAY_Tag}; }
JsonType JsonType__JSON_OBJECT() { return (JsonType){.tag=JsonType__JSON_OBJECT_Tag}; }
struct Option__Match {
    bool is_some;
    Match val;
};

struct MapIterResult__JsonValuePtr {
    MapEntry__JsonValuePtr entry;
    bool has_val;
};

struct Result__Vec__DirEntry {
    bool is_ok;
    Vec__DirEntry val;
    char* err;
};

struct Result__Metadata {
    bool is_ok;
    Metadata val;
    char* err;
};

struct Result__Vec__String {
    bool is_ok;
    Vec__String val;
    char* err;
};

struct Result__File {
    bool is_ok;
    File val;
    char* err;
};

struct JsonValue {
    JsonType kind;
    char* string_val;
    double number_val;
    bool bool_val;
    Vec__JsonValuePtr* array_val;
    Map__JsonValuePtr* object_val;
};

struct String {
    Vec__char vec;
};

struct Result__JsonValue {
    bool is_ok;
    JsonValue val;
    char* err;
};

struct Result__String {
    bool is_ok;
    String val;
    char* err;
};

struct Option__String {
    bool is_some;
    String val;
};

struct Command {
    String program;
    Vec__String args;
};

struct Output {
    String std_out;
    int32_t exit_code;
};

struct Path {
    String str;
};

struct DirEntry {
    String name;
    bool is_dir;
};

struct Option__Path {
    bool is_some;
    Path val;
};

struct Option__DirEntry {
    bool is_some;
    DirEntry val;
};


    typedef struct DirEntry* DirEntryPtr;
    
    // DIR* wrappers - opendir/closedir/readdir use DIR* which conflicts with void*
    void* _z_fs_opendir(const char* name) {
        return opendir(name);
    }

    int _z_fs_closedir(void* dir) {
        return closedir((DIR*)dir);
    }

    // struct stat access - cannot define matching Zen-C struct for stat
    int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file) {
        struct stat st;
        if (stat(path, &st) != 0) return -1;
        *size = st.st_size;
        *is_dir = S_ISDIR(st.st_mode);
        *is_file = S_ISREG(st.st_mode);
        return 0;
    }
    
    // struct dirent access - readdir returns struct dirent*
    int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir) {
        struct dirent* ent = readdir((DIR*)dir);
        if (!ent) return 0;
        strncpy(out_name, ent->d_name, buf_size - 1);
        out_name[buf_size - 1] = 0;
#if defined(_DIRENT_HAVE_D_TYPE) || defined(DT_DIR)
        *is_dir = (ent->d_type == DT_DIR);
#else
        *is_dir = 0; // Fallback for Windows/MinGW if d_type missing
#endif
        return 1;
    }
 
    // mkdir has different signatures on Windows vs POSIX
    int _z_fs_mkdir(const char* path) {
        #ifdef _WIN32
            return mkdir(path);
        #else
            return mkdir(path, 0777);
        #endif
    }


    static uint64_t _time_now_impl(void) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
    }

    static int64_t _z_time_time(void) {
        return (int64_t)time(NULL);
    }


    Vec__JsonValuePtr Vec__JsonValuePtr__new();
    void Vec__JsonValuePtr__push(Vec__JsonValuePtr* self, JsonValue* item);
    Map__JsonValuePtr Map__JsonValuePtr__new();
    void Map__JsonValuePtr__put(Map__JsonValuePtr* self, char* key, JsonValue* val);
    
    static void _json_skip_ws(const char** p) {
        while (**p == ' ' || **p == '\t' || **p == '\n' || **p == '\r') (*p)++;
    }
    
    static char* _json_parse_string(const char** p) {
        if (**p != '"') return NULL;
        (*p)++;
        char buf[4096]; int len = 0;
        while (**p && **p != '"') {
            if (**p == '\\' && (*p)[1]) {
                (*p)++;
                char c = **p;
                if (c == 'n') buf[len++] = '\n';
                else if (c == 't') buf[len++] = '\t';
                else if (c == 'r') buf[len++] = '\r';
                else if (c == '"') buf[len++] = '"';
                else if (c == '\\') buf[len++] = '\\';
                else buf[len++] = c;
            } else {
                buf[len++] = **p;
            }
            (*p)++;
        }
        if (**p == '"') (*p)++;
        buf[len] = '\0';
        return strdup(buf);
    }
    
    static struct JsonValue* _json_parse_value(const char** p);
    
    static struct JsonValue* _json_parse_array(const char** p) {
        if (**p != '[') return NULL;
        (*p)++;
        struct JsonValue* arr = malloc(sizeof(struct JsonValue));
        arr->kind = JsonType__JSON_ARRAY();
        arr->string_val = 0; arr->number_val = 0; arr->bool_val = 0; arr->object_val = 0;
        arr->array_val = malloc(sizeof(Vec__JsonValuePtr));
        *(arr->array_val) = Vec__JsonValuePtr__new();
        
        _json_skip_ws(p);
        if (**p == ']') { (*p)++; return arr; }
        
        while (1) {
            _json_skip_ws(p);
            struct JsonValue* val = _json_parse_value(p);
            if (!val) return NULL;
            Vec__JsonValuePtr__push(arr->array_val, val);
            _json_skip_ws(p);
            if (**p == ']') { (*p)++; return arr; }
            if (**p != ',') return NULL;
            (*p)++;
        }
    }
    
    static struct JsonValue* _json_parse_object(const char** p) {
        if (**p != '{') return NULL;
        (*p)++;
        struct JsonValue* obj = malloc(sizeof(struct JsonValue));
        obj->kind = JsonType__JSON_OBJECT();
        obj->string_val = 0; obj->number_val = 0; obj->bool_val = 0; obj->array_val = 0;
        obj->object_val = malloc(sizeof(Map__JsonValuePtr));
        *(obj->object_val) = Map__JsonValuePtr__new();
        
        _json_skip_ws(p);
        if (**p == '}') { (*p)++; return obj; }
        
        while (1) {
            _json_skip_ws(p);
            char* key = _json_parse_string(p);
            if (!key) return NULL;
            _json_skip_ws(p);
            if (**p != ':') { free(key); return NULL; }
            (*p)++;
            _json_skip_ws(p);
            struct JsonValue* val = _json_parse_value(p);
            if (!val) { free(key); return NULL; }
            Map__JsonValuePtr__put(obj->object_val, key, val);
            free(key);
            _json_skip_ws(p);
            if (**p == '}') { (*p)++; return obj; }
            if (**p != ',') return NULL;
            (*p)++;
        }
    }
    
    static struct JsonValue* _json_parse_value(const char** p) {
        _json_skip_ws(p);
        if (**p == '\0') return NULL;
        
        if (strncmp(*p, "null", 4) == 0) {
            *p += 4;
            struct JsonValue* v = malloc(sizeof(struct JsonValue));
            v->kind = JsonType__JSON_NULL();
            v->string_val = 0; v->number_val = 0; v->bool_val = 0; v->array_val = 0; v->object_val = 0;
            return v;
        }
        if (strncmp(*p, "true", 4) == 0) {
            *p += 4;
            struct JsonValue* v = malloc(sizeof(struct JsonValue));
            v->kind = JsonType__JSON_BOOL();
            v->string_val = 0; v->number_val = 0; v->bool_val = 1; v->array_val = 0; v->object_val = 0;
            return v;
        }
        if (strncmp(*p, "false", 5) == 0) {
            *p += 5;
            struct JsonValue* v = malloc(sizeof(struct JsonValue));
            v->kind = JsonType__JSON_BOOL();
            v->string_val = 0; v->number_val = 0; v->bool_val = 0; v->array_val = 0; v->object_val = 0;
            return v;
        }
        if (**p == '"') {
            char* s = _json_parse_string(p);
            if (!s) return NULL;
            struct JsonValue* v = malloc(sizeof(struct JsonValue));
            v->kind = JsonType__JSON_STRING();
            v->string_val = s; v->number_val = 0; v->bool_val = 0; v->array_val = 0; v->object_val = 0;
            return v;
        }
        if (**p == '-' || (**p >= '0' && **p <= '9')) {
            char* end;
            double num = strtod(*p, &end);
            if (end == *p) return NULL;
            *p = end;
            struct JsonValue* v = malloc(sizeof(struct JsonValue));
            v->kind = JsonType__JSON_NUMBER();
            v->string_val = 0; v->number_val = num; v->bool_val = 0; v->array_val = 0; v->object_val = 0;
            return v;
        }
        if (**p == '[') return _json_parse_array(p);
        if (**p == '{') return _json_parse_object(p);
        return NULL;
    }
    
    struct JsonValue* _json_do_parse(const char* json) {
        const char* ptr = json;
        return _json_parse_value(&ptr);
    }


    // Cross-platform environment variable handling
    int _z_setenv(const char* name, const char* value, int overwrite) {
#ifdef _WIN32
        if (!overwrite && getenv(name) != NULL) return 0;
        return _putenv_s(name, value);
#else
        return setenv(name, value, overwrite);
#endif
    }

    int _z_unsetenv(const char* name) {
#ifdef _WIN32
        char* buf = (char*)malloc(strlen(name) + 2);
        if (!buf) return -1;
        sprintf(buf, "%s=", name);
        int ret = _putenv(buf);
        free(buf);
        return ret;
#else
        return unsetenv(name);
#endif
    }


    // Include TRE implementation for static linking.
    // This ensures linking works without precompiled libraries across all platforms (Windows/Linux/macOS).
    #include "std/third-party/tre/tre_full.c"


    // Minimal raw block: only for opaque FILE* types
    // popen/pclose use FILE* which conflicts with void*
    
    void *_z_popen(const char *command, const char *type) {
#ifdef _WIN32
        return (void *)_popen(command, type);
#else
        return (void *)popen(command, type);
#endif
    }
    
    int _z_pclose(void *stream) {
#ifdef _WIN32
        return _pclose((FILE *)stream);
#else
        return pclose((FILE *)stream);
#endif
    }

Clone Clone__clone(Clone* self) {
    void* res = self->vtable->clone(self->self);
    return (Clone){.self = res, .vtable = self->vtable};
}



void Drop__drop(Drop* self) {
    return self->vtable->drop(self->self);
}


Option__Match Option__Match__Some(Match v);
Option__Match Option__Match__None(void);
bool Option__Match__is_some(Option__Match* self);
bool Option__Match__is_none(Option__Match* self);
void Option__Match__forget(Option__Match* self);
Match Option__Match__unwrap(Option__Match* self);
Match* Option__Match__unwrap_ref(Option__Match* self);
Match Option__Match__unwrap_or(Option__Match* self, Match def_val);
Match Option__Match__expect(Option__Match* self, char* msg);
Option__Match Option__Match__or_else(Option__Match* self, Option__Match other);
Option__string Option__string__Some(string v);
Option__string Option__string__None(void);
bool Option__string__is_some(Option__string* self);
bool Option__string__is_none(Option__string* self);
void Option__string__forget(Option__string* self);
string Option__string__unwrap(Option__string* self);
string* Option__string__unwrap_ref(Option__string* self);
string Option__string__unwrap_or(Option__string* self, string def_val);
string Option__string__expect(Option__string* self, char* msg);
Option__string Option__string__or_else(Option__string* self, Option__string other);
Option__Path Option__Path__Some(Path v);
Option__Path Option__Path__None(void);
bool Option__Path__is_some(Option__Path* self);
bool Option__Path__is_none(Option__Path* self);
void Option__Path__forget(Option__Path* self);
Path Option__Path__unwrap(Option__Path* self);
Path* Option__Path__unwrap_ref(Option__Path* self);
Path Option__Path__unwrap_or(Option__Path* self, Path def_val);
Path Option__Path__expect(Option__Path* self, char* msg);
Option__Path Option__Path__or_else(Option__Path* self, Option__Path other);
Option__bool Option__bool__Some(bool v);
Option__bool Option__bool__None(void);
bool Option__bool__is_some(Option__bool* self);
bool Option__bool__is_none(Option__bool* self);
void Option__bool__forget(Option__bool* self);
bool Option__bool__unwrap(Option__bool* self);
bool* Option__bool__unwrap_ref(Option__bool* self);
bool Option__bool__unwrap_or(Option__bool* self, bool def_val);
bool Option__bool__expect(Option__bool* self, char* msg);
Option__bool Option__bool__or_else(Option__bool* self, Option__bool other);
Option__double Option__double__Some(double v);
Option__double Option__double__None(void);
bool Option__double__is_some(Option__double* self);
bool Option__double__is_none(Option__double* self);
void Option__double__forget(Option__double* self);
double Option__double__unwrap(Option__double* self);
double* Option__double__unwrap_ref(Option__double* self);
double Option__double__unwrap_or(Option__double* self, double def_val);
double Option__double__expect(Option__double* self, char* msg);
Option__double Option__double__or_else(Option__double* self, Option__double other);
Option__charPtr Option__charPtr__Some(char* v);
Option__charPtr Option__charPtr__None(void);
bool Option__charPtr__is_some(Option__charPtr* self);
bool Option__charPtr__is_none(Option__charPtr* self);
void Option__charPtr__forget(Option__charPtr* self);
char* Option__charPtr__unwrap(Option__charPtr* self);
char** Option__charPtr__unwrap_ref(Option__charPtr* self);
char* Option__charPtr__unwrap_or(Option__charPtr* self, char* def_val);
char* Option__charPtr__expect(Option__charPtr* self, char* msg);
Option__charPtr Option__charPtr__or_else(Option__charPtr* self, Option__charPtr other);
Result__JsonValue Result__JsonValue__Ok(JsonValue v);
Result__JsonValue Result__JsonValue__Err(char* e);
bool Result__JsonValue__is_ok(Result__JsonValue* self);
bool Result__JsonValue__is_err(Result__JsonValue* self);
void Result__JsonValue__forget(Result__JsonValue* self);
JsonValue Result__JsonValue__unwrap(Result__JsonValue* self);
JsonValue* Result__JsonValue__unwrap_ref(Result__JsonValue* self);
JsonValue Result__JsonValue__expect(Result__JsonValue* self, char* msg);
Result__JsonValuePtr Result__JsonValuePtr__Ok(JsonValue* v);
Result__JsonValuePtr Result__JsonValuePtr__Err(char* e);
bool Result__JsonValuePtr__is_ok(Result__JsonValuePtr* self);
bool Result__JsonValuePtr__is_err(Result__JsonValuePtr* self);
void Result__JsonValuePtr__forget(Result__JsonValuePtr* self);
JsonValue* Result__JsonValuePtr__unwrap(Result__JsonValuePtr* self);
JsonValue** Result__JsonValuePtr__unwrap_ref(Result__JsonValuePtr* self);
JsonValue* Result__JsonValuePtr__expect(Result__JsonValuePtr* self, char* msg);
Map__JsonValuePtr Map__JsonValuePtr__new(void);
void Map__JsonValuePtr__resize(Map__JsonValuePtr* self, size_t new_cap);
void Map__JsonValuePtr__put(Map__JsonValuePtr* self, char* key, JsonValue* val);
Option__JsonValuePtr Map__JsonValuePtr__get(Map__JsonValuePtr* self, char* key);
bool Map__JsonValuePtr__contains(Map__JsonValuePtr* self, char* key);
void Map__JsonValuePtr__remove(Map__JsonValuePtr* self, char* key);
size_t Map__JsonValuePtr__length(Map__JsonValuePtr* self);
bool Map__JsonValuePtr__is_empty(Map__JsonValuePtr* self);
void Map__JsonValuePtr__free(Map__JsonValuePtr* self);
size_t Map__JsonValuePtr__capacity(Map__JsonValuePtr* self);
bool Map__JsonValuePtr__is_slot_occupied(Map__JsonValuePtr* self, size_t idx);
char* Map__JsonValuePtr__key_at(Map__JsonValuePtr* self, size_t idx);
JsonValue* Map__JsonValuePtr__val_at(Map__JsonValuePtr* self, size_t idx);
MapIter__JsonValuePtr Map__JsonValuePtr__iterator(Map__JsonValuePtr* self);
MapIterResult__JsonValuePtr MapIter__JsonValuePtr__next(MapIter__JsonValuePtr* self);
bool MapIterResult__JsonValuePtr__is_none(MapIterResult__JsonValuePtr* self);
MapEntry__JsonValuePtr MapIterResult__JsonValuePtr__unwrap(MapIterResult__JsonValuePtr* self);
void Map__JsonValuePtr__Drop__drop(Map__JsonValuePtr* self);
Vec__JsonValuePtr Vec__JsonValuePtr__new(void);
Vec__JsonValuePtr Vec__JsonValuePtr__with_capacity(size_t cap);
void Vec__JsonValuePtr__grow(Vec__JsonValuePtr* self);
void Vec__JsonValuePtr__grow_to_fit(Vec__JsonValuePtr* self, size_t new_len);
VecIter__JsonValuePtr Vec__JsonValuePtr__iterator(Vec__JsonValuePtr* self);
VecIterRef__JsonValuePtr Vec__JsonValuePtr__iter_ref(Vec__JsonValuePtr* self);
void Vec__JsonValuePtr__push(Vec__JsonValuePtr* self, JsonValue* item);
void Vec__JsonValuePtr__insert(Vec__JsonValuePtr* self, size_t idx, JsonValue* item);
JsonValue* Vec__JsonValuePtr__pop(Vec__JsonValuePtr* self);
Option__JsonValuePtr Vec__JsonValuePtr__pop_opt(Vec__JsonValuePtr* self);
JsonValue* Vec__JsonValuePtr__remove(Vec__JsonValuePtr* self, size_t idx);
void Vec__JsonValuePtr__append(Vec__JsonValuePtr* self, Vec__JsonValuePtr other);
JsonValue* Vec__JsonValuePtr__get(Vec__JsonValuePtr* self, size_t idx);
JsonValue* Vec__JsonValuePtr__index(Vec__JsonValuePtr* self, size_t idx);
JsonValue** Vec__JsonValuePtr__get_ref(Vec__JsonValuePtr* self, size_t idx);
JsonValue* Vec__JsonValuePtr__last(Vec__JsonValuePtr* self);
size_t Vec__JsonValuePtr__length(Vec__JsonValuePtr* self);
bool Vec__JsonValuePtr__contains(Vec__JsonValuePtr* self, JsonValue* item);
bool Vec__JsonValuePtr__is_empty(Vec__JsonValuePtr* self);
void Vec__JsonValuePtr__clear(Vec__JsonValuePtr* self);
void Vec__JsonValuePtr__free(Vec__JsonValuePtr* self);
JsonValue* Vec__JsonValuePtr__first(Vec__JsonValuePtr* self);
void Vec__JsonValuePtr__set(Vec__JsonValuePtr* self, size_t idx, JsonValue* item);
void Vec__JsonValuePtr__reverse(Vec__JsonValuePtr* self);
bool Vec__JsonValuePtr__eq(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other);
void Vec__JsonValuePtr__forget(Vec__JsonValuePtr* self);
Vec__JsonValuePtr Vec__JsonValuePtr__add(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other);
void Vec__JsonValuePtr__add_assign(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other);
bool Vec__JsonValuePtr__neq(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other);
void Vec__JsonValuePtr__shl(Vec__JsonValuePtr* self, JsonValue* item);
void Vec__JsonValuePtr__shr(Vec__JsonValuePtr* self, JsonValue** out_item);
Vec__JsonValuePtr Vec__JsonValuePtr__mul(Vec__JsonValuePtr* self, size_t count);
void Vec__JsonValuePtr__mul_assign(Vec__JsonValuePtr* self, size_t count);
Vec__JsonValuePtr Vec__JsonValuePtr__clone(Vec__JsonValuePtr* self);
VecIterResult__JsonValuePtr VecIterRef__JsonValuePtr__next(VecIterRef__JsonValuePtr* self);
VecIterRef__JsonValuePtr VecIterRef__JsonValuePtr__iterator(VecIterRef__JsonValuePtr* self);
bool VecIterResult__JsonValuePtr__is_none(VecIterResult__JsonValuePtr* self);
struct JsonValue** VecIterResult__JsonValuePtr__unwrap(VecIterResult__JsonValuePtr* self);
Option__JsonValuePtr VecIter__JsonValuePtr__next(VecIter__JsonValuePtr* self);
VecIter__JsonValuePtr VecIter__JsonValuePtr__iterator(VecIter__JsonValuePtr* self);
Option__JsonValuePtr Option__JsonValuePtr__Some(struct JsonValue* v);
Option__JsonValuePtr Option__JsonValuePtr__None(void);
bool Option__JsonValuePtr__is_some(Option__JsonValuePtr* self);
bool Option__JsonValuePtr__is_none(Option__JsonValuePtr* self);
void Option__JsonValuePtr__forget(Option__JsonValuePtr* self);
struct JsonValue* Option__JsonValuePtr__unwrap(Option__JsonValuePtr* self);
struct JsonValue** Option__JsonValuePtr__unwrap_ref(Option__JsonValuePtr* self);
struct JsonValue* Option__JsonValuePtr__unwrap_or(Option__JsonValuePtr* self, struct JsonValue* def_val);
struct JsonValue* Option__JsonValuePtr__expect(Option__JsonValuePtr* self, char* msg);
Option__JsonValuePtr Option__JsonValuePtr__or_else(Option__JsonValuePtr* self, Option__JsonValuePtr other);
void Vec__JsonValuePtr__Drop__drop(Vec__JsonValuePtr* self);
Result__Vec__DirEntry Result__Vec__DirEntry__Ok(Vec__DirEntry v);
Result__Vec__DirEntry Result__Vec__DirEntry__Err(char* e);
bool Result__Vec__DirEntry__is_ok(Result__Vec__DirEntry* self);
bool Result__Vec__DirEntry__is_err(Result__Vec__DirEntry* self);
void Result__Vec__DirEntry__forget(Result__Vec__DirEntry* self);
Vec__DirEntry Result__Vec__DirEntry__unwrap(Result__Vec__DirEntry* self);
Vec__DirEntry* Result__Vec__DirEntry__unwrap_ref(Result__Vec__DirEntry* self);
Vec__DirEntry Result__Vec__DirEntry__expect(Result__Vec__DirEntry* self, char* msg);
Vec__DirEntryPtr Vec__DirEntryPtr__new(void);
Vec__DirEntryPtr Vec__DirEntryPtr__with_capacity(size_t cap);
void Vec__DirEntryPtr__grow(Vec__DirEntryPtr* self);
void Vec__DirEntryPtr__grow_to_fit(Vec__DirEntryPtr* self, size_t new_len);
VecIter__DirEntryPtr Vec__DirEntryPtr__iterator(Vec__DirEntryPtr* self);
VecIterRef__DirEntryPtr Vec__DirEntryPtr__iter_ref(Vec__DirEntryPtr* self);
void Vec__DirEntryPtr__push(Vec__DirEntryPtr* self, DirEntry* item);
void Vec__DirEntryPtr__insert(Vec__DirEntryPtr* self, size_t idx, DirEntry* item);
DirEntry* Vec__DirEntryPtr__pop(Vec__DirEntryPtr* self);
Option__DirEntryPtr Vec__DirEntryPtr__pop_opt(Vec__DirEntryPtr* self);
DirEntry* Vec__DirEntryPtr__remove(Vec__DirEntryPtr* self, size_t idx);
void Vec__DirEntryPtr__append(Vec__DirEntryPtr* self, Vec__DirEntryPtr other);
DirEntry* Vec__DirEntryPtr__get(Vec__DirEntryPtr* self, size_t idx);
DirEntry* Vec__DirEntryPtr__index(Vec__DirEntryPtr* self, size_t idx);
DirEntry** Vec__DirEntryPtr__get_ref(Vec__DirEntryPtr* self, size_t idx);
DirEntry* Vec__DirEntryPtr__last(Vec__DirEntryPtr* self);
size_t Vec__DirEntryPtr__length(Vec__DirEntryPtr* self);
bool Vec__DirEntryPtr__contains(Vec__DirEntryPtr* self, DirEntry* item);
bool Vec__DirEntryPtr__is_empty(Vec__DirEntryPtr* self);
void Vec__DirEntryPtr__clear(Vec__DirEntryPtr* self);
void Vec__DirEntryPtr__free(Vec__DirEntryPtr* self);
DirEntry* Vec__DirEntryPtr__first(Vec__DirEntryPtr* self);
void Vec__DirEntryPtr__set(Vec__DirEntryPtr* self, size_t idx, DirEntry* item);
void Vec__DirEntryPtr__reverse(Vec__DirEntryPtr* self);
bool Vec__DirEntryPtr__eq(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other);
void Vec__DirEntryPtr__forget(Vec__DirEntryPtr* self);
Vec__DirEntryPtr Vec__DirEntryPtr__add(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other);
void Vec__DirEntryPtr__add_assign(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other);
bool Vec__DirEntryPtr__neq(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other);
void Vec__DirEntryPtr__shl(Vec__DirEntryPtr* self, DirEntry* item);
void Vec__DirEntryPtr__shr(Vec__DirEntryPtr* self, DirEntry** out_item);
Vec__DirEntryPtr Vec__DirEntryPtr__mul(Vec__DirEntryPtr* self, size_t count);
void Vec__DirEntryPtr__mul_assign(Vec__DirEntryPtr* self, size_t count);
Vec__DirEntryPtr Vec__DirEntryPtr__clone(Vec__DirEntryPtr* self);
VecIterResult__DirEntryPtr VecIterRef__DirEntryPtr__next(VecIterRef__DirEntryPtr* self);
VecIterRef__DirEntryPtr VecIterRef__DirEntryPtr__iterator(VecIterRef__DirEntryPtr* self);
bool VecIterResult__DirEntryPtr__is_none(VecIterResult__DirEntryPtr* self);
struct DirEntry** VecIterResult__DirEntryPtr__unwrap(VecIterResult__DirEntryPtr* self);
Option__DirEntryPtr VecIter__DirEntryPtr__next(VecIter__DirEntryPtr* self);
VecIter__DirEntryPtr VecIter__DirEntryPtr__iterator(VecIter__DirEntryPtr* self);
Option__DirEntryPtr Option__DirEntryPtr__Some(struct DirEntry* v);
Option__DirEntryPtr Option__DirEntryPtr__None(void);
bool Option__DirEntryPtr__is_some(Option__DirEntryPtr* self);
bool Option__DirEntryPtr__is_none(Option__DirEntryPtr* self);
void Option__DirEntryPtr__forget(Option__DirEntryPtr* self);
struct DirEntry* Option__DirEntryPtr__unwrap(Option__DirEntryPtr* self);
struct DirEntry** Option__DirEntryPtr__unwrap_ref(Option__DirEntryPtr* self);
struct DirEntry* Option__DirEntryPtr__unwrap_or(Option__DirEntryPtr* self, struct DirEntry* def_val);
struct DirEntry* Option__DirEntryPtr__expect(Option__DirEntryPtr* self, char* msg);
Option__DirEntryPtr Option__DirEntryPtr__or_else(Option__DirEntryPtr* self, Option__DirEntryPtr other);
void Vec__DirEntryPtr__Drop__drop(Vec__DirEntryPtr* self);
Vec__DirEntry Vec__DirEntry__new(void);
Vec__DirEntry Vec__DirEntry__with_capacity(size_t cap);
void Vec__DirEntry__grow(Vec__DirEntry* self);
void Vec__DirEntry__grow_to_fit(Vec__DirEntry* self, size_t new_len);
VecIter__DirEntry Vec__DirEntry__iterator(Vec__DirEntry* self);
VecIterRef__DirEntry Vec__DirEntry__iter_ref(Vec__DirEntry* self);
void Vec__DirEntry__push(Vec__DirEntry* self, DirEntry item);
void Vec__DirEntry__insert(Vec__DirEntry* self, size_t idx, DirEntry item);
DirEntry Vec__DirEntry__pop(Vec__DirEntry* self);
Option__DirEntry Vec__DirEntry__pop_opt(Vec__DirEntry* self);
DirEntry Vec__DirEntry__remove(Vec__DirEntry* self, size_t idx);
void Vec__DirEntry__append(Vec__DirEntry* self, Vec__DirEntry other);
DirEntry Vec__DirEntry__get(Vec__DirEntry* self, size_t idx);
DirEntry Vec__DirEntry__index(Vec__DirEntry* self, size_t idx);
DirEntry* Vec__DirEntry__get_ref(Vec__DirEntry* self, size_t idx);
DirEntry Vec__DirEntry__last(Vec__DirEntry* self);
size_t Vec__DirEntry__length(Vec__DirEntry* self);
bool Vec__DirEntry__contains(Vec__DirEntry* self, DirEntry item);
bool Vec__DirEntry__is_empty(Vec__DirEntry* self);
void Vec__DirEntry__clear(Vec__DirEntry* self);
void Vec__DirEntry__free(Vec__DirEntry* self);
DirEntry Vec__DirEntry__first(Vec__DirEntry* self);
void Vec__DirEntry__set(Vec__DirEntry* self, size_t idx, DirEntry item);
void Vec__DirEntry__reverse(Vec__DirEntry* self);
bool Vec__DirEntry__eq(Vec__DirEntry* self, Vec__DirEntry* other);
void Vec__DirEntry__forget(Vec__DirEntry* self);
Vec__DirEntry Vec__DirEntry__add(Vec__DirEntry* self, Vec__DirEntry* other);
void Vec__DirEntry__add_assign(Vec__DirEntry* self, Vec__DirEntry* other);
bool Vec__DirEntry__neq(Vec__DirEntry* self, Vec__DirEntry* other);
void Vec__DirEntry__shl(Vec__DirEntry* self, DirEntry item);
void Vec__DirEntry__shr(Vec__DirEntry* self, DirEntry* out_item);
Vec__DirEntry Vec__DirEntry__mul(Vec__DirEntry* self, size_t count);
void Vec__DirEntry__mul_assign(Vec__DirEntry* self, size_t count);
Vec__DirEntry Vec__DirEntry__clone(Vec__DirEntry* self);
VecIterResult__DirEntry VecIterRef__DirEntry__next(VecIterRef__DirEntry* self);
VecIterRef__DirEntry VecIterRef__DirEntry__iterator(VecIterRef__DirEntry* self);
bool VecIterResult__DirEntry__is_none(VecIterResult__DirEntry* self);
DirEntry* VecIterResult__DirEntry__unwrap(VecIterResult__DirEntry* self);
Option__DirEntry VecIter__DirEntry__next(VecIter__DirEntry* self);
VecIter__DirEntry VecIter__DirEntry__iterator(VecIter__DirEntry* self);
Option__DirEntry Option__DirEntry__Some(DirEntry v);
Option__DirEntry Option__DirEntry__None(void);
bool Option__DirEntry__is_some(Option__DirEntry* self);
bool Option__DirEntry__is_none(Option__DirEntry* self);
void Option__DirEntry__forget(Option__DirEntry* self);
DirEntry Option__DirEntry__unwrap(Option__DirEntry* self);
DirEntry* Option__DirEntry__unwrap_ref(Option__DirEntry* self);
DirEntry Option__DirEntry__unwrap_or(Option__DirEntry* self, DirEntry def_val);
DirEntry Option__DirEntry__expect(Option__DirEntry* self, char* msg);
Option__DirEntry Option__DirEntry__or_else(Option__DirEntry* self, Option__DirEntry other);
void Vec__DirEntry__Drop__drop(Vec__DirEntry* self);
Result__Metadata Result__Metadata__Ok(Metadata v);
Result__Metadata Result__Metadata__Err(char* e);
bool Result__Metadata__is_ok(Result__Metadata* self);
bool Result__Metadata__is_err(Result__Metadata* self);
void Result__Metadata__forget(Result__Metadata* self);
Metadata Result__Metadata__unwrap(Result__Metadata* self);
Metadata* Result__Metadata__unwrap_ref(Result__Metadata* self);
Metadata Result__Metadata__expect(Result__Metadata* self, char* msg);
Result__Vec__String Result__Vec__String__Ok(Vec__String v);
Result__Vec__String Result__Vec__String__Err(char* e);
bool Result__Vec__String__is_ok(Result__Vec__String* self);
bool Result__Vec__String__is_err(Result__Vec__String* self);
void Result__Vec__String__forget(Result__Vec__String* self);
Vec__String Result__Vec__String__unwrap(Result__Vec__String* self);
Vec__String* Result__Vec__String__unwrap_ref(Result__Vec__String* self);
Vec__String Result__Vec__String__expect(Result__Vec__String* self, char* msg);
Vec__StringPtr Vec__StringPtr__new(void);
Vec__StringPtr Vec__StringPtr__with_capacity(size_t cap);
void Vec__StringPtr__grow(Vec__StringPtr* self);
void Vec__StringPtr__grow_to_fit(Vec__StringPtr* self, size_t new_len);
VecIter__StringPtr Vec__StringPtr__iterator(Vec__StringPtr* self);
VecIterRef__StringPtr Vec__StringPtr__iter_ref(Vec__StringPtr* self);
void Vec__StringPtr__push(Vec__StringPtr* self, String* item);
void Vec__StringPtr__insert(Vec__StringPtr* self, size_t idx, String* item);
String* Vec__StringPtr__pop(Vec__StringPtr* self);
Option__StringPtr Vec__StringPtr__pop_opt(Vec__StringPtr* self);
String* Vec__StringPtr__remove(Vec__StringPtr* self, size_t idx);
void Vec__StringPtr__append(Vec__StringPtr* self, Vec__StringPtr other);
String* Vec__StringPtr__get(Vec__StringPtr* self, size_t idx);
String* Vec__StringPtr__index(Vec__StringPtr* self, size_t idx);
String** Vec__StringPtr__get_ref(Vec__StringPtr* self, size_t idx);
String* Vec__StringPtr__last(Vec__StringPtr* self);
size_t Vec__StringPtr__length(Vec__StringPtr* self);
bool Vec__StringPtr__contains(Vec__StringPtr* self, String* item);
bool Vec__StringPtr__is_empty(Vec__StringPtr* self);
void Vec__StringPtr__clear(Vec__StringPtr* self);
void Vec__StringPtr__free(Vec__StringPtr* self);
String* Vec__StringPtr__first(Vec__StringPtr* self);
void Vec__StringPtr__set(Vec__StringPtr* self, size_t idx, String* item);
void Vec__StringPtr__reverse(Vec__StringPtr* self);
bool Vec__StringPtr__eq(Vec__StringPtr* self, Vec__StringPtr* other);
void Vec__StringPtr__forget(Vec__StringPtr* self);
Vec__StringPtr Vec__StringPtr__add(Vec__StringPtr* self, Vec__StringPtr* other);
void Vec__StringPtr__add_assign(Vec__StringPtr* self, Vec__StringPtr* other);
bool Vec__StringPtr__neq(Vec__StringPtr* self, Vec__StringPtr* other);
void Vec__StringPtr__shl(Vec__StringPtr* self, String* item);
void Vec__StringPtr__shr(Vec__StringPtr* self, String** out_item);
Vec__StringPtr Vec__StringPtr__mul(Vec__StringPtr* self, size_t count);
void Vec__StringPtr__mul_assign(Vec__StringPtr* self, size_t count);
Vec__StringPtr Vec__StringPtr__clone(Vec__StringPtr* self);
VecIterResult__StringPtr VecIterRef__StringPtr__next(VecIterRef__StringPtr* self);
VecIterRef__StringPtr VecIterRef__StringPtr__iterator(VecIterRef__StringPtr* self);
bool VecIterResult__StringPtr__is_none(VecIterResult__StringPtr* self);
struct String** VecIterResult__StringPtr__unwrap(VecIterResult__StringPtr* self);
Option__StringPtr VecIter__StringPtr__next(VecIter__StringPtr* self);
VecIter__StringPtr VecIter__StringPtr__iterator(VecIter__StringPtr* self);
Option__StringPtr Option__StringPtr__Some(struct String* v);
Option__StringPtr Option__StringPtr__None(void);
bool Option__StringPtr__is_some(Option__StringPtr* self);
bool Option__StringPtr__is_none(Option__StringPtr* self);
void Option__StringPtr__forget(Option__StringPtr* self);
struct String* Option__StringPtr__unwrap(Option__StringPtr* self);
struct String** Option__StringPtr__unwrap_ref(Option__StringPtr* self);
struct String* Option__StringPtr__unwrap_or(Option__StringPtr* self, struct String* def_val);
struct String* Option__StringPtr__expect(Option__StringPtr* self, char* msg);
Option__StringPtr Option__StringPtr__or_else(Option__StringPtr* self, Option__StringPtr other);
void Vec__StringPtr__Drop__drop(Vec__StringPtr* self);
Result__bool Result__bool__Ok(bool v);
Result__bool Result__bool__Err(char* e);
bool Result__bool__is_ok(Result__bool* self);
bool Result__bool__is_err(Result__bool* self);
void Result__bool__forget(Result__bool* self);
bool Result__bool__unwrap(Result__bool* self);
bool* Result__bool__unwrap_ref(Result__bool* self);
bool Result__bool__expect(Result__bool* self, char* msg);
Result__String Result__String__Ok(String v);
Result__String Result__String__Err(char* e);
bool Result__String__is_ok(Result__String* self);
bool Result__String__is_err(Result__String* self);
void Result__String__forget(Result__String* self);
String Result__String__unwrap(Result__String* self);
String* Result__String__unwrap_ref(Result__String* self);
String Result__String__expect(Result__String* self, char* msg);
Result__File Result__File__Ok(File v);
Result__File Result__File__Err(char* e);
bool Result__File__is_ok(Result__File* self);
bool Result__File__is_err(Result__File* self);
void Result__File__forget(Result__File* self);
File Result__File__unwrap(Result__File* self);
File* Result__File__unwrap_ref(Result__File* self);
File Result__File__expect(Result__File* self, char* msg);
Vec__String Vec__String__new(void);
Vec__String Vec__String__with_capacity(size_t cap);
void Vec__String__grow(Vec__String* self);
void Vec__String__grow_to_fit(Vec__String* self, size_t new_len);
VecIter__String Vec__String__iterator(Vec__String* self);
VecIterRef__String Vec__String__iter_ref(Vec__String* self);
void Vec__String__push(Vec__String* self, String item);
void Vec__String__insert(Vec__String* self, size_t idx, String item);
String Vec__String__pop(Vec__String* self);
Option__String Vec__String__pop_opt(Vec__String* self);
String Vec__String__remove(Vec__String* self, size_t idx);
void Vec__String__append(Vec__String* self, Vec__String other);
String Vec__String__get(Vec__String* self, size_t idx);
String Vec__String__index(Vec__String* self, size_t idx);
String* Vec__String__get_ref(Vec__String* self, size_t idx);
String Vec__String__last(Vec__String* self);
size_t Vec__String__length(Vec__String* self);
bool Vec__String__contains(Vec__String* self, String item);
bool Vec__String__is_empty(Vec__String* self);
void Vec__String__clear(Vec__String* self);
void Vec__String__free(Vec__String* self);
String Vec__String__first(Vec__String* self);
void Vec__String__set(Vec__String* self, size_t idx, String item);
void Vec__String__reverse(Vec__String* self);
bool Vec__String__eq(Vec__String* self, Vec__String* other);
void Vec__String__forget(Vec__String* self);
Vec__String Vec__String__add(Vec__String* self, Vec__String* other);
void Vec__String__add_assign(Vec__String* self, Vec__String* other);
bool Vec__String__neq(Vec__String* self, Vec__String* other);
void Vec__String__shl(Vec__String* self, String item);
void Vec__String__shr(Vec__String* self, String* out_item);
Vec__String Vec__String__mul(Vec__String* self, size_t count);
void Vec__String__mul_assign(Vec__String* self, size_t count);
Vec__String Vec__String__clone(Vec__String* self);
VecIterResult__String VecIterRef__String__next(VecIterRef__String* self);
VecIterRef__String VecIterRef__String__iterator(VecIterRef__String* self);
bool VecIterResult__String__is_none(VecIterResult__String* self);
String* VecIterResult__String__unwrap(VecIterResult__String* self);
Option__String VecIter__String__next(VecIter__String* self);
VecIter__String VecIter__String__iterator(VecIter__String* self);
Option__String Option__String__Some(String v);
Option__String Option__String__None(void);
bool Option__String__is_some(Option__String* self);
bool Option__String__is_none(Option__String* self);
void Option__String__forget(Option__String* self);
String Option__String__unwrap(Option__String* self);
String* Option__String__unwrap_ref(Option__String* self);
String Option__String__unwrap_or(Option__String* self, String def_val);
String Option__String__expect(Option__String* self, char* msg);
Option__String Option__String__or_else(Option__String* self, Option__String other);
void Vec__String__Drop__drop(Vec__String* self);
Vec__int32_t Vec__int32_t__new(void);
Vec__int32_t Vec__int32_t__with_capacity(size_t cap);
void Vec__int32_t__grow(Vec__int32_t* self);
void Vec__int32_t__grow_to_fit(Vec__int32_t* self, size_t new_len);
VecIter__int32_t Vec__int32_t__iterator(Vec__int32_t* self);
VecIterRef__int32_t Vec__int32_t__iter_ref(Vec__int32_t* self);
void Vec__int32_t__push(Vec__int32_t* self, int32_t item);
void Vec__int32_t__insert(Vec__int32_t* self, size_t idx, int32_t item);
int32_t Vec__int32_t__pop(Vec__int32_t* self);
Option__int32_t Vec__int32_t__pop_opt(Vec__int32_t* self);
int32_t Vec__int32_t__remove(Vec__int32_t* self, size_t idx);
void Vec__int32_t__append(Vec__int32_t* self, Vec__int32_t other);
int32_t Vec__int32_t__get(Vec__int32_t* self, size_t idx);
int32_t Vec__int32_t__index(Vec__int32_t* self, size_t idx);
int32_t* Vec__int32_t__get_ref(Vec__int32_t* self, size_t idx);
int32_t Vec__int32_t__last(Vec__int32_t* self);
size_t Vec__int32_t__length(Vec__int32_t* self);
bool Vec__int32_t__contains(Vec__int32_t* self, int32_t item);
bool Vec__int32_t__is_empty(Vec__int32_t* self);
void Vec__int32_t__clear(Vec__int32_t* self);
void Vec__int32_t__free(Vec__int32_t* self);
int32_t Vec__int32_t__first(Vec__int32_t* self);
void Vec__int32_t__set(Vec__int32_t* self, size_t idx, int32_t item);
void Vec__int32_t__reverse(Vec__int32_t* self);
bool Vec__int32_t__eq(Vec__int32_t* self, Vec__int32_t* other);
void Vec__int32_t__forget(Vec__int32_t* self);
Vec__int32_t Vec__int32_t__add(Vec__int32_t* self, Vec__int32_t* other);
void Vec__int32_t__add_assign(Vec__int32_t* self, Vec__int32_t* other);
bool Vec__int32_t__neq(Vec__int32_t* self, Vec__int32_t* other);
void Vec__int32_t__shl(Vec__int32_t* self, int32_t item);
void Vec__int32_t__shr(Vec__int32_t* self, int32_t* out_item);
Vec__int32_t Vec__int32_t__mul(Vec__int32_t* self, size_t count);
void Vec__int32_t__mul_assign(Vec__int32_t* self, size_t count);
Vec__int32_t Vec__int32_t__clone(Vec__int32_t* self);
VecIterResult__int32_t VecIterRef__int32_t__next(VecIterRef__int32_t* self);
VecIterRef__int32_t VecIterRef__int32_t__iterator(VecIterRef__int32_t* self);
bool VecIterResult__int32_t__is_none(VecIterResult__int32_t* self);
int32_t* VecIterResult__int32_t__unwrap(VecIterResult__int32_t* self);
Option__int32_t VecIter__int32_t__next(VecIter__int32_t* self);
VecIter__int32_t VecIter__int32_t__iterator(VecIter__int32_t* self);
void Vec__int32_t__Drop__drop(Vec__int32_t* self);
Vec__size_t Vec__size_t__new(void);
Vec__size_t Vec__size_t__with_capacity(size_t cap);
void Vec__size_t__grow(Vec__size_t* self);
void Vec__size_t__grow_to_fit(Vec__size_t* self, size_t new_len);
VecIter__size_t Vec__size_t__iterator(Vec__size_t* self);
VecIterRef__size_t Vec__size_t__iter_ref(Vec__size_t* self);
void Vec__size_t__push(Vec__size_t* self, size_t item);
void Vec__size_t__insert(Vec__size_t* self, size_t idx, size_t item);
size_t Vec__size_t__pop(Vec__size_t* self);
Option__size_t Vec__size_t__pop_opt(Vec__size_t* self);
size_t Vec__size_t__remove(Vec__size_t* self, size_t idx);
void Vec__size_t__append(Vec__size_t* self, Vec__size_t other);
size_t Vec__size_t__get(Vec__size_t* self, size_t idx);
size_t Vec__size_t__index(Vec__size_t* self, size_t idx);
size_t* Vec__size_t__get_ref(Vec__size_t* self, size_t idx);
size_t Vec__size_t__last(Vec__size_t* self);
size_t Vec__size_t__length(Vec__size_t* self);
bool Vec__size_t__contains(Vec__size_t* self, size_t item);
bool Vec__size_t__is_empty(Vec__size_t* self);
void Vec__size_t__clear(Vec__size_t* self);
void Vec__size_t__free(Vec__size_t* self);
size_t Vec__size_t__first(Vec__size_t* self);
void Vec__size_t__set(Vec__size_t* self, size_t idx, size_t item);
void Vec__size_t__reverse(Vec__size_t* self);
bool Vec__size_t__eq(Vec__size_t* self, Vec__size_t* other);
void Vec__size_t__forget(Vec__size_t* self);
Vec__size_t Vec__size_t__add(Vec__size_t* self, Vec__size_t* other);
void Vec__size_t__add_assign(Vec__size_t* self, Vec__size_t* other);
bool Vec__size_t__neq(Vec__size_t* self, Vec__size_t* other);
void Vec__size_t__shl(Vec__size_t* self, size_t item);
void Vec__size_t__shr(Vec__size_t* self, size_t* out_item);
Vec__size_t Vec__size_t__mul(Vec__size_t* self, size_t count);
void Vec__size_t__mul_assign(Vec__size_t* self, size_t count);
Vec__size_t Vec__size_t__clone(Vec__size_t* self);
VecIterResult__size_t VecIterRef__size_t__next(VecIterRef__size_t* self);
VecIterRef__size_t VecIterRef__size_t__iterator(VecIterRef__size_t* self);
bool VecIterResult__size_t__is_none(VecIterResult__size_t* self);
size_t* VecIterResult__size_t__unwrap(VecIterResult__size_t* self);
Option__size_t VecIter__size_t__next(VecIter__size_t* self);
VecIter__size_t VecIter__size_t__iterator(VecIter__size_t* self);
void Vec__size_t__Drop__drop(Vec__size_t* self);
Option__size_t Option__size_t__Some(size_t v);
Option__size_t Option__size_t__None(void);
bool Option__size_t__is_some(Option__size_t* self);
bool Option__size_t__is_none(Option__size_t* self);
void Option__size_t__forget(Option__size_t* self);
size_t Option__size_t__unwrap(Option__size_t* self);
size_t* Option__size_t__unwrap_ref(Option__size_t* self);
size_t Option__size_t__unwrap_or(Option__size_t* self, size_t def_val);
size_t Option__size_t__expect(Option__size_t* self, char* msg);
Option__size_t Option__size_t__or_else(Option__size_t* self, Option__size_t other);
Option__int32_t Option__int32_t__Some(int32_t v);
Option__int32_t Option__int32_t__None(void);
bool Option__int32_t__is_some(Option__int32_t* self);
bool Option__int32_t__is_none(Option__int32_t* self);
void Option__int32_t__forget(Option__int32_t* self);
int32_t Option__int32_t__unwrap(Option__int32_t* self);
int32_t* Option__int32_t__unwrap_ref(Option__int32_t* self);
int32_t Option__int32_t__unwrap_or(Option__int32_t* self, int32_t def_val);
int32_t Option__int32_t__expect(Option__int32_t* self, char* msg);
Option__int32_t Option__int32_t__or_else(Option__int32_t* self, Option__int32_t other);
Vec__char Vec__char__new(void);
Vec__char Vec__char__with_capacity(size_t cap);
void Vec__char__grow(Vec__char* self);
void Vec__char__grow_to_fit(Vec__char* self, size_t new_len);
VecIter__char Vec__char__iterator(Vec__char* self);
VecIterRef__char Vec__char__iter_ref(Vec__char* self);
void Vec__char__push(Vec__char* self, char item);
void Vec__char__insert(Vec__char* self, size_t idx, char item);
char Vec__char__pop(Vec__char* self);
Option__char Vec__char__pop_opt(Vec__char* self);
char Vec__char__remove(Vec__char* self, size_t idx);
void Vec__char__append(Vec__char* self, Vec__char other);
char Vec__char__get(Vec__char* self, size_t idx);
char Vec__char__index(Vec__char* self, size_t idx);
char* Vec__char__get_ref(Vec__char* self, size_t idx);
char Vec__char__last(Vec__char* self);
size_t Vec__char__length(Vec__char* self);
bool Vec__char__contains(Vec__char* self, char item);
bool Vec__char__is_empty(Vec__char* self);
void Vec__char__clear(Vec__char* self);
void Vec__char__free(Vec__char* self);
char Vec__char__first(Vec__char* self);
void Vec__char__set(Vec__char* self, size_t idx, char item);
void Vec__char__reverse(Vec__char* self);
bool Vec__char__eq(Vec__char* self, Vec__char* other);
void Vec__char__forget(Vec__char* self);
Vec__char Vec__char__add(Vec__char* self, Vec__char* other);
void Vec__char__add_assign(Vec__char* self, Vec__char* other);
bool Vec__char__neq(Vec__char* self, Vec__char* other);
void Vec__char__shl(Vec__char* self, char item);
void Vec__char__shr(Vec__char* self, char* out_item);
Vec__char Vec__char__mul(Vec__char* self, size_t count);
void Vec__char__mul_assign(Vec__char* self, size_t count);
Vec__char Vec__char__clone(Vec__char* self);
VecIterResult__char VecIterRef__char__next(VecIterRef__char* self);
VecIterRef__char VecIterRef__char__iterator(VecIterRef__char* self);
bool VecIterResult__char__is_none(VecIterResult__char* self);
char* VecIterResult__char__unwrap(VecIterResult__char* self);
Option__char VecIter__char__next(VecIter__char* self);
VecIter__char VecIter__char__iterator(VecIter__char* self);
Option__char Option__char__Some(char v);
Option__char Option__char__None(void);
bool Option__char__is_some(Option__char* self);
bool Option__char__is_none(Option__char* self);
void Option__char__forget(Option__char* self);
char Option__char__unwrap(Option__char* self);
char* Option__char__unwrap_ref(Option__char* self);
char Option__char__unwrap_or(Option__char* self, char def_val);
char Option__char__expect(Option__char* self, char* msg);
Option__char Option__char__or_else(Option__char* self, Option__char other);
void Vec__char__Drop__drop(Vec__char* self);
int _z_pclose(void* stream);
void* _z_popen(const char* command, const char* type);
Vec__String regex_split(char* pattern, char* text);
int32_t regex_count(char* pattern, char* text);
Option__Match regex_find(char* pattern, char* text);
bool regex_match(char* pattern, char* text);
int32_t _z_unsetenv(const char* name);
int32_t _z_setenv(const char* name, const char* value, int32_t overwrite);
size_t _map_hash_str(const char* str);
size_t hash_get_seed(void);
void hash_set_seed(size_t seed);
size_t hash_compute(const void* data, size_t len);
size_t hash_combine(size_t a, size_t b);
int64_t _z_time_time(void);
uint64_t _time_now_impl(void);
int32_t rand(void);
void srand(uint32_t seed);
int _z_fs_closedir(void* dir);
void* _z_fs_opendir(const char* name);
int _z_fs_read_entry(void* dir, char* out_name, int buf_size, int* is_dir);
int _z_fs_get_metadata(const char* path, uint64_t* size, int* is_dir, int* is_file);
int _z_fs_mkdir(const char* path);
char* _z_str_rune(int32_t r);
char* utos(uint32_t n);
char* itos_new(int32_t n);
char* itos(int32_t n);
int32_t read_rune(void);
size_t _utf8_seq_len(uint8_t first_byte);
char* readln(void);
int println(char* fmt, ...);
int print(char* fmt, ...);
char* format_new(char* fmt, ...);
int format_into(char* buffer, size_t size, char* fmt, ...);
char* format(char* fmt, ...);
int32_t strcasecmp(const char* s1, const char* s2);
void sort_double(double* arr, size_t len);
void sort_float(float* arr, size_t len);
void sort_long(long* arr, size_t len);
void sort_int(int32_t* arr, size_t len);
void __zenc_todo_impl(const char* file, int32_t line, const char* func, const char* msg);
void __zenc_panic_impl(const char* file, int32_t line, const char* func, const char* msg);
Command Command__new(char* program);
Command* Command__arg(Command* self, char* arg);
String Command__build_cmd(Command* self);
Output Command__output(Command* self);
int32_t Command__status(Command* self);
void Command__free(Command* self);
Regex Regex__compile(char* pattern);
Regex Regex__compile_with_flags(char* pattern, int32_t flags);
bool Regex__is_valid(Regex* self);
bool Regex__match(Regex* self, char* text);
bool Regex__match_full(Regex* self, char* text);
bool Regex__match_at(Regex* self, char* text, int32_t offset);
bool Regex__is_match(Regex* self, char* text);
Option__Match Regex__find(Regex* self, char* text);
Option__Match Regex__find_at(Regex* self, char* text, int32_t start);
int32_t Regex__count(Regex* self, char* text);
Vec__String Regex__split(Regex* self, char* text);
char* Regex__pattern(Regex* self);
int32_t Regex__flags(Regex* self);
bool Regex__is_valid_pattern(char* pattern);
void Regex__destroy(Regex* self);
Match Match__new(char* text, int32_t start, int32_t len);
char* Match__as_string(Match* self);
int32_t Match__end(Match* self);
Option__string Env__get(string name);
Option__String Env__get_dup(string name);
EnvRes Env__set(string name, string value);
EnvRes Env__unset(string name);
bool EnvRes__eq(EnvRes* self, EnvRes* other);
void Arena__Drop__drop(Arena* self);
Arena Arena__new(size_t cap);
void* Arena__alloc_bytes(Arena* self, size_t size);
char* Arena__dup_str(Arena* self, char* src);
size_t Arena__bytes_used(Arena* self);
size_t Arena__bytes_free(Arena* self);
size_t Arena__save(Arena* self);
void Arena__restore(Arena* self, size_t mark);
void Arena__reset(Arena* self);
void Arena__free(Arena* self);
Path Path__new(char* s);
Path Path__from_string(String s);
char* Path__c_str(Path* self);
Path Path__clone(Path* self);
Path Path__join(Path* self, char* other);
Option__String Path__extension(Path* self);
Option__String Path__file_name(Path* self);
Option__Path Path__parent(Path* self);
void Path__free(Path* self);
String JsonValue__to_string(JsonValue* self);
void JsonValue__stringify(JsonValue* self, String* buf);
void JsonValue__Drop__drop(JsonValue* self);
JsonValue JsonValue__null(void);
JsonValue JsonValue__bool(bool b);
JsonValue JsonValue__number(double n);
JsonValue JsonValue__string(char* s);
JsonValue JsonValue__array(void);
JsonValue JsonValue__object(void);
JsonValue* JsonValue__null_ptr(void);
JsonValue* JsonValue__bool_ptr(bool b);
JsonValue* JsonValue__number_ptr(double n);
JsonValue* JsonValue__string_ptr(char* s);
JsonValue* JsonValue__array_ptr(void);
JsonValue* JsonValue__object_ptr(void);
void JsonValue__push(JsonValue* self, JsonValue val);
void JsonValue__set(JsonValue* self, char* key, JsonValue val);
Result__JsonValuePtr JsonValue__parse(char* json);
Result__JsonValue JsonValue__parse_val(char* json);
bool JsonValue__is_null(JsonValue* self);
bool JsonValue__is_bool(JsonValue* self);
bool JsonValue__is_number(JsonValue* self);
bool JsonValue__is_string(JsonValue* self);
bool JsonValue__is_array(JsonValue* self);
bool JsonValue__is_object(JsonValue* self);
Option__charPtr JsonValue__as_string(JsonValue* self);
Option__int32_t JsonValue__as_int(JsonValue* self);
Option__double JsonValue__as_float(JsonValue* self);
Option__bool JsonValue__as_bool(JsonValue* self);
Option__JsonValuePtr JsonValue__get(JsonValue* self, char* key);
Option__charPtr JsonValue__get_string(JsonValue* self, char* key);
Option__int32_t JsonValue__get_int(JsonValue* self, char* key);
Option__double JsonValue__get_float(JsonValue* self, char* key);
Option__bool JsonValue__get_bool(JsonValue* self, char* key);
Option__JsonValuePtr JsonValue__get_object(JsonValue* self, char* key);
Option__JsonValuePtr JsonValue__get_array(JsonValue* self, char* key);
Option__JsonValuePtr JsonValue__at(JsonValue* self, size_t index);
size_t JsonValue__len(JsonValue* self);
void JsonValue__free(JsonValue* self);
bool JsonValue__eq(JsonValue* self, JsonValue* other);
bool JsonType__eq(JsonType* self, JsonType* other);
uint64_t Time__now(void);
void Time__sleep(Duration d);
void Time__sleep_ms(uint64_t ms);
Duration Duration__from_ms(uint64_t ms);
Duration Duration__from_secs(uint64_t s);
void File__Drop__drop(File* self);
Result__File File__open(char* path, char* mode);
void File__close(File* self);
Result__String File__read_to_string(File* self);
Result__String File__read_all(char* path);
Result__bool File__write_string(File* self, char* content);
Result__Vec__String File__read_lines(char* path);
Result__bool File__write_lines(char* path, Vec__String* lines);
bool File__exists(char* path);
Result__Metadata File__metadata(char* path);
Result__bool File__create_dir(char* path);
Result__bool File__remove_file(char* path);
Result__bool File__remove_dir(char* path);
Result__Vec__DirEntry File__read_dir(char* path);
Result__String File__current_dir(void);
String String__new(char* s);
String String__from(char* s);
String String__from_rune(int32_t r);
String String__from_runes(int32_t* runes, size_t count);
char* String__c_str(String* self);
char* String__to_string(String* self);
void String__destroy(String* self);
void String__forget(String* self);
void String__append(String* self, String* other);
void String__append_c(String* self, char* s);
void String__push_rune(String* self, int32_t r);
void String__append_c_ptr(String* ptr, char* s);
String String__add(String* self, String* other);
void String__add_assign(String* self, String* other);
bool String__eq(String* self, String* other);
bool String__neq(String* self, String* other);
int32_t String__compare(String* self, String* other);
bool String__lt(String* self, String* other);
bool String__gt(String* self, String* other);
bool String__le(String* self, String* other);
bool String__ge(String* self, String* other);
int32_t String__compare_ignore_case(String* self, String* other);
bool String__eq_ignore_case(String* self, String* other);
bool String__eq_str(String* self, char* s);
size_t String__length(String* self);
String String__substring(String* self, size_t start, size_t len);
bool String__contains_str(String* self, char* target);
Option__size_t String__find_str(String* self, char* target);
Vec__size_t String__find_all_str(String* self, char* target);
String String__to_lowercase(String* self);
String String__pad_right(String* self, size_t target_len, char pad_char);
String String__pad_left(String* self, size_t target_len, char pad_char);
String String__to_uppercase(String* self);
Option__size_t String__find(String* self, char target);
void String__print(String* self);
void String__println(String* self);
bool String__is_empty(String* self);
bool String__contains(String* self, char target);
bool String__starts_with(String* self, char* prefix);
bool String__ends_with(String* self, char* suffix);
void String__reserve(String* self, size_t cap);
void String__free(String* self);
size_t String__utf8_seq_len(char first_byte);
size_t String__utf8_len(String* self);
String String__utf8_at(String* self, size_t idx);
int32_t String__utf8_get(String* self, size_t idx);
Vec__int32_t String__runes(String* self);
StringCharsIter String__iterator(String* self);
StringCharsIter String__chars(String* self);
String String__from_runes_vec(Vec__int32_t runes);
void String__insert_rune(String* self, size_t idx, int32_t r);
int32_t String__remove_rune_at(String* self, size_t idx);
String String__utf8_substr(String* self, size_t start_idx, size_t num_chars);
Vec__String String__split(String* self, char delim);
String String__trim(String* self);
String String__replace(String* self, char* target, char* replacement);
Option__int32_t StringCharsIter__next(StringCharsIter* self);
StringCharsIter StringCharsIter__iterator(StringCharsIter* self);
const int32_t Z_SEEK_SET = 0;
const int32_t Z_SEEK_END = 2;
const int32_t Z_F_OK = 0;
size_t __zen_hash_seed = 14695981039346656037ULL;
const size_t _HASH_MAGIC = 1099511628211;
// Auto-Generated RAII Glue for Map__JsonValuePtr
void Map__JsonValuePtr__Drop__glue(Map__JsonValuePtr *self) {
    Map__JsonValuePtr__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__JsonValuePtr
void Vec__JsonValuePtr__Drop__glue(Vec__JsonValuePtr *self) {
    Vec__JsonValuePtr__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__DirEntryPtr
void Vec__DirEntryPtr__Drop__glue(Vec__DirEntryPtr *self) {
    Vec__DirEntryPtr__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__DirEntry
void Vec__DirEntry__Drop__glue(Vec__DirEntry *self) {
    Vec__DirEntry__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__StringPtr
void Vec__StringPtr__Drop__glue(Vec__StringPtr *self) {
    Vec__StringPtr__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__String
void Vec__String__Drop__glue(Vec__String *self) {
    Vec__String__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__int32_t
void Vec__int32_t__Drop__glue(Vec__int32_t *self) {
    Vec__int32_t__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__size_t
void Vec__size_t__Drop__glue(Vec__size_t *self) {
    Vec__size_t__Drop__drop(self);
}


// Auto-Generated RAII Glue for Vec__char
void Vec__char__Drop__glue(Vec__char *self) {
    Vec__char__Drop__drop(self);
}


// Auto-Generated RAII Glue for Arena
void Arena__Drop__glue(Arena *self) {
    Arena__Drop__drop(self);
}


// Auto-Generated RAII Glue for File
void File__Drop__glue(File *self) {
    File__Drop__drop(self);
}


// Auto-Generated RAII Glue for Result__Vec__DirEntry
void Result__Vec__DirEntry__Drop__glue(Result__Vec__DirEntry *self) {
    Vec__DirEntry__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Result__Vec__String
void Result__Vec__String__Drop__glue(Result__Vec__String *self) {
    Vec__String__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Result__File
void Result__File__Drop__glue(Result__File *self) {
    File__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for JsonValue
void JsonValue__Drop__glue(JsonValue *self) {
    JsonValue__Drop__drop(self);
}


// Auto-Generated RAII Glue for String
void String__Drop__glue(String *self) {
    Vec__char__Drop__glue(&self->vec);
}


// Auto-Generated RAII Glue for Result__JsonValue
void Result__JsonValue__Drop__glue(Result__JsonValue *self) {
    JsonValue__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Result__String
void Result__String__Drop__glue(Result__String *self) {
    String__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Option__String
void Option__String__Drop__glue(Option__String *self) {
    String__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Command
void Command__Drop__glue(Command *self) {
    String__Drop__glue(&self->program);
    Vec__String__Drop__glue(&self->args);
}


// Auto-Generated RAII Glue for Output
void Output__Drop__glue(Output *self) {
    String__Drop__glue(&self->std_out);
}


// Auto-Generated RAII Glue for Path
void Path__Drop__glue(Path *self) {
    String__Drop__glue(&self->str);
}


// Auto-Generated RAII Glue for DirEntry
void DirEntry__Drop__glue(DirEntry *self) {
    String__Drop__glue(&self->name);
}


// Auto-Generated RAII Glue for Option__Path
void Option__Path__Drop__glue(Option__Path *self) {
    Path__Drop__glue(&self->val);
}


// Auto-Generated RAII Glue for Option__DirEntry
void Option__DirEntry__Drop__glue(Option__DirEntry *self) {
    DirEntry__Drop__glue(&self->val);
}


// Global Generic Drop Dispatch
#define _z_drop(x) _Generic((x), \
    Map__JsonValuePtr: Map__JsonValuePtr__Drop__glue((void*)&(x)), \
    Vec__JsonValuePtr: Vec__JsonValuePtr__Drop__glue((void*)&(x)), \
    Vec__DirEntryPtr: Vec__DirEntryPtr__Drop__glue((void*)&(x)), \
    Vec__DirEntry: Vec__DirEntry__Drop__glue((void*)&(x)), \
    Vec__StringPtr: Vec__StringPtr__Drop__glue((void*)&(x)), \
    Vec__String: Vec__String__Drop__glue((void*)&(x)), \
    Vec__int32_t: Vec__int32_t__Drop__glue((void*)&(x)), \
    Vec__size_t: Vec__size_t__Drop__glue((void*)&(x)), \
    Vec__char: Vec__char__Drop__glue((void*)&(x)), \
    Arena: Arena__Drop__glue((void*)&(x)), \
    File: File__Drop__glue((void*)&(x)), \
    Result__Vec__DirEntry: Result__Vec__DirEntry__Drop__glue((void*)&(x)), \
    Result__Vec__String: Result__Vec__String__Drop__glue((void*)&(x)), \
    Result__File: Result__File__Drop__glue((void*)&(x)), \
    JsonValue: JsonValue__Drop__glue((void*)&(x)), \
    String: String__Drop__glue((void*)&(x)), \
    Result__JsonValue: Result__JsonValue__Drop__glue((void*)&(x)), \
    Result__String: Result__String__Drop__glue((void*)&(x)), \
    Option__String: Option__String__Drop__glue((void*)&(x)), \
    Command: Command__Drop__glue((void*)&(x)), \
    Output: Output__Drop__glue((void*)&(x)), \
    Path: Path__Drop__glue((void*)&(x)), \
    DirEntry: DirEntry__Drop__glue((void*)&(x)), \
    Option__Path: Option__Path__Drop__glue((void*)&(x)), \
    Option__DirEntry: Option__DirEntry__Drop__glue((void*)&(x)), \
    default: (void)0)

static void _z_test_0() {
    {

#line 7 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Testing String methods..."); fprintf(stdout, "\n"); 0; });

#line 10 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s1 = 1; String s1 = 
#line 10 "tests/lib/collections/test_string_suite.zc"
String__from("Hello World");

#line 11 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_sub = 1; String sub = 
#line 11 "tests/lib/collections/test_string_suite.zc"
String__substring((&s1), 0, 5);

#line 12 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_expected1 = 1; String expected1 = 
#line 12 "tests/lib/collections/test_string_suite.zc"
String__from("Hello");

#line 13 "tests/lib/collections/test_string_suite.zc"
if (
#line 13 "tests/lib/collections/test_string_suite.zc"
String__eq((&sub), (&expected1)))     {

#line 14 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> substring(0, 5): Passed"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 16 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "substring(0, 5) failed");
    }

#line 20 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_sub2 = 1; String sub2 = 
#line 20 "tests/lib/collections/test_string_suite.zc"
String__substring((&s1), 6, 5);

#line 21 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_expected2 = 1; String expected2 = 
#line 21 "tests/lib/collections/test_string_suite.zc"
String__from("World");

#line 22 "tests/lib/collections/test_string_suite.zc"
if (
#line 22 "tests/lib/collections/test_string_suite.zc"
String__eq((&sub2), (&expected2)))     {

#line 23 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> substring(6, 5): Passed"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 25 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "substring(6, 5) failed");
    }

#line 29 "tests/lib/collections/test_string_suite.zc"
    Option__size_t pos = 
#line 29 "tests/lib/collections/test_string_suite.zc"
String__find((&s1), 'W');

#line 30 "tests/lib/collections/test_string_suite.zc"
if (
#line 30 "tests/lib/collections/test_string_suite.zc"
Option__size_t__is_some((&pos)))     {

#line 31 "tests/lib/collections/test_string_suite.zc"
    size_t idx = 
#line 31 "tests/lib/collections/test_string_suite.zc"
Option__size_t__unwrap((&pos));

#line 32 "tests/lib/collections/test_string_suite.zc"
__zenc_assert((idx == ((size_t)(6))), "find('W') index mismatch");

#line 33 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> find('W'): Passed (found at index 6)"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 35 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "find('W') failed (not found)");
    }

#line 39 "tests/lib/collections/test_string_suite.zc"
    Option__size_t pos2 = 
#line 39 "tests/lib/collections/test_string_suite.zc"
String__find((&s1), 'Z');

#line 40 "tests/lib/collections/test_string_suite.zc"
if (
#line 40 "tests/lib/collections/test_string_suite.zc"
Option__size_t__is_none((&pos2)))     {

#line 41 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> find('Z'): Passed (not found)"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 43 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "find('Z') failed (found unexpectedly)");
    }

#line 47 "tests/lib/collections/test_string_suite.zc"
    size_t len = 
#line 47 "tests/lib/collections/test_string_suite.zc"
String__length((&s1));

#line 48 "tests/lib/collections/test_string_suite.zc"
if ((len == ((size_t)(11))))     {

#line 49 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> length(): Passed (11)"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 51 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "length() failed");
    }

#line 55 "tests/lib/collections/test_string_suite.zc"
if (
#line 55 "tests/lib/collections/test_string_suite.zc"
String__contains((&s1), 'W'))     {

#line 56 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> contains('W'): Passed"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 58 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "contains('W') failed");
    }

#line 61 "tests/lib/collections/test_string_suite.zc"
if ((!
#line 61 "tests/lib/collections/test_string_suite.zc"
String__contains((&s1), 'Z')))     {

#line 62 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> !contains('Z'): Passed"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 64 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "!contains('Z') failed");
    }

#line 68 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s2 = 1; String s2 = 
#line 68 "tests/lib/collections/test_string_suite.zc"
String__from("Hello");

#line 69 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s3 = 1; String s3 = 
#line 69 "tests/lib/collections/test_string_suite.zc"
String__from(" World");

#line 70 "tests/lib/collections/test_string_suite.zc"
String__append((&s2), (&s3));

#line 71 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_expected6 = 1; String expected6 = 
#line 71 "tests/lib/collections/test_string_suite.zc"
String__from("Hello World");

#line 72 "tests/lib/collections/test_string_suite.zc"
if (
#line 72 "tests/lib/collections/test_string_suite.zc"
String__eq((&s2), (&expected6)))     {

#line 73 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "  -> append(): Passed"); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 75 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "append() failed");
    }

#line 78 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "All String methods passed!"); fprintf(stdout, "\n"); 0; });

#line 71 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_expected6) String__Drop__glue(&expected6);

#line 69 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s3) String__Drop__glue(&s3);

#line 68 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s2) String__Drop__glue(&s2);

#line 21 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_expected2) String__Drop__glue(&expected2);

#line 20 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_sub2) String__Drop__glue(&sub2);

#line 12 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_expected1) String__Drop__glue(&expected1);

#line 11 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_sub) String__Drop__glue(&sub);

#line 10 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s1) String__Drop__glue(&s1);
    }
}
static void _z_test_1() {
    {

#line 82 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Testing F-Strings and Returns..."); fprintf(stdout, "\n"); 0; });

#line 83 "tests/lib/collections/test_string_suite.zc"
    int32_t x = 100;

#line 84 "tests/lib/collections/test_string_suite.zc"
    int32_t y = 50;

#line 86 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Direct F-String: x="); fprintf(stdout, "%d", x); fprintf(stdout, "\n"); 0; });

#line 87 "tests/lib/collections/test_string_suite.zc"
    string nested = ({ 
#line 87 "tests/lib/collections/test_string_suite.zc"
    static char _fs_buf_1[4096]; _fs_buf_1[0]=0;
    char _fs_t_1[128];
    strcat(_fs_buf_1, "y is ");

#line 87 "tests/lib/collections/test_string_suite.zc"
sprintf(_fs_t_1, _z_str(y), y);

#line 87 "tests/lib/collections/test_string_suite.zc"
    strcat(_fs_buf_1, _fs_t_1);
    _fs_buf_1;
 });

#line 88 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Argument F-String: "); fprintf(stdout, "%s", nested); fprintf(stdout, "\n"); 0; });

#line 89 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Math inside: 100 + 50 = "); fprintf(stdout, "%d", (x + y)); fprintf(stdout, "\n"); 0; });

#line 91 "tests/lib/collections/test_string_suite.zc"
    string inner = ({ 
#line 91 "tests/lib/collections/test_string_suite.zc"
    static char _fs_buf_2[4096]; _fs_buf_2[0]=0;
    char _fs_t_2[128];
    strcat(_fs_buf_2, "Inner(");

#line 91 "tests/lib/collections/test_string_suite.zc"
sprintf(_fs_t_2, _z_str(x), x);

#line 91 "tests/lib/collections/test_string_suite.zc"
    strcat(_fs_buf_2, _fs_t_2);
    strcat(_fs_buf_2, ")");
    _fs_buf_2;
 });

#line 92 "tests/lib/collections/test_string_suite.zc"
    string outer = ({ 
#line 92 "tests/lib/collections/test_string_suite.zc"
    static char _fs_buf_3[4096]; _fs_buf_3[0]=0;
    char _fs_t_3[128];
    strcat(_fs_buf_3, "Outer(");

#line 92 "tests/lib/collections/test_string_suite.zc"
sprintf(_fs_t_3, _z_str(inner), inner);

#line 92 "tests/lib/collections/test_string_suite.zc"
    strcat(_fs_buf_3, _fs_t_3);
    strcat(_fs_buf_3, ")");
    _fs_buf_3;
 });

#line 93 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Composed: "); fprintf(stdout, "%s", outer); fprintf(stdout, "\n"); 0; });

#line 94 "tests/lib/collections/test_string_suite.zc"
    ZC_AUTO outer_res = 
#line 94 "tests/lib/collections/test_string_suite.zc"
strcmp(outer, "Outer(Inner(100))");

#line 95 "tests/lib/collections/test_string_suite.zc"
    int zero = 0;

#line 96 "tests/lib/collections/test_string_suite.zc"
__zenc_assert((outer_res == zero), "Composed f-string failed");
    }
}
static void _z_test_2() {
    {

#line 100 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Testing String Std Ops..."); fprintf(stdout, "\n"); 0; });

#line 101 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s1 = 1; String s1 = 
#line 101 "tests/lib/collections/test_string_suite.zc"
String__from("Hello");

#line 102 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s2 = 1; String s2 = 
#line 102 "tests/lib/collections/test_string_suite.zc"
String__from(" World");

#line 104 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s3 = 1; String s3 = 
#line 104 "tests/lib/collections/test_string_suite.zc"
String__add((&s1), (&s2));

#line 106 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Concatenated: "); fflush(stdout); 0; });

#line 107 "tests/lib/collections/test_string_suite.zc"
String__println((&s3));

#line 109 "tests/lib/collections/test_string_suite.zc"
__zenc_assert((
#line 109 "tests/lib/collections/test_string_suite.zc"
String__length((&s3)) == 11), "Length mismatch");

#line 111 "tests/lib/collections/test_string_suite.zc"
    int __z_drop_flag_s4 = 1; String s4 = 
#line 111 "tests/lib/collections/test_string_suite.zc"
String__from("Hello World");

#line 112 "tests/lib/collections/test_string_suite.zc"
if (
#line 112 "tests/lib/collections/test_string_suite.zc"
String__eq((&s3), (&s4)))     {

#line 113 "tests/lib/collections/test_string_suite.zc"
    ({ fprintf(stdout, "%s", "Equality check passed: Strings are identical."); fprintf(stdout, "\n"); 0; });
    }
 else     {

#line 115 "tests/lib/collections/test_string_suite.zc"
__zenc_assert(false, "Equality check failed");
    }

#line 111 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s4) String__Drop__glue(&s4);

#line 104 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s3) String__Drop__glue(&s3);

#line 102 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s2) String__Drop__glue(&s2);

#line 101 "tests/lib/collections/test_string_suite.zc"
    if (__z_drop_flag_s1) String__Drop__glue(&s1);
    }
}

void _z_run_tests() {
    _z_test_0();
    _z_test_1();
    _z_test_2();
}


#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Match Option__Match__Some(Match v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__Match){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Match Option__Match__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__Match opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(Match));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__Match__is_some(Option__Match* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__Match__is_none(Option__Match* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__Match__forget(Option__Match* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(Match));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Match Option__Match__unwrap(Option__Match* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Match v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Match* Option__Match__unwrap_ref(Option__Match* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Match Option__Match__unwrap_or(Option__Match* self, Match def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Match Option__Match__expect(Option__Match* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Match Option__Match__or_else(Option__Match* self, Option__Match other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__string Option__string__Some(string v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__string){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__string Option__string__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__string opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(char*));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__string__is_some(Option__string* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__string__is_none(Option__string* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__string__forget(Option__string* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(char*));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

string Option__string__unwrap(Option__string* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    string v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

string* Option__string__unwrap_ref(Option__string* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

string Option__string__unwrap_or(Option__string* self, string def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

string Option__string__expect(Option__string* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__string Option__string__or_else(Option__string* self, Option__string other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Path Option__Path__Some(Path v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__Path _z_ret = (Option__Path){.is_some = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) Path__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) Path__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Path Option__Path__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_opt = 1; Option__Path opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(Path));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = opt; memset(&opt, 0, sizeof(_z_ret_mv)); __z_drop_flag_opt = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__Path__Drop__glue(&opt);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__Path__Drop__glue(&opt);
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__Path__is_some(Option__Path* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__Path__is_none(Option__Path* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__Path__forget(Option__Path* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(Path));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Path Option__Path__unwrap(Option__Path* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1; Path v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) Path__Drop__glue(&v);
_z_ret_mv; });

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) Path__Drop__glue(&v);
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Path* Option__Path__unwrap_ref(Option__Path* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Path Option__Path__unwrap_or(Option__Path* self, Path def_val)
{

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_def_val = 1;
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Path _z_ret = self->val; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) Path__Drop__glue(&def_val);
return _z_ret; }
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv)); __z_drop_flag_def_val = 0; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) Path__Drop__glue(&def_val);
_z_ret_mv; });
    }

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) Path__Drop__glue(&def_val);
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Path Option__Path__expect(Option__Path* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__Path Option__Path__or_else(Option__Path* self, Option__Path other)
{

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_other = 1;
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__Path _z_ret = *self; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__Path__Drop__glue(&other);
return _z_ret; }
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = other; memset(&other, 0, sizeof(_z_ret_mv)); __z_drop_flag_other = 0; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__Path__Drop__glue(&other);
_z_ret_mv; });
    }

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__Path__Drop__glue(&other);
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__bool Option__bool__Some(bool v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__bool){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__bool Option__bool__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__bool opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(bool));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__bool__is_some(Option__bool* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__bool__is_none(Option__bool* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__bool__forget(Option__bool* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(bool));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__bool__unwrap(Option__bool* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    bool v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool* Option__bool__unwrap_ref(Option__bool* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__bool__unwrap_or(Option__bool* self, bool def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__bool__expect(Option__bool* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__bool Option__bool__or_else(Option__bool* self, Option__bool other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__double Option__double__Some(double v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__double){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__double Option__double__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__double opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(double));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__double__is_some(Option__double* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__double__is_none(Option__double* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__double__forget(Option__double* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(double));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

double Option__double__unwrap(Option__double* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    double v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

double* Option__double__unwrap_ref(Option__double* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

double Option__double__unwrap_or(Option__double* self, double def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

double Option__double__expect(Option__double* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__double Option__double__or_else(Option__double* self, Option__double other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__charPtr Option__charPtr__Some(char* v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__charPtr){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__charPtr Option__charPtr__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__charPtr opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(char*));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__charPtr__is_some(Option__charPtr* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__charPtr__is_none(Option__charPtr* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__charPtr__forget(Option__charPtr* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(char*));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char* Option__charPtr__unwrap(Option__charPtr* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    char* v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char** Option__charPtr__unwrap_ref(Option__charPtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char* Option__charPtr__unwrap_or(Option__charPtr* self, char* def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char* Option__charPtr__expect(Option__charPtr* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__charPtr Option__charPtr__or_else(Option__charPtr* self, Option__charPtr other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__JsonValue Result__JsonValue__Ok(JsonValue v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    { Result__JsonValue _z_ret = (Result__JsonValue){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; }), .err = NULL}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) JsonValue__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) JsonValue__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__JsonValue Result__JsonValue__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_r = 1; Result__JsonValue r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(JsonValue));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = r; memset(&r, 0, sizeof(_z_ret_mv)); __z_drop_flag_r = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__JsonValue__Drop__glue(&r);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__JsonValue__Drop__glue(&r);
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__JsonValue__is_ok(Result__JsonValue* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__JsonValue__is_err(Result__JsonValue* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__JsonValue__forget(Result__JsonValue* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(JsonValue));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue Result__JsonValue__unwrap(Result__JsonValue* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1; JsonValue v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) JsonValue__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) JsonValue__Drop__glue(&v);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue* Result__JsonValue__unwrap_ref(Result__JsonValue* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue Result__JsonValue__expect(Result__JsonValue* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__JsonValuePtr Result__JsonValuePtr__Ok(JsonValue* v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (Result__JsonValuePtr){.is_ok = true, .val = v, .err = NULL};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__JsonValuePtr Result__JsonValuePtr__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    Result__JsonValuePtr r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(JsonValue*));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return r;
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__JsonValuePtr__is_ok(Result__JsonValuePtr* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__JsonValuePtr__is_err(Result__JsonValuePtr* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__JsonValuePtr__forget(Result__JsonValuePtr* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(JsonValue*));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue* Result__JsonValuePtr__unwrap(Result__JsonValuePtr* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    JsonValue* v = self->val;

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return v;
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue** Result__JsonValuePtr__unwrap_ref(Result__JsonValuePtr* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

JsonValue* Result__JsonValuePtr__expect(Result__JsonValuePtr* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

Map__JsonValuePtr Map__JsonValuePtr__new(void)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (Map__JsonValuePtr){0};
    }
}

#line 92 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

void Map__JsonValuePtr__resize(Map__JsonValuePtr* self, size_t new_cap)
{
    {

#line 93 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    char** old_keys = self->keys;

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    JsonValue** old_vals = self->vals;

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    bool* old_occupied = self->occupied;

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    bool* old_deleted = self->deleted;

#line 97 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t old_cap = self->cap;

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->cap = new_cap);

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->keys = 
#line 100 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
calloc(new_cap, sizeof(char*)));

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->vals = 
#line 101 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
calloc(new_cap, sizeof(JsonValue*)));

#line 102 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->occupied = 
#line 102 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
calloc(new_cap, sizeof(bool)));

#line 103 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->deleted = 
#line 103 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
calloc(new_cap, sizeof(bool)));

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->len = 0);

#line 106 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
for (size_t i = (size_t)(0); i < old_cap; i = (i + 1))     {

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((old_occupied[i] && (!old_deleted[i])))     {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Map__JsonValuePtr__put(self, old_keys[i], old_vals[i]);
    }
    }

#line 113 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((old_keys != NULL))     {

#line 113 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(old_keys);
    }

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((old_vals != NULL))     {

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(old_vals);
    }

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((old_occupied != NULL))     {

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(old_occupied);
    }

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((old_deleted != NULL))     {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(old_deleted);
    }
    }
}

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

void Map__JsonValuePtr__put(Map__JsonValuePtr* self, char* key, JsonValue* val)
{
    {

#line 120 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((self->len >= (self->cap * 0.75)))     {

#line 121 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t new_cap = (self->cap * 2);

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((new_cap < 8))     {

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(new_cap = 8);
    }

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Map__JsonValuePtr__resize(self, new_cap);
    }

#line 126 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t hash = 
#line 126 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
_map_hash_str(key);

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t idx = (hash % self->cap);

#line 129 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
while (true)     {

#line 130 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if (((!self->occupied[idx]) || ((self->occupied[idx] && (!self->deleted[idx])) && (
#line 130 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
strcmp(self->keys[idx], key) == 0))))     {

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((!self->occupied[idx]))     {

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->len = (self->len + 1));
    }

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if (((!self->occupied[idx]) || self->deleted[idx]))     {

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->keys[idx] = 
#line 134 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
strdup(key));
    }

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->vals[idx] = val);

#line 138 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->occupied[idx] = true);

#line 139 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->deleted[idx] = false);

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return ;
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(idx = ((idx + 1) % self->cap));
    }
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

Option__JsonValuePtr Map__JsonValuePtr__get(Map__JsonValuePtr* self, char* key)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((self->cap == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 149 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__None();
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t hash = 
#line 152 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
_map_hash_str(key);

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t idx = (hash % self->cap);

#line 154 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t start_idx = idx;

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
while (true)     {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((!self->occupied[idx]))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__None();
    }

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if (((!self->deleted[idx]) && (
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
strcmp(self->keys[idx], key) == 0)))     {

#line 162 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 162 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__Some(self->vals[idx]);
    }

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(idx = ((idx + 1) % self->cap));

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((idx == start_idx))     {

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 167 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__None();
    }
    }

#line 170 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 170 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__None();
    }
}

#line 173 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

bool Map__JsonValuePtr__contains(Map__JsonValuePtr* self, char* key)
{
    {

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    Option__JsonValuePtr opt = 
#line 174 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Map__JsonValuePtr__get(self, key);

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return 
#line 175 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Option__JsonValuePtr__is_some(&opt);
    }
}

#line 178 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

void Map__JsonValuePtr__remove(Map__JsonValuePtr* self, char* key)
{
    {

#line 179 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((self->cap == 0))     {

#line 179 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return ;
    }

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t hash = 
#line 181 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
_map_hash_str(key);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t idx = (hash % self->cap);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
while (true)     {

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((!self->occupied[idx]))     {

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return ;
    }

#line 187 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if (((!self->deleted[idx]) && (
#line 187 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
strcmp(self->keys[idx], key) == 0)))     {

#line 188 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->deleted[idx] = true);

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->len = (self->len - 1));

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->keys[idx]);

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return ;
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(idx = ((idx + 1) % self->cap));
    }
    }
}

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

size_t Map__JsonValuePtr__length(Map__JsonValuePtr* self)
{
    {

#line 199 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return self->len;
    }
}

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

bool Map__JsonValuePtr__is_empty(Map__JsonValuePtr* self)
{
    {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (self->len == 0);
    }
}

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

void Map__JsonValuePtr__free(Map__JsonValuePtr* self)
{
    {

#line 207 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if (self->keys)     {

#line 208 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
for (size_t i = (size_t)(0); i < self->cap; i = (i + 1))     {

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((self->occupied[i] && (!self->deleted[i])))     {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->keys[i]);
    }
    }

#line 213 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->keys);

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->vals);

#line 215 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->occupied);

#line 216 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
free(self->deleted);
    }

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->keys = 0);

#line 219 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->vals = 0);

#line 220 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->occupied = 0);

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->deleted = 0);

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->len = 0);

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->cap = 0);
    }
}

#line 226 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

size_t Map__JsonValuePtr__capacity(Map__JsonValuePtr* self)
{
    {

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return self->cap;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

bool Map__JsonValuePtr__is_slot_occupied(Map__JsonValuePtr* self, size_t idx)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((idx >= self->cap))     {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return false;
    }

#line 232 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (self->occupied[idx] && (!self->deleted[idx]));
    }
}

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

char* Map__JsonValuePtr__key_at(Map__JsonValuePtr* self, size_t idx)
{
    {

#line 236 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((((idx >= self->cap) || (!self->occupied[idx])) || self->deleted[idx]))     {

#line 237 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return NULL;
    }

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return self->keys[idx];
    }
}

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

JsonValue* Map__JsonValuePtr__val_at(Map__JsonValuePtr* self, size_t idx)
{
    {

#line 243 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return self->vals[idx];
    }
}

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

MapIter__JsonValuePtr Map__JsonValuePtr__iterator(Map__JsonValuePtr* self)
{
    {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (MapIter__JsonValuePtr){.keys = self->keys, .vals = self->vals, .occupied = self->occupied, .deleted = self->deleted, .cap = self->cap};
    }
}

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

MapIterResult__JsonValuePtr MapIter__JsonValuePtr__next(MapIter__JsonValuePtr* self)
{
    {

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
while ((self->idx < self->cap))     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t i = self->idx;

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(self->idx = (self->idx + 1));

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((self->occupied[i] && (!self->deleted[i])))     {

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    MapEntry__JsonValuePtr entry = (MapEntry__JsonValuePtr){.key = self->keys[i], .val = self->vals[i]};

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (MapIterResult__JsonValuePtr){.entry = entry, .has_val = true};
    }
    }

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    MapEntry__JsonValuePtr empty = (MapEntry__JsonValuePtr){.val = self->vals[0]};

#line 83 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (MapIterResult__JsonValuePtr){.entry = empty, .has_val = false};
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

bool MapIterResult__JsonValuePtr__is_none(MapIterResult__JsonValuePtr* self)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return (!self->has_val);
    }
}

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

MapEntry__JsonValuePtr MapIterResult__JsonValuePtr__unwrap(MapIterResult__JsonValuePtr* self)
{
    {

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
if ((!self->has_val))     {

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    ({ fprintf(stderr, "%s", "Panic: Map iterator unwrap on None"); fprintf(stderr, "\n"); 0; });

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
exit(1);
    }

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return self->entry;
    }
}

#line 259 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

void Map__JsonValuePtr__Drop__drop(Map__JsonValuePtr* self)
{
    {

#line 260 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
Map__JsonValuePtr__free(self);
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__JsonValuePtr Vec__JsonValuePtr__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__JsonValuePtr){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__JsonValuePtr Vec__JsonValuePtr__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__JsonValuePtr){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__JsonValuePtr){.data = ((JsonValue**)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(JsonValue*))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__grow(Vec__JsonValuePtr* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((JsonValue**)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(JsonValue*))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__grow_to_fit(Vec__JsonValuePtr* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((JsonValue**)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(JsonValue*))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__JsonValuePtr Vec__JsonValuePtr__iterator(Vec__JsonValuePtr* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__JsonValuePtr){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__JsonValuePtr Vec__JsonValuePtr__iter_ref(Vec__JsonValuePtr* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__JsonValuePtr){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__push(Vec__JsonValuePtr* self, JsonValue* item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__insert(Vec__JsonValuePtr* self, size_t idx, JsonValue* item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(JsonValue*)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__pop(Vec__JsonValuePtr* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__JsonValuePtr Vec__JsonValuePtr__pop_opt(Vec__JsonValuePtr* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__JsonValuePtr__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__JsonValuePtr__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__remove(Vec__JsonValuePtr* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    JsonValue* item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(JsonValue*)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__append(Vec__JsonValuePtr* self, Vec__JsonValuePtr other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(JsonValue*)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__JsonValuePtr__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__get(Vec__JsonValuePtr* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__index(Vec__JsonValuePtr* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue** Vec__JsonValuePtr__get_ref(Vec__JsonValuePtr* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__last(Vec__JsonValuePtr* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__JsonValuePtr__length(Vec__JsonValuePtr* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__JsonValuePtr__contains(Vec__JsonValuePtr* self, JsonValue* item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(JsonValue*)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__JsonValuePtr__is_empty(Vec__JsonValuePtr* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__clear(Vec__JsonValuePtr* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__free(Vec__JsonValuePtr* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

JsonValue* Vec__JsonValuePtr__first(Vec__JsonValuePtr* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__set(Vec__JsonValuePtr* self, size_t idx, JsonValue* item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__reverse(Vec__JsonValuePtr* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    JsonValue* tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__JsonValuePtr__eq(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(JsonValue*)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__forget(Vec__JsonValuePtr* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__JsonValuePtr Vec__JsonValuePtr__add(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__JsonValuePtr result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__append(&result, Vec__JsonValuePtr__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__JsonValuePtr__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__JsonValuePtr__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__add_assign(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__append(self, Vec__JsonValuePtr__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__JsonValuePtr__neq(Vec__JsonValuePtr* self, Vec__JsonValuePtr* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__shl(Vec__JsonValuePtr* self, JsonValue* item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__shr(Vec__JsonValuePtr* self, JsonValue** out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__JsonValuePtr Vec__JsonValuePtr__mul(Vec__JsonValuePtr* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__JsonValuePtr result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__append(&result, Vec__JsonValuePtr__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__JsonValuePtr__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__JsonValuePtr__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__mul_assign(Vec__JsonValuePtr* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(JsonValue*)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__JsonValuePtr Vec__JsonValuePtr__clone(Vec__JsonValuePtr* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__JsonValuePtr){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    JsonValue** new_data = ((JsonValue**)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(JsonValue*)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__JsonValuePtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__JsonValuePtr VecIterRef__JsonValuePtr__next(VecIterRef__JsonValuePtr* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct JsonValue** item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__JsonValuePtr){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__JsonValuePtr){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__JsonValuePtr VecIterRef__JsonValuePtr__iterator(VecIterRef__JsonValuePtr* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__JsonValuePtr__is_none(VecIterResult__JsonValuePtr* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

struct JsonValue** VecIterResult__JsonValuePtr__unwrap(VecIterResult__JsonValuePtr* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__JsonValuePtr VecIter__JsonValuePtr__next(VecIter__JsonValuePtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct JsonValue* item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__JsonValuePtr__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__JsonValuePtr__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__JsonValuePtr VecIter__JsonValuePtr__iterator(VecIter__JsonValuePtr* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__JsonValuePtr Option__JsonValuePtr__Some(struct JsonValue* v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__JsonValuePtr){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__JsonValuePtr Option__JsonValuePtr__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__JsonValuePtr opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(struct JsonValue*));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__JsonValuePtr__is_some(Option__JsonValuePtr* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__JsonValuePtr__is_none(Option__JsonValuePtr* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__JsonValuePtr__forget(Option__JsonValuePtr* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct JsonValue*));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct JsonValue* Option__JsonValuePtr__unwrap(Option__JsonValuePtr* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    struct JsonValue* v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct JsonValue** Option__JsonValuePtr__unwrap_ref(Option__JsonValuePtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct JsonValue* Option__JsonValuePtr__unwrap_or(Option__JsonValuePtr* self, struct JsonValue* def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct JsonValue* Option__JsonValuePtr__expect(Option__JsonValuePtr* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__JsonValuePtr Option__JsonValuePtr__or_else(Option__JsonValuePtr* self, Option__JsonValuePtr other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__JsonValuePtr__Drop__drop(Vec__JsonValuePtr* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__JsonValuePtr__free(self);
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Vec__DirEntry Result__Vec__DirEntry__Ok(Vec__DirEntry v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    { Result__Vec__DirEntry _z_ret = (Result__Vec__DirEntry){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; }), .err = NULL}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__DirEntry__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__DirEntry__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Vec__DirEntry Result__Vec__DirEntry__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_r = 1; Result__Vec__DirEntry r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(Vec__DirEntry));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = r; memset(&r, 0, sizeof(_z_ret_mv)); __z_drop_flag_r = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__Vec__DirEntry__Drop__glue(&r);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__Vec__DirEntry__Drop__glue(&r);
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Vec__DirEntry__is_ok(Result__Vec__DirEntry* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Vec__DirEntry__is_err(Result__Vec__DirEntry* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__Vec__DirEntry__forget(Result__Vec__DirEntry* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec__DirEntry));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__DirEntry Result__Vec__DirEntry__unwrap(Result__Vec__DirEntry* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1; Vec__DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__DirEntry__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__DirEntry__Drop__glue(&v);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__DirEntry* Result__Vec__DirEntry__unwrap_ref(Result__Vec__DirEntry* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__DirEntry Result__Vec__DirEntry__expect(Result__Vec__DirEntry* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntryPtr Vec__DirEntryPtr__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntryPtr){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntryPtr Vec__DirEntryPtr__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntryPtr){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntryPtr){.data = ((DirEntry**)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(DirEntry*))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__grow(Vec__DirEntryPtr* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((DirEntry**)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__grow_to_fit(Vec__DirEntryPtr* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((DirEntry**)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry*))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__DirEntryPtr Vec__DirEntryPtr__iterator(Vec__DirEntryPtr* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__DirEntryPtr){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__DirEntryPtr Vec__DirEntryPtr__iter_ref(Vec__DirEntryPtr* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__DirEntryPtr){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__push(Vec__DirEntryPtr* self, DirEntry* item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__insert(Vec__DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry*)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__pop(Vec__DirEntryPtr* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__DirEntryPtr Vec__DirEntryPtr__pop_opt(Vec__DirEntryPtr* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntryPtr__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntryPtr__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__remove(Vec__DirEntryPtr* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    DirEntry* item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry*)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__append(Vec__DirEntryPtr* self, Vec__DirEntryPtr other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry*)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__DirEntryPtr__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__get(Vec__DirEntryPtr* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__index(Vec__DirEntryPtr* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry** Vec__DirEntryPtr__get_ref(Vec__DirEntryPtr* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__last(Vec__DirEntryPtr* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__DirEntryPtr__length(Vec__DirEntryPtr* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntryPtr__contains(Vec__DirEntryPtr* self, DirEntry* item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(DirEntry*)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntryPtr__is_empty(Vec__DirEntryPtr* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__clear(Vec__DirEntryPtr* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__free(Vec__DirEntryPtr* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntryPtr__first(Vec__DirEntryPtr* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__set(Vec__DirEntryPtr* self, size_t idx, DirEntry* item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__reverse(Vec__DirEntryPtr* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    DirEntry* tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntryPtr__eq(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(DirEntry*)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__forget(Vec__DirEntryPtr* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntryPtr Vec__DirEntryPtr__add(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__DirEntryPtr result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__append(&result, Vec__DirEntryPtr__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntryPtr__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntryPtr__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__add_assign(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__append(self, Vec__DirEntryPtr__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntryPtr__neq(Vec__DirEntryPtr* self, Vec__DirEntryPtr* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__shl(Vec__DirEntryPtr* self, DirEntry* item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__shr(Vec__DirEntryPtr* self, DirEntry** out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntryPtr Vec__DirEntryPtr__mul(Vec__DirEntryPtr* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__DirEntryPtr result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__append(&result, Vec__DirEntryPtr__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntryPtr__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntryPtr__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__mul_assign(Vec__DirEntryPtr* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(DirEntry*)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntryPtr Vec__DirEntryPtr__clone(Vec__DirEntryPtr* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntryPtr){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    DirEntry** new_data = ((DirEntry**)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(DirEntry*)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntryPtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__DirEntryPtr VecIterRef__DirEntryPtr__next(VecIterRef__DirEntryPtr* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct DirEntry** item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__DirEntryPtr){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__DirEntryPtr){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__DirEntryPtr VecIterRef__DirEntryPtr__iterator(VecIterRef__DirEntryPtr* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__DirEntryPtr__is_none(VecIterResult__DirEntryPtr* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

struct DirEntry** VecIterResult__DirEntryPtr__unwrap(VecIterResult__DirEntryPtr* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__DirEntryPtr VecIter__DirEntryPtr__next(VecIter__DirEntryPtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct DirEntry* item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntryPtr__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntryPtr__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__DirEntryPtr VecIter__DirEntryPtr__iterator(VecIter__DirEntryPtr* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntryPtr Option__DirEntryPtr__Some(struct DirEntry* v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__DirEntryPtr){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntryPtr Option__DirEntryPtr__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__DirEntryPtr opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(struct DirEntry*));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__DirEntryPtr__is_some(Option__DirEntryPtr* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__DirEntryPtr__is_none(Option__DirEntryPtr* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__DirEntryPtr__forget(Option__DirEntryPtr* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct DirEntry*));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct DirEntry* Option__DirEntryPtr__unwrap(Option__DirEntryPtr* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    struct DirEntry* v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct DirEntry** Option__DirEntryPtr__unwrap_ref(Option__DirEntryPtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct DirEntry* Option__DirEntryPtr__unwrap_or(Option__DirEntryPtr* self, struct DirEntry* def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct DirEntry* Option__DirEntryPtr__expect(Option__DirEntryPtr* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntryPtr Option__DirEntryPtr__or_else(Option__DirEntryPtr* self, Option__DirEntryPtr other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntryPtr__Drop__drop(Vec__DirEntryPtr* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntryPtr__free(self);
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntry Vec__DirEntry__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntry){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntry Vec__DirEntry__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntry){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntry){.data = ((DirEntry*)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(DirEntry))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__grow(Vec__DirEntry* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((DirEntry*)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__grow_to_fit(Vec__DirEntry* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((DirEntry*)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(DirEntry))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__DirEntry Vec__DirEntry__iterator(Vec__DirEntry* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__DirEntry){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__DirEntry Vec__DirEntry__iter_ref(Vec__DirEntry* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__DirEntry){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__push(Vec__DirEntry* self, DirEntry item)
{

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = ({ __z_drop_flag_item = 0; item; }));

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__insert(Vec__DirEntry* self, size_t idx, DirEntry item)
{

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(DirEntry)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__pop(Vec__DirEntry* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__DirEntry Vec__DirEntry__pop_opt(Vec__DirEntry* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntry__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntry__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__remove(Vec__DirEntry* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; DirEntry item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(DirEntry)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv)); __z_drop_flag_item = 0; 
#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
_z_ret_mv; });

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__append(Vec__DirEntry* self, Vec__DirEntry other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(DirEntry)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__DirEntry__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__get(Vec__DirEntry* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__index(Vec__DirEntry* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* Vec__DirEntry__get_ref(Vec__DirEntry* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__last(Vec__DirEntry* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__DirEntry__length(Vec__DirEntry* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntry__contains(Vec__DirEntry* self, DirEntry item)
{

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(DirEntry)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { bool _z_ret = true; 
#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
return _z_ret; }
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { bool _z_ret = false; 
#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
return _z_ret; }
    }

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntry__is_empty(Vec__DirEntry* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__clear(Vec__DirEntry* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__free(Vec__DirEntry* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry Vec__DirEntry__first(Vec__DirEntry* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__set(Vec__DirEntry* self, size_t idx, DirEntry item)
{

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));
    }

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__reverse(Vec__DirEntry* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_tmp = 1; DirEntry tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = ({ __z_drop_flag_tmp = 0; tmp; }));
(i++);
(j--);

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_tmp) DirEntry__Drop__glue(&tmp);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntry__eq(Vec__DirEntry* self, Vec__DirEntry* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(DirEntry)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__forget(Vec__DirEntry* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntry Vec__DirEntry__add(Vec__DirEntry* self, Vec__DirEntry* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__DirEntry result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__append(&result, Vec__DirEntry__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntry__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntry__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__add_assign(Vec__DirEntry* self, Vec__DirEntry* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__append(self, Vec__DirEntry__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__DirEntry__neq(Vec__DirEntry* self, Vec__DirEntry* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__shl(Vec__DirEntry* self, DirEntry item)
{

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__push(self, ({ __z_drop_flag_item = 0; item; }));
    }

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__shr(Vec__DirEntry* self, DirEntry* out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntry Vec__DirEntry__mul(Vec__DirEntry* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__DirEntry result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__append(&result, Vec__DirEntry__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntry__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__DirEntry__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__mul_assign(Vec__DirEntry* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(DirEntry)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__DirEntry Vec__DirEntry__clone(Vec__DirEntry* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntry){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    DirEntry* new_data = ((DirEntry*)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(DirEntry)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__DirEntry){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__DirEntry VecIterRef__DirEntry__next(VecIterRef__DirEntry* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    DirEntry* item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__DirEntry){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__DirEntry){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__DirEntry VecIterRef__DirEntry__iterator(VecIterRef__DirEntry* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__DirEntry__is_none(VecIterResult__DirEntry* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

DirEntry* VecIterResult__DirEntry__unwrap(VecIterResult__DirEntry* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__DirEntry VecIter__DirEntry__next(VecIter__DirEntry* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; DirEntry item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { Option__DirEntry _z_ret = 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntry__Some(({ __z_drop_flag_item = 0; item; })); 
#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
return _z_ret; }

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) DirEntry__Drop__glue(&item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__DirEntry__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__DirEntry VecIter__DirEntry__iterator(VecIter__DirEntry* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntry Option__DirEntry__Some(DirEntry v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__DirEntry _z_ret = (Option__DirEntry){.is_some = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntry Option__DirEntry__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_opt = 1; Option__DirEntry opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(DirEntry));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = opt; memset(&opt, 0, sizeof(_z_ret_mv)); __z_drop_flag_opt = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__DirEntry__Drop__glue(&opt);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__DirEntry__Drop__glue(&opt);
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__DirEntry__is_some(Option__DirEntry* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__DirEntry__is_none(Option__DirEntry* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__DirEntry__forget(Option__DirEntry* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(DirEntry));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

DirEntry Option__DirEntry__unwrap(Option__DirEntry* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1; DirEntry v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop__glue(&v);
_z_ret_mv; });

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) DirEntry__Drop__glue(&v);
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

DirEntry* Option__DirEntry__unwrap_ref(Option__DirEntry* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

DirEntry Option__DirEntry__unwrap_or(Option__DirEntry* self, DirEntry def_val)
{

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_def_val = 1;
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { DirEntry _z_ret = self->val; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop__glue(&def_val);
return _z_ret; }
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv)); __z_drop_flag_def_val = 0; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop__glue(&def_val);
_z_ret_mv; });
    }

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) DirEntry__Drop__glue(&def_val);
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

DirEntry Option__DirEntry__expect(Option__DirEntry* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__DirEntry Option__DirEntry__or_else(Option__DirEntry* self, Option__DirEntry other)
{

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_other = 1;
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__DirEntry _z_ret = *self; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__DirEntry__Drop__glue(&other);
return _z_ret; }
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = other; memset(&other, 0, sizeof(_z_ret_mv)); __z_drop_flag_other = 0; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__DirEntry__Drop__glue(&other);
_z_ret_mv; });
    }

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__DirEntry__Drop__glue(&other);
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__DirEntry__Drop__drop(Vec__DirEntry* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__DirEntry__free(self);
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Metadata Result__Metadata__Ok(Metadata v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (Result__Metadata){.is_ok = true, .val = v, .err = NULL};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Metadata Result__Metadata__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    Result__Metadata r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(Metadata));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return r;
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Metadata__is_ok(Result__Metadata* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Metadata__is_err(Result__Metadata* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__Metadata__forget(Result__Metadata* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Metadata));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Metadata Result__Metadata__unwrap(Result__Metadata* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    Metadata v = self->val;

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return v;
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Metadata* Result__Metadata__unwrap_ref(Result__Metadata* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Metadata Result__Metadata__expect(Result__Metadata* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Vec__String Result__Vec__String__Ok(Vec__String v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    { Result__Vec__String _z_ret = (Result__Vec__String){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; }), .err = NULL}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__String__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__String__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__Vec__String Result__Vec__String__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_r = 1; Result__Vec__String r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(Vec__String));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = r; memset(&r, 0, sizeof(_z_ret_mv)); __z_drop_flag_r = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__Vec__String__Drop__glue(&r);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__Vec__String__Drop__glue(&r);
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Vec__String__is_ok(Result__Vec__String* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__Vec__String__is_err(Result__Vec__String* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__Vec__String__forget(Result__Vec__String* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(Vec__String));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__String Result__Vec__String__unwrap(Result__Vec__String* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1; Vec__String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__String__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) Vec__String__Drop__glue(&v);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__String* Result__Vec__String__unwrap_ref(Result__Vec__String* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Vec__String Result__Vec__String__expect(Result__Vec__String* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__StringPtr Vec__StringPtr__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__StringPtr){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__StringPtr Vec__StringPtr__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__StringPtr){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__StringPtr){.data = ((String**)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(String*))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__grow(Vec__StringPtr* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((String**)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String*))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__grow_to_fit(Vec__StringPtr* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((String**)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String*))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__StringPtr Vec__StringPtr__iterator(Vec__StringPtr* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__StringPtr){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__StringPtr Vec__StringPtr__iter_ref(Vec__StringPtr* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__StringPtr){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__push(Vec__StringPtr* self, String* item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__insert(Vec__StringPtr* self, size_t idx, String* item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(String*)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__pop(Vec__StringPtr* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__StringPtr Vec__StringPtr__pop_opt(Vec__StringPtr* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__StringPtr__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__StringPtr__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__remove(Vec__StringPtr* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    String* item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(String*)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__append(Vec__StringPtr* self, Vec__StringPtr other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(String*)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__StringPtr__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__get(Vec__StringPtr* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__index(Vec__StringPtr* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String** Vec__StringPtr__get_ref(Vec__StringPtr* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__last(Vec__StringPtr* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__StringPtr__length(Vec__StringPtr* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__StringPtr__contains(Vec__StringPtr* self, String* item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(String*)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__StringPtr__is_empty(Vec__StringPtr* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__clear(Vec__StringPtr* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__free(Vec__StringPtr* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__StringPtr__first(Vec__StringPtr* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__set(Vec__StringPtr* self, size_t idx, String* item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__reverse(Vec__StringPtr* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    String* tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__StringPtr__eq(Vec__StringPtr* self, Vec__StringPtr* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(String*)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__forget(Vec__StringPtr* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__StringPtr Vec__StringPtr__add(Vec__StringPtr* self, Vec__StringPtr* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__StringPtr result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__append(&result, Vec__StringPtr__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__StringPtr__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__StringPtr__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__add_assign(Vec__StringPtr* self, Vec__StringPtr* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__append(self, Vec__StringPtr__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__StringPtr__neq(Vec__StringPtr* self, Vec__StringPtr* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__shl(Vec__StringPtr* self, String* item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__shr(Vec__StringPtr* self, String** out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__StringPtr Vec__StringPtr__mul(Vec__StringPtr* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__StringPtr result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__append(&result, Vec__StringPtr__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__StringPtr__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__StringPtr__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__mul_assign(Vec__StringPtr* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(String*)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__StringPtr Vec__StringPtr__clone(Vec__StringPtr* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__StringPtr){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    String** new_data = ((String**)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(String*)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__StringPtr){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__StringPtr VecIterRef__StringPtr__next(VecIterRef__StringPtr* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct String** item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__StringPtr){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__StringPtr){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__StringPtr VecIterRef__StringPtr__iterator(VecIterRef__StringPtr* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__StringPtr__is_none(VecIterResult__StringPtr* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

struct String** VecIterResult__StringPtr__unwrap(VecIterResult__StringPtr* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__StringPtr VecIter__StringPtr__next(VecIter__StringPtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    struct String* item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__StringPtr__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__StringPtr__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__StringPtr VecIter__StringPtr__iterator(VecIter__StringPtr* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__StringPtr Option__StringPtr__Some(struct String* v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__StringPtr){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__StringPtr Option__StringPtr__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__StringPtr opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(struct String*));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__StringPtr__is_some(Option__StringPtr* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__StringPtr__is_none(Option__StringPtr* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__StringPtr__forget(Option__StringPtr* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(struct String*));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct String* Option__StringPtr__unwrap(Option__StringPtr* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    struct String* v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct String** Option__StringPtr__unwrap_ref(Option__StringPtr* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct String* Option__StringPtr__unwrap_or(Option__StringPtr* self, struct String* def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

struct String* Option__StringPtr__expect(Option__StringPtr* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__StringPtr Option__StringPtr__or_else(Option__StringPtr* self, Option__StringPtr other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__StringPtr__Drop__drop(Vec__StringPtr* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__StringPtr__free(self);
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__bool Result__bool__Ok(bool v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (Result__bool){.is_ok = true, .val = v, .err = NULL};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__bool Result__bool__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    Result__bool r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(bool));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return r;
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__bool__is_ok(Result__bool* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__bool__is_err(Result__bool* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__bool__forget(Result__bool* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(bool));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__bool__unwrap(Result__bool* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    bool v = self->val;

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return v;
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool* Result__bool__unwrap_ref(Result__bool* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__bool__expect(Result__bool* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__String Result__String__Ok(String v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    { Result__String _z_ret = (Result__String){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; }), .err = NULL}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__String Result__String__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_r = 1; Result__String r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(String));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = r; memset(&r, 0, sizeof(_z_ret_mv)); __z_drop_flag_r = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__String__Drop__glue(&r);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__String__Drop__glue(&r);
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__String__is_ok(Result__String* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__String__is_err(Result__String* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__String__forget(Result__String* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(String));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

String Result__String__unwrap(Result__String* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

String* Result__String__unwrap_ref(Result__String* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

String Result__String__expect(Result__String* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__File Result__File__Ok(File v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    { Result__File _z_ret = (Result__File){.is_ok = true, .val = ({ __z_drop_flag_v = 0; v; }), .err = NULL}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

Result__File Result__File__Err(char* e)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_r = 1; Result__File r = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.is_ok = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
(r.err = e);

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&r.val), 0, sizeof(File));

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = r; memset(&r, 0, sizeof(_z_ret_mv)); __z_drop_flag_r = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__File__Drop__glue(&r);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_r) Result__File__Drop__glue(&r);
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__File__is_ok(Result__File* self)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->is_ok;
    }
}

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

bool Result__File__is_err(Result__File* self)
{
    {

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (!self->is_ok);
    }
}

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

void Result__File__forget(Result__File* self)
{
    {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
memset((&self->val), 0, sizeof(File));
    }
}

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

File Result__File__unwrap(Result__File* self)
{
    {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    int __z_drop_flag_v = 1; File v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    if (__z_drop_flag_v) File__Drop__glue(&v);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

File* Result__File__unwrap_ref(Result__File* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on Err: "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return (&self->val);
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"

File Result__File__expect(Result__File* self, char* msg)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
if ((!self->is_ok))     {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "%s", ": "); fprintf(stderr, "%s", self->err); fprintf(stderr, "\n"); 0; });

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
exit(1);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/result.zc"
    return self->val;
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__String Vec__String__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__String){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__String Vec__String__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__String){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__String){.data = ((String*)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(String))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__grow(Vec__String* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((String*)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__grow_to_fit(Vec__String* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((String*)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(String))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__String Vec__String__iterator(Vec__String* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__String){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__String Vec__String__iter_ref(Vec__String* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__String){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__push(Vec__String* self, String item)
{

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = ({ __z_drop_flag_item = 0; item; }));

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__insert(Vec__String* self, size_t idx, String item)
{

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(String)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__pop(Vec__String* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__String Vec__String__pop_opt(Vec__String* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__String__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__String__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__remove(Vec__String* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; String item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(String)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = item; memset(&item, 0, sizeof(_z_ret_mv)); __z_drop_flag_item = 0; 
#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
_z_ret_mv; });

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__append(Vec__String* self, Vec__String other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(String)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__String__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__get(Vec__String* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__index(Vec__String* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* Vec__String__get_ref(Vec__String* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__last(Vec__String* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__String__length(Vec__String* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__String__contains(Vec__String* self, String item)
{

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(String)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { bool _z_ret = true; 
#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
return _z_ret; }
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { bool _z_ret = false; 
#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
return _z_ret; }
    }

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__String__is_empty(Vec__String* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__clear(Vec__String* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__free(Vec__String* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String Vec__String__first(Vec__String* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__set(Vec__String* self, size_t idx, String item)
{

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = ({ __z_drop_flag_item = 0; item; }));
    }

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__reverse(Vec__String* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_tmp = 1; String tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = ({ __z_drop_flag_tmp = 0; tmp; }));
(i++);
(j--);

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_tmp) String__Drop__glue(&tmp);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__String__eq(Vec__String* self, Vec__String* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(String)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__forget(Vec__String* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__String Vec__String__add(Vec__String* self, Vec__String* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__String result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__append(&result, Vec__String__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__String__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__String__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__add_assign(Vec__String* self, Vec__String* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__append(self, Vec__String__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__String__neq(Vec__String* self, Vec__String* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__shl(Vec__String* self, String item)
{

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1;
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__push(self, ({ __z_drop_flag_item = 0; item; }));
    }

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__shr(Vec__String* self, String* out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__String Vec__String__mul(Vec__String* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__String result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__append(&result, Vec__String__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__String__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__String__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__mul_assign(Vec__String* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(String)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__String Vec__String__clone(Vec__String* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__String){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    String* new_data = ((String*)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(String)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__String){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__String VecIterRef__String__next(VecIterRef__String* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    String* item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__String){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__String){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__String VecIterRef__String__iterator(VecIterRef__String* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__String__is_none(VecIterResult__String* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

String* VecIterResult__String__unwrap(VecIterResult__String* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__String VecIter__String__next(VecIter__String* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_item = 1; String item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    { Option__String _z_ret = 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__String__Some(({ __z_drop_flag_item = 0; item; })); 
#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
return _z_ret; }

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_item) String__Drop__glue(&item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__String__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__String VecIter__String__iterator(VecIter__String* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__String Option__String__Some(String v)
{

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1;
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__String _z_ret = (Option__String){.is_some = true, .val = ({ __z_drop_flag_v = 0; v; })}; 
#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
return _z_ret; }
    }

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__String Option__String__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_opt = 1; Option__String opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(String));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = opt; memset(&opt, 0, sizeof(_z_ret_mv)); __z_drop_flag_opt = 0; 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__String__Drop__glue(&opt);
_z_ret_mv; });

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_opt) Option__String__Drop__glue(&opt);
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__String__is_some(Option__String* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__String__is_none(Option__String* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__String__forget(Option__String* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(String));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

String Option__String__unwrap(Option__String* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_v = 1; String v = self->val;
memset(&self->val, 0, sizeof(self->val));

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
_z_ret_mv; });

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

String* Option__String__unwrap_ref(Option__String* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

String Option__String__unwrap_or(Option__String* self, String def_val)
{

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_def_val = 1;
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { String _z_ret = self->val; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop__glue(&def_val);
return _z_ret; }
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = def_val; memset(&def_val, 0, sizeof(_z_ret_mv)); __z_drop_flag_def_val = 0; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop__glue(&def_val);
_z_ret_mv; });
    }

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_def_val) String__Drop__glue(&def_val);
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

String Option__String__expect(Option__String* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__String Option__String__or_else(Option__String* self, Option__String other)
{

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int __z_drop_flag_other = 1;
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    { Option__String _z_ret = *self; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__String__Drop__glue(&other);
return _z_ret; }
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return ({ ZC_AUTO _z_ret_mv = other; memset(&other, 0, sizeof(_z_ret_mv)); __z_drop_flag_other = 0; 
#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__String__Drop__glue(&other);
_z_ret_mv; });
    }

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    if (__z_drop_flag_other) Option__String__Drop__glue(&other);
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__String__Drop__drop(Vec__String* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__String__free(self);
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__int32_t Vec__int32_t__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__int32_t){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__int32_t Vec__int32_t__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__int32_t){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__int32_t){.data = ((int32_t*)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(int32_t))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__grow(Vec__int32_t* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((int32_t*)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(int32_t))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__grow_to_fit(Vec__int32_t* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((int32_t*)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(int32_t))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__int32_t Vec__int32_t__iterator(Vec__int32_t* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__int32_t){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__int32_t Vec__int32_t__iter_ref(Vec__int32_t* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__int32_t){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__push(Vec__int32_t* self, int32_t item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__insert(Vec__int32_t* self, size_t idx, int32_t item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(int32_t)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__pop(Vec__int32_t* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__int32_t Vec__int32_t__pop_opt(Vec__int32_t* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__int32_t__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__int32_t__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__remove(Vec__int32_t* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int32_t item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(int32_t)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__append(Vec__int32_t* self, Vec__int32_t other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(int32_t)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__int32_t__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__get(Vec__int32_t* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__index(Vec__int32_t* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t* Vec__int32_t__get_ref(Vec__int32_t* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__last(Vec__int32_t* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__int32_t__length(Vec__int32_t* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__int32_t__contains(Vec__int32_t* self, int32_t item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(int32_t)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__int32_t__is_empty(Vec__int32_t* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__clear(Vec__int32_t* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__free(Vec__int32_t* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t Vec__int32_t__first(Vec__int32_t* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__set(Vec__int32_t* self, size_t idx, int32_t item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__reverse(Vec__int32_t* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int32_t tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__int32_t__eq(Vec__int32_t* self, Vec__int32_t* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(int32_t)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__forget(Vec__int32_t* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__int32_t Vec__int32_t__add(Vec__int32_t* self, Vec__int32_t* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__int32_t result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__append(&result, Vec__int32_t__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__int32_t__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__int32_t__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__add_assign(Vec__int32_t* self, Vec__int32_t* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__append(self, Vec__int32_t__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__int32_t__neq(Vec__int32_t* self, Vec__int32_t* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__shl(Vec__int32_t* self, int32_t item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__shr(Vec__int32_t* self, int32_t* out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__int32_t Vec__int32_t__mul(Vec__int32_t* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__int32_t result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__append(&result, Vec__int32_t__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__int32_t__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__int32_t__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__mul_assign(Vec__int32_t* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(int32_t)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__int32_t Vec__int32_t__clone(Vec__int32_t* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__int32_t){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int32_t* new_data = ((int32_t*)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(int32_t)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__int32_t){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__int32_t VecIterRef__int32_t__next(VecIterRef__int32_t* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int32_t* item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__int32_t){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__int32_t){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__int32_t VecIterRef__int32_t__iterator(VecIterRef__int32_t* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__int32_t__is_none(VecIterResult__int32_t* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

int32_t* VecIterResult__int32_t__unwrap(VecIterResult__int32_t* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__int32_t VecIter__int32_t__next(VecIter__int32_t* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int32_t item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__int32_t__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__int32_t__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__int32_t VecIter__int32_t__iterator(VecIter__int32_t* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__int32_t__Drop__drop(Vec__int32_t* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__int32_t__free(self);
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__size_t Vec__size_t__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__size_t){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__size_t Vec__size_t__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__size_t){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__size_t){.data = ((size_t*)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(size_t))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__grow(Vec__size_t* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((size_t*)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(size_t))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__grow_to_fit(Vec__size_t* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((size_t*)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(size_t))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__size_t Vec__size_t__iterator(Vec__size_t* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__size_t){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__size_t Vec__size_t__iter_ref(Vec__size_t* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__size_t){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__push(Vec__size_t* self, size_t item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__insert(Vec__size_t* self, size_t idx, size_t item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(size_t)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__pop(Vec__size_t* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__size_t Vec__size_t__pop_opt(Vec__size_t* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__size_t__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__size_t__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__remove(Vec__size_t* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(size_t)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__append(Vec__size_t* self, Vec__size_t other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(size_t)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__size_t__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__get(Vec__size_t* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__index(Vec__size_t* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t* Vec__size_t__get_ref(Vec__size_t* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__last(Vec__size_t* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__length(Vec__size_t* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__size_t__contains(Vec__size_t* self, size_t item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(size_t)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__size_t__is_empty(Vec__size_t* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__clear(Vec__size_t* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__free(Vec__size_t* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__size_t__first(Vec__size_t* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__set(Vec__size_t* self, size_t idx, size_t item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__reverse(Vec__size_t* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__size_t__eq(Vec__size_t* self, Vec__size_t* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(size_t)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__forget(Vec__size_t* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__size_t Vec__size_t__add(Vec__size_t* self, Vec__size_t* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__size_t result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__append(&result, Vec__size_t__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__size_t__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__size_t__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__add_assign(Vec__size_t* self, Vec__size_t* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__append(self, Vec__size_t__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__size_t__neq(Vec__size_t* self, Vec__size_t* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__shl(Vec__size_t* self, size_t item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__shr(Vec__size_t* self, size_t* out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__size_t Vec__size_t__mul(Vec__size_t* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__size_t result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__append(&result, Vec__size_t__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__size_t__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__size_t__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__mul_assign(Vec__size_t* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(size_t)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__size_t Vec__size_t__clone(Vec__size_t* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__size_t){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t* new_data = ((size_t*)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(size_t)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__size_t){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__size_t VecIterRef__size_t__next(VecIterRef__size_t* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t* item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__size_t){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__size_t){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__size_t VecIterRef__size_t__iterator(VecIterRef__size_t* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__size_t__is_none(VecIterResult__size_t* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t* VecIterResult__size_t__unwrap(VecIterResult__size_t* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__size_t VecIter__size_t__next(VecIter__size_t* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__size_t__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__size_t__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__size_t VecIter__size_t__iterator(VecIter__size_t* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__size_t__Drop__drop(Vec__size_t* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__size_t__free(self);
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__size_t Option__size_t__Some(size_t v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__size_t){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__size_t Option__size_t__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__size_t opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(size_t));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__size_t__is_some(Option__size_t* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__size_t__is_none(Option__size_t* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__size_t__forget(Option__size_t* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(size_t));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

size_t Option__size_t__unwrap(Option__size_t* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    size_t v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

size_t* Option__size_t__unwrap_ref(Option__size_t* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

size_t Option__size_t__unwrap_or(Option__size_t* self, size_t def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

size_t Option__size_t__expect(Option__size_t* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__size_t Option__size_t__or_else(Option__size_t* self, Option__size_t other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__int32_t Option__int32_t__Some(int32_t v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__int32_t){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__int32_t Option__int32_t__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__int32_t opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(int32_t));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__int32_t__is_some(Option__int32_t* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__int32_t__is_none(Option__int32_t* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__int32_t__forget(Option__int32_t* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(int32_t));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

int32_t Option__int32_t__unwrap(Option__int32_t* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    int32_t v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

int32_t* Option__int32_t__unwrap_ref(Option__int32_t* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

int32_t Option__int32_t__unwrap_or(Option__int32_t* self, int32_t def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

int32_t Option__int32_t__expect(Option__int32_t* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__int32_t Option__int32_t__or_else(Option__int32_t* self, Option__int32_t other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__char Vec__char__new(void)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__char){.data = NULL};
    }
}

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__char Vec__char__with_capacity(size_t cap)
{
    {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((cap == 0))     {

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__char){.data = NULL};
    }

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__char){.data = ((char*)(
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((cap * sizeof(char))))), .cap = cap};
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__grow(Vec__char* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }
 else     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((char*)(
#line 91 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(char))))));
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__grow_to_fit(Vec__char* self, size_t new_len)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap >= new_len))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->cap == 0))     {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 8);
    }

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((self->cap < new_len))     {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = (self->cap * 2));
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = ((char*)(
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
realloc(self->data, (self->cap * sizeof(char))))));
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__char Vec__char__iterator(Vec__char* self)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIter__char){.data = self->data, .count = self->len};
    }
}

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__char Vec__char__iter_ref(Vec__char* self)
{
    {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterRef__char){.data = self->data, .count = self->len};
    }
}

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__push(Vec__char* self, char item)
{
    {

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__grow(self);
    }

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[self->len] = item);

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__insert(Vec__char* self, size_t idx, char item)
{
    {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx > self->len))     {

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Insert index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len >= self->cap))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__grow(self);
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < self->len))     {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove(((self->data + idx) + 1), (self->data + idx), ((self->len - idx) * sizeof(char)));
    }

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + 1));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__pop(Vec__char* self)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: pop called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[self->len];
    }
}

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__char Vec__char__pop_opt(Vec__char* self)
{
    {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 158 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__char__None();
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__char__Some(self->data[self->len]);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__remove(Vec__char* self, size_t idx)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Remove index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    char item = self->data[idx];

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx < (self->len - 1)))     {

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memmove((self->data + idx), ((self->data + idx) + 1), (((self->len - idx) - 1) * sizeof(char)));
    }

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len - 1));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return item;
    }
}

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__append(Vec__char* self, Vec__char other)
{

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_other = 1;
    {

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t new_len = (self->len + other.len);

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__grow_to_fit(self, new_len);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + self->len), other.data, (other.len * sizeof(char)));

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = new_len);

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__forget(&other);
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_other) Vec__char__Drop__glue(&other);
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__get(Vec__char* self, size_t idx)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 194 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[idx];
    }
}

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__index(Vec__char* self, size_t idx)
{
    {

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__get(self, idx);
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char* Vec__char__get_ref(Vec__char* self, size_t idx)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: Index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (&self->data[idx]);
    }
}

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__last(Vec__char* self)
{
    {

#line 210 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: last called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[(self->len - 1)];
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

size_t Vec__char__length(Vec__char* self)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->len;
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__char__contains(Vec__char* self, char item)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&item), sizeof(char)) == 0))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
(i++);
    }

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }
}

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__char__is_empty(Vec__char* self)
{
    {

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->len == 0);
    }
}

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__clear(Vec__char* self)
{
    {

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);
    }
}

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__free(Vec__char* self)
{
    {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if (self->data)     {

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
free(self->data);
    }

#line 240 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 245 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char Vec__char__first(Vec__char* self)
{
    {

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: first called on empty Vec"); fprintf(stderr, "\n"); 0; });

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->data[0];
    }
}

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__set(Vec__char* self, size_t idx, char item)
{
    {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((idx >= self->len))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: set index out of bounds"); fprintf(stderr, "\n"); 0; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[idx] = item);
    }
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__reverse(Vec__char* self)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t j = (self->len - 1);

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < j))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    char tmp = self->data[i];

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[i] = self->data[j]);

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data[j] = tmp);
(i++);
(j--);
    }
    }
}

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__char__eq(Vec__char* self, Vec__char* other)
{
    {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len != (*other).len))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcmp((&self->data[i]), (&(*other).data[i]), sizeof(char)) != 0))     {

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return false;
    }

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return true;
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__forget(Vec__char* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->data = NULL);

#line 288 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = 0);

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->cap = 0);
    }
}

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__char Vec__char__add(Vec__char* self, Vec__char* other)
{
    {

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__char result = 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__clone(self);

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__append(&result, Vec__char__clone(other));

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__char__Drop__glue(&result);
_z_ret_mv; });

#line 296 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__char__Drop__glue(&result);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__add_assign(Vec__char* self, Vec__char* other)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__append(self, Vec__char__clone(other));
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool Vec__char__neq(Vec__char* self, Vec__char* other)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (!
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__eq(self, other));
    }
}

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__shl(Vec__char* self, char item)
{
    {

#line 312 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__push(self, item);
    }
}

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__shr(Vec__char* self, char* out_item)
{
    {

#line 317 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((out_item != NULL))     {

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
((*out_item) = 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__pop(self));
    }
 else     {

#line 320 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__pop(self);
    }
    }
}

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__char Vec__char__mul(Vec__char* self, size_t count)
{
    {

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    int __z_drop_flag_result = 1; Vec__char result = 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__with_capacity((self->len * count));

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 0;

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__append(&result, Vec__char__clone(self));

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }

#line 334 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__char__Drop__glue(&result);
_z_ret_mv; });

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    if (__z_drop_flag_result) Vec__char__Drop__glue(&result);
    }
}

#line 340 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__mul_assign(Vec__char* self, size_t count)
{
    {

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 0))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__clear(self);

#line 343 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 345 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((count == 1))     {

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return ;
    }

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t original_len = self->len;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__grow_to_fit(self, (self->len * count));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t c = 1;

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((c < count))     {

#line 352 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
memcpy((self->data + (original_len * c)), self->data, (original_len * sizeof(char)));

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->len = (self->len + original_len));

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(c = (c + 1));
    }
    }
}

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Vec__char Vec__char__clone(Vec__char* self)
{
    {

#line 360 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->len == 0))     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__char){.data = NULL};
    }

#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    char* new_data = ((char*)(
#line 363 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
malloc((self->len * sizeof(char)))));

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    size_t i = 0;

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
while ((i < self->len))     {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(new_data[i] = self->data[i]);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(i = (i + 1));
    }

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (Vec__char){.data = new_data, .len = self->len, .cap = self->len};
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterResult__char VecIterRef__char__next(VecIterRef__char* self)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    char* item = (&self->data[self->idx]);

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__char){.ptr = item};
    }

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (VecIterResult__char){.ptr = NULL};
    }
}

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIterRef__char VecIterRef__char__iterator(VecIterRef__char* self)
{
    {

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

bool VecIterResult__char__is_none(VecIterResult__char* self)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return (self->ptr == NULL);
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

char* VecIterResult__char__unwrap(VecIterResult__char* self)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->ptr == NULL))     {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on null VecIterResult"); fprintf(stderr, "\n"); 0; });

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
exit(1);
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return self->ptr;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

Option__char VecIter__char__next(VecIter__char* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
if ((self->idx < self->count))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    char item = self->data[self->idx];

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
(self->idx = (self->idx + 1));

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 47 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__char__Some(item);
    }

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Option__char__None();
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

VecIter__char VecIter__char__iterator(VecIter__char* self)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
    return *self;
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__char Option__char__Some(char v)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (Option__char){.is_some = true, .val = v};
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__char Option__char__None(void)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    Option__char opt = {0};

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
(opt.is_some = false);

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&opt.val), 0, sizeof(char));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return opt;
    }
}

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__char__is_some(Option__char* self)
{
    {

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->is_some;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

bool Option__char__is_none(Option__char* self)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (!self->is_some);
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

void Option__char__forget(Option__char* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
memset((&self->val), 0, sizeof(char));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char Option__char__unwrap(Option__char* self)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap called on None"); fprintf(stderr, "\n"); 0; });

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    char v = self->val;

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return v;
    }
}

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char* Option__char__unwrap_ref(Option__char* self)
{
    {

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: unwrap_ref called on None"); fprintf(stderr, "\n"); 0; });

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return (&self->val);
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char Option__char__unwrap_or(Option__char* self, char def_val)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return def_val;
    }
}

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

char Option__char__expect(Option__char* self, char* msg)
{
    {

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if ((!self->is_some))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    ({ fprintf(stderr, "%s", "Panic: "); fprintf(stderr, "%s", msg); fprintf(stderr, "\n"); 0; });

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
exit(1);
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return self->val;
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"

Option__char Option__char__or_else(Option__char* self, Option__char other)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
if (self->is_some)     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return *self;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/option.zc"
    return other;
    }
}

#line 378 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"

void Vec__char__Drop__drop(Vec__char* self)
{
    {

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/vec.zc"
Vec__char__free(self);
    }
}

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Vec__String regex_split(char* pattern, char* text)
{
    {

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Regex re = 
#line 204 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile(pattern);

#line 205 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int __z_drop_flag_parts = 1; Vec__String parts = 
#line 205 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__split((&re), text);

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__destroy((&re));

#line 207 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 205 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
_z_ret_mv; });

#line 205 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
    }
}

#line 196 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

int32_t regex_count(char* pattern, char* text)
{
    {

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Regex re = 
#line 197 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile(pattern);

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t count = 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__count((&re), text);

#line 199 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__destroy((&re));

#line 200 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return count;
    }
}

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Option__Match regex_find(char* pattern, char* text)
{
    {

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Regex re = 
#line 190 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile(pattern);

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Option__Match result = 
#line 191 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__find((&re), text);

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__destroy((&re));

#line 193 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return result;
    }
}

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool regex_match(char* pattern, char* text)
{
    {

#line 183 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Regex re = 
#line 183 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile(pattern);

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    bool result = 
#line 184 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__match((&re), text);

#line 185 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__destroy((&re));

#line 186 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return result;
    }
}

#line 8 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"

size_t _map_hash_str(const char* str)
{
    {

#line 9 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t hash = 
#line 9 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
hash_get_seed();

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    size_t i = 0;

#line 12 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
while ((str[i] != 0))     {

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    uint8_t b = ((uint8_t)(str[i]));

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(hash = 
#line 15 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
hash_combine(hash, ((size_t)(b))));

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
(i = (i + 1));
    }

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/map.zc"
    return hash;
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"

inline size_t hash_get_seed(void)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
    return __zen_hash_seed;
    }
}

#line 27 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"

void hash_set_seed(size_t seed)
{
    {

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
(__zen_hash_seed = seed);
    }
}

#line 14 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"

size_t hash_compute(const void* data, size_t len)
{
    {

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
    size_t hash = __zen_hash_seed;

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
    uint8_t* bytes = ((uint8_t*)(data));

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
(hash = 
#line 19 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
hash_combine(hash, ((size_t)(bytes[i]))));
    }

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
    return hash;
    }
}

#line 6 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"

inline size_t hash_combine(size_t a, size_t b)
{
    {

#line 7 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
(a = (a ^ b));

#line 8 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
(a = (a * 435));

#line 9 "/home/zuhaitz/zenc-lang/zenc/std/hash.zc"
    return a;
    }
}

#line 162 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* _z_str_rune(int32_t r)
{
    {

#line 163 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    static char buffer[5] = {0};

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    uint32_t val = ((uint32_t)(r));

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((val < 128))     {

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(0, 5)] = ((char)(val)));

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(1, 5)] = 0);
    }

#line 168 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
 else if ((val < 2048))     {

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(0, 5)] = ((char)((192 | (val >> 6)))));

#line 170 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(1, 5)] = ((char)((128 | (val & 63)))));

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(2, 5)] = 0);
    }

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
 else if ((val < 65536))     {

#line 173 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(0, 5)] = ((char)((224 | (val >> 12)))));

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(1, 5)] = ((char)((128 | ((val >> 6) & 63)))));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(2, 5)] = ((char)((128 | (val & 63)))));

#line 176 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(3, 5)] = 0);
    }
 else     {

#line 178 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(0, 5)] = ((char)((240 | (val >> 18)))));

#line 179 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(1, 5)] = ((char)((128 | ((val >> 12) & 63)))));

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(2, 5)] = ((char)((128 | ((val >> 6) & 63)))));

#line 181 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(3, 5)] = ((char)((128 | (val & 63)))));

#line 182 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(buffer[_z_check_bounds(4, 5)] = 0);
    }

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((char*)(buffer));
    }
}

#line 155 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* utos(uint32_t n)
{
    {

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    static char buffer[32] = {0};

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
sprintf(((char*)(buffer)), "%u", n);

#line 158 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((char*)(buffer));
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* itos_new(int32_t n)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    char* buffer = 
#line 148 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
malloc(32);

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((buffer == NULL))     {

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return NULL;
    }

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
sprintf(buffer, "%d", n);

#line 151 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return buffer;
    }
}

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* itos(int32_t n)
{
    {

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    static char buffer[32] = {0};

#line 142 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
sprintf(((char*)(buffer)), "%d", n);

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((char*)(buffer));
    }
}

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

int32_t read_rune(void)
{
    {

#line 113 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    void* std_in = stdin;

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    ZC_AUTO c1 = 
#line 114 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in);

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((c1 == (-1)))     {

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((int32_t)(0));
    }

#line 117 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    uint8_t first = ((uint8_t)(c1));

#line 118 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    size_t len = 
#line 118 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
_utf8_seq_len(first);

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((len == 1))     {

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((int32_t)(first));
    }

#line 121 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    uint32_t val = 0;

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((len == 2))     {

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (((uint32_t)((first & 31))) << 6));

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | ((uint32_t)((((uint8_t)(
#line 124 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63)))));
    }

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
 else if ((len == 3))     {

#line 126 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (((uint32_t)((first & 15))) << 12));

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | (((uint32_t)((((uint8_t)(
#line 127 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63))) << 6)));

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | ((uint32_t)((((uint8_t)(
#line 128 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63)))));
    }

#line 129 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
 else if ((len == 4))     {

#line 130 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (((uint32_t)((first & 7))) << 18));

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | (((uint32_t)((((uint8_t)(
#line 131 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63))) << 12)));

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | (((uint32_t)((((uint8_t)(
#line 132 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63))) << 6)));

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(val = (val | ((uint32_t)((((uint8_t)(
#line 133 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in))) & 63)))));
    }

#line 135 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((int32_t)(val));
    }
}

#line 103 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

size_t _utf8_seq_len(uint8_t first_byte)
{
    {

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    int32_t b = ((int32_t)(first_byte));

#line 105 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((b & 128) == 0))     {

#line 105 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return 1;
    }

#line 106 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((b & 224) == 192))     {

#line 106 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return 2;
    }

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((b & 240) == 224))     {

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return 3;
    }

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((b & 248) == 240))     {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return 4;
    }

#line 109 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return 1;
    }
}

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* readln(void)
{
    {

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    size_t cap = 64;

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    size_t len = 0;

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    char* line = 
#line 65 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
malloc(cap);

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((line == NULL))     {

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return NULL;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    int c = 0;

#line 69 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    void* std_in = stdin;

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
while (true)     {

#line 72 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(c = 
#line 72 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
fgetc(std_in));

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    int eof_c = (-1);

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((c == eof_c))     {

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
break;
    }

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    int nl_c = 10;

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((c == nl_c))     {

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
break;
    }

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((len + 1) >= cap))     {

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(cap = (cap * 2));

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    void* n = 
#line 81 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
realloc(line, cap);

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((n == NULL))     {

#line 83 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
free(line);

#line 84 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return NULL;
    }

#line 86 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(line = n);
    }

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(line[len] = c);

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(len = (len + 1));
    }

#line 93 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    int32_t eof_final = (-1);

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if (((len == 0) && (c == eof_final)))     {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
free(line);

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return NULL;
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
(line[len] = 0);

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return line;
    }
}

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

int println(char* fmt, ...)
{
    {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    va_list ap = {0};
va_start(ap, fmt);

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    ZC_AUTO ret = 
#line 56 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
vprintf(fmt, ap);
va_end(ap);

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
puts("");

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return (ret + 1);
    }
}

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

int print(char* fmt, ...)
{
    {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    va_list ap = {0};
va_start(ap, fmt);

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    ZC_AUTO ret = 
#line 48 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
vprintf(fmt, ap);
va_end(ap);

#line 50 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ret;
    }
}

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* format_new(char* fmt, ...)
{
    {

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    char* buffer = 
#line 33 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
malloc(1024);

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
if ((buffer == NULL))     {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return NULL;
    }

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    va_list ap = {0};
va_start(ap, fmt);

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
vsnprintf(buffer, 1024, fmt, ap);
va_end(ap);

#line 42 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return buffer;
    }
}

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

int format_into(char* buffer, size_t size, char* fmt, ...)
{
    {

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    va_list ap = {0};
va_start(ap, fmt);

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    ZC_AUTO ret = 
#line 26 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
vsnprintf(buffer, size, fmt, ap);
va_end(ap);

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ret;
    }
}

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"

char* format(char* fmt, ...)
{
    {

#line 12 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    static char buffer[1024] = {0};

#line 13 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    va_list ap = {0};
va_start(ap, fmt);

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
vsnprintf(buffer, 1024, fmt, ap);
va_end(ap);

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/io.zc"
    return ((char*)(buffer));
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

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

Command Command__new(char* program)
{
    {

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    return (struct Command){.program = 
#line 26 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__from(program), .args = 
#line 27 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Vec__String__new()};
    }
}

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

Command* Command__arg(Command* self, char* arg)
{
    {

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Vec__String__push((&self->args), String__from(arg));

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    return self;
    }
}

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

String Command__build_cmd(Command* self)
{
    {

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_cmd_str = 1; String cmd_str = 
#line 37 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__substring((&self->program), 0, String__length((&self->program)));
    {

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    VecIterRef__String __it = 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Vec__String__iter_ref(&self->args);

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
for (; 1; )     {

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    VecIterResult__String __opt = 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterRef__String__next(&__it);

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
if (
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterResult__String__is_none(&__opt))     {

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
break;
    }

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    String* arg = 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterResult__String__unwrap(&__opt);
    {

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_space = 1; String space = 
#line 40 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__from(" ");

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__append((&cmd_str), (&space));

#line 42 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&space));

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__append((&cmd_str), arg);

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_space) String__Drop__glue(&space);
    }
    }
    }

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    return ({ ZC_AUTO _z_ret_mv = cmd_str; memset(&cmd_str, 0, sizeof(_z_ret_mv)); __z_drop_flag_cmd_str = 0; 
#line 37 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
_z_ret_mv; });

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
    }
}

#line 50 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

Output Command__output(Command* self)
{
    {

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_cmd_str = 1; String cmd_str = 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Command__build_cmd(self);

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    char* cmd_c = 
#line 52 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__c_str((&cmd_str));

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    void* fp = 
#line 54 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
_z_popen(cmd_c, "r");

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
if ((fp == NULL))     {

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&cmd_str));

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    { Output _z_ret = (struct Output){.std_out = 
#line 60 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__from(""), .exit_code = (-1)}; 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
return _z_ret; }
    }

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_out = 1; String out = 
#line 65 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__from("");

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    size_t buf_size = 1024;

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    char* buf = ((char*)(
#line 67 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
malloc(buf_size)));

#line 69 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
while (true)     {

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    ZC_AUTO fgets_res = 
#line 70 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
fgets(buf, ((int)(buf_size)), fp);

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
if ((fgets_res == NULL))     {

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
break;
    }

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_chunk = 1; String chunk = 
#line 73 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__from(buf);

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__append((&out), (&chunk));

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&chunk));

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_chunk) String__Drop__glue(&chunk);
    }

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int code = 
#line 78 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
_z_pclose(fp);

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
free(buf);

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&cmd_str));

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    { Output _z_ret = (struct Output){.std_out = ({ __z_drop_flag_out = 0; out; }), .exit_code = code}; 
#line 65 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_out) String__Drop__glue(&out);

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
return _z_ret; }

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_out) String__Drop__glue(&out);

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

int32_t Command__status(Command* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    int __z_drop_flag_cmd_str = 1; String cmd_str = 
#line 89 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Command__build_cmd(self);

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    ZC_AUTO code = 
#line 90 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
system(String__c_str((&cmd_str)));

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&cmd_str));

#line 92 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    { int32_t _z_ret = code; 
#line 89 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
return _z_ret; }

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    if (__z_drop_flag_cmd_str) String__Drop__glue(&cmd_str);
    }
}

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"

void Command__free(Command* self)
{
    {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free((&self->program));
    {

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    VecIterRef__String __it = 
#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Vec__String__iter_ref(&self->args);

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
for (; 1; )     {

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    VecIterResult__String __opt = 
#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterRef__String__next(&__it);

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
if (
#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterResult__String__is_none(&__opt))     {

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
break;
    }

#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
    String* s = 
#line 98 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
VecIterResult__String__unwrap(&__opt);
    {

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
String__free(s);
    }
    }
    }

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/process.zc"
Vec__String__free((&self->args));
    }
}

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Regex Regex__compile(char* pattern)
{
    {

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile_with_flags(pattern, (1 | 2));
    }
}

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Regex Regex__compile_with_flags(char* pattern, int32_t flags)
{
    {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    void* preg = 
#line 53 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
malloc(1024);

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZC_AUTO status = 
#line 54 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regcomp(preg, pattern, flags);

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((status != 0))     {

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
free(preg);

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (struct Regex){.preg = NULL, .pattern = NULL, .flags = flags};
    }

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (struct Regex){.preg = preg, .pattern = pattern, .flags = flags};
    }
}

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__is_valid(Regex* self)
{
    {

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (self->preg != NULL);
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__match(Regex* self, char* text)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg == NULL))     {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return false;
    }

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (
#line 68 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regexec(self->preg, text, 0, 0, 0) == 0);
    }
}

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__match_full(Regex* self, char* text)
{
    {

#line 72 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 72 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__match(self, text);
    }
}

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__match_at(Regex* self, char* text, int32_t offset)
{
    {

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg == NULL))     {

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return false;
    }

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZC_AUTO len = 
#line 77 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
strlen(text);

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if (((offset < 0) || (offset > len)))     {

#line 78 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return false;
    }

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (
#line 79 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regexec(self->preg, (text + offset), 0, 0, 0) == 0);
    }
}

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__is_match(Regex* self, char* text)
{
    {

#line 83 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 83 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__match(self, text);
    }
}

#line 86 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Option__Match Regex__find(Regex* self, char* text)
{
    {

#line 87 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg == NULL))     {

#line 87 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 87 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Option__Match__None();
    }

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZRegMatch pmatch[1] = {0};

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((
#line 90 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regexec(self->preg, text, 1, ((void*)(pmatch)), 0) == 0))     {

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t start = pmatch[_z_check_bounds(0, 1)].rm_so;

#line 92 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t end = pmatch[_z_check_bounds(0, 1)].rm_eo;

#line 93 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((start != (-1)))     {

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 94 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Option__Match__Some(Match__new(text, start, (end - start)));
    }
    }

#line 97 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 97 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Option__Match__None();
    }
}

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Option__Match Regex__find_at(Regex* self, char* text, int32_t start)
{
    {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZC_AUTO len = 
#line 101 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
strlen(text);

#line 102 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if (((start < 0) || (start >= len)))     {

#line 103 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 103 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Option__Match__None();
    }

#line 105 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 
#line 105 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__find(self, (text + start));
    }
}

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

int32_t Regex__count(Regex* self, char* text)
{
    {

#line 109 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg == NULL))     {

#line 109 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return 0;
    }

#line 110 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t count = 0;

#line 111 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t pos = 0;

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZC_AUTO t_len = 
#line 112 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
strlen(text);

#line 113 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
while ((pos < t_len))     {

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    char* sub = (text + pos);

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((
#line 115 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regexec(self->preg, sub, 0, 0, 0) == 0))     {

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(count = (count + 1));

#line 117 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(pos = (pos + 1));
    }
 else     {

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
break;
    }
    }

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return count;
    }
}

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Vec__String Regex__split(Regex* self, char* text)
{
    {

#line 126 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int __z_drop_flag_parts = 1; Vec__String parts = 
#line 126 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__String__new();

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg == NULL))     {

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__String__push((&parts), String__from(text));

#line 129 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 126 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
_z_ret_mv; });
    }

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    ZC_AUTO t_len = 
#line 131 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
strlen(text);

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t last_pos = 0;

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t pos = 0;

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
while ((pos < t_len))     {

#line 135 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    char* sub = (text + pos);

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((
#line 136 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regexec(self->preg, sub, 0, 0, 0) == 0))     {

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((pos > last_pos))     {

#line 138 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    char* before = (text + last_pos);

#line 139 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int32_t part_len = (pos - last_pos);

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 140 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__char__new();

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
for (int i = (int)(0); i < part_len; i = (i + 1))     {

#line 142 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__char__push((&v), before[i]);
    }

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__char__push((&v), 0);

#line 145 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__String__push((&parts), (struct String){.vec = ({ __z_drop_flag_v = 0; v; })});

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(last_pos = (pos + 1));

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(pos = (pos + 1));
    }
 else     {

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(pos = (pos + 1));
    }
    }

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((last_pos < t_len))     {

#line 154 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Vec__String__push((&parts), String__from((text + last_pos)));
    }

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 126 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
_z_ret_mv; });

#line 126 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
    }
}

#line 159 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

char* Regex__pattern(Regex* self)
{
    {

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return self->pattern;
    }
}

#line 163 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

int32_t Regex__flags(Regex* self)
{
    {

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return self->flags;
    }
}

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

bool Regex__is_valid_pattern(char* pattern)
{
    {

#line 168 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    Regex test_regex = 
#line 168 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__compile(pattern);

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    bool valid = 
#line 169 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__is_valid((&test_regex));

#line 170 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
Regex__destroy(&test_regex);

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return valid;
    }
}

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

void Regex__destroy(Regex* self)
{
    {

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
if ((self->preg != NULL))     {

#line 176 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
tre_regfree(self->preg);

#line 177 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
free(self->preg);
    }
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

Match Match__new(char* text, int32_t start, int32_t len)
{
    {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (struct Match){.text = text, .start = start, .len = len};
    }
}

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

char* Match__as_string(Match* self)
{
    {

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    char* s = 
#line 30 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
malloc((self->len + 1));

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
memcpy(s, (self->text + self->start), self->len);

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
(s[self->len] = 0);

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return s;
    }
}

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"

int32_t Match__end(Match* self)
{
    {

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/regex.zc"
    return (self->start + self->len);
    }
}

#line 23 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"

Option__string Env__get(string name)
{
    {

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    string value = 
#line 24 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
getenv(name);

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
if ((value == NULL))     {

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return 
#line 26 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
Option__string__None();
    }

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return 
#line 29 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
Option__string__Some(value);
    }
}

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"

Option__String Env__get_dup(string name)
{
    {

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    string value = 
#line 33 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
getenv(name);

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
if ((value == NULL))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return 
#line 35 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
Option__String__None();
    }

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    int __z_drop_flag_v = 1; String v = 
#line 38 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
String__from(value);

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    int __z_drop_flag_o = 1; Option__String o = 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
Option__String__Some(({ __z_drop_flag_v = 0; v; }));

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
String__forget((&v));

#line 42 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return ({ ZC_AUTO _z_ret_mv = o; memset(&o, 0, sizeof(_z_ret_mv)); __z_drop_flag_o = 0; 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    if (__z_drop_flag_o) Option__String__Drop__glue(&o);

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
_z_ret_mv; });

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    if (__z_drop_flag_o) Option__String__Drop__glue(&o);

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    if (__z_drop_flag_v) String__Drop__glue(&v);
    }
}

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"

EnvRes Env__set(string name, string value)
{
    {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    int32_t ret = 
#line 46 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
_z_setenv(name, value, 1);

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
if ((ret == 0))     {

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
EnvRes__OK();
    }

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
EnvRes__ERR();
    }
}

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"

EnvRes Env__unset(string name)
{
    {

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    int32_t ret = 
#line 55 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
_z_unsetenv(name);

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return (((ret == 0)) ? (
#line 57 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
EnvRes__OK()) : (EnvRes__ERR()));
    }
}

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"

bool EnvRes__eq(EnvRes* self, EnvRes* other)
{
    {

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/env.zc"
    return (self->tag == other->tag);
    }
}

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

void Arena__Drop__drop(Arena* self)
{
    {

#line 83 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
Arena__free(self);
    }
}

#line 10 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

Arena Arena__new(size_t cap)
{
    {

#line 11 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    void* ptr = 
#line 11 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
malloc(cap);

#line 12 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return (struct Arena){.data = ptr, .capacity = cap};
    }
}

#line 15 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

void* Arena__alloc_bytes(Arena* self, size_t size)
{
    {

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    size_t aligned = ((self->used + 7) & (~7));

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
if (((aligned + size) > self->capacity))     {

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return 0;
    }

#line 20 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    char* ptr = (((char*)(self->data)) + aligned);

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
(self->used = (aligned + size));

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return ptr;
    }
}

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

#line 42 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

char* Arena__dup_str(Arena* self, char* src)
{
    {

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    ZC_AUTO len = 
#line 43 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
strlen(src);

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    char* ptr = ((char*)(
#line 44 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
Arena__alloc_bytes(self, (len + 1))));

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
if ((ptr != NULL))     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
strcpy(ptr, src);
    }

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return ptr;
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

size_t Arena__bytes_used(Arena* self)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return self->used;
    }
}

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

size_t Arena__bytes_free(Arena* self)
{
    {

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return (self->capacity - self->used);
    }
}

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

size_t Arena__save(Arena* self)
{
    {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
    return self->used;
    }
}

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

void Arena__restore(Arena* self, size_t mark)
{
    {

#line 64 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
if ((mark <= self->used))     {

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
(self->used = mark);
    }
    }
}

#line 69 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

void Arena__reset(Arena* self)
{
    {

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
(self->used = 0);
    }
}

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"

void Arena__free(Arena* self)
{
    {

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
if ((self->data != NULL))     {

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
free(self->data);

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/arena.zc"
(self->data = NULL);
    }
    }
}

#line 12 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Path Path__new(char* s)
{
    {

#line 13 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return (struct Path){.str = 
#line 13 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__new(s)};
    }
}

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Path Path__from_string(String s)
{

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    int __z_drop_flag_s = 1;
    {

#line 17 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    { Path _z_ret = (struct Path){.str = ({ __z_drop_flag_s = 0; s; })}; 
#line 16 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
return _z_ret; }
    }

#line 16 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
}

#line 20 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

char* Path__c_str(Path* self)
{
    {

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 21 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__c_str((&self->str));
    }
}

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Path Path__clone(Path* self)
{
    {

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return (struct Path){.str = 
#line 25 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__new(String__c_str((&self->str)))};
    }
}

#line 28 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Path Path__join(Path* self, char* other)
{
    {

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t base_len = 
#line 29 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__length((&self->str));

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    int __z_drop_flag_new_s = 1; String new_s = 
#line 30 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__from(String__c_str((&self->str)));

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((base_len > 0))     {

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    char last = 
#line 33 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Vec__char__get((&self->str.vec), (base_len - 1));

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if (((last != '/') && (last != '\\')))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    int __z_drop_flag_sep = 1; String sep = 
#line 35 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__new("/");

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__append((&new_s), (&sep));

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_sep) String__Drop__glue(&sep);
    }
    }

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    int __z_drop_flag_other_str = 1; String other_str = 
#line 40 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__new(other);

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__append((&new_s), (&other_str));

#line 43 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    { Path _z_ret = (struct Path){.str = ({ __z_drop_flag_new_s = 0; new_s; })}; 
#line 40 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_other_str) String__Drop__glue(&other_str);

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_new_s) String__Drop__glue(&new_s);
return _z_ret; }

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_other_str) String__Drop__glue(&other_str);

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_new_s) String__Drop__glue(&new_s);
    }
}

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Option__String Path__extension(Path* self)
{
    {

#line 48 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    char* s = 
#line 48 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Path__c_str(self);

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t len = 
#line 49 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__length((&self->str));

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
for (size_t i = (size_t)(len); i > 0; i = (i - 1))     {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    char c = s[(i - 1)];

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((c == '.'))     {

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 57 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__Some(String__substring((&self->str), i, (len - i)));
    }

#line 59 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if (((c == '/') || (c == '\\')))     {

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 60 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__None();
    }
    }

#line 63 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 63 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__None();
    }
}

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Option__String Path__file_name(Path* self)
{
    {

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    char* s = 
#line 67 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Path__c_str(self);

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t len = 
#line 68 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__length((&self->str));

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((len == 0))     {

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 70 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__None();
    }

#line 73 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t end = len;

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
while (((end > 0) && ((s[(end - 1)] == '/') || (s[(end - 1)] == '\\'))))     {

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
(end = (end - 1));
    }

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((end == 0))     {

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 77 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__None();
    }

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t start = end;

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
while ((((start > 0) && (s[(start - 1)] != '/')) && (s[(start - 1)] != '\\')))     {

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
(start = (start - 1));
    }

#line 84 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 84 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__String__Some(String__substring((&self->str), start, (end - start)));
    }
}

#line 87 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

Option__Path Path__parent(Path* self)
{
    {

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    char* s = 
#line 88 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Path__c_str(self);

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t len = 
#line 89 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__length((&self->str));

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((len == 0))     {

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 90 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__Path__None();
    }

#line 93 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    size_t end = len;

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
while (((end > 0) && ((s[(end - 1)] == '/') || (s[(end - 1)] == '\\'))))     {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
(end = (end - 1));
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
while ((((end > 0) && (s[(end - 1)] != '/')) && (s[(end - 1)] != '\\')))     {

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
(end = (end - 1));
    }

#line 103 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((end == 0))     {

#line 103 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 103 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__Path__None();
    }

#line 105 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
while (((end > 0) && ((s[(end - 1)] == '/') || (s[(end - 1)] == '\\'))))     {

#line 106 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
(end = (end - 1));
    }

#line 109 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
if ((end == 0))     {

#line 111 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    return 
#line 111 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__Path__Some(Path__new("/"));
    }

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    int __z_drop_flag_parent_str = 1; String parent_str = 
#line 114 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__substring((&self->str), 0, end);

#line 115 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    { Option__Path _z_ret = 
#line 115 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
Option__Path__Some((struct Path){.str = ({ __z_drop_flag_parent_str = 0; parent_str; })}); 
#line 114 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_parent_str) String__Drop__glue(&parent_str);
return _z_ret; }

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
    if (__z_drop_flag_parent_str) String__Drop__glue(&parent_str);
    }
}

#line 118 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"

void Path__free(Path* self)
{
    {

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/path.zc"
String__free((&self->str));
    }
}

#line 475 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

String JsonValue__to_string(JsonValue* self)
{
    {

#line 476 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    int __z_drop_flag_s = 1; String s = 
#line 476 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__new("");

#line 477 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__stringify(self, (&s));

#line 478 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 476 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
_z_ret_mv; });

#line 476 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }
}

#line 481 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

void JsonValue__stringify(JsonValue* self, String* buf)
{
    {

#line 482 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 482 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NULL().tag))     {

#line 483 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "null");
    }

#line 484 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((self->kind.tag == 
#line 484 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_BOOL().tag))     {

#line 485 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (self->bool_val)     {

#line 485 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "true");
    }
 else     {
String__append_c_ptr(buf, "false");
    }
    }

#line 486 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((self->kind.tag == 
#line 486 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NUMBER().tag))     {

#line 487 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    char tmp[64] = {0};

#line 488 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
sprintf(((char*)(tmp)), "%.15g", self->number_val);

#line 489 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, ((char*)(tmp)));
    }

#line 490 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((self->kind.tag == 
#line 490 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_STRING().tag))     {

#line 491 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\"");

#line 492 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    char* p = self->string_val;

#line 493 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    ZC_AUTO len = 
#line 493 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
strlen(p);

#line 494 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 495 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    char c = p[i];

#line 496 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((c == '"'))     {

#line 496 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\\"");
    }

#line 497 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\\'))     {

#line 497 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\\\");
    }

#line 498 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\n'))     {

#line 498 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\n");
    }

#line 499 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\t'))     {

#line 499 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\t");
    }

#line 500 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\r'))     {

#line 500 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\r");
    }

#line 501 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\b'))     {

#line 501 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\b");
    }

#line 502 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((c == '\f'))     {

#line 502 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\\f");
    }
 else     {

#line 504 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    char tmp[2] = {0};

#line 504 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
(tmp[_z_check_bounds(0, 2)] = c);
(tmp[_z_check_bounds(1, 2)] = 0);

#line 505 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, ((char*)(tmp)));
    }
    }

#line 508 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\"");
    }

#line 509 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((self->kind.tag == 
#line 509 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 510 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "[");

#line 511 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Vec__JsonValuePtr* v = self->array_val;

#line 512 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
for (size_t i = (size_t)(0); i < 
#line 512 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__length(v); i = (i + 1))     {

#line 513 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((i > 0))     {

#line 513 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, ",");
    }

#line 514 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* item = 
#line 514 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__get(v, i);

#line 515 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__stringify((JsonValue[]){(*item)}, buf);
    }

#line 517 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "]");
    }

#line 518 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
 else if ((self->kind.tag == 
#line 518 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 519 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, ({ 
#line 519 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    static char _fs_buf_0[4096]; _fs_buf_0[0]=0;
    char _fs_t_0[128];
    strcat(_fs_buf_0, "{");
    _fs_buf_0;
 }));

#line 520 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Map__JsonValuePtr* m = self->object_val;

#line 521 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    bool first = true;

#line 522 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
for (size_t i = (size_t)(0); i < 
#line 522 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__capacity(m); i = (i + 1))     {

#line 523 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 523 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__is_slot_occupied(m, i))     {

#line 524 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((!first))     {

#line 524 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, ",");
    }

#line 525 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
(first = false);

#line 526 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    char* key = 
#line 526 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__key_at(m, i);

#line 527 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\"");

#line 528 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, key);

#line 529 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "\":");

#line 530 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* val = 
#line 530 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__val_at(m, i);

#line 531 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__stringify(val, buf);
    }
    }

#line 534 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
String__append_c_ptr(buf, "}");
    }
    }
}

#line 469 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

void JsonValue__Drop__drop(JsonValue* self)
{
    {

#line 470 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__free(self);
    }
}

#line 179 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__null(void)
{
    {

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 180 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NULL(), .string_val = NULL, .number_val = 0.0, .bool_val = false, .array_val = NULL, .object_val = NULL};
    }
}

#line 183 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__bool(bool b)
{
    {

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 184 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_BOOL(), .string_val = NULL, .number_val = 0.0, .bool_val = b, .array_val = NULL, .object_val = NULL};
    }
}

#line 187 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__number(double n)
{
    {

#line 188 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 188 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NUMBER(), .string_val = NULL, .number_val = n, .bool_val = false, .array_val = NULL, .object_val = NULL};
    }
}

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__string(char* s)
{
    {

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 192 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_STRING(), .string_val = strdup(s), .number_val = 0.0, .bool_val = false, .array_val = NULL, .object_val = NULL};
    }
}

#line 195 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__array(void)
{
    {

#line 196 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Vec__JsonValuePtr* v = 
#line 196 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(Vec__JsonValuePtr));

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*v) = 
#line 197 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__new());

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 198 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY(), .string_val = NULL, .number_val = 0.0, .bool_val = false, .array_val = v, .object_val = NULL};
    }
}

#line 201 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue JsonValue__object(void)
{
    {

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Map__JsonValuePtr* m = 
#line 202 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(Map__JsonValuePtr));

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*m) = 
#line 203 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__new());

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (struct JsonValue){.kind = 
#line 204 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT(), .string_val = NULL, .number_val = 0.0, .bool_val = false, .array_val = NULL, .object_val = m};
    }
}

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__null_ptr(void)
{
    {

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 212 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 213 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 213 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__null());

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__bool_ptr(bool b)
{
    {

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 218 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 219 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 219 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__bool(b));

#line 220 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__number_ptr(double n)
{
    {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 224 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 225 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 225 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__number(n));

#line 226 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 229 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__string_ptr(char* s)
{
    {

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 230 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 231 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__string(s));

#line 232 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 235 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__array_ptr(void)
{
    {

#line 236 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 236 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 237 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 237 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__array());

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

JsonValue* JsonValue__object_ptr(void)
{
    {

#line 242 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 242 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 243 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = 
#line 243 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__object());

#line 244 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return p;
    }
}

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

void JsonValue__push(JsonValue* self, JsonValue val)
{

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    int __z_drop_flag_val = 1;
    {

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag != 
#line 248 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
    return;
    }

#line 249 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 249 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = ({ __z_drop_flag_val = 0; val; }));

#line 251 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__push(self->array_val, p);
    }

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
}

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

void JsonValue__set(JsonValue* self, char* key, JsonValue val)
{

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    int __z_drop_flag_val = 1;
    {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag != 
#line 255 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
    return;
    }

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* p = 
#line 256 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
malloc(sizeof(JsonValue));

#line 257 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
((*p) = ({ __z_drop_flag_val = 0; val; }));

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__put(self->object_val, key, p);
    }

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
}

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Result__JsonValuePtr JsonValue__parse(char* json)
{
    {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* result = 
#line 262 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
_json_do_parse(json);

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((result != NULL))     {

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 264 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Result__JsonValuePtr__Ok(result);
    }

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 266 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Result__JsonValuePtr__Err("JSON parse error");
    }
}

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Result__JsonValue JsonValue__parse_val(char* json)
{
    {

#line 270 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* result = 
#line 270 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
_json_do_parse(json);

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((result != NULL))     {

#line 272 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    int __z_drop_flag_val = 1; JsonValue val = {0};

#line 273 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    
                val = *result;
            

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(result);

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    { Result__JsonValue _z_ret = 
#line 277 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Result__JsonValue__Ok(({ __z_drop_flag_val = 0; val; })); 
#line 272 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
return _z_ret; }

#line 272 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    if (__z_drop_flag_val) JsonValue__Drop__glue(&val);
    }

#line 279 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 279 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Result__JsonValue__Err("JSON parse error");
    }
}

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_null(JsonValue* self)
{
    {

#line 287 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 287 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NULL().tag);
    }
}

#line 290 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_bool(JsonValue* self)
{
    {

#line 291 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 291 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_BOOL().tag);
    }
}

#line 294 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_number(JsonValue* self)
{
    {

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 295 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NUMBER().tag);
    }
}

#line 298 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_string(JsonValue* self)
{
    {

#line 299 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 299 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_STRING().tag);
    }
}

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_array(JsonValue* self)
{
    {

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 303 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag);
    }
}

#line 306 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__is_object(JsonValue* self)
{
    {

#line 307 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->kind.tag == 
#line 307 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag);
    }
}

#line 314 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__charPtr JsonValue__as_string(JsonValue* self)
{
    {

#line 315 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 315 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_STRING().tag))     {

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 316 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__charPtr__Some(self->string_val);
    }

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 318 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__charPtr__None();
    }
}

#line 321 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__int32_t JsonValue__as_int(JsonValue* self)
{
    {

#line 322 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 322 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NUMBER().tag))     {

#line 323 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 323 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__int32_t__Some(((int32_t)(self->number_val)));
    }

#line 325 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 325 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__int32_t__None();
    }
}

#line 328 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__double JsonValue__as_float(JsonValue* self)
{
    {

#line 329 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 329 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_NUMBER().tag))     {

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 330 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__double__Some(self->number_val);
    }

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 332 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__double__None();
    }
}

#line 335 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__bool JsonValue__as_bool(JsonValue* self)
{
    {

#line 336 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 336 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_BOOL().tag))     {

#line 337 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 337 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__bool__Some(self->bool_val);
    }

#line 339 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 339 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__bool__None();
    }
}

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__JsonValuePtr JsonValue__get(JsonValue* self, char* key)
{
    {

#line 347 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag != 
#line 347 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 348 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 350 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__contains(self->object_val, key))     {

#line 351 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 351 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__get(self->object_val, key);
    }

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 353 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }
}

#line 356 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__charPtr JsonValue__get_string(JsonValue* self, char* key)
{
    {

#line 357 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 357 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 358 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 358 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 359 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__charPtr__None();
    }

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 361 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 362 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 362 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__as_string((&(*val)));
    }
}

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__int32_t JsonValue__get_int(JsonValue* self, char* key)
{
    {

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 366 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 367 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 368 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 368 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__int32_t__None();
    }

#line 370 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 370 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 371 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 371 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__as_int((&(*val)));
    }
}

#line 374 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__double JsonValue__get_float(JsonValue* self, char* key)
{
    {

#line 375 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 375 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 376 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 376 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 377 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 377 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__double__None();
    }

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 379 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 380 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 380 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__as_float((&(*val)));
    }
}

#line 383 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__bool JsonValue__get_bool(JsonValue* self, char* key)
{
    {

#line 384 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 384 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 385 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 385 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 386 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 386 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__bool__None();
    }

#line 388 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 388 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 389 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 389 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__as_bool((&(*val)));
    }
}

#line 392 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__JsonValuePtr JsonValue__get_object(JsonValue* self, char* key)
{
    {

#line 393 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 393 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 394 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 394 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 395 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 395 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }

#line 397 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 397 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 398 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (((*val).kind.tag == 
#line 398 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 399 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 399 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__Some(val);
    }

#line 401 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 401 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }
}

#line 404 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__JsonValuePtr JsonValue__get_array(JsonValue* self, char* key)
{
    {

#line 405 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Option__JsonValuePtr opt = 
#line 405 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__get(self, key);

#line 406 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 406 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__is_none((&opt)))     {

#line 407 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 407 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }

#line 409 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    struct JsonValue* val = 
#line 409 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__unwrap((&opt));

#line 410 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (((*val).kind.tag == 
#line 410 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 411 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 411 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__Some(val);
    }

#line 413 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 413 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }
}

#line 420 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

Option__JsonValuePtr JsonValue__at(JsonValue* self, size_t index)
{
    {

#line 421 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag != 
#line 421 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 422 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 422 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }

#line 424 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((index >= 
#line 424 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__length(self->array_val)))     {

#line 425 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 425 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__None();
    }

#line 427 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 427 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Option__JsonValuePtr__Some(Vec__JsonValuePtr__get(self->array_val, index));
    }
}

#line 430 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

size_t JsonValue__len(JsonValue* self)
{
    {

#line 431 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 431 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 432 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 432 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__length(self->array_val);
    }

#line 434 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 434 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 435 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 
#line 435 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__length(self->object_val);
    }

#line 437 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return 0;
    }
}

#line 440 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

void JsonValue__free(JsonValue* self)
{
    {

#line 441 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 441 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_STRING().tag))     {
free(self->string_val);
    }

#line 442 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 442 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_ARRAY().tag))     {

#line 443 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Vec__JsonValuePtr* v = self->array_val;

#line 444 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
for (size_t i = (size_t)(0); i < 
#line 444 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__length(v); i = (i + 1))     {

#line 445 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* item = 
#line 445 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__get(v, i);

#line 446 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__free(item);

#line 447 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(item);
    }

#line 449 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Vec__JsonValuePtr__clear(v);

#line 450 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(v->data);

#line 451 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(v);
    }

#line 453 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if ((self->kind.tag == 
#line 453 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonType__JSON_OBJECT().tag))     {

#line 454 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    Map__JsonValuePtr* m = self->object_val;

#line 455 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
for (size_t i = (size_t)(0); i < 
#line 455 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__capacity(m); i = (i + 1))     {

#line 456 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
if (
#line 456 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__is_slot_occupied(m, i))     {

#line 457 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    JsonValue* child = 
#line 457 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__val_at(m, i);

#line 458 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
JsonValue__free(child);

#line 459 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(child);
    }
    }

#line 462 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
Map__JsonValuePtr__free(m);

#line 463 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
free(m);
    }
    }
}

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonValue__eq(JsonValue* self, JsonValue* other)
{
    {

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return ((((((self->kind.tag == other->kind.tag) && (self->string_val == other->string_val)) && (self->number_val == other->number_val)) && (self->bool_val == other->bool_val)) && (self->array_val == other->array_val)) && (self->object_val == other->object_val));
    }
}

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"

bool JsonType__eq(JsonType* self, JsonType* other)
{
    {

#line 1 "/home/zuhaitz/zenc-lang/zenc/std/json.zc"
    return (self->tag == other->tag);
    }
}

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"

uint64_t Time__now(void)
{
    {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
    return 
#line 46 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
_time_now_impl();
    }
}

#line 49 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"

void Time__sleep(Duration d)
{
    {

#line 50 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
usleep(((uint32_t)((d.millis * ((uint64_t)(1000))))));
    }
}

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"

void Time__sleep_ms(uint64_t ms)
{
    {

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
usleep(((uint32_t)((ms * ((uint64_t)(1000))))));
    }
}

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"

Duration Duration__from_ms(uint64_t ms)
{
    {

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
    return (struct Duration){.millis = ms};
    }
}

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"

Duration Duration__from_secs(uint64_t s)
{
    {

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/time.zc"
    return (struct Duration){.millis = (s * ((uint64_t)(1000)))};
    }
}

#line 280 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

void File__Drop__drop(File* self)
{
    {

#line 281 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__close(self);
    }
}

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__File File__open(char* path, char* mode)
{
    {

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    void* h = 
#line 37 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fopen(path, mode);

#line 38 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((h == NULL))     {

#line 39 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 39 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__Err("Failed to open file");
    }

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 41 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__Ok((struct File){.handle = h});
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

void File__close(File* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if (self->handle)     {

#line 46 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fclose(self->handle);

#line 47 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
(self->handle = NULL);
    }
    }
}

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__String File__read_to_string(File* self)
{
    {

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((self->handle == NULL))     {

#line 53 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 53 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Err("File not open");
    }

#line 56 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fseek(self->handle, 0, 2);

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO size = 
#line 57 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
ftell(self->handle);

#line 58 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fseek(self->handle, 0, 0);

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    char* buffer = 
#line 60 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
malloc((((size_t)(size)) + 1));

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((buffer == NULL))     {

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 62 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Err("Out of memory");
    }

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO read = 
#line 65 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fread(buffer, 1, size, self->handle);

#line 66 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
(buffer[read] = 0);

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 68 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__new(buffer);

#line 69 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
free(buffer);

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__String res = 
#line 71 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Ok(({ __z_drop_flag_s = 0; s; }));

#line 72 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__forget((&s));

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result__String ret = res;
__z_drop_flag_res = 0;

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__forget((&res));

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 74 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
_z_ret_mv; });

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);

#line 68 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }
}

#line 79 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__String File__read_all(char* path)
{
    {

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__File res = 
#line 80 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__open(path, "rb");

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if (
#line 81 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__is_err((&res)))     {

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    { Result__String _z_ret = 
#line 82 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Err(res.err); 
#line 80 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
return _z_ret; }
    }

#line 85 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_f = 1; File f = 
#line 85 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__unwrap((&res));

#line 86 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_s_res = 1; Result__String s_res = 
#line 86 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__read_to_string((&f));

#line 87 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__close((&f));

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result__String ret = s_res;
__z_drop_flag_s_res = 0;

#line 90 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__forget((&s_res));

#line 91 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 89 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 86 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s_res) Result__String__Drop__glue(&s_res);

#line 85 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop__glue(&f);

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
_z_ret_mv; });

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 86 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s_res) Result__String__Drop__glue(&s_res);

#line 85 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop__glue(&f);

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
    }
}

#line 94 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__bool File__write_string(File* self, char* content)
{
    {

#line 95 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((self->handle == NULL))     {

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 96 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err("File not open");
    }

#line 99 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO len = 
#line 99 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
strlen(content);

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO written = 
#line 100 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
fwrite(content, 1, len, self->handle);

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((written != len))     {

#line 102 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 102 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err("Write incomplete");
    }

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 104 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Ok(true);
    }
}

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__Vec__String File__read_lines(char* path)
{
    {

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__String res = 
#line 108 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__read_all(path);

#line 109 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if (
#line 109 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__is_err((&res)))     {

#line 110 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    { Result__Vec__String _z_ret = 
#line 110 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Vec__String__Err(res.err); 
#line 108 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);
return _z_ret; }
    }

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_content = 1; String content = 
#line 112 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__unwrap((&res));

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_lines = 1; Vec__String lines = 
#line 114 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__split((&content), '\n');

#line 117 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
for (size_t i = (size_t)(0); i < lines.len; i = (i + 1))     {

#line 118 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    String* line = 
#line 118 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__String__get_ref((&lines), i);

#line 119 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    size_t len = 
#line 119 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__length(line);

#line 120 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((len > 0))     {

#line 121 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    char last_char = 
#line 121 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__char__get((&line->vec), (len - 1));

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((last_char == '\r'))     {

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_trimmed = 1; String trimmed = 
#line 123 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__substring(line, 0, (len - 1));

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__destroy(line);

#line 125 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__String__set((&lines), i, ({ __z_drop_flag_trimmed = 0; trimmed; }));

#line 123 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_trimmed) String__Drop__glue(&trimmed);
    }
    }
    }

#line 131 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((lines.len > 0))     {

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    size_t last_idx = (lines.len - 1);

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    String* last_line = 
#line 133 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__String__get_ref((&lines), last_idx);

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((
#line 134 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__length(last_line) == 0))     {

#line 135 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_empty = 1; String empty = 
#line 135 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__String__pop((&lines));

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__destroy((&empty));

#line 135 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_empty) String__Drop__glue(&empty);
    }
    }

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__destroy((&content));

#line 142 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret_res = 1; Result__Vec__String ret_res = 
#line 142 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Vec__String__Ok(({ __z_drop_flag_lines = 0; lines; }));

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result__Vec__String ret = ret_res;
__z_drop_flag_ret_res = 0;

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 143 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__Vec__String__Drop__glue(&ret);

#line 142 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret_res) Result__Vec__String__Drop__glue(&ret_res);

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_lines) Vec__String__Drop__glue(&lines);

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_content) String__Drop__glue(&content);

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);
_z_ret_mv; });

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__Vec__String__Drop__glue(&ret);

#line 142 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret_res) Result__Vec__String__Drop__glue(&ret_res);

#line 114 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_lines) Vec__String__Drop__glue(&lines);

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_content) String__Drop__glue(&content);

#line 108 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);
    }
}

#line 147 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__bool File__write_lines(char* path, Vec__String* lines)
{
    {

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__File res = 
#line 148 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__open(path, "wb");

#line 149 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if (
#line 149 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__is_err((&res)))     {

#line 150 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    { Result__bool _z_ret = 
#line 150 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err(res.err); 
#line 148 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
return _z_ret; }
    }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_f = 1; File f = 
#line 152 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__File__unwrap((&res));

#line 154 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
for (size_t i = (size_t)(0); i < lines->len; i = (i + 1))     {

#line 155 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    String* line = 
#line 155 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__String__get_ref(lines, i);

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__write_string((&f), String__c_str(line));

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__write_string((&f), "\n");
    }

#line 160 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
File__close((&f));

#line 161 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    { Result__bool _z_ret = 
#line 161 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Ok(true); 
#line 152 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop__glue(&f);

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
return _z_ret; }

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_f) File__Drop__glue(&f);

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__File__Drop__glue(&res);
    }
}

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

bool File__exists(char* path)
{
    {

#line 165 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int zero = 0;

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return (
#line 166 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
access(path, 0) == zero);
    }
}

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__Metadata File__metadata(char* path)
{
    {

#line 170 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    uint64_t size = {0};

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int is_d = 0;

#line 172 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int is_f = 0;

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int res = 
#line 174 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_get_metadata(path, (&size), (&is_d), (&is_f));

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int non_zero = 0;

#line 176 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((res != non_zero))     {

#line 177 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 177 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Metadata__Err("Failed to get metadata");
    }

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 180 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Metadata__Ok((struct Metadata){.size = ((uint64_t)(size)), .is_dir = (is_d != non_zero), .is_file = (is_f != non_zero)});
    }
}

#line 187 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__bool File__create_dir(char* path)
{
    {

#line 188 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int status = 
#line 188 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_mkdir(path);

#line 189 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int zero = 0;

#line 190 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((status != zero))     {

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 191 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err("Failed to create directory");
    }

#line 193 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 193 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Ok(true);
    }
}

#line 196 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__bool File__remove_file(char* path)
{
    {

#line 197 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO status = 
#line 197 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
unlink(path);

#line 198 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int zero = 0;

#line 199 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((status != zero))     {

#line 200 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 200 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err("Failed to remove file");
    }

#line 202 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 202 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Ok(true);
    }
}

#line 205 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__bool File__remove_dir(char* path)
{
    {

#line 206 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    ZC_AUTO status = 
#line 206 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
rmdir(path);

#line 207 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int zero = 0;

#line 208 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((status != zero))     {

#line 209 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 209 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Err("Failed to remove directory");
    }

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 211 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__bool__Ok(true);
    }
}

#line 214 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__Vec__DirEntry File__read_dir(char* path)
{
    {

#line 215 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    void* dir = 
#line 215 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_opendir(path);

#line 216 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((dir == NULL))     {

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 217 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Vec__DirEntry__Err("Failed to open directory");
    }

#line 220 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_entries = 1; Vec__DirEntry entries = 
#line 220 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__DirEntry__new();

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    char* name_buf = 
#line 221 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
malloc(256);

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((name_buf == NULL))     {

#line 224 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_closedir(dir);

#line 225 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    { Result__Vec__DirEntry _z_ret = 
#line 225 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Vec__DirEntry__Err("Out of memory"); 
#line 220 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec__DirEntry__Drop__glue(&entries);
return _z_ret; }
    }

#line 228 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int is_d = 0;

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int is_d_zero = 0;

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
while (
#line 231 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_read_entry(dir, name_buf, 256, (&is_d)))     {

#line 232 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int zero_cmp = 0;

#line 233 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if (((
#line 233 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
strcmp(name_buf, ".") == zero_cmp) || (strcmp(name_buf, "..") == zero_cmp)))     {

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
continue;
    }

#line 237 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 237 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__new(name_buf);

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ent = 1; DirEntry ent = (struct DirEntry){.name = ({ __z_drop_flag_s = 0; s; }), .is_dir = (is_d != is_d_zero)};

#line 244 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__forget((&s));

#line 246 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Vec__DirEntry__push((&entries), ({ __z_drop_flag_ent = 0; ent; }));

#line 249 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__forget((&ent.name));

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ent) DirEntry__Drop__glue(&ent);

#line 237 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }

#line 252 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
free(name_buf);

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
_z_fs_closedir(dir);

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__Vec__DirEntry res = 
#line 255 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__Vec__DirEntry__Ok(({ __z_drop_flag_entries = 0; entries; }));

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result__Vec__DirEntry ret = res;
__z_drop_flag_res = 0;

#line 257 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 256 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__Vec__DirEntry__Drop__glue(&ret);

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__Vec__DirEntry__Drop__glue(&res);

#line 220 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec__DirEntry__Drop__glue(&entries);
_z_ret_mv; });

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__Vec__DirEntry__Drop__glue(&ret);

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__Vec__DirEntry__Drop__glue(&res);

#line 220 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_entries) Vec__DirEntry__Drop__glue(&entries);
    }
}

#line 260 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"

Result__String File__current_dir(void)
{
    {

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    char* buf = 
#line 261 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
malloc(1024);

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((buf == NULL))     {

#line 262 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 262 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Err("Out of memory");
    }

#line 264 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
if ((
#line 264 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
getcwd(buf, 1024) == NULL))     {

#line 265 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
free(buf);

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return 
#line 266 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Err("Failed to get current directory");
    }

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_s = 1; String s = 
#line 269 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__new(buf);

#line 270 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
free(buf);

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_res = 1; Result__String res = 
#line 271 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__Ok(({ __z_drop_flag_s = 0; s; }));

#line 272 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
String__forget((&s));

#line 273 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    int __z_drop_flag_ret = 1; Result__String ret = res;
__z_drop_flag_res = 0;

#line 274 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
Result__String__forget((&res));

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    return ({ ZC_AUTO _z_ret_mv = ret; memset(&ret, 0, sizeof(_z_ret_mv)); __z_drop_flag_ret = 0; 
#line 273 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
_z_ret_mv; });

#line 273 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_ret) Result__String__Drop__glue(&ret);

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_res) Result__String__Drop__glue(&res);

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/fs.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }
}

#line 50 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__new(char* s)
{
    {

#line 51 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO len = 
#line 51 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(s);

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 52 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 54 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 55 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), s[i]);
    }

#line 57 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 60 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 61 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 62 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c = v.cap;

#line 65 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 67 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c}}; 
#line 52 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 52 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 70 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__from(char* s)
{
    {

#line 71 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 71 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new(s);
    }
}

#line 74 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__from_rune(int32_t r)
{
    {

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 75 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");

#line 76 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__push_rune(&s, r);

#line 77 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 75 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
_z_ret_mv; });

#line 75 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }
}

#line 80 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__from_runes(int32_t* runes, size_t count)
{
    {

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 81 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");

#line 82 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < count; i = (i + 1))     {

#line 83 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__push_rune(&s, runes[i]);
    }

#line 85 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 81 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
_z_ret_mv; });

#line 81 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }
}

#line 88 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

char* String__c_str(String* self)
{
    {

#line 89 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return self->vec.data;
    }
}

#line 92 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

char* String__to_string(String* self)
{
    {

#line 93 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 93 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__c_str(self);
    }
}

#line 96 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__destroy(String* self)
{
    {

#line 97 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__free((&self->vec));
    }
}

#line 100 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__forget(String* self)
{
    {

#line 101 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&self->vec));
    }
}

#line 104 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__append(String* self, String* other)
{
    {

#line 106 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 107 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 110 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t other_len = (*other).vec.len;

#line 111 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (int i = (int)(0); i < other_len; i = (i + 1))     {

#line 112 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), Vec__char__get((&(*other).vec), i));
    }
    }
}

#line 116 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__append_c(String* self, char* s)
{
    {

#line 117 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 118 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 120 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO len = 
#line 120 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(s);

#line 121 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 122 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), s[i]);
    }

#line 124 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), 0);
    }
}

#line 127 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__push_rune(String* self, int32_t r)
{
    {

#line 128 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((self->vec.len > 0))     {

#line 129 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(self->vec.len = (self->vec.len - 1));
    }

#line 132 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint32_t val = ((uint32_t)(r));

#line 133 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((val < 128))     {

#line 134 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)(val)));
    }

#line 135 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((val < 2048))     {

#line 136 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((192 | (val >> 6)))));

#line 137 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | (val & 63)))));
    }

#line 138 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((val < 65536))     {

#line 139 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((224 | (val >> 12)))));

#line 140 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | ((val >> 6) & 63)))));

#line 141 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | (val & 63)))));
    }
 else     {

#line 143 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((240 | (val >> 18)))));

#line 144 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | ((val >> 12) & 63)))));

#line 145 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | ((val >> 6) & 63)))));

#line 146 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), ((char)((128 | (val & 63)))));
    }

#line 148 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&self->vec), 0);
    }
}

#line 151 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__append_c_ptr(String* ptr, char* s)
{
    {

#line 152 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((ptr->vec.len > 0))     {

#line 153 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(ptr->vec.len = (ptr->vec.len - 1));
    }

#line 155 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO len = 
#line 155 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(s);

#line 156 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (int i = (int)(0); i < len; i = (i + 1))     {

#line 157 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&ptr->vec), s[i]);
    }

#line 159 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&ptr->vec), 0);
    }
}

#line 162 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__add(String* self, String* other)
{
    {

#line 163 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_new_s = 1; String new_s = 
#line 163 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__from(String__c_str(self));

#line 164 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__append((&new_s), other);

#line 166 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = new_s.vec.data;

#line 167 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = new_s.vec.len;

#line 168 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c = new_s.vec.cap;

#line 169 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__forget((&new_s));

#line 171 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c}}; 
#line 163 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_new_s) String__Drop__glue(&new_s);
return _z_ret; }

#line 163 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_new_s) String__Drop__glue(&new_s);
    }
}

#line 174 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__add_assign(String* self, String* other)
{
    {

#line 175 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__append(self, other);
    }
}

#line 178 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__eq(String* self, String* other)
{
    {

#line 179 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int zero = 0;

#line 180 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 180 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strcmp(String__c_str(self), String__c_str((&(*other)))) == zero);
    }
}

#line 183 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__neq(String* self, String* other)
{
    {

#line 184 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (!
#line 184 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__eq(self, other));
    }
}

#line 187 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

int32_t String__compare(String* self, String* other)
{
    {

#line 188 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 188 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strcmp(String__c_str(self), String__c_str((&(*other))));
    }
}

#line 191 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__lt(String* self, String* other)
{
    {

#line 192 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 192 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__compare(self, other) < 0);
    }
}

#line 195 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__gt(String* self, String* other)
{
    {

#line 196 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 196 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__compare(self, other) > 0);
    }
}

#line 199 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__le(String* self, String* other)
{
    {

#line 200 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 200 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__compare(self, other) <= 0);
    }
}

#line 203 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__ge(String* self, String* other)
{
    {

#line 204 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 204 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__compare(self, other) >= 0);
    }
}

#line 207 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

int32_t String__compare_ignore_case(String* self, String* other)
{
    {

#line 208 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 208 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strcasecmp(String__c_str(self), String__c_str((&(*other))));
    }
}

#line 211 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__eq_ignore_case(String* self, String* other)
{
    {

#line 212 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t zero = 0;

#line 213 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 213 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__compare_ignore_case(self, other) == zero);
    }
}

#line 216 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__eq_str(String* self, char* s)
{
    {

#line 217 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int zero = 0;

#line 218 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 218 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strcmp(String__c_str(self), s) == zero);
    }
}

#line 221 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

size_t String__length(String* self)
{
    {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((self->vec.len == 0))     {

#line 222 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 0;
    }

#line 223 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (self->vec.len - 1);
    }
}

#line 226 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__substring(String* self, size_t start, size_t len)
{
    {

#line 227 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((start + len) > 
#line 227 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self)))     {

#line 228 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
__zenc_panic("substring out of bounds");
    }

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 230 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 231 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 232 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), Vec__char__get((&self->vec), (start + i)));
    }

#line 234 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 236 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 237 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 238 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c = v.cap;

#line 239 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 241 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c}}; 
#line 230 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 230 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 243 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__contains_str(String* self, char* target)
{
    {

#line 244 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 244 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__is_some((Option__size_t[]){String__find_str(self, target)});
    }
}

#line 247 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Option__size_t String__find_str(String* self, char* target)
{
    {

#line 248 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 248 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(target);

#line 249 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((t_len == 0))     {

#line 249 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 249 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__Some(0);
    }

#line 250 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t s_len = 
#line 250 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 251 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((t_len > s_len))     {

#line 251 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 251 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__None();
    }

#line 253 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i <= (s_len - t_len); i = (i + 1))     {

#line 254 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    bool is_match = true;

#line 255 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 256 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((
#line 256 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + k)) != target[k]))     {

#line 257 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(is_match = false);

#line 258 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }
    }

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (is_match)     {

#line 261 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 261 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__Some(i);
    }
    }

#line 263 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 263 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__None();
    }
}

#line 266 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Vec__size_t String__find_all_str(String* self, char* target)
{
    {

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_indices = 1; Vec__size_t indices = 
#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__size_t__new();

#line 268 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 268 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(target);

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((t_len == 0))     {

#line 269 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec__size_t__Drop__glue(&indices);
_z_ret_mv; });
    }

#line 270 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t s_len = 
#line 270 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((t_len > s_len))     {

#line 271 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec__size_t__Drop__glue(&indices);
_z_ret_mv; });
    }

#line 273 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i <= (s_len - t_len); i = (i + 1))     {

#line 274 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    bool is_match = true;

#line 275 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 276 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((
#line 276 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + k)) != target[k]))     {

#line 277 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(is_match = false);

#line 278 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }
    }

#line 281 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (is_match)     {

#line 282 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__size_t__push((&indices), i);
    }
    }

#line 286 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = indices; memset(&indices, 0, sizeof(_z_ret_mv)); __z_drop_flag_indices = 0; 
#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec__size_t__Drop__glue(&indices);
_z_ret_mv; });

#line 267 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_indices) Vec__size_t__Drop__glue(&indices);
    }
}

#line 289 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__to_lowercase(String* self)
{
    {

#line 290 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 290 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 291 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 291 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 292 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 293 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 293 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 294 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((c >= 'A') && (c <= 'Z')))     {

#line 295 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), ((char)((((int32_t)(c)) + 32))));
    }
 else     {

#line 297 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), c);
    }
    }

#line 300 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 301 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 302 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 303 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 304 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 305 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c_cap}}; 
#line 291 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 291 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 308 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__pad_right(String* self, size_t target_len, char pad_char)
{
    {

#line 309 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t current_len = 
#line 309 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 310 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((current_len >= target_len))     {

#line 311 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 311 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__from(String__c_str(self));
    }

#line 314 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 314 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 315 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < current_len; i = (i + 1))     {

#line 316 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), Vec__char__get((&self->vec), i));
    }

#line 318 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(current_len); i < target_len; i = (i + 1))     {

#line 319 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), pad_char);
    }

#line 321 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 323 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 324 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 325 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 326 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 327 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c_cap}}; 
#line 314 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 314 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 330 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__pad_left(String* self, size_t target_len, char pad_char)
{
    {

#line 331 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t current_len = 
#line 331 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 332 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((current_len >= target_len))     {

#line 333 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 333 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__from(String__c_str(self));
    }

#line 336 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 336 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 337 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t diff = (target_len - current_len);

#line 338 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < diff; i = (i + 1))     {

#line 339 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), pad_char);
    }

#line 341 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < current_len; i = (i + 1))     {

#line 342 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), Vec__char__get((&self->vec), i));
    }

#line 344 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 346 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 347 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 348 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 349 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 350 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c_cap}}; 
#line 336 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 336 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 353 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__to_uppercase(String* self)
{
    {

#line 354 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 354 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 355 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 355 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 356 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 357 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 357 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 358 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((c >= 'a') && (c <= 'z')))     {

#line 359 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), ((char)((((int32_t)(c)) - 32))));
    }
 else     {

#line 361 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), c);
    }
    }

#line 364 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 365 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char* d = v.data;

#line 366 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t l = v.len;

#line 367 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t c_cap = v.cap;

#line 368 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__forget((&v));

#line 369 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    { String _z_ret = (struct String){.vec = (Vec__char){.data = d, .len = l, .cap = c_cap}}; 
#line 355 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
return _z_ret; }

#line 355 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }
}

#line 372 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Option__size_t String__find(String* self, char target)
{
    {

#line 373 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 373 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 374 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < len; i = (i + 1))     {

#line 375 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((
#line 375 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i) == target))     {

#line 376 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 376 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__Some(i);
    }
    }

#line 379 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 379 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__None();
    }
}

#line 382 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__print(String* self)
{
    {

#line 383 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
printf("%s", String__c_str(self));

#line 384 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
fflush(stdout);
    }
}

#line 387 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__println(String* self)
{
    {

#line 388 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
printf("%s\n", String__c_str(self));
    }
}

#line 391 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__is_empty(String* self)
{
    {

#line 392 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 392 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self) == 0);
    }
}

#line 395 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__contains(String* self, char target)
{
    {

#line 396 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 396 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__size_t__is_some((__typeof__((String__find(self, target)))[]){String__find(self, target)});
    }
}

#line 399 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__starts_with(String* self, char* prefix)
{
    {

#line 400 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO plen = 
#line 400 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(prefix);

#line 401 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((plen > 
#line 401 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self)))     {

#line 401 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return false;
    }

#line 402 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int zero = 0;

#line 403 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 403 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strncmp(String__c_str(self), prefix, plen) == zero);
    }
}

#line 406 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

bool String__ends_with(String* self, char* suffix)
{
    {

#line 407 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO slen = 
#line 407 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(suffix);

#line 408 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 408 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 409 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((slen > len))     {

#line 409 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return false;
    }

#line 410 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t offset = ((int32_t)((len - slen)));

#line 411 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int zero = 0;

#line 412 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (
#line 412 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strcmp((String__c_str(self) + offset), suffix) == zero);
    }
}

#line 415 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__reserve(String* self, size_t cap)
{
    {

#line 416 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__grow_to_fit((&self->vec), (cap + 1));
    }
}

#line 419 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__free(String* self)
{
    {

#line 420 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__free((&self->vec));
    }
}

#line 423 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

size_t String__utf8_seq_len(char first_byte)
{
    {

#line 424 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t b = ((int32_t)(first_byte));

#line 425 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((b & 128) == 0))     {

#line 425 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 1;
    }

#line 426 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((b & 224) == 192))     {

#line 426 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 2;
    }

#line 427 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((b & 240) == 224))     {

#line 427 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 3;
    }

#line 428 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((b & 248) == 240))     {

#line 428 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 4;
    }

#line 429 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 1;
    }
}

#line 432 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

size_t String__utf8_len(String* self)
{
    {

#line 433 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 434 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 435 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 435 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 436 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 437 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 437 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 438 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 
#line 438 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(c)));

#line 439 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }

#line 441 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return count;
    }
}

#line 444 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__utf8_at(String* self, size_t idx)
{
    {

#line 445 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 446 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 447 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 447 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 448 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 449 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 449 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 450 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 450 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(c);

#line 452 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((count == idx))     {

#line 453 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 453 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__substring(self, i, seq);
    }

#line 456 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + seq));

#line 457 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }

#line 459 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 459 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");
    }
}

#line 462 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

int32_t String__utf8_get(String* self, size_t idx)
{
    {

#line 463 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 464 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 465 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 465 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 466 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 467 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 467 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i)));

#line 468 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 468 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(((char)(c)));

#line 470 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((count == idx))     {

#line 471 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 1))     {

#line 471 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)(c));
    }

#line 472 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 2))     {

#line 472 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 472 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63))));
    }

#line 473 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 3))     {

#line 473 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 473 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63))));
    }

#line 474 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 4))     {

#line 474 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 474 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 3)))))) & 63))));
    }
    }

#line 477 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + seq));

#line 478 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }

#line 480 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)(0));
    }
}

#line 483 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Vec__int32_t String__runes(String* self)
{
    {

#line 484 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__int32_t v = 
#line 484 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__int32_t__new();

#line 485 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 486 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 486 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 487 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 488 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 488 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i)));

#line 489 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 489 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(((char)(c)));

#line 490 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t val = 0;

#line 491 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 1))     {

#line 492 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 493 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 494 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 494 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63)))));
    }

#line 495 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 496 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 496 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63)))));
    }

#line 497 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 498 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 498 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 3)))))) & 63)))));
    }

#line 500 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__int32_t__push((&v), val);

#line 501 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + seq));
    }

#line 503 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = v; memset(&v, 0, sizeof(_z_ret_mv)); __z_drop_flag_v = 0; 
#line 484 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__int32_t__Drop__glue(&v);
_z_ret_mv; });

#line 484 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__int32_t__Drop__glue(&v);
    }
}

#line 506 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

StringCharsIter String__iterator(String* self)
{
    {

#line 507 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 507 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__chars(self);
    }
}

#line 510 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

StringCharsIter String__chars(String* self)
{
    {

#line 511 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (struct StringCharsIter){.data = 
#line 512 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__c_str(self), .len = 
#line 513 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self)};
    }
}

#line 518 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__from_runes_vec(Vec__int32_t runes)
{

#line 518 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_runes = 1;
    {

#line 519 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_s = 1; String s = 
#line 519 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");

#line 520 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t i = (size_t)(0); i < runes.len; i = (i + 1))     {

#line 521 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__push_rune((&s), Vec__int32_t__get((&runes), i));
    }

#line 523 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = s; memset(&s, 0, sizeof(_z_ret_mv)); __z_drop_flag_s = 0; 
#line 519 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);

#line 518 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_runes) Vec__int32_t__Drop__glue(&runes);
_z_ret_mv; });

#line 519 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_s) String__Drop__glue(&s);
    }

#line 518 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_runes) Vec__int32_t__Drop__glue(&runes);
}

#line 526 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

void String__insert_rune(String* self, size_t idx, int32_t r)
{
    {

#line 527 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 528 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 529 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 529 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 530 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while (((i < len) && (count < idx)))     {

#line 531 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 
#line 531 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(Vec__char__get((&self->vec), i))));

#line 532 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }

#line 535 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint32_t val = ((uint32_t)(r));

#line 536 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((val < 128))     {

#line 537 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), i, ((char)(val)));
    }

#line 538 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((val < 2048))     {

#line 539 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), i, ((char)((192 | (val >> 6)))));

#line 540 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 1), ((char)((128 | (val & 63)))));
    }

#line 541 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((val < 65536))     {

#line 542 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), i, ((char)((224 | (val >> 12)))));

#line 543 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 1), ((char)((128 | ((val >> 6) & 63)))));

#line 544 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 2), ((char)((128 | (val & 63)))));
    }
 else     {

#line 546 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), i, ((char)((240 | (val >> 18)))));

#line 547 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 1), ((char)((128 | ((val >> 12) & 63)))));

#line 548 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 2), ((char)((128 | ((val >> 6) & 63)))));

#line 549 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__insert((&self->vec), (i + 3), ((char)((128 | (val & 63)))));
    }
    }
}

#line 553 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

int32_t String__remove_rune_at(String* self, size_t idx)
{
    {

#line 554 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 555 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 556 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 556 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 557 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while (((i < len) && (count < idx)))     {

#line 558 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 
#line 558 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(Vec__char__get((&self->vec), i))));

#line 559 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }

#line 562 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((i >= len))     {

#line 562 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ((int32_t)(0));
    }

#line 564 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(
#line 564 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i)));

#line 565 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 565 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(((char)(c)));

#line 566 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t val = 0;

#line 567 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 1))     {

#line 567 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 568 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 568 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(
#line 568 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63)))));
    }

#line 569 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 569 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(
#line 569 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63)))));
    }

#line 570 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 570 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(
#line 570 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + 1)))))) & 63) << 12)) | ((((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 2)))))) & 63) << 6)) | (((int32_t)(((uint8_t)(Vec__char__get((&self->vec), (i + 3)))))) & 63)))));
    }

#line 572 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t j = (size_t)(0); j < seq; j = (j + 1))     {

#line 573 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__remove((&self->vec), i);
    }

#line 575 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return val;
    }
}

#line 578 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__utf8_substr(String* self, size_t start_idx, size_t num_chars)
{
    {

#line 579 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((num_chars == 0))     {

#line 579 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 579 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");
    }

#line 581 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t byte_start = 0;

#line 582 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t byte_len = 0;

#line 584 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t count = 0;

#line 585 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 586 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 586 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 587 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    bool found_start = false;

#line 589 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 591 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((!found_start) && (count == start_idx)))     {

#line 592 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(byte_start = i);

#line 593 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(found_start = true);

#line 595 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = 0);
    }

#line 596 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((!found_start))     {

#line 598 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 598 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 599 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 
#line 599 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(c)));

#line 600 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));

#line 601 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
continue;
    }

#line 605 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((count < num_chars))     {

#line 606 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 606 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i);

#line 607 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 607 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(c);

#line 608 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(byte_len = (byte_len + seq));

#line 609 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + seq));

#line 610 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(count = (count + 1));
    }
 else     {

#line 612 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }
    }

#line 616 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((!found_start))     {

#line 616 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 616 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");
    }

#line 618 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 618 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__substring(self, byte_start, byte_len);
    }
}

#line 620 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Vec__String String__split(String* self, char delim)
{
    {

#line 621 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_parts = 1; Vec__String parts = 
#line 621 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__String__new();

#line 622 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 622 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 623 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((len == 0))     {

#line 623 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 621 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
_z_ret_mv; });
    }

#line 625 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t start = 0;

#line 626 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 628 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < len))     {

#line 629 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((
#line 629 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), i) == delim))     {

#line 631 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__String__push((&parts), String__substring(self, start, (i - start)));

#line 632 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(start = (i + 1));
    }

#line 634 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 1));
    }

#line 638 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((start <= len))     {

#line 639 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__String__push((&parts), String__substring(self, start, (len - start)));
    }

#line 642 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = parts; memset(&parts, 0, sizeof(_z_ret_mv)); __z_drop_flag_parts = 0; 
#line 621 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
_z_ret_mv; });

#line 621 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_parts) Vec__String__Drop__glue(&parts);
    }
}

#line 645 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__trim(String* self)
{
    {

#line 646 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t start = 0;

#line 647 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t len = 
#line 647 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 648 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t end = len;

#line 651 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((start < len))     {

#line 652 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 652 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), start);

#line 653 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {

#line 654 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }

#line 656 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(start = (start + 1));
    }

#line 659 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((start == len))     {

#line 660 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 660 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");
    }

#line 664 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((end > start))     {

#line 665 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    char c = 
#line 665 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (end - 1));

#line 666 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((((c != ' ') && (c != '\t')) && (c != '\n')) && (c != '\r')))     {

#line 667 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }

#line 669 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(end = (end - 1));
    }

#line 672 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 672 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__substring(self, start, (end - start));
    }
}

#line 675 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

String String__replace(String* self, char* target, char* replacement)
{
    {

#line 676 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    ZC_AUTO t_len = 
#line 676 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
strlen(target);

#line 677 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((t_len == 0))     {

#line 677 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 677 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__substring(self, 0, String__length(self));
    }

#line 680 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t s_len = 
#line 680 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__length(self);

#line 681 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_result = 1; String result = 
#line 681 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new("");

#line 683 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t i = 0;

#line 684 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
while ((i < s_len))     {

#line 686 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((i + t_len) <= s_len))     {

#line 687 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    bool is_match = true;

#line 689 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
for (size_t k = (size_t)(0); k < t_len; k = (k + 1))     {

#line 690 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((
#line 690 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__get((&self->vec), (i + k)) != target[k]))     {

#line 691 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(is_match = false);

#line 692 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
break;
    }
    }

#line 696 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (is_match)     {

#line 697 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_r_str = 1; String r_str = 
#line 697 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new(replacement);

#line 698 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__append((&result), (&r_str));

#line 699 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + t_len));

#line 700 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

#line 697 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_r_str) String__Drop__glue(&r_str);
continue;

#line 697 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_r_str) String__Drop__glue(&r_str);
    }
    }

#line 705 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_v = 1; Vec__char v = 
#line 705 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__new();

#line 706 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), Vec__char__get((&self->vec), i));

#line 707 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Vec__char__push((&v), 0);

#line 708 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int __z_drop_flag_ch_s = 1; String ch_s = 
#line 708 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__new(v.data);

#line 709 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__append((&result), (&ch_s));

#line 710 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__destroy((&ch_s));

#line 711 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(i = (i + 1));

#line 708 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_ch_s) String__Drop__glue(&ch_s);

#line 705 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_v) Vec__char__Drop__glue(&v);
    }

#line 713 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return ({ ZC_AUTO _z_ret_mv = result; memset(&result, 0, sizeof(_z_ret_mv)); __z_drop_flag_result = 0; 
#line 681 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_result) String__Drop__glue(&result);
_z_ret_mv; });

#line 681 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    if (__z_drop_flag_result) String__Drop__glue(&result);
    }
}

#line 18 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

Option__int32_t StringCharsIter__next(StringCharsIter* self)
{
    {

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((self->pos >= self->len))     {

#line 19 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 19 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__int32_t__None();
    }

#line 21 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    uint8_t c = ((uint8_t)(self->data[self->pos]));

#line 22 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    size_t seq = 
#line 22 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
String__utf8_seq_len(((char)(c)));

#line 24 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if (((self->pos + seq) > self->len))     {

#line 25 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(self->pos = self->len);

#line 26 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 26 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__int32_t__None();
    }

#line 29 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    int32_t val = 0;

#line 30 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
if ((seq == 1))     {

#line 31 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(c)));
    }

#line 32 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 2))     {

#line 33 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((int32_t)((c & 31))) << 6) | (((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63)))));
    }

#line 34 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 3))     {

#line 35 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)((((((int32_t)((c & 15))) << 12) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63) << 6)) | (((int32_t)(((uint8_t)(self->data[(self->pos + 2)])))) & 63)))));
    }

#line 36 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
 else if ((seq == 4))     {

#line 37 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(val = ((int32_t)(((((((int32_t)((c & 7))) << 18) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 1)])))) & 63) << 12)) | ((((int32_t)(((uint8_t)(self->data[(self->pos + 2)])))) & 63) << 6)) | (((int32_t)(((uint8_t)(self->data[(self->pos + 3)])))) & 63)))));
    }

#line 40 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
(self->pos = (self->pos + seq));

#line 41 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return 
#line 41 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
Option__int32_t__Some(val);
    }
}

#line 44 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"

StringCharsIter StringCharsIter__iterator(StringCharsIter* self)
{
    {

#line 45 "/home/zuhaitz/zenc-lang/zenc/std/string.zc"
    return (*self);
    }
}

int main() { _z_run_tests(); return 0; }
