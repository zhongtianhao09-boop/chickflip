# chickflip (Qt Coin Flip Game)

一个基于 Qt Widgets 的小游戏项目，使用翻金币玩法做关卡挑战，工程使用 `qmake` 管理。

## 环境要求

- Qt Creator（建议 4.15+）
- Qt 5.15 或 Qt 6.x（需包含 `Core`、`Gui`、`Widgets`、`Multimedia` 模块）
- 支持 C++17 的编译器（MSVC / MinGW 均可）

## 快速运行

1. 使用 Qt Creator 打开项目根目录下的 `coinflip.pro`
2. 选择 Kit（编译器 + Qt 版本）
3. 点击「构建」
4. 点击「运行」

## 项目结构（核心文件）

- `main.cpp`：程序入口
- `mymainwindow.*`：主窗口
- `startscene.*`：开始场景
- `selectscene.*`：关卡选择场景
- `playscene.*`：游戏场景
- `coinbutton.*`：金币按钮交互
- `dataconfig.*`：关卡数据
- `res.qrc`、`res/`：图片和音效资源

## 常见问题

- 运行时报资源找不到  
  请确认 `res.qrc` 已加入工程并成功编译。

- 提示缺少多媒体模块  
  请在安装 Qt 时勾选 `Multimedia`，并使用包含该模块的 Kit。

- 构建目录冲突/异常  
  删除 `build/` 后重新构建（该目录已在 `.gitignore` 中忽略）。

## Git 说明

仓库已配置 `.gitignore`，常见 Qt 构建产物和 IDE 临时文件不会被提交。
