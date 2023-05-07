#ifndef SYSTEM_H
#define SYSTEM_H

#include <QVector>
#include <QQueue>
#include <QThread>//�����߳�
#include <QDebug>
#include <QButtonGroup>

#include "Elevator.h"
//ʹ�������ڵ�uiָ��
//#include "ui_mainwindow.h"
//class MainWindow;

////�µ��߳���
//class RunEleThread:public QThread{
//    Q_OBJECT
//private:
//    Elevator* elevator;//���ݵ�ַ
//public:
//    RunEleThread(Elevator* mid){
//        elevator=mid;
//    }
//protected:
//    void run() override{
//        // �����źŲ��������ӵ��ۺ���
//        //QMetaObject::invokeMethod(elevator, "Run", Qt::QueuedConnection);//�������ò�û��ʵ��������߳��зֱ�ִ��Run������ֻ��������߳��е������߳��е�Run����������
//        while(!elevator->m_quit){
//            //�ȴ�����
//            //����ķ������߳���Դ
//            elevator->ele_mutex.lock();
//            while(elevator->requests.isEmpty()&&!elevator->m_quit){
//                if(!elevator->ele_condition.wait(&elevator->ele_mutex))
//                    break;
//            }
//            //�������ϵͳ��������
//            if(elevator->m_quit){
//                break;
//            }
//            //������������е�����
//            ElevatorRequest mid=elevator->requests.takeFirst();
//            elevator->HandleRequestSingle(mid);
//           elevator-> ele_mutex.unlock();
//        }
//    }
//};

//���ݵ���ϵͳ*******************************
class System: public QWidget {
    Q_OBJECT
private:
    //�����б�
    QVector<Elevator*> elevators;
    //�߳��б�
    QVector<QThread*> threads;
    //����¥�İ�ť
    QPushButton* upmove[ElevatorNum][FloorNum];
    QPushButton* downmove[ElevatorNum][FloorNum];
    QButtonGroup upButtonGroup[FloorNum];
    QButtonGroup downButtonGroup[FloorNum];

    friend class MainWindow;

public:
    QWidget* SysUI;//ָ�������ڵ�ָ��

    explicit System(QWidget*p):SysUI(p) {
        // ע��ElevatorRequestΪԪ����
        qRegisterMetaType<ElevatorRequest>("ElevatorRequest");
        connect(this,&System::Request,this,&System::HandleRequest);

        //��ʼ��¥������¼�
        for(int i=0;i<ElevatorNum;i++){
            for(int j=0;j<FloorNum;j++){
                upmove[i][j]=new QPushButton;
                downmove[i][j]=new QPushButton;
            }
        }//��ʼ����ť��
        for(int j=0;j<FloorNum;j++){
            for(int i=0;i<ElevatorNum;i++){
                upButtonGroup[j].addButton(upmove[i][j]);
                downButtonGroup[j].addButton(downmove[i][j]);
            }
            // ���Ӱ�ť��� buttonClicked �ź�
            connect(&upButtonGroup[j], QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=]() {
                // ����ť��� exclusive ��������Ϊ false���Ա����а�ť�����Ա��ָ���״̬
                upButtonGroup[j].setExclusive(false);
                // �������а�ť�������ǵĸ���״̬����Ϊ true
                QList<QAbstractButton *> buttons = upButtonGroup[j].buttons();
                for (QAbstractButton *btn : buttons) {
                    btn->setStyleSheet("QPushButton{border-image: url(../Elevator/upmove_selected.png);}");
                }
                // ����ť��� exclusive �������û� true���Ա�ֻ��һ����ť���Ա�ѡ��
                upButtonGroup[j].setExclusive(true);
                //��������
                ElevatorRequest request;
                request.requestfloor=j+1;
                request.direction=Direction::up;
                HandleRequest(request);
            });
            connect(&downButtonGroup[j], QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=]() {
                // ����ť��� exclusive ��������Ϊ false���Ա����а�ť�����Ա��ָ���״̬
                downButtonGroup[j].setExclusive(false);
                // �������а�ť�������ǵĸ���״̬����Ϊ true
                QList<QAbstractButton *> buttons = downButtonGroup[j].buttons();
                for (QAbstractButton *btn : buttons) {
                    btn->setStyleSheet("QPushButton{border-image: url(../Elevator/downmove_selected.png);}");
                }
                // ����ť��� exclusive �������û� true���Ա�ֻ��һ����ť���Ա�ѡ��
                downButtonGroup[j].setExclusive(true);
                //��������
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
            //join�������ո��߳�ʹ�õ���Դ
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

    //��¥��
    void DrawBuilding();
    //���������˳��
    void SortRequest(Elevator* ele);

signals:
    //���������ϵͳ���͵�����
    void Request(const ElevatorRequest& request);
    //������ʾ��
    void updateDigital(Elevator*ele, int currentfloor,int targetfloor);

public slots:
    //��������
    void HandleRequest(const ElevatorRequest& request);

    //��������ڲ�
    void EnterElevator();

    void on_opendoor_clicked();//����
    void on_closedoor_clicked();//����
    void on_floorButton_clicked();//ѡ��¥��
    void on_warningButton_clicked();//����

    void UpdateDigitalDisplay(Elevator* ele, int currentfloor,int targetfloor);//����������ʾ��

    //�ƶ�
    void Elemove(Elevator* ele);

    //���ݵ���
    void AfterReached(Elevator* ele);
};

#endif
