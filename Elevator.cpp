#include "Elevator.h"
#include <QPropertyAnimation>//动画实现
#include <QPoint>
#include <QMessageBox>//模态对话框
#include <QString>
#include <QMetaObject>//子线程申请父线程的资源
#include <QThread>
#include <QEventLoop>//确保程序等待动画结束才继续运行
#include <QParallelAnimationGroup>//动画组

#include <algorithm>//调用排序
#include <iostream>

/*下面开始是Elevator类的成员函数实现****************************/
//构造函数，默认初始楼层为1，静止，关门，不报警
Elevator::Elevator(int id, QWidget*p): exUI(p){
    //赋值初始默认值
    this->id=id;
    currentfloor=1;
    targetfloor=1;
    direction=Direction::none;
    controldoor=false;
    warning=false;
    m_quit=false;

    //以下是电梯内的对象先申请空间
    ElevatorPage = new QWidget;
    //数字键
    for(int i=0;i<FloorNum;i++)
        floorButton[i]=new QPushButton;
    //警报键
    alarmButton=new QPushButton;
    //开门键
    openButton=new QPushButton;
    //关门键
    closeButton=new QPushButton;
    //数码显示器
    digitaldisplay=new DigitalDisplay(this,ElevatorPage);
    //电梯主体
    upBorder=new QPushButton;
    leftdoor=new QPushButton;
    rightdoor=new QPushButton;
    for(int i=0;i<FloorNum;i++){
    }
    //调用函数画出电梯
    this->DrawElevator();
    //画出电梯内部
    ElevatorPageShow();
}

