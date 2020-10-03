//Created by: Jeremiah Hoydich 
//Created on: 10/3/20
//Last Edited: 10/3/20
//Purpose:
//Exploration into minimizing the hardware necessary for an autonomous boat.
//The idea is that using go routines and channels, you can efficiently get all sensor data, namely gps
//gps data usually causes a bottle neck, so running it concurrently would allow for more hardware to be
//on one microcontroller, namely an Arduino Mega

package main

import (
	"machine"
	"time"
	"math"
)

//Radius of earth
const (
	R 6371000
)
type gpsData struct {
	lat float64
	lon float64
	//figure out correct time var/struct
	t	float64


}
func main() {

	//just adding this var and loop to ensure that libraries stay
	_ := machine.D1

	for {

		time.Sleep(time.Microsecond * 50)

	}
}
func getGPS() float64 float64 {
	//Get gps 
	c := make(chan)

	/*
		get gps data from gps
		put data into struct

	*/

}


//Maybe make receive from a channel
func gpsMath(lat float64, lon float64, destLat float64, destLon float64) float64 float64 {
	//get data from channel
	
	
	deltaLat := destLat - lat
	deltaLon := destLon - lon
	
	//distance calculation
	a := math.Sin(deltaLat / 2)**2 + math.Cos(destLat) * math.Cos(lat)
	c := 2 * math.Atan2(math.Sqrt(a), math.Sqrt(1 - a))
	dist := c * R


	//Calculate bearing angle for servo motor
	x := math.Cos(lat) * math.Sin(deltaLon)
	y := math.Cos(destLat) * math.Sin(lat) - math.Sin(destLat) * math.Cos(lat) * math.Cos(deltaLon)
	B := math.Atan2(x, y) * 180  math.Pi
	return dist, B
}
