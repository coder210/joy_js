# joy_js

> **状态:已合并回 joy 主仓,本仓归档。**
>
> 代码现位于 `joy/modules/js`,通过
> `joy_add_sources(<target> MODULES js)` 启用(默认关闭,可用
> `-DJOY_MODULE_JS=ON` 打开);引擎头引用已统一为 `<joy/xxx.h>`。
> 本仓不再接收更新,仅作历史归档。

joy 引擎的可选 **JavaScript(duktape) 脚本后端**,从 joy 核心拆出的独立仓库。

## 定位
- joy 核心本身**不依赖**本模块、也不携带 duktape。
- 只有想让 JS 脚本驱动引擎的项目,才把它作为可选依赖接入。
- 依赖:joy 公共 API(`<src/joy.h>`)、SDL3、msgpack(jsclib 部分用到)。

## 内容
- `joy_js.h` —— 对外 API(`jsx_*`,承接原 `joy.h` 里的 JS 宿主声明)。
- `jsx.c` —— JS 运行时宿主(创建 duktape、加载脚本、事件分发、热重载)。
- `jsclib/` —— JS 绑定层(`jsopen_*`,把 joy 接口暴露给脚本)。
- `external/duktape/` —— duktape 引擎(amalgamation)。

绑定/宿主源码直接以 `<src/joy.h>`、`<src/jmath.h>` 包含 joy 核心头(由 CMake 的 include
目录解析),本仓不再放置转发头。

## joy 子模块
本仓通过 git submodule 引用 joy 核心,检出于 `joy/`:
```powershell
git clone --recursive https://github.com/coder210/joy_js.git
# 已 clone 过(或想同步最新 joy):
git submodule update --init --recursive
```
`JOY_SOURCE_DIR` 默认即指向本仓 `joy/src`(含 `joy.h`),通常无需手动设置。

## 接入方式(下游工程)
```cmake
add_subdirectory(third_party/joy_js)
target_link_libraries(<game> PRIVATE joy_js)
```
`joy_js` 会自行链接 SDL3 与 msgpack(随 joy 子模块提供,若目标存在则链接)。
