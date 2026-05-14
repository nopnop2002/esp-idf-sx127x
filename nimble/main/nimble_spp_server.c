/*	ESP-NimBLE SPP Server Example

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"

/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static int ble_spp_server_gap_event(struct ble_gap_event *event, void *arg);
static uint8_t own_addr_type;
int gatt_svr_register(void);
static bool conn_handle_subs[CONFIG_BT_NIMBLE_MAX_CONNECTIONS + 1];
static uint16_t ble_spp_svc_gatt_read_val_handle;

#define DEVICE_NAME "ESP_NIMBLE_SERVER" //The Device Name Characteristics in GAP

// 16 Bit SPP Service UUID
#define BLE_SVC_SPP_UUID16 0xABF0

// 16 Bit SPP Service Characteristic UUID
#define BLE_SVC_SPP_CHR_UUID16 0xABF1

static const char *TAG = "SPP";

extern MessageBufferHandle_t xMessageBufferTrans;
extern MessageBufferHandle_t xMessageBufferRecv;
extern size_t xItemSize;

void ble_store_config_init(void);

/**
 * Logs information about a connection to the console.
 */
static void
ble_spp_server_print_conn_desc(struct ble_gap_conn_desc *desc)
{
	ESP_LOGI(pcTaskGetName(NULL), "handle=%d our_ota_addr_type=%d our_ota_addr=",
		desc->conn_handle, desc->our_ota_addr.type);
	//print_addr(desc->our_ota_addr.val);
	ESP_LOGI(pcTaskGetName(NULL), "our_id_addr_type=%d our_id_addr=",
		desc->our_id_addr.type);
	//print_addr(desc->our_id_addr.val);
	ESP_LOGI(pcTaskGetName(NULL), "peer_ota_addr_type=%d peer_ota_addr=",
		desc->peer_ota_addr.type);
	//print_addr(desc->peer_ota_addr.val);
	ESP_LOGI(pcTaskGetName(NULL), "peer_id_addr_type=%d peer_id_addr=",
		desc->peer_id_addr.type);
	//print_addr(desc->peer_id_addr.val);
	ESP_LOGI(pcTaskGetName(NULL), "conn_itvl=%d conn_latency=%d supervision_timeout=%d "
		"encrypted=%d authenticated=%d bonded=%d\n",
		desc->conn_itvl, desc->conn_latency,
		desc->supervision_timeout,
		desc->sec_state.encrypted,
		desc->sec_state.authenticated,
		desc->sec_state.bonded);
}

/**
 * Enables advertising with the following parameters:
 *	   o General discoverable mode.
 *	   o Undirected connectable mode.
 */
static void
ble_spp_server_advertise(void)
{
	struct ble_gap_adv_params adv_params;
	struct ble_hs_adv_fields fields;
	const char *name;
	int rc;

	/**
	 *	Set the advertisement data included in our advertisements:
	 *	   o Flags (indicates advertisement type and other general info).
	 *	   o Advertising tx power.
	 *	   o Device name.
	 *	   o 16-bit service UUIDs (alert notifications).
	 */

	memset(&fields, 0, sizeof fields);

	/* Advertise two flags:
	 *	   o Discoverability in forthcoming advertisement (general)
	 *	   o BLE-only (BR/EDR unsupported).
	 */
	fields.flags = BLE_HS_ADV_F_DISC_GEN |
				   BLE_HS_ADV_F_BREDR_UNSUP;

	/* Indicate that the TX power level field should be included; have the
	 * stack fill this value automatically.  This is done by assigning the
	 * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
	 */
	fields.tx_pwr_lvl_is_present = 1;
	fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

	name = ble_svc_gap_device_name();
	fields.name = (uint8_t *)name;
	fields.name_len = strlen(name);
	fields.name_is_complete = 1;

	fields.uuids16 = (ble_uuid16_t[]) {
		BLE_UUID16_INIT(BLE_SVC_SPP_UUID16)
	};
	fields.num_uuids16 = 1;
	fields.uuids16_is_complete = 1;

	rc = ble_gap_adv_set_fields(&fields);
	if (rc != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "error setting advertisement data; rc=%d\n", rc);
		return;
	}

	/* Begin advertising. */
	memset(&adv_params, 0, sizeof adv_params);
	adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
	adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
	rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
						   &adv_params, ble_spp_server_gap_event, NULL);
	if (rc != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "error enabling advertisement; rc=%d\n", rc);
		return;
	}
}