//电梯移动
void Elevator::Elemove() {
    if(controldoor==false){//电梯必须门关着
        QList<QPushButton*> Ele;
        QList<QPropertyAnimation*> Animations;
        Ele.append(upBorder),Ele.append(leftdoor),Ele.append(rightdoor);
        for(int i=0;i<Ele.count();i++){
            Animations.append(new QPropertyAnimation(Ele.at(i),"pos"));//设置对象和移动方式pos(按点移动)
            Animations.at(i)->setEasingCurve(QEasingCurve::OutQuad); //设置动画效果,先加速后减速
            Animations.at(i)->setDuration(AnimationTime*qFabs(targetfloor-currentfloor));//设置动画时间（ms）
            Animations.at(i)->setStartValue(Ele.at(i)->pos());//起始位置
            Animations.at(i)->setEndValue(QPoint(Ele.at(i)->pos().rx(),WindowsWidth-targetfloor*RowSpace+(i>0?UpBorderWidth:0)));//停止位置
            Animations.at(i)->start();
        }
        connect(Animations.at(0),&QPropertyAnimation::valueChanged,[=](){//运动期间有新的请求截停
            for(int k=1;k<=FloorNum;k++){
                if(qFabs(Ele.at(0)->pos().ry()-WindowsWidth+k*RowSpace)<5){
                    currentfloor=k;
                    break;
                }
            }
            digitaldisplay->label->setText(QString::number(currentfloor));//更新显示

            if(!requests.isEmpty()&&currentfloor==requests.first().requestfloor&&direction==requests.first().direction&&currentfloor!=targetfloor){
                Animations.at(0)->stop(),Animations.at(1)->stop(),Animations.at(2)->stop();
                Animations.at(0)->finished(),Animations.at(1)->finished(),Animations.at(2)->finished();
                floorButton[currentfloor-1]->setStyleSheet("QPushButton:normal");
                direction=Direction::none;
                currentfloor=requests.first().requestfloor;
                emit reached(this);
                return;
            }
        });

        QEventLoop loop;
        QObject::connect(Animations.at(0), &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
        loop.exec();
        direction=Direction::none;
        currentfloor=targetfloor;
        emit reached(this);
    }
    else{
        QMessageBox warningBox;
        warningBox.setText(QString::asprintf("电梯%d门未关！",id));
        warningBox.setWindowTitle("警告");
        warningBox.exec();//返回用户单击的按钮的值
    }
}

//电梯门控制
void Elevator::Closedoor() {
    if(direction==Direction::none&&controldoor==true){//门是开的才能关
        //左半边
        QPropertyAnimation* Animation1=new QPropertyAnimation(leftdoor,"geometry");//设置对象和移动方式geometry(边框形式)
        Animation1->setEasingCurve(QEasingCurve::Linear); //设置动画效果
        Animation1->setDuration(AnimationTime*2);//设置动画时间（ms）
        Animation1->setStartValue(QRect(leftdoor->pos(),QSize(10,DoorWidth)));
        Animation1->setEndValue(QRect(leftdoor->pos(),QSize(DoorLength,DoorWidth)));
        //右半边
        QPropertyAnimation* Animation2=new QPropertyAnimation(rightdoor,"geometry");//设置对象和移动方式geometry(边框形式)
        Animation2->setEasingCurve(QEasingCurve::Linear); //设置动画效果
        Animation2->setDuration(AnimationTime*2);//设置动画时间（ms）
        Animation2->setStartValue(QRect(QPoint(leftdoor->pos().x()+2*DoorLength-10,leftdoor->pos().y()),QSize(10,DoorWidth)));
        Animation2->setEndValue(QRect(QPoint(leftdoor->pos().x()+DoorLength,leftdoor->pos().y()),QSize(DoorLength,DoorWidth)));
        Animation1->start();
        Animation2->start();
        // Wait for the animation to finish
        QEventLoop loop;
        QObject::connect(Animation1, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
        loop.exec();
        controldoor=false;//门关了
    }
}
void Elevator::Opendoor(const bool choose) {
    if(direction==Direction::none&&controldoor==false){//门是关的才能开
        //左半边
        QPropertyAnimation* Animation1=new QPropertyAnimation(leftdoor,"geometry");//设置对象和移动方式geometry(边框形式)
        Animation1->setEasingCurve(QEasingCurve::Linear); //设置动画效果
        Animation1->setDuration(AnimationTime*2);//设置动画时间（ms）
        Animation1->setStartValue(QRect(leftdoor->pos(),QSize(DoorLength,DoorWidth)));//起始位置
        Animation1->setEndValue(QRect(leftdoor->pos(),QSize(10,DoorWidth)));//停止位置
        //右半边
        QPropertyAnimation* Animation2=new QPropertyAnimation(rightdoor,"geometry");//设置对象和移动方式geometry(边框形式)
        Animation2->setEasingCurve(QEasingCurve::Linear); //设置动画效果
        Animation2->setDuration(AnimationTime*2);//设置动画时间（ms）
        Animation2->setStartValue(QRect(QPoint(leftdoor->pos().x()+DoorLength,leftdoor->pos().y()),QSize(DoorLength,DoorWidth)));//起始位置
        Animation2->setEndValue(QRect(QPoint(leftdoor->pos().x()+2*DoorLength-10,leftdoor->pos().y()),QSize(10,DoorWidth)));//停止位置

        Animation1->start();
        Animation2->start();
        // Wait for the animation to finish
        QEventLoop loop;
        QObject::connect(Animation1, &QPropertyAnimation::finished, &loop, &QEventLoop::quit);
        loop.exec();
        controldoor=true;//门关了
        if(choose==true)
            Closedoor();
    }
}
//报警
void Elevator::Warning() {
    QMessageBox warningBox;
    warningBox.setText(QString::asprintf("电梯%d不能运行!",id));
    warningBox.setWindowTitle("警告");
    //    warningBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    //    warningBox.setDefaultButton(QMessageBox::Save);这两行是增加用户选项，并设置默认选项
    warningBox.exec();//返回用户单击的按钮的值
}
//绘制一个电梯
void Elevator::DrawElevator(){
    //用一些QObject拼成一个电梯形状（注意：主窗口这里为1500:900，有5部电梯
    //电梯主体*****************************************
    //电梯上框
    if(1){
        upBorder->setParent(exUI);//将按钮加在当前窗口
        upBorder->resize(UpBorderLength,UpBorderWidth);
        QString trunid=QString::number(id); //将id转化为文本
        upBorder->setText(QString("电梯%1")
                          .arg(trunid));
        upBorder->move((id-1)*ColSpace,EPosition);  //根据id控制位置
        upBorder->show();
    }
    //电梯左门
    if(1){
        leftdoor->setParent(exUI);//将按钮加在当前窗口
        leftdoor->resize(DoorLength,DoorWidth);
        leftdoor->move((id-1)*ColSpace,EPosition+UpBorderWidth);
        leftdoor->show();}
    //电梯右门
    if(1){
        rightdoor->setParent(exUI);//将按钮加在当前窗口
        rightdoor->resize(DoorLength,DoorWidth);
        rightdoor->move((id-1)*ColSpace+DoorLength,EPosition+UpBorderWidth);
        rightdoor->show();}
}
//进入选中电梯内部的界面
void Elevator::ElevatorPageShow(){
    // 电梯页面
    ElevatorPage->resize(ElevatorPageSize,ElevatorPageSize);
    ElevatorPage->setWindowTitle(QString::asprintf("电梯%d",id));
    //数字键选择楼层
    for(int i=0;i<FloorNum;i++){
        floorButton[i]->setParent(ElevatorPage);//将按钮加在电梯窗口
        floorButton[i]->resize(RowSpace,RowSpace);
        floorButton[i]->setText(QString::number(i+1));//将数字转换为字符型
        floorButton[i]->move((i%2==1?0:RowSpace), (FloorNum/2-(i/2)-1)*RowSpace);
        floorButton[i]->show();
    }
    //警报键**********************************
    if(1){
        alarmButton->setParent(ElevatorPage);//将按钮加在当前窗口
        alarmButton->resize(RowSpace,RowSpace);
        alarmButton->setStyleSheet("QPushButton{border-image: url(E:/Exercise/QT/Elevator/warning.png);}"                     //正常样式
                                   "QPushButton:hover{border-image: url(E:/Exercise/QT/Elevator/warning_select.png);}" );                // 选中样式
        alarmButton->move(3*RowSpace,(FloorNum/2-5)*RowSpace);
        alarmButton->show();}
    //开门键
    if(1){
        openButton->setParent(ElevatorPage);//将按钮加在当前窗口
        openButton->resize(RowSpace,RowSpace);
        openButton->setStyleSheet("QPushButton{border-image: url(E:/Exercise/QT/Elevator/opendoor.png);}"                     //正常样式
                                  "QPushButton:hover{border-image: url(E:/Exercise/QT/Elevator/opendoor_selected.png);}" );                // 选中样式
        openButton->move(3*RowSpace,(FloorNum/2-3)*RowSpace);
        openButton->show();}
    //关门键
    if(1){
        closeButton->setParent(ElevatorPage);//将按钮加在当前窗口
        closeButton->resize(RowSpace,RowSpace);
        closeButton->setStyleSheet("QPushButton{border-image: url(E:/Exercise/QT/Elevator/closedoor.png);}"                     //正常样式
                                   "QPushButton:hover{border-image: url(E:/Exercise/QT/Elevator/closedoor_selected.png);}" );                // 选中样式
        closeButton->move(4*RowSpace,(FloorNum/2-3)*RowSpace);
        closeButton->show();}
    ElevatorPage->show();
}

//槽函数********************************************************
//***************************************************************
//***************************************************************
//处理来自子线程的资源请求,电梯开始运行
void  Elevator::Run(){
    while(!m_quit){
        //等待请求
        //互斥的访问主线程资源
        //如果电梯系统宣布结束
        if(m_quit){
            break;
        }
        _Sort_Request();
        //处理请求队列中的请求
        ele_mutex.lock();
        if(!requests.isEmpty()&&direction==Direction::none){
            HandleRequestSingle(requests.first());
        }
        ele_mutex.unlock();
    }
}
//处理请求
void Elevator::HandleRequestSingle(const ElevatorRequest& mid) {
    direction=mid.direction;
    targetfloor=mid.requestfloor;
    if(currentfloor!=targetfloor)
        emit startElemove(this);
    else
        emit reached(this);
}

//完成请求后的处理
void Elevator::HandleReached(){
    ele_mutex.lock();
    if(!requests.isEmpty())
        requests.pop_front();
    ele_mutex.unlock();
    floorButton[currentfloor-1]->setStyleSheet("QPushButton:normal");
    Opendoor(true);
}

//**********************************************************
//两种排序顺序
bool myupcmp(ElevatorRequest& request1,ElevatorRequest& request2){
    return request1.requestfloor<request2.requestfloor;
}
bool mydowncmp(ElevatorRequest& request1,ElevatorRequest& request2){
    return request1.requestfloor>request2.requestfloor;
}

//处理电梯调度的函数
void Elevator::_Sort_Request()
{
    //根据当前运行状态同向到最后，一个方向结束才处理另一个方向
    if(requests.length()>0){
        ElevatorRequest mid;
        for(int i=1;i<requests.length();i++){
            if(requests[i].direction!=requests[0].direction){//若不同就交换
                for(int j=i+1;j<requests.length();j++){
                    if(requests[j].direction==requests[0].direction){
                        mid=requests[j];
                        requests[j]=requests[i];
                        requests[i]=mid;
                        break;
                    }
                }
            }
        }
        int countnum=1;//同向的请求计数
        for(int i=1;i<requests.length();i++){
            if(requests[i].direction==requests[0].direction)
                countnum++;
        }
        if(requests[0].direction==Direction::up){
            std::sort(requests.begin(),requests.begin()+countnum,myupcmp);
            std::sort(requests.begin()+countnum,requests.end(),mydowncmp);
        }
        else{
            std::sort(requests.begin(),requests.begin()+countnum,mydowncmp);
            std::sort(requests.begin()+countnum,requests.end(),myupcmp);
        }
    }
}
