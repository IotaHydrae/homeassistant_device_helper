/**
 * @file main.cpp
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-08
 *
 * MIT License
 *
 * Copyright 2022 IotaHydrae(writeforever@foxmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *
 */

#include <stdio.h>

#include "mqtt_io.h"
#include "Device.h"
#include "Sensor.h"


#define HA_ID       "000102adssdds"
#define HA_NAME     "Loona"
#define HA_SW_VER   "05881cd52242187d49a38e3fd7d675a41d5f564f"
#define HA_MODEL    "Revision a"
#define HA_MANUFACTURER "KEYi TECH"

Device *parent_dev = new Device(
            HA_NAME,
            HA_ID,
            HA_SW_VER,
            HA_MODEL,
            HA_MANUFACTURER);

/* Callback called when the client knows to the best of its abilities that a
 * PUBLISH has been successfully sent. For QoS 0 this means the message has
 * been completely written to the operating system. For QoS 1 this means we
 * have received a PUBACK from the broker. For QoS 2 this means we have
 * received a PUBCOMP from the broker. */
static void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    printf("Message with mid %d has been published.\n", mid);
}

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