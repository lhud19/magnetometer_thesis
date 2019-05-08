""""

Luke Hudspeth
Thesis Project: Evaluating the Efficacy of Magnetometer Based Parking Sensors
Data Collection Suite
Advisor: Timothy Pierson



referenced: https://www.datacamp.com/community/tutorials/gui-tkinter-python#GM
GUI Code adapted from example found: https://pythonspot.com/tk-dropdown-example/
threading example adapted from: https://stackoverflow.com/questions/23100704/running-infinite-loops-using-threads-in-python


"""


from Tkinter import *
import Tkinter as ttk
from ttk import *
from serial import *
from time import *
from threading import Thread
import datetime
import csv


def collect_data(arduino, tkvar1, tkvar2, tkvar3, tkvar4, tkvar5, tkvar6, e1):
    data_instance = data_holder(arduino, tkvar1, tkvar2, tkvar3, tkvar4, tkvar5, tkvar6, e1)
    i = 1
    sleep(2)
    while i != 10:
        sleep(0.15)
        array = arduino.line.split()
        sen_num = str(i)
        if array[0][1] == sen_num:
            print "sens " + sen_num + "got!"
            data_instance.data.append(array[1])
            data_instance.data.append(array[2])
            data_instance.data.append(array[3])
            i += 1
    append_data("data_5_8.csv", data_instance.data)
    return data_instance.data





class run_Aruino(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.daemon = True
        self.start()
        self.line = ""

        # Below 32 everything in ASCII is gibberish
    def run(self):
        counter = 32
        ser = Serial('/dev/tty.usbserial-14120', 9600)  # Establish the connection on a specific port
        while True:
                counter += 1
                ser.write(str(chr(counter)))  # Convert the decimal number to ASCII then send it to the Arduino
                self.line = ser.readline()  # Read the newest output from the Arduino
                #print self.line
                sleep(.1)  # Delay for one tenth of a second
                if counter == 255:
                    counter = 32


class data_holder:
    def __init__(self, arduino, tkvar1, tkvar2, tkvar3, tkvar4, tkvar5, tkvar6, e1):
        self.arduino = arduino
        self.spot_1 = tkvar1
        self.spot_2 = tkvar2
        self.spot_3 = tkvar3
        self.spot_4 = tkvar4
        self.spot_5 = tkvar5
        self.spot_6 = tkvar6
        self.spot_num = e1
        time = str(datetime.datetime.now())
        self.data = []
        self.data = [time, self.spot_num.get(), self.spot_1.get(), self.spot_2.get(), self.spot_3.get(), self.spot_4.get(), self.spot_5.get(), self.spot_6.get()]







def main():

    #create_csv()
    root = Tk()
    root.title("Data Collection")



    topframe=Frame(root).pack()
    # Add a grid
    bottomframe = Frame(root)
    bottomframe.columnconfigure(0, weight=2)
    bottomframe.rowconfigure(0, weight=2)
    bottomframe.pack(pady=250, padx=250, side= "bottom")

    Label(topframe, text="Choose Car Placement").pack()


    # Create a Tkinter variable
    tkvar1 = StringVar(root)
    tkvar2 = StringVar(root)
    tkvar3 = StringVar(root)
    tkvar4 = StringVar(root)
    tkvar5 = StringVar(root)
    tkvar6 = StringVar(root)


    # Dictionary with options
    choices = {'No Car','Sports Car', 'SUV', 'Sedan', 'Large Truck', 'Motorcycle'}



    #Widget formatting and setup
    popupMenu1 = OptionMenu(bottomframe, tkvar1, *choices)
    popupMenu2 = OptionMenu(bottomframe, tkvar2, *choices)
    popupMenu3 = OptionMenu(bottomframe, tkvar3, *choices)
    popupMenu4 = OptionMenu(bottomframe, tkvar4, *choices)
    popupMenu5 = OptionMenu(bottomframe, tkvar5, *choices)
    popupMenu6 = OptionMenu(bottomframe, tkvar6, *choices)

    popupMenu1.grid(row=2, column=1)
    popupMenu2.grid(row=3, column=1)
    popupMenu3.grid(row=2, column=2)
    popupMenu4.grid(row=3, column=2)
    popupMenu5.grid(row=2, column=3)
    popupMenu6.grid(row=3, column=3)

    Label(topframe, text="Spot Number").pack()



    #setting defaults
    tkvar1.set('No Car')
    tkvar2.set('No Car')
    tkvar3.set('No Car')
    tkvar4.set('No Car')
    tkvar5.set('No Car')
    tkvar6.set('No Car')



    #arduino backbone thread:
    arduino = run_Aruino()

    #entry box
    e1 = Entry(topframe)
    e1.pack()


    def callback():
        data = collect_data(arduino, tkvar1, tkvar2, tkvar3, tkvar4, tkvar5, tkvar6, e1)
        print data


    b = Button(topframe, text="Collect",
                     command=callback)  # 'fg - foreground' is used to color the contents
    b.pack()









    root.mainloop()




#first creation of .csv file
def create_csv():
    csvRow = [["Timestamp, Spot Number, Spot Left Front, Spot Front, Spot Right Front, Spot Left, READING, Spot Right, Sen1x, Sen1y, Sen1z, Sen2x, Sen2y, Sen2z, Sen3x, Sen3y, Sen3z, Sen4x, Sen4y, Sen4z, Sen5x, Sen5y, Sen5z, Sen6x, Sen6y, Sen6z, Sen7x, Sen7y, Sen7z, Sen8x, Sen8y, Sen8z, Sen9x, Sen9y, Sen9z"]]
    with open('data.csv', 'w') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerows(csvRow)
    csvFile.close()


#adding data to .csv file
def append_data(csvFile, data):
    with open(csvFile, 'a') as csvFile:
        writer = csv.writer(csvFile)
        writer.writerow(data)
        print "Appended to .csv"

    csvFile.close()


main()