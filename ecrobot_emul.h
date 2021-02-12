#ifndef _ECROBOT_EMUL_H_
#define _ECROBOT_EMUL_H_

// Typedefs used in ecrobot API
typedef unsigned int UINT;
typedef signed  int SINT;
typedef float F32;
typedef double F64;
typedef char CHAR;
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned long U32;
typedef signed long S32;

typedef enum {
	NXT_PORT_A,
	NXT_PORT_B,
	NXT_PORT_C
} MOTOR_PORT_T;

typedef enum {
	NXT_PORT_S1,
	NXT_PORT_S2,
	NXT_PORT_S3,
	NXT_PORT_S4
} SENSOR_PORT_T;

/* NXT servo motor API */
extern int  nxt_motor_get_count(U32 n);
extern void nxt_motor_set_count(U32 n, int count);
extern void nxt_motor_set_speed(U32 n, int speed_percent, int brake);
extern  S32 ecrobot_get_motor_rev(U8 port_id);
extern void ecrobot_set_motor_rev(U8 port_id, S32 rev);
extern void ecrobot_set_motor_speed(U8 port_id, S8 speed);
extern void ecrobot_set_motor_mode_speed(U8 port_id, S32 mode, S8 speed);

/* NXT light sensor API */
extern  U16 ecrobot_get_light_sensor(U8 port_id);
extern void ecrobot_set_light_sensor_active(U8 port_id);
extern void ecrobot_set_light_sensor_inactive(U8 port_id);

/* NXT touch sensor API */
extern   U8 ecrobot_get_touch_sensor(U8 port_id);

/* NXT sound sensor API */
extern  U16 ecrobot_get_sound_sensor(U8 port_id);

/* NXT ultrasonic sensor API */
extern void ecrobot_init_sonar_sensor(U8 port_id);
extern  S32 ecrobot_get_sonar_sensor(U8 port_id);
extern void ecrobot_term_sonar_sensor(U8 port_id);

/* Bluetooth API */
extern void ecrobot_init_bt_master(const U8 *bd_addr, const CHAR *pin);
extern void ecrobot_init_bt_slave(const CHAR *pin);
extern void ecrobot_init_bt_connection(void);
extern U8 ecrobot_get_bt_device_name(CHAR* bd_name);
extern U8 ecrobot_set_bt_device_name(const CHAR* bd_name);
extern SINT ecrobot_get_bt_status(void);
extern U32 ecrobot_send_bt_packet(U8 *buf, U32 bufLen);
extern U32 ecrobot_read_bt_packet(U8 *buf, U32 bufLen);
extern U8 ecrobot_set_bt_factory_settings(void);
extern void ecrobot_term_bt_connection(void);
extern void ecrobot_bt_data_logger(S8 data1, S8 data2);

/* USB API */
extern void ecrobot_init_usb(void);
extern SINT ecrobot_set_name_usb(U8* name);
extern U8 ecrobot_process1ms_usb(void);
extern SINT ecrobot_read_usb(U8* buf, U32 off, U32 len);
extern SINT ecrobot_send_usb(U8* buf, U32 off, U32 len);
extern SINT ecrobot_disconnect_usb(void);
extern void ecrobot_term_usb(void);

/* LCD display command for system */
extern SINT ecrobot_bmp2lcd(const CHAR *file, U8 *lcd, S32 width, S32 height);
extern void display_bitmap_copy(const U8 *data, U32 width, U32 depth, U32 x, U32 y);
extern void display_update(void);
extern void display_clear(U32 updateToo);
extern void display_goto_xy(int x, int y);
extern void display_string(const char *str);
extern void display_hex(U32 val, U32 places);
extern void display_unsigned(U32 val, U32 places);
extern void display_int(int val, U32 places);
extern void ecrobot_status_monitor(const CHAR *target_name);


/* NXT sound API */
extern SINT ecrobot_sound_tone(U32 freq, U32 ms, U32 vol);
extern SINT ecrobot_sound_wav(const CHAR *file, U32 length, S32 freq, U32 vol);
extern void sound_freq(U32 freq, U32 ms);

/* NXT internal status API */

extern   U8 ecrobot_is_ENTER_button_pressed(void);
extern   U8 ecrobot_is_RUN_button_pressed(void);
extern  U16 ecrobot_get_battery_voltage(void);
extern  U32 systick_get_ms(void);
extern  U32 ecrobot_get_systick_ms(void);
extern void systick_wait_ms(U32 ms);

#endif
