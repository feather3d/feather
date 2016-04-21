/***********************************************************************
 *
 * Filename: main.cpp
 *
 * Description: Entry point of the program.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
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
 *
 ***********************************************************************/

#include "deps.hpp"
#include "curves.hpp"
//#include "viewportthread.hpp"
//#include "viewport2.hpp"
//#include "viewport3.hpp"
#include "sg_editor.hpp"
#include "field_model.hpp"
#include "layer_model.hpp"
#include "tree_model.hpp"
#include "qml.hpp"
#include "commands.hpp"
#include "viewport4.hpp"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <Qt3DInput/QInputAspect>
#include <Qt3DQuick/QQmlAspectEngine>

// CONFIG DATA

typedef struct {
    std::string mainpath;
} config;

BOOST_FUSION_ADAPT_STRUCT(
    config,
    (std::string, mainpath)
    )

namespace config_parsing 
{

    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace lambda = boost::lambda;

    template <typename InputIterator>
        struct skipper_grammar : qi::grammar<InputIterator>
    {
        typedef skipper_grammar<InputIterator> type;
        skipper_grammar()
            : skipper_grammar::base_type( skip_on ),
            comment_char( '#' )
        {
            using ascii::char_;
            using ascii::space;
            using ascii::no_case;
            using qi::lit;
            using qi::eol;
            using phoenix::ref;
            using namespace qi::labels;

            comment =
                //char_( ref( comment_char ) )
                char_ ( '#' )
                >> *( char_ - eol ) >> eol
                ;

            comment_charset =
                char_( "!\"#$%&'()*,:;<>?@\\^`{|}~" )
                ;

            skip_on =
                space  // tab/space/cr/lf
                | comment
                ;
        }

        char comment_char;
        qi::rule<InputIterator> comment;
        qi::rule<InputIterator, char()> comment_charset;
        qi::rule<InputIterator> comment_directive;
        qi::rule<InputIterator> skip_on;
    };


    template<typename Iterator, typename Skipper>
        struct config_parser : qi::grammar<Iterator, config(), Skipper>
    {
        config_parser() : config_parser::base_type(data ,"data")
        {
            using qi::lit;
            using qi::lexeme;
            using qi::on_error;
            using qi::fail;
            using qi::int_;
            using qi::uint_;
            using qi::double_;
            //using qi::eol;
            using ascii::char_;
            using ascii::string;
            using namespace qi::labels;
            using phoenix::construct;
            using phoenix::val;
            using phoenix::if_;
            using phoenix::arg_names::arg1;

            data %= lit("mainpath[\"") >> lexeme[+(char_ - '"')] >> lit("\"]");

            // names for rule errors
            data.name("data");

            // error handling
            on_error<fail>
                (
                 data,
                 std::cout
                 << val("Error! Expecting ")
                 << boost::spirit::_4   // what failed
                 << val(" here: \"")
                 << construct<std::string>(boost::spirit::_3, boost::spirit::_2) // interators to error pos, end
                 << val("\"")
                 << std::endl
                );
        }

        qi::rule<Iterator, config(), Skipper> data;
    }; 

}


// load config file
bool load_config(config& _config)
{
    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef config_parsing::skipper_grammar<iterator_type> skipper_type;
    typedef config_parsing::config_parser<iterator_type, skipper_type> config_parser;

    // add std::ios::binary flag to read binary data
    std::ifstream fs("feather.rc", std::ios::in|std::ios::ate);
    long size;
    char* buffer;
    std::string input;

    if(fs.is_open())
    {
        size = fs.tellg();
        buffer = new char[size+1];
        fs.seekg(0, std::ios::beg);
        fs.read(buffer, size);
        buffer[size] = '\0';
        input = buffer;
        fs.close();

        config_parser g; // our grammar

        std::string::const_iterator iter = input.begin();
        std::string::const_iterator end = input.end();

        bool r = phrase_parse(iter, end, g, skipper_type(), _config);

        delete[] buffer;
        
        std::cout << "Config file loaded - \"" << _config.mainpath << "\" is the entry point\n";

    } else {
        std::cout << "Config file failed to load\n";
        return false;
    }

    return true;
};


// callback function for scenegraph singleton
static QObject *get_scenegraph(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    SceneGraph *sg = new SceneGraph();
    return sg;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<BezierCurve>("feather.ui.curves", 1, 0, "BezierCurve");
    //qmlRegisterType<ViewportThread>("feather.viewport", 1, 0, "Viewport");
    //qmlRegisterType<Viewport2>("feather.viewport", 1, 0, "Viewport2");
    //qmlRegisterType<DeferredRenderer>("feather.viewport", 1, 0, "DeferredRenderer");
    //qmlRegisterType<Viewport3>("feather.viewport", 1, 0, "Viewport3");
    qmlRegisterType<VulkanViewport>("feather.viewport", 1, 0, "Viewport");
    qmlRegisterType<SceneGraphEditor>("feather.editors", 1, 0, "SceneGraphEditor");
    qmlRegisterSingletonType<SceneGraph>("feather.scenegraph", 1, 0, "SceneGraph", get_scenegraph);
    qmlRegisterType<TreeModel>("feather.outliner", 1, 0, "OutlinerModel");
    qmlRegisterType<Node>("feather.node", 1, 0, "Node");
    qmlRegisterType<Field>("feather.field", 1, 0, "Field");
    qmlRegisterType<FieldModel>("feather.field", 1, 0, "FieldModel");
    qmlRegisterType<LayerModel>("feather.layer", 1, 0, "LayerModel");
    qmlRegisterType<Parameter>("feather.command", 1, 0, "Parameter");
    qmlRegisterType<Command>("feather.command", 1, 0, "Command");
    qmlRegisterType<Plugins>("feather.plugin", 1, 0, "Plugins");
    qmlRegisterType<Tools>("feather.tools", 1, 0, "Tools");


    // Add the Root node to the scenegraph and setup the scene
    feather::qml::command::init();

    int execReturn = 0;

    config cnfg;
    bool p = load_config(cnfg);

    QQmlApplicationEngine view(cnfg.mainpath.c_str());
    //QQmlApplicationEngine view;
    //QQuickView view;

    //view.aspectEngine()->registerAspect(new Qt3D::QRenderAspect());
    //view.aspectEngine()->registerAspect(new Qt3D::QInputAspect());

    /*
    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QQmlApplicationEngine *>(&view)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(&view));
    view.aspectEngine()->setData(data);
    */

    /*
    Qt3D::Quick::QQmlAspectEngine engine;

    engine.aspectEngine()->registerAspect(new Qt3D::QRenderAspect());
    engine.aspectEngine()->registerAspect(new Qt3D::QInputAspect());

    QVariantMap data;
    data.insert(QStringLiteral("surface"), QVariant::fromValue(static_cast<QQmlApplicationEngine *>(&view)));
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(&view));
    engine.aspectEngine()->setData(data);

    //engine.setSource(QUrl(QStringLiteral("main.qml")));
    engine.setSource(QUrl(cnfg.mainpath.c_str()));

    //view.show();
    */

    execReturn = app.exec();

    /* 
    {
        //QQmlApplicationEngine view("ui/main.qml");
        QQmlApplicationEngine view(cnfg.mainpath.c_str());
        execReturn = app.exec();
    }
    */

    return execReturn;
};
