# cobra

##简介

一个cpp文件，不到500行代码，完成一个光栅化渲染器的主要功能，将渲染结果导出到图片。

##支持功能

* 纹理模式渲染
* 线框模式渲染
* 简易光照

渲染效果如图
![screenshot.jpg](https://github.com/jintiao/cobra/raw/master/screenshot.jpg)

##编译及运行
请使用支持c++11标准的编译器进行编译。

###windows

用visual studio 2015打开vs\cobra.sln，编译然后运行根目录下的cobra.exe。

如果要在vs里调试运行，请在项目properties设置将 Debuggin->Working Directory 设为"$(ProjectDir)..\"。

###osx

在根目录下运行 make 然后运行 ./cobra 。

##技术细节

###程序大致流程
```
创建renderer
	创建frame/depth(z) buffer
设置摄像机
创建model
	创建vertex/index buffer
	创建texture
渲染model
	遍历model的所有三角形，对于每个三角形
		对三个顶点分别调用vertex shader
		剔除测试
		光栅化三角形
			计算三角形的包围盒，对于包围盒内的每个像素
				测试像素在不在三角形内
				透视正确的插值
				depth(z) buffer测试
				调用pixel shader
				frame/depth写入
		线框渲染模式
			2d画线算法
				frame/depth写入
frame buffer导出为图片
```

### 相关名词
* local/world/view/projection/screen coordinate
* triangle edge function
* perspective correct interpolation
* backface culling
* depth buffer test
* texture bilinear filtering
* directional light
* blinn-phong shading
* bresenham line algorithm

##参考资料

* [scratchapixel](http://www.scratchapixel.com/index.php)
* [wikipedia](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
* [opengl](https://www.opengl.org/sdk/docs/)
* [google](https://www.google.com)
