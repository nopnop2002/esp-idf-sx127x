set(component_srcs "main.c")

idf_component_register(SRCS "${component_srcs}" INCLUDE_DIRS "." REQUIRED_IDF_TARGETS esp32s2 esp32s3 esp32p4)


