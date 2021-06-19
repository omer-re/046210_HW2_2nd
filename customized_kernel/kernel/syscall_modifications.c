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
#include <linux/list_manager.h>

////  restricted_syscall_open
long restricted_syscall_open(const char *filename, int flags, int mode) {
    init_list();
    //printk("sys_open\n");
    int is_file_blocked = check_list_for_path(filename);

    if (is_file_blocked)
    {   // file is in block list

        // check if process is privileged
        if (current->is_privileged == 0)
        {
            // “EPERM” (Operation not permitted): The process is not privileged
            printk("sys_open: file is blocked, process isn't privileged, failed with EPERM\n");
            return -EPERM;
        }
            // file is in list and process has permission
        else if (current->is_privileged == 1)
        {
            printk("sys_open: file is blocked, process is privileged\n");
        }
    }
    printk("sys_open PASS\n");
    return sys_open(filename, flags, mode);
}




////  sys_block_add_process
int sys_block_add_process(pid_t pid) {
    init_list();
    printk("sys_block_add_process\n");
    printk("sys_block_add_process: pid entered- %d\n", pid);
    printk("sys_block_add_process: current privileged procs number is %d \n", set_privileged_procs_count(0));

    //   check if such pid exists
    //   if not -  return ESRCH
    if (current == NULL)
    {
        printk("sys_block_add_process: current is NULL\n");
    }

    if (current->is_privileged == 1)
    {
        task_t *pid_itt;
        pid_itt = find_task_by_pid(pid);
        if (pid_itt == NULL)
        {
            // no matching pid
            printk("sys_block_add_process: such pid not exists\n");
            return -ESRCH;
        }
        printk("sys_block_add_process: LINE 100\n");

        // operation allowed:
        printk("sys_block_add_process: Operation allowed\n");
        // take it from regular_proc_queue insert to privileged_queue
        proc_upgrade_queue(pid_itt);

        printk("sys_block_add_process: operation allowed since request came from privileged process\n");
        return set_privileged_procs_count(0);
    }

    // case of none privileged process, but no privileged processes exists
    else if (current->is_privileged == 0)
    {
        printk("sys_block_add_process: LINE 110\n");

        // check if privileged_procs_count==0
        // if (privileged_procs_count==0){
        if (set_privileged_procs_count(0) == 0)
        {
            //      if true: allow operation
            task_t *pid_itt;
            pid_itt = find_task_by_pid(pid);
            if (pid_itt == NULL)
            {
                // no matching pid
                printk("sys_block_add_process: such pid not exists\n");
                return -ESRCH;
            }

            // operation allowed:
            printk("sys_block_add_process: Operation allowed\n");
            // take it from regular_proc_queue insert to privileged_queue
            proc_upgrade_queue(pid_itt);

            printk("sys_block_add_process: operation allowed due to no other privileged procs %d (should be 1)\n",
                   set_privileged_procs_count(0));
            return (set_privileged_procs_count(0));
        }
            //      else: deny
        else
        {
            printk("sys_block_add_process: operation denied due to other privileged procs\n");
            return -EPERM;
        }
    }
    printk("sys_block_add_process: HUGE PROBLEM, SHOULDN'T GET HERE\n");
    return -EPERM;
}

/**
////  sys_block_add_file
// NEW VERSION
int sys_block_add_file(const char *filename) {
    init_list();
    printk("sys_block_add_file entered #\n");


    // validate legal filename
    if (filename == NULL)
    {
        printk("sys_block_add_file: filename is NULL\n");
        return -EFAULT;
    }
    if (current->is_privileged == 0)
    {
        // if you aren't privileged you have nothing to do here.
        printk("sys_block_add_file: no permission to add file\n");
        return -EPERM;

    }
    printk("sys_block_add_file: filename is %s, length of %d\n", filename, strlen(filename));
    if (check_list_for_path(filename) == 1)
    {
        printk("sys_block_add_file: filename is already exists, great\n");

        return 0;
    }

        //  if needs to be added to the list
        /////  CHECK PERMISSIONS

    else if (current->is_privileged == 1)
    {
        if (check_list_for_path(filename) == 1)
        {
            // file already exists, do nothing, success
            printk("sys_block_add_file: filename is already exists, do nothing, success\n");
            return 0;
        }
        printk("sys_block_add_file: file isn't on list, add it.\n");
        if (check_list_for_path(filename) == 0)
        {
            // allocate new (struct path_entry)=new_path_entry
            Path_node_p new_path_node = (Path_node_p) kmalloc(sizeof(struct path_node), GFP_KERNEL);

            // check allocation
            if (new_path_node == NULL)
            {
                // “ENOMEM” (Out of memory): Failure allocating memory.
                printk("sys_block_add_file: failed allocating memory for new Path_entry_p\n");
                return -ENOMEM;
            }
            // initialize array
            int i = 0;
            for (; i < PATH_MAX; i++)
            {
                new_path_node->file_path[i] = '\0';
            }
            printk("sys_block_add_file: allocating successful\n");
            if (copy_from_user(new_path_node->file_path, filename, strlen(filename) * sizeof(char)) != 0)
            {
                printk("write: Error on copying from user space.\n");
                // if failed   “EFAULT” (Bad address): Error copying from user space, including if filename is NULL.
                kfree(new_path_node);
                return -EFAULT;
            }
            printk("sys_block_add_file: copy from user successful\n");
            list_add(&(new_path_node->list_pointer), &file_paths_list_head);
            // increment file path counter
            set_files_paths_count(1);
            printk("sys_block_add_file: new file added successfully\n");
            return 0;
        }
        printk("sys_block_add_file: HUGE PROBLEM, SHOULDN'T GET HERE #1\n");
        return -1;
    }
    printk("sys_block_add_file: HUGE PROBLEM, SHOULDN'T GET HERE #2\n");
    return -1;
}
**/


// stuff from HW2

////  sys_block_clear
int sys_block_clear(Path_node_p path_list_head) {

    init_list();
    printk("block_clear: entered\n");

    if (current->is_privileged != 1)
    {
        printk("block_clear: proc isn't privileged, can't clear list\n");
        return -EPERM;
    }

    else if (set_privileged_procs_count(0) == 0)
    {
        // no need to do nothing. no files.
        printk("block_clear: no need to do nothing. no files.\n");

        return 0;
    }

    else if (set_files_paths_count(0) > 0)
    {
        printk("block_clear: some files in list. free them.\n");
        destroy_list();
        return 0;
    }

    printk("block_clear: logic failure, shouldn't get here.\n");

    return -1;


}

////  sys_block_query
int sys_block_query(const char *filename) {
    printk("block_query: entered\n");

    int res = check_list_for_path(filename);
    if (res == 0)
    {
        printk("block_query: file doesn't exists\n");
    }
    else if (res == 1)
    {
        printk("block_query: file exists\n");
    }
    return res;
}