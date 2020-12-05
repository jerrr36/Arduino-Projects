package main

import (
	"machine"
	"time"

	"tinygo.org/x/drivers/easystepper"
)

func main() {
	motor := easystepper.New(machine.D2, machine.D4, machine.D3, machine.D5, 2048, 15)
	motor.Configure()

	for x := 0; x < 512; x++ {
		motor.Move(512)
		time.Sleep(time.Millisecond * 17)
	}

	motor.Off()

}
