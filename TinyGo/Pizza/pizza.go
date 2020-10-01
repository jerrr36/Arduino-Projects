package main

import (
	"machine"
	"time"
)

//Timer button
type Timer struct {
	timerOn   machine.Pin
	timerDone machine.Pin
	button    machine.Pin
}

//function that scans for button press and starts appropriate timer
func buttonPress(t Timer) {
	//creating button
	b := t.button
	b.Configure(machine.PinConfig{Mode: machine.PinInput})

	for {

		//getting bool value of button
		val := b.Get()

		//delay to allow both goroutines to function properly
		time.Sleep(time.Millisecond * 50)

		//condition to start timers
		if val == false {
			pizzaTimer(t)

		}
	}
}

//Timer function
func pizzaTimer(t Timer) {

	//configuring leds
	l := t.timerOn
	l.Configure(machine.PinConfig{Mode: machine.PinOutput})
	l2 := t.timerDone
	l2.Configure(machine.PinConfig{Mode: machine.PinOutput})

	//leds on timers for baking and done
	l.High()
	time.Sleep(time.Second * 5)
	l.Low()
	l2.High()
	time.Sleep(time.Second * 2)
	l2.Low()

}

func main() {

	timer1 := Timer{machine.D2, machine.D3, machine.D4}
	timer2 := Timer{machine.D5, machine.D6, machine.D7}

	go buttonPress(timer1)
	go buttonPress(timer2)

	for {
		time.Sleep(time.Millisecond * 1000)
	}

}
