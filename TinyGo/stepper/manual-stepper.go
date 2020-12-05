package main

import (
	"machine"
	"time"
)

// Stepper device
type Device struct {
	pins  [4]machine.Pin
	rpm   int32
	steps int32
	delay int32
}

// Creating a new stepper device
func New(pin1, pin2, pin3, pin4 machine.Pin, rpm int32, steps int32) Device {
	return Device{
		pins:  [4]machine.Pin{pin1, pin2, pin3, pin4},
		rpm:   rpm, // There are limits based on different motors
		steps: steps,
		delay: 60000 / (rpm * steps),
	}

}

//Configure initializes the pins on the new device
func (d *Device) Configure() {
	for _, pin := range d.pins {
		pin.Configure(machine.PinConfig{Mode: machine.PinOutput})
	}
}

func main() {
	motor := New(machine.D8, machine.D9, machine.D10, machine.D11, 1, 2048)
	motor.Configure()

	motor.Step(512)
	time.Sleep(time.Second * 3)
	motor.Step(-512)

	motor.Off()
}

//Step takes in an integer for # of steps and turns the motor that number
func (d *Device) Step(s int) {

	if s > 0 {
		for x := 0; x < s; x++ {

			a := x % 4
			switch a {
			case 0:
				d.pins[0].High()
				d.pins[1].Low()
				d.pins[2].High()
				d.pins[3].Low()

			case 1:
				d.pins[0].Low()
				d.pins[1].High()
				d.pins[2].High()
				d.pins[3].Low()
			case 2:
				d.pins[0].Low()
				d.pins[1].High()
				d.pins[2].Low()
				d.pins[3].High()
			case 3:
				d.pins[0].High()
				d.pins[1].Low()
				d.pins[2].Low()
				d.pins[3].High()
			}
			time.Sleep(time.Millisecond * time.Duration(d.delay))
		}
	} else {
		s = -1 * s
		for x := 0; x < s; x++ {

			a := (x + 2*(x%2)) % 4
			switch a {
			case 0:
				d.pins[0].High()
				d.pins[1].Low()
				d.pins[2].High()
				d.pins[3].Low()

			case 1:
				d.pins[0].Low()
				d.pins[1].High()
				d.pins[2].High()
				d.pins[3].Low()
			case 2:
				d.pins[0].Low()
				d.pins[1].High()
				d.pins[2].Low()
				d.pins[3].High()
			case 3:
				d.pins[0].High()
				d.pins[1].Low()
				d.pins[2].Low()
				d.pins[3].High()
			}
			time.Sleep(time.Millisecond * time.Duration(d.delay))
		}
	}

}

//Off removes power from coils
func (d *Device) Off() {
	for _, pin := range d.pins {
		pin.Low()
	}
}
