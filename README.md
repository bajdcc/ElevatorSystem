# 电梯控制系统（MFC）

原地址：https://github.com/bajdcc/dev/tree/master/elevatorsystem

2014年10月所作，炒一下冷饭。顺便修一下bug。

用了AFX中的Dialog，但是界面是我用GDI+画的，那时候刚琢磨完DirectUI。
不过，现在有了新成品https://github.com/bajdcc/GameFramework啦，完成不用MFC的任何东西。
然而GameFramework的软肋是不能动态删除控件（因为lua中我是从0编号的）。

先前用了坑爹的构思来想电梯的设计，在UICore.cpp里面，后来证明有误。

## 截图

![gui](https://github.com/bajdcc/ElevatorSystem/raw/master/screenshots/gui.png)

![debug](https://github.com/bajdcc/ElevatorSystem/raw/master/screenshots/debug.png)