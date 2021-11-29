package main
//package main1//`go run t1.go`: go run: cannot run non-main package
import (
	_"fmt"//			/usr/lib/go-1.13/pkg/linux_amd64/fmt.a: current ar archive
	//"os/exec"//	/usr/lib/go-1.13/pkg/linux_amd64/os/exec.a
/*"mod":t1.go:6:2: cannot find package "mod" in any of:
        /usr/lib/go-1.13/src/mod (from $GOROOT)
        /home/rednoah/go/src/mod (from $GOPATH)
SO A RELATIVE IMPORT w/t any prefix means relative to GOROOT & GOPATH
*/
/*"/mod":import "/mod": cannot import absolute path*/
/*"./main2":<=even mod/t2.go is of 'package main2', the imported name here should be dir name real existing
t1.go:12:2: cannot find package "." in:
        /home/rednoah/opt/preparation/go/src/v2/main2
*/
	"./mod"//ok
)
func main() {
/*mxx();//no package name
# _/home/rednoah/opt/preparation/go/src/var
./t1.go:10:2: imported and not used: "_/home/rednoah/opt/preparation/go/src/var/mod" as main2<==as main2! not mod!
./t1.go:15:2: undefined: mxx*/
/*main2.mxx();//package is ok but unexported error
# _/home/rednoah/opt/preparation/go/src/var
./t1.go:20:2: cannot refer to unexported name main2.mxx
./t1.go:20:2: undefined: main2.mxx*/
/*mod.mxx();//wrong package name
# _/home/rednoah/opt/preparation/go/src/var
./t1.go:10:2: imported and not used: "_/home/rednoah/opt/preparation/go/src/var/mod" as main2
./t1.go:23:2: undefined: mod
*/
	main2.Mxx();
}
