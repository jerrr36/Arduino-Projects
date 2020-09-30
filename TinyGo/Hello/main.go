package main

import (
	"machine"
	"time"
)

func main() {
	go blink()

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
	time.Sleep(time.Millisecond * 10000)
}
