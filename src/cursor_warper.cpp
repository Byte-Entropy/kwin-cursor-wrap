#include <systemd/sd-bus.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <csignal>

static volatile sig_atomic_t running = 1;
static int uinput_fd = -1;

void handle_sig(int) { running = 0; }

void emit_event(int fd, uint16_t type, uint16_t code, int32_t val) {
    struct input_event ie{};
    ie.type = type;
    ie.code = code;
    ie.value = val;
    if (write(fd, &ie, sizeof(ie)) < 0) {
        perror("uinput write failed");
    }
}

int setup_uinput() {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open /dev/uinput");
        return -1;
    }

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    struct uinput_setup usetup{};
    std::snprintf(usetup.name, UINPUT_MAX_NAME_SIZE, "KWin-Cursor-Warper");
    usetup.id.bustype = BUS_VIRTUAL;
    usetup.id.vendor  = 0x1234;
    usetup.id.product = 0x5678;

    if (ioctl(fd, UI_DEV_SETUP, &usetup) < 0 || ioctl(fd, UI_DEV_CREATE) < 0) {
        perror("Failed to create uinput device");
        close(fd);
        return -1;
    }

    usleep(200000);
    return fd;
}

static int method_warp(sd_bus_message *m, void *, sd_bus_error *) {
    int32_t dx = 0, dy = 0;
    int r = sd_bus_message_read(m, "ii", &dx, &dy);
    if (r < 0) return r;

    if (uinput_fd >= 0) {
        emit_event(uinput_fd, EV_REL, REL_X, dx);
        emit_event(uinput_fd, EV_REL, REL_Y, dy);
        emit_event(uinput_fd, EV_SYN, SYN_REPORT, 0);
    }

    return sd_bus_reply_method_return(m, "");
}

static const sd_bus_vtable warper_vtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("Warp", "ii", "", method_warp, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

int main() {
    std::signal(SIGINT, handle_sig);
    std::signal(SIGTERM, handle_sig);

    uinput_fd = setup_uinput();
    if (uinput_fd < 0) return 1;

    sd_bus *bus = nullptr;
    if (sd_bus_open_user(&bus) < 0) return 1;

    sd_bus_slot *slot = nullptr;
    if (sd_bus_add_object_vtable(bus, &slot, "/Warper", "org.berrybyte.CursorWarper", warper_vtable, nullptr) < 0) return 1;
    if (sd_bus_request_name(bus, "org.berrybyte.CursorWarper", 0) < 0) return 1;

    while (running) {
        int r = sd_bus_process(bus, nullptr);
        if (r < 0) break;
        if (r == 0) sd_bus_wait(bus, 100000);
    }

    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
    return 0;
}
