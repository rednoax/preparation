#include <stdio.h>
#include <stdarg.h>

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
	return 0;
}