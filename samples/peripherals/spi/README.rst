.. _bm_spi_sample:

SPI
###

.. contents::
   :local:
   :depth: 2

The SPI sample demonstrates how to configure and use the SPIM and SPIS peripherals with the nrfx drivers.

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

The sample requires the following pin wiring:

Single-device loopback setup:
   Wire the four SPIM pins to the SPIS pins on the same board, matching each signal by name (SCK→SCK, MOSI→MOSI, MISO→MISO, CSN→CSN).

Two-device setup:
   Wire the four SPIM pins on device 1 to the SPIS pins on device 2, matching each signal by name (SCK→SCK, MOSI→MOSI, MISO→MISO, CSN→CSN).
   Optionally, wire the four SPIS pins on device 1 to the SPIM pins on device 2 to test SPIM and SPIS on both devices.

   .. note::

      The sample configures the SPIM peripheral with an extra delay between chip select (CSN) and the first SCK edge (see the :kconfig:option:`CONFIG_SAMPLE_SPI_CSN_TO_CLK_DELAY` Kconfig option).
      The delay is required when talking to an SPI target which needs more time to wake up, like the SPIS peripheral on nRF54L devices, where the high-frequency clock is powered down during sleep and must be running before data can be transferred correctly.
      Alternatively, the target can be configured to keep its high-frequency clock always on, which removes the need for the delay but increases power consumption.
      For more information, see the SPIS peripheral and electrical specification chapters in the product specification of your SoC.

.. include:: /includes/spi_board_connections.txt

.. note:: Board-specific behavior

   * **nRF54L15 DK** — The SPI controller pins overlap with **LED 3** (P1.14, flickers during transfers) and **Button 0** (P1.13, do not press during transfers).
   * **nRF54LV10 DK** — The SPI target pins (P0.00-P0.03) are shared with one of the debugger's virtual serial ports.
     Before running the sample, open the `Board Configurator`_ app in `nRF Connect for Desktop`_ and disable the **Connect port VCOM** entry that is mapped to pins **P0.00-P0.03**, to release these pins from the debugger.
     Leave the other **Connect port VCOM** entry (mapped to pins **P1.04-P1.07**) enabled, since it is the virtual serial port used to read the sample's log output.

Overview
********

The sample initializes the **SPIM** and **SPIS** instances with the pins configured in the :file:`board-config.h` header.
When **Button 2** is pressed, a configurable string (:kconfig:option:`CONFIG_SAMPLE_SPI_MSG`) is sent from the local SPI controller to a connected SPI target.
The local SPI target receives data from a connected SPI controller, logs the received data and toggles **LED 2**.

User interface
**************

LED 0:
   Lit when the device is initialized.

LED 2:
   Toggles when the SPIS peripheral completes a reception.

Button 2:
   Send string from the local SPIM to a connected SPI target.

Building and running
********************

This sample can be found under :file:`samples/peripherals/spi/` in the |BMshort| folder structure.

For details on how to create, configure, and program a sample, see :ref:`getting_started_with_the_samples`.

Testing
=======

You can test this sample by performing the following steps:

1. Compile and program the application.
#. Connect to the kit(s) with a terminal emulator (for example, the `Serial Terminal app`_).
#. In the Serial Terminal, observe that the ``SPI sample initialized`` message is printed.
#. Press **Button 2** to send the configurable string with the SPI controller.
#. Observe that the SPI target prints ``Message received`` and the string set by the :kconfig:option:`CONFIG_SAMPLE_SPI_MSG` Kconfig option (default ``Hello World!``).
#. Observe that **LED 2** toggles when the SPIS peripheral completes a reception.
