/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-18     tanek        first implementation
 */

#include <stdio.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED1_PIN    GET_PIN(D, 4)
#define LED2_PIN    GET_PIN(D, 5)
#define LED3_PIN    GET_PIN(G, 3)

int main(void)
{
    //rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    //rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    //rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);

    while (1) {
        //rt_pin_write(LED1_PIN, PIN_HIGH);
        // rt_pin_write(LED2_PIN, PIN_HIGH);
        //rt_pin_write(LED3_PIN, PIN_HIGH);
        //rt_thread_mdelay(500);
        //rt_pin_write(LED1_PIN, PIN_LOW);
        // rt_pin_write(LED2_PIN, PIN_LOW);
        //rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return 0;
}
