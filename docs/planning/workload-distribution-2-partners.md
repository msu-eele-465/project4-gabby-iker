# Project 4 Workload Distribution Form: 2 Partners

It is recommended that at least some of the functionality is a shared responsibility between two or more members of the team, whereas some of the functionality is assigned solely to an individual. That said, how you break up the project is entirely up to you, so long as the workload distribution (measured in points as defined below) is approximately balanced.

You are encouraged to support each other with an overall goal of successfully completing the project. If one person struggles with their responsibility, the other member is encouraged to step in. However, an agreement should be struck between the partners wherein one partner gives up some of the assigned points for that function relative to the amount of help received.  

You will fill out this form:
1. Before the project starts. This is to document your intention. 
2. Before the demo if there was any deviation from the initial plan.

| Requirement/specification                                                                                | Points | Gab Plan  | Iker Plan | Gab Fin   | Iker Fin  |
|----------------------------------------------------------------------------------------------------------|--------|-----------|-----------|-----------|-----------|
| **Presentation**                                                                                         |        |           |           |           |           |
| Introduction                                                                                             | 1      | 0         | 1         | 0         | 1         |
| Circuit diagram                                                                                          | 2      | 1.5       | 0.5       | 2         | 0         |
| System architecture diagram                                                                              | 2      | 1         | 1         | 0         | 2         |
| Controller high-level flowchart                                                                          | 1      | 0         | 1         | 0         | 1         |
| LED peripheral high-level flowchart                                                                      | 1      | 1         | 0         | 1         | 0         |
| LCD peripheral high-level flowchart                                                                      | 1      | 1         | 0         | 0         | 1         |
| Professional demo                                                                                        | 2      | 1         | 1         | 2         | 0         |
| **LED Patterns**                                                                                         |        |           |           |           |           |
| LED patterns work correctly                                                                              | 1      | 1         | 0         | 1         | 0         |
| **LCD Display**                                                                                          |        |           |           |           |           |
| LCD is blank when the system is locked                                                                   | 0.5    | 0         | 0.5       | 0.5       | 0         |
| LCD displays current keypress when unlocked                                                              | 1      | 0         | 1         | 0         | 1         |
| LCD displays  LED pattern name                                                                           | 2      | 0         | 2         | 0.5       | 1.5       |
| Pressing “C” toggles the cursor state (on or off)                                                        | 1      | 0         | 1         | 0         | 1         |
| Pressing “9” toggles the cursor’s blink state (blinking or not blinking)                                 | 1      | 0         | 1         | 0         | 1         |
| **Main controller**                                                                                      |        |           |           |           |           |
| System must have a status indicator that indicates whether system is locked, being unlocked, or unlocked | 0.5    | 0.5       | 0         | 0.5       | 0         |
| The system should respond quickly to a key press                                                         | 1      | 1         | 0         | 1         | 0         |
| The system must communicate to the LED peripheral when a pattern button is pressed                       | 2      | 2         | 0         | 2         | 0         |
| The system must communicate to the LCD peripheral when any key is pressed                                | 2      | 2         | 0         | 2         | 0         |
| **LED bar peripheral**                                                                                   |        |           |           |           |           |
| Status indicator must indicate when the system has received data                                         | 0.25   | 0.25      | 0         | 0         | 0         |
| Status indicator must indicate when the system has not received data for some time                       | 0.25   | 0.25      | 0         | 0         | 0         |
| System must be controlled via i2c                                                                        | 2      | 2         | 0         | 2         | 0         |
| **LCD peripheral**                                                                                       |        |           |           |           |           |
| Status indicator must indicate when the system has received data                                         | 0.25   | 0         | 0.25      | 0         | 0         |
| Status indicator must indicate when the system has not received data for some time                       | 0.25   | 0         | 0.25      | 0         | 0         |
| System must be controlled via i2c                                                                        | 2      | 2         | 0         | 2         | 0         |
| System must be able to write characters to all 32 segments                                               | 4      | 0         | 4         | 0         | 4         |
| System must be able to write all characters between 00100011 and 01111111 in the Character Font Table    | 4      | 4         | 0         | 0         | 4         |
| LCD contrast is adjustable                                                                               | 2      | 0         | 2         | 0         | 2         |
| LCD must be able to be cleared                                                                           | 1      | 0         | 1         | 1         | 0         |
| LCD cursor can be turned on and off                                                                      | 1      | 0         | 1         | 0         | 1         |
| LCD cursor’s blink functionality can be turned off and on                                                | 1      | 0         | 1         | 0         | 1         |
| Total                                                                                                    | 40     | 18.5      | 21.5      | 17.5      | 21.5      |


## 🚀 Extra credit points
The extra credit points are allotted to both partners. You don't need to formally split these tasks. The points are just here for your information.

| Extra credit requirement/specification                                                                   | Extra Credit Points |
|----------------------------------------------------------------------------------------------------------|---------------------|
| **LED Patterns**                                                                                         |                     |
| pattern 4                                                                                                | 0.25                |
| pattern 5                                                                                                | 0.25                |
| pattern 6                                                                                                | 0.25                |
| pattern 7                                                                                                | 0.25                |
| **LCD Display**                                                                                          |                     |
| LCD displays the LED pattern base transition period                                                      | 1                   |
| **Main controller**                                                                                      |                     |
| The system must detect the key press within 0.25 s                                                       | 0.5                 |
| **LED bar peripheral**                                                                                   |                     |
| Extra credit: The system must complete the requested operation with 0.5 s of receiving the i2c command   | 0.5                 |
| **LCD peripheral**                                                                                       |                     |
| Extra credit: The system must complete the requested operation with 0.5 s of receiving the i2c command   | 0.5                 |
| System must be able to write every character in the Character Font Table                                 | 0.5                 |
| Extra credit: Create and display a custom character                                                      | 2                   |
