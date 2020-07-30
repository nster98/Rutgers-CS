# Problem 3

## Question 1
Signals interrupt the currently-running process, and can be handled in different ways using signal handlers. The advantage is that, when handled (if at all), program flow returns back to the previously-running instruction. The sleep() call merely suspends execution temporarily, allowing for signals to be sent to the process externally.

Sleep() does not come with the guarantee that using signals for synchronization does. Using sleep() to synchronize may not produce the exact same outcome every time if different processes start and finish at different times, but sending signals to a particular process is accurate and effective.

## Question 2
The role of wait_for_children() is to ensure that all children have stopped executing tasks before the parent of those children suspends its own execution. This is to prevany children from potentially finishing their execution and exiting while their parents are blocked waiting on the SIGCONT signal. It ensures the benefit of uniform completion of execution, and omitting this function could create cases of unintentional blocking where parents, once released from suspension, wait for their children to exit, but the children, presumed zombie orphans, have already exited and (potentially) were adopted by the OS. Then the parent will be waiting indefinitely.