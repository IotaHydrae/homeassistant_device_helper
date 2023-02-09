/**
 * @file Component.h
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-09
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

#ifndef __COMPONENT_H
#define __COMPONENT_H

#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>

#include "mqtt_io.h"

#define DISCOVERY_PERFIX "homeassistant"

struct component {
    char const *name;
};

class Component
{
private:
protected:
public:
    struct component *m_comp;
    Component(char const *name)
    {
        m_comp = (struct component *)malloc(sizeof(struct component));

        m_comp->name = name;
    }
    ~Component()
    {

    }
    /* Callback called when the client receives a message. */
    virtual void on_message(struct mosquitto *mosq, void *obj,
                            const struct mosquitto_message *msg)
    {
        /* This blindly prints the payload, but the payload can be anything so take care. */
        printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
    }

    /* Callback called when the client knows to the best of its abilities that a
     * PUBLISH has been successfully sent. For QoS 0 this means the message has
     * been completely written to the operating system. For QoS 1 this means we
     * have received a PUBACK from the broker. For QoS 2 this means we have
     * received a PUBCOMP from the broker. */
    virtual void on_publish(struct mosquitto *mosq, void *obj, int mid)
    {
        printf("Message with mid %d has been published.\n", mid);
    }

    virtual void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
    {
        int rc;

        printf("%s, %d, %s\n", __func__, __LINE__, mosquitto_connack_string(reason_code));
        if (reason_code != 0) {
            /* If the connection fails for any reason, we don't want to keep on
             * retrying in this example, so disconnect. Without this, the client
             * will attempt to reconnect. */
            mosquitto_disconnect(mosq);
        }
    }

};

#endif  /* __COMPONENT_H */