cmake_minimum_required(VERSION 3.8)

project(dcaiq)

set(DCAIQ_LIBS
    ${CMAKE_SOURCE_DIR}/lib/librkaiq.so
    ${CMAKE_SOURCE_DIR}/lib/librkisp_api.so
    ${CMAKE_SOURCE_DIR}/lib/libdcaiq.so
    ${CMAKE_SOURCE_DIR}/lib/libdcaiq_c.so
    ${CMAKE_SOURCE_DIR}/lib/librockchip_mpp.so
    ${CMAKE_SOURCE_DIR}/lib/librockchip_mpp.so.0
    ${CMAKE_SOURCE_DIR}/lib/librockchip_mpp.so.1
    ${CMAKE_SOURCE_DIR}/lib/librockx.so
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/rkaiq DESTINATION "include")
install(FILES ${CMAKE_SOURCE_DIR}/include/dcaiq_api.h DESTINATION "include")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/iqfiles DESTINATION "/etc")
install(FILES ${DCAIQ_LIBS} DESTINATION "lib")




