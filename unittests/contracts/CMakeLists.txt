# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/enu.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/enu.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/enu.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/enu.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/enu.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/enu.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/enu.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/enu.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/enu.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/enu.wrap/)
