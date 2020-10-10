package max6675

import (
	"machine"
	"time"
	"errors"
)

//Device struct
type Device struct {
	SCK machine.Pin
	CS  machine.Pin
	SO  machine.Pin
}

//Var for no thermocouple error
NOTC := errors.New("No Thermocouple connected")


//New creates a new tc struct 
func New(sck machine.Pin, cs machine.Pin, so machine.Pin) Device {
	return Device{sck, cs, so}

}
//Configure MAX6675
func (d *Device) Configure() {

	d.CS.High()
}

//spiRead reads 8 bits from the max6675 chip. Not exported
func (d *Device) spiRead() uint16 {
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
func (d *Device) ReadCelsius() (uint16, error) {
	var temp uint16

	d.CS.Low()
	time.Sleep(time.Microsecond * 10)

	temp |= d.spiRead()
	temp <<= 8
	temp |= d.spiRead()

	d.CS.High()

	if temp == 0x4 {
		err := "No thermocouple connected"
		return 0, NOTC
	}

	temp >>= 3

	//Not working properly
	//t := float64(temp) * .25

	return temp, nil
}

//ReadFarhenheit calls ReadCelsius and converts to farhenheit. Need to fix float issue for it to work
func (d *Device) ReadFarhenheit() (float64, error) {
	t, err := d.ReadCelsius()
	return t * 9.0 / 5.0 + 32, err
}

//ReadKelvin calls ReadCelsius and converts to kelvin. Need to fix float issue for it to work
func (d *Device) ReadKelvin() (float64, error) {
	t, err := d.ReadCelsius()
	return t + 273.15, err
}
