Alpha 1.03

Added C++Conventions.txt to base directory, and manually changed
all naming conventions used in C++ scripts to match.

Added CHEMICAL_DEBUG check to see if required channels of 4
is passed to Sprite2D::SetImage.

ImageData now stores the amount of channels in the actual image
data, even if forced, as opposed to what was in the original image.
Changed testing sprites to 2 different textures, no issues.

Added back ConvertTextureSizedInternalFormatToBaseInternalFormat
renamed to ConvertInternalToBase

todo:

currently move operations on opengl wrapper types delete opengl objects of ID 0. this is possible overhead.