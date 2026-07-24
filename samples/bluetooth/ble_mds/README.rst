.. _ble_mds_sample:

Bluetooth: Memfault Diagnostic Service (MDS)
############################################

.. contents::
   :local:
   :depth: 2

The Memfault Diagnostic Service sample demonstrates how to expose diagnostic data collected by the `Memfault SDK`_ over Bluetooth LE using |BMlong|.
The sample advertises the Memfault Diagnostic Service (MDS) and the Battery Service.
A Bluetooth gateway can connect to the device, read the Memfault upload information, and stream diagnostic chunks to the Memfault cloud.

Requirements
************

The sample supports the following development kits:

.. tabs::

   .. group-tab:: Simple board variants

      The following board variants do **not** have DFU capabilities:

      .. include:: /includes/supported_boards_all_non-mcuboot_variants_s115.txt

      .. include:: /includes/supported_boards_all_non-mcuboot_variants_s145.txt

   .. group-tab:: MCUboot board variants

      The following board variants have DFU capabilities:

      .. include:: /includes/supported_boards_all_mcuboot_variants_s115.txt

      .. include:: /includes/supported_boards_all_mcuboot_variants_s145.txt

Overview
********

The sample uses the bare-metal SoftDevice Bluetooth libraries to initialize advertising, the Battery Service, the Device Information Service, and MDS.
The Memfault SDK collects reboot information, logs, metrics, trace events, and coredumps.
When an MDS gateway subscribes to the Data Export characteristic and enables streaming, the application calls :c:func:`ble_mds_process` from the main loop to send pending Memfault chunks.

.. _ble_mds_sample_getting_started_memfault:

Getting started with Memfault
*****************************

To view decoded metrics, trace events, and coredumps from this sample in the Memfault web UI, complete the cloud setup below before testing with `nRF Connect Device Manager`_.

Memfault account and project
============================

Memfault remote diagnostics are integrated with `nRF Cloud`_.
If you do not already have access:

1. Register through the `nRF Cloud Memfault registration`_ page.
2. `Create a Memfault project`_ for the fleet you want to monitor.
   Each project has a unique **project key**.
3. Set the project key in :kconfig:option:`CONFIG_MEMFAULT_NCS_PROJECT_KEY` in :file:`prj.conf` before building for real use (replace the placeholder key used for CI builds).

The key must match the Memfault project where you expect the device to appear.
For a broader overview of Memfault in |NCS|, see `nRF Cloud powered by Memfault`_.

Symbol file upload
==================

Memfault must associate uploaded diagnostic data with the firmware that produced it.
For **each firmware version** you build and run:

1. Build and program the sample.
2. Upload the ELF symbol file from the build output: :file:`<build>/ble_mds/zephyr/zephyr.elf`.
3. In the `Memfault Dashboard`_, link that symbol file to the **same** ``software_version`` string the device reports.

The sample reports ``software_version`` from the :file:`VERSION` file (see :ref:`ble_mds_sample_firmware_version`).
The version in Memfault must match, or decoding will fail or be incomplete.

Symbol files are required to decode:

* `Memfault: Coredumps`_
* `Memfault: Error Tracking with Trace Events`_ with logs
* Other symbolicated debug information shown in the Memfault UI

Without a matching symbol file, chunks may upload successfully but appear with limited or unusable detail.

Typical upload flow in the Memfault web UI:

1. Log in to the `Memfault Dashboard`_ and open your project.
2. Navigate to :guilabel:`Fleet` > :guilabel:`Devices`.
3. Connect the sample with `nRF Connect Device Manager`_ and generate some diagnostic data.
4. Select the **software version** reported by your device.
5. Click :guilabel:`Upload` and select :file:`zephyr.elf` from your build directory.

Upload a new symbol file whenever you change :file:`VERSION` or release a new firmware build.

Data upload over Bluetooth LE
=============================

This sample does not send data to Memfault over IP.
Instead, it uses the Memfault Diagnostic Service (MDS) over Bluetooth LE:

1. The device exposes upload metadata (device ID, URI, and authorization) over GATT.
2. `nRF Connect Device Manager`_ acts as the MDS gateway: it reads that metadata and forwards chunks to Memfault.
3. The firmware calls :c:func:`ble_mds_process` from the main loop while a gateway has subscribed and enabled streaming.

See :ref:`lib_ble_service_mds` for the MDS library API and :ref:`memfault_bm` for bare-metal Memfault usage rules.

Metrics
=======

The sample defines the following application metrics in :file:`samples/bluetooth/ble_mds/memfault_config/memfault_metrics_heartbeat_config.def`:

* ``button_press_count`` - The number of **Button 2** presses.
* ``battery_soc_pct`` - The simulated battery level.
* ``button_elapsed_time_ms`` - The time measured between two **Button 0** presses.

Pressing **Button 0** the second time stops the timer and triggers a heartbeat collection.

Trace events
============

The sample defines the ``button_state_changed`` trace reason in :file:`samples/bluetooth/ble_mds/memfault_config/memfault_trace_reason_user_config.def`.
The event is collected when **Button 1** changes state.

Core dumps
==========

Press **Button 3** to trigger a hardfault exception by division by zero.
After reboot, reconnect with an MDS gateway to transfer the collected coredump data to Memfault.

