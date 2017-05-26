#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osg/Geometry>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <string>
#include <osgWidget/Util>
#include <osgWidget/WindowManager>
#include <osgWidget/Table>
#include <osgWidget/Box>
#include <osgWidget/Canvas>
#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>
#include <iostream>
#include <sstream>
#include <osg/Billboard>
#include <unistd.h>
#include "client.h"

using namespace osg;
using namespace osgGA;
using namespace std;

osg::MatrixTransform *joint0;
osg::MatrixTransform *joint1;
osg::MatrixTransform *joint2;
osg::MatrixTransform *joint3;
osg::MatrixTransform *joint4;
osg::MatrixTransform *joint5;
osg::MatrixTransform *joint6;
osg::MatrixTransform *joint7;
osg::MatrixTransform *joint8;
osg::MatrixTransform *joint9;
osg::MatrixTransform *joint10;
osg::MatrixTransform *joint11;
osg::MatrixTransform *joint12;
osg::MatrixTransform *joint13;
osg::MatrixTransform *joint14;
osg::MatrixTransform *joint15;
osg::MatrixTransform *joint16;
osg::MatrixTransform *joint17;
osg::MatrixTransform *joint18;
osg::MatrixTransform *joint19;
osg::MatrixTransform *joint20;
osg::MatrixTransform *joint21;
osg::MatrixTransform *joint22;
osg::MatrixTransform *joint23;
osg::MatrixTransform *joint24;

