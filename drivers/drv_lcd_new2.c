/*
 * File      : usart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-08-23     Tanek        the first version
 */

#include "gd32f450z_lcd_eval.h"
#include <gd32f4xx.h>
#include <drv_usart.h>
#include <board.h>
#include <finsh.h>
#include "image1.h"
#include "logo.h"

#ifdef PKG_USING_GUIENGINE


#define RT_HW_LCD_WIDTH                ((uint16_t)320)     /* LCD PIXEL WIDTH            */
#define RT_HW_LCD_HEIGHT               ((uint16_t)480)     /* LCD PIXEL HEIGHT           */

#define LCD_480_320_HSYNC              ((uint32_t)10)      /* Horizontal synchronization */
#define LCD_480_320_HBP                ((uint32_t)20)     /* Horizontal back porch      */
#define LCD_480_320_HFP                ((uint32_t)40)      /* Horizontal front porch     */
#define LCD_480_320_VSYNC              ((uint32_t)2)       /* Vertical synchronization   */
#define LCD_480_320_VBP                ((uint32_t)1)      /* Vertical back porch        */
#define LCD_480_320_VFP                ((uint32_t)4)      /* Vertical front porch       */

#define LCD_BITS_PER_PIXEL             16

#define HORIZONTAL_SYNCHRONOUS_PULSE  10
#define HORIZONTAL_BACK_PORCH         20
#define ACTIVE_WIDTH                  320
#define HORIZONTAL_FRONT_PORCH        40

#define VERTICAL_SYNCHRONOUS_PULSE    2
#define VERTICAL_BACK_PORCH           2
#define ACTIVE_HEIGHT                 480
#define VERTICAL_FRONT_PORCH          4



static rt_uint16_t *lcd_framebuffer = RT_NULL;
static rt_uint16_t *_rt_framebuffer = RT_NULL;
uint8_t blended_address_buffer[87680];

static struct rt_device_graphic_info _lcd_info;
static struct rt_device lcd;

/*!
    \brief      configure TLI GPIO  
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_gpio_config(void)
{
    /* enable the periphral clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    /* configure HSYNC(PC6), VSYNC(PA4), PCLK(PG7), DE(PF10) */
    /* configure LCD_R7(PG6), LCD_R6(PA8), LCD_R5(PA12), LCD_R4(PA11), LCD_R3(PB0), 
                 LCD_G7(PD3), LCD_G6(PC7), LCD_G5(PB11), LCD_G4(PB10), LCD_G3(PG10), LCD_G2(PA6),
                 LCD_B7(PB9), LCD_B6(PB8), LCD_B5(PA3), LCD_B4(PG12), LCD_B3(PG11) */
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_3);
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_4);
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_6);
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_8);
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_11);
    gpio_af_set(GPIOA,GPIO_AF_14,GPIO_PIN_12);
    
    gpio_af_set(GPIOB,GPIO_AF_9,GPIO_PIN_0);
    gpio_af_set(GPIOB,GPIO_AF_14,GPIO_PIN_8);
    gpio_af_set(GPIOB,GPIO_AF_14,GPIO_PIN_9);
    gpio_af_set(GPIOB,GPIO_AF_14,GPIO_PIN_10);
    gpio_af_set(GPIOB,GPIO_AF_14,GPIO_PIN_11);
    
    gpio_af_set(GPIOC,GPIO_AF_14,GPIO_PIN_6);
    gpio_af_set(GPIOC,GPIO_AF_14,GPIO_PIN_7);
    
    gpio_af_set(GPIOD,GPIO_AF_14,GPIO_PIN_3);

    gpio_af_set(GPIOF,GPIO_AF_14,GPIO_PIN_10);
    
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_6);
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_7);
    gpio_af_set(GPIOG,GPIO_AF_9,GPIO_PIN_10);
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_11);
    gpio_af_set(GPIOG,GPIO_AF_9,GPIO_PIN_12);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6
                                |GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3|GPIO_PIN_4
                                |GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                                |GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                                |GPIO_PIN_11);

    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_6|GPIO_PIN_7);

    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11
                                |GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11
                                |GPIO_PIN_12);
}

