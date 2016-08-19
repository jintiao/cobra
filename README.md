# cobra

##简介

一个cpp文件，不到500行代码，完成一个光栅化渲染器的主要功能，将渲染结果导出到图片。

##支持功能

* 纹理模式渲染
* 线框模式渲染
* 单个点光源

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

程序大致流程如下
```
创建renderer
	创建frame/depth(z) buffer
设置摄像机 // projection/view matrix
创建model
	创建vertex/index buffer // obj模型文件读入
	创建texture // bmp文件读入
渲染model
	遍历model的所有三角形，对于每个三角形
		对三个顶点分别调用vertex shader // 坐标系转换
		剔除测试 // backface culling
		光栅化三角形 
			计算三角形的包围盒，对于包围盒内的每个像素 // triangle bounding box
				像素测试 // triangle edge function
				插值 // perspective correct intepolation
				depth/z buffer测试
				调用pixel shader // blinn-phong shading
				frame/depth写入 // texture bilinear filtering
		线框渲染模式
			2d画线算法 // bresenham's line algorithm
				frame/depth写入 
frame buffer导出为图片 // bmp写入
```

##参考资料

* [scratchapixel](http://www.scratchapixel.com/index.php)
* [wikipedia](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
* [opengl](https://www.opengl.org/sdk/docs/)
* [google](https://www.google.com)
