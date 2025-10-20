#
# Copyright (c) 2025 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_library_amend()

if(CONFIG_CLOCK_CONTROL_SDH)
  zephyr_library_sources(nrfx_clock_sdh.c)

  target_include_directories(zephyr_interface BEFORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
