# Energy_Monitor
Project that monitor the electric power consumption

With the development of this project, a way to contain excessive consumption is proposed, since the system has load deactivation in case of excessive energy use (when the power consumption exceeds a Setpoint value), which will generate savings for the user. It also has a screen that displays current consumption data.

Consumption will be measured in kW. To get it, we'll use the AC current sensor (SCT-013) and the single phase AC voltage sensor (ZMPT101B). But for now, we just run a simulation to check that the   microcontroller's peripherals are working properly:

* ADC module: To get the analog values from the sensors, we use 2 ADC channels (one for each sensor). Since this is just a simulation, we just read the analog signal from two potentiometers (0 to 5 V).

* Keypad (4x4) : To allow the user to move from "screen 1" to "screen 3" by pressing 'D' key and permit the user to change preset value of the Setpoint and the Tolerance.

* LCD (16x2): Shows the information to the user (through different screens) (like current consumption, active load channels , S.P, tolerance, etc).
* Timer 0: To establish the sample time. It will compare the current load consumption with setpoint value each certain time, in order to activate or diactivate the loads.

It is worth to say that we use ADC interrupts and Timer 0 interrupts, at the same time, by setting priority in them.

![prototype](https://user-images.githubusercontent.com/91303136/231690216-df90577c-47a0-4ff7-adba-77952a2cb52e.jpg)
![prototype_1](https://user-images.githubusercontent.com/91303136/231690219-4053fd47-fd2a-403d-9fd9-68c0bc957f7b.jpg)
![prototype_3](https://user-images.githubusercontent.com/91303136/231690222-ead72bc5-9b70-4c57-988b-2eaf0075ed83.jpg)
![test1](https://user-images.githubusercontent.com/91303136/231690223-437f3128-6b95-4ea6-beef-3dc71137d71d.jpg)
![test2](https://user-images.githubusercontent.com/91303136/231690225-07af1783-f1ff-41f3-b7f7-d8506b01ca82.jpg)
