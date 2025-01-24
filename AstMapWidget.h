#ifndef CASTMAPWIDGET_H
#define CASTMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMap>

#include <AstS57Parser.h>

#define MIN_AXIS_SCALE            0.5
#define MAX_AXIS_SCALE            300.0

#define DOUBLECLICK_FACTOR        3
#define SCALE_CHANGE_MULTIPLIER   1.05

class CAstMapWidget : public QWidget
{
    Q_OBJECT

public:
    CAstMapWidget(void);
    CAstMapWidget(CAstS57Parser *pMapParser, QWidget *parent = nullptr);

    void SetMapParser(CAstS57Parser *pMapParser){m_pMapParser = pMapParser;}
    void SetOrigin(long lX, long lY){m_dXorigin = lX; m_dYorigin = lY;}

private:
    QWidget                      *m_pParent;
    bool                          m_bMapInit;

    QPainter                     *m_pPainter;
    CAstS57Parser                *m_pMapParser;

    double                        m_dXorigin;
    double                        m_dYorigin;

    double                        m_dYAxisScaleCorrection;

    QPoint                        m_CursorPosition;
    QPoint                        m_LButtonOriginCursorPosition;

    double                        m_dXAxisScale;
    double                        m_dYAxisScale;

    void Line(int from_x, int from_y, int to_x, int to_y);
    void Text(int x, int y, QString text);
    void Polygon(QPolygon polygon);

protected:
    void MapAreaInit(void);
    void ScaleChange(double dScaleCoeff);
    void CoordinateLimitsControl(void);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *);

signals:
    void ViewMousePosSignal(QPoint pos);
};

#endif // CASTMAPWIDGET_H
