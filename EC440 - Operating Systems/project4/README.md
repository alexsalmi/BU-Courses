## METHODOLOGY

To write my solution for this assignment, I more or less just followed the given instructions, the slides from the discussion section.
As for data structures, I used the suggested structures given in the discussion slides for the TLS, page, and hash_element structures. However, instead of storing all of the hash_elements in a hash table, I renamed them to list_element and stored them in a doubly linked list instead.

### TLS_CREATE():
This function was simple to implement. For error checking, all I had to do was search through the linked list for the current thread's node, and if it had one with a TLS of size > 0 return -1. If it does not already exist, I simply had to initialize a new list_element and TLS for the current thread, and initialize the correct number of pages using mmap. 

### TLS_CLONE():
This function was fairly similar to TLS_create(). I error checked for if the current thread has a TLS the same way. Then I also checked to make sure that the target thread already had a TLS. Then I initialized a new list_element and TLS for the current thread, including a new page array, but did not initialize new pages in this array. Instead, I looped through the page array of the target thread, and copied all of the page pointers into the current thread's new page array.

### TLS_READ():
All I had to do for this function was to error check that the current thread has an existing TLS in the linked list, and that the inputted offset + length is not greater than the TLS's total size. After that, I unprotected all of the current thread's pages, read the data using the read functionality given in the discussion slides, and then reprotect all of the pages again.

### TLS_WRITE():
This function was fairly similar to the read function. The error checking was exactly the same, as was the unprotecting and reprotecting. Again, I used the write functionality given in the discussion slides. The only thing I had to add to the write functionality is, in the COW implementation, to add a memcpy from the shared TLS into the new copied TLS, so that the new page is actually a copy of the shared pge, instead of just a new blank page.

### TLS_DESTROY():
For this function, I did the error checking to make sure the current thread had a TLS, just like in the last two functions. Then I looped through the TLS's pages, and if they only had one reference, I freed the memory, but if the page had more than one reference, I simply decremented the page's ref_count instead. 
Then I also freed the rest of the objects: the page array, the TLS, and the linked list node. 

## Complications
When I first started, I was getting build errors about casting pointers to pointers of different lengths, which was easily solved by changing all unsigned ints in the example code to unsigned longs.

I was also having some issues when I first changed from the hash_table to the linked list, with looping through the list for error checking. I was getting a lot of seg faults for dumb mistakes. 

Finally, I was confused as to why I wasn't passing test case 6, until I realized that the COW implementation given in the discussion slides never actually copied the data from the shared page into the new one. This was easily fixed with a memcpy call from the shared page into the new page. 

