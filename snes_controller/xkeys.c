#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#define USBDEV "/dev/ttyUSB1"

Display *display;

int open_port() {
	int fd;

	fd = open(USBDEV, O_RDWR | O_NOCTTY);
	
	if (fd == -1) {
		printf("- Unable to open %s\n", USBDEV);
	} else {
		fcntl(fd, F_SETFL, 0);
		printf("+ Port is open.\n");
	}

	return(fd);
}


int configure_port(int fd) {
	struct termios port_settings;

	// Baud rautes
	cfsetispeed(&port_settings, B57600);
	cfsetospeed(&port_settings, B57600);

	// Parity, stop bits, data bits (nothing)
	port_settings.c_cflag &= ~PARENB;
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cflag |= CS8;

	// Apply
	tcsetattr(fd, TCSANOW, &port_settings);

	return(fd);
}

void process_keys(const char *orig_buffer) {
	char buffer[512];
	char *token;
	unsigned int keycode;

	// Process key up
	keycode = 0;
	strcpy(buffer, orig_buffer);
	token = strtok(buffer," ");
	while (token != NULL) {
		if (strcmp(token, "UP") == 0) {
			keycode = XKeysymToKeycode(display, XK_Up);
		} else if (strcmp(token, "DOWN") == 0) {
			keycode = XKeysymToKeycode(display, XK_Down);
		} else if (strcmp(token, "LEFT") == 0) {
			keycode = XKeysymToKeycode(display, XK_Left);
		} else if (strcmp(token, "RIGHT") == 0) {
			keycode = XKeysymToKeycode(display, XK_Right);
		} else if (strcmp(token, "SELECT") == 0) {
			keycode = XKeysymToKeycode(display, XK_S);
		} else if (strcmp(token, "START") == 0) {
			keycode = XKeysymToKeycode(display, XK_T);
		} else if (strcmp(token, "X") == 0) {
			keycode = XKeysymToKeycode(display, XK_X);
		} else if (strcmp(token, "Y") == 0) {
			keycode = XKeysymToKeycode(display, XK_Y);
		} else if (strcmp(token, "A") == 0) {
			keycode = XKeysymToKeycode(display, XK_A);
		} else if (strcmp(token, "B") == 0) {
			keycode = XKeysymToKeycode(display, XK_B);
		} else if (strcmp(token, "LEFT") == 0) {
			keycode = XKeysymToKeycode(display, XK_L);
		} else if (strcmp(token, "RIGHT") == 0) {
			keycode = XKeysymToKeycode(display, XK_R);
		}

		if (keycode != 0) {
			XTestFakeKeyEvent(display, keycode, True, 0);
		}

		// Next
		token = strtok(NULL, " ");
	}
	
	// Process key down
	keycode = 0;
	strcpy(buffer, orig_buffer);
	//printf("buffer = %s\n", buffer);
	token = strtok(buffer," ");
	while (token != NULL) {
		if (strcmp(token, "UP") == 0) {
			keycode = XKeysymToKeycode(display, XK_Up);
		} else if (strcmp(token, "DOWN") == 0) {
			keycode = XKeysymToKeycode(display, XK_Down);
		} else if (strcmp(token, "LEFT") == 0) {
			keycode = XKeysymToKeycode(display, XK_Left);
		} else if (strcmp(token, "RIGHT") == 0) {
			keycode = XKeysymToKeycode(display, XK_Right);
		} else if (strcmp(token, "SELECT") == 0) {
			keycode = XKeysymToKeycode(display, XK_S);
		} else if (strcmp(token, "START") == 0) {
			keycode = XKeysymToKeycode(display, XK_T);
		} else if (strcmp(token, "X") == 0) {
			keycode = XKeysymToKeycode(display, XK_X);
		} else if (strcmp(token, "Y") == 0) {
			keycode = XKeysymToKeycode(display, XK_Y);
		} else if (strcmp(token, "A") == 0) {
			keycode = XKeysymToKeycode(display, XK_A);
		} else if (strcmp(token, "B") == 0) {
			keycode = XKeysymToKeycode(display, XK_B);
		} else if (strcmp(token, "LEFT") == 0) {
			keycode = XKeysymToKeycode(display, XK_L);
		} else if (strcmp(token, "RIGHT") == 0) {
			keycode = XKeysymToKeycode(display, XK_R);
		}

		if (keycode != 0) {
			XTestFakeKeyEvent(display, keycode, False, 0);
		}

		// Next
		token = strtok(NULL, " ");
	}

	XFlush(display);
}

int main() {
	char buffer[512];
	int fd;

	display = XOpenDisplay(NULL);

	fd = open_port();
	configure_port(fd);
	
	while (1) {
		for (int i = 0; i < sizeof(buffer); i++) buffer[i] = 0;

		lseek(fd, 0, SEEK_SET);
		read(fd, buffer, sizeof(buffer));
		if (buffer[0]) {
			// Remove all useless information first
			for (int i = 0; i < sizeof(buffer); i++) {
				if (!buffer[i]) {
					break;
				} else if (buffer[i] < 32) {
					buffer[i] = 0;
				}
			}
			
			// Process
			process_keys(buffer);
		}
	}

	return 0;
}
