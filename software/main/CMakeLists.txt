# # Sources
# file(GLOB_RECURSE srcs_c "*.c")
# list(APPEND srcs  ${srcs_c})

# file(GLOB_RECURSE srcs_cpp "*.cpp")
# list(APPEND srcs  ${srcs_cpp})


# Sources
set(srcs
            "main.cpp"
            "src/drivers/indication.cpp"
            "src/drivers/motors_on_esp32.cpp"
            "src/high_level/commands.cpp"
            "src/high_level/controlcallback.cpp"
            "src/high_level/registers.cpp"
            "src/port/delay.c"
            "src/port/millis.c"
            "src/subsystems_hw/bluetooth_serial.cpp"
            "src/subsystems_hw/motor_controller.cpp"
            "src/subsystems_hw/position_unit.cpp"
            "src/subsystems_hw/serial.cpp"
            "src/subsystems_hw/zk_i2c.cpp"
            )


# Includes
set(includes    "."
                "include"
                "include/drivers"
                "include/high_level"
                "include/port"
                "include/subsystems_hw")



#  Print
#  *********************************************************************
message("*******************************************************************************")
message("Project's info")

message("-- Sources")
foreach(el IN LISTS srcs)
message(" ${el}")
endforeach()

message("-- Includes")
foreach(el IN LISTS includes)
message(" ${el}")
endforeach()
message("*******************************************************************************")
# ********************************************************************** Print

idf_component_register( SRCS ${srcs}
                        INCLUDE_DIRS ${includes})