/**
 * The nimble host executes this callback when a GAP event occurs.	The
 * application associates a GAP event callback with each connection that forms.
 * ble_spp_server uses the same callback for all connections.
 *
 * @param event					The type of event being signalled.
 * @param ctxt					Various information pertaining to the event.
 * @param arg					Application-specified argument; unused by
 *									ble_spp_server.
 *
 * @return						0 if the application successfully handled the
 *									event; nonzero on failure.	The semantics
 *									of the return code is specific to the
 *									particular GAP event being signalled.
 */
static int
ble_spp_server_gap_event(struct ble_gap_event *event, void *arg)
{
	struct ble_gap_conn_desc desc;
	int rc;

	switch (event->type) {
	case BLE_GAP_EVENT_LINK_ESTAB:
		/* A new connection was established or a connection attempt failed. */
		ESP_LOGI(__FUNCTION__, "connection %s; status=%d ",
			event->connect.status == 0 ? "established" : "failed",
			event->connect.status);
		if (event->connect.status == 0) {
			rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
			assert(rc == 0);
			ble_spp_server_print_conn_desc(&desc);
		}
		if (event->connect.status != 0 || CONFIG_BT_NIMBLE_MAX_CONNECTIONS > 1) {
			/* Connection failed or if multiple connection allowed; resume advertising. */
			ble_spp_server_advertise();
		}
		return 0;

	case BLE_GAP_EVENT_DISCONNECT:
		ESP_LOGI(__FUNCTION__, "disconnect; reason=%d ", event->disconnect.reason);
		ble_spp_server_print_conn_desc(&event->disconnect.conn);

		conn_handle_subs[event->disconnect.conn.conn_handle] = false;

		/* Connection terminated; resume advertising. */
		ble_spp_server_advertise();
		return 0;

	case BLE_GAP_EVENT_CONN_UPDATE:
		/* The central has updated the connection parameters. */
		ESP_LOGI(__FUNCTION__, "connection updated; status=%d ", event->conn_update.status);
		rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
		assert(rc == 0);
		ble_spp_server_print_conn_desc(&desc);
		return 0;

	case BLE_GAP_EVENT_ADV_COMPLETE:
		ESP_LOGI(__FUNCTION__, "advertise complete; reason=%d", event->adv_complete.reason);
		ble_spp_server_advertise();
		return 0;

	case BLE_GAP_EVENT_MTU:
		ESP_LOGI(__FUNCTION__, "mtu update event; conn_handle=%d cid=%d mtu=%d\n",
			event->mtu.conn_handle,
			event->mtu.channel_id,
			event->mtu.value);
		return 0;

	case BLE_GAP_EVENT_SUBSCRIBE:
		ESP_LOGI(__FUNCTION__, "subscribe event; conn_handle=%d attr_handle=%d "
			"reason=%d prevn=%d curn=%d previ=%d curi=%d\n",
			event->subscribe.conn_handle,
			event->subscribe.attr_handle,
			event->subscribe.reason,
			event->subscribe.prev_notify,
			event->subscribe.cur_notify,
			event->subscribe.prev_indicate,
			event->subscribe.cur_indicate);
		conn_handle_subs[event->subscribe.conn_handle] = true;
		return 0;

	default:
		return 0;
	}
}

static void
ble_spp_server_on_reset(int reason)
{
	ESP_LOGE(__FUNCTION__, "Resetting state; reason=%d\n", reason);
}

