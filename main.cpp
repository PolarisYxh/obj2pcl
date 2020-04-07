#include "osgqt.h"
#include "structures.h"
#include <QApplication>
#include <QTime>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/LightModel>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/Camera>
#include <iostream>
//#include <io.h>
#include <osg/Image>
#include <osgDB/WriteFile>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/TexEnvCombine>
#include <osgFX/Scribe>
#include <osgUtil/HighlightMapGenerator>
#include <iostream>
#include <fstream>
#include <vector>
#include <osgViewer/ViewerEventHandlers>
#include <QSplitter>
#include <QGridLayout>
#include <QPushButton>
#include <qtextcodec.h>
using namespace std;
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(image);

    QApplication a(argc, argv);
    osgqt w;
    w.setWindowFlags(w.windowFlags() &~ Qt::WindowMaximizeButtonHint);
    w.setFixedHeight(878);
    w.setFixedWidth(1385);
    w.setWindowTitle(QObject::tr("Shape2Motion Annotaiton Tool"));
    w.show();
    return a.exec();
}
