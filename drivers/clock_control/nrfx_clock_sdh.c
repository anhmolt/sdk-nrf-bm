/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <nrfx_clock.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(nrfx_clock_sdh, CONFIG_CLOCK_CONTROL_LOG_LEVEL);

static nrfx_clock_event_handler_t event_handler;

void nrfx_clock_start(nrf_clock_domain_t domain)
{
	switch (domain) {
	case NRF_CLOCK_DOMAIN_LFCLK:
		event_handler(NRFX_CLOCK_EVT_LFCLK_STARTED); // ?
		break;
	case NRF_CLOCK_DOMAIN_HFCLK:
		(void)sd_clock_hfclk_request();
		break;
	default:
		__ASSERT(0, "Not supported");
		break;
	}
}

void nrfx_clock_stop(nrf_clock_domain_t domain)
{
	switch (domain) {
	case NRF_CLOCK_DOMAIN_LFCLK:
		/* empty */
		break;
	case NRF_CLOCK_DOMAIN_HFCLK:
		(void)sd_clock_hfclk_release();
		break;
	default:
		__ASSERT(0, "Not supported");
		break;
	}
}

void nrfx_clock_enable(void)
{

}

nrfx_err_t nrfx_clock_init(nrfx_clock_event_handler_t handler)
{
	event_handler = handler;

	return NRFX_SUCCESS;
}

extern void CLOCK_POWER_IRQHandler(void);

void nrfx_clock_irq_handler(void)
{
	CLOCK_POWER_IRQHandler();
}
