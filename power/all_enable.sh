sudo sed -i '/dtparam=audio=off/d' /boot/config.txt
sudo sed -i '/dtoverlay=pi3-disable-bt/d' /boot/config.txt

sudo systemctl enable hciuart
sudo reboot
