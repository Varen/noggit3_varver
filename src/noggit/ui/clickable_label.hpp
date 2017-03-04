// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <QtWidgets/QLabel>

namespace ui
{
  class clickable_label : public QLabel
  {
    Q_OBJECT

  public:
    using QLabel::QLabel;

  signals:
    void clicked();

  protected:
    void mouseReleaseEvent (QMouseEvent* event);
  };
}