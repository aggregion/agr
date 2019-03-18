# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/agrio.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/agrio.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/agrio.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/agrio.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/agrio.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/agrio.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/agrio.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/agrio.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/agrio.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/agrio.wrap/)
