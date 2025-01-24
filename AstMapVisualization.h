#ifndef ASTMAPVISUALIZATION_H
#define ASTMAPVISUALIZATION_H

#include <QWidget>

#include "AstMapWidget.h"
#include "AstS57Parser.h"

namespace Ui {
class CAstMapVisualization;
}

class CAstMapVisualization : public QWidget
{
    Q_OBJECT

public:
    explicit CAstMapVisualization(CAstS57Parser *pMapParser, QWidget *parent = nullptr);
    ~CAstMapVisualization();

private:
    Ui::CAstMapVisualization     *ui;

    CAstMapWidget                *m_pMapWidget;

public slots:

    void ViewMousePosSlot(QPoint pos);

};

#endif // ASTMAPVISUALIZATION_H