static void
ble_spp_server_on_sync(void)
{
	int rc;

	rc = ble_hs_util_ensure_addr(0);
	assert(rc == 0);

	/* Figure out address to use while advertising (no privacy for now) */
	rc = ble_hs_id_infer_auto(0, &own_addr_type);
	if (rc != 0) {
		ESP_LOGE(__FUNCTION__, "error determining address type; rc=%d\n", rc);
		return;
	}

	/* Printing ADDR */
	uint8_t addr_val[6] = {0};
	rc = ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

	ESP_LOGI(__FUNCTION__, "Device Address: %02x:%02x:%02x:%02x:%02x:%02x",
		addr_val[0], addr_val[1], addr_val[2], addr_val[3], addr_val[4], addr_val[5]);
	/* Begin advertising. */
	ble_spp_server_advertise();
}

void ble_spp_server_host_task(void *param)
{
	ESP_LOGI(TAG, "BLE Host Task Started");
	/* This function will return only when nimble_port_stop() is executed */
	nimble_port_run();

	nimble_port_freertos_deinit();
}

/* Callback function for custom service */
static int ble_svc_gatt_handler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
	uint8_t buf[xItemSize];
	int length = 0;
	switch (ctxt->op) {
	case BLE_GATT_ACCESS_OP_READ_CHR:
		ESP_LOGI(__FUNCTION__, "Callback for read");
		break;

	case BLE_GATT_ACCESS_OP_WRITE_CHR:
		ESP_LOGI(__FUNCTION__, "Data received in write event,conn_handle = %x,attr_handle = %x", conn_handle, attr_handle);
		ESP_LOGI(__FUNCTION__, "Data received in write event,ctxt->om->om_lene = %d", ctxt->om->om_len);
		ESP_LOGD(__FUNCTION__, "Data received in write event,ctxt->om->om_data = [%.*s]",ctxt->om->om_len, ctxt->om->om_data);
		ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, ctxt->om->om_data, ctxt->om->om_len, ESP_LOG_INFO);

		/*
		[61 62 63 64 65 66 67 0d 0a] to [61 62 63 64 65 66 67]
		*/
		for (int i=0;i<ctxt->om->om_len;i++) {
			if (ctxt->om->om_data[i] == 0x0d) continue;
			if (ctxt->om->om_data[i] == 0x0a) continue;
			if (length < xItemSize) {
				buf[length] = ctxt->om->om_data[i];
				length++;
			}
		}
		ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, buf, length, ESP_LOG_INFO);
		size_t sended = xMessageBufferSendFromISR(xMessageBufferRecv, buf, length, NULL);
		if (sended != length) {
			ESP_LOGE(__FUNCTION__, "xQueueSendFromISR Fail");
		}
		break;

	default:
		ESP_LOGI(__FUNCTION__, "Default Callback");
		break;
	}
	return 0;
}

/* Define new custom service */
static const struct ble_gatt_svc_def new_ble_svc_gatt_defs[] = {
	{
		/*** Service: SPP */
		.type = BLE_GATT_SVC_TYPE_PRIMARY,
		.uuid = BLE_UUID16_DECLARE(BLE_SVC_SPP_UUID16),
		.characteristics = (struct ble_gatt_chr_def[])
		{ {
				/* Support SPP service */
				.uuid = BLE_UUID16_DECLARE(BLE_SVC_SPP_CHR_UUID16),
				.access_cb = ble_svc_gatt_handler,
				.val_handle = &ble_spp_svc_gatt_read_val_handle,
				.flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
			}, {
				0, /* No more characteristics */
			}
		},
	},
	{
		0, /* No more services. */
	},
};

