#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <mac/mac.h>
//#include "DataBase.hpp"

bool getMacAddress(char *buf){
	memset(buf, 0, sizeof(char)* 18);
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, "eth0");
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
		snprintf(buf, sizeof(char)* 18,"%02X:%02X:%02X:%02X:%02X:%02X",
						(unsigned char)s.ifr_addr.sa_data[0],(unsigned char)s.ifr_addr.sa_data[1],
						(unsigned char)s.ifr_addr.sa_data[2],(unsigned char)s.ifr_addr.sa_data[3],
						(unsigned char)s.ifr_addr.sa_data[4],(unsigned char)s.ifr_addr.sa_data[5]);

		close(fd);
		return 0;

	}
	close(fd);
	return 1;
}
