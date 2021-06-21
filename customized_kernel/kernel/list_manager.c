//
// Created by omer.reuveni on 8/2/2021.
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
        a_node = list_entry(pos,
        struct path_node, list_pointer);  // returns pointer to our struct
        list_del(pos); // delete list_t object
        kfree(a_node);  // free what we dynamically allocated
    }
    // destroy list ended
    printk("DESTROY: list destroyed\n");

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
        a_node = list_entry(pos,
        struct path_node, list_pointer);  // returns pointer to our struct



        if (!strcmp(a_node->file_path, pathName))
        {
            printk("SEARCH: file is blocked \n");
            return 1;
        }
    }
    return 0;
}


// Assuming permission checked before. Make sure check permissions before
int proc_upgrade_queue(pid_t proc_pid){
    // check pid is valid
    struct task_struct* proc_moving;
    proc_moving = find_task_by_pid(proc_pid);
    if (proc_moving==NULL){
      //  printk("UPGRADE: PID %d isn't found on queue %s\n",proc_pid,queue);
        return -EFAULT;
    }
    printk("UPGRADE: pull pid %d from queue\n",proc_pid);

    dequeue_task_ext(proc_moving, proc_moving->array);

    // change the task's properties
    // change permission
    proc_moving->is_privileged=1;
    //proc_moving->prio=PRIVILEGED_PRIO;  // moved to sched.c/enqueue_task
    proc_moving->priv_jiffies= jiffies;


    printk("UPGRADE: push pid %d from queue\n",proc_pid);
    enqueue_task_ext(proc_moving, proc_moving->array);

    // increment counter
    set_privileged_procs_count(1);

   // printk("UPGRADE: migration of %d is done\n",task_pointer->pid );
    return set_privileged_procs_count(0);
}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_files_paths_count(int change) {
//    init_list();
    printk("set_files_paths_count: %d\n",change);

    files_paths_count += change;
    return files_paths_count;
}

// if change is 1 or -1 it's changing the counter.
// if it's 0- it just returns the current count.
int set_privileged_procs_count(int change) {
//    init_list();
    printk("set_privileged_procs_count: %d\n",change);
    privileged_procs_count += change;
    return privileged_procs_count;
}



task_t* check_queue_for_senior_process(list_t priv_list) {

    long min_jiffies=-1; // holds the current min while scanning
    task_t * senior_proc;
    senior_proc=NULL;
    int flag_first=0;

    list_t *pos;
    list_for_each(pos, &priv_list)
    {
        task_t *pid_task_struct;
        pid_task_struct = list_entry(pos, task_t , run_list); // returns pointer to struct

        // case it's the first item, set the current min to it
        if(flag_first==0){ //first item in list
            min_jiffies= pid_task_struct->priv_jiffies;
            senior_proc=pid_task_struct;
            flag_first=1;
        }
        // scan for older process
        if (pid_task_struct->priv_jiffies < min_jiffies)  //
        {
            min_jiffies= pid_task_struct->priv_jiffies;
            senior_proc= pid_task_struct;
        }
    }
    if (min_jiffies==-1){
        // failed finding any min process
        printk("check_queue_for_senior_process: failed finding any min process\n");
    }
    printk("check_queue_for_senior_process: returning pid %d with age of %d jiffies\n", (int)(senior_proc->pid), (long)(senior_proc->priv_jiffies));
    return senior_proc;
}



int kill_inheritance_logic(task_t* sender, task_t* receiver){
    if (sender->is_privileged==0 && receiver->is_privileged==0){
        printk("kill_inheritance_logic: A.P==0&&B.P==0. use regular\n");
        return 0;
    }
    if (sender->is_privileged==0 && receiver->is_privileged==1){
        printk("kill_inheritance_logic: A.P==0&&B.P==1. A.P=B.P=1\n");
        //
        proc_upgrade_queue(sender->pid);
        // receiver will make exit() as any regular process.
        return 1;
    }
    if (sender->is_privileged==1 && receiver->is_privileged==1){
        printk("kill_inheritance_logic: A.P==1&&B.P==1. check if A upgrades\n");
        if (receiver->priv_jiffies < sender->priv_jiffies){  // receiver is older
            sender->priv_jiffies=receiver->priv_jiffies;
        }
        // else do nothing, sender will continue as usual, receiver will die on exit()
        return 1;
    }
    printk("kill_inheritance_logic: ERROR, SHOULDN'T GET HERE!\n");

    return -1;
}