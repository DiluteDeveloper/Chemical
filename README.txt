todo:

Add 0 stride warning to vertex layout class
remove vertex layout from shaderprogram class
Combine chunk rendering and chunk generation
combine 2 for loops for each axis of chunk side face generation into one, they are split because one iterates over negative,
one iterates over positive.
not necessarily a bug, but the way chunk blocks are rendered is flipped for 1 axis, since sometimes it goes up in height, sometimes down in height, the iterator can either
start from the top or the bottom, so the layers are inverted on 2 directions. this will need to be changed.