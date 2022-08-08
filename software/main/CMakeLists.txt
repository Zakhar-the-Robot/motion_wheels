# *************************************************************************
#
#  Copyright (c) 2022 Andrei Gramakov. All rights reserved.
#
# This file is licensed under the terms of the MIT license.
# For a copy, see: https://opensource.org/licenses/MIT
#
# site:    https://agramakov.me
# e-mail:  mail@agramakov.me
#
# *************************************************************************

include("../components/SharedVirtualRegisters/SharedVirtualRegisters.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/communication/communication.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/indication/indication.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/motion/motion.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/port/port.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/sensors/sensors.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/main.cmake")

idf_component_get_property(freertos_component_dir freertos COMPONENT_DIR) # for standard FreeRTOS includes 
idf_component_register()

target_sources(${COMPONENT_TARGET} INTERFACE ${SRC_MAIN}
                                          ${SRC_SVR}
                                          ${SRC_COMMUNICATION}
                                          ${SRC_HIGH_LEVEL}
                                          ${SRC_INDICATION}
                                          ${SRC_MOTION}
                                          ${SRC_PORT}
                                          ${SRC_SENSORS})
                        
target_include_directories(${COMPONENT_TARGET} INTERFACE "."
                                                         "${freertos_component_dir}/include/freertos"
                                                         ${INCLUDE_SVR})
                                                         
target_compile_definitions(${COMPONENT_TARGET} INTERFACE "-DSVR_CFG_OS=SVR_OS_FREERTOS")
