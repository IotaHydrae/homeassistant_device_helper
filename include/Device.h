/**
 * @file Device.h
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

#ifndef __DEVICE_H
#define __DEVICE_H

#include <stdio.h>
#include <stdlib.h>

struct device {
    char const *name;
    char const *identifiers;
    char const *sw_version;
    char const *model;
    char const *manufacturer;
};

class Device
{
private:
protected:
public:
    struct device *m_dev;
    Device(char const *name, char const *identifiers, char const *sw_version,
           char const *model, char const *manufacturer)
    {
        m_dev = (struct device *)malloc(sizeof(struct device));

        if (!m_dev) {
            printf("Error: out of memory!\n");
            return;
        }

        m_dev->name = name;
        m_dev->identifiers = identifiers;
        m_dev->sw_version = sw_version;
        m_dev->model = model;
        m_dev->manufacturer = manufacturer;
    }

    ~Device()
    {
        if (m_dev)
            free(m_dev);
    }
};

#endif  /* __DEVICE_H */