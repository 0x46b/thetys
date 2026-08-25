/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Includes */
#include "gatt_svc.h"
#include "common.h"
#include "plant_service.h"
#include <host/ble_gatt.h>
#include <host/ble_uuid.h>
#include <services/gatt/ble_svc_gatt.h>
#include <stdint.h>

/* Private function declarations */
static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg);
static void ble_app_advertise(void);

/* Private variables */
/*GATT UUIDs*/
/* static const ble_uuid128_t SERVICE_PLANT_UUID = { */
/*     .u = {.type = BLE_UUID_TYPE_128}, */
/*     .value = {0x4f, 0xaf, 0xc2, 0x01, 0x1f, 0xb5, 0x45, 0x9e, 0x8f, 0xcc,
 * 0xc5, */
/*               0xc9, 0xc3, 0x31, 0x91, 0x4b}}; */

/* static const ble_uuid128_t CHAR_MOISTURE_UUID = { */
/*     .u = {.type = BLE_UUID_TYPE_128}, */
/*     .value = {0xbe, 0xb5, 0x48, 0x3e, 0x36, 0xe1, 0x46, 0x88, 0xb7, 0xf5,
 * 0xea, */
/*               0x07, 0x36, 0x1b, 0x26, 0xa8}}; */

/* static const ble_uuid128_t CHAR_PUMP_UUID = { */
/*     .u = {.type = BLE_UUID_TYPE_128}, */
/*     .value = {0x82, 0x14, 0x2e, 0x0d, 0xa0, 0xdb, 0x46, 0x54, 0xa6, 0xec,
 * 0xc2, */
/*               0x50, 0x91, 0xa1, 0x82, 0xaa}}; */

/* static const ble_uuid128_t CHAR_CONFIG_UUID = { */
/*     .u = {.type = BLE_UUID_TYPE_128}, */
/*     .value = {0xb1, 0x3e, 0xd8, 0x7d, 0x87, 0x28, 0x40, 0xa2, 0x97, 0x21,
 * 0xa1, */
/*               0xe1, 0x27, 0x9a, 0x0e, 0xbf}}; */

/* Handles */
static uint16_t moisture_handle;
static uint16_t conn_handle;

/* GATT services table */
/* static const struct ble_gatt_svc_def gatt_svr_svcs[] = { */
/*     { */
/*         .type = BLE_GATT_SVC_TYPE_PRIMARY, */
/*         .uuid = &SERVICE_PLANT_UUID.u, // Adresse der Basis-UUID-Struktur */
/*         .characteristics = */
/*             (struct ble_gatt_chr_def[]){ */
/*                 { */
/*                     .uuid = &CHAR_MOISTURE_UUID.u, */
/*                     .access_cb = gatt_svr_chr_access, */
/*                     .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY, */
/*                     .val_handle = &moisture_handle, */
/*                 }, */
/*                 { */
/*                     .uuid = &CHAR_PUMP_UUID.u, */
/*                     .access_cb = gatt_svr_chr_access, */
/*                     .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE, */
/*                 }, */
/*                 { */
/*                     .uuid = &CHAR_CONFIG_UUID.u, */
/*                     .access_cb = gatt_svr_chr_access, */
/*                     .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE, */
/*                 }, */
/*                 {0}}, */
/*     }, */
/*     {0}}; */

/* Private functions */
void fill_gatt_buffer(struct os_mbuf *gatt_buff, char *json) {
  // os_mbuf_append(gat_buff, &moisture_value, sizeof(moisture_value));
}
/* Public functions */
/*
 *  Handle GATT attribute register events
 *      - Service register event
 *      - Characteristic register event
 *      - Descriptor register event
 */
static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *gatt_context,
                               void *arg) {
  return 0;
  /* uint16_t uuid16 = ble_uuid_u16(gatt_context->chr->uuid); */

  /* if (gatt_context->op == BLE_GATT_ACCESS_OP_READ_CHR) { */
  /*   if (attr_handle == moisture_handle) { */
  /*     char *json = jsonify_humidities(); */
  /*     fill_gatt_buffer(gatt_context->om, json); */
  /*     return 0; */
  /*   } */
  /*   if (ble_uuid_cmp(gatt_context->chr->uuid, CHAR_PUMP_UUID) == 0) { */
  /*     os_mbuf_append(gatt_context->om, &pump_state, sizeof(pump_state)); */
  /*     return 0; */
  /*   } */
  /*   if (ble_uuid_cmp(gatt_context->chr->uuid, CHAR_CONFIG_UUID) == 0) { */
  /*     uint8_t config_payload[2] = {gpio_pump, gpio_sensor}; */
  /*     os_mbuf_append(gatt_context->om, config_payload,
   * sizeof(config_payload)); */
  /*     return 0; */
  /*   } */
  /* } */

  /* /\* FALL 2: Smartphone schickt Daten (WRITE EVENT) *\/ */
  /* if (gatt_context->op == BLE_GATT_ACCESS_OP_WRITE_CHR) { */
  /*   if (ble_uuid_cmp(gatt_context->chr->uuid, CHAR_PUMP_UUID) == 0) { */
  /*     if (gatt_context->om->om_len > 0) { */
  /*       pump_state = ctxt->om->om_data[0]; */
  /*       gpio_set_level(gpio_pump, pump_state); */
  /*       ESP_LOGI(TAG, "Pumpe manuell geschaltet: %s", */
  /*                pump_state ? "AN" : "AUS"); */
  /*     } */
  /*     return 0; */
  /*   } */

  /*   if (ble_uuid_cmp(gatt_context->chr->uuid, CHAR_CONFIG_UUID) == 0) { */
  /*     if (gatt_context->om->om_len >= 2) { */
  /*       gpio_pump = ctxt->om->om_data[0]; */
  /*       gpio_sensor = ctxt->om->om_data[1]; */
  /*       update_hardware_pins(); */
  /*     } */
  /*     return 0; */
  /*   } */
  /* } */

  /* return BLE_ATT_ERR_UNLIKELY; */
}
