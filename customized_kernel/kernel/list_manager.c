//
// Created by omer.reuveni on 6/2/2021.
//

#include <linux/list_manager.h>

//// "global" list properties
int files_paths_count;
int privileged_procs_count;
int was_initialized = 0;
list_t file_paths_list_head;

void init_list() {
    if (was_initialized == 0)
    {
        printk("init - list initializing\n");
        printk("init - run #\n");
        INIT_LIST_HEAD(&file_paths_list_head);
        privileged_procs_count = 0;
        files_paths_count = 0;
        was_initialized = 1;
    }
}


// free the list and all path entries
void destroy_list() {
    init_list();
    if (was_initialized == 0)
    {
        // list wasn't initialized yet, nothing to do
        return;
    }
    list_t *pos;
    list_t *temp;
    list_for_each_safe(pos, temp, &file_paths_list_head)
    {
        Path_node_p
        a_node = list_entry(pos, struct path_node, list_pointer);  // returns pointer to our struct
        list_del(pos); // delete list_t object
        kfree(a_node);  // free what we dynamically allocated
    }
    // destroy list ended
    printk("DESTROY: list destroyed\n");

}

task_t check_queue_for_pid(prio_array_t queue, pid_t proc_pid) {
    //  TODO: translate those checks to process queues
    // START
    if (was_initialized == 0)
    {
        printk("SEARCH: list is empty\n");
        return NULL;
    }
    //END
    list_t *pos;
    list_for_each(pos, &queue)
    {
        task_t *pid_task_struct;
        pid_task_struct = list_entry(pos, struct runqueue, list_pointer); // returns pointer to struct

        if (pid_task_struct->pid==proc_pid)  // the pid we need is found
        {
            // return the pointer
            printk("SEARCH: PID %d found on queue %s\n",proc_pid,queue);
            return pos;
        }
    }
    // failed finding the pid in the queue
    printk("SEARCH: PID %d isn't found on queue %s\n",proc_pid,queue);

    return NULL;
}

/**
 * Remove process from queue
 * @param queue - the queue we remove the process from
 * @param proc_pid - pid of the process we'd like to remove
 * @return task_t pointer so we can use it somewhere else
 */
task_t remove_pid_from_queue(prio_array_t queue, pid_t proc_pid) {
    //  TODO: translate those checks to process queues
    // START
    init_list();
    if (was_initialized == 0)
    {
        // list wasn't initialized yet, nothing to do
        return NULL;
    }
    // END

    // look for the pid in the queue
    task_t *proc, *temp;
    proc=check_queue_for_pid(pid, queue);
    // if found, remove it from it current queue
    if (proc==NULL){
        printk("REMOVE: SEARCH failed, therefore remove failed\n");
        return NULL;
    }
    temp=proc;
    list_del(proc); // delete list_t object
    printk("REMOVE: %d pid was removed\n", temp->pid);

    return temp;
}

task_t insert_pid_to_queue(prio_array_t queue, task_t* proc_task) {
    //  TODO: translate those checks to process queues
    // START
    init_list();
    if (was_initialized == 0)
    {
        // list wasn't initialized yet, nothing to do
        return NULL;
    }
    // END

    // look for the pid in the queue
    // if found, remove it from it current queue
    task_t *proc;
    proc=sched.enqueue_task(proc_task, queue);
    // TODO: how to validate success?
    printk("insert_pid_to_queue: insert_pid_to_queue ended\n");
    return proc;


}

int proc_change_queue(pid_t proc_pid, prio_array_t source_queue, prio_array_t dest_queue){
    //  TODO: validations:
    // check pid is valid
    // check source queue is valid
    // check dest queue is valid

    task_t *task_pointer, *temp;

    //  find proc pid in source queue
    //  remove it from source
    task_pointer=remove_pid_from_queue(source_queue, proc_pid);
    if (task_pointer==NULL){ //failed finding
        printk("MIGRATE: SEARCH failed, therefore remove failed\n");
    }
    //  push to dest
    temp=insert_pid_to_queue(dest_queue, task_pointer);
    if (temp==NULL){ //failed finding
        printk("MIGRATE: enqueue failed, therefore insertion failed\n");
    }
    printk("MIGRATE: migration of %d is done\n",task_pointer->pid );

}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_files_paths_count(int change) {
    files_paths_count += change;
    return files_paths_count;
}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_privileged_procs_count(int change) {
    privileged_procs_count += change;
    return privileged_procs_count;
}

int check_list_for_path(const char *pathName) {
    if (was_initialized == 0)
    {
        printk("SEARCH: list is empty\n");
        return 0;
    }
    list_t *pos;
    list_for_each(pos, &file_paths_list_head)
    {
        //Path_node_p itt= list_entry(pos,struct path_node, file_paths_list_head);
        Path_node_p
        a_node = list_entry(pos,struct path_node, list_pointer);  // returns pointer to our struct

        if (!strcmp(a_node->file_path, pathName))
        {
            printk("SEARCH: file is blocked \n");
            return 1;
        }
    }
    return 0;
}

task_t check_queue_for_senior_process(list_t priv_list) {
    //  TODO: translate those checks to process queues
    // START
    if (was_initialized == 0)
    {
        printk("SEARCH: list is empty\n");
        return NULL;
    }
    //END
    long min_jiffies
    task_t * senior_proc;
    int flag_first=0;

    list_t *pos;
    list_for_each(pos, &priv_list)
    {
        task_t *pid_task_struct;
        pid_task_struct = list_entry(pos, struct runqueue, list_pointer); // returns pointer to struct

        if(flag_first==0){ //first item in list
            min_jiffies= pid_task_struct->priv_jiffies;
            senior_proc=pid_task_struct;
            flag_first=1;
        }
        if (pid_task_struct->priv_jiffies < min_jiffies)  //
        {
            min_jiffies= pid_task_struct->priv_jiffies;
            senior_proc= pid_task_struct;
        }
    }
    // failed finding the pid in the queue
    return senior_proc;
}