User interface
**************

LED 0:
   Lit when the device is initialized.

LED 1:
   Lit when a device is connected.

Button 0:
   Starts or stops the ``button_elapsed_time_ms`` metric timer.
   The second press stops the timer and triggers a Memfault heartbeat.

Button 1:
   Records the ``button_state_changed`` trace event on press and release.

Button 2:
   Increments the ``button_press_count`` metric and triggers a Memfault heartbeat.

Button 3:
   Simulates a crash by triggering a hardfault exception.

Configuration
*************

The sample configuration is split between the :file:`prj.conf` file and sample-specific Kconfig options in :file:`Kconfig`.

Before testing with a real gateway, complete :ref:`ble_mds_sample_getting_started_memfault`.
The sample uses :kconfig:option:`CONFIG_MEMFAULT_NCS_DEVICE_ID_HW_ID` with :kconfig:option:`CONFIG_HW_ID_LIBRARY_SOURCE_DEVICE_ID` so each device reports a hardware-derived Memfault device ID.
It selects :kconfig:option:`CONFIG_MEMFAULT_REBOOT_REASON_GET_BASIC` for compatibility across the bare-metal board variants supported by this sample.
Bare-metal builds enable :kconfig:option:`CONFIG_NCS_BM_MEMFAULT_LOCK` by default so Memfault uses an ``irq_lock()``-based lock instead of Zephyr mutexes.

For rules on which Memfault APIs may be called from ISRs versus the main loop, and why, see :ref:`memfault_bm`.
The sample defers heartbeat serialization to the main loop while keeping metric timer start/stop in the button ISR so elapsed-time measurements stay accurate.

You can configure the advertising name with the :kconfig:option:`CONFIG_SAMPLE_BLE_DEVICE_NAME` Kconfig option.

The MDS library settings use the ``CONFIG_BLE_MDS`` prefix.
See :ref:`lib_ble_service_mds` for service API and integration details.
For Memfault SDK options that are not configurable through Kconfig, use :file:`samples/bluetooth/ble_mds/memfault_config/memfault_platform_config.h`.

.. _ble_mds_sample_firmware_version:

Firmware version
================

Firmware version strings are managed from a single :file:`VERSION` file in the application root directory.
Update ``VERSION_MAJOR``, ``VERSION_MINOR``, and ``PATCHLEVEL`` there when you release a new build.
The sample currently ships with version ``0.1.0``.

:file:`Kconfig` propagates ``$(APPVERSION)`` from that file to both Memfault and DIS:

* :kconfig:option:`CONFIG_MEMFAULT_NCS_FW_VERSION` → Memfault ``software_version``
* :kconfig:option:`CONFIG_BLE_DIS_FW_REVISION` → DIS Firmware Revision characteristic

Related fields (software type and hardware revision) are aligned the same way; see the mapping table in :file:`Kconfig`.

:file:`prj.conf` enables :kconfig:option:`CONFIG_MEMFAULT_NCS_FW_VERSION_STATIC` so Memfault reports the static version from the :file:`VERSION` file.
Without this option, the NCS Memfault integration defaults to a dynamic ``<prefix>+<build-id>`` string that does not match DIS.
Upload a matching :file:`zephyr.elf` symbol file to Memfault for each ``software_version`` you deploy (see :ref:`ble_mds_sample_getting_started_memfault`).

To change the hardware revision string, update the default for :kconfig:option:`CONFIG_MEMFAULT_NCS_HW_VERSION` in :file:`Kconfig`.
The device serial number remains configured separately through :kconfig:option:`CONFIG_BLE_DIS_SERIAL_NUMBER` in :file:`prj.conf`.

Building and running
********************

This sample can be found under :file:`samples/bluetooth/ble_mds/` in the |BMshort| folder structure.

For details on how to create, configure, and program a sample, see :ref:`getting_started_with_the_samples`.

Testing the sample
==================

Test this sample with `nRF Connect Device Manager`_ after completing :ref:`ble_mds_sample_getting_started_memfault`.

1. Compile and program the application.
#. Connect to the kit with a terminal emulator, for example the `Serial Terminal app`_.
#. Reset the kit.
#. In the terminal, observe that the ``BLE MDS sample initialized`` message is printed.
#. Observe that the ``Advertising as nRF_BM_MDS`` message is printed.
   You can configure this name using the :kconfig:option:`CONFIG_SAMPLE_BLE_DEVICE_NAME` Kconfig option.
#. Open `nRF Connect Device Manager`_ and scan for devices.
#. Connect to the device and open the diagnostics view.
#. Use the buttons to generate metrics, trace events, and a coredump.
#. Explore the Memfault web UI to inspect the uploaded data.

Dependencies
************

This sample uses the following |BMshort| libraries:

* :c:func:`ble_adv_init`
* :c:func:`ble_bas_init`
* :c:func:`ble_dis_init`
* :c:func:`ble_mds_init`
* :c:func:`bm_buttons_init`
* :c:func:`bm_timer_init`

In addition, it uses the `Memfault firmware SDK`_.

.. _`Memfault firmware SDK`: https://github.com/memfault/memfault-firmware-sdk
