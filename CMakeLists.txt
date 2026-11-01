# ============================================================
# joy_js —— joy 的可选 JavaScript(duktape) 脚本后端（独立仓库）
#
# 包含：duktape 引擎 + jsclib 绑定 + jsx 运行时宿主。
# 作为下游工程的可选依赖按需接入；不启用时 joy 核心无需本目录。
#
# 使用方需提供（cache 变量）：
#   JOY_SOURCE_DIR   joy 源码目录（含 joy.h 的目录，即 <repo>/joy/joy）
#
# 说明：jsclib / jsx.c 直接以 <joy/joy.h>、<joy/jmath.h> 包含 joy 核心头，
# 由下方 include 目录（${JOY_SOURCE_DIR}/.. 即 joy 仓库根）解析；
# 本仓不再放置任何转发头。
# ============================================================
cmake_minimum_required(VERSION 3.16)

# 独立仓库模式：本文件作为仓库根 CMakeLists 使用时需要自带 project()；
# 被上层工程 add_subdirectory 时整段跳过，行为不变。
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(joy_js C)
endif()

if(NOT TARGET joy_js)
    file(GLOB JOY_JS_JSCLIB CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/jsclib/*.c")

    add_library(joy_js STATIC
        jsx.c
        external/duktape/duktape.c
        ${JOY_JS_JSCLIB}
    )

    # 使 #include "joy.h"(转发头) / "jsclib/jsclib.h" 等相对包含成立
    target_include_directories(joy_js PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # joy 核心以 submodule 形式位于本仓 joy/ 下；也兼容同级源码树（../joy）
    if(NOT DEFINED JOY_SOURCE_DIR)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/joy/joy/joy.h")
            set(_joy_default "${CMAKE_CURRENT_SOURCE_DIR}/joy/joy")
        else()
            set(_joy_default "${CMAKE_CURRENT_SOURCE_DIR}/../joy/joy")
        endif()
        set(JOY_SOURCE_DIR "${_joy_default}" CACHE PATH "joy 源码目录（含 joy.h）")
    endif()
    # 使转发头里的 #include <joy/joy.h> / <joy/jmath.h> 成立
    target_include_directories(joy_js PRIVATE "${JOY_SOURCE_DIR}/..")

    # joy 为纯源码引用：定义 JOY_STATIC 让 JOY_API 置空
    target_compile_definitions(joy_js PRIVATE JOY_STATIC)

    # jsclib 依赖 SDL3 与 msgpack（由其自身编译选项提供；不存在则跳过）
    if(TARGET SDL3::SDL3)
        target_link_libraries(joy_js PUBLIC SDL3::SDL3)
    endif()
    if(TARGET msgpack-c-static)
        target_link_libraries(joy_js PUBLIC msgpack-c-static)
    elseif(TARGET msgpack-c)
        target_link_libraries(joy_js PUBLIC msgpack-c)
    endif()
    # msgpack 随 joy 子模块一同提供（<joy 源码根>/msgpack）
    if(NOT DEFINED JOY_MSGPACK_DIR)
        set(JOY_MSGPACK_DIR "${JOY_SOURCE_DIR}/../msgpack")
    endif()
    target_include_directories(joy_js PRIVATE
        "${JOY_MSGPACK_DIR}/include"
        "${CMAKE_BINARY_DIR}/msgpack/include")

    set_property(TARGET joy_js PROPERTY C_STANDARD 99)
    set_property(TARGET joy_js PROPERTY C_STANDARD_REQUIRED ON)

    # 源文件为 UTF-8 无 BOM（含中文注释），MSVC 默认按本地代码页解析会报 C4819
    if(MSVC)
        target_compile_options(joy_js PRIVATE /utf-8)
    endif()
endif()
