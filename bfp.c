#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define CONFIG_FILE "/data/adb/modules/brightness-fix/.tmp"
#define LED_BRIGHTNESS_FILE "/sys/class/leds/lcd-backlight/brightness"

int main() {
    system("if pgrep -x led >/dev/null 2>&1; then kill -STOP $(pgrep -x led); fi");

    FILE *config_file = fopen(CONFIG_FILE, "a+");
    if (config_file == NULL) {
        exit(1);
    }

    int brightness_fd = open(LED_BRIGHTNESS_FILE, O_WRONLY);
    if (brightness_fd == -1) {
        fclose(config_file);
        exit(1);
    }

    char buffer[10];
    if (fgets(buffer, sizeof(buffer), config_file) == NULL) {
        fprintf(config_file, "2047\n");
        fflush(config_file);
    }

    while (1) {
        fseek(config_file, 0, SEEK_SET);
        if (fgets(buffer, sizeof(buffer), config_file) == NULL) {
            break;
        }
        write(brightness_fd, buffer, sizeof(buffer));
        usleep(500000);
    }

    close(brightness_fd);
    fclose(config_file);
    return 0;
}
