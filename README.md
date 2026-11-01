# joy_js

joy 引擎的可选 **JavaScript(duktape) 脚本后端**,从 joy 核心拆出的独立仓库。

## 定位
- joy 核心本身**不依赖**本模块、也不携带 duktape。
- 只有想让 JS 脚本驱动引擎的项目,才把它作为可选依赖接入。
- 依赖:joy 公共 API(`<joy/joy.h>`)、SDL3、msgpack(jsclib 部分用到)。

## 内容
- `joy_js.h` —— 对外 API(`jsx_*`,承接原 `joy.h` 里的 JS 宿主声明)。
- `jsx.c` —— JS 运行时宿主(创建 duktape、加载脚本、事件分发、热重载)。
- `jsclib/` —— JS 绑定层(`jsopen_*`,把 joy 接口暴露给脚本)。
- `external/duktape/` —— duktape 引擎(amalgamation)。
- `joy.h` / `jmath.h` —— 兼容转发头:让 `jsclib/*.c` 内部的 `"../joy.h"` / `"../jmath.h"`
  无需改动即可解析到 joy 核心;从而保持 jsclib 源码与上游一致。

## 接入方式(下游工程)
```cmake
set(JOY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/joy/joy")
add_subdirectory(third_party/joy_js)
target_link_libraries(<game> PRIVATE joy_js)
```
`joy_js` 会自行链接 SDL3 与 msgpack(若目标存在)。
