/*
 * Copyright (c) 2018 Henrik Brix Andersen <henrik@brixandersen.dk>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#ifndef IBEACON_RSSI
#define IBEACON_RSSI 0xc8
#endif

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
/*
 * Set iBeacon demo advertisement data. These values are for
 * demonstration only and must be changed for production environments!
 *
 * UUID:  18ee1516-016b-4bec-ad96-bcb96d166e97
 * Major: 0
 * Minor: 0
 * RSSI:  -56 dBm
 */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	  BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA,
	0x4e, 0x0d, /* Apple Company ID */
	0x02, 0x15, /* iBeacon Type & Length */
	0x4e, 0x49, 0x4b, 0x41, 0x54, 0x54, 0x45, 0x43, /* UUID */
	0x48, 0x42, 0x45, 0x41, 0x43, 0x4f, 0x4e, 0x53, /* UUID */
	0x00, 0x03, /* Index 20, 21: Major */
	0x00, 0x01, /* Index 22, 23: Minor */
	IBEACON_RSSI /* Calibrated RSSI @ 1m */
                ),
};
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("iBeacon started\n");
}

int main(void)
{
	int err;

	printk("Starting iBeacon Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
	return 0;
}
