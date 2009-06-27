/**
\mainpage Libbehavior Documentation

@section Introduction to the library

This is the API Documentation for Libbehavior, a reactive AI library based on the concept of "behavior trees". A behavior tree is a way of organizing a collection of states and the decision processes for when to move between them. Behavior trees have many similiarities to Finite State Machines, but they have a number of additional features that make them more suitable for representing complex and potentially parallel behaviors.

Like Finite State Machines and unlike planners, Behavior Trees are reactive. This means that they do not look ahead to the future, but simply make decisions based on their current state and current inputs. This has a number of implications. First, it is clear that Behavior Trees should not be used for solving intense algorithmic problems like playing chess or pathfinding. Instead, Behavior Trees excel at things like making interesting enemies and non-player characters in RPG, shooting, or adventure games. They can also provide a convenient scripting language for implementing simple, reusable chunks of logic, in cases where the full power (and full complexity) of C++ is not needed.

If this sounds useful to you, continue to the next section, where I explain exactly what behavior trees are and how they work.

@section What is a Behavior Tree?

A Behavior Tree, as one would guess, is a branching data structure that is used to represent behaviors. There are two types of nodes in a behavior tree: internal nodes and leaf nodes. Leaf nodes represent the most primitive actions that could be taken by a character. In a game, this could be walking in a particular direction, using a particular item, and doing a particular attack. The behavior of an agent can be defined as a sequence of these primitive actions, with the caveat that some of the actions could be happening simultaneously.

Of course, a fixed linear sequence of actions is not a very interesting behavior. It lacks awareness of the situation and adaptivity. These qualities are given to the agent via the internal nodes. These nodes decide which of their children nodes to execute, using a number of different schemes. One internal node, the Parallel Node, executes all of its children simultaneously. By combining the Parallel Node with other nodes like the Priority or Probability Nodes, interesting behaviors can be developed that show awareness and adaptivity.

@What Internal Nodes are Available?

This is a brief behavioral description of each of the supplied internal nodes. For specifics, please consult each node's documentation page.



