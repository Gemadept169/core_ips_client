#include "toast.h"
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGridLayout>


QList<ToastWidget *> ToastWidget::allToasts = {};

ToastWidget::ToastWidget(QWidget *parent): QWidget(parent){
    auto layout = new QGridLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setHorizontalSpacing(12);
    layout->setVerticalSpacing(6);
    lbIcon = new QLabel(this);
    lbIcon->setFixedSize(24, 24);

    lbTitle = new QLabel(this);
    lbContent = new QLabel(this);
    lbContent->setWordWrap(true);

    btClose = new QPushButton(this);
    btClose->setFixedSize(24, 24);
    btClose->setAccessibleName("inactive");
    static QString buttonStylesheetPattern = "QPushButton { border: none; background-color: transparent } "
                                             "QPushButton:pressed { border: none; background-color: #33ffffff }"
                                             "QPushButton:hover { border: none; background-color: #44000000 } ";
    btClose->setStyleSheet(buttonStylesheetPattern);
    btClose->setIcon(QIcon(":/dark/icon/ic_close.png"));
    connect(btClose, &QPushButton::clicked, [=](){
        setupAnimationHide();
    });

    auto wAction = new QWidget;
    wAction->setStyleSheet("border: none");
    auto wActionLayout = new QHBoxLayout(wAction);
    wActionLayout->setMargin(0);

    layout->addWidget(lbIcon, 0, 0, 1, 1);
    layout->addWidget(lbTitle, 0, 1, 1, 1);
    layout->addWidget(btClose, 0, 2, 1, 1);
    layout->addWidget(lbContent, 1, 1, 1, 2);
    layout->addWidget(wAction, 2, 0, 1, 3);

    btAction = new QPushButton;
    btAction->setAccessibleName("active");
    btAction->setFixedHeight(24);
    btAction->setMinimumWidth(80);
    btAction->setVisible(false);

    wActionLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    wActionLayout->addWidget(btAction);
}

void ToastWidget::setType(ToastWidget::Type type)
{
    const static QString stylesheetPattern = QString(".QWidget { border-radius: 6px; border: 1px solid %1 }");

    QPixmap pixmap;
    QColor color;
    switch (type){
    case Info:
        pixmap = QPixmap(":/dark/icon/noti/ic_info.png").scaled(24, 24);
        color = QColor("#8b59b6");
        break;
    case Success:
        pixmap = QPixmap(":/dark/icon/noti/ic_done.png").scaled(24, 24);
        color = QColor("#1abc9c");
        break;
    case Warning:
        pixmap = QPixmap(":/dark/icon/noti/ic_warning.png").scaled(24, 24);
        color = QColor("#e67e22");
        break;
    case Critical:
        pixmap = QPixmap(":/dark/icon/noti/ic_critical.png").scaled(24, 24);
        color = QColor("#e74c3c");
        break;
    }
    lbIcon->setPixmap(pixmap);
    lbTitle->setStyleSheet(QString("color: %1; font-family: \"Roboto\"").arg(color.name()));
    setStyleSheet(stylesheetPattern.arg(color.darker().name()));
}

void ToastWidget::setClosable(bool closable)
{
    btClose->setVisible(closable);
}

void ToastWidget::setContent(const QString &title, const QString &message)
{
    lbTitle->setText(title);
    lbContent->setText(message);
}

void ToastWidget::setupAnimationShow()
{
    // Tìm kiếm widget ngoài cùng (có parent = nullptr);
    QWidget *mainWidget = parentWidget();
    while (mainWidget->parentWidget() != nullptr){
        mainWidget = mainWidget->parentWidget();
    }

    int xPosition = mainWidget->width() - FixedWidth - Padding;
    // Vị trí theo chiều dọc của Toast, vị trí này phụ thuộc vào option.position
    int yPosition = mainWidget->height() - FixedHeight - Padding;
    for (auto t: allToasts){
        yPosition -= (t->height() + Spacing);
    }

    auto animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(150);
    animation->setStartValue(QPoint(mainWidget->width(), yPosition));
    animation->setEndValue(QPoint(xPosition, yPosition));
    animation->start();

    show();
}

