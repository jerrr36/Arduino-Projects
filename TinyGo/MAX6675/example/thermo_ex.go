package main

import (
	"machine"
	"max6675"
	"time"
)

func main() {
	thermo := max6675.New(machine.D13, machine.D10, machine.D12)
	thermo.Configure()

	for {
		temp, err := thermo.ReadTemperature()
		if err != nil {
			println(err)
		} else {
			//Off by factor of 4. Issue with floats
			println(temp)
		}

		time.Sleep(time.Second * 1)
	}

}
