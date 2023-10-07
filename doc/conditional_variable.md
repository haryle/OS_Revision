## Conditional Variable

Basically the `wait` and `notify` methods in Java. 

## Definition

Is an explicit queue that threads can put themselves on when some conditions on the state of execution are not met. When the conditions are met, the threads are woken up. 

- `wait()`: when a thread wishes to put itself to sleep.
- `signal()`: when a condition has changed and sleeping thread will be woken up.