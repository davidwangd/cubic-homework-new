gen.cpp		:	随机拧几步生成一个打乱的魔方
(gen)			Input:	standard
				Output:	question
cubic.cpp	:	通过层先法将打乱的魔方还原 
(cubic)			Input:	question
				Output:	output.txt
render.cpp	:	将魔方的还原过程渲染，文档中vertex.shader 和 fragment.shader 都是魔方渲染过程中的shader文件。
(render)		Input:	question output.txt
				Output:	GUI 
Matrix.hpp	:	一个泛型矩阵的头文件，矩阵的头文件，定义矩阵运算等，为cubic.hpp所引用

cubic.hpp	:	一个存储魔方的头文件，被gen.cpp, cubic.cpp, render.cpp共同引用，使用矩阵存储魔方，置换矩阵存储旋转与变换
				downCross, downFace, secondLevel, upCross, upCrossCorrect, upFace, finish
				这些还原函数在cubic.cpp中存在正常实现　其他的文件的包含中声明为空函数

Block		:　	内容	:	存储每个block 的对应位置
				格式	:	三个数字为一组，表示block编号，face编号和格子在这一个面 编号

Rotate		:	内容	:	旋转的映射方式
				格式	:	第一行的８对数字表示一个面的顺时针旋转互相映射　
						之后每１３行一组
						每一组第一个字母表示旋转的方式 数字表示对应的面的编号
						之后12对数字表示旋转的互相映射

question	: 	gen的输出，或者程序测试时人为构造输入，存储魔方的初始状态使得cubic　从中读取信息

faceOrder 	:	记录魔方front面确定之后四个相邻面的顺序的数组
		　		用于查找给定front面确定剩下的面
		　　
standard 	:	用于存储标准魔方格式，方便数据生成器生成数据调试程序

error		:	存储cubic输出的错误调试信息(正常时为空)
colorTrans	:	main通过这个参数将魔方的初始放置方式传递给render
