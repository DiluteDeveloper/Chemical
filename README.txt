Alpha 0.2 Scene Framework

changes this commit:

Too many to count.
Renamed all source and header files to uppercase words.
Added scene functionality and stuff.
Added opengl function buffer system called the opengl handler.

known issues:

codebase is an absolute mess, but works. Intentionally written as quickly as possible
to get a working prototype for my codebase design ideas. next update should be focused on
heavily refining the code including making the component handlers in Scene polymorphic.

todo:

notes:

User code will not be implemented for a while, but when it does, you can create "commands"
or some sort of interface that can be controlled so that its not spaghetti

Currently all scene, component, and handler code is considered to be "not live" as in,
there is no playback version of these components, they will contain all the data needed
to edit them live