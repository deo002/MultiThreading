#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdio.h>
#include"notif.h"
#include"../GLThreads/glthreads.h"
#include "ip.h"

void register_notif_chain(notif_chain_t *nfc, notif_chain_elem_t *nfce) {
	glthread_add(&nfc->notif_chain, &nfce->glnode);
}

void invoke_notification_chain(void *args, nfc_op_t status) {
	rt_entry_t * _arg = (rt_entry_t *)args;
	glthread_node_t *cur = _arg->chain->notif_chain.head;
	do {
		if(cur == NULL) {
			break;
		}

		notif_chain_elem_t *ptr = (notif_chain_elem_t *)((char *)cur - _arg->chain->notif_chain.offset);
		ptr->app_cb(args, status, ptr->id);
		cur = cur->right;
	} while(cur != _arg->chain->notif_chain.head);
}

 notif_chain_t *create_new_notif_chain() {
	notif_chain_t *chain = calloc(1, sizeof(notif_chain_t));
	init_glthread(&chain->notif_chain, OFFSETOF(notif_chain_elem_t, glnode));
	return chain;
}

void delete_all_nfce(notif_chain_t *nfc) {
	while(nfc->notif_chain.head != NULL) {
		notif_chain_elem_t *ptr = (notif_chain_elem_t *)((char *)(nfc->notif_chain.head) - nfc->notif_chain.offset);
                glthread_remove(&nfc->notif_chain, nfc->notif_chain.head);
		free(ptr);
	}
}

void delete_notif_chain(notif_chain_t *nfc) {
	delete_all_nfce(nfc);
	free(nfc);	
}