void ToastWidget::setupDelayHide()
{
    auto timerRemove = new QTimer(this);
    timerRemove->setSingleShot(true);
    connect(timerRemove, &QTimer::timeout, [=](){
        setupAnimationHide();
    });
    int durationMs = 0;
    switch (duration){
    case Duration::Short: durationMs = 1000; break;
    case Duration::Medium: durationMs = 3000; break;
    case Duration::Long: durationMs = 5000; break;
    case Duration::VeryLong: durationMs = 10000; break;
    }

    timerRemove->start(durationMs);
}

void ToastWidget::setupAnimationHide()
{
    if (animationDelete != nullptr){
        return;
    }
    // Tìm kiếm widget ngoài cùng (có parent = nullptr);
    QWidget *mainWidget = parentWidget();
    while (mainWidget->parentWidget() != nullptr){
        mainWidget = mainWidget->parentWidget();
    }

    animationDelete = new QPropertyAnimation(this, "pos");
    animationDelete->setDuration(150);
    animationDelete->setStartValue(pos());
    animationDelete->setEndValue(QPoint(mainWidget->width(), y()));
    animationDelete->start();

    connect(animationDelete, &QPropertyAnimation::finished, [=]{
        deleteLater();
    });
}


void ToastWidget::setAction(const QString &actionTitle, const std::function<void ()> &doAction)
{
    action = doAction;
    btAction->setVisible(actionTitle != nullptr);
    btAction->setAccessibleName("active");
    btAction->setText(actionTitle);
    connect(btAction, &QPushButton::clicked, [=](){
        doAction();
        setupAnimationHide();
    });
}

void ToastWidget::setDuration(const Duration &value)
{
    duration = value;
}

ToastWidget *ToastWidget::info(QWidget *context, const QString &title, const QString &message, Duration duration)
{
    return showToast(context, Info, title, message, duration);
}

ToastWidget *ToastWidget::success(QWidget *context, const QString &title, const QString &message, Duration duration)
{
    return showToast(context, Success, title, message, duration);
}

ToastWidget *ToastWidget::warning(QWidget *context, const QString &title, const QString &message, Duration duration)
{
    return showToast(context, Warning, title, message, duration);
}

ToastWidget *ToastWidget::critical(QWidget *context, const QString &title, const QString &message, Duration duration)
{
    return showToast(context, Critical, title, message, duration);
}

ToastWidget *ToastWidget::showToast(QWidget *context, ToastWidget::Type type, const QString &title, const QString &message, Duration duration)
{
    if (context == nullptr){
        qWarning() << "Cannot toast on widget null";
        return nullptr;
    }

    // Tìm kiếm widget ngoài cùng (có parent = nullptr);
    QWidget *mainWidget = context;
    while (mainWidget->parentWidget() != nullptr){
        mainWidget = mainWidget->parentWidget();
    }

    auto toastWidget = new ToastWidget(mainWidget);
    toastWidget->setType(type);
    toastWidget->setContent(title, message);
    toastWidget->setFixedSize(FixedWidth, FixedHeight);
    toastWidget->setDuration(duration);
    toastWidget->setupAnimationShow();
    if (duration != Forever){
        toastWidget->setupDelayHide();
    }

    allToasts.append(toastWidget);
    connect(toastWidget, &ToastWidget::destroyed, [=](){
        allToasts.removeOne(toastWidget);

        int bottom = mainWidget->height() - FixedHeight - Padding;
        for (int i = 0; i < allToasts.size(); i++){
            auto toast = allToasts.at(i);
            toast->move(toast->x(), bottom - i * (FixedHeight + Spacing));
        }
    });
    return toastWidget;
}