static void
gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
	char buf[BLE_UUID_STR_LEN];

	switch (ctxt->op) {
	case BLE_GATT_REGISTER_OP_SVC:
		ESP_LOGD(__FUNCTION__, "registered service %s with handle=%d\n",
			ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
			ctxt->svc.handle);
		break;

	case BLE_GATT_REGISTER_OP_CHR:
		ESP_LOGD(__FUNCTION__, "registering characteristic %s with "
			"def_handle=%d val_handle=%d\n",
			ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
			ctxt->chr.def_handle,
			ctxt->chr.val_handle);
		break;

	case BLE_GATT_REGISTER_OP_DSC:
		ESP_LOGD(__FUNCTION__, "registering descriptor %s with handle=%d\n",
			ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
			ctxt->dsc.handle);
		break;

	default:
		assert(0);
		break;
	}
}

int gatt_svr_init(void)
{
	int rc = 0;
	ble_svc_gap_init();
	ble_svc_gatt_init();

	rc = ble_gatts_count_cfg(new_ble_svc_gatt_defs);

	if (rc != 0) {
		return rc;
	}

	rc = ble_gatts_add_svcs(new_ble_svc_gatt_defs);
	if (rc != 0) {
		return rc;
	}

	return 0;
}

void nimble_spp_task(void * pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	esp_err_t ret = nimble_port_init();
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(NULL), "Failed to init nimble %d \n", ret);
		return;
	}

	/* Initialize connection_handle array */
	for (int i = 0; i <= CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++) {
		conn_handle_subs[i] = false;
	}

	/* Initialize the NimBLE host configuration. */
	ble_hs_cfg.reset_cb = ble_spp_server_on_reset;
	ble_hs_cfg.sync_cb = ble_spp_server_on_sync;
	ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
	ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

	//ble_hs_cfg.sm_io_cap = CONFIG_EXAMPLE_IO_TYPE;
#ifdef CONFIG_EXAMPLE_BONDING
	ble_hs_cfg.sm_bonding = 1;
#endif
#ifdef CONFIG_EXAMPLE_MITM
	ble_hs_cfg.sm_mitm = 1;
#endif
#ifdef CONFIG_EXAMPLE_USE_SC
	ble_hs_cfg.sm_sc = 1;
#else
	ble_hs_cfg.sm_sc = 0;
#endif
	ESP_LOGI(pcTaskGetName(NULL), "sm_io_cap=%d sm_bonding=%d sm_mitm=%d sm_sc=%d",
		ble_hs_cfg.sm_io_cap, ble_hs_cfg.sm_bonding, ble_hs_cfg.sm_mitm, ble_hs_cfg.sm_sc);
#ifdef CONFIG_EXAMPLE_BONDING
	ble_hs_cfg.sm_our_key_dist = 1;
	ble_hs_cfg.sm_their_key_dist = 1;
#endif

	/* Register custom service */
	int rc = gatt_svr_init();
	assert(rc == 0);

	/* Set the default device name. */
	//rc = ble_svc_gap_device_name_set("NIMBLE_SPP_SERVER");
	rc = ble_svc_gap_device_name_set(DEVICE_NAME);
	assert(rc == 0);

	/* XXX Need to have template for store */
	ble_store_config_init();

	nimble_port_freertos_init(ble_spp_server_host_task);

	uint8_t buf[xItemSize+2];
	while(1){
		size_t received = xMessageBufferReceive(xMessageBufferTrans, buf, xItemSize, portMAX_DELAY);
		/*
		[61 62 63 64 65 66 67] to [61 62 63 64 65 66 67 0d 0a]
		*/
		buf[received++] = 0x0d;
		buf[received++] = 0x0a;
		for (int i = 0; i <= CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++) {
			/* Check if client has subscribed to notifications */
			if (conn_handle_subs[i]) {
				struct os_mbuf *txom;
				txom = ble_hs_mbuf_from_flat(buf, received);
				int rc = ble_gatts_notify_custom(i, ble_spp_svc_gatt_read_val_handle, txom);
				if (rc == 0) {
					ESP_LOGD(pcTaskGetName(NULL), "Notification sent successfully");
				} else {
					ESP_LOGI(pcTaskGetName(NULL), "Error in sending notification rc = %d", rc);
				}
			}
		}
	} // end while

	// never reach here
	vTaskDelete(NULL);
}
