# SweetPopups
# 编译方法
## 使用 Visual Studio
用 Visual Studio 打开项目

选择 Release 配置

生成 → 生成解决方案
## 使用方法
启动程序：运行 SweetPopups.exe

查看弹窗：程序会在屏幕随机位置弹出温馨消息

退出程序：按下键盘上的 K 键（不区分大小写）
## 常见问题
Q: 程序编译失败，显示编码错误
A: 使用英文消息或确保源代码文件保存为 UTF-8 with BOM 编码。

Q: 弹窗出现后程序无响应
A: 使用项目中的修复版本代码，改进了消息循环和线程同步。

Q: 按 K 键无法退出程序
A: 确保程序窗口有焦点，输入法调成大写或ENG模式

Q: 弹窗显示乱码
A: 检查消息文件的编码格式，建议使用 UTF-8。

## 输出文件位置
构建成功后，exe 文件会生成在：

Debug 版本：项目目录\x64\Debug\SweetPopups.exe

Release 版本：项目目录\x64\Release\SweetPopups.exe

# build.bat不可用
