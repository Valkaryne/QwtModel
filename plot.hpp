#ifndef PLOT_HPP
#define PLOT_HPP

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_wheel.h>

constexpr double INCR = 0.0075;
constexpr double SHIFT = INCR * 2060;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget *parent);

    /* Hints for resize QwtPlot */
    QSize sizeHint() const { return QSize(50, 50); }
    QSize minimumSizeHint() const { return QSize(50, 50); }

};

#endif // PLOT_HPP