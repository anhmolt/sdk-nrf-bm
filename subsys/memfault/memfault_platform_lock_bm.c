/*
 * Copyright (c) 2026 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>

#include "memfault/core/platform/overrides.h"

/*
 * Bare-metal Memfault locking uses irq_lock()/irq_unlock() instead of k_mutex.
 *
 * On BM builds (CONFIG_MULTITHREADING=n) there are no preemptive threads, but ISRs
 * can still preempt the main loop. Memfault APIs may be invoked from both contexts
 * (for example metrics from k_timer ISRs and export from the main loop).
 *
 * This file replaces memfault_platform_lock.c from the Memfault Zephyr port when
 * CONFIG_NCS_BM_MEMFAULT_LOCK is enabled (that SDK source is excluded in
 * subsys/memfault/CMakeLists.txt).
 *
 * See doc/nrf-bm/libraries/memfault.rst for usage rules (ISR vs main loop).
 */

static unsigned int bm_memfault_lock_nesting;
static unsigned int bm_memfault_lock_irq_key;

void memfault_lock(void)
{
	if (bm_memfault_lock_nesting++ == 0U) {
		bm_memfault_lock_irq_key = irq_lock();
	}
}

void memfault_unlock(void)
{
	__ASSERT(bm_memfault_lock_nesting > 0U, "Unbalanced memfault_unlock()");

	if (--bm_memfault_lock_nesting == 0U) {
		irq_unlock(bm_memfault_lock_irq_key);
	}
}
