#ifndef ELEVATOR_H
#define ELEVATOR_H

#define FloorNum 20 //层数
#define ElevatorNum 5//电梯数
#define OneFloorHeight 10//单层楼高度
#define SleepTime 10//休眠时间

//下面是一些UI的常数控制**************
#define WindowsLength 1500
#define WindowsWidth 900
#define ElevatorPageSize 600
#define RowSpace 45//行距
#define ColSpace 300//列距
//电梯UI
#define UpBorderLength 80
#define UpBorderWidth 20
#define DoorLength 40
#define DoorWidth 25
//位置控制(均为电梯1的位置，其他根据窗口布局变化)
#define EPosition 855  //电梯
#define MovePositionX 120//上下行按钮
#define MovePositionY 855
//动画所需时间
#define AnimationTime 500

//用于画UI界面
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QPoint>
#include <QMouseEvent>//处理鼠标点击
#include <QMutex>//这两个实现同步机制避免死锁
#include <QWaitCondition>

#include "DigitalDisplay.h"  //数码显示器

//使用主窗口的ui指针
//#include "ui_mainwindow.h"
//class MainWindow;

//移动方向
enum class Direction {
    up,
    down,
    none  //静止
};
//请求类对象
struct ElevatorRequest {
    int requestfloor;//请求的楼层
    Direction direction;//请求方向
};

//电梯类对象
class Elevator :public QObject{
    Q_OBJECT
private:
    //电梯编号
    int id;
    //当前楼层
    int currentfloor;
    //目标楼层
    int targetfloor;
    //方向（上行、下行、静止）
    Direction direction;
    //开关门
    bool controldoor;
    //报警键
    bool warning;
    //电梯内部界面
    QWidget *ElevatorPage;
    //电梯是否结束运行
    bool m_quit;

    //互斥锁
    QMutex ele_mutex;

    // 请求队列
    QVector<ElevatorRequest> requests;

    //可视化UI组件
    //20个楼层按钮/一个报警按钮/开关门按钮/上行按钮/下行按钮
    QPushButton*  floorButton[FloorNum];
    QPushButton* alarmButton;
    QPushButton* closeButton;
    QPushButton* openButton;
    //电梯主体
    QPushButton* upBorder;
    QPushButton* leftdoor;
    QPushButton* rightdoor;

    friend class MainWindow;
    friend class  System;
    friend class DigitalDisplay;
    friend class RunEleThread;

public:
    //构造函数，默认初始楼层为1，静止，关门，不报警
    QWidget* exUI;//创建指针指向主窗口

    explicit Elevator(int id,  QWidget*p);
    //数码显示器
    DigitalDisplay* digitaldisplay;
    //电梯移动
    void Elemove();

    //电梯门控制
    void Closedoor();
    void Opendoor(const bool choose=false);
    //报警
    void Warning();
    //绘制一个电梯
    void DrawElevator();
    //电梯内部
    void ElevatorPageShow();
    //处理请求
    void HandleRequestSingle(const ElevatorRequest& mid);
    //处理电梯调度的函数声明
    void _Sort_Request();
    //处理来自子线程的资源请求,启动电梯运行
    void Run();
    //完成请求后的处理
    void HandleReached();

signals:
    //开始处理请求
    void startElemove(Elevator* ele);
    //完成请求后的操作
    void reached(Elevator* ele);
};

#endif // ELEVATOR_H
