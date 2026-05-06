set(srcs "main.c" "https_client.c")

idf_component_register(SRCS "${srcs}" EMBED_TXTFILES server.crt INCLUDE_DIRS ".")


