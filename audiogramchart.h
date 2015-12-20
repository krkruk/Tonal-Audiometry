#ifndef AUDIOGRAMCHART_H
#define AUDIOGRAMCHART_H
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QImage>
#include <QPixmap>
#include <QQuickImageProvider>
#include <QMap>
#include <QMatrix>
#include <QPoint>
#include "audiogram.h"

class AppEngine;


class AudiogramChart : public AudiogramPlotData
{
    static constexpr int VERTICAL_LINES = 11;
    static constexpr int HORIZONTAL_LINES = 14;
    static constexpr float TEXT_TO_GRID_SPACING = 0.1;
    static constexpr int TEXT_TO_GRID_PX_DISTANCE = 10;

    QSize chartSize;
    QSize gridSize;
    QPoint startDrawGridPoint;
    QPoint endDrawGridPoint;
    int horizontalStepPx{0};
    int verticalStepPx{0};

    bool dataPlotEnable{false};
    QMap<int, int> intensityPxLocation;
    QMap<int, int> frequencyPxLocation;

public:
    AudiogramChart(int width, int height);
    AudiogramChart(const QSize &size);

    void saveJPG(const QString &noExtensionName);
    QPixmap getPixmap();

    void setDataEnabled(bool enable);

private:
    void calculateStepsPx();
    QPainterPath createChartGrid();
    void createTextAxis(QPainter &path, int fontSizePx = 12);
    void createTextLabel(QPainter &path, int fontSizePx = 24);
    void plot(QPainter *painter);
    void paint(QPainter *painter);

    QPoint getCoords(int frequency, int decibel);
};

class AudiogramChartWidget : public QQuickImageProvider
{
    static constexpr int width = 640;
    static constexpr int height = 480;
    AppEngine *engine;

public:
    AudiogramChartWidget(AppEngine *engine);
    ~AudiogramChartWidget();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    QPixmap drawData(AudiogramChart *chart);
    QPixmap drawBlankChart(AudiogramChart *chart);
    QPixmap drawHelloWorld(QSize *size);
};

#endif // AUDIOGRAMCHART_H