osg::MatrixTransform *buildJoint0(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint4(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint5(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint6(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint7(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint8(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint9(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint10(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint11(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint12(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint13(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint14(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint15(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint16(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint17(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint18(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint19(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint20(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint21(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint22(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint23(osg::MatrixTransform *previousJoint);
osg::MatrixTransform *buildJoint24(osg::MatrixTransform *previousJoint);

osg::MatrixTransform *buildEndEffector();

osg::TessellationHints *hints = new osg::TessellationHints;
osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");

float jointAngle1;
float jointAngle2;
float EndEffector;

bool startGame = false;
int positionX = 0;
int positionY = 0;
int rotationText = 0;
int xVal = 0, yVal = 450;

float scale, posX, posY, transX, transY, ratio;
uint32_t x, y, size, rotation;
int stepCount = 0;

float setPointX;
float setPointY;
osg::ref_ptr<osgText::Text> frameCountText = new osgText::Text;
osg::ref_ptr<osgText::Text> goalText = new osgText::Text;
osgWidget::Window* stickMan ;


osgWidget::Label* createLabel( const std::string& name,
                               const std::string& text, float size,
                               const osg::Vec4& color )
{
    osg::ref_ptr<osgWidget::Label> label =
            new osgWidget::Label(name);
    label->setLabel( text );
    label->setFont( "fonts/arial.ttf" );
    label->setFontSize( size );
    label->setFontColor( 1.0f, 1.0f, 1.0f, 1.0f );
    label->setColor( color );
    label->addSize( 0.0f, 25.0f );
    //label->setCanFill( true );
    return label.release();
}

bool tabPressed( osgWidget::Event& ev )
{
    osgWidget::Label* content = static_cast<
            osgWidget::Label*>( ev.getData() );
    if ( !content ) return false;
    osgWidget::Canvas* canvas = dynamic_cast<
            osgWidget::Canvas*>( content->getParent() );
    if ( canvas )
    {
        osgWidget::Canvas::Vector& objs = canvas->getObjects();
        for( unsigned int i=0; i<objs.size(); ++i )
            objs[i]->setLayer( osgWidget::Widget::LAYER_MIDDLE, i );
        content->setLayer( osgWidget::Widget::LAYER_TOP, 0 );
        canvas->resize();
    }
    cout<<content->getName();
    if(content->getName() == "Get Ip"){
        //setPointActive = false;
        //changeGame1 = true;
        //setPointX = 0;
        //setPointY = 0;
    }
    if(content->getName() == "StartSearch"){
        startGame = true;
    }

    return true;
}

osgWidget::Window* createSimpleTabs( float winX, float winY )
{
    osg::ref_ptr<osgWidget::Canvas> contents =
            new osgWidget::Canvas("contents");
    osg::ref_ptr<osgWidget::Box> tabs =
            new osgWidget::Box("tabs", osgWidget::Box::HORIZONTAL);

    string button1 = "StartSearch";
    string button2 = "Get Ip";
    //tabs->setScale(0.8f);

    osg::Vec4 color1(0.0f, (float)0 / 3.0f, 0.0f, 1.0f);
    std::stringstream ss, ss2;
    ss << "Button" ;
    ss2 << "Detected action:" <<" Button " << " is pressed" ;
    osgWidget::Label* content1 = createLabel(button1,
                                             ss2.str(), 11.0f, color1);
    content1->setLayer( osgWidget::Widget::LAYER_LOW, 0 );
    content1->setSize(480.0f,80.0f);
    contents->addWidget( content1, 0.0f, 0.0f );
    osgWidget::Label* tab1 = createLabel(button1,
                                         button1, 17.0f, color1);
    tab1->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
    tab1->addCallback( new osgWidget::Callback(
            &tabPressed, osgWidget::EVENT_MOUSE_PUSH, content1) );
    tab1->setLayer( osgWidget::Widget::LAYER_TOP, 1 );
    tabs->addWidget( tab1);

    osg::Vec4 color(0.0f, (float)1 / 3.0f, 0.0f, 1.0f);

    osgWidget::Label* content2 = createLabel(button2,
                                             ss2.str(), 11.0f, color);
    content2->setLayer( osgWidget::Widget::LAYER_LOW, 0);
    content2->setSize(480.0f,78.0f);
    contents->addWidget( content2, 0.0f, 0.0f );
    osgWidget::Label* tab2 = createLabel(button2,
                                         button2, 17.0f, color);
    tab2->setEventMask( osgWidget::EVENT_MOUSE_PUSH );
    tab2->addCallback( new osgWidget::Callback(
            &tabPressed, osgWidget::EVENT_MOUSE_PUSH, content2) );
    tab2->setLayer( osgWidget::Widget::LAYER_TOP, 1 );
    tabs->addWidget( tab2 );
    tab1->setCanFill(true);
    tab2->setCanFill(true);

    osg::ref_ptr<osgWidget::Box> main =
            new osgWidget::Box("main", osgWidget::Box::VERTICAL);
    main->setOrigin( winX, winY );
    main->attachMoveCallback();
    main->addWidget( tabs->embed() );
    main->addWidget( createLabel("title", "Press any button blow>",
                                 17.0f, osg::Vec4(0.0f, 0.4f, 1.0f, 1.0f)) );
    main->setScale(0.7);
    return main.release();
}


void addTexture(osg::ShapeDrawable* shape, string file){
    // create a simple material
    Material *material = new Material();
    material->setEmission(Material::FRONT, Vec4(0.8, 0.8, 0.8, 1.0));

    Image *image = osgDB::readImageFile(file);
    if (!image) {
        std::cout << "Couldn't load texture." << std::endl;
        return ;
    }
    osg::Texture2D *texture = new Texture2D;
    texture->setDataVariance(Object::DYNAMIC);
    texture->setFilter(Texture::MIN_FILTER, Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(Texture::MAG_FILTER, Texture::LINEAR);
    texture->setWrap(Texture::WRAP_S, Texture::CLAMP);
    texture->setWrap(Texture::WRAP_T, Texture::CLAMP);
    texture->setImage(image);

    StateSet *sphereStateSet = shape->getOrCreateStateSet();
    sphereStateSet->ref();
    sphereStateSet->setAttribute(material);
    sphereStateSet->setTextureAttributeAndModes(0, texture, StateAttribute::ON);
}

void addTexture(osg::Geode* shape, string file){
    // create a simple material
    Material *material = new Material();
    material->setEmission(Material::FRONT, Vec4(0.8, 0.8, 0.8, 1.0));

    Image *image = osgDB::readImageFile(file);
    if (!image) {
        std::cout << "Couldn't load texture." << std::endl;
        return ;
    }
    osg::Texture2D *texture = new Texture2D;
    texture->setDataVariance(Object::DYNAMIC);
    texture->setFilter(Texture::MIN_FILTER, Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(Texture::MAG_FILTER, Texture::LINEAR);
    texture->setWrap(Texture::WRAP_S, Texture::CLAMP);
    texture->setWrap(Texture::WRAP_T, Texture::CLAMP);
    texture->setImage(image);

    StateSet *sphereStateSet = shape->getOrCreateStateSet();
    sphereStateSet->ref();
    sphereStateSet->setAttribute(material);
    sphereStateSet->setTextureAttributeAndModes(0, texture, StateAttribute::ON);
}

osgText::Text* createTextColored( const osg::Vec3& pos,osg::Vec4& color,
                                  const std::string& content,
                                  float size )
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setFont( g_font.get() );
    text->setCharacterSize( size );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( pos );
    text->setText( content );
    text->setColor(color);
    return text.release();
}
/************************ Chart ***********************/
osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
{
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setFont( g_font.get() );
    text->setCharacterSize( size );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( pos );
    text->setText( content );
    text->setColor(osg::Vec4(0.5f, 0.9f, 0.1f, 1.0f));
    return text.release();
}

class GeometryUpdateCallback : public osg::Geometry::UpdateCallback {
public:

    GeometryUpdateCallback(int _cornerX, int _cornerY, int _x, int _y, char axis){
        x = _x;
        y = _y;
        cornerX = _cornerX;
        cornerY = _cornerY;
        this->axis = axis;

        vertices = new osg::Vec3Array;
        for(int i=0; i<x; i++) {
            vertices->push_back(osg::Vec3(cornerX+1+i, cornerY, 0));
        }

    }

    virtual void update(osg::NodeVisitor *, osg::Drawable *drawable) {
        /*osg::Vec3 vecA;
        osg::Vec3 vecB;


        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        vertices->erase(vertices->begin());
        for(int i=0; i<vertices->size()-1; i++) {
            vertices->at(i).set(cornerX+i, vertices->at(i+1).y(), 0);
        }
        if(axis == 'x')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));
        else if(axis == 'y')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));


        geometry->setVertexArray(vertices);
        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP  , 0, vertices->size()));*/

        osg::Vec3 vecA;
        osg::Vec3 vecB;

        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        vertices->erase(vertices->begin());
        for (int i = 0; i < vertices->size() - 1; i++) {
            vertices->at(i).set(cornerX + i, vertices->at(i + 1).y(), 0);
        }
        if (axis == 'x') {
            //
            if (xVal * (y / 400.0f) > y)
                vertices->push_back(osg::Vec3(cornerX + x, cornerY + y, 0));
            else if (xVal * (y / 400.0f) < 0)
                vertices->push_back(osg::Vec3(cornerX + x, cornerY, 0));
                //
            else
                vertices->push_back(osg::Vec3(cornerX + x, cornerY + xVal * (y / 400.0f), 0));
        } else if (axis == 'y')
            //
            if (xVal * (y / 300.0f) > y)
                vertices->push_back(osg::Vec3(cornerX + x, cornerY + y, 0));
            else if (xVal * (y / 300.0f) < 0)
                vertices->push_back(osg::Vec3(cornerX + x, cornerY, 0));
                //
            else
                vertices->push_back(osg::Vec3(cornerX + x, cornerY + yVal * (y / 300.0f), 0));

        geometry->setVertexArray(vertices);
        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, vertices->size()));

    }

protected:
    int x;
    int y;
    int cornerX;
    int cornerY;
    char axis;
    osg::Vec3Array *vertices;

};

osg::Geometry *makeChart(int _cornerX, int _cornerY, int _x, int _y, char axis){

    osg::Geometry *linesGeom = new osg::Geometry();// is my geometry
    osg::DrawArrays *drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);

    linesGeom->addPrimitiveSet(drawArrayLines);
    osg::Vec3Array *vertexData = new osg::Vec3Array;
    linesGeom->setVertexArray(vertexData);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.9f, 0.0f, 0.0f, 1.0f) );
    linesGeom->setColorArray( colors.get() );
    linesGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    linesGeom->setUseDisplayList( false );
    linesGeom->setUseVertexBufferObjects( false );

    linesGeom->setUpdateCallback( new GeometryUpdateCallback(_cornerX, _cornerY, _x, _y, axis) );

    return linesGeom;

}

osg::Geometry *makeChartPano(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int emptyEdge = 30;
    osg::Vec3Array *chartCorner = new osg::Vec3Array;
    chartCorner->push_back(osg::Vec3(_cornerX - emptyEdge, _cornerY- emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x + emptyEdge, _cornerY - emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x + emptyEdge, _cornerY + _y + emptyEdge, 0));
    chartCorner->push_back(osg::Vec3(_cornerX - emptyEdge,_cornerY + _y + emptyEdge, 0));

    chartGeom->setVertexArray(chartCorner);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.2f));
    chartGeom->setColorArray(colors);
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);


    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}
osg::Geometry *makePano(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int emptyEdge = 0;
    osg::Vec3Array *chartCorner = new osg::Vec3Array;
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY + _y, 0));
    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.6f));
    chartGeom->setColorArray(colors);
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);


    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}

osg::Geometry *makePano(int _cornerX, int _cornerY, int _x, int _y, osg::Vec4 color){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int emptyEdge = 0;
    osg::Vec3Array *chartCorner = new osg::Vec3Array;
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY + _y, 0));
    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(color);
    chartGeom->setColorArray(colors);
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);


    osg::StateSet* stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;
}

void writeGoalText(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY) {

    goalText->setFont(g_font.get());
    goalText->setCharacterSize(30.0f);
    goalText->setAxisAlignment(osgText::TextBase::XY_PLANE);
    goalText->setPosition(osg::Vec3(_cornerX, _cornerY, 0));
    goalText->setText(" ");
    goalText->setColor(osg::Vec4(0.5f, 0.9f, 0.1f, 1.0f));
    goalText->setName("goalText");
    goalText->setDataVariance(osg::Object::DYNAMIC);
    geode->addDrawable(goalText);

}


void writeScoreTextTable(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY) {

    frameCountText->setFont(g_font.get());
    frameCountText->setCharacterSize(18.0f);
    frameCountText->setAxisAlignment(osgText::TextBase::XY_PLANE);
    frameCountText->setPosition(osg::Vec3(_cornerX+60, _cornerY+40, 0));

    string posX;//string which will contain the result
    string posY;
    string rot;
    stringstream convertPosX; // stringstream used for the conversion
    stringstream convertPosY;
    stringstream rotation;

    convertPosX << positionX;//add the value of Number to the characters in the stream
    convertPosY << positionY;
    rotation << rotationText;

    posX = convertPosX.str();//set Result to the content of the stream
    posY = convertPosY.str();
    rot =  rotation.str();

    frameCountText->setText(posX +"\n"+posY+"\n"+rot);

    frameCountText->setColor(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));
    frameCountText->setName("frameCount");
    frameCountText->setDataVariance(osg::Object::DYNAMIC);
    geode->addDrawable(frameCountText);

    osg::Vec4Array *colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 0.2f));
    osg::Geometry *tableGeom = makeChartPano(_cornerX, _cornerY, 130, 50);
    //tableGeom->setColorArray(colors);

    geode->addDrawable(tableGeom);
}

void setPositions() {
    string posX;//string which will contain the result
    string posY;
    string rot;
    stringstream convertPosX; // stringstream used for the conversion
    stringstream convertPosY;
    stringstream rotation;

    convertPosX << positionX;//add the value of Number to the characters in the stream
    convertPosY << positionY;
    rotation << rotationText;

    posX = convertPosX.str();//set Result to the content of the stream
    posY = convertPosY.str();
    rot =  rotation.str();

    frameCountText->setText(posX +"\n"+posY+"\n"+rot);
}

osg::Geometry *makePanoLines(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry


    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX+40*0.8f,_cornerY + _y - 30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX+40*0.8f, _cornerY +  30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x - 40*0.8f, _cornerY + 30*0.8f, 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x - 40*0.8f, _cornerY + _y - 30*0.8f, 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return chartGeom;

}

osg::Geometry *makeLines(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry


    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 3));

    return chartGeom;
}



