
set(srcs
            "main.cpp"
            )

set(includes    "."
                "include")
idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${includes})