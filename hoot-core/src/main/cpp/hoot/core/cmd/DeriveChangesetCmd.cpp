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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/OsmChangesetXmlFileWriter.h>
#include <hoot/core/io/OsmChangesetSqlFileWriter.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>

using namespace std;

namespace hoot
{

class DeriveChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveChangesetCmd"; }

  DeriveChangesetCmd() { }

  ~DeriveChangesetCmd()
  {
    _hootApiDb.close();
  }

  virtual QString getName() const { return "derive-changeset"; }

  virtual int runSimple(QStringList args)
  {
    bool isXmlOutput = false;
    QString osmApiDbUrl = "";

    if (args.size() == 0)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 with takes three to four parameters.").arg(getName()));
    }
    else if (args[2].endsWith(".osc"))
    {
      isXmlOutput = true;
      if (args.size() != 3 && args.size() != 5)
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 with XML output takes three parameters.").arg(getName()));
      }
    }
    else if (args[2].endsWith(".osc.sql"))
    {
      if (args.size() != 4 && args.size() != 6)
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 with SQL output takes four or parameters.").arg(getName()));
      }
      osmApiDbUrl = args[3];
    }
    else
    {
      throw HootException("Unsupported changeset output format: " + args[2]);
    }

    const QString input1 = args[0];
    const QString input2 = args[1];
    const QString output = args[2];

    LOG_INFO(
      "Deriving changeset for inputs " << input1.right(50) << ", " << input2.right(50) <<
      " and writing output to " <<
      output.right(50) << "...");

    const double changesetBuffer = ConfigOptions().getChangesetBuffer();
    if (changesetBuffer > 0.0)
    {
      //allow for calculating the changeset with a slightly larger AOI than the default specified
      //bounding box

      QString bboxStr;
      QString convertBoundsParamName;
      //only one of these three should be specified
      if (!ConfigOptions().getConvertBoundingBox().isEmpty())
      {
        bboxStr = ConfigOptions().getConvertBoundingBox();
        convertBoundsParamName = ConfigOptions::getConvertBoundingBoxKey();
      }
      else if (!ConfigOptions().getConvertBoundingBoxHootApiDatabase().isEmpty())
      {
        bboxStr = ConfigOptions().getConvertBoundingBoxHootApiDatabase();
        convertBoundsParamName = ConfigOptions::getConvertBoundingBoxHootApiDatabaseKey();
      }
      else if (!ConfigOptions().getConvertBoundingBoxOsmApiDatabase().isEmpty())
      {
        bboxStr = ConfigOptions().getConvertBoundingBoxOsmApiDatabase();
        convertBoundsParamName = ConfigOptions::getConvertBoundingBoxOsmApiDatabaseKey();
      }
      else
      {
        throw HootException(
          "A changeset buffer was specified but no convert bounding box was specified.");
      }
      geos::geom::Envelope convertBounds = GeometryUtils::envelopeFromConfigString(bboxStr);
      convertBounds.expandBy(changesetBuffer, changesetBuffer);
      conf().set(
        convertBoundsParamName,
        GeometryUtils::envelopeToConfigString(convertBounds));
    }

    //some in these datasets may have status=3 if you're loading conflated data, so use
    //reader.use.file.status and reader.keep.file.status if you want to retain that value
    OsmMapPtr map1(new OsmMap());
    loadMap(map1, input1, true, Status::Unknown1);
    OsmMapPtr map2(new OsmMap());
    loadMap(map2, input2, true, Status::Unknown2);

    //we don't want to include review relations
    boost::shared_ptr<TagKeyCriterion> elementCriterion(
      new TagKeyCriterion(MetadataTags::HootReviewNeeds()));
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(false);
    map1->visitRw(removeElementsVisitor);
    map2->visitRw(removeElementsVisitor);

    //changeset derivation requires element sorting to work properly
    ElementSorterPtr sorted1(new ElementSorter(map1));
    ElementSorterPtr sorted2(new ElementSorter(map2));
    ChangesetDeriverPtr delta(new ChangesetDeriver(sorted1, sorted2));

    if (isXmlOutput)
    {
      OsmChangesetXmlFileWriter().write(output, delta);
    }
    else
    {
      assert(!osmApiDbUrl.isEmpty());
      LOG_DEBUG(osmApiDbUrl);
      OsmChangesetSqlFileWriter(QUrl(osmApiDbUrl)).write(output, delta);
    }

    return 0;
  }

private:

  HootApiDb _hootApiDb;

};

HOOT_FACTORY_REGISTER(Command, DeriveChangesetCmd)

}


