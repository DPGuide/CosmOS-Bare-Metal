# 0 "kernel_main_linux.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "kernel_main_linux.cpp"

# 1 "boot_info.h" 1
       
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stdint.h" 1 3 4
# 9 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stdint.h" 3 4
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdint.h" 1 3 4
# 28 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdint.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 1 3 4
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 1 3 4
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 1 3 4
# 108 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
             
# 117 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
             
# 326 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
       
# 405 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
       
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_secapi.h" 1 3 4
# 44 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_secapi.h" 3 4
extern "C++" {
template <bool __test, typename __dsttype>
  struct __if_array;
template <typename __dsttype>
  struct __if_array <true, __dsttype> {
    typedef __dsttype __type;
};
}
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 306 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 1 3 4
# 9 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 685 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sdks/_mingw_ddk.h" 1 3 4
# 686 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 2 3 4




#pragma pack(push,_CRT_PACKING)



extern "C" {





  typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 99 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/vadefs.h" 3 4
}



#pragma pack(pop)
# 307 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 592 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
extern "C" {
# 604 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{



  __asm__ __volatile__("int {$}3":);





}
# 625 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code)
{




  __asm__ __volatile__("int {$}0x29"::"c"(code));






  __builtin_unreachable();
}
# 665 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw.h" 3 4
const char *__mingw_get_crt_info (void);


}
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 2 3 4




#pragma pack(push,_CRT_PACKING)
# 35 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef unsigned long long size_t;
# 45 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long intptr_t;
# 75 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef unsigned long long uintptr_t;
# 88 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
__extension__ typedef long long ptrdiff_t;
# 106 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
typedef __time64_t time_t;
# 430 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {

  const unsigned short *_locale_pctype;
  int _locale_mb_cur_max;
  unsigned int _locale_lc_codepage;
# 482 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
} threadlocinfo;
# 501 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt.h" 3 4
#pragma pack(pop)
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/crtdefs.h" 2 3 4
# 29 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdint.h" 2 3 4



# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 1 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 13 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stddef.h" 3 4
extern "C" {




  __attribute__ ((__dllimport__)) extern int *__attribute__((__cdecl__)) _errno(void);

  errno_t __attribute__((__cdecl__)) _set_errno(int _Value);
  errno_t __attribute__((__cdecl__)) _get_errno(int *_Value);


  __attribute__ ((__dllimport__)) extern unsigned long __attribute__((__cdecl__)) __threadid(void);

  __attribute__ ((__dllimport__)) extern uintptr_t __attribute__((__cdecl__)) __threadhandle(void);


}
# 2 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 2 3 4
# 441 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 452 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 3 4
} max_align_t;






  typedef decltype(nullptr) nullptr_t;
# 33 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdint.h" 2 3 4


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef long long int_least64_t;
__extension__ typedef unsigned long long uint_least64_t;





typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef long long int_fast64_t;
__extension__ typedef unsigned long long uint_fast64_t;


__extension__ typedef long long intmax_t;
__extension__ typedef unsigned long long uintmax_t;
# 12 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stdint.h" 2 3 4
#pragma GCC diagnostic pop
# 3 "boot_info.h" 2


# 4 "boot_info.h"
struct BootInfo {
    uint32_t framebuffer_addr;
    uint32_t screen_width;
    uint32_t screen_height;

    uint32_t framebuffer_pitch;

    uint32_t ahci_drive_count;
    uint32_t drives_list_ptr;

    uint32_t uhci_base_port;
    uint32_t ac97_bus_port;
    uint32_t net_mac_addr_low;
    uint32_t net_mac_addr_high;


    uint32_t font_ptr;


    uint32_t cartridge_start;
    uint32_t cartridge_end;
} __attribute__((packed));
# 3 "kernel_main_linux.cpp" 2

extern uint8_t global_buf_mbr[4096];
extern uint8_t global_buf_dir[4096];
extern uint8_t global_tmp_dir[4096];

# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 1 3
# 9 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_stdio_config.h" 1 3
# 13 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_stdio_config.h" 3

# 13 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_stdio_config.h" 3
extern "C" {


unsigned long long* __attribute__((__cdecl__)) __local_stdio_printf_options(void);
unsigned long long* __attribute__((__cdecl__)) __local_stdio_scanf_options(void);
# 39 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_stdio_config.h" 3
}
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 2 3

#pragma pack(push,_CRT_PACKING)

       

       

       

       



extern "C" {
# 33 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  struct _iobuf {

    void *_Placeholder;
# 46 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  };
  typedef struct _iobuf FILE;
# 99 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_off_t.h" 1 3




  typedef long _off_t;

  typedef long off32_t;





  __extension__ typedef long long _off64_t;

  __extension__ typedef long long off64_t;
# 26 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_off_t.h" 3
typedef off32_t off_t;
# 100 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 2 3

__attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __acrt_iob_func(unsigned index);

  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) __iob_func(void);
# 112 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __extension__ typedef long long fpos_t;
# 156 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
extern
  __attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);
