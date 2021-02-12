#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ecrobot_emul.h"

/* NXT servo motor API */
/* ------------------- */
#define NXT_N_MOTORS 3

int nxt_motor_get_count(U32 n) {
  if (n < NXT_N_MOTORS) {
    return random() % 360;
  }
  return 0;
}

void nxt_motor_set_count(U32 n, int count) {
}

void nxt_motor_set_speed(U32 n, int speed_percent, int brake) {
}

S32 ecrobot_get_motor_rev(U8 port_id) {
  if (port_id < NXT_N_MOTORS) {
    return random() % 360;
  }
  return 0;
}

void ecrobot_set_motor_rev(U8 port_id, S32 rev) {
}

void ecrobot_set_motor_speed(U8 port_id, S8 speed) {
}

void ecrobot_set_motor_mode_speed(U8 port_id, S32 mode, S8 speed) {
}

/* NXT light sensor API  */
/* --------------------- */
#define NXT_N_SENSORS 4

void ecrobot_set_light_sensor_active(U8 port_id) {
}
void ecrobot_set_light_sensor_inactive(U8 port_id) {
}
U16 ecrobot_get_light_sensor(U8 port_id) {
  if (port_id < NXT_N_SENSORS) {
    return random() * 1024;
  }
  return 0;
}

/* NXT touch sensor API */
/* -------------------- */
U8 ecrobot_get_touch_sensor(U8 port_id) {
  if (port_id < NXT_N_SENSORS) {
    return random() % 2;
  }
  return 0;
}

/* NXT sound sensor API */
/* -------------------- */
U16 ecrobot_get_sound_sensor(U8 port_id) {
  if (port_id < NXT_N_SENSORS) {
    return random() % 1024;
  }
  return 0;
}

/* NXT ultrasonic sensor API */
/* ------------------------- */

void ecrobot_init_sonar_sensor(U8 port_id) {
}
S32 ecrobot_get_sonar_sensor(U8 port_id) {
  if (port_id < NXT_N_SENSORS) {
    return random() % 256 - 1;
  }
  return -1;
}
void ecrobot_term_sonar_sensor(U8 port_id) {
}

/* Bluetooth API */
/* ------------- */

void ecrobot_init_bt_master(const U8 *bd_addr, const CHAR *pin) {
}

void ecrobot_init_bt_slave(const CHAR *pin) {
}

void ecrobot_init_bt_connection(void) {
}

U8 ecrobot_get_bt_device_name(CHAR* bd_name) {
  return 1;
}

U8 ecrobot_set_bt_device_name(const CHAR* bd_name) {
  return 1;
}

SINT ecrobot_get_bt_status(void) {
  return 1;
}

U32 ecrobot_send_bt_packet(U8 *buf, U32 bufLen) {
  return bufLen;
}

U32 ecrobot_read_bt_packet(U8 *buf, U32 bufLen) {
  return bufLen;
}

U8 ecrobot_set_bt_factory_settings(void) {
  return 1;
}

void ecrobot_term_bt_connection(void) {
}

void ecrobot_bt_data_logger(S8 data1, S8 data2) {
}

/* USB API */
/* ------- */
void ecrobot_init_usb(void) {
}

SINT ecrobot_set_name_usb(U8* name) {
  return 1;
}

U8 ecrobot_process1ms_usb(void) {
  return 1;
}

SINT ecrobot_read_usb(U8* buf, U32 off, U32 len) {
  return len;
}

SINT ecrobot_send_usb(U8* buf, U32 off, U32 len) {
  return len;
}

SINT ecrobot_disconnect_usb(void) {
  return 1;
}

void ecrobot_term_usb(void) {
}

/* LCD display command for system */
/* ------------------------------ */

SINT ecrobot_bmp2lcd(const CHAR *file, U8 *lcd, S32 width, S32 height) {
  return 1;
}

void display_bitmap_copy(const U8 *data, U32 width, U32 depth, U32 x, U32 y) {
}

void display_update(void) {
  fflush(stdout);
}

void display_clear(U32 updateToo) {
}

void display_goto_xy(int x, int y) {
}

void display_string(const char *str) {
  printf("%s\n",str);
}

void display_hex(U32 val, U32 places) {
  printf("%lx\n",val);
}

void display_unsigned(U32 val, U32 places) {
  printf("%lu\n",val);
}

void display_int(int val, U32 places) {
  printf("%id\n",val);
}

void ecrobot_status_monitor(const CHAR *target_name) {
}


/* NXT sound API */
/* ------------- */

SINT ecrobot_sound_tone(U32 freq, U32 ms, U32 vol) {
  return 1;
}

SINT ecrobot_sound_wav(const CHAR *file, U32 length, S32 freq, U32 vol) {
  return 1;
}

void sound_freq(U32 freq, U32 ms) {
}

/* NXT internal status API */
/* ----------------------- */

U8 ecrobot_is_ENTER_button_pressed(void) {
  return random() % 2;
}

U8 ecrobot_is_RUN_button_pressed(void) {
  return random() %2;
}

U16 ecrobot_get_battery_voltage(void) {
  return random() % 9000;
}

U32 systick_get_ms(void) {
  return random() % 1000;
}

U32 ecrobot_get_systick_ms(void) {
  return random() % 1000;
}

void systick_wait_ms(U32 ms) {
  usleep(ms*1000);
}
