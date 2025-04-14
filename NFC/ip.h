#ifndef __ROUTING_TABLE__
#define __ROUTING_TABLE__

#include<stdbool.h>
#include "../GLThreads/glthreads.h"
#include "notif.h"

#define IP_SIZE 16
#define OIF_SIZE 32

typedef struct rt_entry_keys_ {
	char dest[IP_SIZE];
	char mask;
}rt_entry_keys_t;

typedef struct rt_entry_ {
	rt_entry_keys_t rt_entry_keys;
	char gw_ip[IP_SIZE];
	char oif[OIF_SIZE];
	notif_chain_t *chain;
	glthread_node_t glnode;
}rt_entry_t;

typedef struct rt_table_ {
	glthread_t table;
}rt_table_t;

void init_rt_table(rt_table_t *rt_table);

rt_entry_t *add_or_update_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask, char *gw_ip, char *oif);

bool delete_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask);

rt_entry_t *look_up_rt_entry(rt_table_t *rt_table, char *dest, char mask);

void dump_rt_table(rt_table_t *rt_table);

void register_for_notification(rt_table_t *rt_table, rt_entry_keys_t *keys, nfc_app_cb app_cb, uint32_t sub_id);

#endif
