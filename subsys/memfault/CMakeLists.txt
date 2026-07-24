#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NCS_BM_MEMFAULT_LOCK)
  zephyr_library()
  zephyr_library_sources(memfault_platform_lock_bm.c)

  # Drop memfault_platform_lock.c from the Memfault SDK library. Linker --wrap is
  # not sufficient because K_MUTEX_DEFINE still pulls that object in and requires
  # k_mutex symbols that are unavailable with CONFIG_MULTITHREADING=n.
  if(DEFINED ZEPHYR_MEMFAULT_FIRMWARE_SDK_MODULE_DIR)
    set(_memfault_lock_src
      "${ZEPHYR_MEMFAULT_FIRMWARE_SDK_MODULE_DIR}/ports/zephyr/common/memfault_platform_lock.c")
    if(EXISTS "${_memfault_lock_src}")
      set_source_files_properties("${_memfault_lock_src}"
        PROPERTIES EXCLUDE_FROM_BUILD TRUE)
    endif()
  endif()

  get_property(zephyr_libs GLOBAL PROPERTY ZEPHYR_LIBS)
  foreach(lib ${zephyr_libs})
    if(NOT TARGET ${lib})
      continue()
    endif()
    get_target_property(sources ${lib} SOURCES)
    if(NOT sources OR sources STREQUAL "sources-NOTFOUND")
      continue()
    endif()

    set(filtered ${sources})
    foreach(src ${sources})
      if(src MATCHES "^\\$<")
        continue()
      endif()
      if(src MATCHES "(/|\\\\)memfault_platform_lock\\.c$")
        list(REMOVE_ITEM filtered ${src})
      endif()
    endforeach()

    if(NOT "${sources}" STREQUAL "${filtered}")
      set_target_properties(${lib} PROPERTIES SOURCES "${filtered}")
    endif()
  endforeach()
endif()
