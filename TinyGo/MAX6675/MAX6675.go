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

//ReadTemperature Function either returns the temperature in millidegrees of celsius or an error
func (d *Device) ReadTemperature() (uint32, error) {
	var data uint16

	d.CS.Low()
	time.Sleep(time.Microsecond * 10)

	data |= d.spiRead()
	data <<= 8
	data |= d.spiRead()

	d.CS.High()

	if data == 0x4 {
	
		return 0, NOTC
	}

	data >>= 3

	//Not working properly
	temp := uint32(data) * 250

	return temp, nil
}

