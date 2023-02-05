# Fruit Sorting Conveyor System
### Control Program

## 1. Hardware:
- Raspi 4B with 4GB Ram.
- Control with interval `10ms` and based on benchtest can run upto 10.000 line of codes.

## 2. Build/Run App Instruction:
### 2.1. Build App
- `makefile` is used to config the app build depedencies of project.
- Following below cmd to build the app.
```
make
```
### 2.2 Run App
- Run below cmd to run the application:
```
bash exec.sh
```
- Run below cmd to exit the application:
```
bash exit.sh
```
## 3. Features
- Ethercat Comm with Beckhoff Ethercat Remote IO.

## 4. Results
- Ethercat Comm with Beckhoff Ethercat Remote IO with delay 1ms without logging.

## 5. TODOs
- [ ] IO Configuration with IO pin map for controling.
  - Focus on loadcell readings, transfering from Msp430 module code to Raspi 4.
  - IO Digital/Analog Mapping List.
- [ ] Add threading control for task scheduling between timing control and other communication.
- [ ] Add Modbus TCP/IP to comm with Ethernet Remote IO.
