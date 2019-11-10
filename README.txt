Wlcome to use this project.

You should run "./installpkg.sh" to install some necessary packages first.

Type "./refresh.sh" to make build directory.

You can find "sever_run" and "client_run" in "build/main/"

If you want to run as a server, type "./server_run"
If you want to run as a client, type "./client_run"




######Power saving######
We have 2 types of power saving method which are static power saving and runtime power saving.
Once the static power saving is on, it will be active even after the raspberry pi rebooted.
On the oter hand, the runtime power saving should be run every time the raspberrypi rebooted.

If you want to save the power usage, please follow the direction listed below.

1. cd power
2. type "sudo ./static_disable.sh" this script needed to be run once and raspberrypi will be rebooted
3. type "sudo ./runtime_disable.sh" this script should be run everytime you reboot the raspberrypi


If you want to enable all disabled features for power saving
tpye "sudo ./all_enable.sh"
