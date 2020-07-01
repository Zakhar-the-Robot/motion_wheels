
set(srcs    "main.c"
            "src/motors.c"
            "../lib/hw_motors/include")

set(includes    "."
                "include")
idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${includes})