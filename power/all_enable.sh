# cpu
sudo sed -i 's/maxcpus=2 //g' /boot/cmdline.txt
sudo sed -i '/arm_freq=700/d' /boot/config.txt

# audio
sudo sed -i '/dtparam=audio=off/d' /boot/config.txt


# bluetooth
sudo sed -i '/dtoverlay=pi3-disable-bt/d' /boot/config.txt
sudo systemctl enable hciuart

sudo reboot
