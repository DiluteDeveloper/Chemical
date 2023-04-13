# CMake generated file
# The compiler generated pdb file needs to be written to disk
# by mspdbsrv. The foreach retry loop is needed to make sure
# the pdb file is ready to be copied.

foreach(retry RANGE 1 30)
  if (EXISTS "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/engine.dir/${PDB_PREFIX}engine.pdb" AND (NOT EXISTS "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/sandbox.dir/${PDB_PREFIX}engine.pdb" OR NOT "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/sandbox.dir/${PDB_PREFIX}engine.pdb  " IS_NEWER_THAN "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/engine.dir/${PDB_PREFIX}engine.pdb"))
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/engine.dir/${PDB_PREFIX}engine.pdb" "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/sandbox.dir/${PDB_PREFIX}" RESULT_VARIABLE result  ERROR_QUIET)
    if (NOT result EQUAL 0)
      execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 1)
    else()
      break()
    endif()
  elseif(NOT EXISTS "D:/Programming/Current_Projects/Chemical_V2/out/build/x64-Debug/engine.dir/${PDB_PREFIX}engine.pdb")
    execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 1)
  endif()
endforeach()
