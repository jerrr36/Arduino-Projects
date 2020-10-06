package max6675

import (
	"machine"
	"time"
)

//Device struct
type Device struct {
	SCK machine.Pin
	CS  machine.Pin
	SO  machine.Pin
}


//New creates a new tc struct 
func New(sck machine.Pin, cs machine.Pin, so machine.Pin) Device {
	return Device{sck, cs, so}

}
//Configure MAX6675
func (d Device) Configure() {

	d.CS.High()
}

//SpiRead reads 8 bits from the max6675 chip
func (d Device) SpiRead() uint16 {
	var i int
	var b uint16 = 0
	for i = 7; i >= 0; i-- {
		d.SCK.Low()
		time.Sleep(time.Microsecond * 10)
		if d.SO.Get() {
			b |= (1 << i)
		}
		d.SCK.High()
		time.Sleep(time.Microsecond * 10)
	}

	return b
}

//ReadCelsius Function to get temp in celsius
func (d Device) ReadCelsius() uint16 {
	var temp uint16

	d.CS.Low()
	time.Sleep(time.Microsecond * 10)

	temp |= d.spiRead()
	temp <<= 8
	temp |= d.spiRead()

	d.CS.High()

	if temp == 0x4 {
		return 0
	}

	temp >>= 3

	//t := float64(temp) * .25

	return temp
}

//ReadFarhenheit calls ReadCelsius and converts to farhenheit
func ReadFarhenheit() {
	return ReadCelsius() * 9.0 / 5.0 + 32
}

//ReadKelvin calls ReadCelsius and converts to kelvin
func ReadKelvin() {
	return ReadCelsius() + 273.15
}