#include "audiogramchart.h"
#include "appengine.h"
#include <QDebug>
#include <QObject>
#include <QVector>


AudiogramChart::AudiogramChart(int width, int height)
    : AudiogramChart(QSize(width, height))
{}

AudiogramChart::AudiogramChart(const QSize &size)
    : chartSize(size)
{
    calculateStepsPx();
}

void AudiogramChart::saveJPG(const QString &noExtensionName)
{
    QImage img(chartSize, QImage::Format_RGB16);
    img.fill(Qt::white);

    QPainter p(&img);
    paint(&p);
    img.save(noExtensionName, "JPG");
}

QPixmap AudiogramChart::getPixmap()
{
    QPixmap pixmap(chartSize);
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    paint(&p);
    return pixmap;
}

void AudiogramChart::setDataEnabled(bool enable)
{
    dataPlotEnable = enable;
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
    path.drawText(freqLabel, Qt::AlignHCenter | Qt::AlignTop, QObject::tr("Frequency [Hz]"));

    //add a intensity label
    QMatrix mat;
    mat.translate(0, chartSize.height());
    mat.rotate(-90);
    path.save();
    path.setMatrix(mat);
    QRect intensLabel(0, 0, chartSize.height(), fontSizePx * 2);
    path.drawText(intensLabel, Qt::AlignHCenter | Qt::AlignTop, QObject::tr("Intensity [dB]"));
    path.restore();
}

void AudiogramChart::plot(QPainter *painter)
{
    QVector<QPoint> points;
    for(auto elem : *this)
        points.append(getCoords(
                          elem.getFrequency(),
                          elem.getVolumeDb()));
    painter->save();
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    painter->drawPolyline(points.data(), points.length());
    painter->restore();
}

void AudiogramChart::paint(QPainter *painter)
{
    auto path = createChartGrid();
    createTextAxis(*painter);
    painter->drawPath(path);
    createTextLabel(*painter);

    if(dataPlotEnable)
        plot(painter);


    painter->end();
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
    chart->update(engine->audiogramPlotData.getSortedData());
    chart->setDataEnabled(true);
    engine->audiogramPlotData.clear();
    return chart->getPixmap();
}

QPixmap AudiogramChartWidget::drawBlankChart(AudiogramChart *chart)
{
    chart->setDataEnabled(true);
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
