#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include "ip.h"
#include<stdint.h>

rt_table_t rt_table;

rt_table_t * publisher_get_rt_table() {
	return &rt_table;
}

extern void create_subscriber_thread(uint32_t client_id);

void main_menu() {
	int choice;
	while(1) {
		printf("Publisher Menu\n");
		printf("1. Add/Update Routing Table Entry\n");
		printf("2. Delete Routing table entry\n");
		printf("3. Dump Routing table\n");
		choice = 0;
		scanf("%d", &choice);

		switch(choice) {
			case 1:
				{
					char dest[IP_SIZE];
					char mask;
					char gw_ip[IP_SIZE];
					char oif[OIF_SIZE];

					printf("Enter destination IP:\n");
					scanf("%s", dest);
					while ((getchar()) != '\n');

					printf("Enter IP mask:\n");
					int d;
					scanf("%d", &d);
					while ((getchar()) != '\n');
					mask = (char)d;

					printf("Enter gateway ip:\n");
					scanf("%s", gw_ip);
					while ((getchar()) != '\n');
					
					printf("Enter OIF:\n");
					scanf("%s", oif);	
					while ((getchar()) != '\n');
					add_or_update_rt_entry(publisher_get_rt_table(), dest, mask, gw_ip, oif);
				}
				break;
			case 2:	
				{

					char dest[IP_SIZE];
					char mask;	

					printf("Enter destination IP:\n");
					scanf("%s", dest);
					while ((getchar()) != '\n');

					printf("Enter IP mask:\n");
					int d;
					scanf("%d", &d);
					while ((getchar()) != '\n');
					mask = (char)d;
					delete_rt_entry(publisher_get_rt_table(), dest, mask);
				}
				break;
			case 3:
				dump_rt_table(publisher_get_rt_table());
		}
	}
}

void *publisher_thread_fn(void * arg) {
	(void)arg;
	add_or_update_rt_entry(publisher_get_rt_table(), "122.1.1.1", 32, "10.1.1.1", "eth1");
	add_or_update_rt_entry(publisher_get_rt_table(), "122.1.1.5", 32, "10.1.1.3", "eth1");
	add_or_update_rt_entry(publisher_get_rt_table(), "122.1.1.6", 32, "10.1.1.8", "eth1");

	main_menu();

	return NULL;
}

void create_publisher_thread() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t publisher_thread;
	pthread_create(&publisher_thread, &attr, publisher_thread_fn, (void *)NULL);
}

int main() {

	init_rt_table(&rt_table);

	create_subscriber_thread(1);
	sleep(3);

	create_subscriber_thread(2);
	sleep(3);

	create_subscriber_thread(3);
	sleep(3);

	create_publisher_thread();	

	pthread_exit(0);

	return 0;
}
