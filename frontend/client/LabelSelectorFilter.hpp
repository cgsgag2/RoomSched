#include <QObject>
#include <QEvent>
#include <QLabel>

class LabelSelectorFilter : public QObject {
protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::Show) {
            QLabel *label = qobject_cast<QLabel*>(obj);
            if (label) {
                label->setTextInteractionFlags(label->textInteractionFlags() | Qt::TextSelectableByMouse);
            }
        }
        return QObject::eventFilter(obj, event);
    }
};