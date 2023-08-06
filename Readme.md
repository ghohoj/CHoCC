# 背景介绍

这个仓库是对论文[^1]的复现，文章核心实现了

# 实现原理

核心算法分为三个部分：

1. 从众多接口的方向中不断寻找最邻近的三个接口，构造由这三个接口计算出他们的内接圆锥并计算出对应的参数，即文中的Projection-based boundary evaluation或Plane-rolling boundary evaluation。（之后的算法主要采取后者）
2. 从这些参数中通过各种方法构造出各种接口，即文中的CHoCC接口，CNTInterface接口，ACHoCCInterface接口。
3. 最后拼接为一个网格。

下面我们分别介绍这三个步骤。





# 效果展示[^1]
1. 运行CHoCCInterface，你可以得到文件./result/CHoCCInterface.obj，展示论文fig.3(c)。
2. 运行CNTInterface，你可以得到文件./result/CHoCCInterface.obj，展示论文fig.15(c)。
3. 运行ACHoCCInterface,你可以得到./result/ACHoCCInterface.obj，展示论文fig.19。在这个方法中你可以调节参数b（b的作用见论文fillet-surface，主要用于调节连接曲面的平滑程度）与是否加入补充点（an additional circle of infinitesimal radius）。
4. 你可以修改配置文件地址参数dir_address或自己编写textshape.txt，展示不同的方向的接口。编写规则详见./data/Readme.md。本仓库已经提供了3接口，6接口，8接口的范例。
5. 运行CHoCCMesh，你可以得到xxx，展示论文fig.2。
6. 运行CNTMesh，你可以得到xxx，展示论文fig.13。

运行结果如下所示（使用vscode-3d-preview展示）：

|                 | 3接口                                                        | 6接口                                                        | 8接口                                                        |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| CHoCCInterface  | ![image-20230806164437709](./assets/image-20230806164437709.png) | ![image-20230806162902496](./assets/image-20230806162902496.png) | ![image-20230806164145702](./assets/image-20230806164145702.png) |
| CNTInterface    | ![image-20230806164526251](./assets/image-20230806164526251.png) | ![image-20230806162950595](./assets/image-20230806162950595.png) | ![image-20230806164758113](./assets/image-20230806164758113.png) |
| ACHoCCInterface |                                                              | ![image-20230806162657109](./assets/image-20230806162657109.png) | ![image-20230806164051653](./assets/image-20230806164051653.png) |

CHoCCMesh网格



CNTMesh网格

# 开源声明与环境配置

## 开源声明

声明：本人对代码享有所有权，除了库函数以外并没有使用任何其他的仓库的代码，所有文件中的代码独立完成。遵循MIT开源协议。同时感谢浙江大学李明老师及其附属团队与论文[^ 1]的原作者。

## 环境配置

系统环境：ubuntu 22.04
编译环境：vscode
图片展示插件：vscode-3d-preview
编译环境：MinGW
代码运行依赖库：Eigen

# 论文引用

[^1]: Wu Y, Gupta A, Kurzeja K, et al. Chocc: convex hull of cospherical circles and applications to lattices[J]. Computer-Aided Design, 2020, 129: 102903.
[^2]: Wu Y, Gupta A, Kurzeja K, et al. Chocc: convex hull of cospherical circles and applications to lattices[J]. Computer-Aided Design, 2020, 129: 102903.

# 疑难点

1. 几何处理中会遇到大量的极限点问题导致程序出错。例如





