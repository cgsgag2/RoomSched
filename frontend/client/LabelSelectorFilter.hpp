#ifndef LABEL_SELECTOR_FILTER_
#define LABEL_SELECTOR_FILTER_

#include <QEvent>
#include <QLabel>
#include <QObject>

namespace roomsched::client {

class LabelSelectorFilter : public QObject {
protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::Show) {
            QLabel *label = qobject_cast<QLabel *>(obj);
            if (label) {
                label->setTextInteractionFlags(
                    label->textInteractionFlags() | Qt::TextSelectableByMouse
                );
            }
        }
        return QObject::eventFilter(obj, event);
    }
};

}  // namespace roomsched::client

#endif  // LABEL_SELECTOR_FILTER_
