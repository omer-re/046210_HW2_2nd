#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

#include "block_api.h"

int main()
{
	// Get current pid
    pid_t pid = getpid();
	int res = -1;
    int query;


	// Add ourselves as a new privileged process
	res = block_add_process(pid);
	assert(res == 1);

	// Add some file to the limited list
	res = block_add_file("/some/file");
	assert(res == 0);

    // Query a f ile
    query = block_query("/some/file");
    assert(res == 1);

    // Clear the list
    res = block_clear();
    assert(res == 0);

	return 0;
}