/*!
    \brief      LCD Configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_config(void)
{
    /* configure the LCD control line */
    lcd_ctrl_line_config();
    lcd_disable();
    lcd_enable();
    
    /* configure the GPIO of TLI */
    tli_gpio_config();
    /* configure the LCD_SPI */
    lcd_spi_config(); 

    /* power on the LCD */
    //lcd_power_on();
	lcd_power_on1();  //New Version 3.5" TFT RGB Hardware needs use this initilize funtion  ---By xufei 2016.10.21
}

/*!
    \brief      configure TLI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_config(void)
{
    tli_parameter_struct               tli_init_struct;
    tli_layer_parameter_struct         tli_layer_init_struct;

    /* initialize the parameters of structure */
    tli_struct_para_init(&tli_init_struct);
    tli_layer_struct_para_init(&tli_layer_init_struct);
    
    rcu_periph_clock_enable(RCU_TLI);
    tli_gpio_config();

    /* configure the PLLSAI clock to generate lcd clock */
    if(ERROR == rcu_pllsai_config(128, 2, 3)){
        while(1);
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV4);
    rcu_osci_on(RCU_PLLSAI_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK)){
        while(1);
    }
    
    /* TLI initialization */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
    
    tli_init_struct.synpsz_hpsz = HORIZONTAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.synpsz_vpsz = VERTICAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.backpsz_hbpsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;
    tli_init_struct.backpsz_vbpsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    tli_init_struct.activesz_hasz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
    tli_init_struct.activesz_vasz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
    tli_init_struct.totalsz_htsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1;
    tli_init_struct.totalsz_vtsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;
    /* LCD background color configure*/
    tli_init_struct.backcolor_red = 0xFF;
    tli_init_struct.backcolor_green = 0xFF;
    tli_init_struct.backcolor_blue = 0xFF; 
    tli_init(&tli_init_struct);

    /* TLI layer0 configuration */
    tli_layer_init_struct.layer_window_leftpos = (0 + 147 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH);
    tli_layer_init_struct.layer_window_rightpos = (162 + 147 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = (0 + 90 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH);
    tli_layer_init_struct.layer_window_bottompos = (320 + 90 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    tli_layer_init_struct.layer_sa = 0xFF;
    tli_layer_init_struct.layer_default_blue = 0xFF;
    tli_layer_init_struct.layer_default_green = 0xFF;
    tli_layer_init_struct.layer_default_red = 0xFF;
    tli_layer_init_struct.layer_default_alpha = 0x0;
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_PASA;
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)gImage_logo;
    tli_layer_init_struct.layer_frame_line_length = ((162 * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (162 * 2);
    tli_layer_init_struct.layer_frame_total_line_number = 320;
    tli_layer_init(LAYER0, &tli_layer_init_struct);
}

static rt_err_t rt_lcd_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
    {        
        memcpy((void *)lcd_framebuffer, _rt_framebuffer, sizeof(rt_uint16_t)*RT_HW_LCD_HEIGHT * RT_HW_LCD_WIDTH);
    }
    break;
    
    case RTGRAPHIC_CTRL_POWERON:
        break;
    
    case RTGRAPHIC_CTRL_POWEROFF:
        break;
    
    case RTGRAPHIC_CTRL_GET_INFO:
        memcpy(args, &_lcd_info, sizeof(_lcd_info));
        break;
    
    case RTGRAPHIC_CTRL_SET_MODE:
        break;
    }

    return RT_EOK;
}

