#ifndef DIGITALDISPLAY_H
#define DIGITALDISPLAY_H

//该文件是数码显示器的一些属性
#include <QtWidgets/QLabel>
#include<QVariantAnimation>//处理数字文本的动画
#include <qmath.h>
#include <QTimer>

#define RowSpace 45//行距
#define ColSpace 300//列距
#define DisplayPositionX 120//数码显示器
#define DisplayPositionY 675
//动画所需时间
#define AnimationTime 500

class Elevator;
// 数码显示器
class DigitalDisplay : public QLabel {
    Q_OBJECT
public:
    QWidget* DUI;//创建指针指向主窗口
    QLabel* label;
    Elevator* ele;
    QTimer* timer;
    //构造函数
    DigitalDisplay(Elevator* t, QWidget*p) ;

    // 更新显示
    void updateDisplay(int currentfloor, int targetfloor);
};

#endif // DIGITALDISPLAY_H
