#ifndef SPECWATERFALL_HPP
#define SPECWATERFALL_HPP

#include <qwt_plot.h>
#include <qwt_plot_spectrocurve.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_widget.h>
#include <qwt_color_map.h>
#include <qwt_plot_marker.h>

constexpr double RANGE = 30.72;
constexpr double LSHIFT = 0.0075 * 2060;
constexpr double RSHIFT = RANGE - LSHIFT;

class SpecWaterfall : public QwtPlot
{
    Q_OBJECT

public:
    SpecWaterfall(QWidget *parent);

    /* Hints for resize QwtPlot */
    QSize sizeHint() const { return QSize(50, 50); }
    QSize minimumSizeHint() const { return QSize(50, 50); }

private:
    QwtPlotSpectrogram  *plot;
    QwtMatrixRasterData *plotData;
    QVector<double> *vec;

    double cntrFrequency;
};

#endif // SPECWATERFALL_HPP
