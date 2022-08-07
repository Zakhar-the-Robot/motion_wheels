

idf_component_get_property(freertos_component_dir freertos COMPONENT_DIR)

# Sources
file (GLOB SRC_HIGH_LEVEL "high_level/*.c" "high_level/*.cpp")
file (GLOB SRC_PORT "port/*.c" "port/*.cpp")
file (GLOB SRC_COMMUNICATION "communication/*.c" "communication/*.cpp")
file (GLOB SRC_INDICATION "indication/*.c" "indication/*.cpp")
file (GLOB SRC_SENSORS "sensors/*.c" "sensors/*.cpp")
file (GLOB SRC_MOTION "motion/*.c" "motion/*.cpp")
set (SRC_SVR         "../components/SharedVirtualRegisters/src/SharedVirtualRegisters.c"
                     "../components/SharedVirtualRegisters/src/svr_mux_freertos.c"
                     "../components/SharedVirtualRegisters/src/svr_mux_noos.c")

set(srcs "main.cpp"
         ${SRC_SVR}
         ${SRC_COMMUNICATION}
         ${SRC_HIGH_LEVEL}
         ${SRC_INDICATION}
         ${SRC_MOTION}
         ${SRC_PORT}
         ${SRC_SENSORS}
    )

# Includes
set(includes    "."
                "communication"
                "indication"
                "high_level"
                "port"
                "motion"
                "sensors"
                "${freertos_component_dir}/include/freertos"
                "../components/SharedVirtualRegisters/include")

idf_component_register( SRCS ${srcs}
                        INCLUDE_DIRS ${includes})

target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-DSVR_CFG_OS=SVR_OS_FREERTOS")
