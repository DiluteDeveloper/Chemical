changes this commit:
Fixed a bug where forward vector was fucked up, needed to be -matrix[2] instead of matrix[2]
origin is now stored everywhere as chunk coordinates instead of real coordinates
general bugfixes/cleanup

todo:
Figure out why forward of -x 1 makes the player move in the direction of x -1 but the objects move the other way?

finish the chunk generator
(advanced) encode vertex data into 32 bits(1 unsigned int)
3 bits for face orientation, some for position within chunk,
some for a blockid

notes: