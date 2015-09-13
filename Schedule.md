# Schedule / Task List

1. May 18 - 22: Investigate requirements for the library and write an API and design document. The design document will include a diverse set of "behavior scenarios" which describe, at a high level, types of behaviors that should be realizable using this system.
  * Deliverable: ~~Design Document~~
  * Deliverable: ~~Specification in the form of fully documented header files~~

2. (May 25 to June 12)

  * ~~Port existing 'Snow Leopard' code base to Clanlib version 2~~

  * ~~Remove code that it not relevant for this project~~
  * ~~Document the remaining code extensively, so it can be understood by people who are unfamiliar with Clanlib.~~
  * ~~Add a system for loading scenarios~~.
  * ~~Implement collision detection using the Clanlib API.~~
  * ~~Test on multiple machines and determine the system requirements~~
  * Milestone: ~~A testbed level with a stationary enemy can be loaded. The enemy can be shot at, takes damage, and can be destroyed.~~

3. (June 15 to 26) Write the core code and behavior tree core nodes and create automated unit tests for them:
  * ~~Sequential Execution Node~~
  * ~~Parallel Execution Node~~
  * ~~Priority Selector Node~~
  * ~~Probability Selector Node~~
  * ~~Conditional Nodes~~
    * ~~int~~
    * ~~bool~~
    * ~~float~~
  * ~~Repeat Node~~
  * ~~Count Limit Node~~
  * ~~Function Call Wrapper Node~~

> Deliverable: Source code and unit tests for core nodes

4. (June 29- July 10) Integrate the behavior tree code into the Snow Leopard engine and write condition nodes (sensing) and primitive leaf nodes (action) that interface with the engine.
> Condition
    * ~~Distance of Player~~
    * ~~Relative angle to Player~~
    * ~~Distance of incoming bullets~~
    * ~~Current Health~~

> Leaf
    * ~~Move in an absolute direction~~
    * ~~Move forward relative to current orientation~~
    * ~~Turn towards the player~~
    * ~~Go straight to a particular point~~
    * ~~Fire weapon~~
    * ~~Wait~~
    * ~~Self Destruct~~

> Deliverable: All nodes required for sensing and action are complete

5. (July 13 - 24)
> Create composite behaviors that will be the first several examples that will be distributed with the code. These will include:

  * ~~Turn to face the player and fire when you have a shot lined up.~~ (scenario 1)
  * ~~Fire homing missiles until you are at 50% health. Then, attempt to kamikaze the player (using the same homing behavior)~~ (scenario 2)
  * ~~Extend scenario 1 to add randomized movement behavior (scenario 3)~~
  * ~~Extend scenario 1 to stay out of the player's lane of fire (scenario 4)~~
  * ~~Have ability to nullify nearby projectiles, but with a limited number of activations~~

> Deliverable: 5 playable scenarios that each showcase a composite behavior

6. (July 27 - August 14)
> Create the second set of composite behaviors.

  * ~~A "director" implemented with a behavior tree selects the composition of the enemy waves sent against the player (shows a non-obvious use of behavior trees)~~
  * ~~Space invaders (hundreds of agents)~~
  * ~~An enemy that is cloaked until the player gets within a certain distance (example of FunctionCall and FloatCondition nodes)~~
  * ~~Enemies that wait for a common signal before they all do an action in unison (Shows how to handle synchronization and communication within the behavior tree framework)~~
  * ~~An enemy that keeps fighting as long as there are friends around, but then surrenders when it's the only one left (Shows how multiple agents can interact to create a story)~~
  * 

> Milestone: A full set of of behavior examples. 5 of them are simple and show how to accomplish one goal. 5 of them are more complex or show interesting ways to use behavior trees.

7. (August 15 - 16)
> Create a website for the project.

  * ~~Deliverable: Improve google code website~~