# homeassistant_device_helper
helpful tools to create device for home assistant on linux.

Build
------------------------
```shell
mkdir build && cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make -j


./mosquitto_test
```

Example
------------------------

```c
int main(int argc, char const *argv[])
{
    struct sensor sensor_temp_cpu_cfg = {
        .name = "Temperature CPU",
        .node_name = "temperature_cpu",
        .ppdev = parent_dev->m_dev,
        .unit_of_measurement = "°C",
    };

    struct sensor sensor_bat_percent_cfg = {
        .name = "Battery percent",
        .node_name = "battery_percent",
        .ppdev = parent_dev->m_dev,
        .unit_of_measurement = "%",
    };

    Sensor *sensor_bat_percent = new Sensor(&sensor_bat_percent_cfg);
    Sensor *sensor_temp_cpu = new Sensor(&sensor_temp_cpu_cfg);

    while (true) {
        sleep(1);

        sensor_bat_percent->send_data(random() % 100);
        sensor_temp_cpu->send_data(random() % 100);
    }

    return 0;
}
```