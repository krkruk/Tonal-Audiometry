#include "audiogramchart.h"
#include "appengine.h"
#include "constants.h"
#include <QDebug>
#include <QObject>
#include <QVector>


QString AudiogramChart::getIntensityLabel() const
{
    return intensityLabel;
}

void AudiogramChart::setIntensityLabel(const QString &value)
{
    intensityLabel = value;
}

QString AudiogramChart::getFrequencyLabel() const
{
    return frequencyLabel;
}

void AudiogramChart::setFrequencyLabel(const QString &value)
{
    frequencyLabel = value;
}

AudiogramChart::AudiogramChart(int width, int height)
    : AudiogramChart(QSize(width, height))
{}

AudiogramChart::AudiogramChart(const QSize &size)
    : intensityLabel(Consts::intensityLabel),
      frequencyLabel(Consts::frequencyLabel), chartSize(size)
{
    calculateStepsPx();
}

bool AudiogramChart::saveImage(const QString &extensionName)
{
    QImage img(chartSize, QImage::Format_RGB32);
    img.fill(Qt::white);

    QPainter p(&img);
    paint(&p);
    return img.save(extensionName);
}

QPixmap AudiogramChart::getPixmap()
{
    QPixmap pixmap(chartSize);
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    paint(&p);
    return pixmap;
}


void AudiogramChart::setDataLeft(AudiogramPlotData &left)
{
    leftDataSorted = left.getSortedData();
    if(!leftDataSorted.isEmpty())
        isLeftDataPresent = true;
}

void AudiogramChart::setDataRight(AudiogramPlotData &right)
{
    rightDataSorted = right.getSortedData();
    if(!rightDataSorted.isEmpty())
        isRightDataPresent = true;
}

void AudiogramChart::clearData()
{
    isRightDataPresent = false;
    isLeftDataPresent = false;
    rightDataSorted.clear();
    leftDataSorted.clear();
}

void AudiogramChart::calculateStepsPx()
{
    startDrawGridPoint.setX(chartSize.width() * TEXT_TO_GRID_SPACING);
    startDrawGridPoint.setY(chartSize.height() * TEXT_TO_GRID_SPACING);

    horizontalStepPx = (chartSize.width() - startDrawGridPoint.x())
                            / (VERTICAL_LINES - 1);

    verticalStepPx = (chartSize.height() - startDrawGridPoint.y())
                            / (HORIZONTAL_LINES - 1);

    endDrawGridPoint.setX(startDrawGridPoint.x() +
                          horizontalStepPx * (VERTICAL_LINES-1));
    endDrawGridPoint.setY(startDrawGridPoint.y() +
                          verticalStepPx * (HORIZONTAL_LINES-1));
}

QPainterPath AudiogramChart::createChartGrid()
{
    QPainterPath path;

    //columns
    for(auto col = startDrawGridPoint.x(); col <=endDrawGridPoint.x();
        col += horizontalStepPx)
    {
        path.moveTo(col, startDrawGridPoint.y());
        path.lineTo(col, endDrawGridPoint.y());
    }

    //rows
    for(auto row = startDrawGridPoint.y(); row <= endDrawGridPoint.y();
        row += verticalStepPx)
    {
        path.moveTo(startDrawGridPoint.x(), row);
        path.lineTo(endDrawGridPoint.x(), row);
    }

    return path;
}

void AudiogramChart::createTextAxis(QPainter &path, int fontSizePx)
{
    auto intensity = -10; const auto intensityStep = 10;
    const auto w = fontSizePx * 3; const auto h = fontSizePx * 2;
    const auto startDrawingCol = startDrawGridPoint.x() - w - TEXT_TO_GRID_PX_DISTANCE;
    const auto startDrawingRow = startDrawGridPoint.y() - h - TEXT_TO_GRID_PX_DISTANCE * 0.5;

    //set up font
    auto font = path.font();
    font.setPixelSize(fontSizePx);
    path.setFont(font);

    //draw intensity in dB column left
    for(auto row = startDrawGridPoint.y(); row <= endDrawGridPoint.y();
        row += verticalStepPx)
    {
        QRect rect(startDrawingCol, row - fontSizePx, w, h);
        intensityPxLocation.insert(intensity, row);
        path.drawText(rect, Qt::AlignRight | Qt::AlignTop, QString::number(intensity));
        intensity += intensityStep;
    }

    //draw frequency in Hz
    auto freq = 125; const auto freqMultiplier = 2; int i = 0;
    for(auto col = startDrawGridPoint.x(); col <=endDrawGridPoint.x();
        col += horizontalStepPx)
    {
        QRect rect(col - h, startDrawingRow, w, h);

        if(freq <= 500 && i < 3)
        {
            path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, QString::number(freq));
            frequencyPxLocation.insert(freq, col);
        }
        switch(i)   //a bit hackathon style :D!
        {
        case 4: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "1000");
            frequencyPxLocation.insert(1000, col);break;
        case 6: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "2000");
            frequencyPxLocation.insert(2000, col);break;
        case 8: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "4000");
            frequencyPxLocation.insert(4000, col);break;
        case 10: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "8000");
            frequencyPxLocation.insert(8000, col);break;
        default: break;
        }

        ++i; freq *= freqMultiplier;
    }
}