class GeoSetLinesUpdateCallback : public osg::Geometry::UpdateCallback {
public:
    GeoSetLinesUpdateCallback(int _cornerX, int _cornerY, int _x, char _axis ){
        x = _x;
        cornerX = _cornerX;
        cornerY = _cornerY;
        axis = _axis;

    }

    virtual void update(osg::NodeVisitor *, osg::Drawable *drawable) {

        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        osg::Vec3Array *chartCorner = new osg::Vec3Array;

        if(axis == 'x') {
            chartCorner->push_back(osg::Vec3(cornerX, cornerY + setPointX/4.0f, 0));
            chartCorner->push_back(osg::Vec3(cornerX + x, cornerY + setPointX/4.0f, 0));
        }
        else if(axis == 'y'){
            chartCorner->push_back(osg::Vec3(cornerX, cornerY + setPointY/4.0f, 0));
            chartCorner->push_back(osg::Vec3(cornerX + x, cornerY + setPointY/4.0f, 0));
        }

        geometry->setVertexArray(chartCorner);

        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
        colors->push_back( osg::Vec4(0.0f, 0.0f, 0.5f, 0.0f) );
        geometry->setColorArray( colors.get() );
        geometry->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

        geometry->setUseDisplayList( false );
        geometry->setUseVertexBufferObjects( false );

        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, 2));

    }

protected:
    int x ;
    int cornerX;
    int cornerY;
    char axis;
    osg::Vec3Array * vertices;

};

osg::Geometry *makeSetPointLine(int _cornerX, int _cornerY, int _x, char axis){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry

    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX + _x,_cornerY , 0));

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.5f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, 2));
    chartGeom->setUpdateCallback( new GeoSetLinesUpdateCallback(_cornerX, _cornerY, _x, axis) );

    return chartGeom;

}


