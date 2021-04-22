/*
 * File      : drv_gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-10-20     ZYH            the first version
 * 2018-04-23     misonyo        port to gd32f30x
 */

#include "drv_gpio.h"
#include <rtdevice.h>
#include <rthw.h>
#include <gd32f4xx.h>
#include "gd32f4xx_exti.h"

#ifdef RT_USING_PIN

#define __GD32_PIN(index, port, pin) {index, RCU_GPIO##port, GPIO##port, \
        GPIO_PIN_##pin, EXTI_SOURCE_GPIO##port, EXTI_SOURCE_PIN##pin}
#define __GD32_PIN_DEFAULT {-1, (rcu_periph_enum)0, 0, 0, 0, 0}

/* GD32 GPIO driver */
struct pin_index
{
    rt_int16_t index;
    rcu_periph_enum clk;
    rt_uint32_t gpio_periph;
    rt_uint32_t pin;
    rt_uint8_t port_src;
    rt_uint8_t pin_src;
};

static const struct pin_index pins[] =
{
#if defined(GPIOA)
    __GD32_PIN(0 ,  A, 0 ),
    __GD32_PIN(1 ,  A, 1 ),
    __GD32_PIN(2 ,  A, 2 ),
    __GD32_PIN(3 ,  A, 3 ),
    __GD32_PIN(4 ,  A, 4 ),
    __GD32_PIN(5 ,  A, 5 ),
    __GD32_PIN(6 ,  A, 6 ),
    __GD32_PIN(7 ,  A, 7 ),
    __GD32_PIN(8 ,  A, 8 ),
    __GD32_PIN(9 ,  A, 9 ),
    __GD32_PIN(10,  A, 10),
    __GD32_PIN(11,  A, 11),
    __GD32_PIN(12,  A, 12),
    __GD32_PIN(13,  A, 13),
    __GD32_PIN(14,  A, 14),
    __GD32_PIN(15,  A, 15),
#if defined(GPIOB)
    __GD32_PIN(16,  B, 0),
    __GD32_PIN(17,  B, 1),
    __GD32_PIN(18,  B, 2),
    __GD32_PIN(19,  B, 3),
    __GD32_PIN(20,  B, 4),
    __GD32_PIN(21,  B, 5),
    __GD32_PIN(22,  B, 6),
    __GD32_PIN(23,  B, 7),
    __GD32_PIN(24,  B, 8),
    __GD32_PIN(25,  B, 9),
    __GD32_PIN(26,  B, 10),
    __GD32_PIN(27,  B, 11),
    __GD32_PIN(28,  B, 12),
    __GD32_PIN(29,  B, 13),
    __GD32_PIN(30,  B, 14),
    __GD32_PIN(31,  B, 15),
#if defined(GPIOC)
    __GD32_PIN(32,  C, 0),
    __GD32_PIN(33,  C, 1),
    __GD32_PIN(34,  C, 2),
    __GD32_PIN(35,  C, 3),
    __GD32_PIN(36,  C, 4),
    __GD32_PIN(37,  C, 5),
    __GD32_PIN(38,  C, 6),
    __GD32_PIN(39,  C, 7),
    __GD32_PIN(40,  C, 8),
    __GD32_PIN(41,  C, 9),
    __GD32_PIN(42,  C, 10),
    __GD32_PIN(43,  C, 11),
    __GD32_PIN(44,  C, 12),
    __GD32_PIN(45,  C, 13),
    __GD32_PIN(46,  C, 14),
    __GD32_PIN(47,  C, 15),
#if defined(GPIOD)
    __GD32_PIN(48,  D, 0),
    __GD32_PIN(49,  D, 1),
    __GD32_PIN(50,  D, 2),
    __GD32_PIN(51,  D, 3),
    __GD32_PIN(52,  D, 4),
    __GD32_PIN(53,  D, 5),
    __GD32_PIN(54,  D, 6),
    __GD32_PIN(55,  D, 7),
    __GD32_PIN(56,  D, 8),
    __GD32_PIN(57,  D, 9),
    __GD32_PIN(58,  D, 10),
    __GD32_PIN(59,  D, 11),
    __GD32_PIN(60,  D, 12),
    __GD32_PIN(61,  D, 13),
    __GD32_PIN(62,  D, 14),
    __GD32_PIN(63,  D, 15),
#if defined(GPIOE)
    __GD32_PIN(64,  E, 0),
    __GD32_PIN(65,  E, 1),
    __GD32_PIN(66,  E, 2),
    __GD32_PIN(67,  E, 3),
    __GD32_PIN(68,  E, 4),
    __GD32_PIN(69,  E, 5),
    __GD32_PIN(70,  E, 6),
    __GD32_PIN(71,  E, 7),
    __GD32_PIN(72,  E, 8),
    __GD32_PIN(73,  E, 9),
    __GD32_PIN(74,  E, 10),
    __GD32_PIN(75,  E, 11),
    __GD32_PIN(76,  E, 12),
    __GD32_PIN(77,  E, 13),
    __GD32_PIN(78,  E, 14),
    __GD32_PIN(79,  E, 15),
#if defined(GPIOF)
    __GD32_PIN(80,  F, 0),
    __GD32_PIN(81,  F, 1),
    __GD32_PIN(82,  F, 2),
    __GD32_PIN(83,  F, 3),
    __GD32_PIN(84,  F, 4),
    __GD32_PIN(85,  F, 5),
    __GD32_PIN(86,  F, 6),
    __GD32_PIN(87,  F, 7),
    __GD32_PIN(88,  F, 8),
    __GD32_PIN(89,  F, 9),
    __GD32_PIN(90,  F, 10),
    __GD32_PIN(91,  F, 11),
    __GD32_PIN(92,  F, 12),
    __GD32_PIN(93,  F, 13),
    __GD32_PIN(94,  F, 14),
    __GD32_PIN(95,  F, 15),
#if defined(GPIOG)
    __GD32_PIN(96,  G, 0),
    __GD32_PIN(97,  G, 1),
    __GD32_PIN(98,  G, 2),
    __GD32_PIN(99,  G, 3),
    __GD32_PIN(100, G, 4),
    __GD32_PIN(101, G, 5),
    __GD32_PIN(102, G, 6),
    __GD32_PIN(103, G, 7),
    __GD32_PIN(104, G, 8),
    __GD32_PIN(105, G, 9),
    __GD32_PIN(106, G, 10),
    __GD32_PIN(107, G, 11),
    __GD32_PIN(108, G, 12),
    __GD32_PIN(109, G, 13),
    __GD32_PIN(110, G, 14),
    __GD32_PIN(111, G, 15),
#if defined(GPIOH)
    __GD32_PIN(112, H, 0),
    __GD32_PIN(113, H, 1),
    __GD32_PIN(114, H, 2),
    __GD32_PIN(115, H, 3),
    __GD32_PIN(116, H, 4),
    __GD32_PIN(117, H, 5),
    __GD32_PIN(118, H, 6),
    __GD32_PIN(119, H, 7),
    __GD32_PIN(120, H, 8),
    __GD32_PIN(121, H, 9),
    __GD32_PIN(122, H, 10),
    __GD32_PIN(123, H, 11),
    __GD32_PIN(124, H, 12),
    __GD32_PIN(125, H, 13),
    __GD32_PIN(126, H, 14),
    __GD32_PIN(127, H, 15),
#if defined(GPIOI)
    __GD32_PIN(128, I, 0),
    __GD32_PIN(129, I, 1),
    __GD32_PIN(130, I, 2),
    __GD32_PIN(131, I, 3),
    __GD32_PIN(132, I, 4),
    __GD32_PIN(133, I, 5),
    __GD32_PIN(134, I, 6),
    __GD32_PIN(135, I, 7),
    __GD32_PIN(136, I, 8),
    __GD32_PIN(137, I, 9),
    __GD32_PIN(138, I, 10),
    __GD32_PIN(139, I, 11),
    __GD32_PIN(140, I, 12),
    __GD32_PIN(141, I, 13),
    __GD32_PIN(142, I, 14),
    __GD32_PIN(143, I, 15),
#if defined(GPIOJ)
    __GD32_PIN(144, J, 0),
    __GD32_PIN(145, J, 1),
    __GD32_PIN(146, J, 2),
    __GD32_PIN(147, J, 3),
    __GD32_PIN(148, J, 4),
    __GD32_PIN(149, J, 5),
    __GD32_PIN(150, J, 6),
    __GD32_PIN(151, J, 7),
    __GD32_PIN(152, J, 8),
    __GD32_PIN(153, J, 9),
    __GD32_PIN(154, J, 10),
    __GD32_PIN(155, J, 11),
    __GD32_PIN(156, J, 12),
    __GD32_PIN(157, J, 13),
    __GD32_PIN(158, J, 14),
    __GD32_PIN(159, J, 15),
#if defined(GPIOK)
    __GD32_PIN(160, K, 0),
    __GD32_PIN(161, K, 1),
    __GD32_PIN(162, K, 2),
    __GD32_PIN(163, K, 3),
    __GD32_PIN(164, K, 4),
    __GD32_PIN(165, K, 5),
    __GD32_PIN(166, K, 6),
    __GD32_PIN(167, K, 7),
    __GD32_PIN(168, K, 8),
    __GD32_PIN(169, K, 9),
    __GD32_PIN(170, K, 10),
    __GD32_PIN(171, K, 11),
    __GD32_PIN(172, K, 12),
    __GD32_PIN(173, K, 13),
    __GD32_PIN(174, K, 14),
    __GD32_PIN(175, K, 15),
#endif /* defined(GPIOK) */
#endif /* defined(GPIOJ) */
#endif /* defined(GPIOI) */
#endif /* defined(GPIOH) */
#endif /* defined(GPIOG) */
#endif /* defined(GPIOF) */
#endif /* defined(GPIOE) */
#endif /* defined(GPIOD) */
#endif /* defined(GPIOC) */
#endif /* defined(GPIOB) */
#endif /* defined(GPIOA) */
#if 0
    __GD32_PIN_DEFAULT,
    __GD32_PIN(1, E, 2),
    __GD32_PIN(2, E, 3),
    __GD32_PIN(3, E, 4),
    __GD32_PIN(4, E, 5),
    __GD32_PIN(5, E, 6),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(7, C, 13),
    __GD32_PIN(8, C, 14),
    __GD32_PIN(9, C, 15),
    __GD32_PIN(10, F, 0),
    __GD32_PIN(11, F, 1),
    __GD32_PIN(12, F, 2),
    __GD32_PIN(13, F, 3),
    __GD32_PIN(14, F, 4),
    __GD32_PIN(15, F, 5),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(18, F, 6),
    __GD32_PIN(19, F, 7),
    __GD32_PIN(20, F, 8),
    __GD32_PIN(21, F, 9),
    __GD32_PIN(22, F, 10),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(26, C, 0),
    __GD32_PIN(27, C, 1),
    __GD32_PIN(28, C, 2),
    __GD32_PIN(29, C, 3),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(34, A, 0),
    __GD32_PIN(35, A, 1),
    __GD32_PIN(36, A, 2),
    __GD32_PIN(37, A, 3),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(40, A, 4),
    __GD32_PIN(41, A, 5),
    __GD32_PIN(42, A, 6),
    __GD32_PIN(43, A, 7),
    __GD32_PIN(44, C, 4),
    __GD32_PIN(45, C, 5),
    __GD32_PIN(46, B, 0),
    __GD32_PIN(47, B, 1),
    __GD32_PIN(48, B, 2),
    __GD32_PIN(49, F, 11),
    __GD32_PIN(50, F, 12),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(53, F, 13),
    __GD32_PIN(54, F, 14),
    __GD32_PIN(55, F, 15),
    __GD32_PIN(56, G, 0),
    __GD32_PIN(57, G, 1),
    __GD32_PIN(58, E, 7),
    __GD32_PIN(59, E, 8),
    __GD32_PIN(60, E, 9),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(63, E, 10),
    __GD32_PIN(64, E, 11),
    __GD32_PIN(65, E, 12),
    __GD32_PIN(66, E, 13),
    __GD32_PIN(67, E, 14),
    __GD32_PIN(68, E, 15),
    __GD32_PIN(69, B, 10),
    __GD32_PIN(70, B, 11),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(73, B, 12),
    __GD32_PIN(74, B, 13),
    __GD32_PIN(75, B, 14),
    __GD32_PIN(76, B, 15),
    __GD32_PIN(77, D, 8),
    __GD32_PIN(78, D, 9),
    __GD32_PIN(79, D, 10),
    __GD32_PIN(80, D, 11),
    __GD32_PIN(81, D, 12),
    __GD32_PIN(82, D, 13),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(85, D, 14),
    __GD32_PIN(86, D, 15),
    __GD32_PIN(87, G, 2),
    __GD32_PIN(88, G, 3),
    __GD32_PIN(89, G, 4),
    __GD32_PIN(90, G, 5),
    __GD32_PIN(91, G, 6),
    __GD32_PIN(92, G, 7),
    __GD32_PIN(93, G, 8),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(96, C, 6),
    __GD32_PIN(97, C, 7),
    __GD32_PIN(98, C, 8),
    __GD32_PIN(99, C, 9),
    __GD32_PIN(100, A, 8),
    __GD32_PIN(101, A, 9),
    __GD32_PIN(102, A, 10),
    __GD32_PIN(103, A, 11),
    __GD32_PIN(104, A, 12),
    __GD32_PIN(105, A, 13),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(109, A, 14),
    __GD32_PIN(110, A, 15),
    __GD32_PIN(111, C, 10),
    __GD32_PIN(112, C, 11),
    __GD32_PIN(113, C, 12),
    __GD32_PIN(114, D, 0),
    __GD32_PIN(115, D, 1),
    __GD32_PIN(116, D, 2),
    __GD32_PIN(117, D, 3),
    __GD32_PIN(118, D, 4),
    __GD32_PIN(119, D, 5),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(122, D, 6),
    __GD32_PIN(123, D, 7),
    __GD32_PIN(124, G, 9),
    __GD32_PIN(125, G, 10),
    __GD32_PIN(126, G, 11),
    __GD32_PIN(127, G, 12),
    __GD32_PIN(128, G, 13),
    __GD32_PIN(129, G, 14),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
    __GD32_PIN(132, G, 15),
    __GD32_PIN(133, B, 3),
    __GD32_PIN(134, B, 4),
    __GD32_PIN(135, B, 5),
    __GD32_PIN(136, B, 6),
    __GD32_PIN(137, B, 7),
    __GD32_PIN_DEFAULT,
    __GD32_PIN(139, B, 8),
    __GD32_PIN(140, B, 9),
    __GD32_PIN(141, E, 0),
    __GD32_PIN(142, E, 1),
    __GD32_PIN_DEFAULT,
    __GD32_PIN_DEFAULT,
#endif
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};
static const struct pin_irq_map pin_irq_map[] =
{
    {GPIO_PIN_0, EXTI0_IRQn},
    {GPIO_PIN_1, EXTI1_IRQn},
    {GPIO_PIN_2, EXTI2_IRQn},
    {GPIO_PIN_3, EXTI3_IRQn},
    {GPIO_PIN_4, EXTI4_IRQn},
    {GPIO_PIN_5, EXTI5_9_IRQn},
    {GPIO_PIN_6, EXTI5_9_IRQn},
    {GPIO_PIN_7, EXTI5_9_IRQn},
    {GPIO_PIN_8, EXTI5_9_IRQn},
    {GPIO_PIN_9, EXTI5_9_IRQn},
    {GPIO_PIN_10, EXTI10_15_IRQn},
    {GPIO_PIN_11, EXTI10_15_IRQn},
    {GPIO_PIN_12, EXTI10_15_IRQn},
    {GPIO_PIN_13, EXTI10_15_IRQn},
    {GPIO_PIN_14, EXTI10_15_IRQn},
    {GPIO_PIN_15, EXTI10_15_IRQn},
};
struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
};

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
const struct pin_index *get_pin(rt_uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = pins + pin;
        if (index->index == -1)
        index = RT_NULL;
    }
    else
    {
        index = RT_NULL;
    }

    return index;
};

