#include "System.h"
#include <qmath.h>
#define MAXNUM 999999
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <QPropertyAnimation>
/*电梯的上下键规则：
*1、上楼就按上的按钮，在电梯往下的时候就不会在你这个楼层停。往上的时候到了你的楼层这里就停下带你上去。
*
*2、下楼就按向下的按钮，在电梯往上的时候就不会在你这个楼层停。往下的时候到了你的楼层这里就停下带你下去。
*/

//处理请求
void System::HandleRequest(const ElevatorRequest& request) {
    //下面是按照规则的最优分配（也就是选择最优的电梯添加该请求（这里最优应该为时间最优,此处每层楼的运动时间相等就只计算距离））
    int minDistance=MAXNUM;
    int minTurn=0;
    for(int i=0;i<elevators.length();i++){
        if(!elevators[i]->requests.isEmpty()&&elevators[i]->direction==request.direction
                &&elevators[i]->currentfloor==request.requestfloor&&elevators[i]->currentfloor!=elevators[i]->targetfloor)
        {
            elevators[i]->requests.append(request);
            continue;
        }
        if(elevators[i]->direction==Direction::none){//当前空闲的电梯全部添加此请求
            //添加进对应电梯的请求队列
            elevators[i]->requests.append(request);
        }
        else if(elevators[i]->direction==Direction::down&&request.direction==Direction::down){//若是下行电梯下行请求
            if(elevators[i]->currentfloor > request.requestfloor){
                if(minDistance>qFabs(elevators[i]->currentfloor-request.requestfloor)){
                    minDistance=qFabs(elevators[i]->currentfloor-request.requestfloor);
                    minTurn=i;
                }
            }
            else{//此时电梯需要先经历一次转向
                int k=0;
                for(k=0;k<elevators[i]->requests.length();k++)
                    if(elevators[i]->requests[k].direction!=Direction::down)
                        break;
                int thistime=qFabs(elevators[i]->currentfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor))
                        +qFabs(request.requestfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor));
                if(minDistance>thistime){
                    minDistance=thistime;
                    minTurn=i;
                }
            }
        }
        else if(elevators[i]->direction==Direction::up&&request.direction==Direction::up){//若是上行电梯上行请求
            if(elevators[i]->currentfloor < request.requestfloor){
                if(minDistance>qFabs(elevators[i]->currentfloor-request.requestfloor)){
                    minDistance=qFabs(elevators[i]->currentfloor-request.requestfloor);
                    minTurn=i;
                }
            }
            else{//此时电梯需要先经历一次转向
                int k=0;
                for(k=0;k<elevators[i]->requests.length();k++)
                    if(elevators[i]->requests[k].direction!=Direction::up)
                        break;
                int thistime=qFabs(elevators[i]->currentfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor))
                        +qFabs(request.requestfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor));
                if(minDistance>thistime){
                    minDistance=thistime;
                    minTurn=i;
                }
            }
        }
        else if(elevators[i]->direction==Direction::up&&request.direction==Direction::down){//若是上行电梯下行请求
            //此时电梯需要先经历一次转向
            int k=0;
            for(k=0;k<elevators[i]->requests.length();k++)
                if(elevators[i]->requests[k].direction!=Direction::up)
                    break;
            int thistime=qFabs(elevators[i]->currentfloor - (k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor))
                    +qFabs(request.requestfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor));
            if(minDistance>thistime){
                minDistance=thistime;
                minTurn=i;
            }
        }
        else if(elevators[i]->direction==Direction::down&&request.direction==Direction::up){//若是下行电梯上行请求
            //此时电梯需要先经历一次转向
            int k=0;
            for(k=0;k<elevators[i]->requests.length();k++)
                if(elevators[i]->requests[k].direction!=Direction::down)
                    break;
            int thistime =qFabs(elevators[i]->currentfloor-(k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor))
                    +qFabs(request.requestfloor- (k>0?elevators[i]->requests[k-1].requestfloor:elevators[i]->targetfloor));
            if(minDistance>thistime){
                minDistance=thistime;
                minTurn=i;
            }
        }
    }
    if(elevators[minTurn]->direction!=Direction::none){//电梯静止的情况已经处理过
        //添加进对应电梯的请求队列
        elevators[minTurn]->requests.append(request);
    }
}

//画楼层
void System::DrawBuilding(){
    //每一层的上行、下行键
    for(int i=0;i<ElevatorNum;i++){
        for(int j=0;j<FloorNum;j++){
            //上行键
            if(1){
                upmove[i][j]->setParent(SysUI);//将按钮加在当前窗口
                upmove[i][j]->resize(RowSpace,RowSpace);
                upmove[i][j]->setStyleSheet("QPushButton{border-image: url(../Elevator/upmove.png);}"                     //正常样式
                                            "QPushButton:hover{border-image: url(../Elevator/upmove_selected.png);}");                // 选中样式
                upmove[i][j]->move(MovePositionX+i*ColSpace,MovePositionY-j*RowSpace);
                //                connect(upmove[i][j],SIGNAL(clicked()),this,SLOT(on_upmoveButton_clicked()));//按钮连接到一个函数
                upmove[i][j]->show();
            }
            //下行键
            if(1){
                downmove[i][j]->setParent(SysUI);//将按钮加在当前窗口
                downmove[i][j]->resize(RowSpace,RowSpace);
                downmove[i][j]->setStyleSheet("QPushButton{border-image: url(../Elevator/downmove.png);}"                     //正常样式
                                              "QPushButton:hover{border-image: url(../Elevator/downmove_selected.png);}");                // 选中样式
                downmove[i][j]->move(MovePositionX+RowSpace+i*ColSpace,MovePositionY-j*RowSpace);
                //                connect(downmove[i][j],SIGNAL(clicked()),this,SLOT(on_downmoveButton_clicked()));//按钮连接到一个函数
                downmove[i][j]->show();
            }
        }
    }
    //楼层标签
    for(int j=1;j<=ElevatorNum;j++){
        for(int i=0;i<FloorNum;i++){
            QLabel* floorLabel=new QLabel;
            floorLabel->setParent(SysUI);//将标签设置在主窗口
            floorLabel->resize(RowSpace,RowSpace);
            floorLabel->setText(QString::number(i+1));//将数字转换为字符型
            floorLabel->move(j*ColSpace-RowSpace, WindowsWidth-(i+1)*RowSpace);
            floorLabel->show();
        }
    }
}

