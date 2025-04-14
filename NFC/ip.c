#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include "../GLThreads/glthreads.h"
#include "ip.h"

void init_rt_table(rt_table_t *rt_table) {
	rt_table->table.head = NULL;
	rt_table->table.offset = OFFSETOF(rt_entry_t, glnode);
}

rt_entry_t *add_or_update_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask, char *gw_ip, char *oif) {
	glthread_node_t *start = rt_table->table.head;
	do {
		if(start == NULL) {
			break;
		}
		rt_entry_t *entry = (rt_entry_t *)((char *)start - rt_table->table.offset);
		if(strcmp(dest_ip, entry->rt_entry_keys.dest) == 0 && mask == entry->rt_entry_keys.mask) {
			if(gw_ip == NULL && oif == NULL) {
				return entry;
			}
			memcpy(entry->gw_ip, gw_ip, IP_SIZE);
			memcpy(entry->oif, oif, OIF_SIZE);
			invoke_notification_chain(entry, NFC_MOD);
			return entry;
		}
		start = start->right;
	}while(start != rt_table->table.head);

	rt_entry_t *new_entry = calloc(1, sizeof(rt_entry_t));
	memcpy(new_entry->rt_entry_keys.dest, dest_ip, IP_SIZE);
	new_entry->rt_entry_keys.mask = mask;
	if(gw_ip != NULL) {
		memcpy(new_entry->gw_ip, gw_ip, IP_SIZE);
	}
	if(oif != NULL) {
		memcpy(new_entry->oif, oif, OIF_SIZE);
	}
	glthread_add(&rt_table->table, &new_entry->glnode); 
	new_entry->chain = create_new_notif_chain();
	return new_entry;
}

bool delete_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask) {	
	glthread_node_t *start = rt_table->table.head;
	do {
		if(start == NULL) {
			break;
		}
		rt_entry_t *entry = (rt_entry_t *)((char *)start - rt_table->table.offset);
		if(strcmp(dest_ip, entry->rt_entry_keys.dest) == 0 && mask == entry->rt_entry_keys.mask) {
			invoke_notification_chain(entry, NFC_DEL);
			delete_notif_chain(entry->chain);
			glthread_remove(&rt_table->table, start);
			free(entry);
			return true;
		}
		start = start->right;
	}while(start != rt_table->table.head);

	return false;
}

rt_entry_t *look_up_rt_entry(rt_table_t *rt_table, char *dest, char mask) {
	glthread_node_t *start = rt_table->table.head;
	do {
		if(start == NULL) {
			break;
		}
		rt_entry_t *entry = (rt_entry_t *)((char *)start - rt_table->table.offset);
		if(strcmp(dest, entry->rt_entry_keys.dest) == 0 && mask == entry->rt_entry_keys.mask) {
			return entry;
		}
		start = start->right;
	}while(start != rt_table->table.head);

	return NULL;
}

void dump_rt_table(rt_table_t *rt_table) {
	glthread_node_t *start = rt_table->table.head;
	do {
		if(start == NULL) {
			break;
		}
		rt_entry_t *entry = (rt_entry_t *)((char *)start - rt_table->table.offset);
		printf("Destination IP: %s\nIP Mask: %d\nGateway IP: %s\nOIF: %s\n", entry->rt_entry_keys.dest, entry->rt_entry_keys.mask, entry->gw_ip, entry->oif);
		printf("List of all subscribers: ");
			
		glthread_node_t *ptr = entry->chain->notif_chain.head;
		do {
			if(ptr == NULL) {
				break;
			}
			notif_chain_elem_t *notif_entry = (notif_chain_elem_t *)((char *)ptr - entry->chain->notif_chain.offset);
			printf("%d ", notif_entry->id);
			ptr = ptr->right;
		}while(ptr != entry->chain->notif_chain.head);
		printf("\n\n");
		
		start = start->right;
	}while(start != rt_table->table.head);
}

void register_for_notification(rt_table_t *rt_table, rt_entry_keys_t *keys, nfc_app_cb app_cb, uint32_t sub_id) {
	rt_entry_t *ptr = look_up_rt_entry(rt_table, keys->dest, keys->mask);
	bool is_added = false;
	if(ptr == NULL) {
		ptr = add_or_update_rt_entry(rt_table, keys->dest, keys->mask, NULL, NULL);
		is_added = true;
	}
	notif_chain_elem_t *elem = calloc(1, sizeof(notif_chain_elem_t));
	elem->id = sub_id;
	elem->app_cb = app_cb;
	register_notif_chain(ptr->chain, elem);
	if(is_added) {
		invoke_notification_chain(ptr, NFC_ADD);
	}
}