osg::Geometry *makeGuideLines(int _cornerX, int _cornerY, int _x, int _y, char axes) {

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry
    int emptyBetween;

    if (axes == 'y')
        emptyBetween = _y / 6;
    else if (axes == 'x')
        emptyBetween = _y / 8;
    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    for (int i = 0; i < 8; i++) {
        chartCorner->push_back(osg::Vec3(_cornerX, _cornerY + (i + 1) * emptyBetween, 0));
        chartCorner->push_back(osg::Vec3(_cornerX + line + _x, _cornerY + (i + 1) * emptyBetween, 0));
    }

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.7f, 0.7f, 0.7f, 0.1f));
    chartGeom->setColorArray(colors.get());
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

    chartGeom->setUseDisplayList(false);
    chartGeom->setUseVertexBufferObjects(false);

    if (axes == 'y')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 12));
    else if (axes == 'x')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 16));

    osg::StateSet *stateset = chartGeom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);


    return chartGeom;

}

osg::Geometry *makeShortLines(int _cornerX, int _cornerY, int _x, int _y, char axis) {

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry
    float emptyBetween;

    if (axis == 'y')
        emptyBetween = _y / 6.0f;
    else if (axis == 'x')
        emptyBetween = _y / 8.0f;
    int line = 5;

    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    for (int i = 0; i < 8; i++) {
        chartCorner->push_back(osg::Vec3(_cornerX - line, _cornerY + (i + 1) * emptyBetween - 1, 0));
        chartCorner->push_back(osg::Vec3(_cornerX, _cornerY + (i + 1) * emptyBetween - 1, 0));
    }

    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
    chartGeom->setColorArray(colors.get());
    chartGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

    chartGeom->setUseDisplayList(false);
    chartGeom->setUseVertexBufferObjects(false);

    if (axis == 'y')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 12));
    else if (axis == 'x')
        chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 16));


    return chartGeom;

}
osg::Geometry *makeVerticalLine(int _cornerX, int _cornerY, int _x, int _y){

    osg::Geometry *chartGeom = new osg::Geometry();// is my geometry


    osg::Vec3Array *chartCorner = new osg::Vec3Array;

    chartCorner->push_back(osg::Vec3(_cornerX +_x , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX , _cornerY , 0));
    chartCorner->push_back(osg::Vec3(_cornerX ,_cornerY + _y , 0));



    chartGeom->setVertexArray(chartCorner);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
    chartGeom->setColorArray( colors.get() );
    chartGeom->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );

    chartGeom->setUseDisplayList( false );
    chartGeom->setUseVertexBufferObjects( false );

    chartGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 3));

    return chartGeom;
}


void makeAllTextYChart(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY, int _y){

    int emptyEdge = 30;

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - emptyEdge + 5, _cornerY + _y + 10, 0),
            "Y-Coordinate of Camera",
            15.0f)
    );

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 20 , _cornerY-3, 0),
            "0",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + _y/6-3, 0),
            "100",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 2*_y/6-3, 0),
            "200",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 3*_y/6-3, 0),
            "300",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 4*_y/6-3, 0),
            "400",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 5*_y/6-3, 0),
            "500",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 6*_y/6-3, 0),
            "600",
            10.0f)
    );
}
void makeAllTextXChart(osg::ref_ptr<osg::Geode> geode, int _cornerX, int _cornerY, int _y){

    int emptyEdge = 30;

    geode->addDrawable( createText(
            osg::Vec3(_cornerX - emptyEdge + 5, _cornerY + _y + 10, 0),
            "X-Coordinate of Camera",
            15.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 20 , _cornerY-3, 0),
            "0",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + _y/8-3, 0),
            "100",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 2*_y/8-3, 0),
            "200",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 3*_y/8-3, 0),
            "300",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 4*_y/8-3, 0),
            "400",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 5*_y/8-3, 0),
            "500",
            10.0f)
    );
    geode->addDrawable( createText(
            osg::Vec3(_cornerX - 25 , _cornerY + 6*_y/8-3, 0),
            "600",
            10.0f)
    );
    geode->addDrawable(createText(
            osg::Vec3(_cornerX - 25, _cornerY + 7 * _y / 8 - 3, 0),
            "700",
            10.0f)
    );
    geode->addDrawable(createText(
            osg::Vec3(_cornerX - 25, _cornerY + 8 * _y / 8 - 3, 0),
            "800",
            10.0f)
    );
}

