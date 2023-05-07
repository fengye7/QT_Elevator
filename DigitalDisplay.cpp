#include "DigitalDisplay.h"
#include "Elevator.h"

//���캯��
DigitalDisplay::DigitalDisplay(Elevator* t, QWidget*p) : ele(t), DUI(p) {
//    timer=new QTimer(this);
//    connect(timer,&QTimer::timeout,this,&DigitalDisplay::updateDisplay);
//    timer->start(AnimationTime);

    label=new QLabel("1");
    label->setParent(DUI);
    label->setAlignment(Qt::AlignHCenter);//���ֶ��뷽ʽˮƽ����
    label->setStyleSheet("QLabel{color:#ff0000;background-color:#E6E6FA;}"
                         "QLabel{font-family:Microsoft Yahei;font-size:15pt;}"
                         "QLabel{border:2px solid #000000;border-radius:10px;}");
    label->setGeometry(3*RowSpace, 0, 2*RowSpace,RowSpace);//��ʾ����λ��
    label->show();
}
// ������ʾ
void DigitalDisplay::updateDisplay(int currentfloor, int targetfloor) {
    // ����һ��QVariantAnimation����
    QVariantAnimation *animation = new QVariantAnimation(label);
    // ���ö�������ʼֵ�ͽ���ֵ
    animation->setStartValue(currentfloor);
    animation->setEndValue(targetfloor);
    // ���ó���ʱ��������ƶ�ʱ����ȴﵽͬ���仯�������Ǽ���ʵ�֣�ʵ��Ӧ��ʵʱ���¥��仯�����£�
    animation->setDuration(AnimationTime*qFabs(currentfloor-targetfloor));
    // ���ö�����easing curveΪQEasingCurve::OutQuad
    animation->setEasingCurve(QEasingCurve::OutQuad);
    // ����valueChanged()�źźͲۺ���
    connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
        // ��QVariant���͵�valueת��Ϊint���ͣ�����������ΪQLabel���ı�
        label->setText(QString::number(value.toInt()));
        ele->currentfloor=value.toInt();
        if(!ele->requests.isEmpty()&&currentfloor==ele->requests.first().requestfloor&&ele->direction==ele->requests.first().direction&&currentfloor!=targetfloor){
            animation->stop();
            animation->finished();
        }
    });
    animation->start(); // ��������
}
