#include <stdio.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
/*
based on kernel typecheck.h
*/
#define typecheck(type,x) \
({	type dummy; \
	typeof(x) dummy2; \
	(void)(&dummy == &dummy2); \
	1; \
})

/* The emitted message can be:

1. &dummy == &dummy2
typecheck.c: In function 'main':
typecheck.c:5:2: warning: statement with no effect [-Wunused-value]
  (&dummy == &dummy2); \
  ^
typecheck.c:16:2: note: in expansion of macro 'typecheck'
  typecheck(unsigned long, flag);
  ^

2. void(&dummy == &dummy2);
typecheck.c: In function 'main':
typecheck.c:5:7: error: expected identifier or '(' before '&' token
  void(&dummy == &dummy2); \
       ^
typecheck.c:23:2: note: in expansion of macro 'typecheck'
  typecheck(unsigned long, flag);
  ^
typecheck.c:4:12: warning: unused variable 'dummy2' [-Wunused-variable]
  typeof(x) dummy2; \
            ^
typecheck.c:23:2: note: in expansion of macro 'typecheck'
  typecheck(unsigned long, flag);
  ^
typecheck.c:3:9: warning: unused variable 'dummy' [-Wunused-variable]
 ({ type dummy; \
         ^
typecheck.c:23:2: note: in expansion of macro 'typecheck'
  typecheck(unsigned long, flag);
  ^
*/

/*
Lesson 11: Typecasting in C and C++
http://www.cprogramming.com/tutorial/lesson11.html
...
To typecast something, simply put the type of variable you want the actual variable to act as inside parentheses in front of the actual variable. (char)a will make 'a' function as a char. 
...
The typecast described above is a C-style cast, C++ supports two other types. First is the function-style cast:

int main()       
{
  cout<< char ( 65 ) <<"\n"; 
  cin.get();
}

This is more like a function call than a cast as the type to be cast to is like the name of the function and the value to be cast is like the argument to the function. 
*/
int main()
{
	/*
There  are  two  ways  of  writing  the  argument  to  typeof: with  an  expression  or  with  a type.  
	*/
	/*
	1. error of "(int *[4]) dummy;"
typecheck.c: In function 'main':
typecheck.c:67:13: error: 'dummy' undeclared (first use in this function)
  (int *[4]) dummy;
             ^
typecheck.c:67:13: note: each undeclared identifier is reported only once for each function it appears in 
	2. error of "int*[4] dummy;"
typecheck.c: In function 'main':
typecheck.c:78:6: error: expected identifier or '(' before '[' token
  int*[4] dummy;
      ^

	*/
	typeof(int*[4]) array;//absolutely ok!!!
	typeof(array[0]) p;
	int i = 0x8086;
	unsigned long flag;
	typecheck(unsigned long, flag);
	/*
	flag = unsigned long(i);
typecheck.c: In function 'main':
typecheck.c:50:9: error: expected expression before 'unsigned'
  flag = unsigned long(i);
         ^
typecheck.c:47:6: warning: unused variable 'i' [-Wunused-variable]
  int i = 0x8086;
      ^
	*/
	flag = (unsigned long)i;//no erro, ok!
	/* gcc -Wall will emit:
typecheck.c: In function 'main':
typecheck.c:8:16: warning: comparison of distinct pointer types lacks a cast [enabled by default]
  (void)(&dummy == &dummy2); \
                ^
typecheck.c:61:2: note: in expansion of macro 'typecheck'
  typecheck(unsigned long, i);
  ^
	*/
	//typecheck(unsigned long, i);//uncomment it to see warning!!!
	printf("array size %d, element number %d\n", sizeof(array), ARRAY_SIZE(array));
	typecheck(int*, p);//ok
	typecheck(typeof(int*[4]), array);//absolutely ok!!!
	/*
	typecheck((int*[4]), array);
typecheck.c: In function 'main':
typecheck.c:7:9: error: 'dummy' undeclared (first use in this function)
 ({ type dummy; \
         ^
typecheck.c:110:2: note: in expansion of macro 'typecheck'
  typecheck((int*[4]), array);
  ^
typecheck.c:7:9: note: each undeclared identifier is reported only once for each function it appears in
 ({ type dummy; \
         ^
typecheck.c:110:2: note: in expansion of macro 'typecheck'
  typecheck((int*[4]), array);
  ^
	*/
	/*
	typecheck(int*[4], array);
typecheck.c: In function 'main':
typecheck.c:126:16: error: expected identifier or '(' before '[' token
  typecheck(int*[4], array);
                ^
typecheck.c:7:4: note: in definition of macro 'typecheck'
 ({ type dummy; \
    ^
typecheck.c:9:10: error: 'dummy' undeclared (first use in this function)
  (void)(&dummy == &dummy2); \
          ^
typecheck.c:126:2: note: in expansion of macro 'typecheck'
  typecheck(int*[4], array);
  ^
typecheck.c:9:10: note: each undeclared identifier is reported only once for each function it appears in
  (void)(&dummy == &dummy2); \
          ^
typecheck.c:126:2: note: in expansion of macro 'typecheck'
  typecheck(int*[4], array);
  ^
	*/
	/*
Another way to refer to the type of an expression is with  typeof. The syntax of using of this keyword looks like sizeof, but the construct acts semantically like a type name defined with typedef. 
	*/
	/*
	absolutely ok to do the following, typeof(array) acts like a type name defined with typedef!
A  typeof  construct  can  be  used  anywhere  a  typedef  name  can  be  used.For  example, you can use it in a declaration, in a cast, or inside of  sizeof or typeof. 
	*/
	typecheck(typeof(array), array);
	/*
If you are writing a header file that must work when included in ISO C programs, write __typeof__ instead of typeof. 
	*/
	typecheck(__typeof__(int*), p);
	return 0;
}