extern
  __attribute__((__format__(__gnu_scanf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_scanf(const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vscanf(const char * __restrict__ Format, va_list argp);
extern
  __attribute__((__format__(__gnu_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
extern
  __attribute__((__format__(__gnu_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);

extern
  __attribute__((__format__(__gnu_printf__,3,0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,
                               va_list _ArgList);
extern
  __attribute__((__format__(__gnu_printf__,3,4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
extern
  __attribute__((__format__(__gnu_printf__,1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,3))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_asprintf(char ** __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__gnu_printf__,2,0))) __attribute__((nonnull (1,2)))
  int __attribute__((__cdecl__)) __mingw_vasprintf(char ** __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));

extern
  __attribute__((__format__(__ms_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...)
  __asm__("sscanf");
extern
  __attribute__((__format__(__ms_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vsscanf(const char * __restrict__ _Str,const char * __restrict__ _Format,va_list argp)
  __asm__("vsscanf");
extern
  __attribute__((__format__(__ms_scanf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_scanf(const char * __restrict__ _Format,...)
  __asm__("scanf");
extern
  __attribute__((__format__(__ms_scanf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vscanf(const char * __restrict__ _Format,va_list argp)
  __asm__("vscanf");
extern
  __attribute__((__format__(__ms_scanf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...)
  __asm__("fscanf");
extern
  __attribute__((__format__(__ms_scanf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,va_list argp)
  __asm__("vfscanf");

extern
  __attribute__((__format__(__ms_printf__, 1,2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_printf(const char * __restrict__ , ... )
  __asm__("printf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vprintf (const char * __restrict__ , va_list)
  __asm__("vprintf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fprintf (FILE * __restrict__ , const char * __restrict__ , ...)
  __asm__("fprintf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list)
  __asm__("vfprintf") __attribute__ ((__nothrow__))
;
extern
  __attribute__((__format__(__ms_printf__, 2,3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_sprintf (char * __restrict__ , const char * __restrict__ , ...)
  __asm__("sprintf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 2,0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vsprintf (char * __restrict__ , const char * __restrict__ , va_list)
  __asm__("vsprintf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 3,4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_snprintf (char * __restrict__ , size_t , const char * __restrict__ , ...)
  __asm__("snprintf") __attribute__ ((__nothrow__));
extern
  __attribute__((__format__(__ms_printf__, 3,0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_vsnprintf (char * __restrict__ , size_t , const char * __restrict__ , va_list)
  __asm__("vsnprintf") __attribute__ ((__nothrow__));


  int __attribute__((__cdecl__)) __stdio_common_vsprintf(unsigned long long options, char *str, size_t len, const char *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vfprintf(unsigned long long options, FILE *file, const char *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vsscanf(unsigned long long options, const char *input, size_t length, const char *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vfscanf(unsigned long long options, FILE *file, const char *format, _locale_t locale, va_list valist);
# 444 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

  __attribute__((__format__ (__gnu_printf__, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
  __attribute__((__format__ (__gnu_printf__, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) printf(const char * __restrict__ _Format,...);
  __attribute__((__format__ (__gnu_printf__, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,...) ;

  __attribute__((__format__ (__gnu_printf__, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (__gnu_printf__, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) vprintf(const char * __restrict__ _Format,va_list _ArgList);
  __attribute__((__format__ (__gnu_printf__, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vsprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,va_list _Args) ;

 
  __attribute__((__format__ (__gnu_scanf__, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
 
  __attribute__((__format__ (__gnu_scanf__, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) scanf(const char * __restrict__ _Format,...);
 
  __attribute__((__format__ (__gnu_scanf__, 2, 3))) __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...);







  __attribute__((__format__ (__gnu_scanf__, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vfscanf (FILE *__stream, const char *__format, __builtin_va_list __local_argv);

  __attribute__((__format__ (__gnu_scanf__, 2, 0))) __attribute__ ((__nonnull__ (2)))
  int vsscanf (const char * __restrict__ __source, const char * __restrict__ __format, __builtin_va_list __local_argv);
  __attribute__((__format__ (__gnu_scanf__, 1, 0))) __attribute__ ((__nonnull__ (1)))
  int vscanf(const char *__format, __builtin_va_list __local_argv);


#pragma GCC diagnostic pop
# 535 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _filbuf(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flsbuf(int _Ch,FILE *_File);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fsopen(const char *_Filename,const char *_Mode,int _ShFlag);

  void __attribute__((__cdecl__)) clearerr(FILE *_File);
  int __attribute__((__cdecl__)) fclose(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fcloseall(void);



  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _fdopen(int _FileHandle,const char *_Mode);

  int __attribute__((__cdecl__)) feof(FILE *_File);
  int __attribute__((__cdecl__)) ferror(FILE *_File);
  int __attribute__((__cdecl__)) fflush(FILE *_File);
  int __attribute__((__cdecl__)) fgetc(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fgetchar(void);
  int __attribute__((__cdecl__)) fgetpos(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  int __attribute__((__cdecl__)) fgetpos64(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  char *__attribute__((__cdecl__)) fgets(char * __restrict__ _Buf,int _MaxCount,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fileno(FILE *_File);







  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _tempnam(const char *_DirName,const char *_FilePrefix);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _flushall(void);
  FILE *__attribute__((__cdecl__)) fopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode) ;
  FILE *fopen64(const char * __restrict__ filename,const char * __restrict__ mode);
  int __attribute__((__cdecl__)) fputc(int _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fputchar(int _Ch);
  int __attribute__((__cdecl__)) fputs(const char * __restrict__ _Str,FILE * __restrict__ _File);
  size_t __attribute__((__cdecl__)) fread(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  FILE *__attribute__((__cdecl__)) freopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode,FILE * __restrict__ _File) ;
  int __attribute__((__cdecl__)) fsetpos(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fsetpos64(FILE *_File,const fpos_t *_Pos);
  int __attribute__((__cdecl__)) fseek(FILE *_File,long _Offset,int _Origin);
  long __attribute__((__cdecl__)) ftell(FILE *_File);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseeki64(FILE *_File,long long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _ftelli64(FILE *_File);

  inline __attribute__((__cdecl__)) int fseeko(FILE *_File, _off_t _Offset, int _Origin) {
    return fseek(_File, _Offset, _Origin);
  }
  inline __attribute__((__cdecl__)) int fseeko64(FILE *_File, _off64_t _Offset, int _Origin) {
    return _fseeki64(_File, _Offset, _Origin);
  }
  inline __attribute__((__cdecl__)) _off_t ftello(FILE *_File) {
    return ftell(_File);
  }
  inline __attribute__((__cdecl__)) _off64_t ftello64(FILE *_File) {
    return _ftelli64(_File);
  }
# 622 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  size_t __attribute__((__cdecl__)) fwrite(const void * __restrict__ _Str,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) getc(FILE *_File);
  int __attribute__((__cdecl__)) getchar(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getmaxstdio(void);
  char *__attribute__((__cdecl__)) gets(char *_Buffer)
    __attribute__((__warning__("Using gets() is always unsafe - use fgets() instead")));
  int __attribute__((__cdecl__)) _getw(FILE *_File);


  void __attribute__((__cdecl__)) perror(const char *_ErrMsg);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pclose(FILE *_File);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _popen(const char *_Command,const char *_Mode);





  int __attribute__((__cdecl__)) putc(int _Ch,FILE *_File);
  int __attribute__((__cdecl__)) putchar(int _Ch);
  int __attribute__((__cdecl__)) puts(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putw(int _Word,FILE *_File);


  int __attribute__((__cdecl__)) remove(const char *_Filename);
  int __attribute__((__cdecl__)) rename(const char *_OldFilename,const char *_NewFilename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _unlink(const char *_Filename);

  int __attribute__((__cdecl__)) unlink(const char *_Filename) ;


  void __attribute__((__cdecl__)) rewind(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _rmtmp(void);
  void __attribute__((__cdecl__)) setbuf(FILE * __restrict__ _File,char * __restrict__ _Buffer) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmaxstdio(int _Max);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_output_format(unsigned int _Format);
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _get_output_format(void);
  int __attribute__((__cdecl__)) setvbuf(FILE * __restrict__ _File,char * __restrict__ _Buf,int _Mode,size_t _Size);

  __attribute__ ((__pure__))
  __attribute__((__format__ (__gnu_printf__, 1, 2))) __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) _scprintf(const char * __restrict__ _Format,...);
  __attribute__((__format__ (__gnu_scanf__, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) _snscanf(const char * __restrict__ _Src,size_t _MaxCount,const char * __restrict__ _Format,...) ;







  __attribute__ ((__pure__))
  __attribute__((__format__(__ms_printf__, 1,0))) __attribute__ ((__nonnull__ (1)))
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscprintf(const char * __restrict__ _Format,va_list _ArgList);
  FILE *__attribute__((__cdecl__)) tmpfile(void) ;
  char *__attribute__((__cdecl__)) tmpnam(char *_Buffer);
  int __attribute__((__cdecl__)) ungetc(int _Ch,FILE *_File);


  __attribute__((__format__ (__gnu_printf__, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) _vsnprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,va_list _Args) ;
  __attribute__((__format__ (__gnu_printf__, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) _snprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,...) ;
# 737 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

  __attribute__((__format__ (__gnu_printf__, 3, 0))) __attribute__ ((__nonnull__ (3)))
  int vsnprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, va_list __local_argv);

  __attribute__((__format__ (__gnu_printf__, 3, 4))) __attribute__ ((__nonnull__ (3)))
  int snprintf (char * __restrict__ __stream, size_t __n, const char * __restrict__ __format, ...);
# 803 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
#pragma GCC diagnostic pop
# 905 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_printf_count_output(int _Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _get_printf_count_output(void);




                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vswscanf (const wchar_t * __restrict__ _Str,const wchar_t * __restrict__ Format,va_list argp);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wscanf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwscanf(const wchar_t * __restrict__ Format, va_list argp);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwscanf (FILE * __restrict__ fp, const wchar_t * __restrict__ Format,va_list argp);

                                                      __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
                                                      __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_wprintf(const wchar_t * __restrict__ _Format,...);
                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __mingw_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __mingw_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_swprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , ...);
                                                      __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __mingw_vswprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ ,va_list);

                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...)
  __asm__("swscanf");
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vswscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,va_list)
  __asm__("vswscanf");
                                                    __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_wscanf(const wchar_t * __restrict__ _Format,...)
  __asm__("wscanf");
                                                    __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vwscanf(const wchar_t * __restrict__ _Format, va_list)
  __asm__("vwscanf");
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...)
  __asm__("fwscanf");
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list)
  __asm__("vfwscanf");

                                                     __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);


                                                     __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_wprintf(const wchar_t * __restrict__ _Format,...)
  __asm__("wprintf");
                                                    __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) __ms_vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList)
  __asm__("vfwprintf");
                                                    __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) __ms_vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList)
  __asm__("vwprintf");
                                                     __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_swprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , ...)
  __asm__("swprintf");
                                                     __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_vswprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ ,va_list)
  __asm__("vswprintf");
                                                     __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_snwprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , ...)
  __asm__("snwprintf");
                                                     __attribute__ ((__nonnull__ (3)))
  int __attribute__((__cdecl__)) __ms_vsnwprintf(wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list)
  __asm__("vsnwprintf");


  int __attribute__((__cdecl__)) __stdio_common_vswprintf(unsigned long long options, wchar_t *str, size_t len, const wchar_t *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vfwprintf(unsigned long long options, FILE *file, const wchar_t *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vswscanf(unsigned long long options, const wchar_t *input, size_t length, const wchar_t *format, _locale_t locale, va_list valist);
  int __attribute__((__cdecl__)) __stdio_common_vfwscanf(unsigned long long options, FILE *file, const wchar_t *format, _locale_t locale, va_list valist);
# 1101 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
 
  int __attribute__((__cdecl__)) fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);

 
  int __attribute__((__cdecl__)) swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...);

 
  int __attribute__((__cdecl__)) wscanf(const wchar_t * __restrict__ _Format,...);

  __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vfwscanf(FILE *__stream, const wchar_t *__format, va_list __local_argv);

  __attribute__ ((__nonnull__ (2)))
  int __attribute__((__cdecl__)) vswscanf(const wchar_t * __restrict__ __source, const wchar_t * __restrict__ __format, va_list __local_argv);

  __attribute__ ((__nonnull__ (1)))
  int __attribute__((__cdecl__)) vwscanf(const wchar_t *__format, va_list __local_argv);

  int __attribute__((__cdecl__)) fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) wprintf(const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
  int __attribute__((__cdecl__)) vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
# 1145 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  int __attribute__((__cdecl__)) swprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...);
  int __attribute__((__cdecl__)) vswprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args);
  int __attribute__((__cdecl__)) snwprintf(wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
  int __attribute__((__cdecl__)) vsnwprintf(wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, va_list arg);
# 1158 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfsopen(const wchar_t *_Filename,const wchar_t *_Mode,int _ShFlag);


  wint_t __attribute__((__cdecl__)) fgetwc(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwchar(void);
  wint_t __attribute__((__cdecl__)) fputwc(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) getwc(FILE *_File);
  wint_t __attribute__((__cdecl__)) getwchar(void);
  wint_t __attribute__((__cdecl__)) putwc(wchar_t _Ch,FILE *_File);
  wint_t __attribute__((__cdecl__)) putwchar(wchar_t _Ch);
  wint_t __attribute__((__cdecl__)) ungetwc(wint_t _Ch,FILE *_File);
  wchar_t *__attribute__((__cdecl__)) fgetws(wchar_t * __restrict__ _Dst,int _SizeInWords,FILE * __restrict__ _File);
  int __attribute__((__cdecl__)) fputws(const wchar_t * __restrict__ _Str,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putws(const wchar_t *_Str);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _scwprintf(const wchar_t * __restrict__ _Format,...);




  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vsnwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vscwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _swprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _vswprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,va_list _Args);


# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/swprintf.inl" 1 3
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/swprintf.inl" 3
extern "C++" {

                                                      __attribute__ ((__nonnull__ (2)))
int vswprintf (wchar_t *__stream, const wchar_t *__format, __builtin_va_list __local_argv) __asm__("_vswprintf");

                                                      __attribute__ ((__nonnull__ (2)))
int swprintf (wchar_t *__stream, const wchar_t *__format, ...) __asm__("_swprintf");

}
# 1188 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 2 3
# 1201 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtempnam(const wchar_t *_Directory,const wchar_t *_FilePrefix);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _snwscanf(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfdopen(int _FileHandle ,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfopen(const wchar_t * __restrict__ _Filename,const wchar_t *__restrict__ _Mode) ;
  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wfreopen(const wchar_t * __restrict__ _Filename,const wchar_t * __restrict__ _Mode,FILE * __restrict__ _OldFile) ;



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wperror(const wchar_t *_ErrMsg);

  __attribute__ ((__dllimport__)) FILE *__attribute__((__cdecl__)) _wpopen(const wchar_t *_Command,const wchar_t *_Mode);




  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wremove(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wtmpnam(wchar_t *_Buffer);

  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fgetwc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _fputwc_nolock(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t __attribute__((__cdecl__)) _ungetwc_nolock(wint_t _Ch,FILE *_File);
# 1249 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fgetc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fputc_nolock(int _Char, FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putc_nolock(int _Char, FILE *_File);
# 1264 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _lock_file(FILE *_File);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _unlock_file(FILE *_File);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fclose_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fflush_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseek_nolock(FILE *_File,long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _ftell_nolock(FILE *_File);
  __extension__ __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fseeki64_nolock(FILE *_File,long long _Offset,int _Origin);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _ftelli64_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fwrite_nolock(const void * __restrict__ _DstBuf,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _ungetc_nolock(int _Ch,FILE *_File);
# 1286 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  char *__attribute__((__cdecl__)) tempnam(const char *_Directory,const char *_FilePrefix) ;



  int __attribute__((__cdecl__)) fcloseall(void) ;
  FILE *__attribute__((__cdecl__)) fdopen(int _FileHandle,const char *_Format) ;
  int __attribute__((__cdecl__)) fgetchar(void) ;
  int __attribute__((__cdecl__)) fileno(FILE *_File) ;
  int __attribute__((__cdecl__)) flushall(void) ;
  int __attribute__((__cdecl__)) fputchar(int _Ch) ;
  int __attribute__((__cdecl__)) getw(FILE *_File) ;
  int __attribute__((__cdecl__)) putw(int _Ch,FILE *_File) ;
  int __attribute__((__cdecl__)) rmtmp(void) ;
# 1315 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_wide_utf8 (const wchar_t * const wptr, char **mbptr, size_t * buflen);
# 1329 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
int __attribute__((__cdecl__)) __mingw_str_utf8_wide (const char *const mbptr, wchar_t ** wptr, size_t * buflen);
# 1338 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
void __attribute__((__cdecl__)) __mingw_str_free(void *ptr);






  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnl(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnle(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlp(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnlpe(int _Mode,const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnv(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnve(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvp(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wspawnvpe(int _Mode,const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 1369 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 3
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnv(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnve(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvp(int _Mode,const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnvpe(int _Mode,const char *_Filename,const char *const *_ArgList,const char *const *_Env);




}


       
       
       
       

#pragma pack(pop)

# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 1 3
# 9 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 2 3
# 21 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
extern "C" {







  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) clearerr_s(FILE *_File);

  size_t __attribute__((__cdecl__)) fread_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);


  int __attribute__((__cdecl__)) __stdio_common_vsprintf_s(unsigned long long _Options, char *_Str, size_t _Len, const char *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vsprintf_p(unsigned long long _Options, char *_Str, size_t _Len, const char *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vsnprintf_s(unsigned long long _Options, char *_Str, size_t _Len, size_t _MaxCount, const char *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vfprintf_s(unsigned long long _Options, FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vfprintf_p(unsigned long long _Options, FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList);

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfscanf_s_l(FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfscanf(0x0001ULL, _File, _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vfscanf_s(FILE *_File, const char *_Format, va_list _ArgList)
  {
    return _vfscanf_s_l(_File, _Format, __null, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vscanf_s_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfscanf_s_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vscanf_s(const char *_Format, va_list _ArgList)
  {
    return _vfscanf_s_l((__acrt_iob_func(0)), _Format, __null, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fscanf_s_l(FILE *_File, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfscanf_s_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) fscanf_s(FILE *_File, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfscanf_s_l(_File, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _scanf_s_l(const char *_Format, _locale_t _Locale ,...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfscanf_s_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) scanf_s(const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfscanf_s_l((__acrt_iob_func(0)), _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfscanf_l(FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfscanf(0, _File, _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vscanf_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfscanf_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fscanf_l(FILE *_File, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfscanf_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _scanf_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfscanf_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsscanf_s_l(const char *_Src, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsscanf(0x0001ULL, _Src, (size_t)-1, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vsscanf_s(const char *_Src, const char *_Format, va_list _ArgList)
  {
    return _vsscanf_s_l(_Src, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sscanf_s_l(const char *_Src, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsscanf_s_l(_Src, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) sscanf_s(const char *_Src, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsscanf_s_l(_Src, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsscanf_l(const char *_Src, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsscanf(0, _Src, (size_t)-1, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sscanf_l(const char *_Src, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsscanf_l(_Src, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }


  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snscanf_s_l(const char *_Src, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = __stdio_common_vsscanf(0x0001ULL, _Src, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snscanf_s(const char *_Src, size_t _MaxCount, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = __stdio_common_vsscanf(0x0001ULL, _Src, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }


  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snscanf_l(const char *_Src, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = __stdio_common_vsscanf(0, _Src, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }


  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfprintf_s_l(FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfprintf_s((*__local_stdio_printf_options()), _File, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vfprintf_s(FILE *_File, const char *_Format, va_list _ArgList)
  {
    return _vfprintf_s_l(_File, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vprintf_s_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfprintf_s_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vprintf_s(const char *_Format, va_list _ArgList)
  {
    return _vfprintf_s_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fprintf_s_l(FILE *_File, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfprintf_s_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _printf_s_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfprintf_s_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) fprintf_s(FILE *_File, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfprintf_s_l(_File, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) printf_s(const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfprintf_s_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnprintf_c_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf((*__local_stdio_printf_options()), _DstBuf, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnprintf_c(char *_DstBuf, size_t _MaxCount, const char *_Format, va_list _ArgList)
  {
    return _vsnprintf_c_l(_DstBuf, _MaxCount, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snprintf_c_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsnprintf_c_l(_DstBuf, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snprintf_c(char *_DstBuf, size_t _MaxCount, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsnprintf_c_l(_DstBuf, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnprintf_s_l(char *_DstBuf, size_t _DstSize, size_t _MaxCount, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsnprintf_s((*__local_stdio_printf_options()), _DstBuf, _DstSize, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vsnprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount, const char *_Format, va_list _ArgList)
  {
    return _vsnprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount, const char *_Format, va_list _ArgList)
  {
    return _vsnprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snprintf_s_l(char *_DstBuf, size_t _DstSize, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsnprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snprintf_s(char *_DstBuf, size_t _DstSize, size_t _MaxCount, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsnprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsprintf_s_l(char *_DstBuf, size_t _DstSize, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf_s((*__local_stdio_printf_options()), _DstBuf, _DstSize, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vsprintf_s(char *_DstBuf, size_t _Size, const char *_Format, va_list _ArgList)
  {
    return _vsprintf_s_l(_DstBuf, _Size, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sprintf_s_l(char *_DstBuf, size_t _DstSize, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsprintf_s_l(_DstBuf, _DstSize, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) sprintf_s(char *_DstBuf, size_t _DstSize, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsprintf_s_l(_DstBuf, _DstSize, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfprintf_p_l(FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfprintf_p((*__local_stdio_printf_options()), _File, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfprintf_p(FILE *_File, const char *_Format, va_list _ArgList)
  {
    return _vfprintf_p_l(_File, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vprintf_p_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfprintf_p_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vprintf_p(const char *_Format, va_list _ArgList)
  {
    return _vfprintf_p_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fprintf_p_l(FILE *_File, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = __stdio_common_vfprintf_p((*__local_stdio_printf_options()), _File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fprintf_p(FILE *_File, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfprintf_p_l(_File, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _printf_p_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfprintf_p_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _printf_p(const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfprintf_p_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsprintf_p_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf_p((*__local_stdio_printf_options()), _DstBuf, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsprintf_p(char *_Dst, size_t _MaxCount, const char *_Format, va_list _ArgList)
  {
    return _vsprintf_p_l(_Dst, _MaxCount, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sprintf_p_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsprintf_p_l(_DstBuf, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sprintf_p(char *_Dst, size_t _MaxCount, const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsprintf_p_l(_Dst, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vscprintf_p_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf_p(0x0002ULL, __null, 0, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vscprintf_p(const char *_Format, va_list _ArgList)
  {
    return _vscprintf_p_l(_Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _scprintf_p_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vscprintf_p_l(_Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _scprintf_p(const char *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vscprintf_p_l(_Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfprintf_l(FILE *_File, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfprintf((*__local_stdio_printf_options()), _File, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vprintf_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfprintf_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fprintf_l(FILE *_File, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfprintf_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _printf_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfprintf_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnprintf_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf(0x0001ULL, _DstBuf, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snprintf_l(char *_DstBuf, size_t _MaxCount, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsnprintf_l(_DstBuf, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsprintf_l(char *_DstBuf, const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vsnprintf_l(_DstBuf, (size_t)-1, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _sprintf_l(char *_DstBuf, const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsprintf_l(_DstBuf, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vscprintf_l(const char *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsprintf(0x0002ULL, __null, 0, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _scprintf_l(const char *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vscprintf_l(_Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
# 583 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) vsnprintf_s(char (&_DstBuf)[__size], size_t _MaxCount, const char* _Format, va_list _ArgList) { return vsnprintf_s(_DstBuf,__size,_MaxCount,_Format,_ArgList); } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) _vsnprintf_s(char (&_DstBuf)[__size], size_t _MaxCount, const char* _Format, va_list _ArgList) { return _vsnprintf_s(_DstBuf,__size,_MaxCount,_Format,_ArgList); } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) vsprintf_s(char (&_DstBuf)[__size], const char* _Format, va_list _ArgList) { return vsprintf_s(_DstBuf,__size,_Format,_ArgList); } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) sprintf_s(char (&_DstBuf)[__size], const char* _Format, ...) { va_list __vaargs; __builtin_va_start(__vaargs,_Format); int __retval = vsprintf_s(_DstBuf,__size,_Format,__vaargs); __builtin_va_end(__vaargs); return __retval; } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) _snprintf_s(char (&_DstBuf)[__size], size_t _MaxCount, const char* _Format, ...) { va_list __vaargs; __builtin_va_start(__vaargs,_Format); int __retval = _vsnprintf_s(_DstBuf,__size,_MaxCount,_Format,__vaargs); __builtin_va_end(__vaargs); return __retval; } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) fopen_s(FILE **_File,const char *_Filename,const char *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) freopen_s(FILE** _File, const char *_Filename, const char *_Mode, FILE *_Stream);

  __attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) gets_s(char*,rsize_t);
  extern "C++" { template <size_t __size> inline char* __attribute__((__cdecl__)) get_s(char (&_DstBuf)[__size]) { return get_s(_DstBuf,__size); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpfile_s(FILE **_File);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) tmpnam_s(char*,rsize_t);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) tmpnam_s(char (&_DstBuf)[__size]) { return tmpnam_s(_DstBuf,__size); } }




  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _getws_s(wchar_t *_Str,size_t _SizeInWords);
  extern "C++" { template <size_t __size> inline wchar_t* __attribute__((__cdecl__)) _getws_s(wchar_t (&_DstBuf)[__size]) { return _getws_s(_DstBuf,__size); } }


  int __attribute__((__cdecl__)) __stdio_common_vswprintf_s(unsigned long long _Options, wchar_t *_Str, size_t _Len, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vsnwprintf_s(unsigned long long _Options, wchar_t *_Str, size_t _Len, size_t _MaxCount, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList);
  int __attribute__((__cdecl__)) __stdio_common_vfwprintf_s(unsigned long long _Options, FILE *_File, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList);

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfwscanf_s_l(FILE *_File, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfwscanf((*__local_stdio_scanf_options()) | 0x0001ULL, _File, _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vfwscanf_s(FILE* _File, const wchar_t *_Format, va_list _ArgList)
  {
    return _vfwscanf_s_l(_File, _Format, __null, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vwscanf_s_l(const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfwscanf_s_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vwscanf_s(const wchar_t *_Format, va_list _ArgList)
  {
    return _vfwscanf_s_l((__acrt_iob_func(0)), _Format, __null, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fwscanf_s_l(FILE *_File, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfwscanf_s_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) fwscanf_s(FILE *_File, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfwscanf_s_l(_File, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _wscanf_s_l(const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfwscanf_s_l((__acrt_iob_func(0)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) wscanf_s(const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfwscanf_s_l((__acrt_iob_func(0)), _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vswscanf_s_l(const wchar_t *_Src, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vswscanf((*__local_stdio_scanf_options()) | 0x0001ULL, _Src, (size_t)-1, _Format, _Locale, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vswscanf_s(const wchar_t *_Src, const wchar_t *_Format, va_list _ArgList)
  {
    return _vswscanf_s_l(_Src, _Format, __null, _ArgList);
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _swscanf_s_l(const wchar_t *_Src, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vswscanf_s_l(_Src, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) swscanf_s(const wchar_t *_Src, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vswscanf_s_l(_Src, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnwscanf_s_l(const wchar_t *_Src, size_t _MaxCount, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vswscanf((*__local_stdio_scanf_options()) | 0x0001ULL, _Src, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snwscanf_s_l(const wchar_t *_Src, size_t _MaxCount, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsnwscanf_s_l(_Src, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snwscanf_s(const wchar_t *_Src, size_t _MaxCount, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsnwscanf_s_l(_Src, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vfwprintf_s_l(FILE *_File, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vfwprintf_s((*__local_stdio_printf_options()), _File, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vwprintf_s_l(const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return _vfwprintf_s_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vfwprintf_s(FILE *_File, const wchar_t *_Format, va_list _ArgList)
  {
    return _vfwprintf_s_l(_File, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vwprintf_s(const wchar_t *_Format, va_list _ArgList)
  {
    return _vfwprintf_s_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _fwprintf_s_l(FILE *_File, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfwprintf_s_l(_File, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _wprintf_s_l(const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vfwprintf_s_l((__acrt_iob_func(1)), _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) fwprintf_s(FILE *_File, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfwprintf_s_l(_File, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) wprintf_s(const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vfwprintf_s_l((__acrt_iob_func(1)), _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vswprintf_s_l(wchar_t *_DstBuf, size_t _DstSize, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vswprintf_s((*__local_stdio_printf_options()), _DstBuf, _DstSize, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) vswprintf_s(wchar_t *_DstBuf, size_t _DstSize, const wchar_t *_Format, va_list _ArgList)
  {
    return _vswprintf_s_l(_DstBuf, _DstSize, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _swprintf_s_l(wchar_t *_DstBuf, size_t _DstSize, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vswprintf_s_l(_DstBuf, _DstSize, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) swprintf_s(wchar_t *_DstBuf, size_t _DstSize, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vswprintf_s_l(_DstBuf, _DstSize, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }

  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnwprintf_s_l(wchar_t *_DstBuf, size_t _DstSize, size_t _MaxCount, const wchar_t *_Format, _locale_t _Locale, va_list _ArgList)
  {
    return __stdio_common_vsnwprintf_s((*__local_stdio_printf_options()), _DstBuf, _DstSize, _MaxCount, _Format, _Locale, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _vsnwprintf_s(wchar_t *_DstBuf, size_t _DstSize, size_t _MaxCount, const wchar_t *_Format, va_list _ArgList)
  {
    return _vsnwprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, __null, _ArgList);
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snwprintf_s_l(wchar_t *_DstBuf, size_t _DstSize, size_t _MaxCount, const wchar_t *_Format, _locale_t _Locale, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Locale);
    _Ret = _vsnwprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, _Locale, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
  inline __attribute__((__cdecl__)) int __attribute__((__cdecl__)) _snwprintf_s(wchar_t *_DstBuf, size_t _DstSize, size_t _MaxCount, const wchar_t *_Format, ...)
  {
    __builtin_va_list _ArgList;
    int _Ret;
    __builtin_va_start(_ArgList, _Format);
    _Ret = _vsnwprintf_s_l(_DstBuf, _DstSize, _MaxCount, _Format, __null, _ArgList);
    __builtin_va_end(_ArgList);
    return _Ret;
  }
# 862 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) vswprintf_s(wchar_t (&_Dst)[__size], const wchar_t* _Format, va_list _ArgList) { return vswprintf_s(_Dst,__size,_Format,_ArgList); } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) swprintf_s(wchar_t (&_Dst)[__size], const wchar_t* _Format, ...) { va_list __vaargs; __builtin_va_start(__vaargs,_Format); int __retval = vswprintf_s(_Dst,__size,_Format,__vaargs); __builtin_va_end(__vaargs); return __retval; } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) _vsnwprintf_s(wchar_t (&_DstBuf)[__size], size_t _MaxCount, const wchar_t* _Format, va_list _ArgList) { return _vsnwprintf_s(_DstBuf,__size,_MaxCount,_Format,_ArgList); } }
  extern "C++" { template <size_t __size> inline int __attribute__((__cdecl__)) _snwprintf_s(wchar_t (&_DstBuf)[__size], size_t _MaxCount, const wchar_t* _Format, ...) { va_list __vaargs; __builtin_va_start(__vaargs,_Format); int __retval = _vsnwprintf_s(_DstBuf,__size,_MaxCount,_Format,__vaargs); __builtin_va_end(__vaargs); return __retval; } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wfreopen_s(FILE **_File,const wchar_t *_Filename,const wchar_t *_Mode,FILE *_OldFile);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wtmpnam_s(wchar_t *_DstBuf,size_t _SizeInWords);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wtmpnam_s(wchar_t (&_DstBuf)[__size]) { return _wtmpnam_s(_DstBuf,__size); } }
# 912 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdio_s.h" 3
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _fread_nolock_s(void *_DstBuf,size_t _DstSize,size_t _ElementSize,size_t _Count,FILE *_File);



}
# 1388 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdio.h" 2 3
# 9 "kernel_main_linux.cpp" 2
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 1 3
# 21 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 3
extern "C" {
# 45 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 3
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int __attribute__((__cdecl__)) memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * __attribute__((__cdecl__)) memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memcpy_s (void *_dest,size_t _numberOfElements,const void *_src,size_t _count);
  void * __attribute__((__cdecl__)) mempcpy (void *_Dst, const void *_Src, size_t _Size);
  void * __attribute__((__cdecl__)) memset(void *_Dst,int _Val,size_t _Size);

  void * __attribute__((__cdecl__)) memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int __attribute__((__cdecl__)) memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;


  char * __attribute__((__cdecl__)) _strset(char *_Str,int _Val) ;
  char * __attribute__((__cdecl__)) _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * __attribute__((__cdecl__)) strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * __attribute__((__cdecl__)) strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int __attribute__((__cdecl__)) strcmp(const char *_Str1,const char *_Str2);
  size_t __attribute__((__cdecl__)) strlen(const char *_Str);
  size_t __attribute__((__cdecl__)) strnlen(const char *_Str,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memmove(void *_Dst,const void *_Src,size_t _Size) ;




  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdup(const char *_Src);



  char *__attribute__((__cdecl__)) strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int __attribute__((__cdecl__)) strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strerror(const char *_ErrMsg) ;
  char *__attribute__((__cdecl__)) strerror(int) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strpbrk(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strrev(char *_Str);
  size_t __attribute__((__cdecl__)) strspn(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strstr(const char *_Str,const char *_SubStr);
  char *__attribute__((__cdecl__)) strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
       

  char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);
       
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);






  char *__attribute__((__cdecl__)) strdup(const char *_Src) ;



  int __attribute__((__cdecl__)) strcmpi(const char *_Str1,const char *_Str2) ;
  int __attribute__((__cdecl__)) stricmp(const char *_Str1,const char *_Str2) ;
  char *__attribute__((__cdecl__)) strlwr(char *_Str) ;
  int __attribute__((__cdecl__)) strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int __attribute__((__cdecl__)) strncasecmp (const char *, const char *, size_t);
  int __attribute__((__cdecl__)) strcasecmp (const char *, const char *);







  char *__attribute__((__cdecl__)) strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char *__attribute__((__cdecl__)) strrev(char *_Str) ;
  char *__attribute__((__cdecl__)) strset(char *_Str,int _Val) ;
  char *__attribute__((__cdecl__)) strupr(char *_Str) ;
# 149 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsdup(const wchar_t *_Str);



  wchar_t *__attribute__((__cdecl__)) wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  wchar_t *__attribute__((__cdecl__)) wcschr(const wchar_t *_Str,wchar_t _Ch);
  int __attribute__((__cdecl__)) wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t *__attribute__((__cdecl__)) wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t __attribute__((__cdecl__)) wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t __attribute__((__cdecl__)) wcslen(const wchar_t *_Str);
  size_t __attribute__((__cdecl__)) wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t *__attribute__((__cdecl__)) _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
  wchar_t *__attribute__((__cdecl__)) wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t __attribute__((__cdecl__)) wcsspn(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
  wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim,wchar_t ** __restrict__ _Ptr) ;
  wchar_t *__attribute__((__cdecl__)) _wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;




  extern "C++" inline wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) { return _wcstok(_Str, _Delim); }

  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);






  wchar_t *__attribute__((__cdecl__)) wcsdup(const wchar_t *_Str) ;




  int __attribute__((__cdecl__)) wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int __attribute__((__cdecl__)) wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsrev(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t *__attribute__((__cdecl__)) wcslwr(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsupr(wchar_t *_Str) ;
  int __attribute__((__cdecl__)) wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;




}


# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/string_s.h" 1 3
# 9 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/string_s.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/string_s.h" 2 3
# 21 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/string_s.h" 3
extern "C" {


  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strset_s(char *_Dst,size_t _DstSize,int _Value);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strset_s(char (&_Dst)[__size], int _Value) { return _strset_s(_Dst,__size,_Value); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strerror_s(char *_Buf,size_t _SizeInBytes,const char *_ErrMsg);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strerror_s(char (&_Buf)[__size], const char * _ErrMsg) { return _strerror_s(_Buf,__size,_ErrMsg); } }
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) strerror_s(char *_Buf,size_t _SizeInBytes,int _ErrNum);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) strerror_s(char (&_Buf)[__size], int _ErrNum) { return strerror_s(_Buf,__size,_ErrNum); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s(char *_Str,size_t _Size);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strlwr_s(char (&_Str)[__size]) { return _strlwr_s(_Str,__size); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strlwr_s_l(char (&_Str)[__size], _locale_t _Locale) { return _strlwr_s_l(_Str,__size,_Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strnset_s(char *_Str,size_t _Size,int _Val,size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strnset_s(char (&_Str)[__size], int _Val, size_t _MaxCount) { return _strnset_s(_Str,__size,_Val,_MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s(char *_Str,size_t _Size);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strupr_s(char (&_Str)[__size]) { return _strupr_s(_Str,__size); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strupr_s_l(char (&_Str)[__size], _locale_t _Locale) { return _strupr_s_l(_Str,__size,_Locale); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncat_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) strncat_s(char (&_Dst)[__size], const char * _Src, size_t _MaxCount) { return strncat_s(_Dst,__size,_Src,_MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncat_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strncat_s_l(char (&_Dst)[__size], const char * _Src, size_t _MaxCount, _locale_t _Locale) { return _strncat_s_l(_Dst,__size,_Src,_MaxCount,_Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcpy_s(char *_Dst, rsize_t _SizeInBytes, const char *_Src);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) strcpy_s(char (&_Dest)[__size], const char * _Source) { return strcpy_s(_Dest,__size,_Source); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncpy_s(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) strncpy_s(char (&_Dest)[__size], const char * _Source, size_t _MaxCount) { return strncpy_s(_Dest,__size,_Source,_MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncpy_s_l(char *_Dst, size_t _DstSizeInChars, const char *_Src, size_t _MaxCount, _locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _strncpy_s_l(char (&_Dest)[__size], const char * _Source, size_t _MaxCount, _locale_t _Locale) { return _strncpy_s_l(_Dest,__size,_Source,_MaxCount,_Locale); } }
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) strtok_s(char *_Str,const char *_Delim,char **_Context);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtok_s_l(char *_Str,const char *_Delim,char **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcat_s(char *_Dst, rsize_t _SizeInBytes, const char * _Src);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) strcat_s(char (&_Dest)[__size], const char * _Source) { return strcat_s(_Dest,__size,_Source); } }

  inline __attribute__((__always_inline__)) size_t __attribute__((__cdecl__)) strnlen_s(const char * _src, size_t _count) {
    return _src ? strnlen(_src, _count) : 0;
  }

  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) memmove_s(void *_dest,size_t _numberOfElements,const void *_src,size_t _count);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t *_Buf,size_t _SizeInWords,int _ErrNum);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t (&buffer)[__size], int error) { return _wcserror_s(buffer,__size,error); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) __wcserror_s(wchar_t *_Buffer,size_t _SizeInWords,const wchar_t *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s(wchar_t *_Dst,size_t _DstSizeInWords,wchar_t _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s(wchar_t *_Str,size_t _SizeInWords,wchar_t _Val);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t *_Str,size_t _SizeInWords);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t (&_Str)[__size]) { return _wcslwr_s(_Str,__size); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t *_Str,size_t _SizeInWords,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t (&_Str)[__size], _locale_t _Locale) { return _wcslwr_s_l(_Str,__size,_Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t *_Str,size_t _Size);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t (&_Str)[__size]) { return _wcsupr_s(_Str,__size); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t *_Str,size_t _Size,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t (&_Str)[__size], _locale_t _Locale) { return _wcsupr_s_l(_Str,__size,_Locale); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t *_Dst, rsize_t _SizeInWords, const wchar_t *_Src);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t (&_Dest)[__size], const wchar_t * _Source) { return wcscpy_s(_Dest,__size,_Source); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t * _Dst, rsize_t _SizeInWords, const wchar_t *_Src);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t (&_Dest)[__size], const wchar_t * _Source) { return wcscat_s(_Dest,__size,_Source); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t (&_Dst)[__size], const wchar_t * _Src, size_t _MaxCount) { return wcsncat_s(_Dst,__size,_Src,_MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t (&_Dst)[__size], const wchar_t * _Src, size_t _MaxCount, _locale_t _Locale) { return _wcsncat_s_l(_Dst,__size,_Src,_MaxCount,_Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t (&_Dest)[__size], const wchar_t * _Source, size_t _MaxCount) { return wcsncpy_s(_Dest,__size,_Source,_MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t *_Dst, size_t _DstSizeInChars, const wchar_t *_Src, size_t _MaxCount, _locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t (&_Dest)[__size], const wchar_t * _Source, size_t _MaxCount, _locale_t _Locale) { return _wcsncpy_s_l(_Dest,__size,_Source,_MaxCount,_Locale); } }
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcstok_s_l(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t *_Str,size_t _SizeInChars,wchar_t _Val,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t (&_Str)[__size], wchar_t _Val, _locale_t _Locale) { return _wcsset_s_l(_Str,__size,_Val,_Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t *_Str,size_t _SizeInChars,wchar_t _Val, size_t _Count,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t (&_Str)[__size], wchar_t _Val, size_t _Count, _locale_t _Locale) { return _wcsnset_s_l(_Str,__size,_Val,_Count,_Locale); } }

  inline __attribute__((__always_inline__)) size_t __attribute__((__cdecl__)) wcsnlen_s(const wchar_t * _src, size_t _count) {
    return _src ? wcsnlen(_src, _count) : 0;
  }



}
# 226 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/string.h" 2 3
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 2 3
# 21 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
#pragma pack(push,_CRT_PACKING)


extern "C" {






__attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) _getcwd (char*, int);





  typedef unsigned long _fsize_t;





  struct _finddata32_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    _fsize_t size;
    char name[260];
  };

  struct _finddata32i64_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    __extension__ long long size;
    char name[260];
  };

  struct _finddata64i32_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    _fsize_t size;
    char name[260];
  };

  struct __finddata64_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    __extension__ long long size;
    char name[260];
  };
# 102 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
  struct _wfinddata32_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    _fsize_t size;
    wchar_t name[260];
  };

  struct _wfinddata32i64_t {
    unsigned attrib;
    __time32_t time_create;
    __time32_t time_access;
    __time32_t time_write;
    __extension__ long long size;
    wchar_t name[260];
  };

  struct _wfinddata64i32_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    _fsize_t size;
    wchar_t name[260];
  };

  struct _wfinddata64_t {
    unsigned attrib;
    __time64_t time_create;
    __time64_t time_access;
    __time64_t time_write;
    __extension__ long long size;
    wchar_t name[260];
  };
# 195 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _access(const char *_Filename,int _AccessMode);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _access_s(const char *_Filename,int _AccessMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _chmod(const char *_Filename,int _Mode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _chsize(int _FileHandle,long _Size) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _chsize_s (int _FileHandle,long long _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _close(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _commit(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _creat(const char *_Filename,int _PermissionMode) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _dup(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _dup2(int _FileHandleSrc,int _FileHandleDst);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _eof(int _FileHandle);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _filelength(int _FileHandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst32(const char *_Filename,struct _finddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext32(intptr_t _FindHandle,struct _finddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findclose(intptr_t _FindHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isatty(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _locking(int _FileHandle,int _LockMode,long _NumOfBytes);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _lseek(int _FileHandle,long _Offset,int _Origin);
  _off64_t lseek64(int fd,_off64_t offset, int whence);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _mktemp(char *_TemplateName) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _mktemp_s (char *_TemplateName,size_t _Size);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _pipe(int *_PtHandles,unsigned int _PipeSize,int _TextMode);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _read(int _FileHandle,void *_DstBuf,unsigned int _MaxCharCount);
# 231 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _setmode(int _FileHandle,int _Mode);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _tell(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _umask(int _Mode) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _umask_s (int _NewMode,int *_OldMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _write(int _FileHandle,const void *_Buf,unsigned int _MaxCharCount);

  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _filelengthi64(int _FileHandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst32i64(const char *_Filename,struct _finddata32i64_t *_FindData);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _findfirst64(const char *_Filename,struct __finddata64_t *_FindData);



  intptr_t __attribute__((__cdecl__)) _findfirst64i32(const char *_Filename,struct _finddata64i32_t *_FindData);
# 262 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext32i64(intptr_t _FindHandle,struct _finddata32i64_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _findnext64(intptr_t _FindHandle,struct __finddata64_t *_FindData);
  int __attribute__((__cdecl__)) _findnext64i32(intptr_t _FindHandle,struct _finddata64i32_t *_FindData);
# 283 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
  __extension__ long long __attribute__((__cdecl__)) _lseeki64(int _FileHandle,long long _Offset,int _Origin);
  __extension__ long long __attribute__((__cdecl__)) _telli64(int _FileHandle);



  int __attribute__((__cdecl__)) chdir (const char *) ;




  char *__attribute__((__cdecl__)) getcwd (char *, int) ;



  int __attribute__((__cdecl__)) mkdir (const char *) ;
  char *__attribute__((__cdecl__)) mktemp(char *) ;
  int __attribute__((__cdecl__)) rmdir (const char*) ;
  int __attribute__((__cdecl__)) chmod (const char *, int) ;



  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _sopen_s(int *_FileHandle,const char *_Filename,int _OpenFlag,int _ShareFlag,int _PermissionMode);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _open(const char *_Filename,int _OpenFlag,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _sopen(const char *_Filename,int _OpenFlag,int _ShareFlag,...) ;



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _waccess(const wchar_t *_Filename,int _AccessMode);
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _waccess_s (const wchar_t *_Filename,int _AccessMode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wchmod(const wchar_t *_Filename,int _Mode);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcreat(const wchar_t *_Filename,int _PermissionMode) ;
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst32(const wchar_t *_Filename,struct _wfinddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext32(intptr_t _FindHandle,struct _wfinddata32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wunlink(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wrename(const wchar_t *_OldFilename,const wchar_t *_NewFilename);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wmktemp(wchar_t *_TemplateName) ;
  __attribute__((dllimport)) errno_t __attribute__((__cdecl__)) _wmktemp_s (wchar_t *_TemplateName, size_t _SizeInWords);

  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst32i64(const wchar_t *_Filename,struct _wfinddata32i64_t *_FindData);
  intptr_t __attribute__((__cdecl__)) _wfindfirst64i32(const wchar_t *_Filename,struct _wfinddata64i32_t *_FindData);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wfindfirst64(const wchar_t *_Filename,struct _wfinddata64_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext32i64(intptr_t _FindHandle,struct _wfinddata32i64_t *_FindData);
  int __attribute__((__cdecl__)) _wfindnext64i32(intptr_t _FindHandle,struct _wfinddata64i32_t *_FindData);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wfindnext64(intptr_t _FindHandle,struct _wfinddata64_t *_FindData);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsopen_s(int *_FileHandle,const wchar_t *_Filename,int _OpenFlag,int _ShareFlag,int _PermissionFlag);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wopen(const wchar_t *_Filename,int _OpenFlag,...) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsopen(const wchar_t *_Filename,int _OpenFlag,int _ShareFlag,...) ;



  int __attribute__((__cdecl__)) __lock_fhandle(int _Filehandle);
  void __attribute__((__cdecl__)) _unlock_fhandle(int _Filehandle);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_osfhandle(int _FileHandle);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _open_osfhandle(intptr_t _OSFileHandle,int _Flags);


  int __attribute__((__cdecl__)) access(const char *_Filename,int _AccessMode) ;
  int __attribute__((__cdecl__)) chmod(const char *_Filename,int _AccessMode) ;
  int __attribute__((__cdecl__)) chsize(int _FileHandle,long _Size) ;
  int __attribute__((__cdecl__)) close(int _FileHandle) ;
  int __attribute__((__cdecl__)) creat(const char *_Filename,int _PermissionMode) ;
  int __attribute__((__cdecl__)) dup(int _FileHandle) ;
  int __attribute__((__cdecl__)) dup2(int _FileHandleSrc,int _FileHandleDst) ;
  int __attribute__((__cdecl__)) eof(int _FileHandle) ;
  long __attribute__((__cdecl__)) filelength(int _FileHandle) ;
  int __attribute__((__cdecl__)) isatty(int _FileHandle) ;
  int __attribute__((__cdecl__)) locking(int _FileHandle,int _LockMode,long _NumOfBytes) ;
  long __attribute__((__cdecl__)) lseek(int _FileHandle,long _Offset,int _Origin) ;
  char *__attribute__((__cdecl__)) mktemp(char *_TemplateName) ;
  int __attribute__((__cdecl__)) open(const char *_Filename,int _OpenFlag,...) ;
  int __attribute__((__cdecl__)) read(int _FileHandle,void *_DstBuf,unsigned int _MaxCharCount) ;
  int __attribute__((__cdecl__)) setmode(int _FileHandle,int _Mode) ;
  int __attribute__((__cdecl__)) sopen(const char *_Filename,int _OpenFlag,int _ShareFlag,...) ;
  long __attribute__((__cdecl__)) tell(int _FileHandle) ;
  int __attribute__((__cdecl__)) umask(int _Mode) ;
  int __attribute__((__cdecl__)) write(int _Filehandle,const void *_Buf,unsigned int _MaxCharCount) ;
# 503 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/io.h" 3
}



#pragma pack(pop)
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 2 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_startup.h" 1 3
# 13 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_startup.h" 3
extern "C" {


typedef enum _crt_app_type {
    _crt_unknown_app,
    _crt_console_app,
    _crt_gui_app
} _crt_app_type;

__attribute__ ((__dllimport__)) _crt_app_type __attribute__((__cdecl__)) _query_app_type(void);
__attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _set_app_type(_crt_app_type _Type);

typedef enum _crt_argv_mode {
    _crt_argv_no_arguments,
    _crt_argv_unexpanded_arguments,
    _crt_argv_expanded_arguments
} _crt_argv_mode;

__attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _configure_narrow_argv(_crt_argv_mode mode);
__attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _configure_wide_argv(_crt_argv_mode mode);

__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _initialize_narrow_environment(void);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _initialize_wide_environment(void);

__attribute__ ((__dllimport__)) char** __attribute__((__cdecl__)) _get_initial_narrow_environment(void);
__attribute__ ((__dllimport__)) wchar_t** __attribute__((__cdecl__)) _get_initial_wide_environment(void);

__attribute__ ((__dllimport__)) char* __attribute__((__cdecl__)) _get_narrow_winmain_command_line(void);
__attribute__ ((__dllimport__)) wchar_t* __attribute__((__cdecl__)) _get_wide_winmain_command_line(void);

__attribute__ ((__dllimport__)) char **__attribute__((__cdecl__)) __p__acmdln(void);


__attribute__ ((__dllimport__)) wchar_t **__attribute__((__cdecl__)) __p__wcmdln(void);


typedef void (__attribute__((__cdecl__)) *_PVFV)(void);
typedef int (__attribute__((__cdecl__)) *_PIFV)(void);
typedef void (__attribute__((__cdecl__)) *_PVFI)(int);

__attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _initterm(_PVFV* _First, _PVFV* _Last);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _initterm_e(_PIFV* _First, _PIFV* _Last);

typedef struct _onexit_table_t {
    _PVFV* _first;
    _PVFV* _last;
    _PVFV* _end;
} _onexit_table_t;

typedef int (__attribute__((__cdecl__)) *_onexit_t)(void);

__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _initialize_onexit_table(_onexit_table_t*);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _register_onexit_function(_onexit_table_t*,_onexit_t);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _execute_onexit_table(_onexit_table_t*);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _crt_atexit(_PVFV func);
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _crt_at_quick_exit(_PVFV func);


}
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 2 3


# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/types.h" 1 3
# 43 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/types.h" 3
typedef unsigned short _ino_t;

typedef unsigned short ino_t;





typedef unsigned int _dev_t;

typedef unsigned int dev_t;
# 62 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/types.h" 3
__extension__
typedef long long _pid_t;




typedef _pid_t pid_t;





typedef unsigned short _mode_t;


typedef _mode_t mode_t;






typedef unsigned int useconds_t;




struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};





__extension__
typedef unsigned long long _sigset_t;
# 14 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 2 3



extern "C" {
# 32 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 3
  typedef void (__attribute__((__cdecl__)) *_beginthread_proc_type)(void *);
  typedef unsigned (__attribute__((__stdcall__)) *_beginthreadex_proc_type)(void *);

  __attribute__ ((__dllimport__)) uintptr_t __attribute__((__cdecl__)) _beginthread(_beginthread_proc_type _StartAddress,unsigned _StackSize,void *_ArgList);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _endthread(void) __attribute__ ((__noreturn__));
  __attribute__ ((__dllimport__)) uintptr_t __attribute__((__cdecl__)) _beginthreadex(void *_Security,unsigned _StackSize,_beginthreadex_proc_type _StartAddress,void *_ArgList,unsigned _InitFlag,unsigned *_ThrdAddr);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _endthreadex(unsigned _Retval) __attribute__ ((__noreturn__));



  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) exit(int _Code) __attribute__ ((__noreturn__));
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _exit(int _Code) __attribute__ ((__noreturn__));

  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) quick_exit(int _Code) __attribute__ ((__noreturn__));




  void __attribute__((__cdecl__)) _Exit(int) __attribute__ ((__noreturn__));






       

  void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) abort(void);
       



  typedef void (__attribute__((__stdcall__)) *_tls_callback_type)(void*,unsigned long,void*);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _register_thread_local_exe_atexit_callback(_tls_callback_type callback);

  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _cexit(void);
  void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _c_exit(void);

  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _getpid(void);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _cwait(int *_TermStat,intptr_t _ProcHandle,int _Action);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execl(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execle(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execlp(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execlpe(const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execv(const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execve(const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execvp(const char *_Filename,const char *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _execvpe(const char *_Filename,const char *const *_ArgList,const char *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnl(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnle(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnlp(int _Mode,const char *_Filename,const char *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _spawnlpe(int _Mode,const char *_Filename,const char *_ArgList,...);
# 95 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 3
  int __attribute__((__cdecl__)) system(const char *_Command);




  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecl(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecle(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexeclp(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexeclpe(const wchar_t *_Filename,const wchar_t *_ArgList,...);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecv(const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecve(const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecvp(const wchar_t *_Filename,const wchar_t *const *_ArgList);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _wexecvpe(const wchar_t *_Filename,const wchar_t *const *_ArgList,const wchar_t *const *_Env);
# 124 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wsystem(const wchar_t *_Command);




  intptr_t __attribute__((__cdecl__)) _loaddll(char *_Filename);
  int __attribute__((__cdecl__)) _unloaddll(intptr_t _Handle);
  int (__attribute__((__cdecl__)) *__attribute__((__cdecl__)) _getdllprocaddr(intptr_t _Handle,char *_ProcedureName,intptr_t _Ordinal))(void);
# 161 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/process.h" 3
  int __attribute__((__cdecl__)) getpid(void) ;



  intptr_t __attribute__((__cdecl__)) cwait(int *_TermStat,intptr_t _ProcHandle,int _Action) ;

  int __attribute__((__cdecl__)) execl(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execle(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execlp(const char *_Filename,const char *_ArgList,...) ;
  int __attribute__((__cdecl__)) execlpe(const char *_Filename,const char *_ArgList,...) ;






  intptr_t __attribute__((__cdecl__)) spawnl(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnle(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnlp(int,const char *_Filename,const char *_ArgList,...) ;
  intptr_t __attribute__((__cdecl__)) spawnlpe(int,const char *_Filename,const char *_ArgList,...) ;





  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) execv(const char *_Filename,char *const _ArgList[]) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) execve(const char *_Filename,char *const _ArgList[],char *const _Env[]) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) execvp(const char *_Filename,char *const _ArgList[]) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) execvpe(const char *_Filename,char *const _ArgList[],char *const _Env[]) ;






  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) spawnv(int,const char *_Filename,char *const _ArgList[]) ;
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) spawnve(int,const char *_Filename,char *const _ArgList[],char *const _Env[]) ;
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) spawnvp(int,const char *_Filename,char *const _ArgList[]) ;
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) spawnvpe(int,const char *_Filename,char *const _ArgList[],char *const _Env[]) ;




}
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 2 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/getopt.h" 1 3
# 19 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/getopt.h" 3
extern "C" {


extern int optind;
extern int optopt;
extern int opterr;


extern char *optarg;

extern int getopt(int nargc, char * const *nargv, const char *options);


}
# 13 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 2 3
# 41 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 3
extern "C" {


       

unsigned int __attribute__((__cdecl__)) sleep (unsigned int);
       




int __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) usleep(useconds_t);







int ftruncate(int, off32_t);
int ftruncate64(int, off64_t);
int truncate(const char *, off32_t);
int truncate64(const char *, off64_t);
# 93 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 3
  void __attribute__((__cdecl__)) swab(char *_Buf1,char *_Buf2,int _SizeInBytes) ;
# 104 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 3
}


# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/pthread_unistd.h" 1 3
# 108 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/unistd.h" 2 3
# 10 "kernel_main_linux.cpp" 2
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/cpuid.h" 1 3 4
# 267 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/cpuid.h" 3 4
static __inline unsigned int
__get_cpuid_max (unsigned int __ext, unsigned int *__sig)
{
  unsigned int __eax, __ebx, __ecx, __edx;
# 309 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/cpuid.h" 3 4
  __asm__ __volatile__ ("cpuid\n\t" : "=a" (__eax), "=b" (__ebx), "=c" (__ecx), "=d" (__edx) : "0" (__ext));

  if (__sig)
    *__sig = __ebx;

  return __eax;
}






static __inline int
__get_cpuid (unsigned int __leaf,
      unsigned int *__eax, unsigned int *__ebx,
      unsigned int *__ecx, unsigned int *__edx)
{
  unsigned int __ext = __leaf & 0x80000000;
  unsigned int __maxlevel = __get_cpuid_max (__ext, 0);

  if (__maxlevel == 0 || __maxlevel < __leaf)
    return 0;

  __asm__ __volatile__ ("cpuid\n\t" : "=a" (*__eax), "=b" (*__ebx), "=c" (*__ecx), "=d" (*__edx) : "0" (__leaf));
  return 1;
}



static __inline int
__get_cpuid_count (unsigned int __leaf, unsigned int __subleaf,
     unsigned int *__eax, unsigned int *__ebx,
     unsigned int *__ecx, unsigned int *__edx)
{
  unsigned int __ext = __leaf & 0x80000000;
  unsigned int __maxlevel = __get_cpuid_max (__ext, 0);

  if (__builtin_expect (__maxlevel == 0, 0) || __maxlevel < __leaf)
    return 0;

  __asm__ __volatile__ ("cpuid\n\t" : "=a" (*__eax), "=b" (*__ebx), "=c" (*__ecx), "=d" (*__edx) : "0" (__leaf), "2" (__subleaf));
  return 1;
}

static __inline void
__cpuidex (int __cpuid_info[4], int __leaf, int __subleaf)
{
  __asm__ __volatile__ ("cpuid\n\t" : "=a" (__cpuid_info[0]), "=b" (__cpuid_info[1]), "=c" (__cpuid_info[2]), "=d" (__cpuid_info[3]) : "0" (__leaf), "2" (__subleaf))
                                    ;
}
# 11 "kernel_main_linux.cpp" 2
# 88 "kernel_main_linux.cpp"
# 1 "schneider_lang.h" 1







       



# 11 "schneider_lang.h"
struct usb_bot_cbw {
    uint32_t signature;
    uint32_t tag;
    uint32_t data_transfer_length;
    uint8_t flags;
    uint8_t lun;
    uint8_t cmd_length;
    uint8_t scsi_cmd[16];
} __attribute__((packed));


struct usb_bot_csw {
    uint32_t signature;
    uint32_t tag;
    uint32_t data_residue;
    uint8_t status;
} __attribute__((packed));



typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
# 161 "schneider_lang.h"
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 1 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 2 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 2 3 4
# 162 "schneider_lang.h" 2

struct MemoryBlock {
    uint32_t size;
    uint8_t is_free;
    MemoryBlock* next;
} __attribute__((packed));


void init_heap();




# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 1 3
# 36 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 1 3
# 46 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 1 3
# 37 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wc++23-extensions"
# 336 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
  typedef long long unsigned int size_t;
  typedef long long int ptrdiff_t;


  typedef decltype(nullptr) nullptr_t;


#pragma GCC visibility push(default)


  extern "C++" __attribute__ ((__noreturn__, __always_inline__))
  inline void __terminate() noexcept
  {
    void terminate() noexcept __attribute__ ((__noreturn__,__cold__));
    terminate();
  }
#pragma GCC visibility pop
}
# 369 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
# 573 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
#pragma GCC visibility push(default)




  __attribute__((__always_inline__))
  constexpr inline bool
  __is_constant_evaluated() noexcept
  {





    return __builtin_is_constant_evaluated();



  }
#pragma GCC visibility pop
}
# 617 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
namespace std
{
#pragma GCC visibility push(default)

  extern "C++" __attribute__ ((__noreturn__)) __attribute__((__cold__))
  void
  __glibcxx_assert_fail
    (const char* __file, int __line, const char* __function,
     const char* __condition)
  noexcept;
#pragma GCC visibility pop
}
# 727 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/os_defines.h" 1 3
# 728 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 2 3


# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/cpu_defines.h" 1 3
# 731 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 2 3
# 887 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
namespace __gnu_cxx
{
  typedef __decltype(0.0bf16) __bfloat16_t;
}
# 949 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/pstl/pstl_config.h" 1 3
# 950 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/x86_64-w64-mingw32/bits/c++config.h" 2 3



#pragma GCC diagnostic pop
# 47 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 2 3
# 80 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 1 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_wstdlib.h" 1 3
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/corecrt_wstdlib.h" 3
extern "C" {






  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wdupenv_s(wchar_t **_Buffer,size_t *_BufferSizeInWords,const wchar_t *_VarName);




  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itow_s (int _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _itow_s(int _Val, wchar_t (&_DstBuf)[__size], int _Radix) { return _itow_s(_Val, _DstBuf, __size, _Radix); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltow_s (long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _ltow_s(long _Val, wchar_t (&_DstBuf)[__size], int _Radix) { return _ltow_s(_Val, _DstBuf, __size, _Radix); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultow_s (unsigned long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _ultow_s(unsigned long _Val, wchar_t (&_DstBuf)[__size], int _Radix) { return _ultow_s(_Val, _DstBuf, __size, _Radix); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wgetenv_s(size_t *_ReturnSize,wchar_t *_DstBuf,size_t _DstSizeInWords,const wchar_t *_VarName);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wgetenv_s(size_t* _ReturnSize, wchar_t (&_DstBuf)[__size], const wchar_t* _VarName) { return _wgetenv_s(_ReturnSize, _DstBuf, __size, _VarName); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64tow_s(long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64tow_s(unsigned long long _Val,wchar_t *_DstBuf,size_t _SizeInWords,int _Radix);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wmakepath_s(wchar_t *_PathResult,size_t _SizeInWords,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wmakepath_s(wchar_t (&_PathResult)[__size], const wchar_t* _Drive, const wchar_t* _Dir, const wchar_t* _Filename, const wchar_t* _Ext) { return _wmakepath_s(_PathResult,__size,_Drive,_Dir,_Filename,_Ext); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wputenv_s(const wchar_t *_Name,const wchar_t *_Value);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsearchenv_s(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath,size_t _SizeInWords);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wsearchenv_s(const wchar_t* _Filename, const wchar_t* _EnvVar, wchar_t (&_ResultPath)[__size]) { return _wsearchenv_s(_Filename, _EnvVar, _ResultPath, __size); } }

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wsplitpath_s(const wchar_t *_FullPath,wchar_t *_Drive,size_t _DriveSizeInWords,wchar_t *_Dir,size_t _DirSizeInWords,wchar_t *_Filename,size_t _FilenameSizeInWords,wchar_t *_Ext,size_t _ExtSizeInWords);
  extern "C++" { template <size_t __drive_size, size_t __dir_size, size_t __name_size, size_t __ext_size> inline errno_t __attribute__((__cdecl__)) _wsplitpath_s(const wchar_t *_Dest, wchar_t (&__drive)[__drive_size], wchar_t (&__dir)[__dir_size], wchar_t (&__name)[__name_size], wchar_t (&__ext)[__ext_size]) { return _wsplitpath_s(_Dest, __drive, __drive_size, __dir, __dir_size, __name, __name_size, __ext, __ext_size); } }


}
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 1 3 4
# 34 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/syslimits.h" 1 3 4






#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 1 3 4
# 210 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/limits.h" 1 3 4
# 211 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 2 3 4
# 10 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/syslimits.h" 2 3 4
#pragma GCC diagnostic pop
# 35 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/limits.h" 2 3 4
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 26 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
#pragma pack(push,_CRT_PACKING)


extern "C" {
# 50 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef int (__attribute__((__cdecl__)) *_onexit_t)(void);
# 60 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef struct _div_t {
    int quot;
    int rem;
  } div_t;

  typedef struct _ldiv_t {
    long quot;
    long rem;
  } ldiv_t;





#pragma pack(4)
  typedef struct {
    unsigned char ld[10];
  } _LDOUBLE;
#pragma pack()



  typedef struct {
    double x;
  } _CRT_DOUBLE;

  typedef struct {
    float f;
  } _CRT_FLOAT;

       


  typedef struct {
    long double x;
  } _LONGDOUBLE;

       

#pragma pack(4)
  typedef struct {
    unsigned char ld12[12];
  } _LDBL12;
#pragma pack()
# 113 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
__attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) ___mb_cur_max_func(void);
# 135 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  typedef void (__attribute__((__cdecl__)) *_purecall_handler)(void);

  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _set_purecall_handler(_purecall_handler _Handler);
  __attribute__ ((__dllimport__)) _purecall_handler __attribute__((__cdecl__)) _get_purecall_handler(void);

  typedef void (__attribute__((__cdecl__)) *_invalid_parameter_handler)(const wchar_t *,const wchar_t *,const wchar_t *,unsigned int,uintptr_t);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _set_invalid_parameter_handler(_invalid_parameter_handler _Handler);
  __attribute__ ((__dllimport__)) _invalid_parameter_handler __attribute__((__cdecl__)) _get_invalid_parameter_handler(void);
# 151 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) unsigned long *__attribute__((__cdecl__)) __doserrno(void);

  errno_t __attribute__((__cdecl__)) _set_doserrno(unsigned long _Value);
  errno_t __attribute__((__cdecl__)) _get_doserrno(unsigned long *_Value);
  __attribute__ ((__dllimport__)) char **__attribute__((__cdecl__)) __sys_errlist(void);
  __attribute__ ((__dllimport__)) int *__attribute__((__cdecl__)) __sys_nerr(void);



  __attribute__ ((__dllimport__)) char ***__attribute__((__cdecl__)) __p___argv(void);
  __attribute__ ((__dllimport__)) int *__attribute__((__cdecl__)) __p__fmode(void);
  __attribute__ ((__dllimport__)) int *__attribute__((__cdecl__)) __p___argc(void);
  __attribute__ ((__dllimport__)) wchar_t ***__attribute__((__cdecl__)) __p___wargv(void);
  __attribute__ ((__dllimport__)) char **__attribute__((__cdecl__)) __p__pgmptr(void);
  __attribute__ ((__dllimport__)) wchar_t **__attribute__((__cdecl__)) __p__wpgmptr(void);

  errno_t __attribute__((__cdecl__)) _get_pgmptr(char **_Value);
  errno_t __attribute__((__cdecl__)) _get_wpgmptr(wchar_t **_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_fmode(int _Mode);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_fmode(int *_PMode);
# 221 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) char ***__attribute__((__cdecl__)) __p__environ(void);
  __attribute__ ((__dllimport__)) wchar_t ***__attribute__((__cdecl__)) __p__wenviron(void);
# 234 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__osplatform(void);
  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__osver(void);
  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__winver(void);
  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__winmajor(void);
  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__winminor(void);
# 256 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  errno_t __attribute__((__cdecl__)) _get_osplatform(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_osver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winver(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winmajor(unsigned int *_Value);
  errno_t __attribute__((__cdecl__)) _get_winminor(unsigned int *_Value);




  extern "C++" {
    template <typename _CountofType,size_t _SizeOfArray> char (*__countof_helper( _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];

  }
# 296 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) unsigned int __attribute__((__cdecl__)) _set_abort_behavior(unsigned int _Flags,unsigned int _Mask);



  int __attribute__((__cdecl__)) abs(int _X);
  long __attribute__((__cdecl__)) labs(long _X);


  __extension__ long long __attribute__((__cdecl__)) _abs64(long long);

  extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) long long __attribute__((__cdecl__)) _abs64(long long x) {
    return __builtin_llabs(x);
  }


  int __attribute__((__cdecl__)) atexit(void (__attribute__((__cdecl__)) *)(void));

  int __attribute__((__cdecl__)) at_quick_exit(void (__attribute__((__cdecl__)) *)(void));



  double __attribute__((__cdecl__)) atof(const char *_String);
  double __attribute__((__cdecl__)) _atof_l(const char *_String,_locale_t _Locale);

  int __attribute__((__cdecl__)) atoi(const char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoi_l(const char *_Str,_locale_t _Locale);
  long __attribute__((__cdecl__)) atol(const char *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _atol_l(const char *_Str,_locale_t _Locale);


  void *__attribute__((__cdecl__)) bsearch(const void *_Key,const void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));
  void __attribute__((__cdecl__)) qsort(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(const void *,const void *));

  unsigned short __attribute__((__cdecl__)) _byteswap_ushort(unsigned short _Short);
  unsigned long __attribute__((__cdecl__)) _byteswap_ulong (unsigned long _Long);
  __extension__ unsigned long long __attribute__((__cdecl__)) _byteswap_uint64(unsigned long long _Int64);
  div_t __attribute__((__cdecl__)) div(int _Numerator,int _Denominator);
  char *__attribute__((__cdecl__)) getenv(const char *_VarName) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _itoa(int _Value,char *_Dest,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _i64toa(long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ui64toa(unsigned long long _Val,char *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64(const char *_String);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _atoi64_l(const char *_String,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _strtoi64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64(const char *_String,char **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _strtoui64_l(const char *_String,char **_EndPtr,int _Radix,_locale_t _Locale);
  ldiv_t __attribute__((__cdecl__)) ldiv(long _Numerator,long _Denominator);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ltoa(long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) mblen(const char *_Ch,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mblen_l(const char *_Ch,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen(const char *_Str);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrlen_l(const char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen(const char *_Str,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstrnlen_l(const char *_Str,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mbtowc(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _mbtowc_l(wchar_t * __restrict__ _DstCh,const char * __restrict__ _SrcCh,size_t _SrcSizeInBytes,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) mbstowcs(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _mbstowcs_l(wchar_t * __restrict__ _Dest,const char * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) mkstemp(char *template_name);
  int __attribute__((__cdecl__)) rand(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_error_mode(int _Mode);
  void __attribute__((__cdecl__)) srand(unsigned int _Seed);
# 369 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
inline __attribute__((__cdecl__))
double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtod(const char * __restrict__ _Str,char ** __restrict__ _EndPtr)
{
  double __attribute__((__cdecl__)) __mingw_strtod (const char * __restrict__, char ** __restrict__);
  return __mingw_strtod( _Str, _EndPtr);
}

inline __attribute__((__cdecl__))
float __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtof(const char * __restrict__ _Str,char ** __restrict__ _EndPtr)
{
  float __attribute__((__cdecl__)) __mingw_strtof (const char * __restrict__, char ** __restrict__);
  return __mingw_strtof( _Str, _EndPtr);
}






  long double __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) strtold(const char * __restrict__ , char ** __restrict__ );


  extern double __attribute__((__cdecl__)) __attribute__ ((__nothrow__))
  __strtod (const char * __restrict__ , char ** __restrict__);







  float __attribute__((__cdecl__)) __mingw_strtof (const char * __restrict__, char ** __restrict__);
  double __attribute__((__cdecl__)) __mingw_strtod (const char * __restrict__, char ** __restrict__);
  long double __attribute__((__cdecl__)) __mingw_strtold(const char * __restrict__, char ** __restrict__);

  __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _strtof_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,_locale_t _Locale);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _strtod_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) strtol(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _strtol_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) strtoul(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _strtoul_l(const char * __restrict__ _Str,char ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);




  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ultoa(unsigned long _Value,char *_Dest,int _Radix) ;
  int __attribute__((__cdecl__)) wctomb(char *_MbCh,wchar_t _WCh) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wctomb_l(char *_MbCh,wchar_t _WCh,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcstombs(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcstombs_l(char * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _MaxCount,_locale_t _Locale) ;
# 452 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  void *__attribute__((__cdecl__)) calloc(size_t _NumOfElements,size_t _SizeOfElements);
  void __attribute__((__cdecl__)) free(void *_Memory);
  void *__attribute__((__cdecl__)) malloc(size_t _Size);
  void *__attribute__((__cdecl__)) realloc(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _aligned_free(void *_Memory);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_malloc(size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_malloc(size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_realloc(void *_Memory,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_realloc(void *_Memory,size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _recalloc(void *_Memory,size_t _Count,size_t _Size);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _aligned_msize(void *_Memory,size_t _Alignment,size_t _Offset);
# 487 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _itow(int _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ltow(long _Value,wchar_t *_Dest,int _Radix) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ultow(unsigned long _Value,wchar_t *_Dest,int _Radix) ;

  double __attribute__((__cdecl__)) __mingw_wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) __mingw_wcstof(const wchar_t * __restrict__ nptr, wchar_t ** __restrict__ endptr);
  long double __attribute__((__cdecl__)) __mingw_wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);
# 506 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  double __attribute__((__cdecl__)) wcstod(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr);
  float __attribute__((__cdecl__)) wcstof(const wchar_t * __restrict__ nptr, wchar_t ** __restrict__ endptr);


  long double __attribute__((__cdecl__)) wcstold(const wchar_t * __restrict__, wchar_t ** __restrict__);

  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wcstod_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,_locale_t _Locale);
  __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _wcstof_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,_locale_t _Locale);
  long __attribute__((__cdecl__)) wcstol(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wcstol_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  unsigned long __attribute__((__cdecl__)) wcstoul(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _wcstoul_l(const wchar_t * __restrict__ _Str,wchar_t ** __restrict__ _EndPtr,int _Radix,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wgetenv(const wchar_t *_VarName) ;




  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _wtof_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wtoi_l(const wchar_t *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol(const wchar_t *_Str);
  __attribute__ ((__dllimport__)) long __attribute__((__cdecl__)) _wtol_l(const wchar_t *_Str,_locale_t _Locale);

  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _i64tow(long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _ui64tow(unsigned long long _Val,wchar_t *_DstBuf,int _Radix) ;
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64(const wchar_t *_Str);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wtoi64_l(const wchar_t *_Str,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) long long __attribute__((__cdecl__)) _wcstoi64_l(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64(const wchar_t *_Str,wchar_t **_EndPtr,int _Radix);
  __extension__ __attribute__ ((__dllimport__)) unsigned long long __attribute__((__cdecl__)) _wcstoui64_l(const wchar_t *_Str ,wchar_t **_EndPtr,int _Radix,_locale_t _Locale);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _putenv(const char *_EnvString);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wputenv(const wchar_t *_EnvString);
# 550 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fullpath(char *_FullPath,const char *_Path,size_t _SizeInBytes);




  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl(_CRT_DOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl(_LDOUBLE *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt(_CRT_FLOAT *_Result,char *_Str);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atodbl_l(_CRT_DOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoldbl_l(_LDOUBLE *_Result,char *_Str,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _atoflt_l(_CRT_FLOAT *_Result,char *_Str,_locale_t _Locale);
# 579 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
unsigned long __attribute__((__cdecl__)) _lrotl(unsigned long,int);
unsigned long __attribute__((__cdecl__)) _lrotr(unsigned long,int);





  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _makepath(char *_Path,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
  _onexit_t __attribute__((__cdecl__)) _onexit(_onexit_t _Func);





       
       


  __extension__ unsigned long long __attribute__((__cdecl__)) _rotl64(unsigned long long _Val,int _Shift);
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long Value,int Shift);
       
       
       
       


  unsigned int __attribute__((__cdecl__)) _rotr(unsigned int _Val,int _Shift);
  unsigned int __attribute__((__cdecl__)) _rotl(unsigned int _Val,int _Shift);
       
       
  __extension__ unsigned long long __attribute__((__cdecl__)) _rotr64(unsigned long long _Val,int _Shift);
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _searchenv(const char *_Filename,const char *_EnvVar,char *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _splitpath(const char *_FullPath,char *_Drive,char *_Dir,char *_Filename,char *_Ext) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _swab(char *_Buf1,char *_Buf2,int _SizeInBytes);







  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wfullpath(wchar_t *_FullPath,const wchar_t *_Path,size_t _SizeInWords);



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wmakepath(wchar_t *_ResultPath,const wchar_t *_Drive,const wchar_t *_Dir,const wchar_t *_Filename,const wchar_t *_Ext);




  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsearchenv(const wchar_t *_Filename,const wchar_t *_EnvVar,wchar_t *_ResultPath) ;
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _wsplitpath(const wchar_t *_FullPath,wchar_t *_Drive,wchar_t *_Dir,wchar_t *_Filename,wchar_t *_Ext) ;


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _beep(unsigned _Frequency,unsigned _Duration) __attribute__ ((__deprecated__));

  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _seterrormode(int _Mode) __attribute__ ((__deprecated__));
  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _sleep(unsigned long _Duration) __attribute__ ((__deprecated__));
# 657 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
  char *__attribute__((__cdecl__)) ecvt(double _Val,int _NumOfDigits,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) fcvt(double _Val,int _NumOfDec,int *_PtDec,int *_PtSign) ;
  char *__attribute__((__cdecl__)) gcvt(double _Val,int _NumOfDigits,char *_DstBuf) ;
  char *__attribute__((__cdecl__)) itoa(int _Val,char *_DstBuf,int _Radix) ;
  char *__attribute__((__cdecl__)) ltoa(long _Val,char *_DstBuf,int _Radix) ;
  int __attribute__((__cdecl__)) putenv(const char *_EnvString) ;






  char *__attribute__((__cdecl__)) ultoa(unsigned long _Val,char *_Dstbuf,int _Radix) ;
  _onexit_t __attribute__((__cdecl__)) onexit(_onexit_t _Func);





  typedef struct { __extension__ long long quot, rem; } lldiv_t;

  __extension__ lldiv_t __attribute__((__cdecl__)) lldiv(long long, long long);

  __extension__ long long __attribute__((__cdecl__)) llabs(long long);




  __extension__ long long __attribute__((__cdecl__)) strtoll(const char * __restrict__, char ** __restrict, int);
  __extension__ unsigned long long __attribute__((__cdecl__)) strtoull(const char * __restrict__, char ** __restrict__, int);


  __extension__ long long __attribute__((__cdecl__)) atoll (const char *);


  __extension__ long long __attribute__((__cdecl__)) wtoll (const wchar_t *);
  __extension__ char *__attribute__((__cdecl__)) lltoa (long long, char *, int);
  __extension__ char *__attribute__((__cdecl__)) ulltoa (unsigned long long , char *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) lltow (long long, wchar_t *, int);
  __extension__ wchar_t *__attribute__((__cdecl__)) ulltow (unsigned long long, wchar_t *, int);
# 711 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 3
}


#pragma pack(pop)

# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 1 3
# 9 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 1 3
# 30 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 1 3
# 31 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 2 3
# 10 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sec_api/stdlib_s.h" 2 3


extern "C" {






  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _dupenv_s(char **_PBuffer,size_t *_PBufferSizeInBytes,const char *_VarName);




  __attribute__ ((__dllimport__)) void * __attribute__((__cdecl__)) bsearch_s(const void *_Key,const void *_Base,rsize_t _NumOfElements,rsize_t _SizeOfElements,int (__attribute__((__cdecl__)) * _PtFuncCompare)(void *, const void *, const void *), void *_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) getenv_s(size_t *_ReturnSize,char *_DstBuf,rsize_t _DstSize,const char *_VarName);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) getenv_s(size_t * _ReturnSize, char (&_Dest)[__size], const char * _VarName) { return getenv_s(_ReturnSize, _Dest, __size, _VarName); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _itoa_s(int _Value,char *_DstBuf,size_t _Size,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _itoa_s(int _Value, char (&_Dest)[__size], int _Radix) { return _itoa_s(_Value, _Dest, __size, _Radix); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _i64toa_s(long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ui64toa_s(unsigned long long _Val,char *_DstBuf,size_t _Size,int _Radix);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ltoa_s(long _Val,char *_DstBuf,size_t _Size,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _ltoa_s(long _Value, char (&_Dest)[__size], int _Radix) { return _ltoa_s(_Value, _Dest, __size, _Radix); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) mbstowcs_s(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) mbstowcs_s(size_t * _PtNumOfCharConverted, wchar_t (&_Dest)[__size], const char * _Source, size_t _MaxCount) { return mbstowcs_s(_PtNumOfCharConverted, _Dest, __size, _Source, _MaxCount); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _mbstowcs_s_l(size_t *_PtNumOfCharConverted,wchar_t *_DstBuf,size_t _SizeInWords,const char *_SrcBuf,size_t _MaxCount,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _mbstowcs_s_l(size_t * _PtNumOfCharConverted, wchar_t (&_Dest)[__size], const char * _Source, size_t _MaxCount, _locale_t _Locale) { return _mbstowcs_s_l(_PtNumOfCharConverted, _Dest, __size, _Source, _MaxCount, _Locale); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ultoa_s(unsigned long _Val,char *_DstBuf,size_t _Size,int _Radix);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _ultoa_s(unsigned long _Value, char (&_Dest)[__size], int _Radix) { return _ultoa_s(_Value, _Dest, __size, _Radix); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wctomb_s(int *_SizeConverted,char *_MbCh,rsize_t _SizeInBytes,wchar_t _WCh);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wctomb_s_l(int *_SizeConverted,char *_MbCh,size_t _SizeInBytes,wchar_t _WCh,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcstombs_s(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) wcstombs_s(size_t* _PtNumOfCharConverted, char (&_Dst)[__size], const wchar_t* _Src, size_t _MaxCountInBytes) { return wcstombs_s(_PtNumOfCharConverted, _Dst, __size, _Src, _MaxCountInBytes); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcstombs_s_l(size_t *_PtNumOfCharConverted,char *_Dst,size_t _DstSizeInBytes,const wchar_t *_Src,size_t _MaxCountInBytes,_locale_t _Locale);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _wcstombs_s_l(size_t* _PtNumOfCharConverted, char (&_Dst)[__size], const wchar_t* _Src, size_t _MaxCountInBytes, _locale_t _Locale) { return _wcstombs_s_l(_PtNumOfCharConverted, _Dst, __size, _Src, _MaxCountInBytes, _Locale); } }


  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _ecvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDights,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _fcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDec,int *_PtDec,int *_PtSign);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _gcvt_s(char *_DstBuf,size_t _Size,double _Val,int _NumOfDigits);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _makepath_s(char *_PathResult,size_t _Size,const char *_Drive,const char *_Dir,const char *_Filename,const char *_Ext);
  extern "C++" { template <size_t __size> inline errno_t __attribute__((__cdecl__)) _makepath_s(char (&_PathResult)[__size], const char* _Drive, const char* _Dir, const char* _Filename, const char* _Ext) { return _makepath_s(_PathResult,__size,_Drive,_Dir,_Filename,_Ext); } }
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _putenv_s(const char *_Name,const char *_Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _searchenv_s(const char *_Filename,const char *_EnvVar,char *_ResultPath,size_t _SizeInBytes);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _splitpath_s(const char *_FullPath,char *_Drive,size_t _DriveSize,char *_Dir,size_t _DirSize,char *_Filename,size_t _FilenameSize,char *_Ext,size_t _ExtSize);
  extern "C++" { template <size_t __drive_size, size_t __dir_size, size_t __name_size, size_t __ext_size> inline errno_t __attribute__((__cdecl__)) _splitpath_s(const char *_Dest, char (&__drive)[__drive_size], char (&__dir)[__dir_size], char (&__name)[__name_size], char (&__ext)[__ext_size]) { return _splitpath_s(_Dest, __drive, __drive_size, __dir, __dir_size, __name, __name_size, __ext, __ext_size); } }



  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) qsort_s(void *_Base,size_t _NumOfElements,size_t _SizeOfElements,int (__attribute__((__cdecl__)) *_PtFuncCompare)(void *,const void *,const void *),void *_Context);





}
# 717 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 1 3
# 11 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
#pragma pack(push,_CRT_PACKING)


extern "C" {
# 52 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
  typedef struct _heapinfo {
    int *_pentry;
    size_t _size;
    int _useflag;
  } _HEAPINFO;



  __attribute__ ((__dllimport__)) unsigned int *__attribute__((__cdecl__)) __p__amblksiz(void);
# 129 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
void * __mingw_aligned_malloc (size_t _Size, size_t _Alignment);
void __mingw_aligned_free (void *_Memory);
void * __mingw_aligned_offset_realloc (void *_Memory, size_t _Size, size_t _Alignment, size_t _Offset);
void * __mingw_aligned_offset_malloc (size_t, size_t, size_t);
void * __mingw_aligned_realloc (void *_Memory, size_t _Size, size_t _Offset);
size_t __mingw_aligned_msize (void *memblock, size_t alignment, size_t offset);



# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/mm_malloc.h" 1 3 4
# 27 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/mm_malloc.h" 3 4
# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 1 3 4
# 28 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/mm_malloc.h" 2 3 4

# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/errno.h" 1 3 4
# 12 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/errno.h" 3 4
extern "C" {
# 239 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/errno.h" 3 4
}
# 30 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/mm_malloc.h" 2 3 4


static __inline__ void *
_mm_malloc (size_t __size, size_t __align)
{
  void * __malloc_ptr;
  void * __aligned_ptr;


  if (__align & (__align - 1))
    {

      (*_errno()) = 22;

      return ((void *) 0);
    }

  if (__size == 0)
    return ((void *) 0);





    if (__align < 2 * sizeof (void *))
      __align = 2 * sizeof (void *);

  __malloc_ptr = malloc (__size + __align);
  if (!__malloc_ptr)
    return ((void *) 0);


  __aligned_ptr = (void *) (((size_t) __malloc_ptr + __align)
       & ~((size_t) (__align) - 1));


  ((void **) __aligned_ptr)[-1] = __malloc_ptr;

  return __aligned_ptr;
}

static __inline__ void
_mm_free (void *__aligned_ptr)
{
  if (__aligned_ptr)
    free (((void **) __aligned_ptr)[-1]);
}
# 139 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 2 3





  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _resetstkoflw (void);

  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _set_malloc_crt_max_wait(unsigned long _NewValue);







  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _expand(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _msize(void *_Memory);
# 167 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _get_sbh_threshold(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_sbh_threshold(size_t _NewValue);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_amblksiz(size_t _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_amblksiz(size_t *_Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapadd(void *_Memory,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapchk(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapmin(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapset(unsigned int _Fill);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapwalk(_HEAPINFO *_EntryInfo);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _heapused(size_t *_Used,size_t *_Commit);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_heap_handle(void);
# 190 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void *_MarkAllocaS(void *_Ptr,unsigned int _Marker) {
    if(_Ptr) {
      *((unsigned int*)_Ptr) = _Marker;
      _Ptr = (char*)_Ptr + 16;
    }
    return _Ptr;
  }
# 218 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void __attribute__((__cdecl__)) _freea(void *_Memory) {
    unsigned int _Marker;
    if(_Memory) {
      _Memory = (char*)_Memory - 16;
      _Marker = *(unsigned int *)_Memory;
      if(_Marker==0xDDDD) {
 free(_Memory);
      }





    }
  }
# 261 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/malloc.h" 3
}


#pragma pack(pop)
# 718 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stdlib.h" 2 3
# 84 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 2 3

#pragma GCC diagnostic pop

# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/bits/std_abs.h" 1 3
# 39 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/bits/std_abs.h" 3
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wlong-long"
# 52 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/bits/std_abs.h" 3
extern "C++"
{
namespace std
{


  using ::abs;


  inline long
  abs(long __i) { return __builtin_labs(__i); }



  inline long long
  abs(long long __x) { return __builtin_llabs (__x); }
# 76 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/bits/std_abs.h" 3
  inline constexpr double
  abs(double __x)
  { return __builtin_fabs(__x); }

  inline constexpr float
  abs(float __x)
  { return __builtin_fabsf(__x); }

  inline constexpr long double
  abs(long double __x)
  { return __builtin_fabsl(__x); }



  __extension__ inline constexpr __int128
  abs(__int128 __x) { return __x >= 0 ? __x : -__x; }
# 141 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/bits/std_abs.h" 3
  __extension__ inline constexpr
  __float128
  abs(__float128 __x)
  {






    return __builtin_signbit(__x) ? -__x : __x;

  }



}
}

#pragma GCC diagnostic pop
# 88 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 2 3
# 131 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
extern "C++"
{
namespace std
{


  using ::div_t;
  using ::ldiv_t;

  using ::abort;



  using ::atexit;


  using ::at_quick_exit;


  using ::atof;
  using ::atoi;
  using ::atol;
  using ::bsearch;
  using ::calloc;
  using ::div;
  using ::exit;
  using ::free;
  using ::getenv;
  using ::labs;
  using ::ldiv;
  using ::malloc;

  using ::mblen;
  using ::mbstowcs;
  using ::mbtowc;

  using ::qsort;


  using ::quick_exit;


  using ::rand;
  using ::realloc;
  using ::srand;
  using ::strtod;
  using ::strtol;
  using ::strtoul;
  using ::system;

  using ::wcstombs;
  using ::wctomb;



  inline ldiv_t
  div(long __i, long __j) noexcept { return ldiv(__i, __j); }




}
# 205 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
namespace __gnu_cxx
{



  using ::lldiv_t;





  using ::_Exit;



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
  using ::llabs;

  inline lldiv_t
  div(long long __n, long long __d)
  { lldiv_t __q; __q.quot = __n / __d; __q.rem = __n % __d; return __q; }

  using ::lldiv;
#pragma GCC diagnostic pop
# 240 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
  using ::atoll;
  using ::strtoll;
  using ::strtoull;

  using ::strtof;
  using ::strtold;


}

namespace std
{

  using ::__gnu_cxx::lldiv_t;

  using ::__gnu_cxx::_Exit;

  using ::__gnu_cxx::llabs;
  using ::__gnu_cxx::div;
  using ::__gnu_cxx::lldiv;

  using ::__gnu_cxx::atoll;
  using ::__gnu_cxx::strtof;
  using ::__gnu_cxx::strtoll;
  using ::__gnu_cxx::strtoull;
  using ::__gnu_cxx::strtold;
}
# 284 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/cstdlib" 3
}
# 37 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/c++/stdlib.h" 2 3

using std::abort;
using std::atexit;
using std::exit;


  using std::at_quick_exit;


  using std::quick_exit;


  using std::_Exit;




using std::div_t;
using std::ldiv_t;

using std::abs;
using std::atof;
using std::atoi;
using std::atol;
using std::bsearch;
using std::calloc;
using std::div;
using std::free;
using std::getenv;
using std::labs;
using std::ldiv;
using std::malloc;

using std::mblen;
using std::mbstowcs;
using std::mbtowc;

using std::qsort;
using std::rand;
using std::realloc;
using std::srand;
using std::strtod;
using std::strtol;
using std::strtoul;
using std::system;

using std::wcstombs;
using std::wctomb;
# 176 "schneider_lang.h" 2
# 89 "kernel_main_linux.cpp" 2
# 1 "cosmos_partition.h" 1






#pragma pack(push, 1)



# 10 "cosmos_partition.h"
struct GPTHeader {
    uint64_t signature;
    uint32_t revision;
    uint32_t headerSize;
    uint32_t headerCRC32;
    uint32_t reserved;
    uint64_t myLBA;
    uint64_t alternateLBA;
    uint64_t firstUsableLBA;
    uint64_t lastUsableLBA;
    uint8_t diskGUID[16];
    uint64_t partitionEntryLBA;
    uint32_t numPartitionEntries;
    uint32_t partitionEntrySize;
    uint32_t partitionEntryArrayCRC32;
};


struct GPTPartitionEntry {
    uint8_t partitionTypeGUID[16];
    uint8_t uniquePartitionGUID[16];
    uint64_t startingLBA;
    uint64_t endingLBA;
    uint64_t attributes;
    uint16_t partitionName[36];
};

#pragma pack(pop)


extern "C" void scan_partitions(uint8_t port_no);


extern uint64_t gpt_partition_starts[16];
extern int gpt_partition_count;
# 90 "kernel_main_linux.cpp" 2
# 1 "arcade.h" 1
       






struct VirtualMachine {
    const char* system_name;
    const char* file_extension;
    void (*reset)();
    void (*load_rom)(unsigned long long ram_addr, int size);
    void (*run_frame)();
    void (*render)(int wx, int wy, int ww, int wh);
    void (*os2_handle_input)(int scancode, bool is_pressed);
};


extern VirtualMachine emu_chip8;
extern VirtualMachine emu_gameboy;
extern VirtualMachine* active_vm;


extern void run_pong_engine(int wx, int wy, int ww, int wh, bool blocked);
extern void run_blobby_engine(int wx, int wy, int ww, int wh, bool blocked);

extern void run_browser_engine(int wx, int wy, int ww, int wh, bool blocked);


extern int pong_state;
extern int bv_state;
# 91 "kernel_main_linux.cpp" 2

# 1 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 1 3 4
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 2 "C:/ProgramData/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/include/stddef.h" 2 3 4
# 93 "kernel_main_linux.cpp" 2

# 1 "cosmos_fat32.h" 1






struct FAT32_BPB {
    uint8_t jump[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t dir_entries;
    uint16_t total_sectors_16;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat_16;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t sectors_per_fat_32;
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t nt_flags;
    uint8_t signature;
    uint32_t serial_number;
    char label[11];
    char sys_id[8];
    uint8_t boot_code[420];
    uint16_t boot_signature;
} __attribute__((packed));

struct FAT32_DirectoryEntry {
    char name[11];
    uint8_t attr;
    uint8_t nt_res;
    uint8_t crt_time_tenth;
    uint16_t crt_time;
    uint16_t crt_date;
    uint16_t lst_acc_date;
    uint16_t fst_clus_hi;
    uint16_t wrt_time;
    uint16_t wrt_date;
    uint16_t fst_clus_lo;
    uint32_t size;
} __attribute__((packed));

struct FAT32_LFN_Entry {
    uint8_t order;
    uint16_t name1[5];
    uint8_t attr;
    uint8_t type;
    uint8_t checksum;
    uint16_t name2[6];
    uint16_t fst_clus_lo;
    uint16_t name3[2];
} __attribute__((packed));

struct FAT32_ParsedFile {
    uint8_t exists;
    char name[64];
    uint32_t size;
    uint32_t start_lba;
    uint8_t is_folder;
    uint8_t parent_idx;
};

extern int fat32_data_start;
extern int fat32_sectors_per_cluster;
extern int fat32_root_lba;

extern bool fat32_init(int partition_lba, uint8_t* bpb_buffer);
extern bool fat32_list_dir(int folder_lba, uint8_t* dir_buffer, FAT32_ParsedFile* output_files, int max_files, uint8_t current_folder_id, int current_page_offset);
# 95 "kernel_main_linux.cpp" 2
# 1 "elf_loader.h" 1






typedef struct {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;


typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;




static void memcpy_elf(void* dest, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (uint64_t i = 0; i < n; i++) d[i] = s[i];
}

static void memset_elf(void* s, uint8_t c, uint64_t n) {
    uint8_t* p = (uint8_t*)s;
    for (uint64_t i = 0; i < n; i++) p[i] = c;
}



static uint64_t parse_and_load_elf(uint8_t* elf_data) {
    Elf64_Ehdr* header = (Elf64_Ehdr*)elf_data;


    if (header->e_ident[0] != 0x7F ||
        header->e_ident[1] != 'E' ||
        header->e_ident[2] != 'L' ||
        header->e_ident[3] != 'F') {
        return 0;
    }


    if (header->e_ident[4] != 2) return 0;

    Elf64_Phdr* phdrs = (Elf64_Phdr*)(elf_data + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (phdrs[i].p_type == 1) {
            uint8_t* dest = (uint8_t*)phdrs[i].p_vaddr;
            uint8_t* src = elf_data + phdrs[i].p_offset;


            if (phdrs[i].p_filesz > 0) {
                memcpy_elf(dest, src, phdrs[i].p_filesz);
            }


            if (phdrs[i].p_memsz > phdrs[i].p_filesz) {
                uint64_t bss_size = phdrs[i].p_memsz - phdrs[i].p_filesz;
                memset_elf(dest + phdrs[i].p_filesz, 0, bss_size);
            }
        }
    }

    return header->e_entry;
}
# 96 "kernel_main_linux.cpp" 2


# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/dirent.h" 1 3
# 19 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/dirent.h" 3
#pragma pack(push,_CRT_PACKING)



# 22 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/dirent.h" 3
extern "C" {


struct dirent
{
 long d_ino;
 unsigned short d_reclen;
 unsigned short d_namlen;
 char d_name[260];
};






typedef struct
{

 struct _finddata64i32_t dd_dta;




 struct dirent dd_dir;


 intptr_t dd_handle;







 int dd_stat;


 char dd_name[1];
} DIR;

DIR* __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) opendir (const char*);
struct dirent* __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) readdir (DIR*);
int __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) closedir (DIR*);
void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) rewinddir (DIR*);
long __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) telldir (DIR*);
void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) seekdir (DIR*, long);




struct _wdirent
{
 long d_ino;
 unsigned short d_reclen;
 unsigned short d_namlen;
 wchar_t d_name[260];
};





typedef struct
{

 struct _wfinddata64i32_t dd_dta;




 struct _wdirent dd_dir;


 intptr_t dd_handle;







 int dd_stat;


 wchar_t dd_name[1];
} _WDIR;

_WDIR* __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wopendir (const wchar_t*);
struct _wdirent* __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wreaddir (_WDIR*);
int __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wclosedir (_WDIR*);
void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wrewinddir (_WDIR*);
long __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wtelldir (_WDIR*);
void __attribute__((__cdecl__)) __attribute__ ((__nothrow__)) _wseekdir (_WDIR*, long);



}


#pragma pack(pop)
# 99 "kernel_main_linux.cpp" 2
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 1 3
# 16 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 3
#pragma pack(push,_CRT_PACKING)


extern "C" {
# 58 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 3
# 1 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_stat64.h" 1 3
# 22 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/_mingw_stat64.h" 3
  struct _stat32 {
    _dev_t st_dev;
   _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    _off_t st_size;
    __time32_t st_atime;
    __time32_t st_mtime;
    __time32_t st_ctime;
  };

  struct _stat32i64 {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    __extension__ long long st_size;
    __time32_t st_atime;
    __time32_t st_mtime;
    __time32_t st_ctime;
  };

  struct _stat64i32 {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    _off_t st_size;
    __time64_t st_atime;
    __time64_t st_mtime;
    __time64_t st_ctime;
  };

  struct _stat64 {
    _dev_t st_dev;
    _ino_t st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    _dev_t st_rdev;
    __extension__ long long st_size;
    __time64_t st_atime;
    __time64_t st_mtime;
    __time64_t st_ctime;
  };
# 59 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 2 3
# 69 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fstat32(int _FileDes,struct _stat32 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stat32(const char *_Name,struct _stat32 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fstat64(int _FileDes,struct _stat64 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fstat32i64(int _FileDes,struct _stat32i64 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fstat64i32(int _FileDes,struct _stat64i32 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stat64(const char *_Name,struct _stat64 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stat32i64(const char *_Name,struct _stat32i64 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stat64i32(const char *_Name,struct _stat64i32 *_Stat);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wstat32(const wchar_t *_Name,struct _stat32 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wstat32i64(const wchar_t *_Name,struct _stat32i64 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wstat64i32(const wchar_t *_Name,struct _stat64i32 *_Stat);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wstat64(const wchar_t *_Name,struct _stat64 *_Stat);
# 137 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 3
struct stat {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  off_t st_size;
  time_t st_atime;
  time_t st_mtime;
  time_t st_ctime;
};
# 166 "C:/ProgramData/mingw64/mingw64/x86_64-w64-mingw32/include/sys/stat.h" 3
int __attribute__((__cdecl__)) fstat(int _Desc, struct stat *_Stat) __asm__("_fstat64i32");
int __attribute__((__cdecl__)) stat(const char *_Filename, struct stat *_Stat) __asm__("stat64i32");
int __attribute__((__cdecl__)) wstat(const wchar_t *_Filename, struct stat *_Stat) __asm__("wstat64i32");




struct stat64 {
  _dev_t st_dev;
  _ino_t st_ino;
  unsigned short st_mode;
  short st_nlink;
  short st_uid;
  short st_gid;
  _dev_t st_rdev;
  __extension__ long long st_size;
  __time64_t st_atime;
  __time64_t st_mtime;
  __time64_t st_ctime;
};
int __attribute__((__cdecl__)) fstat64(int _Desc, struct stat64 *_Stat);
int __attribute__((__cdecl__)) stat64(const char *_Filename, struct stat64 *_Stat);
int __attribute__((__cdecl__)) wstat64(const wchar_t *_Filename, struct stat64 *_Stat);




}


#pragma pack(pop)
# 100 "kernel_main_linux.cpp" 2






# 105 "kernel_main_linux.cpp"
char linux_file_paths[28][256];


extern "C" void c8_reset();
extern unsigned char c8_ram[4096];
extern unsigned short c8_pc;
extern int c8_state;

extern bool browser_download_complete;
extern uint8_t browser_download_buffer[3000000];
extern int browser_download_len;
extern int browser_content_length;
extern char browser_url[512];
extern "C" void send_dns_query(const char* domain);
extern void send_tcp_syn(uint32_t dest_ip, uint16_t dest_port);
extern uint32_t ip_str_to_u32(const char* ip_str);
struct TCPSocket {
    uint32_t remote_ip;
    uint16_t local_port;
    uint16_t remote_port;
    uint32_t my_seq;
    uint32_t my_ack;
    uint32_t remote_seq;
    int state;
};
extern TCPSocket browser_tcp;

bool pkg_download_active = 0;
bool pkg_download_execute = 0;
uint32_t pkg_target_ip = 0;
uint16_t pkg_target_port = 80;
void* pkg_output_win_ptr = 0;





struct DriveInfo { int type; int size_mb; int base_port; char model[41]; };


extern DriveInfo drives[8];
extern int drive_count;
extern uint32_t usb_io_base;
extern uint64_t global_xhci_base_addr;


extern bool ahci_read_sectors(int port_no, int lba, int count, uint64_t buffer_addr);
extern bool ahci_write_sectors(int port_no, int lba, int count, uint64_t buffer_addr);
extern bool usb_bot_read_sectors(int dev_addr, int ep_out, int ep_in, uint32_t lba, int num_sectors, uint8_t* buffer);
extern int usb_bot_get_capacity(int dev_addr, int ep_out, int ep_in);
extern bool usb_enumerate_device(int port_idx, int new_address);


static inline uint16_t inw(uint16_t port) {
    return 0;
}

extern uint32_t screen_w;
extern uint32_t screen_h;
uint32_t screen_pitch = 1920 * 4;
uint32_t* os2_fb = nullptr;
extern uint32_t* bb;

extern uint32_t cartridge_start;
extern uint32_t cartridge_end;

bool is_app_running = false;


extern uint32_t global_ahci_abar;
struct HBA_PORT;
extern HBA_PORT* get_active_ahci_port();
extern bool ahci_read(HBA_PORT* port, uint32_t startlba, void* target_ram_address);

extern "C" void disable_nx_for_app(unsigned long long virtual_addr, unsigned long long size_in_bytes);



uint32_t os2_net_bar0 = 0;
uint16_t os2_net_vendor = 0;
uint16_t os2_net_device = 0;

uint32_t os2_pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    return 0;
}

extern void map_mmio_64(uint64_t phys_addr);
extern void intel_e1000_init(uint32_t mmio_addr, uint16_t device_id);

extern uint32_t active_ahci_bar5;
void os2_smart_scan() {
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint32_t slot = 0; slot < 32; slot++) {
            for (uint32_t f = 0; f < 8; f++) {
                uint32_t vendor_device = os2_pci_read(bus, slot, f, 0);
                if (vendor_device != 0xFFFFFFFF) {
                    uint32_t class_reg = os2_pci_read(bus, slot, f, 0x08);
                    uint8_t class_code = (class_reg >> 24) & 0xFF;


                    if (class_code == 0x02) {
                        os2_net_vendor = vendor_device & 0xFFFF;
                        os2_net_device = vendor_device >> 16;
                        uint32_t bar0 = os2_pci_read(bus, slot, f, 0x10);
                        uint32_t clean_bar0 = bar0 & 0xFFFFFFF0;
                        if ((bar0 & 0x06) == 0x04) {
                            uint32_t bar1 = os2_pci_read(bus, slot, f, 0x14);
                            if (bar1 != 0) continue;
                        }
                        if (clean_bar0 != 0 && os2_net_bar0 == 0) {
                            os2_net_bar0 = clean_bar0;
                            uint32_t pci_cmd = os2_pci_read(bus, slot, f, 0x04);
                            uint32_t pci_addr = 0x80000000 | (bus << 16) | (slot << 11) | (f << 8) | 0x04;





                            map_mmio_64(os2_net_bar0);

                            if (os2_net_vendor == 0x8086) {
                                intel_e1000_init(os2_net_bar0, os2_net_device);
                            }
                        }
                    } else if (class_code == 0x01 && ((class_reg >> 16) & 0xFF) == 0x06) {

                        uint32_t bar5 = os2_pci_read(bus, slot, f, 0x24) & 0xFFFFFFF0;
                        if(bar5 != 0 && active_ahci_bar5 == 0) {
                            active_ahci_bar5 = bar5;
                            global_ahci_abar = bar5;
                            map_mmio_64(bar5);
                            struct HBA_MEM { uint32_t cap, ghc, is, pi, vs, ccc_ctl, ccc_pts, em_loc, em_ctl, cap2, bohc; uint8_t rsv[0x70]; struct HBA_PORT { uint32_t clb, clbu, os2_fb, fbu, is, ie, cmd, rsv1, tfd, sig, ssts, sctl, serr, sact, ci, sntf, fbs; } ports[32]; };
                            HBA_MEM* hba = (HBA_MEM*)bar5;
                            hba->ghc |= 0x80000000;
                            hba->ghc &= ~(1 << 1);
                            for(int i=0; i<32; i++) {
                                if(hba->pi & (1 << i)) {
                                    uint32_t ssts = hba->ports[i].ssts;
                                    if((ssts & 0x0F) == 3 && ((ssts >> 8) & 0x0F) == 1) {
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void internal_test_task() {
    while(1) {

        char* vga = (char*)0xB8000;
        vga[0] = 'T';
        vga[1] = 0x0A;


        for(volatile int i = 0; i < 1000000; i++) {}


        ;
    }
}




extern void run_browser_engine(int wx, int wy, int ww, int wh, bool blocked);
extern void run_pong_engine(int wx, int wy, int ww, int wh, bool blocked);
extern void run_blobby_engine(int wx, int wy, int ww, int wh, bool blocked);
extern void run_holyspirit(int wx, int wy, int ww, int wh, bool blocked);
extern void run_smash_cats_engine(int wx, int wy, int ww, int wh, bool blocked);
extern void browser_handle_keyboard(int os2_key_scancode, uint8_t ascii);
void run_emulator_engine(int, int, int, int, bool);




volatile uint64_t os2_system_ticks = 0;
uint64_t last_window_click = 0;


int task_quantum = 0;


void yield() {
    task_quantum = 15;




    ;
}




void dynamic_task_worker() {
    while(1) {

        volatile int z = 0;
        z++;
        yield();
    }
}



struct Task {
    uint64_t rsp;
    uint8_t stack[8192];
    bool active;
    bool paused;
};

Task tasks[4];
int current_task = 0;
int num_tasks = 1;




struct InterruptFrame {

    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;


    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));
void sys_idle_task() {
    while(1) {



        ;
    }
}

void create_task(void (*entry_point)()) {

    ;

    int id = -1;
    for (int i = 1; i < 4; i++) {
        if (!tasks[i].active) {
            id = i;
            break;
        }
    }
    if (id == -1 && num_tasks < 4) {
        id = num_tasks;
    }
    if (id == -1) {
        ;
        return;
    }






    uint64_t stack_base = 0x01200000 + (id * 0x100000);
    stack_base &= ~0xF;

    InterruptFrame* os2_frame = (InterruptFrame*)(stack_base - sizeof(InterruptFrame));


    uint8_t* frame_ptr = (uint8_t*)os2_frame;
    for (uint32_t i = 0; i < sizeof(InterruptFrame); i++) {
        frame_ptr[i] = 0;
    }


    uint64_t current_cs = 0, current_ss = 0;
    ;
    ;


    os2_frame->rip = (uint64_t)entry_point;
    os2_frame->cs = current_cs;
    os2_frame->ss = current_ss;



    os2_frame->rflags = 0x0202;


    os2_frame->rsp = stack_base - 8;


    tasks[id].rsp = (uint64_t)os2_frame;
    tasks[id].active = true;
    tasks[id].paused = false;
    if (id >= num_tasks) num_tasks = id + 1;


    ;
}

extern "C" uint64_t schedule(uint64_t old_rsp) {
    os2_system_ticks++;

    tasks[current_task].rsp = old_rsp;


    for (int i = 0; i < num_tasks; i++) {
        current_task++;
        if (current_task >= num_tasks) current_task = 0;
        if (tasks[current_task].active && !tasks[current_task].paused) {
            break;
        }
    }




    return tasks[current_task].rsp;
}




__attribute__((naked)) void pit_isr() {
    ;
}



extern void run_cosmos_script(char* file_buffer, int file_size);
extern char term_buffer[15][64];
int os2_save_step = 0;
char os2_save_filename[32] = "NEWFILE";
int save_as_mode = 0;
int os2_save_name_idx = 7;
char os2_new_folder_name[32] = "NEWDIR";
int os2_folder_name_idx = 6;




uint8_t user_programs[4][65536] __attribute__((aligned(4096)));



void os2_str_cat(char* dest, const char* src) {

    while (*dest) {
        dest++;
    }

    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;
}



uint32_t pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);


    ;
    return 0;
}
struct MirrorEntry {
    uint32_t bus, dev, func;
    uint32_t vendor, device;
    uint64_t bar0;
    char name[32];
};
MirrorEntry os2_mirror_list[32];
int os2_mirror_count = 0;
bool show_oracle = false;

extern void pci_scan_all();
extern bool xhci_bot_read_sectors(uint8_t slot_id, int lba, uint32_t dest_ram);
extern void xhci_poll_events_and_mouse();
extern void (*usb_mouse_callback)(int, int, int);
extern "C" void check_incoming();
extern "C" void net_check_link();
extern "C" void send_arp_ping();





extern int selected_drive_idx;
extern int ahci_read_sectors(uint32_t lba, uint64_t dest_ram);
extern bool xhci_bot_read_sectors(uint8_t slot_id, uint32_t lba, uint64_t dest_ram);
extern char linux_dev_paths[8][48];




struct CFS_DIR_ENTRY {
    uint8_t type;
    char filename[11];
    uint16_t start_lba;
    uint16_t file_size;
    uint8_t parent_idx;
    uint8_t reserved;
} __attribute__((packed));

bool disk_read_auto(uint32_t lba, uint64_t target_ram_addr) {

    if (lba == 1002) {

        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)target_ram_addr;
        for (int i=0; i<28; i++) entries[i].type = 0;

        const char* path = "/opt/meinos";
        if (selected_drive_idx == 1) path = "/media";

        DIR* dir = opendir(path);
        if (dir) {
            int slot = 0;
            struct dirent* ent;
            while ((ent = readdir(dir)) != 
# 533 "kernel_main_linux.cpp" 3 4
                                          __null 
# 533 "kernel_main_linux.cpp"
                                               && slot < 28) {
                if (ent->d_name[0] == '.') continue;
                entries[slot].type = (ent->d_type == DT_DIR) ? 2 : 1;
                strncpy((char*)entries[slot].filename, ent->d_name, 10);
                entries[slot].filename[10] = 0;
                entries[slot].start_lba = 1000000 + slot;

                char full_path[256];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);
                strncpy(linux_file_paths[slot], full_path, 255);
                linux_file_paths[slot][255] = 0;

                struct stat st;
                if (stat(full_path, &st) == 0) {
                    entries[slot].file_size = st.st_size;
                } else {
                    entries[slot].file_size = 0;
                }
                slot++;
            }
            closedir(dir);
        }
        return true;
    } else if (lba >= 1000000 && lba < 1000028) {

        int slot = lba - 1000000;
        FILE* f = fopen(linux_file_paths[slot], "rb");
        if (f) {
            fread((void*)target_ram_addr, 1, 1024*1024, f);
            fclose(f);
        }
        return true;
    }



    if (selected_drive_idx >= 0 && selected_drive_idx < 8 && linux_dev_paths[selected_drive_idx][0] != 0) {
        FILE* f = fopen(linux_dev_paths[selected_drive_idx], "rb");
        if (f) {
            off_t offset = (off_t)lba * 512;
            fseeko(f, offset, 
# 573 "kernel_main_linux.cpp" 3
                             0
# 573 "kernel_main_linux.cpp"
                                     );
            size_t read_bytes = fread((void*)target_ram_addr, 1, 512, f);
            fclose(f);
            if (read_bytes == 512) return true;
        }
    }
    return true;
# 617 "kernel_main_linux.cpp"
}




extern bool xhci_bot_write_sectors(uint8_t slot_id, uint32_t lba, uint64_t dest_ram);

bool disk_write_auto(uint32_t lba, uint64_t source_ram_addr) {

    if (lba == 1002) {

        FILE* f = fopen("/opt/meinos/.cosmos_dir", "wb");
        if (f) {
            fwrite((void*)source_ram_addr, 1, 512, f);
            fclose(f);
        }
        return true;
    } else if (lba >= 1000000 && lba < 1000028) {

        int slot = lba - 1000000;
        if (linux_file_paths[slot][0] != 0) {
            FILE* f = fopen(linux_file_paths[slot], "wb");
            if (f) {
                fwrite((void*)source_ram_addr, 1, 512, f);
                fclose(f);
            }
        }
        return true;
    }

    if (selected_drive_idx >= 0 && selected_drive_idx < 8 && linux_dev_paths[selected_drive_idx][0] != 0) {
        FILE* f = fopen(linux_dev_paths[selected_drive_idx], "r+b");
        if (f) {
            off_t offset = (off_t)lba * 512;
            fseeko(f, offset, 
# 651 "kernel_main_linux.cpp" 3
                             0
# 651 "kernel_main_linux.cpp"
                                     );
            fwrite((void*)source_ram_addr, 1, 512, f);
            fclose(f);
            return true;
        }
    }

    {
        char sector_path[256];
        snprintf(sector_path, sizeof(sector_path), "/opt/meinos/.cosmos_sectors/lba_%u.bin", lba);
        mkdir("/opt/meinos/.cosmos_sectors", 0755);
        FILE* f = fopen(sector_path, "wb");
        if (f) {
            fwrite((void*)source_ram_addr, 1, 512, f);
            fclose(f);
        }
    }
    return true;
# 706 "kernel_main_linux.cpp"
}




static inline void outl(uint16_t port, uint32_t val) { ; }
static inline uint32_t inl(uint16_t port) { return 0; }
static inline void outw(uint16_t port, uint16_t val) { ; }







extern int init_xhci_probe(uint64_t base_addr, int id);

void find_and_init_usb() {
    extern char os2_cmd_status[256];
    extern void os2_str_cpy(char* d, const char* s);
    usb_io_base = 0;

    for(int bus = 0; bus < 256; bus++) {
        for(int slot = 0; slot < 32; slot++) {
            for(int func = 0; func < 8; func++) {
                uint32_t vendor_device = pci_read(bus, slot, func, 0);

                if ((vendor_device & 0xFFFF) != 0xFFFF) {
                    uint32_t class_info = pci_read(bus, slot, func, 0x08);
                    uint8_t class_code = (class_info >> 24) & 0xFF;
                    uint8_t subclass = (class_info >> 16) & 0xFF;
                    uint8_t prog_if = (class_info >> 8) & 0xFF;

                    if (class_code == 0x0C && subclass == 0x03) {


                        if (prog_if == 0x30) {
                            uint32_t bar0 = pci_read(bus, slot, func, 0x10);
                            uint32_t clean_bar0 = bar0 & 0xFFFFFFF0;
                            uint32_t bar1 = pci_read(bus, slot, func, 0x14);

                            uint64_t full_bar = clean_bar0;
                            if ((bar0 & 0x04) != 0) {
                                full_bar = ((uint64_t)bar1 << 32) | clean_bar0;
                            }

                            os2_str_cpy(os2_cmd_status, "xHCI (USB 3.0) FOUND!");
                            init_xhci_probe(full_bar, 1);

                        }

                        else if (prog_if == 0x00) {

                            uint32_t pci_cmd_addr = 0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | 0x04;
                            outl(0xCF8, pci_cmd_addr);
                            uint32_t pci_cmd = inl(0xCFC);
                            outl(0xCF8, pci_cmd_addr);
                            outl(0xCFC, pci_cmd | 0x05);


                            uint32_t bar4 = pci_read(bus, slot, func, 0x20);
                            if(bar4 & 0x01) {
                                usb_io_base = bar4 & 0xFFFC;
                                os2_str_cpy(os2_cmd_status, "UHCI (USB 1.1) FOUND!");
                            }
                        }

                        else if(prog_if == 0x20) {
                            uint32_t bar0 = pci_read(bus, slot, func, 0x10);
                            extern uint64_t global_ehci_base_addr;
                            global_ehci_base_addr = bar0 & 0xFFFFFFF0;


                            os2_str_cpy(os2_cmd_status, "EHCI (USB 2.0) FOUND!");
                        }
                    }
                }
            }
        }
    }
}


int ahci_write_sectors(uint32_t lba, uint64_t buffer_addr) {
    return disk_write_auto(lba, buffer_addr);
}






int ahci_read_sectors(uint32_t lba, uint64_t buffer_addr) {
    return disk_read_auto(lba, buffer_addr);
}



extern int ahci_write_sectors(uint32_t lba, uint64_t src_ram);
extern bool xhci_bot_write_sectors(uint8_t slot_id, uint32_t lba, uint64_t src_ram);


void mem_set(void* ptr, uint8_t value, uint32_t num) {
    uint8_t* p = (uint8_t*)ptr;
    while (num--) *p++ = value;
}
extern "C" void ahci_mount_drive();
extern "C" void usb_scan_and_mount();
extern void ahci_read_mbr();
extern "C" void play_hda_wav(uint32_t pcm_addr, uint32_t size_bytes, uint16_t sample_rate, uint16_t channels, uint16_t bits);
# 852 "kernel_main_linux.cpp"
inline uint8_t inb(uint16_t port) { return 0; }
inline void outb(uint16_t port, uint8_t val) { ; }
void os2_mouse_wait(uint8_t type) { uint32_t t = 100000; while(t--) { if(type == 0 && (inb(0x64)&1)) return; if(type == 1 && !(inb(0x64)&2)) return; } }
void os2_mouse_write(uint8_t w) { os2_mouse_wait(1); outb(0x64, 0xD4); os2_mouse_wait(1); outb(0x60, w); }
uint8_t os2_mouse_read() { os2_mouse_wait(0); return inb(0x60); }
void os2_init_mouse() { os2_mouse_wait(1); outb(0x64,0xA8); os2_mouse_wait(1); outb(0x64,0x20); os2_mouse_wait(0); uint8_t s=inb(0x60)|2; os2_mouse_wait(1); outb(0x64,0x60); os2_mouse_wait(1); outb(0x60,s); os2_mouse_write(0xF6); os2_mouse_read(); os2_mouse_write(0xF4); os2_mouse_read(); }



int os2_rtc_h, os2_rtc_m, os2_rtc_day, os2_rtc_mon, os2_rtc_year;
uint8_t os2_bcd2bin(uint8_t b) { return ((b >> 4) * 10) + (b & 0xF); }
void os2_read_rtc() {
    outb(0x70, 4); os2_rtc_h = os2_bcd2bin(inb(0x71));
    outb(0x70, 2); os2_rtc_m = os2_bcd2bin(inb(0x71));
    outb(0x70, 7); os2_rtc_day = os2_bcd2bin(inb(0x71));
    outb(0x70, 8); os2_rtc_mon = os2_bcd2bin(inb(0x71));
    outb(0x70, 9); os2_rtc_year = os2_bcd2bin(inb(0x71));
    os2_rtc_h = (os2_rtc_h + 1) % 24;
}


uint32_t os2_frame = 0;
extern int mouse_x; extern int mouse_y; extern bool mouse_down; extern bool mouse_just_pressed;
extern bool key_new;
bool os2_mouse_right_down = false;
int mouse_sub_x = 40000;
int mouse_sub_y = 30000;


int mouse_sens = 120;



void update_mouse_position(int dx, int dy, int btn) {



    int real_dx = (signed char)dx;
    int real_dy = (signed char)dy;


    mouse_sub_x += (real_dx * mouse_sens);
    mouse_sub_y += (real_dy * mouse_sens);


    int new_x = mouse_sub_x / 100;
    int new_y = mouse_sub_y / 100;


    if (new_x < 0) { new_x = 0; mouse_sub_x = 0; }
    if (new_x > 799) { new_x = 799; mouse_sub_x = 799 * 100; }

    if (new_y < 0) { new_y = 0; mouse_sub_y = 0; }
    if (new_y > 599) { new_y = 599; mouse_sub_y = 599 * 100; }


    mouse_x = new_x;
    mouse_y = new_y;


    if (btn & 1) {
        if (!mouse_down) mouse_just_pressed = true;
        else mouse_just_pressed = false;
        mouse_down = true;
    } else {
        mouse_down = false;
        mouse_just_pressed = false;
    }




    if (btn & 2) {
        os2_mouse_right_down = true;
    } else {
        os2_mouse_right_down = false;
    }
}
static uint8_t m_packet[3]; static int m_ptr = 0;
bool os2_galaxy_open = false; int os2_galaxy_expansion = 0;
int os2_input_cooldown = 0; bool click_consumed = false;
struct Window { int id; char title[16]; int x, y, w, h; bool open, minimized, fullscreen; uint32_t color; char content[2048]; int cursor_pos; };
struct Planet { int ang; int dist; char name[8]; int cur_x, cur_y; };
struct Star { int x, y, z, type, speed; };
int os2_win_z[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
Window os2_windows[20];




void os2_focus_window(int id);
extern void os2_str_cpy(char* d, const char* s);
void os2_debug_print(const char* msg) {
    if(!os2_windows[7].open) {
        os2_windows[7].open = true;
        os2_windows[7].minimized = false;
        os2_str_cpy(os2_windows[7].title, "SYSTEM DEBUG (POPUP)");
        os2_windows[7].x = 200; os2_windows[7].y = 10;
        os2_windows[7].w = 400; os2_windows[7].h = 200;
        os2_windows[7].color = 0x550000;
        os2_windows[7].cursor_pos = 0;
        for(int i=0; i<2048; i++) os2_windows[7].content[i] = 0;
        os2_focus_window(7);
    }

    int len = 0;
    while(os2_windows[7].content[len] && len < 4000) len++;


    if(len > 1800) {
        len = 0;
        for(int i=0; i<2048; i++) os2_windows[7].content[i] = 0;
    }

    int m = 0;
    while(msg[m] && len < 2000) {
        char c = msg[m++];
        if (c >= 'a' && c <= 'z') c = c - 32;
        if (c == '=') c = ':';
        os2_windows[7].content[len++] = c;
    }
    os2_windows[7].content[len] = '\n';
    os2_windows[7].content[len+1] = 0;
    os2_windows[7].cursor_pos = len + 1;
}

Planet os2_planets[5];
Star os2_stars[200];
int os2_drag_win = -1; int os2_drag_off_x = 0; int os2_drag_off_y = 0; int os2_resize_win = -1; bool os2_z_blocked = false;
static uint32_t rng_seed = 123456789;
uint32_t os2_cosmos_random() { rng_seed = (rng_seed * 1103515245 + 12345) & 0x7FFFFFFF; return rng_seed; }
int os2_int_sqrt(int n) { int x=n, y=1; while(x>y){x=(x+y)/2; y=n/x;} return x; }
const int sin_lut[256] = { 1, 2, 4, 7, 9, 12, 14, 17, 19, 21, 24, 26, 28, 30, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 56, 58, 60, 61, 63, 64, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 79, 80, 81, 81, 82, 82, 83, 83, 83, 84, 84, 84, 84, 84, 84, 84, 83, 83, 83, 82, 82, 81, 81, 80, 79, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 68, 67, 66, 64, 63, 61, 60, 58, 56, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 30, 28, 26, 24, 21, 19, 17, 14, 12, 9, 7, 4, 2, 1, -1, -2, -4, -7, -9, -12, -14, -17, -19, -21, -24, -26, -28, -30, -33, -35, -37, -39, -41, -43, -45, -47, -49, -51, -53, -55, -56, -58, -60, -61, -63, -64, -66, -67, -68, -70, -71, -72, -73, -74, -75, -76, -77, -78, -79, -79, -80, -81, -81, -82, -82, -83, -83, -83, -84, -84, -84, -84, -84, -84, -84, -83, -83, -83, -82, -82, -81, -81, -80, -79, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -68, -67, -66, -64, -63, -61, -60, -58, -56, -55, -53, -51, -49, -47, -45, -43, -41, -39, -37, -35, -33, -30, -28, -26, -24, -21, -19, -17, -14, -12, -9, -7, -4, -2, -1 };
int os2_Cos(int a) { return sin_lut[(a + 64) % 256]; }
int os2_Sin(int a) { return sin_lut[a % 256]; }
void os2_str_cpy(char* d, const char* s) { while(*s) *d++ = *s++; *d=0; }
int os2_str_len(const char* s) { int l=0; while(*s++)l++; return l; }
bool os2_is_over(int mx, int my, int ox, int oy, int r) { return (mx-ox)*(mx-ox) + (my-oy)*(my-oy) < r*r; }
bool os2_is_over_rect(int mx, int my, int x, int y, int w, int h) { return (mx >= x && mx <= x+w && my >= y && my <= y+h); }



void os2_byte_to_hex(unsigned char byte, char* str) {
    const char* hex_digits = "0123456789ABCDEF";
    str[0] = hex_digits[(byte >> 4) & 0x0F];
    str[1] = hex_digits[byte & 0x0F];
    str[2] = 0;
}

bool os2_str_equal(const char* s1, const char* s2) {
    while(*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (*(const unsigned char*)s1 == *(const unsigned char*)s2);
}
bool os2_str_starts(const char* full, const char* prefix) {
    while(*prefix) { if(*prefix++ != *full++) return false; }
    return true;
}
void print_win(Window* win, const char* text) {
    while (*text && win->cursor_pos < 2000) { win->content[win->cursor_pos++] = *text++; }
    win->content[win->cursor_pos] = 0;
}
void hex_to_str(uint32_t val, char* buf) {
    const char hex_chars[] = "0123456789ABCDEF"; buf[0] = '0'; buf[1] = 'x'; buf[10] = '\0';
    for(int i = 7; i >= 0; i--) { buf[i + 2] = hex_chars[val & 0xF]; val >>= 4; }
}



uint32_t active_ahci_bar5 = 0;
uint32_t active_sata_port = 0;
uint32_t detected_ports[8];
int detected_port_count = 0;
int selected_drive_idx = -1;





struct FileEntry { uint8_t exists; char name[64]; uint16_t size; uint16_t start_lba; uint8_t is_folder; uint8_t parent_idx; };
FileEntry cfs_files[28];
uint32_t active_file_lba = 0;
uint32_t active_file_idx = 0;

uint8_t current_folder_id = 255;
bool dsk_mgr_opened = false;

uint32_t drive_total_gb = 0;
uint32_t drive_used_kb = 0;
bool is_mounted = false;
void int_to_str(uint32_t n, char* s) { if(n==0){s[0]='0';s[1]=0;return;} int i=0; uint32_t t=n; while(t>0){t/=10;i++;} s[i]=0; while(n>0){s[--i]=(n%10)+'0';n/=10;} }



struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct IDTPtr { uint16_t limit; uint64_t base; } __attribute__((packed));
IDTEntry idt[256]; IDTPtr idt_ptr;

void set_idt_gate(int n, uint64_t handler, uint8_t flags = 0x8E) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].ist = 0;
    idt[n].type_attr = flags;
    idt[n].offset_mid = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

void remap_pic() { outb(0x20, 0x11); outb(0xA0, 0x11); outb(0x21, 0x20); outb(0xA1, 0x28); outb(0x21, 0x04); outb(0xA1, 0x02); outb(0x21, 0x01); outb(0xA1, 0x01); outb(0x21, 0xFE); outb(0xA1, 0xFF); }
struct interrupt_frame;
int sys_selected_item = 0;
int sys_max_items = 5;
uint8_t os2_key_scancode = 0;
bool key_ready = false;

void init_pit(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

__attribute__((interrupt, target("general-regs-only")))
void dummy_isr(struct interrupt_frame* os2_frame) {
    outb(0x20, 0x20);
}

__attribute__((interrupt, target("general-regs-only")))
void keyboard_isr(struct interrupt_frame* os2_frame) {
    os2_key_scancode = inb(0x60);
    key_ready = true;
    outb(0x20, 0x20);
}
char get_ascii_qwertz(uint8_t sc) {
    char k_low[128] = { 0 };
    k_low[1] = 27; k_low[2] = '1'; k_low[3] = '2'; k_low[4] = '3'; k_low[5] = '4'; k_low[6] = '5'; k_low[7] = '6'; k_low[8] = '7'; k_low[9] = '8'; k_low[10] = '9'; k_low[11] = '0'; k_low[12] = '-'; k_low[13] = '='; k_low[14] = '\b'; k_low[15] = '\t';
    k_low[16] = 'q'; k_low[17] = 'w'; k_low[18] = 'e'; k_low[19] = 'r'; k_low[20] = 't'; k_low[21] = 'z'; k_low[22] = 'u'; k_low[23] = 'i'; k_low[24] = 'o'; k_low[25] = 'p'; k_low[28] = '\n';
    k_low[30] = 'a'; k_low[31] = 's'; k_low[32] = 'd'; k_low[33] = 'f'; k_low[34] = 'g'; k_low[35] = 'h'; k_low[36] = 'j'; k_low[37] = 'k'; k_low[38] = 'l';
    k_low[43] = '/';
    k_low[44] = 'y'; k_low[45] = 'x'; k_low[46] = 'c'; k_low[47] = 'v'; k_low[48] = 'b'; k_low[49] = 'n'; k_low[50] = 'm'; k_low[51] = ','; k_low[52] = '.'; k_low[53] = '-'; k_low[55] = '*'; k_low[57] = ' ';
    k_low[41] = '/';
    k_low[27] = '/';
    k_low[53] = '-';
    k_low[74] = '-';
    k_low[78] = '+';
    if (sc < 128) return k_low[sc]; return 0;
}




struct HBA_PORT {
    uint32_t clb, clbu, os2_fb, fbu, is, ie, cmd, res0, tfd, sig, ssts, sctl, serr, sact, ci, sntf, fbs, res1[11], vendor[4];
};

struct HBA_MEM {
    uint32_t cap, ghc, is, pi, vs, ccc_ctl, ccc_pts, em_loc, em_ctl, cap2, bohc;
    uint8_t res[0xA0 - 0x2C];
    uint8_t vendor[0x100 - 0xA0];
    HBA_PORT ports[32];
};
uint32_t pci_read(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    return inl(0xCFC);
}
void pci_write(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset, uint32_t val) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    outl(0xCFC, val);
}



struct HBA_CMD_HEADER { uint16_t flags; uint16_t prdtl; uint32_t prdbc; uint32_t ctba; uint32_t ctbau; uint32_t res1[4]; };
struct HBA_PRDT_ENTRY {
    uint32_t dba;
    uint32_t dbau;
    uint32_t res1;
    uint32_t dbc;
};
struct HBA_CMD_TBL { uint8_t cfis[64]; uint8_t acmd[16]; uint8_t rsv[48]; HBA_PRDT_ENTRY prdt_entry[1]; };
void ahci_init_port(HBA_PORT* port, int port_no) {
    HBA_MEM* hba = (HBA_MEM*)active_ahci_bar5;
    hba->ghc |= (1 << 31);
    hba->ghc &= ~(1 << 1);
    hba->is = 0xFFFFFFFF;
    port->cmd &= ~1; int w=0; while((port->cmd & (1<<15)) && w++<100000);
    port->cmd &= ~(1<<4); w=0; while((port->cmd & (1<<14)) && w++<100000);
    uint32_t base = 0x00800000 + (port_no * 0x10000);
    for(int i=0; i<0x10000; i++) ((uint8_t*)base)[i] = 0;
    port->clb = base; port->clbu = 0;
    port->os2_fb = base + 0x400; port->fbu = 0;
    HBA_CMD_HEADER* cmdh = (HBA_CMD_HEADER*)port->clb;
    for(int i=0; i<32; i++) {
        cmdh[i].prdtl = 1;
        cmdh[i].ctba = base + 0x1000 + (i * 0x100);
        cmdh[i].ctbau = 0;
    }
    port->ie = 0; port->serr = 0xFFFFFFFF; port->is = 0xFFFFFFFF;
    port->cmd |= (1<<4); port->cmd |= 1;
}
int ahci_rw(uint32_t lba, uint64_t buffer_addr, int is_write) {
    if(active_ahci_bar5 == 0) return 0;
    HBA_PORT* port = &((HBA_MEM*)active_ahci_bar5)->ports[active_sata_port];
    port->is = 0xFFFFFFFF;


    uint64_t clb_full = port->clb | ((uint64_t)port->clbu << 32);
    HBA_CMD_HEADER* cmdheader = (HBA_CMD_HEADER*)clb_full;

    cmdheader[0].flags = 5 | (is_write ? (1 << 6) : 0) | (1 << 16);
    cmdheader[0].prdtl = 1;
    cmdheader[0].prdbc = 0;


    uint64_t ctba_full = cmdheader[0].ctba | ((uint64_t)cmdheader[0].ctbau << 32);
    HBA_CMD_TBL* cmdtbl = (HBA_CMD_TBL*)ctba_full;

    for(int i=0; i<138; i++) ((uint8_t*)cmdtbl)[i] = 0;


    cmdtbl->prdt_entry[0].dba = (uint32_t)(buffer_addr & 0xFFFFFFFF);
    cmdtbl->prdt_entry[0].dbau = (uint32_t)((buffer_addr >> 32) & 0xFFFFFFFF);
    cmdtbl->prdt_entry[0].dbc = 511;
    cmdtbl->prdt_entry[0].res1 = 0;

    uint8_t* fis = (uint8_t*)cmdtbl->cfis;
    fis[0] = 0x27; fis[1] = 0x80; fis[2] = is_write ? 0xCA : 0xC8;
    fis[4] = lba & 0xFF; fis[5] = (lba >> 8) & 0xFF; fis[6] = (lba >> 16) & 0xFF;
    fis[7] = 0x40 | ((lba >> 24) & 0x0F); fis[12] = 1;


    uint32_t spin = 0;
    while((port->tfd & (0x80 | 0x08)) && spin++ < 1000000);
    if(spin >= 1000000) return 0;

    port->ci = 1;

    spin = 0;
    while(1) {
        if((port->ci & 1) == 0) break;
        if(port->is & (1 << 30)) return 0;
        if(spin++ > 1000000) return 0;
    }
    if(port->tfd & 0x01) return 0;
    return 1;
}


int ahci_identify(uint64_t buffer_addr) {
    if(active_ahci_bar5 == 0) return 0;
    HBA_PORT* port = &((HBA_MEM*)active_ahci_bar5)->ports[active_sata_port];
    port->is = 0xFFFFFFFF; port->serr = 0xFFFFFFFF;

    uint32_t cmd_list_addr = port->clb;
    if (cmd_list_addr == 0) cmd_list_addr = 0x400000 + (active_sata_port * 0x10000);
    HBA_CMD_HEADER* cmdheader = (HBA_CMD_HEADER*)cmd_list_addr;
    cmdheader[0].flags = 5; cmdheader[0].prdtl = 1;

    uint32_t ctba = cmdheader[0].ctba;
    if (ctba == 0) ctba = 0x402000 + (active_sata_port * 0x10000);
    cmdheader[0].ctba = ctba;
    HBA_CMD_TBL* cmdtbl = (HBA_CMD_TBL*)ctba;

    for(int i=0; i<80; i++) ((uint8_t*)cmdtbl)[i] = 0;
    cmdtbl->prdt_entry[0].dba = buffer_addr; cmdtbl->prdt_entry[0].dbau = 0;
    cmdtbl->prdt_entry[0].res1 = 0;
    cmdtbl->prdt_entry[0].dbc = 511;
    uint8_t* fis = (uint8_t*)cmdtbl->cfis;
    fis[0] = 0x27; fis[1] = 0x80; fis[2] = 0xEC;



    int spin = 0; while((port->tfd & (0x80 | 0x08)) && spin < 1000000) spin++;
    port->ci = 1;
    int timeout = 100000000;
    while(timeout > 0) {
        if((port->ci & 1) == 0) break;
        if(port->is & (1<<30)) return 0;
        timeout--;
    }



    if(timeout == 0) return 0;
    return 1;
}




__attribute__((aligned(4096))) uint8_t bare_metal_ahci_mem[0x10000];

extern "C" void send_dhcp_discover();
extern char ip_address[32];
extern char net_mask[32];
extern char gateway_ip[32];
char os2_user_name[32] = "COSMOS";
char os2_cpu_brand[49] = "SCANNING CPU...";
uint8_t os2_sys_lang = 0;
uint8_t os2_sys_theme = 0;

char os2_cmd_status[256] = "SYSTEM READY";
uint32_t os2_hda_debug_c0_resp = 0;

char os2_hw_storage[256] = "PRESS TO SCAN";
char os2_hw_net[256] = "PRESS TO SCAN";
char os2_hw_gpu[256] = "PRESS TO SCAN";
char os2_hw_usb[256] = "PRESS TO SCAN";
void os2_get_cpu_brand() {
    uint32_t a = 0, b = 0, c = 0, d = 0;


    ;

    if(a < 0x80000004) { os2_str_cpy(os2_cpu_brand,"GENERIC X86"); return; }

    char* s = os2_cpu_brand;
    for(uint32_t i=0x80000002; i<=0x80000004; i++){

        ;
        *(uint32_t*)s=a; s+=4; *(uint32_t*)s=b; s+=4; *(uint32_t*)s=c; s+=4; *(uint32_t*)s=d; s+=4;
    }
    os2_cpu_brand[48]=0;

    int write_idx = 0; int space_count = 0;
    for(int i=0; i<48; i++) {
        char ch = os2_cpu_brand[i]; if(ch == 0) break;
        if(ch >= 'a' && ch <= 'z') ch -= 32;
        if((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-') {
            os2_cpu_brand[write_idx++] = ch; space_count = 0;
        } else if(ch == ' ' || ch == '@') {
            if(space_count == 0 && write_idx > 0) os2_cpu_brand[write_idx++] = ' ';
            space_count++;
        }
    }
    os2_cpu_brand[write_idx] = 0;
}
void os2_system_reboot() { outb(0x64, 0xFE); }
void os2_system_shutdown() {
    outw(0xB004, 0x2000);
    outw(0x4004, 0x3400);



    while(1) { }

}

void scan_pci_class(uint8_t target_class, char* out_buf, const char* prefix) {

    if (target_class == 0x01) { os2_str_cpy(out_buf, "CTRL [8086:2829]"); return; }
    if (target_class == 0x02) { os2_str_cpy(out_buf, "NIC  [8086:100E]"); return; }
    if (target_class == 0x03) { os2_str_cpy(out_buf, "GPU  [1234:1111]"); return; }
    os2_str_cpy(out_buf, "VIRTUAL DEVICE");
    return;

    for(uint16_t b=0; b<256; b++) {
        for(uint16_t s=0; s<32; s++) {
            for(uint16_t f=0; f<28; f++) {
                uint32_t vd = pci_read(b,s,f,0);
                if((vd & 0xFFFF) != 0xFFFF) {
                    uint32_t cls = pci_read(b,s,f,8);
                    if(((cls >> 24) & 0xFF) == target_class) {
                        os2_str_cpy(out_buf, prefix);
                        int len = os2_str_len(out_buf);
                        out_buf[len++] = ' '; out_buf[len++] = '[';
                        char hex[12]; hex_to_str(vd, hex);
                        for(int i=0; i<10; i++) out_buf[len++] = hex[i];
                        out_buf[len++] = ']'; out_buf[len] = 0;
                        return;
                    }
                }
            }
        }
    }
    os2_str_cpy(out_buf, "NOT FOUND ON PCI BUS");
}
void scan_pci_drives(Window* dsk_win) {

    print_win(dsk_win, "COSMOS LINUX HARDWARE SCANNER\n--------------------------\n");


    ahci_mount_drive();


    for (int i = 0; i < drive_count; i++) {
        char info[128];
        snprintf(info, sizeof(info), "[%s] %s (%d MB)\n",
            drives[i].type == 3 ? "USB" : "SATA",
            drives[i].model,
            drives[i].size_mb);
        print_win(dsk_win, info);
    }

    if (drive_count > 0) {
        selected_drive_idx = 0;
        print_win(dsk_win, "\nDRIVES DETECTED & READY.\n");
    } else {
        print_win(dsk_win, "NO DRIVES FOUND.\n");
    }


    send_dhcp_discover();
    char net_info[128];
    snprintf(net_info, sizeof(net_info), "NETWORK: %s\n", ip_address);
    print_win(dsk_win, net_info);

    return;
# 1419 "kernel_main_linux.cpp"
}
void os2_focus_window(int id) {
    int found_at = -1;
    for(int i=0; i<20; i++) if(os2_win_z[i] == id) found_at = i;
    if(found_at == -1) return;
    for(int i=found_at; i<19; i++) os2_win_z[i] = os2_win_z[i+1];
    os2_win_z[19] = id;
}


int v_cx = 400;
int v_cy = 300;
uint64_t last_click_time = 0;



void os2_handle_input() {
    mouse_just_pressed = false;


    for(int i = 0; i < 32; i++) {
        uint8_t st = inb(0x64);


        if ((st & 1) == 0) break;

        uint8_t d = inb(0x60);


        if (st & 0x20) {
            m_packet[m_ptr] = d;




            if (m_ptr == 0 && !(d & 0x08)) {
                continue;
            }

            m_ptr++;


            if (m_ptr == 3) {
                m_ptr = 0;

                int dx = m_packet[1];
                int dy = m_packet[2];


                if (m_packet[0] & 0x10) dx |= 0xFFFFFF00;
                if (m_packet[0] & 0x20) dy |= 0xFFFFFF00;


                dy = -dy;


                int btn = m_packet[0] & 0x07;


                update_mouse_position(dx, dy, btn);
            }
        } else {

            os2_key_scancode = d;
            key_ready = true;
        }
    }
}
# 1496 "kernel_main_linux.cpp"
struct Vec3 { float x, y, z; };
struct Vec2 { int x, y; };



float bare_sin(float x) {
    return 0.0f;
}

float bare_cos(float x) {
    return 0.0f;
}




Vec3 RotateY(Vec3 p, float angle) {
    float s = bare_sin(angle);
    float c = bare_cos(angle);
    return { p.x * c - p.z * s, p.y, p.x * s + p.z * c };
}

Vec3 RotateX(Vec3 p, float angle) {
    float s = bare_sin(angle);
    float c = bare_cos(angle);
    return { p.x, p.y * c - p.z * s, p.y * s + p.z * c };
}


float camera_fov = 500.0f;
float camera_z_offset = 600.0f;



float global_cam_rot_x = 0.0f;
float global_cam_rot_y = 0.0f;
int last_mouse_x = 400;
int last_mouse_y = 300;


void PutPixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= screen_w || y >= screen_h) return;
    uint8_t* pixel_addr = (uint8_t*)bb + (y * screen_pitch) + (x * 4);
    *(uint32_t*)pixel_addr = color;
}





Vec2 Project3D(Vec3 point, int cx, int cy) {
    float z = point.z + camera_z_offset;


    if (z <= 0.1f) return { -1, -1 };


    float projected_x = (point.x / z) * camera_fov;
    float projected_y = (point.y / z) * camera_fov;


    return { cx + (int)projected_x, cy - (int)projected_y };
}
# 1569 "kernel_main_linux.cpp"
float EdgeFunction(Vec2 a, Vec2 b, int px, int py) {
    return ((float)px - (float)a.x) * ((float)b.y - (float)a.y) - ((float)py - (float)a.y) * ((float)b.x - (float)a.x);
}

void DrawTriangle(Vec2 v0, Vec2 v1, Vec2 v2, uint32_t color) {

    int v0x = (int)v0.x; int v0y = (int)v0.y;
    int v1x = (int)v1.x; int v1y = (int)v1.y;
    int v2x = (int)v2.x; int v2y = (int)v2.y;


    int min_x = v0x; if (v1x < min_x) min_x = v1x; if (v2x < min_x) min_x = v2x;
    int min_y = v0y; if (v1y < min_y) min_y = v1y; if (v2y < min_y) min_y = v2y;
    int max_x = v0x; if (v1x > max_x) max_x = v1x; if (v2x > max_x) max_x = v2x;
    int max_y = v0y; if (v1y > max_y) max_y = v1y; if (v2y > max_y) max_y = v2y;


    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= screen_w) max_x = screen_w - 1;
    if (max_y >= screen_h) max_y = screen_h - 1;


    for (int y = min_y; y <= max_y; y++) {
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + (y * screen_pitch));
        for (int x = min_x; x <= max_x; x++) {

            float w0 = EdgeFunction(v1, v2, x, y);
            float w1 = EdgeFunction(v2, v0, x, y);
            float w2 = EdgeFunction(v0, v1, x, y);


            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                row_ptr[x] = color;
            }
        }
    }
}

void DrawTriangle3D(Vec3 p0, Vec3 p1, Vec3 p2, uint32_t color, int cx, int cy) {
    Vec2 v0 = Project3D(p0, cx, cy);
    Vec2 v1 = Project3D(p1, cx, cy);
    Vec2 v2 = Project3D(p2, cx, cy);
    if (v0.x == -1 || v1.x == -1 || v2.x == -1) return;
    DrawTriangle(v0, v1, v2, color);
}


void DrawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}





void DrawLine3D(Vec3 p1, Vec3 p2, uint32_t color, int cx, int cy) {
    Vec2 v1 = Project3D(p1, cx, cy);
    Vec2 v2 = Project3D(p2, cx, cy);


    if (v1.x == -1 || v2.x == -1) return;


    DrawLine(v1.x, v1.y, v2.x, v2.y, color);
}

int clip_min_x = 0;
int clip_min_y = 0;
int clip_max_x = 4000;
int clip_max_y = 4000;

void os2_Put(int x, int y, uint32_t c) { if(x<clip_min_x || x>=clip_max_x || y<clip_min_y || y>=clip_max_y || x<0 || x>=screen_w || y<0 || y>=screen_h) return; bb[y*screen_w+x]=c; }
void os2_PutAlpha(int x, int y, uint32_t c) { if(x<clip_min_x || x>=clip_max_x || y<clip_min_y || y>=clip_max_y || x<0 || x>=screen_w || y<0 || y>=screen_h) return; uint32_t bg = bb[y*screen_w+x]; uint32_t s1 = ((c & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); bb[y*screen_w+x] = ((s1 & 0xFEFEFE) >> 1) + ((bg & 0xFEFEFE) >> 1); }
void os2_Swap() {




    if (!os2_fb || !bb) return;
    uint64_t* dst = (uint64_t*)os2_fb;
    uint64_t* src = (uint64_t*)bb;


    uint32_t total_blocks = (screen_h * screen_pitch) / 8;

    for (uint32_t i = 0; i < total_blocks; i++) {
        dst[i] = src[i];
    }
}
void os2_DrawRoundedRect(int x, int y, int rw, int rh, int r, uint32_t c) { for(int iy=0;iy<rh;iy++)for(int ix=0;ix<rw;ix++){ bool corn=false; if(ix<r && iy<r && (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=true; if(ix>rw-r && iy<r && (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=true; if(ix<r && iy>rh-r && (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=true; if(ix>rw-r && iy>rh-r && (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=true; if(!corn) os2_Put(x+ix,y+iy,c); } }
void os2_DrawGlassRect(int x, int y, int rw, int rh, int r, uint32_t c) {
    int cr = (c >> 16) & 0xFF; int cg = (c >> 8) & 0xFF; int cb = c & 0xFF;
    for(int iy=0; iy<rh; iy++) {
        for(int ix=0; ix<rw; ix++) {
            bool corn=false;
            if(ix<r && iy<r && (r-ix)*(r-ix)+(r-iy)*(r-iy)>r*r) corn=true;
            if(ix>rw-r && iy<r && (ix-(rw-r))*(ix-(rw-r))+(r-iy)*(r-iy)>r*r) corn=true;
            if(ix<r && iy>rh-r && (r-ix)*(r-ix)+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=true;
            if(ix>rw-r && iy>rh-r && (ix-(rw-r))*(ix-(rw-r))+(iy-(rh-r))*(iy-(rh-r))>r*r) corn=true;
            if(!corn) {
                int sx = x + ix; int sy = y + iy;
                if(sx >= 0 && sx < screen_w && sy >= 0 && sy < screen_h) {
                    uint32_t bg = bb[sy * screen_w + sx];
                    int bg_r = (bg >> 16) & 0xFF; int bg_g = (bg >> 8) & 0xFF; int bg_b = bg & 0xFF;
                    int f_r = (bg_r + cr) >> 1; int f_g = (bg_g + cg) >> 1; int f_b = (bg_b + cb) >> 1;
                    bb[sy * screen_w + sx] = (f_r << 16) | (f_g << 8) | f_b;
                }
            }
        }
    }
}
void os2_DrawChar(int x, int y, char c, uint32_t col, bool bold) {
    if(c >= 'a' && c <= 'z') c -= 32;
    static const uint8_t f_u[] = { 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22, 0x7F,0x41,0x41,0x22,124, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x09,0x01, 0x3E,0x41,0x49,0x49,0x7A, 0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41, 0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x0C,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E, 0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31, 0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x3F,0x40,0x38,0x40,0x3F, 0x63,0x14,0x08,0x14,0x63, 0x07,0x08,0x70,0x08,0x07, 0x61,0x51,0x49,0x45,0x43 };
    static const uint8_t f_n[] = { 0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x42,0x61,0x51,0x49,0x46, 0x21,0x41,0x45,0x4B,0x31, 0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x30, 0x01,0x71,0x09,0x05,0x03, 0x36,0x49,0x49,0x49,0x36, 0x06,0x49,0x49,0x29,0x1E };
    const uint8_t* ptr = 0;
    if(c >= 'A' && c <= 'Z') ptr = &f_u[(c-'A')*5]; else if(c >= '0' && c <= '9') ptr = &f_n[(c-'0')*5];
    else if(c == ':') { static uint8_t s[]={0,0x36,0x36,0,0}; ptr=s; }
    else if(c == '.') { static uint8_t s[]={0,0x60,0x60,0,0}; ptr=s; }
    else if(c == '-') { static uint8_t s[]={0,0x08,0x08,0x08,0}; ptr=s; }
    else if(c == '=') { static uint8_t s[]={0,0x14,0x14,0x14,0}; ptr=s; }
    else if(c == '/') { static uint8_t s[]={0x20,0x10,0x08,0x04,0x02}; ptr=s; }
    else if(c == '<') { static uint8_t s[]={0x08,0x14,0x22,0,0}; ptr=s; }
    else if(c == '>') { static uint8_t s[]={0x22,0x14,0x08,0,0}; ptr=s; }
    else if(c == '"') { static uint8_t s[]={0,0x03,0,0x03,0}; ptr=s; }
    else if(c == '\'') { static uint8_t s[]={0,0x03,0,0,0}; ptr=s; }
    else if(c == '_') { static uint8_t s[]={0x40,0x40,0x40,0x40,0x40}; ptr=s; }
    else if(c == '?') { static uint8_t s[]={0x02,0x51,0x09,0x09,0x06}; ptr=s; }
    else if(c == '&') { static uint8_t s[]={0x3A,0x2C,0x3C,0x12,0x40}; ptr=s; }
    if(!ptr) return;
    uint32_t glow_col = (col < 0x555555) ? 0xFFFFFF : 0x000000;
    for(int m=0;m<5;m++){
        uint8_t l=ptr[m];
        for(int n=0;n<7;n++) {
            if((l>>n)&1) {
                os2_PutAlpha(x+m-1, y+n-1, glow_col); os2_PutAlpha(x+m, y+n-1, glow_col); os2_PutAlpha(x+m+1, y+n-1, glow_col);
                os2_PutAlpha(x+m-1, y+n, glow_col); os2_PutAlpha(x+m+1, y+n, glow_col);
                os2_PutAlpha(x+m-1, y+n+1, glow_col); os2_PutAlpha(x+m, y+n+1, glow_col); os2_PutAlpha(x+m+1, y+n+1, glow_col);
                if(bold) os2_PutAlpha(x+m+2, y+n, glow_col);
            }
        }
    }
    for(int m=0;m<5;m++){ uint8_t l=ptr[m]; for(int n=0;n<7;n++) if((l>>n)&1) { os2_Put(x+m, y+n, col); if(bold) os2_Put(x+m+1, y+n, col); } }
}
void os2_Text(int x, int y, const char* s, uint32_t col, bool bold) {
    if(!s) return; int ox = x;
    while(*s) {
        if(*s == '\n') { y += 15; x = ox; s++; continue; }
        os2_DrawChar(x,y,*s++,col,bold); x+=(bold?7:6);
    }
}
void os2_TextC(int cp, int y, const char* s, uint32_t col, bool bold) { if(!s) return; int l=0; while(s[l])l++; os2_Text(cp-(l*(bold?7:6))/2, y, s, col, bold); }
void os2_DrawAeroCursor(int mx, int my) {
    static const char* c_map[17] = {
        "*", "**", "*.*", "*..*", "*...*", "*....*", "*.....*", "*......*",
        "*.......*", "*........*", "*.........*", "*......****", "*...*..*",
        "*..* *..*", "*.* *..*", "** *..*", "        **"
    };
    for(int y = 0; y < 17; y++) { int len = os2_str_len(c_map[y]); for(int x = 0; x < len; x++) if(c_map[y][x] != ' ') os2_PutAlpha(mx + x + 3, my + y + 4, 0x000000); }
    for(int y = 0; y < 17; y++) { int len = os2_str_len(c_map[y]); for(int x = 0; x < len; x++) { if(c_map[y][x] == '*') os2_Put(mx + x, my + y, 0x000000); else if(c_map[y][x] == '.') os2_Put(mx + x, my + y, 0xFFFFFF); } }
}
void os2_DrawDenseGalaxy(int cx, int cy, int exp) {
    if (exp <= 5) return; int max_radius = (400 * exp) / 320; uint32_t l_seed = 123456;
    for(int i = 0; i < 8000; i++) {
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; int rand_val1 = l_seed % max_radius;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; int rand_val2 = l_seed % 30;
        l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; int rand_val3 = l_seed % 256;
        int d = rand_val1; if(d < 50) continue;
        int scatter_x = (l_seed % 7) - 3; l_seed = (l_seed * 1103515245 + 12345) & 0x7FFFFFFF; int scatter_y = (l_seed % 7) - 3;
        int angle = 0; int intensity_mod = 1;

        if(i % 3 == 0) { int target_d = 60 + ((d / 50) * 50); d = target_d + (rand_val2 - 15); angle = (rand_val3 - os2_frame + 256) % 256; intensity_mod = 2; }

        else { d = (d * d) / max_radius; if(d < 50) d = 50 + (l_seed % 20); angle = (((i % 2) * 128) + (d / 2) - os2_frame + 256) % 256; }
        int final_d = d + (rand_val2 - 15); int final_a = (angle + (l_seed % 10) - 5 + 256) % 256;
        int px = cx + (os2_Cos(final_a) * final_d) / 84 + scatter_x; int py = cy + (os2_Sin(final_a) * final_d * 3 / 4) / 84 + scatter_y;
        if(px < 0 || px >= screen_w || py < 0 || py >= screen_h) continue;
        int r = 0, g = 0, b = 0;
        if(d < 100) { r = 255; g = 180 - d; b = 60; } else if(d < 180) { r = 160 - (d - 100); g = 50; b = 255; } else { r = 20; g = 30; b = 255 - (d - 180); }
        uint32_t bg = bb[py * screen_w + px]; int bg_r = (bg >> 16) & 0xFF; int bg_g = (bg >> 8) & 0xFF; int bg_b = bg & 0xFF;
        int intensity = 255 - (os2_int_sqrt(rand_val2*rand_val2) * 8); if(intensity < 0) intensity = 0; intensity = (intensity * intensity_mod) / 2;
        if(i % 100 == 0) { r = 255; g = 255; b = 255; intensity = 255; }
        int f_r = bg_r + (r * intensity / 256); if(f_r > 255) f_r = 255; int f_g = bg_g + (g * intensity / 256); if(f_g > 255) f_g = 255; int f_b = bg_b + (b * intensity / 256); if(f_b > 255) f_b = 255;
        bb[py * screen_w + px] = (f_r << 16) | (f_g << 8) | f_b;
    }
}
void DrawActiveSun(int cx, int cy, int radius) {
    int r2 = radius * radius;
    int glow_radius = radius + 40;
    int glow_r2 = glow_radius * glow_radius;

    int start_y = -glow_radius; int end_y = glow_radius;
    int start_x = -glow_radius; int end_x = glow_radius;

    if(cy + start_y < 0) start_y = -cy;
    if(cy + end_y >= screen_h) end_y = screen_h - 1 - cy;
    if(cx + start_x < 0) start_x = -cx;
    if(cx + end_x >= screen_w) end_x = screen_w - 1 - cx;

    float time = os2_frame * 0.05f;
    float s_time = bare_sin(time);
    float c_time = bare_cos(time);
    float boil_offset = os2_frame * 0.2f;

    for(int y = start_y; y <= end_y; y++) {
        int screen_y = cy + y;
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + (screen_y * screen_pitch));

        for(int x = start_x; x <= end_x; x++) {
            int dist_sq = x*x + y*y;
            if(dist_sq <= glow_r2) {
                int screen_x = cx + x;
                uint32_t* pixel = &row_ptr[screen_x];

                if(dist_sq <= r2) {



                    int z = os2_int_sqrt(r2 - dist_sq);
                    float rx = x * c_time - z * s_time;
                    float rz = x * s_time + z * c_time;

                    int nz = (z * 255) / radius;
                    int r = nz;
                    int g = (nz * 170) / 255;
                    int b = (nz * 20) / 255;

                    *pixel = (r << 16) | (g << 8) | b;
                } else {



                    int dist = os2_int_sqrt(dist_sq);


                    float px = (float)x; float py = (float)y;
                    float ax = (px > 0) ? px : -px;
                    float ay = (py > 0) ? py : -py;
                    float p_angle = 0;

                    if (ax > ay) p_angle = ay / (ax + 0.0001f);
                    else p_angle = 2.0f - ax / (ay + 0.0001f);

                    if (px < 0 && py >= 0) p_angle = 4.0f - p_angle;
                    else if (px < 0 && py < 0) p_angle = 4.0f + p_angle;
                    else if (px >= 0 && py < 0) p_angle = 8.0f - p_angle;



                    float wave1 = bare_sin(p_angle * 12.0f + time * 3.0f);
                    float wave2 = bare_cos(p_angle * 7.0f - boil_offset);
                    float flare = wave1 * wave2;

                    if (flare < 0) flare = 0;


                    int dynamic_glow = radius + 2 + (int)(flare * 25.0f);


                    if(dynamic_glow <= radius) dynamic_glow = radius + 1;

                    if (dist <= dynamic_glow) {
                        int divisor = dynamic_glow - radius;
                        int alpha = 255 - ((dist - radius) * 255 / divisor);
                        alpha = (alpha * alpha) / 255;

                        if(alpha > 0) {
                            uint32_t bg = *pixel;




                            int final_r = (255 * alpha + ((bg >> 16) & 0xFF) * (255 - alpha)) / 255;
                            int final_g = (120 * alpha + ((bg >> 8) & 0xFF) * (255 - alpha)) / 255;
                            int final_b = ( 0 * alpha + (bg & 0xFF) * (255 - alpha)) / 255;

                            *pixel = (final_r << 16) | (final_g << 8) | final_b;
                        }
                    }
                }
            }
        }
    }
}







struct RenderObj {
    int type;
    Vec3 pos;
    float radius;
    uint32_t color;
};

void DrawPlanet3D(Vec3 pos, float radius, uint32_t color, int cx, int cy) {
    float z = pos.z + camera_z_offset;
    if (z <= 0.1f) return;

    Vec2 center = Project3D(pos, cx, cy);
    if (center.x == -1) return;


    int screen_r = (int)((radius / z) * camera_fov);
    if (screen_r <= 0) return;

    int start_y = -screen_r; int end_y = screen_r;
    int start_x = -screen_r; int end_x = screen_r;

    if (center.y + start_y < 0) start_y = -center.y;
    if (center.y + end_y >= screen_h) end_y = screen_h - 1 - center.y;
    if (center.x + start_x < 0) start_x = -center.x;
    if (center.x + end_x >= screen_w) end_x = screen_w - 1 - center.x;

    int r2 = screen_r * screen_r;
    uint32_t cr = (color >> 16) & 0xFF;
    uint32_t cg = (color >> 8) & 0xFF;
    uint32_t cb = color & 0xFF;

    for (int y = start_y; y <= end_y; y++) {
        uint32_t* row_ptr = (uint32_t*)((uint8_t*)bb + ((center.y + y) * screen_pitch));
        for (int x = start_x; x <= end_x; x++) {
            int dist_sq = x*x + y*y;
            if (dist_sq <= r2) {

                int pz = os2_int_sqrt(r2 - dist_sq);
                float light = (float)pz / (float)screen_r;

                uint32_t final_r = (uint32_t)(cr * light);
                uint32_t final_g = (uint32_t)(cg * light);
                uint32_t final_b = (uint32_t)(cb * light);

                row_ptr[center.x + x] = (final_r << 16) | (final_g << 8) | final_b;
            }
        }
    }
}
void os2_DrawOrganicPlanet(int cx, int cy, int radius, uint32_t base_col) {
    int r2 = radius * radius; int glow_radius = radius + 8; int glow_r2 = glow_radius * glow_radius;
    int base_r = (base_col >> 16) & 0xFF; int base_g = (base_col >> 8) & 0xFF; int base_b = base_col & 0xFF;
    for(int y = -glow_radius; y <= glow_radius; y++) {
        for(int x = -glow_radius; x <= glow_radius; x++) {
            int dist_sq = x*x + y*y; int screen_x = cx + x; int screen_y = cy + y;
            if(screen_x < 0 || screen_x >= screen_w || screen_y < 0 || screen_y >= screen_h) continue;
            if(dist_sq <= r2) {
                int nz = os2_int_sqrt(r2 - dist_sq) * 255 / radius; int edge_dist = 255 - nz;
                int light_x = x + (radius / 2); int light_y = y + (radius / 2); int l_dist_sq = light_x*light_x + light_y*light_y; int diffuse = 0;
                if(l_dist_sq < r2) diffuse = 255 - (os2_int_sqrt(l_dist_sq) * 255 / radius);
                int noise = (((x + radius) * 17) + ((y + radius) * 31)) % 20; int banding = (os2_Sin(((y + radius) * 100) / radius) + 64) / 8;
                int r = (base_r * nz) / 255; int g = (base_g * nz) / 255; int b = (base_b * nz) / 255;
                r += (diffuse * base_r) / 256; g += (diffuse * base_g) / 256; b += (diffuse * base_b) / 256;
                int rim = (edge_dist * edge_dist) / 255; r += (rim * base_r) / 512; g += (rim * base_g) / 512; b += (rim * base_b) / 512;
                r = (r * (220 + noise + banding)) / 256; g = (g * (220 + noise + banding)) / 256; b = (b * (220 + noise + banding)) / 256;
                if(r > 255) r = 255; if(g > 255) g = 255; if(b > 255) b = 255;
                bb[screen_y * screen_w + screen_x] = (r << 16) | (g << 8) | b;
            } else if (dist_sq <= glow_r2 && dist_sq > r2) {
                int alpha = 255 - ((os2_int_sqrt(dist_sq) - radius) * 255 / (glow_radius - radius)); alpha = (alpha * alpha) / 255;
                if(alpha > 0) {
                    uint32_t bg = bb[screen_y * screen_w + screen_x];
                    int final_r = (base_r * alpha + ((bg >> 16) & 0xFF) * (255 - alpha)) / 255; int final_g = (base_g * alpha + ((bg >> 8) & 0xFF) * (255 - alpha)) / 255; int final_b = (base_b * alpha + (bg & 0xFF) * (255 - alpha)) / 255;
                    bb[screen_y * screen_w + screen_x] = (final_r << 16) | (final_g << 8) | final_b;
                }
            }
        }
    }
}

volatile uint32_t app_window_buffer[2500] = {0};
volatile bool os2_app_window_active = false;


volatile uint32_t* app_overlay_buffer = new uint32_t[1920*1080];


unsigned char app_bin[] = {
  0xf3, 0x0f, 0x1e, 0xfa, 0xb8, 0x04, 0x00, 0x00, 0x00, 0xcd, 0x80, 0x48,
  0x85, 0xc0, 0x0f, 0x84, 0x0c, 0x01, 0x00, 0x00, 0x41, 0x57, 0x49, 0x89,
  0xc3, 0x41, 0xba, 0x01, 0x00, 0x00, 0x00, 0xbe, 0x05, 0x00, 0x00, 0x00,
  0x41, 0x56, 0x41, 0xb9, 0x05, 0x00, 0x00, 0x00, 0x45, 0x31, 0xf6, 0x48,
  0x8d, 0x88, 0x10, 0x27, 0x00, 0x00, 0x41, 0x55, 0x49, 0xc7, 0xc7, 0xfe,
  0xff, 0xff, 0xff, 0x41, 0x54, 0x55, 0xbd, 0x01, 0x00, 0x00, 0x00, 0x53,
  0x48, 0x89, 0xea, 0x48, 0x83, 0xec, 0x20, 0x90, 0x48, 0x89, 0xcf, 0x4c,
  0x89, 0xd8, 0x4c, 0x29, 0xdf, 0x83, 0xe7, 0x04, 0x74, 0x12, 0x49, 0x8d,
  0x43, 0x04, 0x41, 0xc7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0xc8,
  0x74, 0x18, 0x66, 0x90, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83,
  0xc0, 0x08, 0xc7, 0x40, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x48, 0x39, 0xc8,
  0x75, 0xea, 0x45, 0x6b, 0xe1, 0x32, 0x48, 0x89, 0x34, 0x24, 0x89, 0xf7,
  0x44, 0x89, 0xcd, 0x4c, 0x89, 0x4c, 0x24, 0x08, 0x44, 0x8d, 0x6e, 0x04,
  0x45, 0x8d, 0x41, 0x04, 0x83, 0xff, 0x31, 0x42, 0x8d, 0x34, 0x27, 0x89,
  0xe8, 0x41, 0x0f, 0x96, 0xc1, 0x83, 0xf8, 0x31, 0x77, 0x12, 0x45, 0x84,
  0xc9, 0x74, 0x0d, 0x48, 0x63, 0xde, 0x49, 0x8d, 0x1c, 0x9b, 0xc7, 0x03,
  0x00, 0xff, 0x00, 0x00, 0x83, 0xc0, 0x01, 0x83, 0xc6, 0x32, 0x44, 0x39,
  0xc0, 0x75, 0xde, 0x83, 0xc7, 0x01, 0x44, 0x39, 0xef, 0x75, 0xc9, 0x48,
  0x8b, 0x34, 0x24, 0x4c, 0x8b, 0x4c, 0x24, 0x08, 0x4c, 0x01, 0xd6, 0x49,
  0x01, 0xd1, 0x48, 0x85, 0xf6, 0x7e, 0x5a, 0x48, 0x83, 0xfe, 0x2e, 0x4d,
  0x0f, 0x4d, 0xd7, 0x4d, 0x85, 0xc9, 0x7e, 0x46, 0x49, 0x83, 0xf9, 0x2e,
  0x49, 0x0f, 0x4d, 0xd7, 0xc7, 0x44, 0x24, 0x1c, 0x00, 0x00, 0x00, 0x00,
  0x8b, 0x44, 0x24, 0x1c, 0x3d, 0x4f, 0xc3, 0x00, 0x00, 0x7e, 0x11, 0x4c,
  0x89, 0xf0, 0xcd, 0x80, 0xe9, 0x33, 0xff, 0xff, 0xff, 0x0f, 0x1f, 0x00,
  0xf3, 0x90, 0xeb, 0xfc, 0xf3, 0x90, 0x8b, 0x44, 0x24, 0x1c, 0x83, 0xc0,
  0x01, 0x89, 0x44, 0x24, 0x1c, 0x8b, 0x44, 0x24, 0x1c, 0x3d, 0x4f, 0xc3,
  0x00, 0x00, 0x7e, 0xe8, 0xeb, 0xd5, 0xba, 0x02, 0x00, 0x00, 0x00, 0xeb,
  0xbb, 0x41, 0xba, 0x02, 0x00, 0x00, 0x00, 0xeb, 0xa6
};
void diagnose_ahci_ports() {
    if (global_ahci_abar == 0) {
        os2_str_cpy(os2_cmd_status, "ERR: ABAR IST NULL");
        return;
    }

    volatile uint32_t* abar = (volatile uint32_t*)global_ahci_abar;



    if ((abar[0] & (1 << 31)) == 0) {
        abar[0] = abar[0] | (1 << 31);
        for(volatile int i=0; i<100000; i++);
    }


    uint32_t pi = abar[3];



    if (pi == 0 || pi == 0xFFFFFFFF) {


        os2_str_cpy(os2_cmd_status, "ERR: MMIO/PAGING BLOCKIERT (PI=0)");
        return;
    }

    int active_ports_count = 0;




    for (int i = 0; i < 32; i++) {
        if (pi & (1 << i)) {
            active_ports_count++;

            volatile uint32_t* port_regs = abar + 64 + (i * 32);
            uint32_t ssts = port_regs[10];
            uint8_t det = ssts & 0x0F;

            if (det == 3) {
                os2_str_cpy(os2_cmd_status, "HDD AUF PORT GEFUNDEN!");
                return;
            }
        }
    }


    if (active_ports_count > 0) {
        os2_str_cpy(os2_cmd_status, "ERR: PORTS DA, ABER HDD FEHLT!");
    } else {
        os2_str_cpy(os2_cmd_status, "ERR: KEINE PORTS VORHANDEN");
    }
}
unsigned int app_bin_len = 333;

__attribute__((section(".text"), aligned(4096))) uint8_t safe_app_buffer[65536] = {0};

bool has_free_task_slot() {
    if (num_tasks < 4) return true;
    for (int i = 1; i < 4; i++) {
        if (!tasks[i].active) return true;
    }
    return false;
}

bool load_and_run_bin(uint32_t start_lba, uint32_t sector_count) {
    if (!has_free_task_slot()) return false;

    uint64_t target_ram = 0x01100000;
    uint8_t* ram = (uint8_t*)target_ram;

    os2_str_cpy(os2_cmd_status, "LADE FLUMMI...");

    for(uint32_t i=0; i < 512 * sector_count; i++) ram[i] = 0;

    for(uint32_t s=0; s < sector_count; s++) {
        if (!disk_read_auto(start_lba + s, target_ram + (s * 512))) {
            os2_str_cpy(os2_cmd_status, "ERR: LESEFEHLER!");
            return false;
        }
        for(volatile int w=0; w<2000; w++) ;
    }





    ;

    disable_nx_for_app(target_ram, 8192);

    ;




    bool valid = false;
    if (ram[0] == 0xF3 && ram[1] == 0x0F && ram[2] == 0x1E && ram[3] == 0xFA) valid = true;
    if (ram[0] == 0x90 && ram[1] == 0x90 && ram[2] == 0x90 && ram[3] == 0x90) valid = true;

    if (valid) {

        create_task((void (*)()) target_ram);
        is_app_running = true;

        os2_str_cpy(os2_cmd_status, "BINGO! FLUMMI LÄUFT!");
        return true;
    }

    os2_str_cpy(os2_cmd_status, "ERR: FALSCHE SIGNATUR AUF DISK!");
    return false;
}




void os2_focus_window(int id);
extern char os2_cpu_brand[49];
void os2_system_reboot();
extern void system_init_usb();
extern "C" uint32_t xhci_bot_get_capacity(uint8_t slot_id);
void toggle_app(int id) {
    Window* win = &os2_windows[id];
    if(win->open && !win->minimized && os2_win_z[19] == win->id) { win->minimized = true; }
    else { win->open = true; win->minimized = false; os2_focus_window(win->id); }
}





extern "C" unsigned char* decode_image(unsigned char* buffer, int len, int* w, int* h, int* comp, int req_comp);
extern "C" void free_image(unsigned char* ptr);
volatile char last_app_key = 0;

extern void play_sound(uint32_t n_freq, int duration);
extern void play_freq(uint32_t f);

extern "C" uint64_t syscall_dispatcher(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    if (sys_num == 0) { yield(); return 0; }
    else if (sys_num == 1) { print_win(&os2_windows[5], (char*)arg1); return 0; }
    else if (sys_num == 2) { uint64_t key = last_app_key; last_app_key = 0; return key; }
    else if (sys_num == 3) { os2_Put(arg1, arg2, arg3); return 0; }




    else if (sys_num == 4) {
        os2_app_window_active = true;
        return (uint64_t)(volatile void*)app_window_buffer;
    }

    else if (sys_num == 5) {
        os2_app_window_active = true;
        return (uint64_t)(volatile void*)app_overlay_buffer;
    }

    else if (sys_num == 6) {
        play_sound(arg1, arg2);
        return 0;
    }

    else if (sys_num == 7) {
        play_freq(arg1);
        return 0;
    }

    return 0;
}

__attribute__((naked)) void syscall_isr() {
    ;
}



uint32_t e1000_mmio_base = 0;

extern "C" void hda_init_controller(uint32_t hda_base);

uint32_t pci_dev_count = 0;
void pci_scan_advanced() {
    pci_dev_count = 0;
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint32_t slot = 0; slot < 32; slot++) {

            for (uint32_t func = 0; func < 8; func++) {

                uint32_t vendor_device = pci_read(bus, slot, func, (uint32_t)0);
                if (vendor_device == 0xFFFFFFFF) {

                    if (func == 0) break;
                    continue;
                }
                pci_dev_count++;

                uint32_t vendor = vendor_device & 0xFFFF;
                uint32_t device = vendor_device >> 16;



                uint32_t nic_class = pci_read(bus, slot, func, (uint32_t)0x08);
                uint8_t nic_cls = (nic_class >> 24) & 0xFF;
                if (vendor == 0x8086 && nic_cls == 0x02 && e1000_mmio_base == 0) {
                    uint32_t bar0 = pci_read(bus, slot, func, (uint32_t)0x10);
                    uint32_t clean_bar0 = bar0 & 0xFFFFFFF0;

                    if ((bar0 & 0x04) != 0) {
                        uint32_t bar1 = pci_read(bus, slot, func, (uint32_t)0x14);
                        if (bar1 != 0) continue;
                    }
                    e1000_mmio_base = clean_bar0;
                    if (e1000_mmio_base != 0) {

                        uint32_t pci_cmd_addr2 = 0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | 0x04;
                        outl(0xCF8, pci_cmd_addr2);
                        uint32_t pci_cmd2 = inl(0xCFC);
                        outl(0xCF8, pci_cmd_addr2);
                        outl(0xCFC, pci_cmd2 | 0x06);
                        map_mmio_64(e1000_mmio_base);
                        intel_e1000_init(e1000_mmio_base, device);
                    }
                }



                uint32_t cls = pci_read(bus, slot, func, 0x08);
                uint8_t base_class = (cls >> 24) & 0xFF;
                uint8_t sub_class = (cls >> 16) & 0xFF;

                if (base_class == 0x04 && sub_class <= 0x03) {
                    extern uint32_t hda_base_addr;
                    if (hda_base_addr != 0) continue;

                    uint32_t bar0 = pci_read(bus, slot, func, 0x10);
                    uint32_t bar_type = bar0 & 0x06;
                    uint32_t mmio = bar0 & 0xFFFFFFF0;


                    if (bar_type == 0x04) {
                        uint32_t bar1 = pci_read(bus, slot, func, 0x14);


                        if (bar1 != 0) continue;
                    }

                    hda_base_addr = mmio;


                    uint32_t cmd = pci_read(bus, slot, func, 0x04);
                    pci_write(bus, slot, func, 0x04, cmd | 0x06);

                    if (hda_base_addr != 0) {

                        map_mmio_64(hda_base_addr);

                        map_mmio_64(0x02200000);
                        hda_init_controller(hda_base_addr);
                    }
                }
            }
        }
    }
}


extern void send_dhcp_discover();

void read_mac_address(char* mac_text_buffer) {

    uint32_t ral = *(volatile uint32_t*)(e1000_mmio_base + 0x5400);
    uint32_t rah = *(volatile uint32_t*)(e1000_mmio_base + 0x5404);


    uint8_t mac[6];
    mac[0] = ral & 0xFF;
    mac[1] = (ral >> 8) & 0xFF;
    mac[2] = (ral >> 16) & 0xFF;
    mac[3] = (ral >> 24) & 0xFF;
    mac[4] = rah & 0xFF;
    mac[5] = (rah >> 8) & 0xFF;


    int pos = 0;
    for (int i = 0; i < 6; i++) {
        char hex_byte[3];
        os2_byte_to_hex(mac[i], hex_byte);

        mac_text_buffer[pos++] = hex_byte[0];
        mac_text_buffer[pos++] = hex_byte[1];

        if (i < 5) mac_text_buffer[pos++] = ':';
    }
    mac_text_buffer[pos] = 0;
}
void dynamic_task_worker();

void sleep_ms(uint32_t ms);
char cmd_input_buf[64];
int cmd_input_idx = 0;
void process_cmd(char* input, Window* cmd_win) {

    print_win(cmd_win, "C:\\> "); print_win(cmd_win, input); print_win(cmd_win, "\n");
    if(os2_str_equal(input, "CLS")) {
        cmd_win->cursor_pos = 0; cmd_win->content[0] = 0;
    }
    else if(os2_str_starts(input, "ECHO ")) {
        print_win(cmd_win, input + 5); print_win(cmd_win, "\n");
    }
    else if(os2_str_equal(input, "DIR")) {
        print_win(cmd_win, "--- CFS DIRECTORY ---\n");
        int count = 0;
        for(int i=0; i<28; i++) {
            if(cfs_files[i].exists) {
                print_win(cmd_win, cfs_files[i].name);
                print_win(cmd_win, "   [FILE]\n");
                count++;
            }
        }
        if(count == 0) print_win(cmd_win, "NO FILES FOUND (MOUNT DRIVE FIRST)\n");
    }
    else if(os2_str_equal(input, "SYSINFO")) {
        print_win(cmd_win, "OS: COSMOS V2 (32-BIT PROTECTED MODE)\nCPU: ");
        print_win(cmd_win, os2_cpu_brand); print_win(cmd_win, "\n");
    }



    else if(os2_str_equal(input, "MEM")) {
        print_win(cmd_win, "RAM  : 8192 MB TOTAL\n");
        print_win(cmd_win, "USED : 14 MB (KERNEL)\n");
        print_win(cmd_win, "FREE : 8178 MB AVAILABLE\n");
    }



    else if(os2_str_equal(input, "PART")) {
        print_win(cmd_win, "SCANNING ALL SATA PORTS (0-5)...\n");
        int total_found = 0;


        for (uint8_t port = 0; port < 6; port++) {



            scan_partitions(port);


            if (gpt_partition_count > 0) {
                print_win(cmd_win, "--------------------------------\n");
                print_win(cmd_win, "DRIVE FOUND ON SATA PORT ");
                char portStr[4]; int_to_str(port, portStr); print_win(cmd_win, portStr);
                print_win(cmd_win, "\n");


                for (int p = 0; p < gpt_partition_count; p++) {
                    print_win(cmd_win, "-> PART ");
                    char numStr[4]; int_to_str(p + 1, numStr); print_win(cmd_win, numStr);
                    print_win(cmd_win, " | START LBA: ");


                    char lbaStr[15];
                    os2_byte_to_hex((gpt_partition_starts[p] >> 24) & 0xFF, lbaStr);
                    os2_byte_to_hex((gpt_partition_starts[p] >> 16) & 0xFF, lbaStr + 2);
                    os2_byte_to_hex((gpt_partition_starts[p] >> 8) & 0xFF, lbaStr + 4);
                    os2_byte_to_hex(gpt_partition_starts[p] & 0xFF, lbaStr + 6);
                    lbaStr[8] = 0;

                    print_win(cmd_win, lbaStr);
                    print_win(cmd_win, "\n");
                    total_found++;
                }
            }
        }

        if (total_found == 0) {
            print_win(cmd_win, "ERR: NO PARTITIONS FOUND ON ANY PORT.\n");
        } else {
            print_win(cmd_win, "--------------------------------\n");
        }
    }



    else if(os2_str_equal(input, "NET")) {
        if (os2_net_bar0 == 0) {
            print_win(cmd_win, "SCANNING PCI BUS...\n");
            os2_smart_scan();
        }

        if (os2_net_bar0 == 0) {
            print_win(cmd_win, "ERR  : NO NETWORK CONTROLLER FOUND!\n");
        } else {
            char v_str[5], d_str[5];
            os2_byte_to_hex(os2_net_vendor >> 8, v_str); os2_byte_to_hex(os2_net_vendor & 0xFF, v_str + 2); v_str[4] = 0;
            os2_byte_to_hex(os2_net_device >> 8, d_str); os2_byte_to_hex(os2_net_device & 0xFF, d_str + 2); d_str[4] = 0;

            print_win(cmd_win, "NIC  : ID ");
            print_win(cmd_win, v_str); print_win(cmd_win, "-"); print_win(cmd_win, d_str);
            print_win(cmd_win, "\n");

            if (os2_net_vendor == 0x8086 && os2_net_device == 0x100E) {
                print_win(cmd_win, "TYPE : INTEL E1000 (LEGACY)\n");

                char real_mac[20];
                read_mac_address(real_mac);

                print_win(cmd_win, "MAC  : ");
                print_win(cmd_win, real_mac);
                print_win(cmd_win, "\n");
            }
            else if (os2_net_vendor == 0x10EC) {
                print_win(cmd_win, "TYPE : REALTEK CHIPSET\n");
            }
            else if (os2_net_vendor == 0x8086) {
                print_win(cmd_win, "TYPE : MODERN INTEL CHIPSET\n");
            }
            else {
                print_win(cmd_win, "TYPE : UNKNOWN ADAPTER\n");
            }


            print_win(cmd_win, "STAT : WAITING FOR LINK...\n");
            net_check_link();
            send_dhcp_discover();
            print_win(cmd_win, "STAT : DHCP DISCOVER SENT!\n");
        }
    }



    else if(os2_str_equal(input, "DHCP")) {
        if (os2_net_bar0 != 0) {
            print_win(cmd_win, "SENDING DHCP DISCOVER BROADCAST...\n");
            print_win(cmd_win, "WAITING FOR DHCP OFFER...\n");
        } else {
            print_win(cmd_win, "ERR: RUN 'NET' COMMAND FIRST\n");
        }
    }



    else if(os2_str_equal(input, "IP")) {
        print_win(cmd_win, "INTERFACE : ETH0\n");
        print_win(cmd_win, "IPv4 ADDR : "); print_win(cmd_win, ip_address); print_win(cmd_win, "\n");
        print_win(cmd_win, "SUBNET    : "); print_win(cmd_win, net_mask); print_win(cmd_win, "\n");
        print_win(cmd_win, "GATEWAY   : "); print_win(cmd_win, gateway_ip); print_win(cmd_win, "\n");
        print_win(cmd_win, "DNS       : 8.8.8.8\n");
    }



    else if(os2_str_equal(input, "HELP")) {
        print_win(cmd_win, "AVAILABLE COMMANDS:\n");
        print_win(cmd_win, " SYSINFO, MEM, NET, IP, DIR, CLS\n");
        print_win(cmd_win, " MKDIR [NAME], ECHO [TEXT]\n");
        print_win(cmd_win, " START, RUNAPP, REBOOT\n");
    }
    else if(os2_str_equal(input, "HELP")) {
        print_win(cmd_win, "AVAILABLE COMMANDS:\n");
        print_win(cmd_win, " SYSINFO, MEM, NET, DIR, CLS\n");
        print_win(cmd_win, " MKDIR [NAME], ECHO [TEXT]\n");
        print_win(cmd_win, " START, RUNAPP, REBOOT\n");
    }
    else if(os2_str_starts(input, "MKDIR ")) {
        print_win(cmd_win, "SPINNING UP SATA DRIVE...\n");

        uint64_t buf_dir = (uint64_t)global_buf_dir;


        ahci_read_sectors(1002, buf_dir);

        sleep_ms(50);

        CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
        bool found = false;

        for (int i = 0; i < 8; i++) {
            if (dir[i].type == 0) {
                dir[i].type = 2;
                dir[i].file_size = 0;
                dir[i].start_lba = 0;

                for(int n=0; n<11; n++) dir[i].filename[n] = 0;
                char* new_name = input + 6;

                for(int n=0; n<10 && new_name[n] != 0 && new_name[n] != '\n' && new_name[n] != '\r'; n++) {
                    dir[i].filename[n] = new_name[n];
                }


                ahci_write_sectors(1002, buf_dir);

                sleep_ms(50);

                print_win(cmd_win, "HDD WRITE OK: ");
                print_win(cmd_win, dir[i].filename);
                print_win(cmd_win, "\n");

                found = true;
                break;
            }
        }
        if (!found) print_win(cmd_win, "ERROR: SATA ROOT DIR FULL!\n");
    }



    else if(os2_str_equal(input, "START")) {
        if (has_free_task_slot()) {
            create_task(dynamic_task_worker);
            print_win(cmd_win, "BACKGROUND TASK SPAWNED.\n");
        } else {
            print_win(cmd_win, "ERROR: TASK LIMIT REACHED (4/4).\n");
        }
    }



    else if(os2_str_starts(input, "INSTALL ") || os2_str_starts(input, "GET ")) {
        bool is_install = os2_str_starts(input, "INSTALL ");
        if (is_install && !has_free_task_slot()) {
            print_win(cmd_win, "ERROR: TASK LIMIT REACHED (4/4).\n");
        } else {
            char url[512];
            char* in_ptr = input + (is_install ? 8 : 4);

            bool has_slash = false;
            for(int i=0; in_ptr[i] != 0; i++) {
                if (in_ptr[i] == '/') has_slash = true;
            }

            if (has_slash) {
                os2_str_cpy(url, in_ptr);
            } else {
                os2_str_cpy(url, "10.0.2.2/");
                os2_str_cat(url, in_ptr);
                os2_str_cat(url, ".PKG");
            }

            print_win(cmd_win, "RESOLVING ");
            print_win(cmd_win, url);
            print_win(cmd_win, "...\n");

            pkg_download_active = 1;
            pkg_download_execute = is_install;
            browser_download_complete = 0;
            browser_tcp.state = 0;
            browser_download_len = 0;
            os2_str_cpy(browser_url, url);
            pkg_output_win_ptr = (void*)cmd_win;
            if (os2_str_starts(url, "10.0.2.2")) { pkg_target_port = 8000; } else { pkg_target_port = 80; }

            char query_dom[64]; int _qidx = 0;
            int start_idx = 0;
            if(os2_str_starts(browser_url, "HTTP://") || os2_str_starts(browser_url, "http://")) start_idx = 7;
            else if(os2_str_starts(browser_url, "HTTPS://") || os2_str_starts(browser_url, "https://")) start_idx = 8;
            for(int k=start_idx; k<63+start_idx; k++) { if(browser_url[k] == 0 || browser_url[k] == '/') break; query_dom[_qidx++] = browser_url[k]; }
            query_dom[_qidx] = 0;
            uint32_t target_ip = ip_str_to_u32(query_dom);
            pkg_target_ip = target_ip;

            if(target_ip == 0) {
                send_dns_query(query_dom);
            } else {
                if (os2_net_bar0 == 0) {
                    print_win(cmd_win, "AUTO-INIT NETWORK...\n");
                    os2_smart_scan();
                    net_check_link();
                    send_dhcp_discover();


                    for(volatile int wait = 0; wait < 10000000; wait++) ;
                }
                send_tcp_syn(target_ip, pkg_target_port);
            }
        }
    }



    else if(os2_str_equal(input, "RUNAPP")) {

        bool success = load_and_run_bin(10000, 10);
        if (success) {
            print_win(cmd_win, "LOADING APP FROM DISK...\n");
        } else {
            print_win(cmd_win, "ERROR: FAILED TO LOAD APP OR TASK LIMIT REACHED.\n");
        }
    }

    else if(os2_str_equal(input, "REBOOT")) {
        os2_system_reboot();
    }
    else if(input[0] != 0) {
        print_win(cmd_win, "UNKNOWN COMMAND OR BAD SYNTAX.\n");
    }
}
void sleep_ms(uint32_t ms) {
    uint64_t target_ticks = os2_system_ticks + ms;

    while (os2_system_ticks < target_ticks) {



        ;
    }
}
void background_task() {
    while(1) {

        volatile int x = 0;
        x++;


        ;
    }
}


extern uint32_t global_ahci_abar;

void bare_metal_port_init(int port_no) {
    if (port_no < 0) return;
    volatile uint32_t* abar = (volatile uint32_t*)global_ahci_abar;
    volatile uint32_t* port_regs = abar + 64 + (port_no * 32);


    port_regs[6] &= ~0x00000001;
    port_regs[6] &= ~0x00000010;


    int timeout = 1000000;
    while ((port_regs[6] & (1 << 14)) && timeout--) { ; }
    timeout = 1000000;
    while ((port_regs[6] & (1 << 15)) && timeout--) { ; }

    os2_str_cpy(os2_cmd_status, "PORT REBASE (64-BIT RAM)!");


    uint64_t base_addr = (uint64_t)&bare_metal_ahci_mem[0];


    volatile char* mem = (volatile char*)base_addr;
    for (int i = 0; i < 0x10000; i++) mem[i] = 0;


    uint64_t clb = base_addr;
    uint64_t os2_fb = base_addr + 0x1000;
    uint64_t ctb = base_addr + 0x2000;


    port_regs[0] = (uint32_t)(clb & 0xFFFFFFFF);
    port_regs[1] = (uint32_t)(clb >> 32);

    port_regs[2] = (uint32_t)(os2_fb & 0xFFFFFFFF);
    port_regs[3] = (uint32_t)(os2_fb >> 32);

    volatile uint32_t* cmd_list = (volatile uint32_t*)clb;
    cmd_list[0] = (1 << 16) | 5;
    cmd_list[1] = 0;
    cmd_list[2] = (uint32_t)(ctb & 0xFFFFFFFF);
    cmd_list[3] = (uint32_t)(ctb >> 32);


    port_regs[12] = 0xFFFFFFFF;
    port_regs[4] = 0xFFFFFFFF;


    timeout = 1000000;
    while (((port_regs[8] & 0x88) != 0) && timeout--) { ; }

    port_regs[6] |= 0x00000010;
    port_regs[6] |= 0x00000001;
}




extern "C" void kernel_save_download(unsigned long long source_ram, int file_size, const char* name) {

    int free_slot = -1;
    for (int i = 0; i < 28; i++) {
        if (cfs_files[i].exists == 0) {
            free_slot = i;
            break;
        }
    }

    if (free_slot == -1) return;


    uint32_t next_lba = 10000;
    for (int i = 0; i < 28; i++) {
        if (cfs_files[i].exists && cfs_files[i].start_lba >= next_lba) {
            next_lba = cfs_files[i].start_lba + ((cfs_files[i].size + 511) / 512) + 1;
        }
    }


    uint32_t sectors = (file_size + 511) / 512;
    for (uint32_t s = 0; s < sectors; s++) {
        disk_write_auto(next_lba + s, source_ram + (s * 512));

        for(volatile int w = 0; w < 2000; w++) ;
    }


    cfs_files[free_slot].exists = 1;
    cfs_files[free_slot].start_lba = next_lba;
    cfs_files[free_slot].size = file_size;
    cfs_files[free_slot].is_folder = 0;
    cfs_files[free_slot].parent_idx = 255;


    for (int i = 0; i < 63; i++) {
        cfs_files[free_slot].name[i] = name[i];
        if (name[i] == 0) break;
    }
    cfs_files[free_slot].name[63] = 0;
}




extern "C" void kernel_save_browser_download() {
 extern uint8_t browser_download_buffer[];
 extern int browser_download_len;
 extern char browser_url[];


 uint32_t payload_start = 0;
 for(uint32_t i=0; i<browser_download_len-4; i++) {
  if (browser_download_buffer[i] == '\r' && browser_download_buffer[i+1] == '\n' &&
   browser_download_buffer[i+2] == '\r' && browser_download_buffer[i+3] == '\n') {
   payload_start = i + 4;
   break;
  }
 }
 if (payload_start == 0 || payload_start >= browser_download_len) return;

 uint32_t file_size = browser_download_len - payload_start;


 uint32_t next_lba = 11000;
 int free_slot = -1;
 for (int i = 0; i < 28; i++) {
  if (cfs_files[i].exists && cfs_files[i].start_lba >= next_lba) {
   next_lba = cfs_files[i].start_lba + ((cfs_files[i].size + 511) / 512) + 1;
  }
  if (cfs_files[i].exists == 0 && free_slot == -1) {
   free_slot = i;
  }
 }
 if (free_slot == -1) return;


 uint32_t sectors = (file_size + 511) / 512;
 for (uint32_t s=0; s<sectors; s++) {
  uint64_t ram_addr = (uint64_t)&browser_download_buffer[payload_start + s*512];
  disk_write_auto(next_lba + s, ram_addr);
  for(volatile int w=0; w<2000; w++) ;
 }


 uint64_t buf_dir = (uint64_t)global_buf_dir;
 ahci_read_sectors(1002, buf_dir);
 for(volatile int w=0; w<500000; w++) ;

 CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
 dir[free_slot].type = 1;
 dir[free_slot].file_size = file_size;
 dir[free_slot].start_lba = next_lba;
 dir[free_slot].parent_idx = 255;


 for(int n=0; n<11; n++) { dir[free_slot].filename[n] = 0; cfs_files[free_slot].name[n] = 0; }
 int slash_idx = 0;
 for (int k=0; k<100; k++) {
  if (browser_url[k] == '/') slash_idx = k + 1;
  if (browser_url[k] == 0) break;
 }
 os2_str_cpy(dir[free_slot].filename, &browser_url[slash_idx]);
 os2_str_cpy(cfs_files[free_slot].name, &browser_url[slash_idx]);

 ahci_write_sectors(1002, (uint64_t)buf_dir);
 for(volatile int w=0; w<500000; w++) ;


 cfs_files[free_slot].exists = 1;
 cfs_files[free_slot].is_folder = 0;
 cfs_files[free_slot].size = file_size;
 cfs_files[free_slot].start_lba = next_lba;
 cfs_files[free_slot].parent_idx = 255;
}



extern "C" void open_browser_save_as() {
    os2_windows[2].open = true;
    os2_windows[2].minimized = false;
    os2_focus_window(2);
    os2_save_step = 0;
    save_as_mode = 1;
    os2_str_cpy(os2_save_filename, "DL_APP.BIN");
    os2_save_name_idx = 10;
}




struct __attribute__((packed)) TBAHeader {
    char magic[4];
    uint32_t fileCount;
    uint32_t indexOffset;
    uint16_t mandalaSeed;
    uint16_t purityScore;
};

struct __attribute__((packed)) TBAFileEntry {
    char name[512];
    uint64_t offset;
    uint64_t originalSize;
    uint64_t compressedSize;
    uint8_t isCompressed;
    uint8_t isPureStream;
    uint8_t padding[6];
};

struct TBA_BitState {
    uint64_t acc;
    int nB;
    int srcIdx;
    uint8_t* src;
    int src_len;
};

__attribute__((optimize("O0")))
uint64_t TBA_ReadBits(TBA_BitState* s, int count) {
    while (s->nB < count && s->srcIdx < s->src_len) {
        s->acc |= ((uint64_t)s->src[s->srcIdx++] << s->nB);
        s->nB += 8;
    }
    uint64_t res = s->acc & (((uint64_t)1 << count) - 1);
    s->acc >>= count;
    s->nB -= count;
    return res;
}

__attribute__((optimize("O0")))
void decompress_tba(uint8_t* src, int src_len, uint8_t* dst, int originalSize) {
    TBA_BitState s = {0, 0, 0, src, src_len};
    int dstIdx = 0;

    while (dstIdx < originalSize && s.srcIdx <= s.src_len) {
        if(TBA_ReadBits(&s, 1)) {
            uint64_t len = TBA_ReadBits(&s, 8);
            uint64_t dist = TBA_ReadBits(&s, 15);
            if(dstIdx >= dist && dist > 0) {
                int start = dstIdx - dist;
                for(int k = 0; k < len && dstIdx < originalSize; k++) {
                    dst[dstIdx] = dst[start + k];
                    dstIdx++;
                }
            } else {
                for(int k = 0; k < len && dstIdx < originalSize; k++) {
                    dst[dstIdx++] = 0;
                }
            }
        } else {
            if(dstIdx < originalSize) {
                dst[dstIdx++] = (uint8_t)TBA_ReadBits(&s, 8);
            }
        }
    }
    while (dstIdx < originalSize) dst[dstIdx++] = 0;
}


extern bool sound_muted;
extern void play_sound(uint32_t n_freq, int duration);
extern void play_freq(uint32_t f);

extern "C" uint8_t __bss_start[];
extern "C" uint8_t __bss_end[];

extern "C" void kernel_main64(BootInfo* boot_info) {
# 2889 "kernel_main_linux.cpp"
 screen_w = boot_info->screen_width;
    screen_h = boot_info->screen_height;
    screen_pitch = boot_info->framebuffer_pitch;





    if (!bb) bb = new uint32_t[1920 * 1080];

    cartridge_start = boot_info->cartridge_start;
    cartridge_end = boot_info->cartridge_end;

    if (screen_pitch == 0) {
        screen_pitch = screen_w * 4;
    }
# 2933 "kernel_main_linux.cpp"
    tasks[0].active = true;
# 2947 "kernel_main_linux.cpp"
    os2_read_rtc();
    os2_get_cpu_brand();
    usb_mouse_callback = update_mouse_position;
    os2_init_mouse();


    for(int i=0; i<200; i++) {
        os2_stars[i].x = (os2_cosmos_random() % 1599) - 799;
        os2_stars[i].y = (os2_cosmos_random() % 1199) - 599;
        os2_stars[i].z = (os2_cosmos_random() % 1000) + 1;
        os2_stars[i].type = os2_cosmos_random() % 1000;

        os2_stars[i].speed = (os2_cosmos_random() % 6) + 4;
    }



    for(int i=0; i<20; i++) {
        os2_windows[i].id = i;
        os2_windows[i].open = false;
        os2_windows[i].minimized = false;
        os2_windows[i].cursor_pos = 0;
        mem_set((uint8_t*)os2_windows[i].content, 0, 2048);
    }
    mem_set((uint8_t*)cmd_input_buf, 0, 64);
    cmd_input_idx = 0;

    os2_str_cpy(os2_windows[0].title, "NOTEPAD"); os2_windows[0].x=100; os2_windows[0].y=100; os2_windows[0].w=400; os2_windows[0].h=300; os2_windows[0].color=0xEEEEEE;
    os2_str_cpy(os2_windows[1].title, "APPS"); os2_windows[1].x=150; os2_windows[1].y=150; os2_windows[1].w=350; os2_windows[1].h=250; os2_windows[1].color=0xDDDDDD;
    os2_str_cpy(os2_windows[2].title, "SAVE AS..."); os2_windows[2].x=200; os2_windows[2].y=150; os2_windows[2].w=300; os2_windows[2].h=200; os2_windows[2].color=0xDDDDDD;
    os2_str_cpy(os2_windows[3].title, "SYSTEM"); os2_windows[3].x=200; os2_windows[3].y=50; os2_windows[3].w=350; os2_windows[3].h=480; os2_windows[3].color=0xFFD700;
    os2_str_cpy(os2_windows[4].title, "DISK MGR"); os2_windows[4].x=250; os2_windows[4].y=200; os2_windows[4].w=450; os2_windows[4].h=350; os2_windows[4].color=0x888888;
    os2_str_cpy(os2_windows[5].title, "CMD"); os2_windows[5].x=100; os2_windows[5].y=300; os2_windows[5].w=450; os2_windows[5].h=250; os2_windows[5].color=0x111111;
    os2_str_cpy(os2_windows[6].title, "EMULATOR"); os2_windows[6].x=150; os2_windows[6].y=100; os2_windows[6].w=500; os2_windows[6].h=400; os2_windows[6].color=0x222222;
    os2_str_cpy(os2_windows[7].title, "DEBUG"); os2_windows[7].x=200; os2_windows[7].y=150; os2_windows[7].w=300; os2_windows[7].h=300; os2_windows[7].color=0x004488;
    os2_str_cpy(os2_windows[8].title, "ACCESS"); os2_windows[8].x=300; os2_windows[8].y=100; os2_windows[8].w=250; os2_windows[8].h=300; os2_windows[8].color=0x44AAAA;
    os2_str_cpy(os2_windows[9].title, "IMAGE VIEWER"); os2_windows[9].x=50; os2_windows[9].y=50; os2_windows[9].w=600; os2_windows[9].h=450; os2_windows[9].color=0x111111;
    os2_str_cpy(os2_windows[10].title, "GERAETE MGR"); os2_windows[10].x=100; os2_windows[10].y=80; os2_windows[10].w=600; os2_windows[10].h=450; os2_windows[10].color=0x111111;
    os2_str_cpy(os2_windows[11].title, "ARCADE: PONG"); os2_windows[11].x=200; os2_windows[11].y=150; os2_windows[11].w=450; os2_windows[11].h=300; os2_windows[11].color=0x112211;
    os2_str_cpy(os2_windows[12].title, "ARCADE: BLOBBY"); os2_windows[12].x=250; os2_windows[12].y=100; os2_windows[12].w=450; os2_windows[12].h=300; os2_windows[12].color=0x4488FF;
 os2_str_cpy(os2_windows[14].title, "APP-BIN"); os2_windows[14].x=250; os2_windows[14].y=100; os2_windows[14].w=450; os2_windows[14].h=300; os2_windows[14].color=0x4488FF;


 scan_pci_drives(&os2_windows[4]);


    mem_set(os2_hw_storage, 0, 256);
    mem_set(os2_hw_net, 0, 256);
    mem_set(os2_hw_gpu, 0, 256);
# 3004 "kernel_main_linux.cpp"
 check_incoming();


    os2_hw_storage[35] = 0;
    os2_hw_net[35] = 0;
    os2_hw_gpu[35] = 0;
    os2_hw_usb[35] = 0;

    int clock_dirs[] = {213, 0, 42, 85, 128};
    for(int i=0; i<5; i++) {
        os2_planets[i].ang = clock_dirs[i]; os2_planets[i].dist = 10;
        os2_planets[i].cur_x = 400; os2_planets[i].cur_y = 300;
        os2_str_cpy(os2_planets[i].name, (i==0?(char*)"TXT":i==1?(char*)"APP":i==2?(char*)"SYS":i==3?(char*)"DSK":(char*)"CMD"));
    }
    int map_ids[]={0,1,3,4,5};
    while(1) {




        extern unsigned char linux_scancode;
        extern uint32_t linux_mx, linux_my;
        extern bool linux_mdown;

        mouse_x = linux_mx;
        mouse_y = linux_my;
        if (linux_mdown) {
            if (!mouse_down) { mouse_just_pressed = true; mouse_down = true; }
            else { mouse_just_pressed = false; }
        } else {
            mouse_down = false;
            mouse_just_pressed = false;
        }

        if (linux_scancode != 0) {
            unsigned char d = linux_scancode;
            linux_scancode = 0;
            os2_key_scancode = d;
            key_ready = true;
        }

        if(os2_input_cooldown > 0) os2_input_cooldown--;
        click_consumed = false; os2_z_blocked = false; bool mouse_handled = false;




        if((last_app_key == 'd' || last_app_key == 'D') && os2_input_cooldown == 0) {
            send_dhcp_discover();
            last_app_key = 0;
            os2_input_cooldown = 25;
        }

        if(os2_frame % 10 == 0) check_incoming();




        static int pkg_last_download_len = 0;
        static int pkg_idle_frames = 0;
        static int pkg_syn_retry_cooldown = 0;

        if (pkg_download_active) {
            if (browser_tcp.state == 0) {
                pkg_syn_retry_cooldown++;
                if (pkg_syn_retry_cooldown > 60) {
                    pkg_syn_retry_cooldown = 0;
                    Window* p_win = pkg_output_win_ptr ? (Window*)pkg_output_win_ptr : &os2_windows[5];
                    print_win(p_win, "DEBUG: SENDING TCP SYN...\n");
                    if (pkg_target_ip != 0) {
                        send_tcp_syn(pkg_target_ip, pkg_target_port);
                    } else if (browser_tcp.remote_ip != 0) {
                        send_tcp_syn(browser_tcp.remote_ip, pkg_target_port);
                    }
                }
            } else if (browser_download_complete == 0 && browser_download_len > 0) {
                if (browser_download_len != pkg_last_download_len) {
                    if ((browser_download_len / 1024) != (pkg_last_download_len / 1024)) {
                        Window* p_win = pkg_output_win_ptr ? (Window*)pkg_output_win_ptr : &os2_windows[5];
                        print_win(p_win, "DOWNLOADING: ");
                        char tmp[20];
                        int_to_str(browser_download_len / 1024, tmp);
                        print_win(p_win, tmp);
                        print_win(p_win, " KB");
                        if (browser_content_length > 0) {
                            print_win(p_win, " / ");
                            char tmp2[20];
                            int_to_str(browser_content_length / 1024, tmp2);
                            print_win(p_win, tmp2);
                            print_win(p_win, " KB (");
                            char tmp3[10];
                            int_to_str((browser_download_len * 100) / browser_content_length, tmp3);
                            print_win(p_win, tmp3);
                            print_win(p_win, "%)");
                        }
                        print_win(p_win, "...\n");
                    }
                    pkg_last_download_len = browser_download_len;
                    pkg_idle_frames = 0;
                } else {
                    pkg_idle_frames++;
                    if (pkg_idle_frames > 300) {
                        browser_download_complete = 1;
                    }
                }
            } else {
                pkg_idle_frames++;
                if (pkg_idle_frames > 300) {
                    browser_download_complete = 1;
                }
            }
        }

        if (pkg_download_active && browser_download_complete) {
            pkg_download_active = 0;
            Window* c_win = pkg_output_win_ptr ? (Window*)pkg_output_win_ptr : &os2_windows[5];

            if (browser_download_len > 0) {
                print_win(c_win, "DOWNLOAD COMPLETE. SAVING TO DISK...\n");
            } else {
                print_win(c_win, "DOWNLOAD FAILED: CONNECTION REFUSED!\n");
            }

            uint32_t payload_start = 0;
            for(uint32_t i=0; i<browser_download_len-4; i++) {
                if (browser_download_buffer[i] == '\r' && browser_download_buffer[i+1] == '\n' &&
                    browser_download_buffer[i+2] == '\r' && browser_download_buffer[i+3] == '\n') {
                    payload_start = i + 4;
                    break;
                }
                if (browser_download_buffer[i] == '\n' && browser_download_buffer[i+1] == '\n') {
                    payload_start = i + 2;
                    break;
                }
            }

            if (payload_start > 0 && payload_start < browser_download_len) {

                uint32_t next_lba = 11000;
                int free_slot = -1;
                for (int i = 0; i < 28; i++) {
                    if (cfs_files[i].exists && cfs_files[i].start_lba >= next_lba) {
                        next_lba = cfs_files[i].start_lba + ((cfs_files[i].size + 511) / 512) + 1;
                    }
                    if (cfs_files[i].exists == 0 && free_slot == -1) {
                        free_slot = i;
                    }
                }

                if (free_slot != -1) {
                    uint64_t buf_dir = (uint64_t)global_buf_dir;
                    ahci_read_sectors(1002, buf_dir);
                    sleep_ms(20);

                    CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)(unsigned long long)buf_dir;
                    dir[free_slot].type = 1;
                    dir[free_slot].file_size = browser_download_len - payload_start;
                    dir[free_slot].start_lba = next_lba;

                    for(int n=0; n<11; n++) { dir[free_slot].filename[n] = 0; cfs_files[free_slot].name[n] = 0; }

                    int slash_idx = 0;
                    for (int k=0; k<100; k++) {
                        if (browser_url[k] == '/') slash_idx = k + 1;
                        if (browser_url[k] == 0) break;
                    }
                    os2_str_cpy(dir[free_slot].filename, &browser_url[slash_idx]);
                    os2_str_cpy(cfs_files[free_slot].name, &browser_url[slash_idx]);

                    cfs_files[free_slot].start_lba = next_lba;
                    cfs_files[free_slot].size = dir[free_slot].file_size;
                    cfs_files[free_slot].is_folder = false;
                    cfs_files[free_slot].exists = 1;

                    ahci_write_sectors(1002, (uint64_t)buf_dir);
                    sleep_ms(20);

                    uint32_t payload_len = browser_download_len - payload_start;
                    uint32_t sectors = (payload_len + 511) / 512;
                    for (uint32_t s=0; s<sectors; s++) {
                        uint64_t ram_addr = (uint64_t)&browser_download_buffer[payload_start + s*512];
                        ahci_write_sectors(next_lba + s, ram_addr);
                    }
                    print_win(c_win, "SAVED TO VIRTUAL HDD!\n");
                    if (pkg_download_execute) {
                        print_win(c_win, "EXECUTING ELF...\n");
                        uint64_t entry_point = parse_and_load_elf(&browser_download_buffer[payload_start]);
                        if (entry_point != 0) {
                            create_task((void (*)()) entry_point);
                            print_win(c_win, "BINGO! ELF IS RUNNING!\n");
                        } else {
                            print_win(c_win, "ERROR: INVALID ELF FILE.\n");
                        }
                    } else {
                        print_win(c_win, "DOWNLOAD SUCCESSFUL!\n");
                    }
                } else {
                    print_win(c_win, "ERROR: DISK FULL (NO SLOTS).\n");
                }
            } else {
                print_win(c_win, "ERROR: HTTP PAYLOAD NOT FOUND.\n");
            }
        }



        if (key_ready) {
            key_ready = false;
            uint8_t sc = os2_key_scancode;

            if(!(sc & 0x80)) {


                last_app_key = get_ascii_qwertz(sc);
                key_new = true;




                char dbg_str[30] = "KEY: ";
                char h_str[5];
                os2_byte_to_hex(sc, h_str);
                os2_str_cat(dbg_str, h_str);
                os2_str_cpy(os2_cmd_status, dbg_str);


                if(sc == 0x3B) toggle_app(0);
                if(sc == 0x3C) toggle_app(1);
                if(sc == 0x3D) toggle_app(3);
                if(sc == 0x3E) toggle_app(4);
                if(sc == 0x3F) toggle_app(5);
    if(sc == 0x40) toggle_app(6);
                if(sc == 0x44) toggle_app(7);


                int fw_id = os2_win_z[19];
                Window* fw = &os2_windows[fw_id];

                if(fw && fw->open && !fw->minimized) {

                    if(fw->id == 3) {
                        if(sc == 0x50) {
                            if (sys_selected_item < 4) sys_selected_item++;
                        }

                        if(sc == 0x48) {
                            if (sys_selected_item > 0) sys_selected_item--;
                        }

                        if(sc == 0x1C) {


                            if (sys_selected_item == 0) {
                                os2_mirror_count = 0;
                                for(uint32_t b=0; b<256; b++) {
                                    for(uint32_t d=0; d<32; d++) {
                                        for(uint32_t f=0; f<28; f++) {
                                            uint32_t id = pci_read(b, d, f, 0);
                                            if((id & 0xFFFF) != 0xFFFF && id != 0 && os2_mirror_count < 30) {
                                                os2_mirror_list[os2_mirror_count].bus = b;
                                                os2_mirror_list[os2_mirror_count].dev = d;
                                                os2_mirror_list[os2_mirror_count].func = f;
                                                os2_mirror_list[os2_mirror_count].vendor = id & 0xFFFF;
                                                os2_mirror_list[os2_mirror_count].device = id >> 16;

                                                uint32_t bar0_l = pci_read(b, d, f, 0x10);
                                                uint32_t bar0_h = pci_read(b, d, f, 0x14);

                                                uint32_t class_rev = pci_read(b, d, f, 0x08);
                                                uint32_t cls = (class_rev >> 24) & 0xFF;
                                                uint32_t sub = (class_rev >> 16) & 0xFF;


                                                if((bar0_l & 0x06) == 0x04) {
                                                    os2_mirror_list[os2_mirror_count].bar0 = ((uint64_t)bar0_h << 32) | (bar0_l & 0xFFFFFFF0);
                                                } else {
                                                    os2_mirror_list[os2_mirror_count].bar0 = bar0_l & 0xFFFFFFF0;
                                                }

                                                if(cls == 0x0C && sub == 0x03) os2_str_cpy(os2_mirror_list[os2_mirror_count].name, "USB 3.0 (xHCI)");
                                                else if(cls == 0x01 && sub == 0x06) os2_str_cpy(os2_mirror_list[os2_mirror_count].name, "SATA (AHCI)");
                                                else if(cls == 0x02) os2_str_cpy(os2_mirror_list[os2_mirror_count].name, "NETWORK");
                                                else if(cls == 0x03) os2_str_cpy(os2_mirror_list[os2_mirror_count].name, "GRAPHICS");
                                                else os2_str_cpy(os2_mirror_list[os2_mirror_count].name, "SYSTEM DEVICE");

                                                os2_mirror_count++;
                                            }
                                        }
                                    }
                                }

                                show_oracle = true;
                            }


                            else if (sys_selected_item == 1) { scan_pci_class(0x01, os2_hw_storage, "CTRL"); }
                            else if (sys_selected_item == 2) { scan_pci_class(0x02, os2_hw_net, "NIC"); }
                            else if (sys_selected_item == 3) { scan_pci_class(0x03, os2_hw_gpu, "GPU"); }


                            else if (sys_selected_item == 4) {
                                system_init_usb();
                            }
                        }
                    }

                    if(fw->id == 0) {
                        if(sc == 0x0E) { if(fw->cursor_pos > 0) { fw->cursor_pos--; fw->content[fw->cursor_pos] = 0; } }
                        else if(sc == 0x1C) { if(fw->cursor_pos < 2000) { fw->content[fw->cursor_pos++] = '\n'; fw->content[fw->cursor_pos] = 0; } }
                        else {
                            char c = get_ascii_qwertz(sc);
                            if(c >= 'a' && c <= 'z') c -= 32;
                            if(c && fw->cursor_pos < 2000) { fw->content[fw->cursor_pos++] = c; fw->content[fw->cursor_pos] = 0; }
                        }
                    }


                    if(fw->id == 5) {
                        if(sc == 0x0E) { if(cmd_input_idx > 0) { cmd_input_idx--; cmd_input_buf[cmd_input_idx] = 0; } }
                        else if(sc == 0x1C) { process_cmd(cmd_input_buf, fw); cmd_input_idx = 0; cmd_input_buf[0] = 0; }
                        else {
                            char c = get_ascii_qwertz(sc);
                            if(c >= 'a' && c <= 'z') c -= 32;
                            if(c && cmd_input_idx < 60) { cmd_input_buf[cmd_input_idx++] = c; cmd_input_buf[cmd_input_idx] = 0; }
                        }
                    }
                }

                if(fw->id == 2) {
                    if(sc == 0x0E) {
                        if(os2_save_step == 1 && os2_save_name_idx > 0) os2_save_filename[--os2_save_name_idx] = 0;
                        else if(os2_save_step == 2 && os2_folder_name_idx > 0) os2_new_folder_name[--os2_folder_name_idx] = 0;
                    }
                    else {
                        char c = get_ascii_qwertz(sc);
                        if(c >= 'a' && c <= 'z') c -= 32;
                        if(c >= 32) {
                            if(os2_save_step == 1 && os2_save_name_idx < 10) {
                                os2_save_filename[os2_save_name_idx++] = c;
                                os2_save_filename[os2_save_name_idx] = 0;
                            }
                            else if(os2_save_step == 2 && os2_folder_name_idx < 10) {
                                os2_new_folder_name[os2_folder_name_idx++] = c;
                                os2_new_folder_name[os2_folder_name_idx] = 0;
                            }
                        }
                    }
                }
            }
        }
        if(os2_frame % 100 == 0) {
            os2_read_rtc();
        }



        if(mouse_down) {
             if(os2_drag_win != -1) {
                 os2_windows[os2_drag_win].x = mouse_x - os2_drag_off_x;
                 os2_windows[os2_drag_win].y = mouse_y - os2_drag_off_y;
                 mouse_handled=true; click_consumed=true;
             }
             else if(os2_resize_win != -1) {
                 int nw = mouse_x - os2_windows[os2_resize_win].x;
                 int nh = mouse_y - os2_windows[os2_resize_win].y;
                 if(nw > 100) os2_windows[os2_resize_win].w = nw;
                 if(nh > 100) os2_windows[os2_resize_win].h = nh;
                 mouse_handled=true; click_consumed=true;
             }
        } else { os2_drag_win = -1; os2_resize_win = -1; }

        if(!mouse_handled) {
            for(int i=19; i>=0; i--) {
                int k = os2_win_z[i]; Window* win=&os2_windows[k];
                if(win->open && !win->minimized) {
                    int wx=(win->fullscreen?0:win->x); int wy=(win->fullscreen?0:win->y); int ww=(win->fullscreen?screen_w:win->w); int wh=(win->fullscreen?screen_h:win->h);
                    if(mouse_x>=wx && mouse_x<=wx+ww && mouse_y>=wy && mouse_y<=wy+wh) {
                        os2_z_blocked = true;
                        if(mouse_just_pressed) {
                           click_consumed = true;
                           os2_focus_window(k);
                           int bx = wx + ww/2;


                           if(mouse_y < wy+40) {


                               if(mouse_x > bx-70 && mouse_x < bx-30) {
                                   if((os2_system_ticks - last_window_click) > 250) {
                                       win->minimized=true; last_window_click = os2_system_ticks;
                                   }
                               }

                               else if(mouse_x > bx-20 && mouse_x < bx+40) {
                                   if((os2_system_ticks - last_window_click) > 250) {
                                       win->fullscreen = !win->fullscreen; last_window_click = os2_system_ticks;
                                   }
                               }

                               else if(mouse_x > bx+45 && mouse_x < bx+70) {
                                   if((os2_system_ticks - last_window_click) > 250) {
                                       win->open=false; last_window_click = os2_system_ticks;
                                   }
                               }

                               else {
                                   os2_drag_win = k; os2_drag_off_x = mouse_x - wx; os2_drag_off_y = mouse_y - wy;
                               }
                           }


                           if(mouse_x > wx+ww-20 && mouse_y > wy+wh-20) { os2_resize_win = k; }
                        }
                        mouse_handled = true; break;
                    }
                }
            }
        }



        for(int i = 0; i < screen_w*screen_h; i++) bb[i] = 0x000000;
        for(int i=0; i<200; i++) {
            int t = os2_stars[i].type;
            if(t >= 995) os2_stars[i].z -= 1; else if(t >= 980) os2_stars[i].z -= os2_stars[i].speed; else if(t >= 950) os2_stars[i].z -= (os2_stars[i].speed + 4); else os2_stars[i].z -= os2_stars[i].speed;
            if(os2_stars[i].z <= 0) {
                os2_stars[i].z = 1000; os2_stars[i].x = (os2_cosmos_random() % 1599) - 799; os2_stars[i].y = (os2_cosmos_random() % 1199) - 599;
                os2_stars[i].type = os2_cosmos_random() % 1000; os2_stars[i].speed = (os2_cosmos_random() % 10) + 8;
            }
            int sx = v_cx + (os2_stars[i].x * 256) / os2_stars[i].z; int sy = v_cy + (os2_stars[i].y * 256) / os2_stars[i].z;
            if(sx >= 0 && sx < screen_w && sy >= 0 && sy < screen_h) {
                if(t < 900) {
                    int intensity = 255 - (os2_stars[i].z / 4); if(intensity < 0) intensity = 0; if(intensity > 255) intensity = 255;
                    uint32_t col = (intensity << 16) | (intensity << 8) | (intensity); os2_Put(sx, sy, col);
                } else if(t < 950) {
                    int pulse = (os2_Sin((os2_frame * 5) + i) + 256) / 2; if(pulse > 255) pulse = 255;
                    uint32_t col = (pulse << 16) | (pulse << 8) | 255; os2_Put(sx, sy, col);
                } else if(t < 980) {
                    os2_Put(sx, sy, 0xFFFFFF);
                    int tail1_x = v_cx + (os2_stars[i].x * 256) / (os2_stars[i].z + 20); int tail1_y = v_cy + (os2_stars[i].y * 256) / (os2_stars[i].z + 20); os2_Put(tail1_x, tail1_y, 0xFF8800);
                    int tail2_x = v_cx + (os2_stars[i].x * 256) / (os2_stars[i].z + 40); int tail2_y = v_cy + (os2_stars[i].y * 256) / (os2_stars[i].z + 40); os2_Put(tail2_x, tail2_y, 0xAA0000);
                } else if(t < 995) {
                    int r = 3000 / os2_stars[i].z;
                    if(r > 0 && r < 40) {
                        int r2 = r*r; int focus_x = (sx - v_cx) * r / 400; int focus_y = (sy - v_cy) * r / 300;
                        for(int cy_a=-r; cy_a<=r; cy_a++) for(int cx_a=-r; cx_a<=r; cx_a++) if(cx_a*cx_a+cy_a*cy_a <= r2) {
                            int pos_x = cx_a + r; int pos_y = cy_a + r; int noise = ((pos_x * 17 + pos_y * 31) % 40);
                            int lx = cx_a + focus_x; int ly = cy_a + focus_y; int l_dist = os2_int_sqrt(lx*lx + ly*ly);
                            int diffuse = 30; if(l_dist < r) diffuse += 225 - (l_dist * 225 / r);
                            int gray = ((50 + noise) * diffuse) / 256; if(gray > 255) gray = 255;
                            os2_Put(sx+cx_a, sy+cy_a, (gray<<16)|((gray*9)/10<<8)|((gray*8)/10));
                        }
                    }
                } else {
                    int r = 6000 / os2_stars[i].z;
                    if(r > 0 && r < 60) {
                        for(int j=0; j<30; j++) {
                            int ang1 = (j * 15 + (os2_frame/3)) % 256; int dist = (j * r) / 30;
                            int gx1 = sx + (os2_Cos(ang1)*dist)/84; int gy1 = sy + (os2_Sin(ang1)*dist*3/4)/84; os2_PutAlpha(gx1, gy1, 0xAA22AA);
                            int ang2 = (ang1 + 128) % 256; int gx2 = sx + (os2_Cos(ang2)*dist)/84; int gy2 = sy + (os2_Sin(ang2)*dist*3/4)/84; os2_PutAlpha(gx2, gy2, 0x2288AA);
                        }
                        os2_Put(sx, sy, 0xFFFFFF);
                    }
                }
            }
        }

        if(!os2_z_blocked && mouse_just_pressed && !click_consumed && os2_is_over(mouse_x, mouse_y, v_cx, v_cy, 50)) {
            os2_galaxy_open = !os2_galaxy_open; click_consumed = true;
        }
        if(os2_galaxy_open && os2_galaxy_expansion < 320) os2_galaxy_expansion += 24;
        if(!os2_galaxy_open && os2_galaxy_expansion > 0) os2_galaxy_expansion -= 30;




        for(int i=0; i<5; i++) {
            Window* win = &os2_windows[map_ids[i]];
            if(os2_galaxy_expansion > 10 || win->minimized || win->open) {
                int px = os2_planets[i].cur_x; int py = os2_planets[i].cur_y;
                bool hov = os2_is_over(mouse_x, mouse_y, px, py, 20);
                uint32_t p_col = 0x888888;
                if(i == 0) p_col = 0xA05566; if(i == 1) p_col = 0x44AA88;
                if(i == 2) p_col = 0x6677CC; if(i == 3) p_col = 0xCC9955;
                if(i == 4) p_col = 0x8899AA;
                if(hov && !os2_z_blocked) {
                    int hr = ((p_col >> 16) & 0xFF) + 40; if(hr>255) hr=255;
                    int hg = ((p_col >> 8) & 0xFF) + 40; if(hg>255) hg=255;
                    int hb = (p_col & 0xFF) + 40; if(hb>255) hb=255;
                    os2_DrawOrganicPlanet(px, py, 22, (hr<<16)|(hg<<8)|hb);
                } else {
                    os2_DrawOrganicPlanet(px, py, 20, p_col);
                }
                if(win->open) {
                    os2_DrawOrganicPlanet(px-30, py, 5, 0x8899AA);
                    os2_DrawOrganicPlanet(px+30, py, 5, 0x8899AA);
                }
                os2_TextC(px, py-4, os2_planets[i].name, 0xFFFFFF, true);
                if(!os2_z_blocked && mouse_just_pressed && !click_consumed && hov) {
                    if(win->minimized) win->minimized = false;
                    else { win->open = true; os2_focus_window(win->id); }
                    click_consumed = true;
                }
            }
        }

        os2_DrawDenseGalaxy(v_cx, v_cy, os2_galaxy_expansion);





        float t_mer = os2_frame * 0.041f;
        float t_ven = os2_frame * 0.016f;
        float t_ear = os2_frame * 0.010f;
        float t_mar = os2_frame * 0.005f;
        float t_jup = os2_frame * 0.001f;
        float t_sat = os2_frame * 0.0005f;
        float t_ura = os2_frame * 0.0002f;
        float t_nep = os2_frame * 0.0001f;


        Vec3 p1_mer = { bare_cos(t_mer) * 140.0f, 5.0f * bare_sin(t_mer), bare_sin(t_mer) * 140.0f };
        Vec3 p2_ven = { bare_cos(t_ven) * 180.0f, -3.0f * bare_sin(t_ven), bare_sin(t_ven) * 180.0f };
        Vec3 p3_ear = { bare_cos(t_ear) * 230.0f, 0.0f, bare_sin(t_ear) * 230.0f };
        Vec3 p4_mar = { bare_cos(t_mar) * 280.0f, 8.0f * bare_sin(t_mar), bare_sin(t_mar) * 280.0f };
        Vec3 p5_jup = { bare_cos(t_jup) * 400.0f, -10.0f* bare_sin(t_jup), bare_sin(t_jup) * 400.0f };
        Vec3 p6_sat = { bare_cos(t_sat) * 520.0f, 15.0f* bare_sin(t_sat), bare_sin(t_sat) * 520.0f };
        Vec3 p7_ura = { bare_cos(t_ura) * 640.0f, -5.0f * bare_sin(t_ura), bare_sin(t_ura) * 640.0f };
        Vec3 p8_nep = { bare_cos(t_nep) * 750.0f, 2.0f * bare_sin(t_nep), bare_sin(t_nep) * 750.0f };


        RenderObj system[9] = {
            { 0, {0, 0, 0}, 80, 0 },
            { 1, p1_mer, 3, 0xAAAAAA },
            { 1, p2_ven, 7, 0xFFDD88 },
            { 1, p3_ear, 8, 0x0088FF },
            { 1, p4_mar, 5, 0xFF4400 },
            { 1, p5_jup, 24, 0xDDAA77 },
            { 1, p6_sat, 20, 0xEEDD99 },
            { 1, p7_ura, 12, 0x66CCFF },
            { 1, p8_nep, 11, 0x2244AA }
        };






        bool right_mouse_held = os2_mouse_right_down;


        if (right_mouse_held) {
            global_cam_rot_y += (mouse_x - last_mouse_x) * -0.005f;
            global_cam_rot_x += (mouse_y - last_mouse_y) * -0.005f;
        }

        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;


        float sy = bare_sin(global_cam_rot_y); float cy = bare_cos(global_cam_rot_y);
        float sx = bare_sin(global_cam_rot_x); float cx = bare_cos(global_cam_rot_x);


        for (int i = 0; i < 9; i++) {
            Vec3 p = system[i].pos;
            float nx = p.x * cy - p.z * sy;
            float nz = p.x * sy + p.z * cy;
            p.x = nx; p.z = nz;

            float ny = p.y * cx - p.z * sx;
            nz = p.y * sx + p.z * cx;
            p.y = ny; p.z = nz;

            system[i].pos = p;
        }



        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 8; j++) {
                if (system[j].pos.z < system[j+1].pos.z) {
                    RenderObj temp = system[j];
                    system[j] = system[j+1];
                    system[j+1] = temp;
                }
            }
        }


        for (int i = 0; i < 9; i++) {
            if (system[i].type == 0) {
                Vec2 s_2d = Project3D(system[i].pos, v_cx, v_cy);
                if (s_2d.x != -1) {
                    DrawActiveSun(s_2d.x, s_2d.y, system[i].radius);
                }
            } else {
                DrawPlanet3D(system[i].pos, system[i].radius, system[i].color, v_cx, v_cy);
            }
        }



        os2_TextC(v_cx, v_cy-15, "COSMOS", 0x000000, true);
        os2_TextC(v_cx, v_cy+5, "SYSTEM", 0x000000, true);
        char ts[]="00:00";
        ts[0]='0'+os2_rtc_h/10; ts[1]='0'+os2_rtc_h%10;
        ts[3]='0'+os2_rtc_m/10; ts[4]='0'+os2_rtc_m%10;
        os2_TextC(v_cx, v_cy+20, ts, 0x000000, true);
        char ds[]="00.00.2000";
        ds[0]='0'+os2_rtc_day/10; ds[1]='0'+os2_rtc_day%10;
        ds[3]='0'+os2_rtc_mon/10; ds[4]='0'+os2_rtc_mon%10;
        ds[8]='0'+(os2_rtc_year%100)/10; ds[9]='0'+os2_rtc_year%10;
        os2_TextC(v_cx, v_cy+35, ds, 0x000000, true);



        for(int i=0; i<5; i++) {
            Window* win = &os2_windows[map_ids[i]];
            int target_x, target_y;
            if(win->minimized) {
                target_x = 250 + (i * 70); target_y = 560;
            } else if(win->open) {
                int orbit_dist = 120 + i*60;
                target_x = v_cx + (os2_Cos(os2_planets[i].ang) * orbit_dist) / 84;
                target_y = v_cy + (os2_Sin(os2_planets[i].ang) * orbit_dist * 3/4) / 84;
            } else {
                if(os2_galaxy_expansion >= 100) {
                    if(os2_planets[i].dist < 120 + i*60) os2_planets[i].dist += 6;
                    if(os2_planets[i].dist > 50) {
                         if(os2_frame % 3 == 0) os2_planets[i].ang = (os2_planets[i].ang + 1) % 256;
                    }
                } else {
                    if(os2_planets[i].dist > 10) os2_planets[i].dist -= 15;
                }
                target_x = v_cx + (os2_Cos(os2_planets[i].ang) * os2_planets[i].dist) / 84;
                target_y = v_cy + (os2_Sin(os2_planets[i].ang) * os2_planets[i].dist * 3/4) / 84;
            }
            os2_planets[i].cur_x += (target_x - os2_planets[i].cur_x) / 2;
            os2_planets[i].cur_y += (target_y - os2_planets[i].cur_y) / 2;
        }





        bool is_modal_blocked = (os2_windows[2].open && !os2_windows[2].minimized);
        for(int i=0; i<20; i++) {
            int k = os2_win_z[i];
            Window* win = &os2_windows[k];
            if(!win->open || win->minimized) continue;
            int wx=(win->fullscreen?0:win->x);
            int wy=(win->fullscreen?0:win->y);
            int ww=(win->fullscreen?screen_w:win->w);
            int wh=(win->fullscreen?screen_h:win->h);



            bool blocked = (is_modal_blocked && k != 2);

            os2_DrawGlassRect(wx, wy, ww, wh, 12, win->color);

            os2_DrawRoundedRect(wx+12, wy, ww-24, 1, 0, 0x999999);
            os2_DrawRoundedRect(wx, wy+12, 1, wh-24, 0, 0x999999);
            uint32_t txt_color = (win->color > 0x888888) ? 0x000000 : 0xFFFFFF;
            os2_Text(wx+15, wy+15, win->title, txt_color, true);

            int bx = wx + ww/2;
            os2_Text(bx-60, wy+15, "MIN", 0x555555, true);
            os2_Text(bx-10, wy+15, "FULL", 0x555555, true);
            os2_Text(bx+50, wy+15, "X", 0x000000, true);



            if(win->id == 8) {
                run_browser_engine(wx, wy, ww, wh, blocked);
            }



            if(win->id == 9) {
                if(win->content[0] == 'B' && win->content[1] == 'M' && win->content[2] == 'P') {
                    uint8_t* bmp = (uint8_t*)0x0C000000;
                    if(bmp[0] == 'B' && bmp[1] == 'M') {
                        int pixel_offset = bmp[10] | (bmp[11]<<8) | (bmp[12]<<16) | (bmp[13]<<24);
                        int img_w = bmp[18] | (bmp[19]<<8) | (bmp[20]<<16) | (bmp[21]<<24);
                        int img_h = bmp[22] | (bmp[23]<<8) | (bmp[24]<<16) | (bmp[25]<<24);
                        uint16_t bpp = bmp[28] | (bmp[29]<<8);

                        if(bpp == 24 && img_w > 0 && img_h > 0 && img_w < 1920 && img_h < 1080) {
                            int row_padded = (img_w * 3 + 3) & (~3);
                            int start_x = wx + 10;
                            int start_y = wy + 40;

                            for(int y = 0; y < img_h; y++) {
                                int draw_y = start_y + (img_h - 1 - y);
                                if(draw_y > wy + wh - 10) continue;

                                int row_offset = y * row_padded;
                                for(int x = 0; x < img_w; x++) {
                                    int draw_x = start_x + x;
                                    if(draw_x > wx + ww - 10) break;

                                    int p_idx = pixel_offset + row_offset + (x * 3);
                                    uint32_t r = bmp[p_idx + 2];
                                    uint32_t g = bmp[p_idx + 1];
                                    uint32_t b = bmp[p_idx];
                                    uint32_t col = (r << 16) | (g << 8) | b;
                                    os2_Put(draw_x, draw_y, col);
                                }
                            }
                        } else {
                            os2_TextC(wx + ww/2, wy + wh/2, "UNSUPPORTED BMP (24-BIT ONLY)", 0xFF0000, true);
                        }
                    } else {
                        os2_TextC(wx + ww/2, wy + wh/2, "INVALID BMP FILE", 0xFF0000, true);
                    }
                }
            }



            if(win->id == 11) {
                run_pong_engine(wx, wy, ww, wh, blocked);
            }



            if(win->id == 12) {
                run_blobby_engine(wx, wy, ww, wh, blocked);
            }



            if(win->id == 10) {
                run_holyspirit(wx, wy, ww, wh, blocked);
            }



            if(win->id == 14) {
                run_smash_cats_engine(wx, wy, ww, wh, blocked);

                if (os2_app_window_active) {
                    uint32_t win_start_x = wx + (ww - 50) / 2;
                    uint32_t win_start_y = wy + 40 + (wh - 40 - 50) / 2;
                    for (int y = 0; y < 50; y++) {
                        for (int x = 0; x < 50; x++) {
                            uint32_t pixel_color = app_window_buffer[y * 50 + x];
                            if (pixel_color != 0) {
                                os2_Put(win_start_x + x, win_start_y + y, pixel_color);
                            }
                        }
                    }
                }
            }



            if(win->id == 6) {
                run_emulator_engine(wx, wy, ww, wh, blocked);
            }

            if(win->id == 3) {
                int mid = wx + ww/2;
                int btn_y = wy + 45;


                bool is_active = (os2_win_z[19] == win->id);


                char lang_lbl[20], theme_lbl[30];
                if(os2_sys_lang == 0) os2_str_cpy(lang_lbl, "[ LANG: EN ]"); else os2_str_cpy(lang_lbl, "[ SPR: DE ]");
                if(os2_sys_lang == 0) { if(os2_sys_theme == 0) os2_str_cpy(theme_lbl, "[ THEME: COMPUTER ]"); else os2_str_cpy(theme_lbl, "[ THEME: GENESIS ]"); }
                else { if(os2_sys_theme == 0) os2_str_cpy(theme_lbl, "[ THEMA: COMPUTER ]"); else os2_str_cpy(theme_lbl, "[ THEMA: GENESIS ]"); }

                int slider_x = wx + 20;
                int slider_y = wy + 140;
                int slider_w = 150;


                os2_DrawRoundedRect(slider_x, slider_y, slider_w, 8, 4, 0x444444);



                int knob_x = slider_x + ((mouse_sens - 10) * slider_w / 290);
                os2_DrawRoundedRect(knob_x - 5, slider_y - 2, 10, 12, 4, 0xFFFFFF);


                os2_Text(wx + 180, slider_y - 2, "MOUSE SENSITIVITY:", 0xAAAAAA, true);




                bool mouse_klick_slider = (mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, slider_x, slider_y, slider_w, 20));

                if(os2_input_cooldown == 0 && mouse_klick_slider) {

                    int rel_x = mouse_x - slider_x;


                    mouse_sens = (rel_x * 290 / slider_w) + 10;


                    if(mouse_sens < 10) mouse_sens = 10;
                    if(mouse_sens > 300) mouse_sens = 300;

                    os2_input_cooldown = 10;
                }
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+5, btn_y, 140, 20)) { os2_sys_lang = !os2_sys_lang; os2_input_cooldown = 25; }
                os2_Text(wx+10, btn_y+4, lang_lbl, 0x000000, true);

                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+5, btn_y+30, 200, 20)) { os2_sys_theme = !os2_sys_theme; os2_input_cooldown = 25; }
                os2_Text(wx+10, btn_y+34, theme_lbl, 0x000000, true);


                os2_DrawRoundedRect(wx+20, wy+120, ww-40, 2, 0, 0xAAAAAA);


                os2_TextC(mid, wy+140, os2_user_name, 0x222222, true);
                os2_TextC(mid, wy+160, os2_cpu_brand, 0x0000FF, true);

                int right_x = wx + ww - 160;

                os2_Text(right_x, wy+140, ip_address, 0x222222, true);

                os2_Text(right_x, wy+155, net_mask, 0x555555, false);

                os2_Text(right_x, wy+170, gateway_ip, 0x555555, false);


                os2_TextC(mid, wy+200, "HARDWARE STATUS", 0x000000, true);
                os2_Text(wx+30, wy+230, "CORE:", 0x555555, true); os2_Text(wx+130, wy+230, "64-BIT PROTECTED MODE", 0x00AA00, true);
                os2_Text(wx+30, wy+250, "MEM:", 0x555555, true); os2_Text(wx+130, wy+250, "4 GB ADDRESS SPACE", 0x00AA00, true);

                char l_disk[20] = "STORAGE:"; char l_net[20] = "NETWORK:"; char l_gpu[20] = "GRAPHIC:"; char l_usb[20] = "USB HOST:";
                if(os2_sys_lang != 0) { os2_str_cpy(l_disk, "FESTPLATTE:"); os2_str_cpy(l_net, "NETZWERK:"); os2_str_cpy(l_gpu, "GRAFIK:"); }


                uint32_t c_st_lbl = 0x555555, c_st_val = 0x0044CC;
                uint32_t c_nt_lbl = 0x555555, c_nt_val = 0x0044CC;
                uint32_t c_gp_lbl = 0x555555, c_gp_val = 0x0044CC;
                uint32_t c_us_lbl = 0x555555, c_us_val = 0x0044CC;
                uint32_t btn_color = 0x444444;


                if (sys_selected_item == 0) { btn_color = 0xAA0000; }
                if (sys_selected_item == 1) { c_st_lbl = 0xFF0000; c_st_val = 0xFF0000; }
                if (sys_selected_item == 2) { c_nt_lbl = 0xFF0000; c_nt_val = 0xFF0000; }
                if (sys_selected_item == 3) { c_gp_lbl = 0xFF0000; c_gp_val = 0xFF0000; }
                if (sys_selected_item == 4) { c_us_lbl = 0xFF0000; c_us_val = 0xFF0000; }



    int btn_scan_x = wx + 20;
    int btn_scan_y = wy + 150;

    uint32_t btn_oracle_color = 0x444444;

    if (sys_selected_item == 0) { btn_oracle_color = 0xAA0000; }

    os2_DrawRoundedRect(btn_scan_x, btn_scan_y, 250, 30, 4, btn_oracle_color);
    os2_Text(btn_scan_x + 10, btn_scan_y + 8, "OPEN 64-BIT ORACLE", 0xFFFFFF, true);


    bool mouse_klick_oracle = (mouse_just_pressed && os2_is_over_rect(mouse_x, mouse_y, btn_scan_x, btn_scan_y, 250, 30));

    if(os2_input_cooldown == 0 && is_active && mouse_klick_oracle) {
     if (sys_selected_item == 0) {
                        scan_pci_class(0x01, os2_hw_storage, "CTRL");
                        scan_pci_class(0x02, os2_hw_net, "NIC");
                        scan_pci_class(0x03, os2_hw_gpu, "GPU");
                        system_init_usb();
                    } else if (sys_selected_item == 1) { scan_pci_class(0x01, os2_hw_storage, "CTRL"); }
                    else if (sys_selected_item == 2) { scan_pci_class(0x02, os2_hw_net, "NIC"); }
                    else if (sys_selected_item == 3) { scan_pci_class(0x03, os2_hw_gpu, "GPU"); }
                    else if (sys_selected_item == 4) { system_init_usb(); }
     os2_input_cooldown = 25;
    }


                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+270, 300, 20)) {
                    scan_pci_class(0x01, os2_hw_storage, "CTRL");
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+275, l_disk, c_st_lbl, true); os2_Text(wx+130, wy+275, os2_hw_storage, c_st_val, true);



                bool net_hov = os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+295, 180, 20);
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && net_hov) {
                    scan_pci_class(0x02, os2_hw_net, "NIC");
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+300, l_net, c_nt_lbl, true);
                os2_Text(wx+130, wy+300, os2_hw_net, net_hov ? 0xFFFFFF : c_nt_val, true);


                bool dhcp_hov = os2_is_over_rect(mouse_x, mouse_y, wx+ww-130, wy+295, 100, 20);
                os2_DrawRoundedRect(wx+ww-130, wy+295, 100, 20, 3, dhcp_hov ? 0xFF8800 : 0xAA5500);
                os2_TextC(wx+ww-80, wy+302, "DHCP REQ", 0xFFFFFF, true);

                os2_TextC(wx+ww-80, wy+325, os2_cmd_status, 0xFF0000, true);

                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && dhcp_hov) {
                    os2_str_cpy(os2_cmd_status, "BROADCASTING DHCP DISCOVER...");
                    if(os2_net_bar0 == 0) os2_smart_scan();
                    net_check_link();
                    send_dhcp_discover();
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+300, l_net, c_nt_lbl, true); os2_Text(wx+130, wy+300, os2_hw_net, c_nt_val, true);


                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+320, 300, 20)) {
                    scan_pci_class(0x03, os2_hw_gpu, "GPU");
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+325, l_gpu, c_gp_lbl, true); os2_Text(wx+130, wy+325, os2_hw_gpu, c_gp_val, true);


                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+345, 300, 20)) {
                    system_init_usb();
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+350, l_usb, c_us_lbl, true); os2_Text(wx+130, wy+350, os2_hw_usb, c_us_val, true);




                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+370, 100, 20)) {
                    sound_muted = !sound_muted;
                    if(sound_muted) play_freq(0);
                    os2_input_cooldown = 25;
                }
                os2_Text(wx+30, wy+375, sound_muted ? "SOUND: MUTED" : "SOUND: ON", sound_muted ? 0xFF0000 : 0x00FF00, true);


                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+150, wy+370, 100, 20)) {
                    play_sound(1200, 10);
                    os2_input_cooldown = 25;
                }
                os2_DrawRoundedRect(wx+145, wy+370, 90, 18, 3, 0x555555);
                os2_Text(wx+150, wy+375, "TEST BEEP", 0xFFFFFF, true);


                extern uint32_t hda_base_addr;
                extern uint32_t hda_dac_nid;
                extern uint32_t hda_pin_nid;
                extern uint32_t hda_output_stream_offset;
                extern uint32_t hda_codec_id;
                extern uint32_t pci_dev_count;

                char hda_dbg[64];
                if(hda_base_addr == 0) {
                    os2_str_cpy(hda_dbg, "HDA: NOT FOUND (SCANNED: ");
                    char tmp[8]; int_to_str(pci_dev_count, tmp);
                    os2_str_cat(hda_dbg, tmp);
                    os2_str_cat(hda_dbg, " DEV)");
                } else if(hda_dac_nid == 0) {
                    os2_str_cpy(hda_dbg, "HDA: NO CODEC! C0=");
                    char tmp[12]; hex_to_str(os2_hda_debug_c0_resp, tmp);
                    os2_str_cat(hda_dbg, tmp);
                } else {

                    os2_str_cpy(hda_dbg, "HDA:OK C=");
                    char tmp[8]; int_to_str(hda_codec_id, tmp);
                    os2_str_cat(hda_dbg, tmp);
                    os2_str_cat(hda_dbg, " D=");
                    int_to_str(hda_dac_nid, tmp);
                    os2_str_cat(hda_dbg, tmp);
                    os2_str_cat(hda_dbg, " P=");
                    int_to_str(hda_pin_nid, tmp);
                    os2_str_cat(hda_dbg, tmp);
                }
                os2_Text(wx+30, wy+395, hda_dbg, hda_dac_nid ? 0x00FF00 : 0xFF4400, false);



                os2_TextC(mid, wy+405, "LIVE TASK SCHEDULER", 0x000000, true);

                int task_y = wy + 425;
                for(int t = 0; t < 4; t++) {
                    if(tasks[t].active) {
                        char s_id[5]; int_to_str(t, s_id);
                        char* t_name = (char*)((t == 0) ? "COSMOS KERNEL" : "BACKGROUND TASK");


                        uint32_t c_box = (current_task == t) ? 0x00AA00 : (tasks[t].paused ? 0xAA8800 : 0x555555);

                        os2_DrawRoundedRect(wx+30, task_y, 290, 20, 3, c_box);
                        os2_Text(wx+40, task_y+4, "TASK", 0xFFFFFF, true);
                        os2_Text(wx+80, task_y+4, s_id, 0xFFFFFF, true);
                        os2_Text(wx+110, task_y+4, t_name, 0xFFFFFF, true);

                        if(t > 0) {

                            os2_DrawRoundedRect(wx+250, task_y+2, 26, 16, 2, tasks[t].paused ? 0x00AA00 : 0x222222);
                            os2_Text(wx+254, task_y+4, "||", 0xFFFFFF, false);
                            if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+250, task_y+2, 26, 16)) {
                                tasks[t].paused = !tasks[t].paused;
                                if (tasks[t].paused) play_freq(0);
                                os2_input_cooldown = 20;
                            }


                            os2_DrawRoundedRect(wx+280, task_y+2, 26, 16, 2, 0xAA0000);
                            os2_Text(wx+288, task_y+4, "X", 0xFFFFFF, false);
                            if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+280, task_y+2, 26, 16)) {
                                tasks[t].active = false;
                                tasks[t].paused = false;
                                play_freq(0);
                                os2_input_cooldown = 20;
                            }
                        }

                        task_y += 25;
                    }
                }


                os2_DrawRoundedRect(wx+30, wy+wh-50, 120, 30, 4, 0xAA0000); os2_TextC(wx+90, wy+wh-40, "REBOOT", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+30, wy+wh-50, 120, 30)) { os2_system_reboot(); }
                os2_DrawRoundedRect(wx+ww-150, wy+wh-50, 120, 30, 4, 0x000000); os2_TextC(wx+ww-90, wy+wh-40, "SHUT DOWN", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-150, wy+wh-50, 120, 30)) { os2_system_shutdown(); }
            }



            if(win->id == 1) {
                os2_TextC(wx+ww/2, wy+40, "INSTALLED APPS", 0x222222, true);






                os2_DrawRoundedRect(wx+20, wy+80, 150, 50, 5, 0x00FF00);
                os2_TextC(wx+95, wy+97, "PLAY PONG", 0x000000, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && !blocked && os2_is_over_rect(mouse_x, mouse_y, wx+20, wy+80, 150, 50)) {
                    os2_windows[11].open = true; os2_windows[11].minimized = false; os2_focus_window(11); os2_input_cooldown = 25; os2_windows[2].open = false;
                }


                os2_DrawRoundedRect(wx+180, wy+80, 150, 50, 5, 0x00AAFF);
                os2_TextC(wx+255, wy+97, "PLAY BLOBBY", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && !blocked && os2_is_over_rect(mouse_x, mouse_y, wx+180, wy+80, 150, 50)) {
                    os2_windows[12].open = true; os2_windows[12].minimized = false; os2_focus_window(12); os2_input_cooldown = 25; os2_windows[2].open = false;
                }






                os2_DrawRoundedRect(wx+20, wy+140, 150, 50, 5, 0xCC5500);
                os2_TextC(wx+95, wy+157, "WEB EXPLORER", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && !blocked && os2_is_over_rect(mouse_x, mouse_y, wx+20, wy+140, 150, 50)) {
                    os2_windows[8].open = true; os2_windows[8].minimized = false; os2_str_cpy(os2_windows[8].title, "WEB BROWSER"); os2_focus_window(8); os2_input_cooldown = 25; os2_windows[2].open = false;
                }


                os2_DrawRoundedRect(wx+180, wy+140, 150, 50, 5, 0xAA00CC);
                os2_TextC(wx+255, wy+157, "EMULATOR", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && !blocked && os2_is_over_rect(mouse_x, mouse_y, wx+180, wy+140, 150, 50)) {
                    os2_windows[6].open = true; os2_windows[6].minimized = false; os2_str_cpy(os2_windows[6].title, "EMULATOR"); os2_focus_window(6); os2_input_cooldown = 25; os2_windows[2].open = false;
                }






                os2_DrawRoundedRect(wx+20, wy+200, 310, 50, 5, 0x004400);
                os2_TextC(wx+175, wy+217, "COSMOS DEFENDER", 0x00FFBB, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && !blocked && os2_is_over_rect(mouse_x, mouse_y, wx+20, wy+200, 310, 50)) {
                    os2_windows[10].open = true; os2_windows[10].minimized = false; os2_str_cpy(os2_windows[10].title, "COSMOS DEFENDER"); os2_focus_window(10); os2_input_cooldown = 25; os2_windows[2].open = false;
                }
            }




            if(win->id == 4) {

                static bool drives_scanned = false;
                if(!drives_scanned) {
                    drives_scanned = true;
                    drive_count = 0;

                    ahci_mount_drive();


                    if (cartridge_start > 0) {
                        drives[drive_count].type = 5;
                        drives[drive_count].size_mb = (cartridge_end - cartridge_start) / (1024 * 1024);
                        if (drives[drive_count].size_mb == 0) drives[drive_count].size_mb = 1;
                        drives[drive_count].base_port = 0;
                        os2_str_cpy(drives[drive_count].model, "OS CARTRIDGE");
                        drive_count++;
                    }

                    ahci_read_mbr();
                }


                static bool usb_scanned = false;

                bool is_active = (os2_win_z[19] == win->id);
    txt_color = (win->color > 0x888888) ? 0x000000 : 0xFFFFFF;
                uint64_t buf_mbr = (uint64_t)global_buf_mbr;
                uint64_t buf_dir = (uint64_t)global_buf_dir;
    static bool is_ntfs_drive = false;
    static bool is_fat32_drive = false;
                static bool need_ui_refresh = false;
                static int current_page_offset = 0;
    static uint32_t active_ntfs_folder_lba = 5;
    static uint32_t active_fat32_folder_lba = 0;

                static bool clipboard_active = false;
                static char clipboard_name[12] = {0};
    static bool clipboard_is_folder = false;




                if(dsk_mgr_opened) {
                    os2_DrawRoundedRect(wx+15, wy+45, 180, 55, 4, 0x222222); os2_Text(wx+25, wy+50, "DRIVE CAPACITY:", 0xAAAAAA, true);
                    char s_cap[10]; int_to_str(drive_total_gb, s_cap); char s_kb[10]; int_to_str(drive_used_kb, s_kb);
                    char* cap_lbl = (char*)((selected_drive_idx == 99) ? "MB TOTAL" : "GB TOTAL");
                    os2_Text(wx+25, wy+65, s_cap, 0x00FF00, true); os2_Text(wx+55, wy+65, cap_lbl, 0x00FF00, true);
                    os2_Text(wx+25, wy+80, s_kb, 0xFF8800, true); os2_Text(wx+55, wy+80, "KB USED", 0xFF8800, true);




                            if(!is_ntfs_drive && !is_fat32_drive) {

                        os2_DrawRoundedRect(wx+280, wy+45, 80, 25, 4, 0xAA5500); os2_TextC(wx+320, wy+53, "+ FILE", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+280, wy+45, 80, 25)) {
                            for(int i=0; i<512; i++) ((char*)buf_dir)[i] = 0;
                            if(selected_drive_idx != 99) {
                                active_sata_port = drives[selected_drive_idx].base_port;
                                ahci_read_sectors(1002, (uint64_t)buf_dir);
                                for(volatile int wait = 0; wait < 1000000; wait++) ;
                                CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)buf_dir;
                                for(int i=0; i<28; i++) {
                                    if(dir[i].type == 0) {
                                        dir[i].type = 1; dir[i].file_size = 5120; dir[i].start_lba = 10000;
                                        for(int n=0; n<11; n++) { dir[i].filename[n] = 0; cfs_files[i].name[n] = 0; }
                                        os2_str_cpy(dir[i].filename, "APP.BIN");
                                        os2_str_cpy(cfs_files[i].name, "APP.BIN");

                                        cfs_files[i].start_lba = 10000;
                                        cfs_files[i].size = 5120;
                                        cfs_files[i].is_folder = false;

                                        ahci_write_sectors(1002, (uint64_t)buf_dir);
                                        for(volatile int wait2 = 0; wait2 < 1000000; wait2++) ;




                                        cfs_files[i].exists = 1;
                                        break;
                                    }
                                }
                            }
                            os2_input_cooldown = 15;
                        }


                        os2_DrawRoundedRect(wx+280, wy+75, 80, 25, 4, 0xAA00AA); os2_TextC(wx+320, wy+83, "+ FOLD", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+280, wy+75, 80, 25)) {
                            for(int i=0; i<512; i++) ((char*)buf_dir)[i] = 0;
                            if(selected_drive_idx != 99) {
                                active_sata_port = drives[selected_drive_idx].base_port;
                                ahci_read_sectors(1002, (uint64_t)buf_dir);
                                for(volatile int wait = 0; wait < 1000000; wait++) ;
                                CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)buf_dir;
                                for(int i=0; i<28; i++) {
                                    if(dir[i].type == 0) {
                                        dir[i].type = 2; dir[i].file_size = 0; dir[i].start_lba = 0;
                                        dir[i].parent_idx = current_folder_id;
                                        for(int n=0; n<11; n++) { dir[i].filename[n] = 0; cfs_files[i].name[n] = 0; }
                                        os2_str_cpy(dir[i].filename, "TESTFOLDER");
                                        os2_str_cpy(cfs_files[i].name, "TESTFOLDER");

                                        cfs_files[i].start_lba = 0;
                                        cfs_files[i].size = 0;
                                        cfs_files[i].is_folder = true;
                                        cfs_files[i].parent_idx = current_folder_id;

                                        ahci_write_sectors(1002, (uint64_t)buf_dir);
                                        for(volatile int wait2 = 0; wait2 < 1000000; wait2++) ;

                                        cfs_files[i].exists = 1;
                                        break;
                                    }
                                }
                            }
                            os2_input_cooldown = 15;
                        }
                    }




                    if(clipboard_active) {
                        os2_DrawRoundedRect(wx+370, wy+45, 80, 25, 4, 0x00AA55);
                        os2_TextC(wx+410, wy+53, "PASTE", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+370, wy+45, 80, 25)) {
                            if(!is_ntfs_drive && !is_fat32_drive) {
                                uint64_t tmp_dir = (uint64_t)global_tmp_dir;

                                for(int k = 0; k < 512; k++) ((char*)tmp_dir)[k] = 0;

                                disk_read_auto(1002, tmp_dir);
                                for(volatile int wait = 0; wait < 100000; wait++) ;

                                CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)tmp_dir;
                                int free_slot = -1;
                                for(int s=0; s<28; s++) { if(entries[s].type == 0) { free_slot = s; break; } }

                                if(free_slot != -1) {
                                    uint32_t target_lba = 4000 + (free_slot * 10);

                                    if(!clipboard_is_folder) {

                                        for(int s=0; s<10; s++) {
                                            disk_write_auto(target_lba + s, (uint64_t)global_buf_dir + (s * 512));
                                            for(volatile int wait = 0; wait < 50000; wait++) ;
                                        }
                                        entries[free_slot].type = 1;
                                        entries[free_slot].file_size = 5120;
                                        entries[free_slot].start_lba = target_lba;
                                    } else {

                                        entries[free_slot].type = 2;
                                        entries[free_slot].file_size = 0;
                                        entries[free_slot].start_lba = 0;
                                    }

                                    entries[free_slot].parent_idx = current_folder_id;

                                    for(int n=0; n<11; n++) entries[free_slot].filename[n] = 0;
                                    os2_str_cpy(entries[free_slot].filename, clipboard_name);

                                    disk_write_auto(1002, tmp_dir);
                                    for(volatile int wait = 0; wait < 200000; wait++) ;

                                    print_win(win, "\n[OK] PASTED FROM CLIPBOARD.\n");
                                    need_ui_refresh = true;
                                    clipboard_active = false;
                                } else {
                                    print_win(win, "\n[ERR] FOLDER IS FULL.\n");
                                }
                            } else {
                                print_win(win, "\n[ERR] PASTE ONLY ON CFS NOW.\n");
                            }
                            os2_input_cooldown = 25;
                        }
                    }




                    if(current_folder_id != 255) {
                        os2_Text(wx+15, wy+120, "TARGET:", 0xAAAAAA, true);
                        os2_Text(wx+80, wy+120, cfs_files[current_folder_id].name, 0x00FF00, true);


                        os2_DrawRoundedRect(wx+15, wy+140, 60, 20, 2, 0x444444); os2_Text(wx+20, wy+145, "[ BACK ]", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+15, wy+140, 60, 20)) {
                            current_folder_id = 255; current_page_offset = 0; need_ui_refresh = true; os2_input_cooldown = 15; active_ntfs_folder_lba = 5;
                            if (is_fat32_drive) active_fat32_folder_lba = fat32_root_lba;
                        }


                        os2_DrawRoundedRect(wx+85, wy+140, 45, 20, 2, 0x444444); os2_Text(wx+92, wy+145, "PREV", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+85, wy+140, 45, 20)) {
                            if (current_page_offset >= 10) current_page_offset -= 10;
                            need_ui_refresh = true; os2_input_cooldown = 15;
                        }


                        os2_DrawRoundedRect(wx+135, wy+140, 45, 20, 2, 0x444444); os2_Text(wx+142, wy+145, "NEXT", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+135, wy+140, 45, 20)) {
                            current_page_offset += 10;
                            need_ui_refresh = true; os2_input_cooldown = 15;
                        }


                        os2_DrawRoundedRect(wx+185, wy+140, 45, 20, 2, 0x0055AA); os2_Text(wx+194, wy+145, "REF", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+185, wy+140, 45, 20)) {
                            need_ui_refresh = true; os2_input_cooldown = 15;
                        }
                    } else {
                        os2_Text(wx+15, wy+120, "--- ROOT DIRECTORY ---", 0xFFFFFF, true);
                    }


                    os2_DrawRoundedRect(wx+210, wy+45, 60, 25, 4, 0x444444); os2_TextC(wx+240, wy+53, "BACK", 0xFFFFFF, true);
                    if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+210, wy+45, 60, 25)) {
                        dsk_mgr_opened = false; os2_input_cooldown = 15;
                    }




                    if(is_ntfs_drive && need_ui_refresh) {

                        int target_mft_id = (current_folder_id == 255) ? 5 : active_ntfs_folder_lba;

                        for(int f=0; f<28; f++) { cfs_files[f].exists = 0; cfs_files[f].name[0] = 0; }
                        int file_idx = 0; int skipped = 0;
                        uint8_t* mft_cache = new uint8_t[10*1024*1024];

                        for(int rec = 16; rec < 50000; rec++) {
                            if (file_idx >= 28) break;

                            uint8_t* mft_rec = mft_cache + (rec * 1024);

                            if (mft_rec[0] == 'F' && mft_rec[1] == 'I' && mft_rec[2] == 'L' && mft_rec[3] == 'E' && (*(uint16_t*)&mft_rec[22] & 0x01)) {
                                int attr_pos = *(uint16_t*)&mft_rec[20];
                                while(attr_pos > 0 && attr_pos < 1000) {
                                    uint32_t attr_type = *(uint32_t*)&mft_rec[attr_pos];
                                    uint32_t attr_len = *(uint32_t*)&mft_rec[attr_pos + 4];
                                    if (attr_type == 0xFFFFFFFF || attr_len <= 0) break;

                                    if (attr_type == 0x30 && mft_rec[attr_pos + 8] == 0) {
                                        int fn_base = attr_pos + *(uint16_t*)&mft_rec[attr_pos + 20];
                                        if (fn_base < 0 || fn_base + 80 >= 1024 || *(uint32_t*)&mft_rec[fn_base + 0] != target_mft_id || mft_rec[fn_base + 65] == 2) { attr_pos += attr_len; continue; }


                                        if (skipped < current_page_offset) { skipped++; break; }

                                        uint8_t name_len = mft_rec[fn_base + 64];
                                        cfs_files[file_idx].exists = 1; cfs_files[file_idx].parent_idx = current_folder_id;
                                        cfs_files[file_idx].is_folder = (*(uint16_t*)&mft_rec[22] & 0x02) ? 1 : 0;
                                        cfs_files[file_idx].size = *(uint32_t*)&mft_rec[fn_base + 48]; cfs_files[file_idx].start_lba = rec;

                                        int chars_to_copy = (name_len > 60) ? 60 : name_len;
                                        for(int c=0; c<chars_to_copy; c++) {
                                            char ch = mft_rec[fn_base + 66 + (c * 2)];
                                            if (ch >= 'a' && ch <= 'z') ch -= 32;
                                            if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '_' || ch == '-' || ch == ' ') { cfs_files[file_idx].name[c] = ch; }
                                            else { cfs_files[file_idx].name[c] = '_'; }
                                        }
                                        cfs_files[file_idx].name[chars_to_copy] = 0;
                                        file_idx++;
                                        break;
                                    }
                                    attr_pos += attr_len;
                                }
                            }
                        }
                        need_ui_refresh = false;
                    }

                    if(is_fat32_drive && need_ui_refresh) {
                        for(int f=0; f<28; f++) { cfs_files[f].exists = 0; cfs_files[f].name[0] = 0; }

                        uint8_t* dir_buf = (uint8_t*)buf_dir;
                        disk_read_auto(active_fat32_folder_lba, (uint64_t)dir_buf);
                        for(volatile int w = 0; w < 500000; w++) ;

                        FAT32_ParsedFile pfiles[28];
                        if (fat32_list_dir(active_fat32_folder_lba, dir_buf, pfiles, 28, current_folder_id, current_page_offset)) {
                            for(int i=0; i<28; i++) {
                                cfs_files[i].exists = pfiles[i].exists;
                                cfs_files[i].parent_idx = pfiles[i].parent_idx;
                                cfs_files[i].is_folder = pfiles[i].is_folder;
                                cfs_files[i].size = pfiles[i].size;
                                cfs_files[i].start_lba = pfiles[i].start_lba;
                                for(int n=0; n<64; n++) cfs_files[i].name[n] = pfiles[i].name[n];
                            }
                        }

                        need_ui_refresh = false;
                    }




                    int y_off = wy + 170;
                    int visible_count = 0;
                    for(int i=0; i<28; i++) {
                        if(cfs_files[i].exists && cfs_files[i].parent_idx == current_folder_id) {
                            if (visible_count >= 10) break;


                            bool is_hov = os2_is_over_rect(mouse_x, mouse_y, wx+15, y_off, 265, 20);
                            uint32_t icon_col = cfs_files[i].is_folder ? 0xFFAA00 : (is_hov ? 0x00AAFF : 0x0088FF);
                            os2_DrawRoundedRect(wx+15, y_off, 16, 16, 2, icon_col);
                            os2_Text(wx+40, y_off+4, cfs_files[i].name, is_hov ? 0x00FF00 : 0xFFFFFF, false);


                            os2_DrawRoundedRect(wx+190, y_off, 40, 16, 2, 0x0055AA);
                            os2_Text(wx+198, y_off+4, "OPEN", 0xFFFFFF, false);


                            os2_DrawRoundedRect(wx+235, y_off, 40, 16, 2, 0xAA5500);
                            os2_Text(wx+241, y_off+4, "COPY", 0xFFFFFF, false);




                            if(os2_input_cooldown == 0 && mouse_just_pressed && os2_is_over_rect(mouse_x, mouse_y, wx+190, y_off, 40, 16)) {

                                if (!is_active) os2_focus_window(win->id);

                                if(cfs_files[i].is_folder) {
                                    print_win(win, "\n[SYS] ENTERING FOLDER...\n");
                                    active_ntfs_folder_lba = cfs_files[i].start_lba;
                                    active_fat32_folder_lba = cfs_files[i].start_lba;
                                    current_folder_id = i;
                                    current_page_offset = 0;
                                    need_ui_refresh = true;
                                } else {
                                    bool is_tba = false;
                                    bool is_jpg_png = false;
                                    bool is_bmp = false;
                                    bool is_ch8 = false;
                                    bool is_bin = false;
                                    bool is_wav = false;


                                    for(int c=0; c<61; c++) {
                                        if(cfs_files[i].name[c] == 'B' && cfs_files[i].name[c+1] == 'I' && cfs_files[i].name[c+2] == 'N') is_bin = true;


                                        if(cfs_files[i].name[c] == 'C' && cfs_files[i].name[c+1] == 'H' && cfs_files[i].name[c+2] == '8') is_ch8 = true;
                                        if(cfs_files[i].name[c] == 'c' && cfs_files[i].name[c+1] == 'h' && cfs_files[i].name[c+2] == '8') is_ch8 = true;

                                        if((cfs_files[i].name[c] == 'W' && cfs_files[i].name[c+1] == 'A' && cfs_files[i].name[c+2] == 'V') ||
                                           (cfs_files[i].name[c] == 'w' && cfs_files[i].name[c+1] == 'a' && cfs_files[i].name[c+2] == 'v')) is_wav = true;
                                        if((cfs_files[i].name[c] == 'B' && cfs_files[i].name[c+1] == 'M' && cfs_files[i].name[c+2] == 'P') ||
                                           (cfs_files[i].name[c] == 'b' && cfs_files[i].name[c+1] == 'm' && cfs_files[i].name[c+2] == 'p')) is_bmp = true;
                                        if((cfs_files[i].name[c] == 'J' && cfs_files[i].name[c+1] == 'P' && cfs_files[i].name[c+2] == 'G') ||
                                           (cfs_files[i].name[c] == 'j' && cfs_files[i].name[c+1] == 'p' && cfs_files[i].name[c+2] == 'g') ||
                                           (cfs_files[i].name[c] == 'P' && cfs_files[i].name[c+1] == 'N' && cfs_files[i].name[c+2] == 'G') ||
                                           (cfs_files[i].name[c] == 'p' && cfs_files[i].name[c+1] == 'n' && cfs_files[i].name[c+2] == 'g')) is_jpg_png = true;

                                        if((cfs_files[i].name[c] == 'T' && cfs_files[i].name[c+1] == 'B' && cfs_files[i].name[c+2] == 'A') ||
                                           (cfs_files[i].name[c] == 't' && cfs_files[i].name[c+1] == 'b' && cfs_files[i].name[c+2] == 'a')) is_tba = true;
                                    }


                                    if(is_tba) {
                                        print_win(win, "\n[SYS] DECOMPRESSING TBA CONTAINER...\n");

                                        uint32_t tba_lba = cfs_files[i].start_lba;
                                        uint32_t tba_size_bytes = cfs_files[i].size;

                                        if(tba_size_bytes == 0) tba_size_bytes = 1024 * 1024;

                                        uint8_t* comp_buffer = new uint8_t[10*1024*1024];
                                        uint8_t* decomp_buffer = new uint8_t[10*1024*1024];

                                        uint32_t tba_sectors = (tba_size_bytes + 511) / 512;
                                        for(uint32_t s = 0; s < tba_sectors; s++) {
                                            disk_read_auto(tba_lba + s, (uint64_t)comp_buffer + (s * 512));
                                        }
                                        for(volatile int w = 0; w < 500000; w++) ;

                                        TBAHeader* hdr = (TBAHeader*)comp_buffer;
                                        if(hdr->magic[0] == 'T' && hdr->magic[1] == 'B' && hdr->magic[2] == 'A' && hdr->magic[3] == '4') {

                                            uint32_t file_count = hdr->fileCount;
                                            uint64_t buf_dir = (uint64_t)global_buf_dir;
                                            disk_read_auto(1002, buf_dir);
                                            for(volatile int w = 0; w < 500000; w++) ;
                                            CFS_DIR_ENTRY* cfs_dir = (CFS_DIR_ENTRY*)buf_dir;

                                            uint32_t next_free_lba = 12000;

                                            for(uint32_t f = 0; f < file_count; f++) {

                                                uint8_t* entry_ptr = comp_buffer + sizeof(TBAHeader) + (f * sizeof(TBAFileEntry));
                                                TBAFileEntry* current_entry = (TBAFileEntry*)entry_ptr;

                                                int name_start = 0;
                                                int path_len = 0;
                                                for(int k = 0; k < 512; k++) {
                                                    char c = current_entry->name[k];
                                                    if (c == 0) break;
                                                    if (c == '/' || c == '\\') { name_start = k + 1; path_len = k; }
                                                }

                                                int true_name_len = 0;
                                                while (current_entry->name[name_start + true_name_len] != 0 &&
                                                       current_entry->name[name_start + true_name_len] != ' ' &&
                                                       (name_start + true_name_len) < 512) {
                                                    true_name_len++;
                                                }

                                                uint8_t target_parent = current_folder_id;

                                                if(path_len > 0) {
                                                    char folder_name[11];
                                                    for(int n = 0; n < 11; n++) folder_name[n] = 0;
                                                    int f_start = 0;
                                                    for (int k = path_len - 1; k >= 0; k--) {
                                                        if (k > 0 && (current_entry->name[k-1] == '/' || current_entry->name[k-1] == '\\')) { f_start = k; break; }
                                                    }
                                                    int fn_len = path_len - f_start;
                                                    if (fn_len > 11) fn_len = 11;
                                                    for(int n = 0; n < fn_len; n++) {
                                                        char ch = current_entry->name[f_start + n];
                                                        if (ch >= 'a' && ch <= 'z') ch -= 32;
                                                        folder_name[n] = ch;
                                                    }
                                                    int found_folder_slot = -1;
                                                    for(int cs = 0; cs < 28; cs++) {
                                                        if (cfs_dir[cs].type == 2 && cfs_dir[cs].parent_idx == current_folder_id) {
                                                            bool match = true;
                                                            for(int n=0; n<11; n++) { if (cfs_dir[cs].filename[n] != folder_name[n]) match = false; }
                                                            if (match) { found_folder_slot = cs; break; }
                                                        }
                                                    }
                                                    if (found_folder_slot == -1) {
                                                        for(int cs = 0; cs < 28; cs++) { if (cfs_dir[cs].type == 0) { found_folder_slot = cs; break; } }
                                                        if (found_folder_slot != -1) {
                                                            cfs_dir[found_folder_slot].type = 2;
                                                            cfs_dir[found_folder_slot].start_lba = 0;
                                                            cfs_dir[found_folder_slot].file_size = 0;
                                                            cfs_dir[found_folder_slot].parent_idx = current_folder_id;
                                                            cfs_dir[found_folder_slot].reserved = 0;
                                                            for(int n=0; n<11; n++) cfs_dir[found_folder_slot].filename[n] = folder_name[n];

                                                            cfs_files[found_folder_slot].exists = 1;
                                                            cfs_files[found_folder_slot].is_folder = 1;
                                                            cfs_files[found_folder_slot].parent_idx = current_folder_id;
                                                            cfs_files[found_folder_slot].size = 0;
                                                            cfs_files[found_folder_slot].start_lba = 0;
                                                            for(int n = 0; n < 11; n++) cfs_files[found_folder_slot].name[n] = folder_name[n];
                                                            cfs_files[found_folder_slot].name[11] = 0;
                                                        }
                                                    }
                                                    if (found_folder_slot != -1) target_parent = found_folder_slot;
                                                }

                                                uint32_t comp_sz = (uint32_t)current_entry->compressedSize;
                                                uint32_t orig_sz = (uint32_t)current_entry->originalSize;

                                                if(true_name_len == 0 || orig_sz == 0) continue;

                                                int free_slot = -1;
                                                for(int cs = 0; cs < 28; cs++) {
                                                    if (cfs_dir[cs].type == 0) { free_slot = cs; break; }
                                                }
                                                if(free_slot == -1) {
                                                    print_win(win, "[ERR] CFS FULL! CANNOT EXTRACT MORE.\n");
                                                    break;
                                                }

                                                uint8_t* src_ptr = comp_buffer + current_entry->offset;
                                                for(uint32_t m = 0; m < orig_sz + 512; m++) decomp_buffer[m] = 0;




                                                if(current_entry->isPureStream == 1) {

                                                    uint32_t framesCount = orig_sz / 26;
                                                    uint32_t dst_p = 0;
                                                    for(uint32_t fc = 0; fc < framesCount; fc++) {
                                                        decomp_buffer[dst_p] = 0x2A;
                                                        for(uint32_t cp = 0; cp < 24; cp++) decomp_buffer[dst_p + 1 + cp] = src_ptr[(fc * 24) + cp];
                                                        decomp_buffer[dst_p + 25] = 0xFF;
                                                        dst_p += 26;
                                                    }
                                                }
                                                else if(current_entry->isCompressed == 1) {
                                                    uint32_t bCnt = *(uint32_t*)src_ptr; src_ptr += 4;
                                                    uint32_t mapBytes = (bCnt + 7) / 8;
                                                    uint8_t* mapData = src_ptr; src_ptr += mapBytes;

                                                    uint32_t dstIdx = 0;
                                                    for(uint32_t b = 0; b < bCnt; b++) {
                                                        uint32_t currentBlockSize = 65536;
                                                        if(b == bCnt - 1) {
                                                            uint32_t rem = orig_sz % 65536;
                                                            if(rem != 0) currentBlockSize = rem;
                                                        }

                                                        bool isBlockComp = (mapData[b / 8] & (1 << (b % 8))) != 0;
                                                        if(isBlockComp) {
                                                            uint32_t cSize = *(uint32_t*)src_ptr; src_ptr += 4;
                                                            decompress_tba((uint8_t*)src_ptr, cSize, (uint8_t*)(decomp_buffer + dstIdx), currentBlockSize);
                                                            src_ptr += cSize;
                                                        } else {
                                                            for(uint32_t m = 0; m < currentBlockSize; m++) decomp_buffer[dstIdx + m] = src_ptr[m];
                                                            src_ptr += currentBlockSize;
                                                        }
                                                        dstIdx += currentBlockSize;
                                                    }
                                                } else {

                                                    for(uint32_t m = 0; m < orig_sz; m++) decomp_buffer[m] = src_ptr[m];
                                                }


                                                uint32_t write_sectors = (orig_sz + 511) / 512;
                                                for(uint32_t ws = 0; ws < write_sectors; ws++) {
                                                    disk_write_auto(next_free_lba + ws, (uint64_t)decomp_buffer + (ws * 512));
                                                    for(volatile int w = 0; w < 2000; w++) ;
                                                }

                                                cfs_dir[free_slot].type = 1;
                                                cfs_dir[free_slot].start_lba = (uint16_t)next_free_lba;
                                                cfs_dir[free_slot].file_size = (uint16_t)orig_sz;
                                                cfs_dir[free_slot].parent_idx = target_parent;
                                                cfs_dir[free_slot].reserved = 0;

                                                for(int n = 0; n < 11; n++) cfs_dir[free_slot].filename[n] = 0;

                                                int dot_idx = -1; int end_idx = 0;
                                                for(int k = 511; k >= 0; k--) {
                                                    char c = current_entry->name[k];
                                                    if (end_idx == 0 && c > 32 && c <= 126) end_idx = k;
                                                    if (c == '.' && dot_idx == -1) dot_idx = k;
                                                }
                                                int start_idx = 0;
                                                if (end_idx > 0) {
                                                    for(int k = end_idx; k >= 0; k--) {
                                                        char c = current_entry->name[k];
                                                        if (c == '/' || c == '\\') { start_idx = k + 1; break; }
                                                    }
                                                }
                                                if (dot_idx != -1 && dot_idx > start_idx) {
                                                    int base_len = dot_idx - start_idx;
                                                    int ext_len = end_idx - dot_idx + 1;
                                                    if (ext_len > 4) ext_len = 4;
                                                    int max_base_len = 11 - ext_len;
                                                    if (base_len > max_base_len) base_len = max_base_len;

                                                    for(int n = 0; n < base_len; n++) cfs_dir[free_slot].filename[n] = current_entry->name[start_idx + n];
                                                    for(int n = 0; n < ext_len; n++) cfs_dir[free_slot].filename[base_len + n] = current_entry->name[dot_idx + n];
                                                } else {
                                                    int len = end_idx - start_idx + 1;
                                                    if (len > 11) len = 11;
                                                    for(int n = 0; n < len; n++) cfs_dir[free_slot].filename[n] = current_entry->name[start_idx + n];
                                                }

                                                for(int n = 0; n < 11; n++) {
                                                    char ch = cfs_dir[free_slot].filename[n];
                                                    if (ch >= 'a' && ch <= 'z') cfs_dir[free_slot].filename[n] = ch - 32;
                                                }

                                                cfs_files[free_slot].exists = 1;
                                                cfs_files[free_slot].is_folder = 0;
                                                cfs_files[free_slot].parent_idx = target_parent;
                                                cfs_files[free_slot].size = orig_sz;
                                                cfs_files[free_slot].start_lba = next_free_lba;
                                                for(int n = 0; n < 11; n++) cfs_files[free_slot].name[n] = cfs_dir[free_slot].filename[n];
                                                cfs_files[free_slot].name[11] = 0;

                                                next_free_lba += write_sectors + 2;
                                            }

                                            disk_write_auto(1002, buf_dir);
                                            for(volatile int w = 0; w < 500000; w++) ;

                                            print_win(win, "[OK] SUCCESS! ALL ROMS UNPACKED!\n");
                                        } else {
                                            print_win(win, "[ERR] BAD TBA SIGNATURE!\n");
                                        }
                                        os2_input_cooldown = 25;
                                        need_ui_refresh = true;
                                    }
                                    else if(is_jpg_png) {
                                        print_win(win, "\n[SYS] DECODING JPG/PNG IN MEMORY...\n");
                                        uint32_t raw_ram_addr = 0x0E000000;
                                        uint32_t total_sectors = (cfs_files[i].size / 512) + 1;
                                        for(uint32_t sec = 0; sec < total_sectors; sec++) {
                                            disk_read_auto(cfs_files[i].start_lba + sec, raw_ram_addr + (sec * 512));
                                            for(volatile int wait = 0; wait < 2000; wait++) ;
                                        }
                                        int w=0, h=0, comp=0;
                                        unsigned char* decoded = decode_image((unsigned char*)raw_ram_addr, cfs_files[i].size, &w, &h, &comp, 3);
                                        if(decoded != 0) {
                                            uint32_t bmp_ram_addr = 0x0C000000;
                                            uint8_t* bmp = (uint8_t*)bmp_ram_addr;
                                            for(int z=0; z<54; z++) bmp[z] = 0;
                                            bmp[0] = 'B'; bmp[1] = 'M';
                                            *(uint32_t*)(bmp_ram_addr + 2) = 54 + (w * h * 3);
                                            *(uint32_t*)(bmp_ram_addr + 10) = 54;
                                            *(uint32_t*)(bmp_ram_addr + 14) = 40;
                                            *(int32_t*)(bmp_ram_addr + 18) = w;
                                            *(int32_t*)(bmp_ram_addr + 22) = h;
                                            *(uint16_t*)(bmp_ram_addr + 26) = 1;
                                            *(uint16_t*)(bmp_ram_addr + 28) = 24;
                                            *(uint32_t*)(bmp_ram_addr + 30) = 0;
                                            *(uint32_t*)(bmp_ram_addr + 34) = w * h * 3;

                                            uint8_t* pixels = (uint8_t*)(bmp_ram_addr + 54);
                                            for(int y=0; y<h; y++) {
                                                for(int x=0; x<w; x++) {
                                                    int src_idx = (y * w + x) * 3;
                                                    int dst_idx = ((h - 1 - y) * w + x) * 3;
                                                    pixels[dst_idx] = decoded[src_idx + 2];
                                                    pixels[dst_idx + 1] = decoded[src_idx + 1];
                                                    pixels[dst_idx + 2] = decoded[src_idx];
                                                }
                                            }

                                            free_image(decoded);

                                            print_win(win, "\n[SYS] OPENING IMAGE VIEWER...\n");
                                            os2_windows[9].open = true; os2_windows[9].minimized = false; os2_focus_window(9);
                                            os2_str_cpy(os2_windows[9].title, cfs_files[i].name);
                                            os2_windows[9].content[0] = 'B'; os2_windows[9].content[1] = 'M'; os2_windows[9].content[2] = 'P'; os2_windows[9].content[3] = 0;
                                        } else {
                                            print_win(win, "\n[ERR] DECODE FAILED!\n");
                                        }
                                    } else if(is_bmp) {
                                        print_win(win, "\n[SYS] OPENING IMAGE VIEWER...\n");
                                        os2_windows[9].open = true; os2_windows[9].minimized = false; os2_focus_window(9);
                                        os2_str_cpy(os2_windows[9].title, cfs_files[i].name);
                                        uint32_t bmp_ram_addr = 0x0C000000;
                                        uint32_t total_sectors = (cfs_files[i].size / 512) + 1;
                                        for(uint32_t sec = 0; sec < total_sectors; sec++) {
                                            disk_read_auto(cfs_files[i].start_lba + sec, bmp_ram_addr + (sec * 512));
                                            for(volatile int wait = 0; wait < 2000; wait++) ;
                                        }
                                        os2_windows[9].content[0] = 'B'; os2_windows[9].content[1] = 'M'; os2_windows[9].content[2] = 'P'; os2_windows[9].content[3] = 0;
         } else if(is_bin) {
                                        print_win(win, "\n[SYS] LOADING BINARY APP...\n");

                                        uint32_t bin_sz = cfs_files[i].size;
                                        if(bin_sz == 0) bin_sz = 16384;
                                        uint32_t app_sectors = (bin_sz + 511) / 512;

                                        if (load_and_run_bin(cfs_files[i].start_lba, app_sectors)) {
                                            print_win(win, "[SYS] TASK SPAWNED!\n");




           os2_windows[14].id = 14;
           os2_windows[14].w = 400;
           os2_windows[14].h = 300;
           os2_windows[14].x = 312;
           os2_windows[14].y = 234;

           os2_windows[14].open = true;
           os2_windows[14].minimized = false;
           os2_str_cpy(os2_windows[14].title, "Super Smash Cats");

           os2_windows[14].content[0] = 'A';
           os2_windows[14].content[1] = 'P';
           os2_windows[14].content[2] = 'P';
           os2_windows[14].content[3] = 0;

           os2_focus_window(14);

                                        } else {
                                            print_win(win, "[ERR] LOAD FAILED!\n");
                                        }
                                    } else if(is_ch8) {
                                        print_win(win, "\n[SYS] SENDING .BIN TO CHIP-8 HYPERVISOR...\n");


                                        c8_reset();


                                        int sectors = (cfs_files[i].size + 511) / 512;
                                        if (sectors > 7) sectors = 7;


                                        unsigned long long target_ram = (unsigned long long)&c8_ram[0x200];
                                        for (int s = 0; s < sectors; s++) {
                                            disk_read_auto(cfs_files[i].start_lba + s, target_ram + (s * 512));
                                            for(volatile int wait = 0; wait < 2000; wait++) ;
                                        }


                                        c8_pc = 0x200;
                                        c8_state = 1;

                                        extern VirtualMachine emu_chip8;
                                        extern VirtualMachine* active_vm;
                                        active_vm = &emu_chip8;


                                        os2_windows[13].open = true;
                                        os2_windows[13].minimized = false;
                                        os2_focus_window(13);

                                        print_win(win, "[SYS] EMULATOR LAUNCHED!\n");
                                    } else if(is_wav) {
                                        print_win(win, "\n[SYS] PLAYING WAV AUDIO...\n");
                                        uint32_t wav_ram_addr = 0x0B500000;
                                        uint32_t total_sectors = (cfs_files[i].size / 512) + 1;
                                        for(uint32_t sec = 0; sec < total_sectors; sec++) {
                                            disk_read_auto(cfs_files[i].start_lba + sec, wav_ram_addr + (sec * 512));
                                            for(volatile int wait = 0; wait < 2000; wait++) ;
                                        }

                                        uint8_t* wav = (uint8_t*)wav_ram_addr;
                                        if (wav[0] == 'R' && wav[1] == 'I' && wav[2] == 'F' && wav[3] == 'F') {
                                            uint16_t channels = *(uint16_t*)&wav[22];
                                            uint32_t sample_rate = *(uint32_t*)&wav[24];
                                            uint16_t bits = *(uint16_t*)&wav[34];
                                            uint32_t data_size = 0;
                                            uint32_t data_offset = 12;
                                            while (data_offset < 100) {
                                                if (wav[data_offset] == 'd' && wav[data_offset+1] == 'a' && wav[data_offset+2] == 't' && wav[data_offset+3] == 'a') {
                                                    data_size = *(uint32_t*)&wav[data_offset+4];
                                                    data_offset += 8;
                                                    break;
                                                }
                                                data_offset++;
                                            }
                                            if (data_size > 0) {
                                                play_hda_wav(wav_ram_addr + data_offset, data_size, sample_rate, channels, bits);
                                            }
                                        }

                                    } else {
                                        print_win(win, "\n[SYS] OPENING IN NOTEPAD...\n");
                                        os2_windows[0].open = true; os2_windows[0].minimized = false; os2_focus_window(0);
                                        active_file_lba = cfs_files[i].start_lba; active_file_idx = i;
                                        os2_str_cpy(os2_windows[0].title, cfs_files[i].name);
                                        uint32_t text_ram_addr = 0x03000000; char* text_buffer = (char*)text_ram_addr;
                                        for(int j=0; j<2000; j++) text_buffer[j] = 0;

                                        disk_read_auto(active_file_lba, text_ram_addr);
                                        for(volatile int wait = 0; wait < 1000000; wait++) ;

                                        int limit = cfs_files[i].size;
                                        if(limit > 2000) limit = 2000; if(limit == 0) limit = 512;
                                        int c_idx = 0;
                                        for(int c=0; c<limit; c++) {
                                            char ch = text_buffer[c]; if(ch == 0) break;
                                            if((ch >= 32 && ch <= 126) || ch == '\n') { os2_windows[0].content[c_idx++] = ch; }
                                        }
                                        os2_windows[0].content[c_idx] = 0; os2_windows[0].cursor_pos = c_idx;
                                    }
                                }
                                os2_input_cooldown = 25;
                            }




                            if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+235, y_off, 40, 16)) {
                                os2_str_cpy(win->title, "READING...");
                                print_win(win, "\n[SYS] COPY TO RAM CLIPBOARD...\n");


                                clipboard_is_folder = cfs_files[i].is_folder;

                                if(!clipboard_is_folder) {
                                    uint32_t copy_ram_buffer = 0x09000000;
                                    for(int j=0; j<5120; j++) ((char*)copy_ram_buffer)[j] = 0;


                                    for(int s=0; s<10; s++) {
                                        disk_read_auto(cfs_files[i].start_lba + s, copy_ram_buffer + (s * 512));
                                        for(volatile int wait = 0; wait < 50000; wait++) ;
                                    }
                                }


                                os2_str_cpy(clipboard_name, cfs_files[i].name);
                                clipboard_active = true;

                                os2_str_cpy(win->title, "IN CLIPBOARD");
                                print_win(win, "[OK] READY TO PASTE.\n");
                                os2_input_cooldown = 25;
                            }

                            visible_count++;
                            y_off += 25;
                        }
                    }
                } else {


     int list_y = wy + 60;
     os2_Text(wx+15, list_y - 15, "AVAILABLE DRIVES:", 0xAAAAAA, true);


                    os2_DrawRoundedRect(wx+140, list_y - 18, 35, 16, 2, 0x444444);
                    os2_Text(wx+146, list_y - 15, "REF", 0xFFFFFF, true);
                    if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+140, list_y - 18, 35, 16)) {
                        drives_scanned = false;
                        os2_input_cooldown = 15;
                    }


                    os2_DrawRoundedRect(wx+180, list_y - 18, 65, 16, 2, 0x333333);
                    os2_Text(wx+185, list_y - 15, "NO USB", 0x888888, true);

     for(int i=0; i < drive_count; i++) {
      bool is_sel = (selected_drive_idx == i);
      os2_DrawRoundedRect(wx+15, list_y, 160, 25, 4, is_sel ? 0x0088FF : 0x333333);
      os2_Text(wx+25, list_y+5, drives[i].model, 0xFFFFFF, true);

      if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+15, list_y, 160, 25)) {
       selected_drive_idx = i;
       is_mounted = false;
       os2_input_cooldown = 15;
      }
      list_y += 30;
     }


     uint32_t btn_col = (selected_drive_idx == -1) ? 0x444444 : 0x00AA00;
     os2_DrawRoundedRect(wx+150, wy+60, 80, 25, 4, btn_col);
     os2_TextC(wx+190, wy+68, "OPEN", 0xFFFFFF, true);

     os2_DrawRoundedRect(wx+240, wy+60, 80, 25, 4, (selected_drive_idx == -1) ? 0x444444 : 0xAA0000);
     os2_TextC(wx+280, wy+68, "FORMAT", 0xFFFFFF, true);

     char diag[64];
     os2_str_cpy(diag, "USB IO BASE: 0x");
     hex_to_str(usb_io_base, diag + 15);
     os2_Text(wx+15, wy+220, diag, 0xAAAAAA, false);

     char diag2[64];
     os2_str_cpy(diag2, "xHCI BASE: 0x");
     hex_to_str((uint32_t)global_xhci_base_addr, diag2 + 13);
     os2_Text(wx+15, wy+235, diag2, 0xAAAAAA, false);
     os2_Text(wx+15, wy+250, os2_cmd_status, 0xFF8800, false);


                    if(selected_drive_idx != -1 && os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+240, wy+60, 80, 25)) {
                        win->cursor_pos = 0;
                        char* mbr = (char*)buf_mbr; for(int i=0; i<512; i++) mbr[i] = 0;
                        mbr[3] = 'C'; mbr[4] = 'F'; mbr[5] = 'S'; mbr[510] = 0x55; mbr[511] = 0xAA;
                        disk_write_auto(0, (uint64_t)buf_mbr);
                        for(volatile int wait = 0; wait < 1000000; wait++) ;

                        char* dir = (char*)buf_dir; for(int i=0; i<512; i++) dir[i] = 0;
                        disk_write_auto(1002, (uint64_t)buf_dir);
                        for(volatile int wait2 = 0; wait2 < 1000000; wait2++) ;

                        for(int i=0; i<28; i++) { cfs_files[i].exists = 0; cfs_files[i].is_folder = 0; cfs_files[i].parent_idx = 255; }
                        is_mounted = false; current_folder_id = 255;
                        print_win(win, "\n[OK] OS2-CFS V2 FORMATTED.\n");
                        os2_input_cooldown = 15;
                    }




                    if(selected_drive_idx != -1 && os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+150, wy+60, 80, 25)) {
                        win->cursor_pos = 0;

                        int drive_type = drives[selected_drive_idx].type;


                        if(drive_type == 2) {
                            active_sata_port = drives[selected_drive_idx].base_port;
                            ahci_identify((uint64_t)buf_mbr);
                            uint32_t lba_low = *(uint32_t*)(buf_mbr + 200);
                            drive_total_gb = lba_low / 2097152; if(drive_total_gb == 0) drive_total_gb = 1;
                        } else {
                            drive_total_gb = drives[selected_drive_idx].size_mb / 1024;
                            if(drive_total_gb == 0) drive_total_gb = 1;
                        }

                        for(int i=0; i<512; i++) ((char*)buf_mbr)[i] = 0;


                        disk_read_auto(0, (uint64_t)buf_mbr);
                        for(volatile int wait = 0; wait < 1000000; wait++) ;

                        uint8_t* boot = (uint8_t*)buf_mbr;
                        for(int i=0; i<28; i++) { cfs_files[i].exists = 0; cfs_files[i].parent_idx = 255; cfs_files[i].is_folder = 0; }
                        is_mounted = true; drive_used_kb = 0;


                        if(boot[3] == 'C' && boot[4] == 'F' && boot[5] == 'S') {
                            is_ntfs_drive = false;
                            for(int i=0; i<512; i++) ((char*)buf_dir)[i] = 0;

                            disk_read_auto(1002, (uint64_t)buf_dir);
                            for(volatile int wait2 = 0; wait2 < 1000000; wait2++) ;

                            CFS_DIR_ENTRY* dir = (CFS_DIR_ENTRY*)buf_dir;
                            for(int i=0; i<28; i++) {
                                if(dir[i].type != 0) {
                                    cfs_files[i].exists = 1;
                                    cfs_files[i].is_folder = (dir[i].type == 2) ? 1 : 0;
                                    cfs_files[i].parent_idx = dir[i].parent_idx;
                                    for(int n=0; n<11; n++) { char c = dir[i].filename[n]; cfs_files[i].name[n] = (c >= 32 && c <= 126) ? c : 0; }
                                    cfs_files[i].name[11] = 0;
                                    cfs_files[i].size = dir[i].file_size;
                                    cfs_files[i].start_lba = dir[i].start_lba;
                                    drive_used_kb += dir[i].file_size;
                                }
                            }
                            need_ui_refresh = true;
                            print_win(win, "\n[OK] OS2 CFS V2 MOUNTED.\n");
                        }

                        else if(boot[0] == 0x50 && boot[1] == 0x4B && boot[2] == 0x03 && boot[3] == 0x04) {
                            print_win(win, "\n[OK] ANDROID APK DETECTED.\n");
                        }

                        else {
                            uint8_t part_type = boot[446 + 4];
                            for(int i=0; i<28; i++) cfs_files[i].exists = 0;

                            int target_ntfs_lba = 0;
                            int target_fat32_lba = 0;

                            if(boot[510] == 0x55 && boot[511] == 0xAA) {
                                if(part_type == 0xEE) {
                                    print_win(win, "\n[OK] GPT DRIVE DETECTED.\n");
                                    disk_read_auto(1, (uint64_t)buf_dir);
                                    for(volatile int w = 0; w < 500000; w++) ;
                                    uint64_t table_lba = *(uint64_t*)(buf_dir + 72);

                                    disk_read_auto(table_lba, (uint64_t)buf_dir);
                                    for(volatile int w2 = 0; w2 < 500000; w2++) ;
                                    for(int p=0; p<4; p++) {
                                        uint64_t slba = *(uint64_t*)(buf_dir + (p * 128) + 32);
                                        if(slba > 0) {
                                            disk_read_auto(slba, (uint64_t)buf_mbr);
                                            for(volatile int w3 = 0; w3 < 200000; w3++) ;

                                            if (((uint8_t*)buf_mbr)[3]=='N' && ((uint8_t*)buf_mbr)[4]=='T') { target_ntfs_lba = slba; break; }

                                            if (((uint8_t*)buf_mbr)[82]=='F' && ((uint8_t*)buf_mbr)[83]=='A' && ((uint8_t*)buf_mbr)[84]=='T') { target_fat32_lba = slba; }
                                        }
                                    }
                                } else {
                                    for(int p=0; p<4; p++) {
                                        uint8_t pt = boot[446 + (p * 16) + 4];
                                        if(pt == 0x07 || (boot[3]=='N' && boot[4]=='T')) {
                                            target_ntfs_lba = *(int*)&boot[446 + (p * 16) + 8];
                                            break;
                                        } else if(pt == 0x0B || pt == 0x0C || (boot[82]=='F' && boot[83]=='A' && boot[84]=='T')) {
                                            target_fat32_lba = (boot[82]=='F') ? 0 : *(int*)&boot[446 + (p * 16) + 8];
                                            break;
                                        }
                                    }
                                }
                            }

                            if(target_ntfs_lba > 0) {
                                print_win(win, "\n[OK] NTFS VOLUME FOUND!\n");
                                is_ntfs_drive = true;
                                is_fat32_drive = false;

                                disk_read_auto(target_ntfs_lba, (uint64_t)buf_dir);
                                for(volatile int w = 0; w < 500000; w++) ;
                                uint8_t* vbr = (uint8_t*)buf_dir;
                                int sec_per_cluster = vbr[13];
                                uint64_t mft_cluster = *(uint64_t*)&vbr[48];
                                uint64_t mft_lba = target_ntfs_lba + (mft_cluster * sec_per_cluster);

                                uint8_t* mft_cache = new uint8_t[10*1024*1024];
                                print_win(win, "[SYS] CACHING MFT RECORDS (BULK)...\n");



                                int total_sectors = 50000 * 2;
                                int bulk_size = 128;
                                int port_no = drives[selected_drive_idx].base_port;

                                for(int offset = 0; offset < total_sectors; offset += bulk_size) {
                                    int remaining = total_sectors - offset;
                                    int chunk = (remaining < bulk_size) ? remaining : bulk_size;
                                    uint64_t lba = mft_lba + offset;
                                    uint64_t ram = (uint64_t)(mft_cache + (offset * 512));
                                    ahci_read_sectors(port_no, lba, chunk, ram);
                                    for(volatile int w1 = 0; w1 < 5000; w1++) ;
                                }
                                print_win(win, "[OK] CACHE READY! RAM SPEED UNLOCKED.\n");
                                need_ui_refresh = true;
                            } else if(target_fat32_lba > 0 || (boot[82]=='F' && boot[83]=='A' && boot[84]=='T')) {
                                print_win(win, "\n[OK] FAT32 VOLUME FOUND!\n");
                                is_fat32_drive = true;
                                is_ntfs_drive = false;

                                disk_read_auto(target_fat32_lba, (uint64_t)buf_dir);
                                for(volatile int w = 0; w < 500000; w++) ;

                                if (fat32_init(target_fat32_lba, (uint8_t*)buf_dir)) {
                                    active_fat32_folder_lba = fat32_root_lba;
                                    need_ui_refresh = true;
                                } else {
                                    print_win(win, "\n[ERR] FAT32 INIT FAILED!\n");
                                    is_fat32_drive = false;
                                }
                            } else { print_win(win, "\n[ERR] NO VALID NTFS/FAT32 FOUND.\n"); }
                        }

                        current_folder_id = 255;
                        dsk_mgr_opened = true;
                        os2_input_cooldown = 15;
                    }
    }
   }



            if(win->id == 0) {

                bool is_active = (os2_win_z[19] == win->id);


                uint32_t safe_txt_color = 0xFFFFFF;
                os2_Text(wx+15, wy+45, win->content, safe_txt_color, false);


                if(os2_win_z[19] == win->id && (os2_frame / 20) % 2 == 0) {
                    int cursor_off_x = 0; int cursor_off_y = 0;
                    for(int c_idx = 0; c_idx < win->cursor_pos; c_idx++) {
                        if(win->content[c_idx] == '\n') { cursor_off_y += 15; cursor_off_x = 0; }
                        else cursor_off_x += 6;
                    }
                    os2_DrawRoundedRect(wx + 15 + cursor_off_x, wy + 45 + cursor_off_y, 6, 10, 0, safe_txt_color);
                }


                os2_DrawRoundedRect(wx+ww-80, wy+15, 60, 20, 3, 0x005500); os2_TextC(wx+ww-50, wy+21, "SAVE", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-80, wy+15, 60, 20)) {
                    os2_str_cpy(win->title, "SAVING...");

                    uint32_t file_ram_addr = 0x09000000; char* file_data = (char*)file_ram_addr;
                    for(int i=0; i<512; i++) file_data[i] = 0;
                    for(int i=0; i < win->cursor_pos; i++) file_data[i] = win->content[i];

                    int test_lba = 500;
                    if(active_file_lba > 0) test_lba = active_file_lba;

                    if(disk_write_auto(test_lba, file_ram_addr)) {
                        os2_str_cpy(win->title, "NOTEPAD - SAVED!");
                    } else {
                        os2_str_cpy(win->title, "NOTEPAD - WRITE ERROR!");
                    }
                    os2_input_cooldown = 25;
                }


                os2_DrawRoundedRect(wx+ww-160, wy+15, 75, 20, 3, 0x444444);
                os2_TextC(wx+ww-122, wy+21, "SAVE AS", 0xFFFFFF, true);
                if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-160, wy+15, 75, 20)) {
                    os2_windows[2].open = true;
                    os2_windows[2].minimized = false;
                    os2_focus_window(2);
                    os2_save_step = 0;
                    os2_input_cooldown = 25;
                }
            }



            if(win->id == 2) {
                bool is_active = (os2_win_z[19] == win->id);

                uint64_t buf_dir = (uint64_t)global_buf_dir;
                uint32_t text_ram_addr = 0x09000000;
                if(os2_save_step == 0) {
                    os2_Text(wx+20, wy+40, "DESTINATION: ACTIVE MOUNTED DRIVE", 0x000000, true);
                    if(!is_mounted) {
                        os2_Text(wx+20, wy+70, "NO CFS DRIVE MOUNTED!", 0xFF0000, true);
                        os2_Text(wx+20, wy+90, "PLEASE OPEN DISK MGR AND MOUNT FIRST.", 0x555555, true);
                    } else {
                        os2_Text(wx+20, wy+70, "DRIVE IS READY. SELECT ACTION:", 0x00AA00, true);

                        os2_DrawRoundedRect(wx+20, wy+wh-40, 110, 25, 5, 0xCCCCCC);
                        os2_TextC(wx+75, wy+wh-32, "NEW FOLDER", 0x000000, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+20, wy+wh-40, 110, 25)) {
                            os2_save_step = 2; os2_input_cooldown = 25;
                        }

                        os2_DrawRoundedRect(wx+ww-110, wy+wh-40, 90, 25, 5, 0x555555);
                        os2_TextC(wx+ww-65, wy+wh-32, "NEXT (FILE)", 0xFFFFFF, true);
                        if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-110, wy+wh-40, 90, 25)) {
                            os2_save_step = 1; os2_input_cooldown = 25;
                        }
                    }
                } else if(os2_save_step == 1) {
                    os2_Text(wx+20, wy+40, "ENTER FILENAME (MAX 11 CHARS):", 0x000000, true);
                    os2_DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC);
                    os2_Text(wx+25, wy+65, os2_save_filename, 0x000000, true);
                    if((os2_frame/20)%2 == 0) os2_DrawChar(wx+25+(os2_save_name_idx*7), wy+65, '_', 0x000000, true);
                    os2_DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x00AA00);
                    os2_TextC(wx+ww-60, wy+wh-32, "SAVE", 0xFFFFFF, true);
                    if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) {

                        char* file_data = (char*)text_ram_addr;
                        for(int i=0; i<5120; i++) file_data[i] = 0;
                        for(int i=0; i < os2_windows[0].cursor_pos; i++) file_data[i] = os2_windows[0].content[i];

                        disk_read_auto(1002, buf_dir);
                        for(volatile int wait=0; wait<500000; wait++) ;

                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)buf_dir;
                        int slot = -1;
                        for(int i=0; i<28; i++) { if(entries[i].type == 0) { slot = i; break; } }
                        if(slot != -1) {
                            if (save_as_mode == 0) {



                                char* file_data = (char*)text_ram_addr;
                                for(int i=0; i<5120; i++) file_data[i] = 0;
                                for(int i=0; i < os2_windows[0].cursor_pos; i++) file_data[i] = os2_windows[0].content[i];

                                uint32_t target_sec = 4000 + slot;
                                disk_write_auto(target_sec, text_ram_addr);
                                for(volatile int wait=0; wait<500000; wait++) ;

                                entries[slot].file_size = 5120;
                                entries[slot].start_lba = target_sec;
                            } else {



                                extern uint8_t browser_download_buffer[];
                                extern int browser_download_len;

                                uint32_t payload_start = 0;
                                for(uint32_t i=0; i<browser_download_len-4; i++) {
                                    if (browser_download_buffer[i] == '\r' && browser_download_buffer[i+1] == '\n' &&
                                        browser_download_buffer[i+2] == '\r' && browser_download_buffer[i+3] == '\n') {
                                        payload_start = i + 4;
                                        break;
                                    }
                                }

                                uint32_t file_size = browser_download_len - payload_start;


                                uint32_t target_sec = 11000;
                                for (int i = 0; i < 28; i++) {
                                    if (cfs_files[i].exists && cfs_files[i].start_lba >= target_sec) {
                                        target_sec = cfs_files[i].start_lba + ((cfs_files[i].size + 511) / 512) + 1;
                                    }
                                }


                                uint32_t sectors = (file_size + 511) / 512;
                                for (uint32_t s=0; s<sectors; s++) {
                                    disk_write_auto(target_sec + s, (uint64_t)&browser_download_buffer[payload_start + s*512]);
                                    for(volatile int w=0; w<2000; w++) ;
                                }

                                entries[slot].file_size = file_size;
                                entries[slot].start_lba = target_sec;
                            }




                            entries[slot].type = 1;
                            for(int n=0; n<11; n++) entries[slot].filename[n] = 0;
                            os2_str_cpy(entries[slot].filename, os2_save_filename);


                            entries[slot].parent_idx = current_folder_id;

                            disk_write_auto(1002, buf_dir);
                            for(volatile int wait2=0; wait2<500000; wait2++) ;


                            cfs_files[slot].exists = 1;
                            cfs_files[slot].is_folder = 0;
                            cfs_files[slot].parent_idx = current_folder_id;
                            os2_str_cpy(cfs_files[slot].name, os2_save_filename);
                            cfs_files[slot].size = entries[slot].file_size;
                            cfs_files[slot].start_lba = entries[slot].start_lba;

                            active_file_lba = entries[slot].start_lba;
                            active_file_idx = slot;

                            os2_str_cpy(os2_windows[0].title, "SAVED SUCCESS!");
                            save_as_mode = 0;
                        } else {
                            os2_str_cpy(os2_windows[0].title, "ERR: ROOT FULL!");
                        }
                        win->open = false;
                        os2_input_cooldown = 25;
                    }
                } else if(os2_save_step == 2) {
                    os2_Text(wx+20, wy+40, "FOLDER NAME (MAX 11 CHARS):", 0x000000, true);
                    os2_DrawRoundedRect(wx+20, wy+60, ww-40, 25, 2, 0xCCCCCC);
                    os2_Text(wx+25, wy+65, os2_new_folder_name, 0x000000, true);
                    if((os2_frame/20)%2 == 0) os2_DrawChar(wx+25+(os2_folder_name_idx*7), wy+65, '_', 0x000000, true);
                    os2_DrawRoundedRect(wx+ww-100, wy+wh-40, 80, 25, 5, 0x0055AA);
                    os2_TextC(wx+ww-60, wy+wh-32, "CREATE", 0xFFFFFF, true);
                    if(os2_input_cooldown == 0 && mouse_just_pressed && is_active && os2_is_over_rect(mouse_x, mouse_y, wx+ww-100, wy+wh-40, 80, 25)) {

                        disk_read_auto(1002, buf_dir);
                        for(volatile int wait=0; wait<500000; wait++) ;
                        CFS_DIR_ENTRY* entries = (CFS_DIR_ENTRY*)buf_dir;
                        int slot = -1;
                        for(int i=0; i<28; i++) { if(entries[i].type == 0) { slot = i; break; } }
                        if(slot != -1) {

                            entries[slot].type = 2;
                            for(int n=0; n<11; n++) entries[slot].filename[n] = 0;
                            os2_str_cpy(entries[slot].filename, os2_new_folder_name);
                            entries[slot].file_size = 0;
                            entries[slot].start_lba = 0;


                            entries[slot].parent_idx = current_folder_id;

                            disk_write_auto(1002, buf_dir);
                            for(volatile int wait2=0; wait2<500000; wait2++) ;


                            cfs_files[slot].exists = 1;
                            cfs_files[slot].is_folder = 1;
                            cfs_files[slot].parent_idx = current_folder_id;
                            os2_str_cpy(cfs_files[slot].name, os2_new_folder_name);
                            cfs_files[slot].size = 0;
                            cfs_files[slot].start_lba = 0;

                            os2_str_cpy(os2_windows[0].title, "FOLDER CREATED!");
                        } else {
                            os2_str_cpy(os2_windows[0].title, "ERR: ROOT FULL!");
                        }
                        win->open = false;
                        os2_input_cooldown = 25;
                    }
                }
            }



            if(win->id == 7) {
                uint32_t dbg_color = 0xFF5555;
                os2_Text(wx+15, wy+45, win->content, dbg_color, false);

                static uint64_t fps_last_time = 0;
                static int fps_frames = 0;
                static int fps_current = 0;
                fps_frames++;
                if (os2_system_ticks - fps_last_time >= 1000) {
                    fps_current = fps_frames;
                    fps_frames = 0;
                    fps_last_time = os2_system_ticks;
                }
                char fps_buf[32];
                os2_str_cpy(fps_buf, "FPS: ");
                int_to_str(fps_current, fps_buf + 5);
                os2_Text(wx+15, wy+75, fps_buf, 0x00FF00, true);
            }




            if(win->id == 5) {
                uint32_t cmd_color = 0x00FF00;
                os2_Text(wx+15, wy+45, win->content, cmd_color, false);

                int lines = 0; for(int i=0; i<win->cursor_pos; i++) { if(win->content[i] == '\n') lines++; }
                int prompt_y = wy + 45 + (lines * 15);

                os2_Text(wx+15, prompt_y, "C:\\> ", cmd_color, true);
                os2_Text(wx+55, prompt_y, cmd_input_buf, cmd_color, true);

            }
        }
        os2_DrawAeroCursor(mouse_x, mouse_y);


        if (os2_app_window_active) {
            uint32_t* overlay = (uint32_t*)app_overlay_buffer;
            for (int i = 0; i < screen_w * screen_h; i++) {
                if (overlay[i] != 0) {
                    bb[i] = overlay[i];
                    overlay[i] = 0;
                }
            }
        }


        key_new = false;
        last_app_key = 0;
        mouse_just_pressed = false;

        os2_Swap();

        usleep(16000);

        os2_frame++;
    }
}
