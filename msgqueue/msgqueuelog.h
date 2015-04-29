#ifndef MSGQUEUELOG_H
#define MSGQUEUELOG_H
int initqueue(key_t key);
int msgprintf(int pid, char *fmt, ...);
int msgwrite(int pid, void *buf, int len);
#endif