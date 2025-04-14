#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include "ip.h"

extern rt_table_t *publisher_get_rt_table();

static void test_cb(void *arg,  nfc_op_t nfc_op_code, uint32_t client_id) {
	rt_entry_t *rt_entry;
	printf("%s() client : %u, Notified with opcode %s\n",
			__FUNCTION__, client_id, get_str_op_code(nfc_op_code));

	rt_entry = (rt_entry_t *) arg;

	printf("%-20s %-4d %-20s %s\n",
			rt_entry->rt_entry_keys.dest,
			rt_entry->rt_entry_keys.mask,
			rt_entry->gw_ip,
			rt_entry->oif);
	printf("\n");
}

void *subscriber_fn_callback(void *arg) {
	rt_entry_keys_t rt_entry_keys;

	uint32_t _arg = *(uint32_t *)arg;
	memset(&rt_entry_keys, 0, sizeof(rt_entry_keys_t));
	strncpy(rt_entry_keys.dest, "122.1.1.1", IP_SIZE);
	rt_entry_keys.mask = 32;
	register_for_notification(publisher_get_rt_table(), &rt_entry_keys, test_cb, _arg);	

	memset(&rt_entry_keys, 0, sizeof(rt_entry_keys_t));
	strncpy(rt_entry_keys.dest, "122.1.1.5", IP_SIZE);
	rt_entry_keys.mask = 32;
	register_for_notification(publisher_get_rt_table(), &rt_entry_keys, test_cb, _arg);	

	memset(&rt_entry_keys, 0, sizeof(rt_entry_keys_t));
	strncpy(rt_entry_keys.dest, "122.1.1.6", IP_SIZE);
	rt_entry_keys.mask = 32;
	register_for_notification(publisher_get_rt_table(), &rt_entry_keys, test_cb, _arg);	

	pause();

	return NULL;
}

void create_subscriber_thread(uint32_t client_id) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	uint32_t *id = calloc(1, sizeof(uint32_t));
	*id = client_id;

	pthread_t thread;
	pthread_create(&thread, &attr, subscriber_fn_callback, (void *)id);

	printf("Subscriber %d created\n", client_id);
}
