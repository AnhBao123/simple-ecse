# simple-ecse
A small entity-component-system-event made as a demo for my upcoming project.
Important things:
 - Entity: simple unique ID which is an integer. Entity manager is implemented by std::queue.
 - Component: rather called a set of data group together, used for System's logic.
 - System: process logic based on entities' components information, there are some other systems like Render system.
 - Event: ways for systems to comunicate, for eg. an entity touched the ground, Collision system would process and send relative message to Gravity system, so player's double jump is reset.
<p> I only implemented Rigid Body, Input, Collision and Health systems.
But I think it can be extended for further developments if wanted.
