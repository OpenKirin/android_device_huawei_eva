#include <unistd.h>
#include <cutils/properties.h>

#define PROP_NAME "ro.timer.finished"

int main(int argc, char* argv[]) {
    argc;
    argv[0];
    sleep(10);
    property_set(PROP_NAME, "true");
}
