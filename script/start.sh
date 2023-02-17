# Build app
make

# Start app
sudo ./script/stop.sh
nohup sudo ./bin/app > ./log/nohup.log &

# Start python
nohup python ./app-ui/test/zeromq.py > ./log/_python.log &

# Clean cmd
clear
ls ./bin/ -all --block-size=M



