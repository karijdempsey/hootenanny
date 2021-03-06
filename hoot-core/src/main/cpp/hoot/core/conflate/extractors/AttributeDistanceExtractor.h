/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
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
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ATTRIBUTEDISTANCEEXTRACTOR_H
#define ATTRIBUTEDISTANCEEXTRACTOR_H

#include "WayFeatureExtractor.h"

namespace hoot
{

/**
 * See exporatory funds report for details.
 */
class AttributeDistanceExtractor : public WayFeatureExtractor
{
public:
  static std::string className() { return "hoot::AttributeDistanceExtractor"; }

  AttributeDistanceExtractor(ValueAggregator* wayAgg = 0, QString key = "") :
    WayFeatureExtractor(wayAgg), _key(key) {}

  virtual std::string getClassName() const { return className(); }

  virtual std::string getName() const;

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const;

  bool _useWeight;
  QString _key;
};

}

#endif // ATTRIBUTEDISTANCEEXTRACTOR_H
