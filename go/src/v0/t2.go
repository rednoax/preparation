package main
import "fmt"
func mxx() {
	var a = 1 //to make the final exec-elf 's `readelf -s` contains mxx()
	fmt.Println("mxx", a)
}