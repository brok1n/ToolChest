# 快速下载器  FastDownloader

因为 IDM 软件试用到期了，所以自己写一个来用。

目前主要功能已实现，可下载各类大小文件


## 功能列表及支持情况
[✔] 多线程同时下载一个文件  
[✔] 多线程下载进度展示  
[✔] 多线程下载总进度显示  
[✔] 跟迅雷类似的监听复制到内存中的下载地址 自动弹下载框  
[✔] 关闭程序默认最小化到托盘，可以在托盘中彻底退出程序  
[✔] 本地化，目前只支持中文和英文 非中文系统语言都显示成英文  
[✔] 下载完成提示  
[✖] 下载完 通知栏弹消息  
[✖] 全部暂停  
[✖] 全部开始  
[✖] 全部删除  
[✖] 多选  
[✖] 右键菜单  
[✖] 设置界面  
[✖] 开机启动  
[✖] 浏览器插件  


## 更新记录
### 2020-07-16
- 解决下载进度条更新卡顿 更新不及时问题
- 解决最小化后 单双击托盘图标 或 点击托盘菜单 打开  主窗口仍然是最小化问题
- 添加程序只能打开一个实例功能

### 2020-05-06
- 解决大文件下载崩溃问题

### 2020-05-03
- 单线程下载文件  
- 多线程下载文件  
- 单线程，多线程下载进度使用进度条显示  
- 监听用户复制的可下载链接，用户取消后，该地址将不会再监听  
- 关闭主界面后，自动最小化到系统托盘  
- 本地化，支持简体中文和英文两种语言  
- 无法下载大文件，实测下载超过1.47G文件程序会崩溃  


## 现在的运行效果

#### 首页

![](main.png)


#### 新建下载

![](new_task.png)


#### 下载中

![](downloading.png)