void gd32_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;
    rt_uint32_t pin_mode;
    rt_uint32_t pin_pull;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    rcu_periph_clock_enable(index->clk);
    
   switch(mode)
   {
   case PIN_MODE_OUTPUT:
        /* output setting */
        pin_mode = GPIO_MODE_OUTPUT;
        pin_pull = GPIO_PUPD_NONE;
        gpio_output_options_set(index->gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, index->pin);
        break;
   case PIN_MODE_OUTPUT_OD:
        /* output setting: od. */
        pin_mode = GPIO_MODE_OUTPUT;
        pin_pull = GPIO_PUPD_NONE;
        gpio_output_options_set(index->gpio_periph, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, index->pin);
        break;
   case PIN_MODE_INPUT:
        /* input setting: not pull. */
        pin_mode = GPIO_MODE_INPUT;
        pin_pull = GPIO_PUPD_NONE;
        break;
   case PIN_MODE_INPUT_PULLUP:
        /* input setting: pull up. */
        pin_mode = GPIO_MODE_INPUT;
        pin_pull = GPIO_PUPD_PULLUP;
        break;
   case PIN_MODE_INPUT_PULLDOWN:
        /* input setting: pull down. */
        pin_mode = GPIO_MODE_INPUT;
        pin_pull = GPIO_PUPD_PULLDOWN;
        break;
   default:
        pin_mode = GPIO_MODE_INPUT;
        pin_pull = GPIO_PUPD_NONE;
        break;
   }

    gpio_mode_set(index->gpio_periph, pin_mode, pin_pull, index->pin);
}

