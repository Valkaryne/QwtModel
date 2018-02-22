#ifndef POLARPLOT_HPP
#define POLARPLOT_HPP

#include <qwt_polar_plot.h>
#include <qwt_polar_curve.h>
#include <qwt_polar_grid.h>
#include <qwt_scale_div.h>
#include <qwt_symbol.h>

class PolarPlot : public QwtPolarPlot
{
    Q_OBJECT

public:
    PolarPlot(QWidget *parent = 0);

    QSize sizeHint() const { return QSize(500, 500); }
    QSize minimumSizeHint() const { return QSize(500, 500); }

private:
    QwtPolarCurve *phaseMarker;
};

#endif // POLARPLOT_HPP
