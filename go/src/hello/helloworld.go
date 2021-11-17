/*
note:
Why package is of name 'main'
$(pwd) is /home/rednoah/opt/preparation/go/src/hello/helloworld.go. There
is no any directory or file of name 'main'. So the cause is not related to
directory or file name.
The real reason to use 'main' as package name is the 'func main' defind in
THIS FILE(当前文件中有main函数定义,则package的名称要使用main,package的名称与当前
文件名没有关系)
*/
package main
import "fmt"
func main() {
	fmt.Print("hello world!\n")
}
