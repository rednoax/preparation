#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
/*
This pragma declares symbol to be weak, as if the declaration had the attribute
of the same name. The pragma may appear before or after the declaration of
symbol. It is not an error for symbol to never be defined at all
*/
/*
when there is no pragma for _gcry_vcontrol:
61: 00000000000006aa   123 FUNC    GLOBAL DEFAULT   14 _gcry_vcontrol
when there is pragma for _gcry_vcontrol
61: 00000000000006aa   123 FUNC    WEAK   DEFAULT   14 _gcry_vcontrol
*/
#pragma weak _gcry_vcontrol
#pragma weak _NoSuchFunc//It is not an error for symbol to never be defined at all, but if you use it like printf("%p", _NoSuchFunc) then undeclared error will happen
#if !PTHREAD_IN_USE_DETECTION_HARD
#warning PTHREAD_IN_USE_DETECTION_HARD not defined
#define use_pthread_p0() (!!_NoSuchFunc)//if use_pthread_p0 is not used, there will be no error. undeclared error otherwise, eg printf("%d", use_pthread_p0)!
#define use_pthread_p1() (!!_gcry_vcontrol)
#endif
typedef enum {
	GPG_ERR_NO_ERROR = 0,
} gpg_err_code_t;

typedef gpg_err_code_t gcry_err_code_t;
//enum varaibale name seems useless
enum gcry_log_levels {
	GCRY_LOG_CONT	= 0,
	GCRY_LOG_INFO	= 10,
};

enum gcry_ctl_cmds {
	GCRYCTL_SET_VERBOSITY	= 19,
};

gcry_err_code_t _gcry_vcontrol(enum gcry_ctl_cmds cmd, va_list args)
{
	int level = va_arg(args, int);
	printf("%s:%d\n", __func__, level);
	return GPG_ERR_NO_ERROR;
	//return -1;//no compiler error to return a non gcry_err_code_t! They are all int actually?
	//return GCRYCTL_SET_VERBOSITY;//no compiler error to return a non gcry_err_code_t! They are all int actually?
}

unsigned int gcry_control(enum gcry_ctl_cmds cmd, ...)
{
	va_list args;
	va_start(args, cmd);
	_gcry_vcontrol(cmd, args);
	va_end(args);
	return 0;
}

int main()
{
	printf("GCRY_LOG_CONT:%d\n", GCRY_LOG_CONT);
	printf("GCRY_LOG_INFO:%d\n", GCRY_LOG_INFO);
	//gcry_control(GCRYCTL_SET_VERBOSITY);//ok
	//gcry_control(GCRY_LOG_CONT);//it's ok too!
	gcry_control(GCRYCTL_SET_VERBOSITY, -10);
	printf("pthread_mutex_t: %ld B\n", sizeof(pthread_mutex_t));//40 B
	//printf("%p\n", _NoSuchFunc);//error: '_NoSuchFunc' undeclared (first use in this function)
	printf("%p\n", _gcry_vcontrol);//print function address like 0x55de40b786aa
	//printf("%d\n", use_pthread_p0());//enum.c:19:29: error: '_NoSuchFunc' undeclared (first use in this function)
	printf("%d\n", use_pthread_p1());//1
	return 0;
}