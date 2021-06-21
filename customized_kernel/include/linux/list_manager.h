//
// Created by omer.reuveni on 6/2/2021.
//
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/limits.h>
#include <linux/list.h>


#ifndef _LIST_MANAGER_H
#define _LIST_MANAGER_H

#define PRIVILEGED_PRIO 99

extern list_t file_paths_list_head;
typedef struct path_node {
    char file_path[PATH_MAX];
    list_t list_pointer;
    //struct path_entry *next;
} *Path_node_p;


extern int files_paths_count;
extern int privileged_procs_count;

extern int was_initialized;

int set_files_paths_count(int change);
int set_privileged_procs_count(int change);
void init_list(void);
int check_list_for_path(const char* pathName);
void destroy_list(void);
//task_t check_queue_for_pid(prio_array_t queue, pid_t proc_pid);
int proc_upgrade_queue(pid_t proc_pid);
task_t *check_queue_for_senior_process(list_t priv_list);
int kill_inheritance_logic(task_t* sender, task_t* receiver);
void print_queue(void) ;

#endif //LIST_MANAGER_H
