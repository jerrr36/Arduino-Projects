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

func (t Timer) buttonPress() {
	b := t.button
	b.Configure(machine.PinConfig{Mode: machine.PinInput})

	for {
		val := b.Get()
		if val == false {
			t.pizzaTimer()
		}
	}
}

//Timer method
func (t Timer) pizzaTimer() {

	l := t.timerOn
	l.Configure(machine.PinConfig{Mode: machine.PinOutput})
	l2 := t.timerDone
	l2.Configure(machine.PinConfig{Mode: machine.PinOutput})

	l.High()
	time.Sleep(time.Second * 5)
	l.Low()
	l2.High()
	time.Sleep(time.Second * 2)
	l2.Low()
	return
}

func main() {

	timer1 := Timer{machine.D2, machine.D3, machine.D4}
	timer2 := Timer{machine.D5, machine.D6, machine.D7}

	go timer1.buttonPress()
	go timer2.buttonPress()

	for {
		time.Sleep(time.Millisecond * 1000)
	}

}
