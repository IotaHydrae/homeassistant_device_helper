/**
 * @file Sensor.h
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

#pragma once

#ifndef __SENSOR_H
#define __SENSOR_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include <mosquitto.h>
#include <cjson/cJSON.h>

#include "Device.h"
#include "Component.h"

struct sensor {
    char const *name;
    char const *node_name;
    struct device *ppdev;
    char const *unit_of_measurement;
    char const *icon;
    bool force_update;

    char config_topic[64];
    char state_topic[64];

    MQTT_IO *mqtt_io;
    void (*on_message)(struct mosquitto *mosq, void *obj,
                       const struct mosquitto_message *msg);
    void (*on_publish)(struct mosquitto *mosq, void *obj, int mid);
    void (*on_connect)(struct mosquitto *mosq, void *obj, int reason_code);

};

class Sensor : public Component
{

private:
    MQTT_IO *mqtt_io;
    struct sensor   *m_sensor;
    int send_config()
    {
        int rc;
        char *string = NULL;

        cJSON *device = cJSON_CreateObject();
        cJSON *payload = cJSON_CreateObject();


        // cJSON_AddStringToObject(payload, "~", path_state);
        cJSON_AddStringToObject(payload, "name", m_sensor->name);

        cJSON_AddStringToObject(payload, "state_topic", m_sensor->state_topic);

        char uid[64];
        sprintf(uid, "0a7476cc-d6c1-40ba-8ae1-60%ld", random());
        cJSON_AddStringToObject(payload, "unique_id", uid);

        cJSON_AddStringToObject(payload, "unit_of_measurement", m_sensor->unit_of_measurement);

        // if (cJSON_AddStringToObject(payload, "device", device) == NULL)
        // {
        //     goto end;
        // }
        cJSON_AddItemToObject(payload, "device", device);

        cJSON_AddStringToObject(device, "identifiers", m_sensor->ppdev->identifiers);


        cJSON_AddStringToObject(device, "name", m_sensor->ppdev->name);

        cJSON_AddStringToObject(device, "sw_version", m_sensor->ppdev->sw_version);

        cJSON_AddStringToObject(device, "model", m_sensor->ppdev->model);

        cJSON_AddStringToObject(device, "manufacturer", m_sensor->ppdev->manufacturer);

        // cJSON_AddItemToObject(payload, "device", cJSON_CreateObject());

        cJSON_AddStringToObject(payload, "force_update",
                                m_sensor->force_update ? "True" : "False");

        string = cJSON_PrintUnformatted(payload);
        if (string == NULL) {
            fprintf(stderr, "Failed to print monitor.\n");
        }
        // printf("%s\n", string);

        m_sensor->mqtt_io->publish(m_sensor->config_topic, string, 2);
        return 0;
    }
protected:
public:
    int send_data(double value)
    {
        char payload[64];

        sprintf(payload, "%.1f\n", value);
        m_sensor->mqtt_io->publish(m_sensor->state_topic, payload, 2);
        return 0;
    }
    Sensor(MQTT_IO *mqtt_io, struct sensor *p_sensor) : Component("sensor")
    {
    }
    Sensor(struct sensor *p_sensor) : Component("sensor")
    {
        printf("Registering Sensor \"%s\" ...\n", p_sensor->name);

        m_sensor = (struct sensor *)malloc(sizeof(struct sensor));
        if (!m_sensor) {
            printf("Error: out of memory!\n");
            return;
        }

        memcpy(m_sensor, p_sensor, sizeof(struct sensor));
        mqtt_io = new MQTT_IO(
                    SERVER_IP,
                    SERVER_PORT,
                    USERNAME,
                    PASSWORD
        );
        m_sensor->mqtt_io = mqtt_io;

        if (m_sensor->on_connect)
            mosquitto_connect_callback_set(m_sensor->mqtt_io->m_handle.mosq,
                                           m_sensor->on_connect);
        if (m_sensor->on_message)
            mosquitto_message_callback_set(m_sensor->mqtt_io->m_handle.mosq,
                                           m_sensor->on_message);
        if (m_sensor->on_publish)
            mosquitto_publish_callback_set(m_sensor->mqtt_io->m_handle.mosq,
                                           m_sensor->on_publish);

        sprintf(m_sensor->state_topic, "%s/%s/%s/state", m_sensor->ppdev->name,
                Component::m_comp->name,
                m_sensor->node_name);
        // printf("%s\n", m_sensor->state_topic);

        sprintf(m_sensor->config_topic, DISCOVERY_PERFIX "/%s/%s/%s/config",
                Component::m_comp->name,
                m_sensor->ppdev->name, m_sensor->node_name);
        // printf("%s, %d\n", m_sensor->config_topic, (int)strlen(m_sensor->config_topic));

        send_config();
    }
    ~Sensor()
    {

    }
};

#endif /* __SENSOR_H */
