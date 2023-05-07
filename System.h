#ifndef SYSTEM_H
#define SYSTEM_H

#include <QVector>
#include <QQueue>
#include <QThread>//创建线程
#include <QDebug>
#include <QButtonGroup>

#include "Elevator.h"
//使用主窗口的ui指针
//#include "ui_mainwindow.h"
//class MainWindow;

////新的线程类
//class RunEleThread:public QThread{
//    Q_OBJECT
//private:
//    Elevator* elevator;//电梯地址
//public:
//    RunEleThread(Elevator* mid){
//        elevator=mid;
//    }
//protected:
//    void run() override{
//        // 发出信号并将其连接到槽函数
//        //QMetaObject::invokeMethod(elevator, "Run", Qt::QueuedConnection);//这样调用并没有实现在五个线程中分别执行Run函数，只是在五个线程中调用主线程中的Run函数！！！
//        while(!elevator->m_quit){
//            //等待请求
//            //互斥的访问主线程资源
//            elevator->ele_mutex.lock();
//            while(elevator->requests.isEmpty()&&!elevator->m_quit){
//                if(!elevator->ele_condition.wait(&elevator->ele_mutex))
//                    break;
//            }
//            //如果电梯系统宣布结束
//            if(elevator->m_quit){
//                break;
//            }
//            //处理请求队列中的请求
//            ElevatorRequest mid=elevator->requests.takeFirst();
//            elevator->HandleRequestSingle(mid);
//           elevator-> ele_mutex.unlock();
//        }
//    }
//};

//电梯调度系统*******************************
class System: public QWidget {
    Q_OBJECT
private:
    //电梯列表
    QVector<Elevator*> elevators;
    //线程列表
    QVector<QThread*> threads;
    //上下楼的按钮
    QPushButton* upmove[ElevatorNum][FloorNum];
    QPushButton* downmove[ElevatorNum][FloorNum];
    QButtonGroup upButtonGroup[FloorNum];
    QButtonGroup downButtonGroup[FloorNum];

    friend class MainWindow;

public:
    QWidget* SysUI;//指向主窗口的指针

    explicit System(QWidget*p):SysUI(p) {
        // 注册ElevatorRequest为元类型
        qRegisterMetaType<ElevatorRequest>("ElevatorRequest");
        connect(this,&System::Request,this,&System::HandleRequest);

        //初始化楼层的上下键
        for(int i=0;i<ElevatorNum;i++){
            for(int j=0;j<FloorNum;j++){
                upmove[i][j]=new QPushButton;
                downmove[i][j]=new QPushButton;
            }
        }//初始化按钮组
        for(int j=0;j<FloorNum;j++){
            for(int i=0;i<ElevatorNum;i++){
                upButtonGroup[j].addButton(upmove[i][j]);
                downButtonGroup[j].addButton(downmove[i][j]);
            }
            // 连接按钮组的 buttonClicked 信号
            connect(&upButtonGroup[j], QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=]() {
                // 将按钮组的 exclusive 属性设置为 false，以便所有按钮都可以保持高亮状态
                upButtonGroup[j].setExclusive(false);
                // 遍历所有按钮，将它们的高亮状态设置为 true
                QList<QAbstractButton *> buttons = upButtonGroup[j].buttons();
                for (QAbstractButton *btn : buttons) {
                    btn->setStyleSheet("QPushButton{border-image: url(../Elevator/upmove_selected.png);}");
                }
                // 将按钮组的 exclusive 属性设置回 true，以便只有一个按钮可以被选中
                upButtonGroup[j].setExclusive(true);
                //发送请求
                ElevatorRequest request;
                request.requestfloor=j+1;
                request.direction=Direction::up;
                HandleRequest(request);
            });
            connect(&downButtonGroup[j], QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=]() {
                // 将按钮组的 exclusive 属性设置为 false，以便所有按钮都可以保持高亮状态
                downButtonGroup[j].setExclusive(false);
                // 遍历所有按钮，将它们的高亮状态设置为 true
                QList<QAbstractButton *> buttons = downButtonGroup[j].buttons();
                for (QAbstractButton *btn : buttons) {
                    btn->setStyleSheet("QPushButton{border-image: url(../Elevator/downmove_selected.png);}");
                }
                // 将按钮组的 exclusive 属性设置回 true，以便只有一个按钮可以被选中
                downButtonGroup[j].setExclusive(true);
                //发送请求
                ElevatorRequest request;
                request.requestfloor=j+1;
                request.direction=Direction::down;
                HandleRequest(request);
            });
        }

        //create threads***********************************************
        for (int i = 0; i < ElevatorNum; i++) {
            elevators.push_back(new Elevator(i + 1, p));
            connect(elevators[i],&Elevator::startElemove,this,&System::Elemove);
            connect(elevators[i]->openButton,&QPushButton::clicked,this,&System::on_opendoor_clicked);
            connect(elevators[i]->closeButton,&QPushButton::clicked,this,&System::on_closedoor_clicked);
            connect(elevators[i]->alarmButton,&QPushButton::clicked,this,&System::on_warningButton_clicked);
            for(int k=0;k<FloorNum;k++)
                connect(elevators[i]->floorButton[k],&QPushButton::clicked,this,&System::on_floorButton_clicked);
            connect(this,&System::updateDigital,this,&System::UpdateDigitalDisplay);
            connect(elevators[i],&Elevator::reached,this,&System::AfterReached);
            connect(elevators[i]->upBorder,&QPushButton::clicked,this,&System::EnterElevator);

            threads.push_back(new QThread);
            elevators[i]->moveToThread(threads[i]);
            connect(threads[i],&QThread::started,elevators[i],&Elevator::Run);
            threads[i]->start();
        }

        //drawBuilding
        DrawBuilding();

        //Debug
        for (int i = 0; i < ElevatorNum; i++) {
            qDebug()<<i<<'#'<<threads[i]->isRunning()<<'*'<<threads[i]->isFinished();
        }
    }

    ~System() {
        for (int i = 0; i < ElevatorNum; i++) {
            elevators[i]->m_quit=true;
            //join函数回收该线程使用的资源
            (*threads[i]).quit();
            (*threads[i]).wait();
            delete threads[i];
           elevators[i]->deleteLater();
        }
        for(int i=0;i<ElevatorNum;i++){
            for(int j=0;j<FloorNum;j++){
                upmove[i][j]->deleteLater();
                downmove[i][j]->deleteLater();
            }
        }
    }

    //画楼层
    void DrawBuilding();
    //处理请求的顺序
    void SortRequest(Elevator* ele);

signals:
    //电梯向电梯系统发送的请求
    void Request(const ElevatorRequest& request);
    //更新显示器
    void updateDigital(Elevator*ele, int currentfloor,int targetfloor);

public slots:
    //处理请求
    void HandleRequest(const ElevatorRequest& request);

    //进入电梯内部
    void EnterElevator();

    void on_opendoor_clicked();//开门
    void on_closedoor_clicked();//关门
    void on_floorButton_clicked();//选择楼层
    void on_warningButton_clicked();//警告

    void UpdateDigitalDisplay(Elevator* ele, int currentfloor,int targetfloor);//更新数码显示器

    //移动
    void Elemove(Elevator* ele);

    //电梯到达
    void AfterReached(Elevator* ele);
};

#endif
