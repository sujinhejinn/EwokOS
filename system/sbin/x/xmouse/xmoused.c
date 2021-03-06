#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/vdevice.h>
#include <sys/proto.h>
#include <x/xcntl.h>
#include <x/xevent.h>

static int _x_pid = -1;

static void input(int8_t state, int8_t rx, int8_t ry) {
	xevent_t ev;
	ev.type = XEVT_MOUSE;
	ev.state = XEVT_MOUSE_MOVE;
	ev.value.mouse.x = 0;
	ev.value.mouse.y = 0;
	ev.value.mouse.rx = rx;
	ev.value.mouse.ry = ry;

	if(state == 2) //down
		ev.state = XEVT_MOUSE_DOWN;
	else if(state == 1) //up
		ev.state = XEVT_MOUSE_UP;

	proto_t in;
	PF->init(&in, NULL, 0)->add(&in, &ev, sizeof(xevent_t));
	dev_cntl_by_pid(_x_pid, X_DCNTL_INPUT, &in, NULL);
	PF->clear(&in);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	_x_pid = -1;

	int fd = open("/dev/mouse0", O_RDONLY);
	if(fd < 0)
		return 1;

	while(true) {
		if(_x_pid > 0) {
			int8_t mv[4];
			//if(read(fd, mv, 4) == 4)
			if(read_nblock(fd, mv, 4) == 4)
				input(mv[0], mv[1], mv[2]);
			else
				usleep(3000);
		}
		else {
			_x_pid = dev_get_pid("/dev/x");
			usleep(100000);
		}
	}

	close(fd);
	return 0;
}
