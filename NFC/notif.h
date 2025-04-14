#ifndef __NOTIF__
#define __NOTIF__

#include "../GLThreads/glthreads.h"
#include<stdlib.h>
#include<stdbool.h>

#define MAX_KEY_SIZE 64

typedef enum{
	NFC_UNKNOWN,
	NFC_ADD,
	NFC_MOD,
	NFC_DEL,
} nfc_op_t;

typedef void (*nfc_app_cb)(void *, nfc_op_t, unsigned int);

typedef struct notif_chain_ {
	glthread_t notif_chain;
} notif_chain_t;

typedef struct notif_chain_elem_ {
	nfc_app_cb app_cb;
	unsigned int id;
	glthread_node_t glnode;
}notif_chain_elem_t;

static inline char *get_str_op_code(nfc_op_t nfc_op_code) {
	switch(nfc_op_code) {

		case NFC_UNKNOWN:
			return "NFC_UNKNOWN";
		case NFC_ADD:
			return "NFC_ADD";
		case NFC_MOD:
			return "NFC_MOD";
		case NFC_DEL:
			return "NFC_DEL";
		default:
			return NULL;
	}
}

void register_notif_chain(notif_chain_t *nfc, notif_chain_elem_t *nfce);

void invoke_notification_chain(void *args, nfc_op_t status);

notif_chain_t * create_new_notif_chain();

void delete_all_nfce(notif_chain_t *nfc);

void delete_notif_chain(notif_chain_t *nfc);

#endif
