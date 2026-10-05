set pagination off
info threads
thread apply all bt
echo \n--- owner LWP of each mutex ---\n
p book._M_mutex.__data.__owner
p risk._M_mutex.__data.__owner
