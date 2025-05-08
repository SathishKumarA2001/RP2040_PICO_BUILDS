#!/bin/bash

# --- Functions ---

create_target_dir() {
    if [ ! -d "$TARGET_DIR" ]; then
        echo "Creating directory: $TARGET_DIR"
        mkdir -p "$TARGET_DIR"
    fi
}

create_build_dir() {
    BUILD_DIR="$TARGET_DIR/build"
    if [ -d "$TARGET_DIR" ] && [ ! -d "$BUILD_DIR" ]; then
        echo "Creating build directory: $BUILD_DIR"
        mkdir "$BUILD_DIR"
    fi
}

create_main_file() {
    MAIN_FILE="$TARGET_DIR/main.c"

    if [ ! -e "$MAIN_FILE" ]; then
        echo "Creating file: $MAIN_FILE"
        touch "$MAIN_FILE"
    fi
}

create_cmake_import_file() {
    PICO_SDK_PATH="./pico-sdk"
    PICO_SDK_IMPORT_CMAKE_FILE="pico_sdk_import.cmake"
    PICO_SDK_IMPORT_CMAKE_FILE_CLONE="$PICO_SDK_PATH/external/$PICO_SDK_IMPORT_CMAKE_FILE"

    if [ ! -e "$PICO_SDK_IMPORT_CMAKE_FILE_CLONE" ]; then
        echo "Not found: $PICO_SDK_IMPORT_CMAKE_FILE_CLONE"
        exit 1
    fi

    echo "Creating file: $PICO_SDK_IMPORT_CMAKE_FILE"
    cp "$PICO_SDK_IMPORT_CMAKE_FILE_CLONE" "$TARGET_DIR/$PICO_SDK_IMPORT_CMAKE_FILE"
}

create_cmakelists_txt() {
    CMAKELISTS_FILE="$TARGET_DIR/CMakeLists.txt"

    if [ ! -e "$CMAKELISTS_FILE" ]; then
        echo "Creating file: $CMAKELISTS_FILE"

        echo "
        cmake_minimum_required(VERSION 3.13)

        set(PICO_SDK_PATH "./../../pico-sdk") # Set this config for your pico-sdk path

        include(pico_sdk_import.cmake)

        project(main C CXX ASM)

        set(CMAKE_C_STANDARD 11)
        set(CMAKE_CXX_STANDARD 17)

        pico_sdk_init()
        add_executable(main
            main.c
        )

        pico_enable_stdio_usb(main 1)
        pico_enable_stdio_uart(main 0)
        pico_add_extra_outputs(main)

        target_link_libraries(main pico_stdlib hardware_adc pico_multicore)" > $CMAKELISTS_FILE

    fi
}

cleanup() {
    echo "Rolling back changes..."
    rm -rf $TARGET_DIR
    exit 1
}

# Register trap for any error or exit
trap cleanup ERR

# --- Main script ---

echo "Script is running..."

ACTION=$1  # The first argument passed to the script (build or destroy)

# --- Ask user for target directory ---
read -p "Enter the project directory name: " TARGET_DIR

if [ "$ACTION" == "destroy" ]; then
    read -p "Are you sure to destroy the project? (y/n): " IS_DESTROY
    if [ "$IS_DESTROY" != "y" ]; then
        echo "Exiting..."
        exit 0
    fi

    rm -rf "$TARGET_DIR"
    echo "Directory removed: $TARGET_DIR"
    exit 0
fi

if [ -d "$TARGET_DIR" ]; then
    echo "Directory already exists: $TARGET_DIR"
    read -p "Want to recreate it? (y/n): " IS_RECREATE
    
    if [ "$IS_RECREATE" == "y" ]; then
        echo "Recreating directory: $TARGET_DIR"
        rm -rf "$TARGET_DIR"
        mkdir -p "$TARGET_DIR"
    fi
    
    if [ "$IS_RECREATE" != "y" ]; then
        echo "Exiting..."
        exit 0
    fi
fi

create_target_dir
create_build_dir
create_main_file
create_cmake_import_file
create_cmakelists_txt

echo "Script ran successfully"