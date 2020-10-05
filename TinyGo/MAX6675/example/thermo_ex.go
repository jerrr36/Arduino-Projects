package main

import (
	"machine"
	"max6675"
	"time"
)

func main() {
	thermo := max6675.Device{machine.D52, machine.D53, machine.D50}
	thermo.Configure()

	for {
		temp := thermo.ReadCelsius()

		println(temp)

		time.Sleep(time.Second * 1)
	}

}
