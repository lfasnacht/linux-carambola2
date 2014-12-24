/*
 *  8devices Carambola2 board support
 *
 *  Copyright (C) 2013 Darius Augulis <darius@8devices.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include "common.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define CARAMBOLA2_GPIO_LED_WLAN		0
#define CARAMBOLA2_GPIO_LED_ETH0		14
#define CARAMBOLA2_GPIO_LED_ETH1		13

#define CARAMBOLA2_GPIO_BTN_JUMPSTART		11

#define CARAMBOLA2_KEYS_POLL_INTERVAL		20	/* msecs */
#define CARAMBOLA2_KEYS_DEBOUNCE_INTERVAL	(3 * CARAMBOLA2_KEYS_POLL_INTERVAL)

#define CARAMBOLA2_MAC0_OFFSET			0x0000
#define CARAMBOLA2_MAC1_OFFSET			0x0006
#define CARAMBOLA2_CALDATA_OFFSET		0x1000
#define CARAMBOLA2_WMAC_MAC_OFFSET		0x1002

static struct gpio_led carambola2_leds_gpio[] __initdata = {
	{
		.name		= "carambola2:green:wlan",
		.gpio		= CARAMBOLA2_GPIO_LED_WLAN,
		.active_low	= 1,
	}, {
		.name		= "carambola2:orange:eth0",
		.gpio		= CARAMBOLA2_GPIO_LED_ETH0,
		.active_low	= 0,
	}, {
		.name		= "carambola2:orange:eth1",
		.gpio		= CARAMBOLA2_GPIO_LED_ETH1,
		.active_low	= 0,
	}
};

static struct gpio_keys_button carambola2_gpio_keys[] __initdata = {
	{
		.desc		= "jumpstart button",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = CARAMBOLA2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= CARAMBOLA2_GPIO_BTN_JUMPSTART,
		.active_low	= 1,
	},
};

static struct spi_board_info carambola_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "w25q128",
		.controller_data = NULL,
		.platform_data = NULL
	}
};

static struct ath79_spi_platform_data carambola_spi_data = {
	.bus_num	= 0,
	.num_chipselect = 1,
};

static void __init carambola2_common_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_spi(&carambola_spi_data, carambola_spi_info,
			    ARRAY_SIZE(carambola_spi_info));
	ath79_register_wmac(art + CARAMBOLA2_CALDATA_OFFSET);
}

static void __init carambola2_setup(void)
{
	carambola2_common_setup();

	ath79_register_leds_gpio(-1, ARRAY_SIZE(carambola2_leds_gpio),
				 carambola2_leds_gpio);
	ath79_register_gpio_keys_polled(-1, CARAMBOLA2_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(carambola2_gpio_keys),
					carambola2_gpio_keys);
	ath79_register_usb();
}

MIPS_MACHINE(ATH79_MACH_CARAMBOLA2, "CARAMBOLA2", "8devices Carambola2 board",
		carambola2_setup);
