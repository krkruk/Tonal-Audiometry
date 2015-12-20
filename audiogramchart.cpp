#include "audiogramchart.h"
#include <QDebug>
#include <QObject>
#include <QMatrix>

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
            path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, QString::number(freq));
        switch(i)   //a bit hackathon style :D!
        {
        case 4: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "1000"); break;
        case 6: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "2000"); break;
        case 8: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "4000"); break;
        case 10: path.drawText(rect, Qt::AlignCenter | Qt::AlignBottom, "8000"); break;
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

void AudiogramChart::paint(QPainter *painter)
{
    auto path = createChartGrid();
    createTextAxis(*painter);
    createTextLabel(*painter);

    painter->drawPath(path);
    painter->end();
}

