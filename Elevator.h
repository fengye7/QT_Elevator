#ifndef ELEVATOR_H
#define ELEVATOR_H

#define FloorNum 20 //����
#define ElevatorNum 5//������
#define OneFloorHeight 10//����¥�߶�
#define SleepTime 10//����ʱ��

//������һЩUI�ĳ�������**************
#define WindowsLength 1500
#define WindowsWidth 900
#define ElevatorPageSize 600
#define RowSpace 45//�о�
#define ColSpace 300//�о�
//����UI
#define UpBorderLength 80
#define UpBorderWidth 20
#define DoorLength 40
#define DoorWidth 25
//λ�ÿ���(��Ϊ����1��λ�ã��������ݴ��ڲ��ֱ仯)
#define EPosition 855  //����
#define MovePositionX 120//�����а�ť
#define MovePositionY 855
//��������ʱ��
#define AnimationTime 500

//���ڻ�UI����
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QPoint>
#include <QMouseEvent>//���������
#include <QMutex>//������ʵ��ͬ�����Ʊ�������
#include <QWaitCondition>

#include "DigitalDisplay.h"  //������ʾ��

//ʹ�������ڵ�uiָ��
//#include "ui_mainwindow.h"
//class MainWindow;

//�ƶ�����
enum class Direction {
    up,
    down,
    none  //��ֹ
};
//���������
struct ElevatorRequest {
    int requestfloor;//�����¥��
    Direction direction;//������
};

//���������
class Elevator :public QObject{
    Q_OBJECT
private:
    //���ݱ��
    int id;
    //��ǰ¥��
    int currentfloor;
    //Ŀ��¥��
    int targetfloor;
    //�������С����С���ֹ��
    Direction direction;
    //������
    bool controldoor;
    //������
    bool warning;
    //�����ڲ�����
    QWidget *ElevatorPage;
    //�����Ƿ��������
    bool m_quit;

    //������
    QMutex ele_mutex;

    // �������
    QVector<ElevatorRequest> requests;

    //���ӻ�UI���
    //20��¥�㰴ť/һ��������ť/�����Ű�ť/���а�ť/���а�ť
    QPushButton*  floorButton[FloorNum];
    QPushButton* alarmButton;
    QPushButton* closeButton;
    QPushButton* openButton;
    //��������
    QPushButton* upBorder;
    QPushButton* leftdoor;
    QPushButton* rightdoor;

    friend class MainWindow;
    friend class  System;
    friend class DigitalDisplay;
    friend class RunEleThread;

public:
    //���캯����Ĭ�ϳ�ʼ¥��Ϊ1����ֹ�����ţ�������
    QWidget* exUI;//����ָ��ָ��������

    explicit Elevator(int id,  QWidget*p);
    //������ʾ��
    DigitalDisplay* digitaldisplay;
    //�����ƶ�
    void Elemove();

    //�����ſ���
    void Closedoor();
    void Opendoor(const bool choose=false);
    //����
    void Warning();
    //����һ������
    void DrawElevator();
    //�����ڲ�
    void ElevatorPageShow();
    //��������
    void HandleRequestSingle(const ElevatorRequest& mid);
    //������ݵ��ȵĺ�������
    void _Sort_Request();
    //�����������̵߳���Դ����,������������
    void Run();
    //��������Ĵ���
    void HandleReached();

signals:
    //��ʼ��������
    void startElemove(Elevator* ele);
    //��������Ĳ���
    void reached(Elevator* ele);
};

#endif // ELEVATOR_H
