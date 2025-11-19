# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Euronext_Surveillance_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Euronext_Surveillance_autogen.dir\\ParseCache.txt"
  "Euronext_Surveillance_autogen"
  )
endif()
