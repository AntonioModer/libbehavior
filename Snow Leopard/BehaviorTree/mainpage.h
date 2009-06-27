/**
\mainpage Libbehavior Documentation

@section Introduction to the library

This is the API Documentation for Libbehavior, a reactive AI library based on the concept of "behavior trees". A behavior tree is a way of organizing a collection of states and the decision processes for when to move between them. Behavior trees have many similiarities to Finite State Machines, but they have a number of additional features that make them more suitable for representing complex and potentially parallel behaviors.

Like Finite State Machines and unlike planners, Behavior Trees are reactive. This means that they do not look ahead to the future, but simply make decisions based on their current state and current inputs. This has a number of implications. First, it is clear that Behavior Trees should not be used for solving intense algorithmic problems like playing chess or pathfinding. Instead, Behavior Trees excel at things like making interesting enemies and non-player characters in RPG, shooting, or adventure games. They can also provide a convenient scripting language for implementing simple, reusable chunks of logic, in cases where the full power (and full complexity) of C++ is not needed.

If this sounds useful to you, continue to the next section, where I explain exactly what behavior trees are and how they work.

@section What is a Behavior Tree?

A Behavior Tree, as one would guess, is a branching data structure that is used to represent behaviors. There are two types of nodes in a behavior tree: internal nodes and leaf nodes. Leaf nodes represent the most primitive actions that could be taken by a character. In a game, this could be walking in a particular direction, using a particular item, and doing a particular attack. The behavior of an agent can be defined as a sequence of these primitive actions, with the caveat that some of the actions could be happening simultaneously.

Of course, a fixed linear sequence of actions is not a very interesting behavior. It lacks awareness of the situation and adaptivity. These qualities are given to the agent via the internal nodes. These nodes decide which of their children nodes to execute, using a number of different schemes. One internal node, the Parallel Node, executes all of its children simultaneously. By combining the Parallel Node with other nodes like the Priority or Probability Nodes, interesting behaviors can be developed that show awareness and adaptivity.

@section What Internal Nodes are Available?

This is a brief behavioral description of each of the supplied internal nodes. For specifics, please consult each node's documentation page.

- Count Limit Node
This node restricts a particular path of behavior from executing more than the specified number of times. It can either put a strict limitation in repetition, or reset the limitation after some time. This is useful for, e.g. preventing your agents from repeating the same line of dialog to the point where the player will become annoyed.
- ParallelNode
This is a powerful node that executes all of its children at the same time. This not just useful for simultanous behaviors, it can also be used for pursuing a main path of behavior while simultanously monitoring the world for problems that would make the behavior invalid. Examples of this type of usage are given in the test bench.
- Priority Node
The Priority Node maintains an ordered sequence of behaviors, where earlier ones are more desireable (but may fail to complete), and later ones are less desireable, but presumably more likely to work. For example, if a character is trying to get through a door and they have the key in pocket, they can simply take out the key and open the door. If they don't have a key in pocket, that behavior will fail. The next behavior in priority order may be to go to the backyard and get the spare key. If that fails, the next behavior may be to call someone who has a key. This will continue until either one of the approaches succeeds, in which case the priority node will return success, or all of the nodes fail. In the latter case, the priority node will also return failure.
-Probability Node
The probability node introduces randomness into the agent's behavior. It works by associating a weight with each of its children. When it needs to select a behavior, it makes a random choice among its children, based on their weights. The weighting approach facilitates "tweaking" behaviors by allowing changes to be made without requiring that all the probabilities be re-balanced.
- Repeat Node
The Repeat Node is unusual because it only has one child. It runs that child either a fixed number of times or indefinitely.
- Sequential Node
The Sequential Node is the workhorse internal node. It simply runs all of its children in sequential order. If one of them fails, it also fails. Once all of them succeeds, it also succeeds.

By composing these internal nodes together or possibly making your own, a large range of behavior can be expressed in a compact and reusable way.

@section What Leaf Nodes are Available?

Since leaf nodes are application-specific, you will have to implement most of them yourself. However, there are a handful of useful nodes provided. Some of them can wrap function and member calls in order to save you time and reduce code duplication. Others are handy "stand ins" for when some behavior is not yet implemented, or when you are debugging behaviors.
