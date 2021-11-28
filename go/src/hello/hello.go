/*
note:
Why package is of name 'main'
$(pwd) is /home/rednoah/opt/preparation/go/src/hello/helloworld.go. There
is no any directory or file of name 'main'. So the cause is not related to
directory or file name.
a. for 'package main':
The real reason to use 'main' as package name is the 'func main' defind in
THIS FILE(当前文件中有main函数定义,则package的名称要使用main,package的名称与当前
文件名没有关系);
b. for 'package non_main'
v2/mod shows the library souce .go's package name xx in 'package xx' needs not to be == the dir
name holding .go. Here the dir name is 'mod', which!='main2' in v2/mod/t2.go's 'package main2'.
But by convention, they'd better be the same. package under /use/share/go-1.13/src conforms the
convention.
---
https://go.dev/doc/gopath_code#PackageNames
1. the 1st statement in a Go source file must be
package name
2. executable commands must AL use package main
*/
package main
import "fmt"
func main() {
	fmt.Print("hello world!\n")
}
