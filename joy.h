#ifndef JOY_JS_JOY_SHIM_H
#define JOY_JS_JOY_SHIM_H

/*
 * 兼容转发头。
 * jsclib/*.c 内部使用 `#include "../joy.h"`，相对本仓根解析到本文件；
 * 这里转发到 joy 核心的真实头 <joy/joy.h>，从而保持 jsclib 源码零改动，
 * 便于与 joy 上游同步。编译本模块时需把 joy 源码的父目录加入 include 路径。
 */
#include <joy/joy.h>

#endif /* JOY_JS_JOY_SHIM_H */
