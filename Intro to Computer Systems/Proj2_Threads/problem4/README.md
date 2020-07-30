# Question 1

**Signal does not re-install itself**

When sending signals from the command line to the process, only SIGINT will be handled correctly. With the code written, the thread that isn't blocking SIGINT, which is Thread 1, will print out
"Recieved SIGINT". However, due to how signal handlers work, there is no way to pass the thread id to the handler, but we know based on analysis of the code that only Thread 1 is handling it.
By sending SIGILL or SIGSTOP however, the whole point of those two signals is to stop or kill the process. Since we are working with threads, there is only one process, which is main, and so main is
terminated, along with all threads. There is no way for the threads to handle SIGILL or SIGSTOP, since at least one thread is responsible for handling it. We can't block it, so nothing is printed
from the handler. This is all when sending the signal once. When sending the signal SIGINT again, the signal is not being re-installed. Thus, it will not print out "Recieved SIGINT".

**Signal does re-install itself**

The only difference when re-installing the signal is that the process will continue printing out "Recieved SIGINT" when that signal is sent. This of course only applies when sending the same signal twice.
Since the signal is being reinstalled, the process can continue looking for SIGINT.

# Question 2

**Signal does not re-install itself**

* Case 1: Sending signal to specific thread

	* When a thread sends a signal to a thread, it will handle it only if it sends SIGINT, and for that matter only if it sends SIGINT to Thread 1, which is responsible for handling SIGINT. If sent to any other threads, then the thread will ignore it. If sending any other signal, such as SIGILL or SIGSTOP, the threads will not be able to handle due to the nature of the those signals and will halt the process. Again, if sending the signal twice however, since the signal handler wont be re-installed, it won't handle SIGINT and will halt execution.

* Case 2: Sending signal to main process

	* When a thread sends a signal to the main process, it will go off to whichever process is supposed to handle it. This is because threads are all part of the same process, so when sending it to the process, it's the same as sending it to whichever thread is responsible for handling that signal. When sending the signal twice, since the signal did not re-install itself, the thread that needs to handle the signal will not be able to handle.

**Signal does reinstall itself**

* Case 1: Sending signal to specific thread

	* As with question 1, the only difference re-installing the signal makes is the ability to send the signal the second time and for the thread to handle it multiple times. As before, only SIGINT will be handled.

* Case 2: Sending signal to main process

	* The same thing applies before, where the process is the same for all threads, and since the signal will be reinstalled, thread 1 will be able to handle SIGINT until the program is finished.

# Remarks

**Output**

./prob4
Thread3
Thread2
Thread1
^CRecieved SIGINT
^CRecieved SIGINT
^CRecieved SIGINT
^CRecieved SIGINT
Thread 3 sum: 2305843008139952128
Thread 1 sum: 2305843008139952128
Thread 2 sum: 2305843008139952128

This output is achieved by sending the SIGINT signal through the command line. If sending SIGILL or SIGSTOP using ```kill -19 <process ID>``` or ```kill -23 <process ID>``` in a different window, the
process terminates, even though the threads are set up correctly to handle the signal.

In each experiment, only SIGINT was able to be sent. The main process is stopped, since we are working with threads, and threads are all part of the same process. Signals are sent to a process, so even
though the threads are handling it the process will terminate and in turn stop all threads.
