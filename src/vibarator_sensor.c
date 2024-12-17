#include "vibarator_sensor.h"

void init_vibarator_sensor() {
    gpio_init(VIBARATOR_SENSOR_PIN);
    gpio_set_dir(VIBARATOR_SENSOR_PIN, GPIO_IN);
      gpio_pull_down(VIBARATOR_SENSOR_PIN);
}

int get_vibarator_sensor() {
  return gpio_get(VIBARATOR_SENSOR_PIN);
}