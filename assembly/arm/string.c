void printascii(char *);

void test0()
{
    printascii('360');//'360':(0x33<<16)+(0x36<<8)+0x30=3356208, r0==3356208
//the following can't print string "MHZ"!
    printascii('MHZ');//('M'<<16)+('H'<<8)+('Z')=5064794, r0=5064794, ie 0x4d485a in range [0,+128M) that is undocumented
}

void test1()
{
    printascii("MHZ");//r0:an address holding .asciz "MHZ"
}