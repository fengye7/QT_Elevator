#include "DigitalDisplay.h"
#include "Elevator.h"

//构造函数
DigitalDisplay::DigitalDisplay(Elevator* t, QWidget*p) : ele(t), DUI(p) {
//    timer=new QTimer(this);
//    connect(timer,&QTimer::timeout,this,&DigitalDisplay::updateDisplay);
//    timer->start(AnimationTime);

    label=new QLabel("1");
    label->setParent(DUI);
    label->setAlignment(Qt::AlignHCenter);//文字对齐方式水平居中
    label->setStyleSheet("QLabel{color:#ff0000;background-color:#E6E6FA;}"
                         "QLabel{font-family:Microsoft Yahei;font-size:15pt;}"
                         "QLabel{border:2px solid #000000;border-radius:10px;}");
    label->setGeometry(3*RowSpace, 0, 2*RowSpace,RowSpace);//显示器的位置
    label->show();
}
// 更新显示
void DigitalDisplay::updateDisplay(int currentfloor, int targetfloor) {
    // 创建一个QVariantAnimation对象
    QVariantAnimation *animation = new QVariantAnimation(label);
    // 设置动画的起始值和结束值
    animation->setStartValue(currentfloor);
    animation->setEndValue(targetfloor);
    // 设置持续时间与电梯移动时间相等达到同步变化（这里是简易实现，实际应该实时检测楼层变化来更新）
    animation->setDuration(AnimationTime*qFabs(currentfloor-targetfloor));
    // 设置动画的easing curve为QEasingCurve::OutQuad
    animation->setEasingCurve(QEasingCurve::OutQuad);
    // 连接valueChanged()信号和槽函数
    connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
        // 将QVariant类型的value转换为int类型，并将其设置为QLabel的文本
        label->setText(QString::number(value.toInt()));
        ele->currentfloor=value.toInt();
        if(!ele->requests.isEmpty()&&currentfloor==ele->requests.first().requestfloor&&ele->direction==ele->requests.first().direction&&currentfloor!=targetfloor){
            animation->stop();
            animation->finished();
        }
    });
    animation->start(); // 启动动画
}
