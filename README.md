# Fruit Sorting Conveyor System
### Control Program

## 1. Hardware:
- Raspi 4B with 4GB RAM.
- Control with interval upto `1ms`.
- IO Configuration: 
  - Beckhoff Remote IO BK1120 + DIOs + AIOs via Ethercat Protocol

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
bash ./script/start.sh
```
- Run below cmd to exit the application:
```
bash ./script/stop.sh
```
## 3. Features
- Ethercat Comm with Beckhoff Ethercat Remote IO.
- API Server Endpoint by Mongoose 7.9.

## 4. Results
- Ethercat Comm with Beckhoff Ethercat Remote IO with delay 1ms without logging.

## 5. TODOs
- [ ] IO Configuration with IO pin map for controling.
  - [x] Dev function to Rd/Wr Io more easily via Ethercat.
  - [x] IO Digital/Analog Mapping List.
  - [ ] Beckhoff KL3202 has conversion time ~ 250 ms -> Consider another module for faster AI conversion.

- [x] Add threading control for task scheduling between timing control and other communication.
- [x] Add Ethercat to comm with Ethernet Remote IO.
  - [ ] Add Modbus TCP/IP to comm with Ethernet Remote IO. -> depriotize Modbus TCP/IP.
- [ ] Logging handler to avoid CPU overload. -> Reduce log rate so depriotize.

- [ ] Setup API Endpoint Server configure to comm between Web Front-end UI with Controller Program.
   - [x] Zero Mq to comm between C program and Django Python API Server.
   - [ ] RestAPI Server: Python Django
   - [ ] Listing APIs to support data uploaded.
   - [ ] Develop APIs.