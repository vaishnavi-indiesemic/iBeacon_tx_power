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
//-20
#define IBEACON_RSSI 0x38
//-40
// #define IBEACON_RSSI 0x5a
#endif

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
	0x00, 0x04, /* Index 20, 21: Major */
	0x00, 0x05, /* Index 22, 23:(1-15) Minor */
	IBEACON_RSSI /* Calibrated RSSI @ 1m */
                ),
};
static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");
	/* Start advertising */
	struct bt_le_adv_param adv_param = {
        .options = BT_LE_ADV_OPT_NONE,
        .interval_min = 1600, 
        .interval_max = 1600,
        .id = BT_ID_DEFAULT,
    };

    /* Start advertising using the custom interval parameters */
    err = bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), NULL, 0);
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
