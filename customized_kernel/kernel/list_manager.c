//
// Created by omer.reuveni on 6/2/2021.
//

#include <linux/list_manager.h>

//// "global" list properties
int files_paths_count;
int privileged_procs_count;
//Path_entry_p file_paths_list_head;

int was_initialized =0;

void init_list(){
    if (was_initialized==0){
        printk("init - list initializing\n");
        printk("init - run 3\n");
        INIT_LIST_HEAD(file_paths_list_head);
        privileged_procs_count=0;
        files_paths_count=0;
        was_initialized=1;
    }
}

Path_entry_p get_list_head(){
    return file_paths_list_head;
}



// free the list and all path entries
void destroy_list() {
    init_list();
    if (was_initialized==0){
        // list wasn't initialized yet, nothing to do
        return;
    }
    list_t *pos;
    list_t temp;
    list_for_each(pos, file_paths_list_head){
        temp = *pos;
        Path_node_p a_node	= list_entry(pos, struct path_node, list_pointer);  // returns pointer to our struct
        kfree(a_node);  // free what we dynamically allocated
        list_del(temp); // delete list_t object
    }
    // destroy list ended
    printk("DESTROY: list destroyed\n");

}


// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_files_paths_count(int change){
    files_paths_count+=change;
    return files_paths_count;
}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_privileged_procs_count(int change){
    privileged_procs_count+=change;
    return privileged_procs_count;
}


int check_list_for_path(const char* pathName) {
    if (was_initialized==0){
        printk("SEARCH: list is empty\n");
        return 0;
    }
    list_t pos;
    list_for_each(pos, file_paths_list_head){
        Path_node_p itt= list_entry(pos,struct path_node, file_paths_list_head);
        if (!strcmp(list_entry->file_path, pathName))
        {
            printk("SEARCH: file is blocked \n");
            return 1;
        }
    }
    return 0;
}