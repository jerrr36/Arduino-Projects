package max6675

import (
	"machine"
)

//Device struct
type Device struct {
	
	SCK machine.Pin
	CS  machine.Pin
	SO  machine.Pin
}

//Configure MAX6675
func (d Device) Configure() {
	d.SCK.Configure(machine.PinConfig{Mode: machine.PinOutput})
	d.CS.Configure(machine.PinConfig{Mode: machine.PinOutput})
	d.SO.Configure(machine.PinConfig{Mode: machine.PinInput})

	d.CS.High()
}
func (d Device) spiRead() byte {
	var i int
	byte d = 0
	for i = 7; i >= 0; i-- {
		d.SCK.Low()
		time.Sleep(time.Microsecond * 10)
		if (d.SO.Get()) {
			d |= (1 << i)
		}
		d.SCK.High()
		time.Sleep(time.Microsecond * 10)
	}

	return d
}
func (d Device) readCelsius() float64 {
	var temp uint16
	
	d.CS.Low()
	time.Sleep(time.Microsecond * 10)
	temp |= d.spiRead()
	temp <<= 8
	temp |= d.spiRead()

	d.CS.High()

	if temp & 0x4 {
		return nil
	}

	temp >>= 3

	return temp * .25

}
