#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<QDebug>
//主窗口大小
#define WindowsLength 1500
#define WindowsWidth 900
#define RowSpace 45//行距
#define ColSpace 300//列距

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化退出按钮
    exitButton=new QPushButton("Exit");
    //设置整个窗口为白色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    ui->setupUi(this);
    this->setWindowFlags(Qt::Widget);//将窗口属性设置为Widget,方便后续类的界面绘制
    this->setWindowTitle(QString("电梯调度系统"));
    this->setFixedSize(WindowsLength,WindowsWidth);//设置程序的主窗口大小

    //创建一个电梯调度系统
    elevatorSystem=new System(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){//窗口变化时自动调用
    //退出键
    exitButton->setParent(this);
    exitButton->resize(RowSpace,RowSpace);
    exitButton->move(MovePositionX+2*RowSpace,MovePositionY-FloorNum*RowSpace+RowSpace);
    connect(exitButton,&QPushButton::clicked,this,&MainWindow::Exit);
    exitButton->show();

    //创建对象
    QPainter painter;
    painter.begin(this);

    //绘图前准备画笔、画刷
    QPen pen; //画笔。绘制图形边线，由颜色、宽度、线风格等参数组成
    pen.setColor(QColor(46,139,87,255));//前三个参数表示RGB(此处是seagreen4)，第四个参数表示透明度（255为全色）
    //开始绘图
    painter.setPen(pen);
    //*******************绘画主窗口布局*******************
    //绘画19条横线，将主窗口分为20层楼
    for(int i = 0; i <= FloorNum - 1; i++)
        painter.drawLine(QPoint(0, i*RowSpace), QPoint(WindowsLength, i*RowSpace));
    //绘画4条竖线,将窗口分为五部电梯
    for(int i = 0; i <= ElevatorNum - 1; i++)
        painter.drawLine(QPoint(i*ColSpace, 0), QPoint(i*ColSpace, WindowsWidth));
}

//处理鼠标点击事件（选择一个楼层）
//void MainWindow::mouseReleaseEvent(QMouseEvent *ev){
//    if (ev->button() != Qt::LeftButton) { // 排除鼠标右键点击
//        return;
//    }
//    QPoint pt= ev->pos();  //获取当前鼠标位置坐标
//    int clicked_x=pt.x(),clicked_y=pt.y();
//    for(int i=0;i<FloorNum;i++){
//        for(int j=0;j<ElevatorNum;j++){
//            qDebug()<<clicked_x<<"#"<<elevatorSystem->elevators[j]->floorButton[i]->pos().x()<<"#"<<elevatorSystem->elevators[j]->floorButton[i]->pos().rx();
//            if(clicked_x>elevatorSystem->elevators[j]->floorButton[i]->pos().x()
//                    &&clicked_x < (elevatorSystem->elevators[j]->floorButton[i]->pos().rx()+RowSpace)
//                    &&clicked_y>elevatorSystem->elevators[j]->floorButton[i]->pos().y()
//                    &&clicked_y< (elevatorSystem->elevators[j]->floorButton[i]->pos().ry()+RowSpace)){
//                //修改属性，增加请求
//                QMessageBox warningBox;
//                warningBox.setText(QString::asprintf("选中了楼层%d",i));
//                warningBox.setWindowTitle("chenggong ");
//                warningBox.exec();//返回用户单击的按钮的值
//            }
//        }
//    }
//}
