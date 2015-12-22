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


class AudiogramChart
{
    static constexpr int VERTICAL_LINES = 11;
    static constexpr int HORIZONTAL_LINES = 14;
    static constexpr float TEXT_TO_GRID_SPACING = 0.1;
    static constexpr int TEXT_TO_GRID_PX_DISTANCE = 10;
    static constexpr int PLOT_LINE_WIDTH = 4;

    QString intensityLabel;
    QString frequencyLabel;

    QSize chartSize;
    QSize gridSize;
    QPoint startDrawGridPoint;
    QPoint endDrawGridPoint;
    int horizontalStepPx{0};
    int verticalStepPx{0};

    QMap<int, int> intensityPxLocation;
    QMap<int, int> frequencyPxLocation;

    QList<AudiogramData> leftDataSorted;
    QList<AudiogramData> rightDataSorted;

    bool isLeftDataPresent {false};
    bool isRightDataPresent {false};

public:
    AudiogramChart(int width, int height);
    AudiogramChart(const QSize &size);

    bool saveImage(const QString &extensionName);
    QPixmap getPixmap();

    void setDataLeft(AudiogramPlotData &left);
    void setDataRight(AudiogramPlotData &right);

    void clearData();

    QString getIntensityLabel() const;
    void setIntensityLabel(const QString &value);

    QString getFrequencyLabel() const;
    void setFrequencyLabel(const QString &value);

private:
    void calculateStepsPx();
    QPainterPath createChartGrid();
    void createTextAxis(QPainter &path, int fontSizePx = 12);
    void createTextLabel(QPainter &path, int fontSizePx = 24);

    void __plot(QPainter *painter, const QPen &pen, const QList<AudiogramData> &data, char signum = '-');
    void plotLeft(QPainter *painter, const QPen &pen);
    void plotRight(QPainter *painter, const QPen &pen);

    void __paint(QPainter *painter);
    void paint(QPainter *painter);
    void paintLeft(QPainter *painter);
    void paintRight(QPainter *painter);

    QPoint getCoords(int frequency, int decibel);
};

class AudiogramChartWidget : public QQuickImageProvider
{
    using SSDir = SoundSample::Direction;

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
