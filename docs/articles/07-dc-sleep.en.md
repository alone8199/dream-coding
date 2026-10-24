# Falling Asleep: dc sleep and the Seven-Hour Floor

[中文](./07-dc-sleep.md) | [English](./07-dc-sleep.en.md)

`dc sleep` is the falling-asleep command of dream coding. Its role is to mark the start of a stretch of sleep and check whether the sleep duration meets the minimum requirement. It does not run any script itself; the actual execution happens while you are asleep.

The basic usage of the command is simple:

```
$ dc sleep
Entering REM cycle...
zzz... (8h)
```

If no duration is given, the default is eight hours. You can also pass an hour count explicitly, such as `dc sleep 7.5`. The duration must be a non-negative number. Otherwise the command exits with code 2 and prints an error message stating the requirement.

The most critical rule is the seven-hour floor. If the duration passed is less than seven hours, the command outputs `TimeoutError: sleep too short` and exits with code 1:

```
$ dc sleep 4
TimeoutError: sleep too short (4h < 7h)
```

The reasoning behind this rule is that one complete sleep cycle takes about ninety minutes, and the REM stage usually appears in the second half of the night. With too little time, the dream is often cut off at its most valuable stage, and the results lack coherence. Seven hours is an empirical floor that ensures the brain can complete at least several full cycles.

The output of `dc sleep` has two lines. The first line, `Entering REM cycle...`, indicates entry into the rapid eye movement stage; the second line shows the sleep duration. These two lines have no real computational meaning. Their role is to give the developer a clear sense of ritual: sleep has begun, and the day's work can end.

From an implementation perspective, the logic of `dc sleep` is very simple: parse the arguments, check that the value is valid, compare the duration with the floor, and then print the result. It does not modify any files, nor does it start background processes. This simplicity is by design. Sleep itself is a passive process, and the command should remain passive too.

In actual use, there are several common misconceptions. The first is treating `dc sleep` as a real sleep switch, believing that running it will put you to sleep. In fact, falling asleep is the body's business; the command is only a marker. The second is running the command repeatedly to pad the duration. Each run prints a message about entering REM, but the durations do not add up. The third is waking up midway through the night and still believing the dream is finished.

The value of `dc sleep` is that it turns sleep into a recordable action. It gives the developer a clear starting point each time they fall asleep, and it makes the rhythm of the whole process clear.
