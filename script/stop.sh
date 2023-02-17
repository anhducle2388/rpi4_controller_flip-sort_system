clear

app_pid=`/bin/ps -fu "root" | grep "./bin/app" | grep -v "grep" | awk '{print $2}' | tail -n 1`
if [ -n "$app_pid" ]; then
   echo Kill cc-app instance with PID = $app_pid
   sudo kill -9 $app_pid
fi

app_pid=`/bin/ps -fu "pi" | grep "python " | grep -v "grep" | awk '{print $2}'`
if [ -n "$app_pid" ]; then
   echo Kill python instance with PID = $app_pid
   sudo kill -9 $app_pid
fi