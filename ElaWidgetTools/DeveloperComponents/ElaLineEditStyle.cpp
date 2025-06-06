#include "ElaLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ElaTheme.h"
ElaLineEditStyle::ElaLineEditStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaLineEditStyle::~ElaLineEditStyle()
{
}

void ElaLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_PanelLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            QRect lineEditRect = fopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            // 边框绘制
            painter->setPen(ElaThemeColor(_themeMode, BasicBorder));
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(lineEditRect.adjusted(1, 1, -1, -1), 6, 6);
            painter->setPen(Qt::NoPen);
            //  背景绘制
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(ElaThemeColor(_themeMode, DialogBase));
            }
            else if (fopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
            }
            else
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicBaseAlpha));
            }
            painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, lineEditRect.height());
            path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
            path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 225, 45);
            path.closeSubpath();
            painter->drawPath(path);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
