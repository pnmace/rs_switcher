#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int fd;
struct input_event report;
struct input_event lmb;

void setup() {
	struct uinput_setup usetup;

	// create key events that we will be using
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

	// create our fake input devices
	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0x1234;
	usetup.id.product = 0x5678;
	strcpy(usetup.name, "fake mouse");
	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);

	// initialize the global structs
	report.type = EV_SYN;
	report.code = SYN_REPORT;
	report.value = 0;

	lmb.type = EV_KEY;
	lmb.code = BTN_LEFT;
	lmb.value = 1;
}

void click() {
	lmb.value = 1;
	write(fd, &lmb, sizeof(lmb));
	write(fd, &report, sizeof(report));
	lmb.value = 0;
	write(fd, &lmb, sizeof(lmb));
	write(fd, &report, sizeof(report));
}

int main(void) {
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	setup();

	for (;;) {

		sleep(1);
	
		click();

	}

   	ioctl(fd, UI_DEV_DESTROY);
   	close(fd);

   	return 0;
}
