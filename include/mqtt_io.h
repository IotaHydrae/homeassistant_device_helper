/**
 * @file mqtt_io.h
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

#ifndef __MQTT_IO_H
#define __MQTT_IO_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <mosquitto.h>

#define SERVER_IP   "192.168.0.190"
#define SERVER_PORT "1883"
#define USERNAME    "keyi"
#define PASSWORD    "keyifamily"
#define KEEP_ALIVE  60
#define QOS_LEVEL   2

struct mqtt_handler {
    struct mosquitto *mosq;

    char const *server_ip;
    char const *server_port;
    int keepalive;

    char const *username;
    char const *passwd;
};

static void sensor_temp_cpu_on_connect(struct mosquitto *mosq, void *obj, int reason_code)
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

static void *mqtt_io_init_loop(void *arg)
{
    int rc;
    struct mqtt_handler *handle = (struct mqtt_handler *)arg;
    /* Run the network loop in a background thread, this call returns quickly. */
    printf("starting loop ...\n");
    rc = mosquitto_loop_start(handle->mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        mosquitto_destroy(handle->mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
    }

    while (1);

    mosquitto_loop_forever(handle->mosq, -1, 1);
}

class MQTT_IO
{
private:
protected:
public:
    struct mqtt_handler m_handle;
    MQTT_IO(char const *server_ip, char const *server_port, char const *username,
            char const *passwd)
    {
        int rc;
        printf("initializing MQTT IO ...\n");
        mosquitto_lib_init();

        m_handle.server_ip = server_ip;
        m_handle.server_port = server_port;
        m_handle.username = username;
        m_handle.passwd = passwd;

        m_handle.mosq = mosquitto_new(NULL, true, NULL);
        if (!m_handle.mosq) {
            fprintf(stderr, "oom\n");
            exit(EXIT_FAILURE);
        }

        // mosquitto_connect_callback_set(handle->mosq,
        //                                sensor_temp_cpu_on_connect);

        printf("setting uname %s also passwd %s\n", m_handle.username, m_handle.passwd);
        mosquitto_username_pw_set(m_handle.mosq, m_handle.username, m_handle.passwd);
        rc = mosquitto_connect(m_handle.mosq, m_handle.server_ip, 1883, 60);
        if (rc != MOSQ_ERR_SUCCESS) {
            mosquitto_destroy(m_handle.mosq);
            fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        }

        pthread_t tid_mqtt_io_loop;
        pthread_create(&tid_mqtt_io_loop, NULL, mqtt_io_init_loop, (void *)&m_handle);
    }

    ~MQTT_IO()
    {
        printf("deinitializing MQTT IO ...\n");
        mosquitto_destroy(MQTT_IO::m_handle.mosq);
        mosquitto_lib_cleanup();
    }

    int subscribe(char *topic, int qos)
    {
        int rc;

        if (!m_handle.mosq)
            return 1;

        /* Making subscriptions in the on_connect() callback means that if the
         * connection drops and is automatically resumed by the client, then the
         * subscriptions will be recreated when the client reconnects. */
        rc = mosquitto_subscribe(m_handle.mosq, NULL, "example/temperature", 1);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error subscribing: %s\n", mosquitto_strerror(rc));
            /* We might as well disconnect if we were unable to subscribe */
            mosquitto_disconnect(m_handle.mosq);
        }

        return 0;
    }

    int publish(char *topic, char payload[], int qos)
    {
        int rc;
        // struct mqtt_handler *handle = (struct mqtt_handler *)p_m_handle;
        // printf("%d\n", p_m_handle);
        printf("topic : %s\n", topic);
        printf("publishing message : %s\n", payload);
        /* Publish the message
         * mosq - our client instance
         * *mid = NULL - we don't want to know what the message id for this message is
         * topic = "example/temperature" - the topic on which this message will be published
         * payloadlen = strlen(payload) - the length of our payload in bytes
         * payload - the actual payload
         * qos = 2 - publish with QoS 2 for this example
         * retain = false - do not use the retained message feature for this message
         */
        rc = mosquitto_publish(m_handle.mosq, NULL, topic, strlen(payload), payload, qos,
                               false);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
        }

        return 0;
    }


};

#endif