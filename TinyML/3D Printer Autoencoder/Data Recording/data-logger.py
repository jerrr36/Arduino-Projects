import serial

ser = serial.Serial('COM8', 9600)
with open("data.csv", 'w') as csv:
    while True:
        line = str(ser.readline())
        line = line[2:-5]
        csv.write(line+"\n")
        

        