//开门
void System::on_opendoor_clicked()
{
    // 获取发送信号的对象
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 区分是哪一个按钮发来的信号
    for(int i=0;i<ElevatorNum;i++){
        if(senderButton==elevators[i]->openButton)
            elevators[i]->Opendoor();
    }
}
//关门
void System::on_closedoor_clicked()
{
    // 获取发送信号的对象
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 区分是哪一个按钮发来的信号
    for(int i=0;i<ElevatorNum;i++){
        if(senderButton==elevators[i]->closeButton)
            elevators[i]->Closedoor();
    }
}
//警告
void System::on_warningButton_clicked()
{
    // 获取发送信号的对象
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 区分是哪一个按钮发来的信号
    for(int i=0;i<ElevatorNum;i++){
        if(senderButton==elevators[i]->alarmButton)
            elevators[i]->Warning();
    }
}
//选择楼层
void System::on_floorButton_clicked(){
    /*
 *到目前为止，电梯运行的基本原则叫顺向截停。
 *意思是电梯一定服务完相同运行方向要求后再换个方向运行，反复循环。
*/
    // 获取发送信号的对象
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 区分是哪一个按钮发来的信号
    for(int id=0;id<ElevatorNum;id++){
        for(int i=0;i<FloorNum;i++){
            if (senderButton == elevators[id]->floorButton[i]) {
                //设置选中该层楼
                elevators[id]->floorButton[i]->setStyleSheet("QPushButton{border: 1px solid red;}");

                ElevatorRequest mid;
                mid.direction=(i+1-elevators[id]->currentfloor>0)?(Direction::up):(Direction::down);
                mid.requestfloor=i+1;
                elevators[id]->requests.append(mid);//添加到请求队列，遵循上述顺向截停原则调整请求顺序
            }
        }
    }
}

//更新显示器
void System::UpdateDigitalDisplay(Elevator* ele, int currentfloor,int targetfloor)
{
    ele->digitaldisplay->updateDisplay(currentfloor,targetfloor);
}

//电梯移动
void System::Elemove(Elevator* ele) {
    ele->Elemove();
}

//进入电梯内部
void System::EnterElevator(){
    // 获取发送信号的对象
    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    // 区分是哪一个按钮发来的信号
    for(int i=0;i<ElevatorNum;i++){
        if(senderButton==elevators[i]->upBorder)
            elevators[i]->ElevatorPage->show();
    }
}

//请求调度
bool myupcmp(ElevatorRequest& request1,ElevatorRequest& request2);
bool mydowncmp(ElevatorRequest& request1,ElevatorRequest& request2);
//处理电梯调度的函数
void System::SortRequest(Elevator* ele)
{
    //根据当前运行状态同向到最后，一个方向结束才处理另一个方向
    if(ele->requests.length()>0){
        ElevatorRequest mid;
        for(int i=1;i<ele->requests.length();i++){
            if(ele->requests[i].direction!=ele->requests[0].direction){//若不同就交换
                for(int j=i+1;j<ele->requests.length();j++){
                    if(ele->requests[j].direction==ele->requests[0].direction){
                        mid=ele->requests[j];
                        ele->requests[j]=ele->requests[i];
                        ele->requests[i]=mid;
                        break;
                    }
                }
            }
        }
        int countnum=1;//同向的请求计数
        for(int i=1;i<ele->requests.length();i++){
            if(ele->requests[i].direction==ele->requests[0].direction)
                countnum++;
        }
        if(ele->requests[0].direction==Direction::up){
            std::sort(ele->requests.begin(),ele->requests.begin()+countnum,myupcmp);
            std::sort(ele->requests.begin()+countnum,ele->requests.end(),mydowncmp);
        }
        else{
            std::sort(ele->requests.begin(),ele->requests.begin()+countnum,mydowncmp);
            std::sort(ele->requests.begin()+countnum,ele->requests.end(),myupcmp);
        }
    }
}

//电梯到达
void System::AfterReached(Elevator* ele){
    upButtonGroup[ele->currentfloor-1].setExclusive(false);
    QList<QAbstractButton *> buttons = upButtonGroup[ele->currentfloor-1].buttons();
    for (QAbstractButton *btn : buttons) {
        btn->setStyleSheet("QPushButton{border-image: url(../Elevator/upmove.png);}");
    }
    upButtonGroup[ele->currentfloor-1].setExclusive(true);

    downButtonGroup[ele->currentfloor-1].setExclusive(false);
    QList<QAbstractButton *> buttons2 = downButtonGroup[ele->currentfloor-1].buttons();
    for (QAbstractButton *btn : buttons2) {
        btn->setStyleSheet("QPushButton{border-image: url(../Elevator/upmove.png);}");
    }
    downButtonGroup[ele->currentfloor-1].setExclusive(true);

    ele->HandleReached();
}
