/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "circleitem.h"

#include <QPainter>
#include <QPen>
#include <QSGSimpleRectNode>

CircleItem::CircleItem(QQuickItem *parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml", parent} {
  setFlag(ItemHasContents, true);
  setAntialiasing(true);
}

void CircleItem::paint(QPainter *painter) {
  Q_UNUSED(painter)

  return;
  //  painter->save();
  //  const auto pen_width = 4.0;

  //  QPen pen(color(), pen_width);
  //  painter->setPen(pen);
  //  painter->setRenderHints(QPainter::Antialiasing, true);
  //  painter->drawEllipse(x() + pen_width / 2.0, y() + pen_width / 2.0,
  //                       width() - pen_width, height() - pen_width);
  //  painter->restore();
}

QSGNode *CircleItem::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *) {
  QSGGeometryNode *node = nullptr;
  QSGGeometry *geometry = nullptr;
  const int num_segments = 360;
  const int line_width = 3;

  if (!node) {
    node = new QSGGeometryNode();
    geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),
                               num_segments + 1);
    geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
    geometry->setLineWidth(line_width);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(QColor(255, 0, 0));

    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);
  } else {
    node = static_cast<QSGGeometryNode *>(old_node);
    geometry = node->geometry();
    geometry->allocate(num_segments + 1);
  }

  const qreal cx = width() / 2 + line_width / 2;
  const qreal cy = height() / 2 + line_width / 2;
  const qreal r = qMin(height() / 2, width() / 2) - line_width;

  for (int ii = 0; ii < num_segments + 1; ii++) {
    qreal theta =
        2.0 * M_PI * qreal(ii) / qreal(num_segments);  // get the current angle

    qreal x = r * cosf(theta);  // calculate the x component
    qreal y = r * sinf(theta);  // calculate the y component

    geometry->vertexDataAsPoint2D()[ii].set(x + cx, y + cy);
  }

  node->markDirty(QSGNode::DirtyGeometry);

  return node;
}

AbstractItem::EditableProperties CircleItem::editableProperties() const {
  return AbstractItem::editableProperties();
}