/*!
    \brief      IPA initialize and configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ipa_config(uint32_t baseaddress)
{
    ipa_destination_parameter_struct  ipa_destination_init_struct;
    ipa_foreground_parameter_struct   ipa_fg_init_struct;  
  
    /* initialize the parameters of structure */
    ipa_foreground_struct_para_init(&ipa_fg_init_struct);
    ipa_destination_struct_para_init(&ipa_destination_init_struct);
    
    rcu_periph_clock_enable(RCU_IPA);
   
    ipa_deinit();
    /* IPA pixel format convert mode configure */
    ipa_pixel_format_convert_mode_set(IPA_FGTODE);
    /* destination pixel format configure */
    ipa_destination_init_struct.destination_pf = IPA_DPF_RGB565;  
    /* destination memory base address configure */
    ipa_destination_init_struct.destination_memaddr = (uint32_t)&blended_address_buffer;
    /* destination pre-defined alpha value RGB configure */
    ipa_destination_init_struct.destination_pregreen = 0;
    ipa_destination_init_struct.destination_preblue = 0;
    ipa_destination_init_struct.destination_prered = 0;
    ipa_destination_init_struct.destination_prealpha = 0;
    /* destination line offset configure */
    ipa_destination_init_struct.destination_lineoff = 0;
    /* height of the image to be processed configure */
    ipa_destination_init_struct.image_height = 137;
    /* width of the image to be processed configure */
    ipa_destination_init_struct.image_width = 320;
    /* IPA destination initialization */
    ipa_destination_init(&ipa_destination_init_struct);
  
    /* IPA foreground configure */
    ipa_fg_init_struct.foreground_memaddr = baseaddress;
    ipa_fg_init_struct.foreground_pf = FOREGROUND_PPF_RGB565;
    ipa_fg_init_struct.foreground_alpha_algorithm = IPA_FG_ALPHA_MODE_0;
    ipa_fg_init_struct.foreground_prealpha = 0;
    ipa_fg_init_struct.foreground_lineoff = 0;
    ipa_fg_init_struct.foreground_preblue = 0;
    ipa_fg_init_struct.foreground_pregreen = 0;
    ipa_fg_init_struct.foreground_prered = 0;
    /* foreground initialization */
    ipa_foreground_init(&ipa_fg_init_struct);
}

/*!
    \brief      configure TLI peripheral and display blend image
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_blend_config(void)
{
    tli_layer_parameter_struct         tli_layer_init_struct;
    /* initialize the parameters of structure */
    tli_layer_struct_para_init(&tli_layer_init_struct);
  
    /* TLI layer1 configuration */
    tli_layer_init_struct.layer_window_leftpos = (0 + 0 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH);
    tli_layer_init_struct.layer_window_rightpos = (137 + 0 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = (0 + 100 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH);
    tli_layer_init_struct.layer_window_bottompos = (320 + 100 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    tli_layer_init_struct.layer_sa = 0xFF; 
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_PASA;
    tli_layer_init_struct.layer_default_alpha = 0;
    tli_layer_init_struct.layer_default_blue = 0;
    tli_layer_init_struct.layer_default_green = 0;
    tli_layer_init_struct.layer_default_red = 0;
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&blended_address_buffer;
    tli_layer_init_struct.layer_frame_line_length = ((137 * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (137 * 2);
    tli_layer_init_struct.layer_frame_total_line_number = 320;
    tli_layer_init(LAYER1, &tli_layer_init_struct);
}

int gd32_hw_lcd_init(void)
{
    _rt_framebuffer = rt_malloc_align(sizeof(rt_uint16_t) * RT_HW_LCD_WIDTH * RT_HW_LCD_HEIGHT, 32);
    if (_rt_framebuffer == RT_NULL) 
        return -1; /* no memory yet */
    
    lcd_config();
    tli_config();
    tli_layer_enable(LAYER0);  
    tli_reload_config(TLI_FRAME_BLANK_RELOAD_EN);
    tli_enable();
    //tli_blend_config();
    //tli_reload_config(TLI_REQUEST_RELOAD_EN);

    //ipa_config((uint32_t)&gImage_logo);
    //ipa_transfer_enable(); 
    //while(RESET == ipa_interrupt_flag_get(IPA_INT_FLAG_FTF));

    //ipa_config((uint32_t)&gImage_image1);
    //ipa_transfer_enable(); 
    //while(RESET == ipa_interrupt_flag_get(IPA_INT_FLAG_FTF));
    
    _lcd_info.bits_per_pixel = LCD_BITS_PER_PIXEL;
    _lcd_info.pixel_format   = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    _lcd_info.framebuffer    = (void *)_rt_framebuffer;
    _lcd_info.width          = RT_HW_LCD_WIDTH;
    _lcd_info.height         = RT_HW_LCD_HEIGHT;
    
    lcd.type    = RT_Device_Class_Graphic;
    lcd.init    = NULL;
    lcd.open    = NULL;
    lcd.close   = NULL;
    lcd.read    = NULL;
    lcd.write   = NULL;
    lcd.control = rt_lcd_control;
    lcd.user_data = (void *)&_lcd_info;
    
    /* register lcd device to RT-Thread */
    rt_device_register(&lcd, "lcd", RT_DEVICE_FLAG_RDWR);
    
    return 0;
}
INIT_DEVICE_EXPORT(gd32_hw_lcd_init);
#endif
