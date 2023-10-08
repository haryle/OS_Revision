## Event loop

Wait for some events to occur, then handle the event. 

```C
while (1){
    events = getEvents();
    for (e in events)
        handle(e);
}
```
When a handler processes an event, it is the only activity taking place in the system. Therefore, deciding which event to handle next is equivalent to scheduling. 

Question: how does an event-based server determine which events are taking place and in particular with regards to network and disk I/O. How can an event
server tell if a message has arrive for it. 

## Select or Poll API 

The `select` system call to receive event information. 

- Check a file descriptor and return corresponding events:
    - whether ready to read 
    - whether ready to write 
    - whether there is any exception
    - can also set timeout to set poll time.
- Allows for building non-blocking event loop. Check for incoming packets, reads from sockets, and replies if needed. 

## Using select

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    // Open and setup sockets (not shown)
    // Main loop
    while (1){
        // init fd_set to all 0
        fd_set readFDs;
        FD_ZERO (&readFD);

        // Set bits for descriptor
        int fd;
        for (fd = minFD; fd < maxFD; fd++)
            FD_SET(fd, &readFDs);
        // select 
        int rc = select(maxFD+1, &readFDs, NULL, NULL, NULL);

        // check which has data using FD_ISSET()
        int df;
        for (fd = minFD; fd < maxFD; fd++){
            if (FD_ISSET(fd, &readFDs))
                processDF(fd);
        }
    }
}
```

After some initialisation, the server enters an infinite loop.
Inside the loop, it uses `FD_ZERO()` to first clear the set of file descriptors. Then use `FD_SET()` to include all file descriptors from `minFD` to `maxFD` to the set. Example: all the network sockets the server is paying attention to. Finally, call `select()` to see which connections have data available upon them. 

## Asynchronous IO 

```C
struct aiocb {
    int             aio_fildes;    /* File descriptor */
    off_t           aio_offset;    /* File offset */
    volatile void  *aio_buf;       /* Location of buffer */
    size_t          aio_nbytes;    /* Length of transfer */
    int             aio_reqprio;   /* Request priority offset */
    struct sigevent aio_sigevent;  /* Signal number and value */
    int             aio_lio_opcode;/* Operation to be performed */
};
```

To issue an asynchronous read to a file, an application should fill in this structure with relevant information:
- File descriptor of file to read `aio_fildes`
- Offset within the file: `aio_offset`
- Length of request: `aio_nbytes`
- Target memory location to which the result should be copied `aio_buf`

After the application is filled, then it should issue an async read (call the API). 

```C
int aio_read(struct aiocb *aiocbp);
```

To check that the read has finshed, 

```C
int aio_error(const struct aiocb *aiocbp);
```

This checks for whether the request has completed. If it has, the routine returns success. If not, EINPROGRESS is returned. Therefore we can poll using `aio_error` to check if request has finished. 