void gd32_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    gpio_bit_write(index->gpio_periph, index->pin, (bit_status)value);
}

int gd32_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return value;
    }

    value = gpio_input_bit_get(index->gpio_periph, index->pin);

    return value;
}


rt_inline rt_int32_t bit2bitno(rt_uint32_t bit)
{
    rt_uint8_t i;
    for (i = 0; i < 32; i++)
    {
        if ((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}
rt_inline const struct pin_irq_map *get_pin_irq_map(rt_uint32_t pinbit)
{
    rt_int32_t mapindex = bit2bitno(pinbit);
    if (mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_NULL;
    }
    return &pin_irq_map[mapindex];
};
rt_err_t gd32_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                              rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t hdr_index = -1;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_EINVAL;
    }
    hdr_index = bit2bitno(index->pin);
    if (hdr_index < 0 || hdr_index >= ITEM_NUM(pin_irq_map))
    {
        return RT_EINVAL;
    }

    level = rt_hw_interrupt_disable();
    if (pin_irq_hdr_tab[hdr_index].pin == pin &&
        pin_irq_hdr_tab[hdr_index].hdr == hdr &&
        pin_irq_hdr_tab[hdr_index].mode == mode &&
        pin_irq_hdr_tab[hdr_index].args == args)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    if (pin_irq_hdr_tab[hdr_index].pin != -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EFULL;
    }
    pin_irq_hdr_tab[hdr_index].pin = pin;
    pin_irq_hdr_tab[hdr_index].hdr = hdr;
    pin_irq_hdr_tab[hdr_index].mode = mode;
    pin_irq_hdr_tab[hdr_index].args = args;
    rt_hw_interrupt_enable(level);

    return RT_EOK;
}
rt_err_t gd32_pin_detach_irq(struct rt_device *device, rt_int32_t pin)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t hdr_index = -1;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_EINVAL;
    }
    hdr_index = bit2bitno(index->pin);
    if (hdr_index < 0 || hdr_index >= ITEM_NUM(pin_irq_map))
    {
        return RT_EINVAL;
    }

    level = rt_hw_interrupt_disable();
    if (pin_irq_hdr_tab[hdr_index].pin == -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    pin_irq_hdr_tab[hdr_index].pin = -1;
    pin_irq_hdr_tab[hdr_index].hdr = RT_NULL;
    pin_irq_hdr_tab[hdr_index].mode = 0;
    pin_irq_hdr_tab[hdr_index].args = RT_NULL;
    rt_hw_interrupt_enable(level);

    return RT_EOK;
}
rt_err_t gd32_pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled)
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
    rt_base_t level;
    rt_int32_t hdr_index = -1;
    exti_trig_type_enum trigger_mode;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_EINVAL;
    }
    if (enabled == PIN_IRQ_ENABLE)
    {
        hdr_index = bit2bitno(index->pin);
        if (hdr_index < 0 || hdr_index >= ITEM_NUM(pin_irq_map))
        {
            return RT_EINVAL;
        }
        level = rt_hw_interrupt_disable();
        if (pin_irq_hdr_tab[hdr_index].pin == -1)
        {
            rt_hw_interrupt_enable(level);
            return RT_EINVAL;
        }
        irqmap = &pin_irq_map[hdr_index];
   
        switch (pin_irq_hdr_tab[hdr_index].mode)
        {
            case PIN_IRQ_MODE_RISING:
                trigger_mode = EXTI_TRIG_RISING;
                break;
            case PIN_IRQ_MODE_FALLING:
                trigger_mode = EXTI_TRIG_FALLING;
                break;
            case PIN_IRQ_MODE_RISING_FALLING:
                trigger_mode = EXTI_TRIG_BOTH;
                break;
            default:
                rt_hw_interrupt_enable(level);
                return RT_EINVAL;
        }

        rcu_periph_clock_enable(RCU_SYSCFG);

        /* enable and set interrupt priority */
        nvic_irq_enable(irqmap->irqno, 5U, 0U);
        
        /* connect EXTI line to  GPIO pin */
        //gpio_exti_source_select(index->port_src, index->pin_src);
        syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN13);

        /* configure EXTI line */
        exti_init((exti_line_enum)(index->pin), EXTI_INTERRUPT, trigger_mode);
        exti_interrupt_flag_clear((exti_line_enum)(index->pin));
        
        rt_hw_interrupt_enable(level);
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if (irqmap == RT_NULL)
        {
            return RT_EINVAL;
        }
        nvic_irq_disable(irqmap->irqno);
    }
    else
    {
        return RT_EINVAL;
    }

    return RT_EOK;
}
const static struct rt_pin_ops _gd32_pin_ops =
{
    gd32_pin_mode,
    gd32_pin_write,
    gd32_pin_read,
    gd32_pin_attach_irq,
    gd32_pin_detach_irq,
    gd32_pin_irq_enable,
    RT_NULL,
};

