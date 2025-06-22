#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <functional>

class QGridLayout;
class QLabel;
class QPushButton;
class QPropertyAnimation;
class ToastWidget : public QWidget {

public:
    /**
     * @brief The Type enum dạng của toast ảnh hưởng đến màu & icon
     */
    enum Type {
        Info,
        Success,
        Warning,
        Critical
    };
    /**
     * @brief The Duration enum mô tả thời gian hiển thị của toast
     */
    enum Duration {
        Short,    // 1 giây
        Medium,   // 2 giây
        Long,     // 5 giây
        VeryLong, // 10 giây
        Forever   // Cho đến khi nào người dùng nhấn chuột vào phía ngoài object
    };

    enum {
        FixedWidth = 500,
        FixedHeight = 120,
        Padding = 80,
        Spacing = 6
    };

    ToastWidget(QWidget *parent = nullptr);

    void setType(Type type);

    void setClosable(bool closable);

    void setContent(const QString &title, const QString &message);

    void setupAnimationShow();

    void setupDelayHide();

    void setupAnimationHide();

    void setDuration(const Duration &value);

    void setAction(const QString &actionTitle, const std::function<void ()> &value);
private:

public:
    static ToastWidget *info(QWidget *context, const QString &title, const QString &message, Duration duration = Duration::Medium);
    static ToastWidget *success(QWidget *context, const QString &title, const QString &message, Duration duration = Duration::Medium);
    static ToastWidget *warning(QWidget *context, const QString &title, const QString &message, Duration duration = Duration::Medium);
    static ToastWidget *critical(QWidget *context, const QString &title, const QString &message, Duration duration = Duration::Medium);
    static ToastWidget *showToast(QWidget *context, Type type, const QString &title, const QString &message, Duration duration = Duration::Medium);
private:
    QGridLayout *layout;
    QLabel *lbIcon;
    QLabel *lbTitle;
    QLabel *lbContent;
    QPushButton *btClose;
    QPushButton *btAction = nullptr;
    QPropertyAnimation *animationDelete = nullptr;
    Duration duration;

    std::function<void()> action;

    static QList<ToastWidget *> allToasts;
};


#endif // TOAST_H