void drawCharts(osg::ref_ptr<osg::Geode> geode,float x, float y){

    geode->addDrawable(makeChart(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeLines(x, y+150, 250, 100));
    geode->addDrawable(makeShortLines(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeGuideLines(x, y+150, 250, 100, 'x'));
    geode->addDrawable(makeSetPointLine(x, y+150, 250, 'x'));
    geode->addDrawable(makeChartPano(x, y+150, 250, 100));

    geode->addDrawable(makeChart(x, y, 250, 50, 'y'));
    geode->addDrawable(makeLines(x, y, 250, 75));
    geode->addDrawable(makeShortLines(x, y, 250, 75, 'y'));
    geode->addDrawable(makeGuideLines(x, y, 250, 75, 'y'));
    geode->addDrawable(makeSetPointLine(x, y, 250, 'y'));
    geode->addDrawable(makeChartPano(x, y, 250, 75));


}

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:

    KeyboardEventHandler() {}

    static void translate(float x, float y, float z, osg::MatrixTransform *joint) {
        osg::Matrix trans;
        trans.makeTranslate(x, y, z);
        joint->setMatrix(trans * joint->getMatrix());
        //joint->setMatrix( trans);
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
    {
        switch (ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                switch (ea.getKey())
                {
                    case 'd':
                        translate(-0.2, 0.0, 0.0,joint9);
                        return true;
                    case 'a':
                        translate(0.2, 0.0, 0.0,joint9);
                        return true;
                    case 'x':
                        translate(-0.2, 0.0, 0.0,joint2);
                        return true;
                    case 'w':
                        translate(0.2, 0.0, 0.0,joint2);
                        return true;
                }
            }
            default:
                break;

        }
        //return false to allow mouse manipulation
        return false;
    }
};

osg::Group *createShapes() {

    osg::Group *group = new osg::Group();
    osg::MatrixTransform *transform = new osg::MatrixTransform();
    group->addChild(transform);

    joint0 = buildJoint0(transform);
    joint1 = buildJoint1(joint0);
    joint2 = buildJoint2(joint0);
    joint3 = buildJoint3(joint0);
    joint4 = buildJoint4(joint0);
    joint5 = buildJoint5(joint0);
    joint6 = buildJoint6(joint3);
    joint7 = buildJoint7(joint3);
    joint8 = buildJoint8(joint2);
    joint10 = buildJoint10(joint8);
    joint9 = buildJoint9(joint10);
    joint11 = buildJoint11(joint9);
    joint12 = buildJoint12(joint1);
    joint13 = buildJoint13(joint1);
    joint14 = buildJoint14(joint1);
    joint15 = buildJoint15(joint1);
    joint16 = buildJoint16(joint1);
    joint17 = buildJoint17(joint1);
    joint18 = buildJoint18(joint10);
    joint19 = buildJoint19(joint10);
    joint20 = buildJoint20(joint10);
    joint21 = buildJoint21(joint11);
    joint22 = buildJoint22(joint21);
    joint23 = buildJoint23(joint22);
    joint24 = buildJoint24(joint23);
    //joint2->addChild(buildEndEffector());

    //joint2->setNodeMask(0x0);

    return group;
}

osg::MatrixTransform *buildJoint0(osg::MatrixTransform *previousJoint)
{
    double length = 20;
    double width = 1;
    double hight = 1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.0f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "profile2.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint1(osg::MatrixTransform *previousJoint)
{
    double length = 20;
    double width = 1;
    double hight = 1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -9.5f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "profile2.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint2(osg::MatrixTransform *previousJoint)
{
    double length = 20;
    double width = 1;
    double hight = 1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 9.5f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "profile2.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint3(osg::MatrixTransform *previousJoint)
{
    double length = 20;
    double width = 1;
    double hight = 1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 10.0f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "profile2.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint4(osg::MatrixTransform *previousJoint)
{
    double length = 1.5;
    double width = 0.1;
    double hight = 2;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(8.0f, -10.5f, -1.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "glass02.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint5(osg::MatrixTransform *previousJoint)
{
    double length = 1.5;
    double width = 0.1;
    double hight = 2;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-8.0f, -10.5f, -1.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "glass02.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint6(osg::MatrixTransform *previousJoint)
{
    double length = 1.5;
    double width = 0.1;
    double hight = 2;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(8.0f, 10.5f, -1.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "glass02.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint7(osg::MatrixTransform *previousJoint)
{
    double length = 1.5;
    double width = 0.1;
    double hight = 2;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-8.0f, 10.5f, -1.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "glass02");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint8(osg::MatrixTransform *previousJoint)
{
    double length = 2;
    double width = 2;
    double hight = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 9.5f, 0.55f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint9(osg::MatrixTransform *previousJoint)
{
    double length = 2;
    double width = 2;
    double hight = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    /*osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);*/
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-9.5f, 0.0f, 0.55f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*upper profil*/
osg::MatrixTransform *buildJoint10(osg::MatrixTransform *previousJoint)
{
    double length = 21;
    double width = 1;
    double hight = 1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, 0.0f, 1.1f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "profile2.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint11(osg::MatrixTransform *previousJoint)
{
    double width = 1.5;
    double length = 0.1;
    double hight = 2.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.5f, 0.0f, 1.5f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*right side Motor holder */
osg::MatrixTransform *buildJoint12(osg::MatrixTransform *previousJoint)
{
    double width = 4.0;
    double length = 0.1;
    double hight = 1.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-10.0f, 10.0f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*right side motor*/
osg::MatrixTransform *buildJoint13(osg::MatrixTransform *previousJoint)
{
    double width = 1.5;
    double length = 1.5;
    double hight = 1.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-10.75f, 11.1f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "motorRight.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*right side motor wheel*/
osg::MatrixTransform *buildJoint14(osg::MatrixTransform *previousJoint)
{
    float radius = 0.3;
    float height = 0.7;


    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-11.0f, 0.0f, 9.5f), radius, height),
                                   hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

/*left Motor holder */
osg::MatrixTransform *buildJoint15(osg::MatrixTransform *previousJoint)
{
    double width = 4.0;
    double length = 0.1;
    double hight = 1.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(10.0f, 10.0f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*left side motor*/
osg::MatrixTransform *buildJoint16(osg::MatrixTransform *previousJoint)
{
    double width = 1.5;
    double length = 1.5;
    double hight = 1.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(10.75f, 11.1f, 0.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "motorLeft.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*left motor wheel*/
osg::MatrixTransform *buildJoint17(osg::MatrixTransform *previousJoint)
{
    float radius = 0.3;
    float height = 0.7;


    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(-11.0f, 0.0f, -9.5f), radius, height),
                                   hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint18(osg::MatrixTransform *previousJoint)
{
    double width = 3.5;
    double length = 1.5;
    double hight = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -10.5f, 1.6f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
/*upper motor*/
osg::MatrixTransform *buildJoint19(osg::MatrixTransform *previousJoint)
{
    double width = 1.5;
    double length = 1.5;
    double hight = 1.5;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -11.5f, 2.4f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    addTexture(plate, "motorDown.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

/*upper motor wheel*/
osg::MatrixTransform *buildJoint20(osg::MatrixTransform *previousJoint)
{
    float radius = 0.3;
    float height = 0.7;


    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, -11.5f, 1.1f), radius, height),
                                   hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint21(osg::MatrixTransform *previousJoint)
{
    double width = 0.1;
    double length = 0.5;
    double hight = 3.0;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f, -0.6f, 3.0f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}
osg::MatrixTransform *buildJoint22(osg::MatrixTransform *previousJoint)
{
    double width = 0.5;
    double length = 3.0;
    double hight = 0.1;

    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(-2.1f, 0.0f, 4.5f),length, width, hight), hints);
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 1.0, 0.0, 0.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint23(osg::MatrixTransform *previousJoint)
{
    float radius = 0.1;
    float height = 2.0;


    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 3.0f, 3.5f), radius, height),
                                   hints);
    plate->setColor(osg::Vec4(0.2f, 0.2f, 0.2f, 0.2f));
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

osg::MatrixTransform *buildJoint24(osg::MatrixTransform *previousJoint)
{
    float radius = 0.8;


    osg::MatrixTransform *xTransform = new osg::MatrixTransform();
    previousJoint->addChild(xTransform);
    osg::Matrix xRot = osg::Matrix::rotate(osg::PI_2, 0.0, 0.0, 1.0);
    xTransform->setMatrix(xRot);
    osg::Geode *joint = new osg::Geode();//
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(3.0f, 0.0f, 1.8f), radius), hints);
    plate->setColor(osg::Vec4(0.4f, 0.4f, 0.4f, 0.4f));
    joint->addDrawable(plate);
    xTransform->addChild(joint);

    osg::MatrixTransform *zTransform = new osg::MatrixTransform();
    /*osg::Matrix zTrans = osg::Matrix::translate(0.0, 0.0, 0.0);
    osg::Matrix zRot = osg::Matrix::rotate(-(osg::PI_2 + jointAngle2), 0.0, 0.0, 1.0);
    zTransform->setMatrix(zTrans * zRot);*/
    xTransform->addChild(zTransform);

    //addTexture(plate, "altTahta.bmp");

    //zTransform->setNodeMask( rcvShadowMask );
    return zTransform;
}

void setCameraPosision(osgViewer::Viewer & viewer){

    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;

    viewer.setCameraManipulator(manipulator);

    osg::Vec3d eye(1.0, -70.0, 40.0);
    osg::Vec3d center(1.0, 1.0, 1.0);
    osg::Vec3d up(0.0, 1.0, 0.0);

    manipulator->setAutoComputeHomePosition(false);
    manipulator->setHomePosition(eye, center, up);
    manipulator->home(0.0);
}

osg::Geometry* createQuad()
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image =
            osgDB::readImageFile( "stickMan.bmp" );
    texture->setImage( image.get() );
    osg::ref_ptr<osg::Geometry> quad=
            osg::createTexturedQuadGeometry(
                    osg::Vec3(-0.5f, 0.0f,-0.5f),
                    osg::Vec3(1.0f,0.0f,0.0f),
                    osg::Vec3(0.0f,0.0f,1.0f) );
    osg::StateSet* ss = quad->getOrCreateStateSet();
    ss->setTextureAttributeAndModes( 0, texture.get() );
    return quad.release();
}

osgWidget::Window* locateStickMan(const std::string& name, osgWidget::Box::BoxType bt) {
    osg::ref_ptr<osgWidget::Box>    box     = new osgWidget::Box(name, bt, true);

    osgWidget::Widget* widget3 = new osgWidget::Widget("", 84.0f, 99.0f);

    widget3->setImage("stickMan.bmp");
    widget3->setTexCoord(0.0f, 0.0f, osgWidget::Widget::LOWER_LEFT);
    widget3->setTexCoord(1.0f, 0.0f, osgWidget::Widget::LOWER_RIGHT);
    widget3->setTexCoord(1.0f, 1.0f, osgWidget::Widget::UPPER_RIGHT);
    widget3->setTexCoord(0.0f, 1.0f, osgWidget::Widget::UPPER_LEFT);

    box->addWidget(widget3);

    return box.release();
}

class stickManUpdateCallback : public osgWidget::Callback {
public:

    stickManUpdateCallback(int _x, int _y, int _size){
        x = _x;
        y = _y;
        size = _size;

        /*ertices = new osg::Vec3Array;
        for(int i=0; i<x; i++) {
            vertices->push_back(osg::Vec3(cornerX+1+i, cornerY, 0));
        }*/

    }

    virtual void update(osg::NodeVisitor *, osgWidget::Window* drawable) {
        int i;
        /*osg::Vec3 vecA;
        osg::Vec3 vecB;


        osg::Geometry *geometry = dynamic_cast<osg::Geometry *>(drawable);
        if (!geometry) return;

        vertices->erase(vertices->begin());
        for(int i=0; i<vertices->size()-1; i++) {
            vertices->at(i).set(cornerX+i, vertices->at(i+1).y(), 0);
        }
        if(axis == 'x')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));
        else if(axis == 'y')
            vertices->push_back(osg::Vec3(cornerX + x, cornerY + (rand() % y) , 0));


        geometry->setVertexArray(vertices);
        geometry->removePrimitiveSet(0, 1);
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP  , 0, vertices->size()));*/

        osg::Vec3 vecA;
        osg::Vec3 vecB;

        osgWidget::Window* pano = dynamic_cast<osgWidget::Window*>(drawable);
        if (!pano) return;

        scale = size/2/99;
        posX = x/2 + 925.0;
        posY = (297.0 - y/2) + 50.0;

        if(rotation < 90){
            ratio = rotation/90;
            transX = ratio*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX+transX, posY);
        }else if(rotation < 180){
            ratio = (rotation - 90)/90;
            transX = (1-ratio)*size;
            transY = ratio*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX+transX, posY+transY);
        }else if(rotation < 270){
            ratio = (rotation - 180)/90;
            transX = ratio*size;
            transY = (1-ratio)*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX-transX, posY+transY);
        }else{
            ratio = (rotation-270)/90;
            transX = (1-ratio)*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX-transX, posY);
        }


    }

protected:
    int x;
    int y;
    int size;
    osg::Vec3Array *vertices;

};

#ifdef _WIN32

DWORD WINAPI thread_func(LPVOID lpParameter) {
    uint32_t x, y;
    uint8_t d;
    uint16_t x_l, y_l, size, rotation;
    int opcode;
    int stepCount = 0;
    int i = 0;
    float scale, posX, posY, transX, transY, ratio;

    //stickMan->setOrigin(1020.0f, 50.0f);

    Client client;

    while(1){
        if(true){
            if((opcode = client.waitDataFromServer()) == -1)
            break;

        switch(opcode) {
            case 0:
            ++ stepCount;
            if(stepCount <= 4){
            Sleep(100);
            for(i = 0; i< 10; ++i){
            Sleep(5);
            KeyboardEventHandler::translate(0.1, 0.0, 0.0,joint9);
            }
            xVal += 100;
            }
            else if(stepCount == 5){
            Sleep(100);
            for(i = 0; i< 15; ++i){
            Sleep(5);
            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint2);
            }
            yVal -= 100;
            }
            else if(stepCount <= 9){
            Sleep(100);
            for(i = 0; i< 10; ++i){
            Sleep(5);
            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint9);
            }
            xVal -= 100;
            }
            else if(stepCount == 10){
            Sleep(100);
            for(i = 0; i< 15; ++i){
            Sleep(5);
            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint2);
            }
            yVal -= 100;
            }
            else{
            Sleep(100);
            for(i = 0; i< 10; ++i){
            Sleep(5);
            KeyboardEventHandler::translate(0.1, 0.0, 0.0,joint9);
            }
            xVal += 100;
            }
            break;
            case 1:
            client.getCoordinates(x, y, d, x_l, y_l, size, rotation);
            stickMan->setNodeMask(0xfffff);

            if(d > 0){
            positionX = x;
            positionY = y;
            rotationText = rotation;
            }else{
            positionX = -1;
            positionY = -1;
            rotationText = -1;
            }
            setPositions();


            cout << "X: " << x << endl << "Y: " << y << endl << "Rotation:" << rotation << endl <<
            "x_l: " << x_l << endl << "y_l:" << y_l << endl << "length: " << size << endl
            << "D: " << (int)d << endl;

            scale = size/2/99;
            posX = x_l/2 + 925.0;
            posY = (297.0 - y_l/2) + 50.0;

            if(rotation < 90){
            ratio = rotation/90;
            transX = ratio*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX+transX, posY);
            }else if(rotation < 180){
            ratio = (rotation - 90)/90;
            transX = (1-ratio)*size;
            transY = ratio*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX+transX, posY+transY);
            }else if(rotation < 270){
            cout<< "here"<<endl;
            ratio = (rotation - 180)/90;
            transX = ratio*size;
            transY = (1-ratio)*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX-transX, posY+transY);
            }else{
            ratio = (rotation-270)/90;
            transX = (1-ratio)*size;
            stickMan->setScale(scale);
            stickMan->setRotate(rotation);
            stickMan->setOrigin(posX-transX, posY);
            }

            stickMan->setOrigin(1120.0f, 145.0f);

            break;

            default:
                cerr << "Unknown opcode: " << opcode << endl;
                break;
            }
        }

    }
}

//FOR LİNUX
#elif __linux__

pthread_t thread;

void *thread_func(void *a) {
    uint32_t x, y;
    uint8_t d;
    uint16_t x_l, y_l, size, rotation;
    int opcode;
    int stepCount = 0;
    int i = 0;
    float scale, posX, posY, transX, transY, ratio;

    stickMan->setOrigin(1020.0f, 50.0f);

    Client client;

    while(1){
        if(true){
             if((opcode = client.waitDataFromServer()) == -1)
                 break;

            switch(opcode) {
                case 0:
                    ++ stepCount;
                    if(stepCount <= 4){
                        usleep(100000);
                        for(i = 0; i< 10; ++i){
                            KeyboardEventHandler::translate(0.1, 0.0, 0.0,joint9);
                        }
                        xVal += 100;
                    }
                    else if(stepCount == 5){
                        usleep(100000);
                        for(i = 0; i< 10; ++i){
                            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint2);
                        }
                        yVal -= 100;
                    }
                    else if(stepCount <= 9){
                        usleep(100000);
                        for(i = 0; i< 10; ++i){
                            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint9);
                        }
                        xVal -= 100;
                    }
                    else if(stepCount == 10){
                        usleep(100000);
                        for(i = 0; i< 10; ++i){
                            KeyboardEventHandler::translate(-0.1, 0.0, 0.0,joint2);
                        }
                        yVal -= 100;
                    }
                    else{
                        usleep(100000);
                        for(i = 0; i< 10; ++i){
                            KeyboardEventHandler::translate(0.1, 0.0, 0.0,joint9);
                        }
                        xVal += 100;
                    }
                    break;
                case 1:
                    client.getCoordinates(x, y, d, x_l, y_l, size, rotation);
                    stickMan->setNodeMask(0xfffff);

                    if(d > 0){
                        positionX = x;
                        positionY = y;
                        rotationText = rotation;
                    }else{
                        positionX = -1;
                        positionY = -1;
                        rotationText = -1;
                    }
                    setPositions();

                    cout << "X: " << x << endl << "Y: " << y << endl << "Rotation:" << rotation << endl <<
                         "x_l: " << x_l << endl << "y_l:" << y_l << endl << "length: " << size << endl
                         << "D: " << (int)d << endl;

                    scale = size/2/99;
                    posX = x_l/2 + 925.0;
                    posY = (297.0 - y_l/2) + 50.0;

                    if(rotation < 90){
                        ratio = rotation/90;
                        transX = ratio*size;
                        stickMan->setScale(scale);
                        stickMan->setRotate(rotation);
                        stickMan->setOrigin(posX+transX, posY);
                    }else if(rotation < 180){
                        ratio = (rotation - 90)/90;
                        transX = (1-ratio)*size;
                        transY = ratio*size;
                        stickMan->setScale(scale);
                        stickMan->setRotate(rotation);
                        stickMan->setOrigin(posX+transX, posY+transY);
                    }else if(rotation < 270){
                        cout<< "here"<<endl;
                        ratio = (rotation - 180)/90;
                        transX = ratio*size;
                        transY = (1-ratio)*size;
                        stickMan->setScale(scale);
                        stickMan->setRotate(rotation);
                        stickMan->setOrigin(posX-transX, posY+transY);
                    }else{
                        ratio = (rotation-270)/90;
                        transX = (1-ratio)*size;
                        stickMan->setScale(scale);
                        stickMan->setRotate(rotation);
                        stickMan->setOrigin(posX-transX, posY);
                    }

                    stickMan->setOrigin(1120.0f, 145.0f);

                    break;

                default:
                    cerr << "Unknown opcode: " << opcode << endl;
                    break;
            }
        }

    }
}

#endif

int main(int, char **) {
    hints->setDetailRatio(0.5f);

    jointAngle1 = 0;
    jointAngle2 = 0.0;
    EndEffector = 1.0;
    //Vec3d eye, center, up;
    osgViewer::Viewer viewer;

    viewer.createDistortionTexture(50,50);

    setCameraPosision(viewer);

    viewer.setUpViewInWindow(5, 35, 1360, 680);
//--------------------------------------------Text------------------------------------------------

    osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;

    osg::Vec4 color = osg::Vec4(0.5f, 0.9f, 0.1f, 1.0f);

    textGeode->addDrawable(createTextColored(osg::Vec3(500.0f, 650.0f, 0.0f), color,
                                             "Project: Find Joe", 30.0f));

    textGeode->addDrawable(createTextColored( osg::Vec3(600.0f, 600.0f, 0.0f), color,
                                              "- by Group 3", 15.0f));

    color = osg::Vec4(0.9f, 0.9f, 0.9f, 1.0f);
    textGeode->addDrawable(createTextColored( osg::Vec3(50.0f, 170.0f, 0.0f), color,
                                              "Positon of the Stick Man :", 15.0f));

    textGeode->addDrawable(createTextColored( osg::Vec3(1010.0f, 370.0f, 0.0f), color,
                                              "Positon of the Stick Man :", 15.0f));
    color = osg::Vec4(0.8f, 0.8f, 0.8f, 0.8f);
    textGeode->addDrawable(createTextColored( osg::Vec3(100, 112, 0.0f), color,
                                              "X :", 15.0f));
    textGeode->addDrawable(createTextColored( osg::Vec3(100, 92, 0.0f), color,
                                              "Y :", 15.0f));
    textGeode->addDrawable(createTextColored( osg::Vec3(80, 74, 0.0f), color,
                                              "Rot :", 15.0f));

//-----------------------------------------------Grafics------------------------------------------
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    drawCharts(geode, 50.0, 300.0);
    makeAllTextXChart(geode, 50, 450, 100);
    makeAllTextYChart(geode, 50, 300, 75);

    // geode'de derinlik kavramını kaldırıdm
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset->setRenderBinDetails(11,"RenderBin");

    osg::ref_ptr<osg::Geode> geodeFrameCount = new osg::Geode;
    writeScoreTextTable(geodeFrameCount, 100, 70);

    osg::StateSet* frameStateSet = geodeFrameCount->getOrCreateStateSet();
    frameStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    frameStateSet->setRenderBinDetails(11,"RenderBin");

    osg::ref_ptr<osg::Geode> paperGeode = new osg::Geode;
    color = osg::Vec4(0.8f, 0.8f, 0.8f, 0.8f);
    osg::ref_ptr<osg::Geometry>  paperPano = makePano( 925.0f, 50.0f, 420.0f, 297.0f, color);
    paperGeode->addDrawable(paperPano);

    paperGeode->addDrawable(makeLines(1009.0f, 50.0, 250, 297));
    paperGeode->addDrawable(makeLines(1093.0f, 50.0, 250, 297));
    paperGeode->addDrawable(makeLines(1177.0f, 50.0, 250, 297));
    paperGeode->addDrawable(makeLines(1261.0f, 50.0, 250, 297));
    paperGeode->addDrawable(makeVerticalLine(925.0f, 149.0f, 420, 250));
    paperGeode->addDrawable(makeVerticalLine(925.0f, 248.0f, 420, 250));

    stickMan = locateStickMan("stickMan", osgWidget::Box::HORIZONTAL);
    stickMan->getBackground()->setColor(0.2f, 0.2f, 0.2f, 0.2f);
    //stickMan->addCallback(new stickManUpdateCallback(925.0f, 50.0f, 99.0f));

    //stickMan->attachMoveCallback();


//-----------------------------------------------layer/buttons----------------------------------------
    osg::ref_ptr<osgWidget::WindowManager> wm = new osgWidget::WindowManager(&viewer, 1350.0f, 680.0f, 0xf0000000);

    osg::Camera* camera = wm->createParentOrthoCamera();
    wm->addChild( createSimpleTabs(985.0f, 485.0f) );
    wm->addChild(stickMan);

    /*osg::Geode *joint = new osg::Geode();
    osg::ShapeDrawable* plate;
    plate = new osg::ShapeDrawable(new osg::Box(osg::Vec3(900.0f, 50.0f, 1.5f),2, 2, 2), hints);
    joint->addDrawable(plate);
    //wm->addChild(joint);
    //camera->addChild(joint);*/
    stickMan->setOrigin(1000.0f, 145.0f);
    stickMan->setVisibilityMode(osgWidget::Window::VM_ENTIRE);
    stickMan->setRotate(135.0);
    stickMan->setOrigin(1150.0f, 180.0f);
    stickMan->setScale(1.0);
    stickMan->setNodeMask(0x0);

    wm->resizeAllWindows();
    viewer.addEventHandler(new KeyboardEventHandler() );
    /*viewer.addEventHandler(
            new osgWidget::MouseHandler(wm.get()) );
    viewer.addEventHandler( new osgWidget::ResizeHandler(wm.get(),
                                                         camera) );
    viewer.addEventHandler(
            new osgWidget::CameraSwitchHandler(wm.get(), camera) );
    viewer.addEventHandler(new PickHandler());

    //osg::Camera *camera = createHUDCamera(0, 1024, 0, 768);*/
    camera->addChild(textGeode.get());
    camera->addChild(geode);
    camera->addChild(geodeFrameCount);
    camera->addChild(paperGeode);

    camera->getOrCreateStateSet()->setMode(
            GL_LIGHTING, osg::StateAttribute::OFF);
    osg::ref_ptr<osg::Group> root = createShapes();
    root->addChild( camera );

    viewer.setSceneData( root.get() );
    viewer.realize();

#if __linux__
    int i;
    pthread_create(&thread, NULL, thread_func, (void *) i);//LINUX

#elif _WIN32
    CreateThread(NULL, 0, thread_func, NULL, 0, 0);//WINDOWS
#endif

    return viewer.run();
}