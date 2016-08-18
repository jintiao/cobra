# cobra

##简介

一个cpp文件，不到500行代码，完成一个光栅化渲染器的主要功能，将渲染结果导出到图片(参考screenshot1.bmp)。
![screenshot.jpg](https://github.com/jintiao/cobra/raw/master/screenshot.jpg)

##支持功能

* 纹理模式渲染
* 线框模式渲染
* 方向光

##编译及运行

###windows 

用visual studio 2015打开vs\cobra.sln，编译即可。
如果要在vs里调试运行，请在项目properties设置将 Debuggin->Working Directory 设为"$(ProjectDir)..\"。

###osx/linux

在根目录下运行 make 然后运行 ./cobra 。

##相关技术

* local/world/view/projection/screen 坐标系转换
* triangle edge function
* perspective correct interpolation
* backface culling
* depth buffer test
* texture bilinear filtering
* directional light
* blinn-phong shading
* bresenham line algorithm
* bmp文件读写
* obj文件读入
