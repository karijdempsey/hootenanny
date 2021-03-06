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
#ifndef KEEPHIGHWAYSVISITOR_H
#define KEEPHIGHWAYSVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Removes all ways and relations that are not part of a linear highway.
 * Note: You may have to run this multiple times to get the desired effect.
 */
class KeepHighwaysVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::KeepHighwaysVisitor"; }

  KeepHighwaysVisitor() {}

  virtual ~KeepHighwaysVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  /**
   * KeepHighwaysVisitor requires a read/write map.
   */
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  virtual void visit(const ConstElementPtr& e);

private:
  OsmMap* _map;
};

}

#endif // KEEPHIGHWAYSVISITOR_H
