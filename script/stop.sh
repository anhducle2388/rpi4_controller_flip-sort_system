app_pid=`/bin/ps -fu "root" | grep "./bin/app" | grep -v "grep" | awk '{print $2}' | tail -n 1`
echo Kill ./bin/app with PID= $app_pid
sudo kill -9 $app_pid