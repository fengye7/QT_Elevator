#ifndef MAINWINDOW_H   //��ֹ��ͷ�ļ����ظ�����
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "System.h"
class System;//ǰ������

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //��ʼ����ص�ֵ
    void init();
    //����������¼���ѡ��һ��¥�㣩,���������¼�����ʱ�Զ�����
//    void mouseReleaseEvent(QMouseEvent *ev);(û�гɹ��ﵽҪ����Ҫ���޷����յ����԰�ť�ϵĵ�����Ժ�����������ֱ����QPainter)

    //�����ǿ��ӻ�����
protected:
    //ֻҪ����״̬�ı��Զ����ô˺���
    //��Ҫ���ƴ��ڱ���ͨ���˺���ʵ��,���Ƶ���
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    //����һ�����ݵ���ϵͳ
    System* elevatorSystem;

    //�˳���ť
    QPushButton* exitButton;

public slots:
    //�˳�
    void Exit(){
        // �رղ��˳�Ӧ�ó���
        QCoreApplication::quit();
    }
};
#endif // MAINWINDOW_H
