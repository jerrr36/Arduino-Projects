package main

import (
	"machine"
	"time"
)

func main() {
	go blink()

	//for loop keeps main() from ending this allows go routine to continue indefinitely
	for {
		time.Sleep(time.Millisecond * 1000)
	}

}

func blink() {

	led := machine.LED
	led.Configure(machine.PinConfig{Mode: machine.PinOutput})

	for {
		led.Low()
		delay()
		led.High()
		delay()

	}

}

func delay() {
	time.Sleep(time.Millisecond * 1000)
}
