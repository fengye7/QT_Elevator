#ifndef MAINWINDOW_H   //防止该头文件被重复引用
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "System.h"
class System;//前置声明

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //初始化相关的值
    void init();
    //处理鼠标点击事件（选择一个楼层）,当相关鼠标事件发生时自动调用
//    void mouseReleaseEvent(QMouseEvent *ev);(没有成功达到要求，主要是无法接收到来自按钮上的点击，以后此类问题可以直接用QPainter)

    //下面是可视化界面
protected:
    //只要窗口状态改变自动调用此函数
    //需要绘制窗口必须通过此函数实现,绘制电梯
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    //创建一个电梯调度系统
    System* elevatorSystem;

    //退出按钮
    QPushButton* exitButton;

public slots:
    //退出
    void Exit(){
        // 关闭并退出应用程序
        QCoreApplication::quit();
    }
};
#endif // MAINWINDOW_H
