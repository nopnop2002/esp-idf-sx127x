set(component_srcs "lora.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "include")
