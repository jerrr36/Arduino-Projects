package main

import (
	"time"
)

func main() {
	thermo := max6675.Device()
	thermo.Configure()

	for {
		temp := thermo.readCelsius()
		println(temp)
		time.Sleep(time.Second * 1)
	}

}