void AudiogramChart::createTextLabel(QPainter &path, int fontSizePx)
{
    //draw a frequency label
    auto font = path.font();
    font.setPixelSize(fontSizePx);
    path.setFont(font);
    QRect freqLabel(0, 0, chartSize.width(), fontSizePx * 2);
    path.drawText(freqLabel, Qt::AlignHCenter | Qt::AlignTop, frequencyLabel);

    //add a intensity label
    QMatrix mat;
    mat.translate(0, chartSize.height());
    mat.rotate(-90);
    path.save();
    path.setMatrix(mat);
    QRect intensLabel(0, 0, chartSize.height(), fontSizePx * 2);
    path.drawText(intensLabel, Qt::AlignHCenter | Qt::AlignTop, intensityLabel);
    path.restore();
}

void AudiogramChart::__plot(QPainter *painter, const QPen &pen, const QList<AudiogramData> &data, char signum)
{
    QVector<QPoint> points;
    for(auto elem : data)
        points.append(getCoords(
                          elem.getFrequency(),
                          elem.getVolumeDb()));
    painter->save();
    painter->setPen(pen);
    painter->drawPolyline(points.data(), points.length());
    if(signum == 'o')
    {
        for(auto elem : points)
            painter->drawEllipse(elem, PLOT_LINE_WIDTH, PLOT_LINE_WIDTH);
    }
    if(signum == 's')
    {
        for(auto elem : points)
        {

            painter->drawRect(elem.x() - PLOT_LINE_WIDTH,
                              elem.y() - PLOT_LINE_WIDTH,
                              PLOT_LINE_WIDTH*2, PLOT_LINE_WIDTH*2);
        }
    }
    painter->restore();
}

void AudiogramChart::plotLeft(QPainter *painter, const QPen &pen)
{
    __plot(painter, pen, leftDataSorted, 'o');
}

void AudiogramChart::plotRight(QPainter *painter, const QPen &pen)
{
    __plot(painter, pen, rightDataSorted, 's');
}

void AudiogramChart::__paint(QPainter *painter)
{
    auto path = createChartGrid();
    createTextAxis(*painter);
    painter->drawPath(path);
    createTextLabel(*painter);
}

void AudiogramChart::paint(QPainter *painter)
{
    __paint(painter);

    if(isLeftDataPresent)
        paintLeft(painter);
    if(isRightDataPresent)
        paintRight(painter);

    painter->end();
}

void AudiogramChart::paintLeft(QPainter *painter)
{
    QPen pen;
    pen.setWidth(PLOT_LINE_WIDTH);
    pen.setColor(Qt::blue);
    plotLeft(painter, pen);
}

void AudiogramChart::paintRight(QPainter *painter)
{
    QPen pen;
    pen.setWidth(PLOT_LINE_WIDTH);
    pen.setColor(Qt::red);
    plotRight(painter, pen);
}

QPoint AudiogramChart::getCoords(int frequency, int decibel)
{
    QPoint point(
                frequencyPxLocation.value(frequency, 0),
                intensityPxLocation.value(decibel, 0));
    return point;
}


AudiogramChartWidget::AudiogramChartWidget(AppEngine *engine)
    : QQuickImageProvider(QQuickImageProvider::Pixmap),
      engine(engine)
{

}

AudiogramChartWidget::~AudiogramChartWidget()
{

}

QPixmap AudiogramChartWidget::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (size)
        *size = QSize(width, height);
    if(requestedSize.width() > 0 && requestedSize.height() > 0)
        *size = requestedSize;

    AudiogramChart chart(*size);
    QPixmap pixmap;
    if(id == "data")
        pixmap = drawData(&chart);
    else
    {
        if(id == "blank")
            pixmap = drawBlankChart(&chart);
        else
            pixmap = drawHelloWorld(size);
    }

    return pixmap;
}

QPixmap AudiogramChartWidget::drawData(AudiogramChart *chart)
{
    chart->setDataRight(engine->audiogramPlotDataRight);
    chart->setDataLeft(engine->audiogramPlotDataLeft);

    return chart->getPixmap();
}

QPixmap AudiogramChartWidget::drawBlankChart(AudiogramChart *chart)
{
    chart->clearData();
    return chart->getPixmap();
}

QPixmap AudiogramChartWidget::drawHelloWorld(QSize *size)
{
    QPixmap pixmap(*size);
    pixmap.fill(Qt::black);
    QPainter painter(&pixmap);
    QPen pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    auto font = painter.font();
    font.setPixelSize(20);
    font.setCapitalization(QFont::MixedCase);
    painter.setFont(font);
    QRect rect(0, 0, size->width(), size->height());
    painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, "Hello world :D!");
    return pixmap;
}
