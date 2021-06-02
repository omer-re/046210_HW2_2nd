//
// Created by omer.reuveni on 6/2/2021.
//
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#ifndef _LIST_MANAGER_H
#define _LIST_MANAGER_H

list_t file_paths_list_head;

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
#endif //LIST_MANAGER_H