int rt_hw_pin_init(void)
{
    int result;

    result = rt_device_pin_register("pin", &_gd32_pin_ops, RT_NULL);
    
    return result;
}
INIT_BOARD_EXPORT(rt_hw_pin_init);

rt_inline void pin_irq_hdr(int irqno)
{
    if (pin_irq_hdr_tab[irqno].hdr)
    {
        pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}

void GD32_GPIO_EXTI_IRQHandler(rt_int8_t exti_line)
{
    if(RESET != exti_interrupt_flag_get((exti_line_enum)(1 << exti_line)))
    {
        pin_irq_hdr(exti_line);
        exti_interrupt_flag_clear((exti_line_enum)(1 << exti_line));
    } 
}
void EXTI0_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(0);
    rt_interrupt_leave();
}
void EXTI1_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(1);
    rt_interrupt_leave();
}
void EXTI2_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(2);
    rt_interrupt_leave();
}
void EXTI3_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(3);
    rt_interrupt_leave();
}
void EXTI4_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(4);
    rt_interrupt_leave();
}
void EXTI5_9_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(5);
    GD32_GPIO_EXTI_IRQHandler(6);
    GD32_GPIO_EXTI_IRQHandler(7);
    GD32_GPIO_EXTI_IRQHandler(8);
    GD32_GPIO_EXTI_IRQHandler(9);
    rt_interrupt_leave();
}
void EXTI10_15_IRQHandler(void)
{
    rt_interrupt_enter();
    GD32_GPIO_EXTI_IRQHandler(10);
    GD32_GPIO_EXTI_IRQHandler(11);
    GD32_GPIO_EXTI_IRQHandler(12);
    GD32_GPIO_EXTI_IRQHandler(13);
    GD32_GPIO_EXTI_IRQHandler(14);
    GD32_GPIO_EXTI_IRQHandler(15);
    rt_interrupt_leave();
}

#endif
