
idf_component_register(
    SRC_DIRS "."
    INCLUDE_DIRS ".")

spiffs_create_partition_assets(
    animation
    "../lottie_assets"
    FLASH_IN_PROJECT
    MMAP_FILE_SUPPORT_FORMAT ".json"
)
