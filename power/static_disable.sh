sudo sed -i '1s/^/maxcpus=2 /' /boot/cmdline.txt

sudo cat disable.txt >> /boot/config.txt

sudo systemctl disable hciuart
sudo reboot
