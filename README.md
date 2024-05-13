# 英语课堂派app
## 大家好，我是张新博（大傻逼）
## 环境依赖
* qt6.7
    1. Multimedia模块
    2. MySql 8.0
* MinGW编译器

## Config
### 配置云端数据库服务器
> qt6以后没有mysql驱动，需要自行下载配置。

1. 从git仓库：https://github.com/thecodemonkey86/qt_mysql_driver
中找到qt6.7，MinGW对应的驱动压缩包，下载压缩包
2. 压缩包内的/sqldrivers/qsqlmysql.debug和qsqlmysql.dll拷贝到C:\Qt\6.7.0\mingw_64\plugins\sqldrivers（路径自行查找）
3. 将压缩包内的libcrypto-3-x64.dll，libmysql.dll和libssl-3-x64.dll拷贝到C:\Qt\6.7.0\mingw_64\bin中。

> 远端数据库服务器选择阿里云     

1. 根据参考资料1搭建即可。

## 运行代码
打开qtcreator，加载CMakeLists.txt，等待构建成功后点击运行即可。

## 参考资料
1. https://blog.csdn.net/qq_64770714/article/details/132981341

