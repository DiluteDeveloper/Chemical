#pragma once

#if defined(EMSCRIPTEN)
    #include <GLES3/gl3.h>
#elif defined(GLAD) 
    #include <glad/gl.h>
#else
    #error "No GL loader defined! #define ES3 or #define GLAD"
#endif


