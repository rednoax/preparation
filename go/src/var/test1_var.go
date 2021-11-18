package main
//package main1//cannot run non-main package
import (
	"fmt"//			/usr/lib/go-1.13/pkg/linux_amd64/fmt.a: current ar archive
	//"os/exec"//	/usr/lib/go-1.13/pkg/linux_amd64/os/exec.a
)
func main() {
	var a int = 3498
	fmt.Println("a = ", a)
}
