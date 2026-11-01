#ifndef JOY_JS_JMATH_SHIM_H
#define JOY_JS_JMATH_SHIM_H

/*
 * 兼容转发头。
 * jsclib/*.c 内部使用 `#include "../jmath.h"`，相对本仓根解析到本文件；
 * 这里转发到 joy 核心的 <joy/jmath.h>，保持 jsclib 源码零改动。
 */
#include <joy/jmath.h>

#endif /* JOY_JS_JMATH_SHIM_H */
