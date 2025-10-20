/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SDH_CLOCK_CONTROL_NRFX_POWER_CLOCK_H__
#define SDH_CLOCK_CONTROL_NRFX_POWER_CLOCK_H__

#if (NRFX_CLOCK_ENABLED != 0)
#error "Expected disabled nrfx_clock"
#endif

/* If this file is included then it means that nrfx_clock_sdh is being used. */
#undef NRFX_CLOCK_ENABLED
#define NRFX_CLOCK_ENABLED 1

#include <drivers/include/nrfx_power_clock.h>

#endif /* SDH_CLOCK_CONTROL_NRFX_POWER_CLOCK_H__ */
