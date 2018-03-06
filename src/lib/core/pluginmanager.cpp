/***********************************************************************
 *
 * Filename: pluginmanager.cpp
 *
 * Description: Loads plugins and manages interaction with the scenegraph.
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

#include "pluginmanager.hpp"
#include "types.hpp"

using namespace feather;

PluginManager::PluginManager():m_pluginPath("~/.feather/plugins")
{

}

PluginManager::~PluginManager()
{
    for(auto n : m_plugins)
        dlclose(n.handle);
}

status PluginManager::load_plugins()
{
    boost::filesystem::path plugin_path("/usr/share/feather/plugins");
    typedef std::vector<boost::filesystem::path> files;
    files plugin_paths;

    if (boost::filesystem::exists(plugin_path))
    {
        if (boost::filesystem::is_regular_file(plugin_path))   
            std::cout << plugin_path << " size is " << boost::filesystem::file_size(plugin_path) << '\n';

        else if (boost::filesystem::is_directory(plugin_path)) {
            std::cout << plugin_path << "is a directory\n";
            std::copy(boost::filesystem::directory_iterator(plugin_path), boost::filesystem::directory_iterator(), std::back_inserter(plugin_paths));
            for (files::const_iterator it (plugin_paths.begin()); it != plugin_paths.end(); ++it)
            {
                if((*it).extension()==".so") {
                    // load plugin here
                    std::cout << "LOADING: " << *it << std::endl;
                    PluginData node;
                    node.path = (*it).string();
                    status s = load_node(node);
                    if(s.state){
                        m_plugins.push_back(node);
                        std::cout << node.path << " loaded\n";
                    }
                    else
                        std::cout << node.path << " " << s.msg << std::endl;
                }
            }
        }

        else
            std::cout << plugin_path << "exists, but is neither a regular file nor a directory\n";
    }
    else
        std::cout << plugin_path << "does not exist\n";

    return status();
}

status PluginManager::fields_init(int node, field::Fields&  fields)
{
    std::cout << "field init node " << node << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_fields_init(node,fields) );
    return status();
}

status PluginManager::update_properties(int node, field::Fields&  fields)
{
    std::cout << "update properties for node " << node << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_update_property(node,fields) );
    return status();
}

status PluginManager::do_it(int node, field::Fields&  fields)
{
    std::cout << "call node " << node << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_do_it(node,fields) );
    return status();
}

/*
status PluginManager::draw_it(int node, draw::DrawItems&  items)
{
    std::cout << "call node " << node << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_draw_it(node,items) );
    return status();
}
*/

status PluginManager::load_node(PluginData &node)
{
   char *error;

   node.handle = dlopen(node.path.c_str(), RTLD_LAZY);
    if (!node.handle) 
    {
        fprintf(stderr, "%s\n", dlerror());
        return status(FAILED,"loaded failed to load");
    }

    node.name = (std::string(*)())dlsym(node.handle, "name");
    node.description = (std::string(*)())dlsym(node.handle, "description");
    node.author = (std::string(*)())dlsym(node.handle, "author");
    node.fields_init = (status(*)(int,field::Fields&))dlsym(node.handle, "fields_init");
    node.update_properties = (status(*)(int,field::Fields&))dlsym(node.handle, "update_properties");
    node.do_it = (status(*)(int,field::Fields&))dlsym(node.handle, "do_it");
    //node.draw_it = (status(*)(int,draw::DrawItems&))dlsym(node.handle, "draw_it");
    node.node_exist = (bool(*)(int))dlsym(node.handle, "node_exist");
    node.node_drawable = (bool(*)(int))dlsym(node.handle, "node_drawable");
    node.node_draw_items = (status(*)(int,draw::DrawItems&))dlsym(node.handle, "node_draw_items");
    node.node_type = (status(*)(int,node::Type&))dlsym(node.handle, "node_type");
    node.node_icon = (bool(*)(int,std::string&))dlsym(node.handle, "node_icon");
    node.create_fields = (status(*)(int,field::Fields&))dlsym(node.handle,"create_fields");
    //node.get_field = (field::FieldBase*(*)(int,int,field::Fields&))dlsym(node.handle, "get_field");
    node.get_fid_list = (status(*)(int,field::connection::Type,field::Fields&,std::vector<field::FieldBase*>&))dlsym(node.handle, "get_fid_list");
    node.render_start = (status(*)(int,render::RenderProperties&))dlsym(node.handle, "render_start");
    node.render_stop = (status(*)(int,render::RenderProperties&))dlsym(node.handle, "render_stop");
    node.render_properties = (status(*)(int,render::RenderProperties&))dlsym(node.handle, "render_properties");
    node.render_exist = (bool(*)(int))dlsym(node.handle, "render_exist");
    node.render_buffer = (status(*)(int,render::RenderBuffer&))dlsym(node.handle, "render_buffer");
    node.render_buffer_exist = (bool(*)(int))dlsym(node.handle, "render_buffer_exist");
    node.command_exist = (bool(*)(std::string))dlsym(node.handle, "command_exist");
    node.command = (status(*)(std::string,parameter::ParameterList))dlsym(node.handle, "command");
    node.parameter_name = (status(*)(std::string,int,std::string&))dlsym(node.handle, "parameter_name");
    node.parameter_type = (status(*)(std::string,int,parameter::Type&))dlsym(node.handle, "parameter_type");


    if ((error = dlerror()) != NULL)  
    {
        fprintf(stderr, "%s\n", error);
        return status(FAILED,"library error");
    }

    //std::cout << "node id=" << node.get_id() << std::endl; 
    //(*fn)(&x);
    //printf("Valx=%d\n",x);

    return status();
}

status PluginManager::create_fields(int node, field::Fields& fields)
{
    std::cout << "create field for node " << node << std::endl;
    for(uint i=0; i < m_plugins.size(); i++) {
        if(m_plugins[i].node_exist(node))
            return m_plugins[i].create_fields(node,fields);
    }
    return status();
}

void PluginManager::get_draw_items(int nid, draw::DrawItems& items)
{
    std::for_each(m_plugins.begin(),m_plugins.end(), call_draw_item(nid,items) );
}

//field::FieldBase* PluginManager::get_fieldBase(int uid, int node, int field, field::Fields& fields)
//{
    // REMOVED FOR THE NEW FIELD LAYOUT
    /*
    for(uint i=0; i < m_plugins.size(); i++) {
        //std::cout << "looking in plugin "  << i << " for node " << node << std::endl;
        if(m_plugins[i].node_exist(node)) {
            //std::cout << "Found node " << node << std::endl;
            // TODO - I need to verify that the field data actually exists
            //std::cout << "PluginManager::get_fieldBase(" << uid << "," << node << "," << field << "," << &fields << ")\n";
            return m_plugins[i].get_field(node,field,fields);
        }
    }*/

    //std::cout << "Never found node " << node << std::endl;
//    return nullptr;
//}

status PluginManager::load_command(PluginData &command)
{
    return status(FAILED,"no command to load");
}

status PluginManager::run_command(std::string cmd, parameter::ParameterList params)
{
    status s(FAILED,"failed to call command");
    std::for_each(m_plugins.begin(),m_plugins.end(), [&cmd,&params,&s](PluginData plugin){ if(plugin.command_exist(cmd) && s.state==FAILED) { s=plugin.command(cmd,params); } } );
    return s;
}

status PluginManager::run_command_string(std::string str)
{
    std::string cmd;
    parameter::ParameterList params;

    using boost::spirit::ascii::string;
    using boost::spirit::ascii::char_;
    using boost::spirit::qi::lexeme;
    using boost::spirit::qi::int_;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::lit;
    using boost::spirit::qi::eol;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;
    using boost::phoenix::val;

    std::string cmdstr;
    std::string paramstr;
    std::vector<std::string> parameters;
    std::string::iterator first = str.begin();
    std::string::iterator last = str.end();

    bool r = phrase_parse(first, last, ( *(char_-'(') >> '(' >> *(char_-')') >> ')' ), space, cmdstr, paramstr);

    if(!r)
    {
        std::cout << "command failed\n";
        return status(FAILED,"Failed to parse command");
    }

    std::cout << "the command is " << cmdstr << ", the params are " << paramstr << std::endl;

    std::cout << "params character length = " << paramstr.length() << std::endl;

    if(paramstr.length()) {
        first = paramstr.begin();
        last = paramstr.end();


        r = phrase_parse(first, last, *( *(char_ - ',') >> ',' || eol ), space, parameters);

        if(!r)
        {
            std::cout << "params failed\n";
            return status(FAILED,"Failed to parse parameters");
        }

        // they and get the parameter type
        bool passed=true;
        int key=1;
        std::for_each(parameters.begin(), parameters.end(), [&cmdstr,this,&key,&passed,&params](std::string val){ bool r = add_parameter_to_list(cmdstr,key,val,params); key++; if(!r){ passed=false; } } );

        if(!passed)
            return status(FAILED,"unable to get parse parameters");

    }

    // replaced call_command(cmd,params)
    status s(FAILED,"command failed to run");
    std::for_each(m_plugins.begin(),m_plugins.end(), [&cmdstr,&params,&s](PluginData plugin){ if(plugin.command_exist(cmdstr) && s.state==FAILED) { s=plugin.command(cmdstr,params); } } );

    return s;
}

int PluginManager::min_uid()
{
    return 0;
}

int PluginManager::max_uid()
{
    // TODO - need to get the real max uid
    // This is current not being called.
    // Could get rid of but will keep here
    // for the time being.
    return 2;
}

status PluginManager::node_icon_file(int nid, std::string& file)
{
    std::for_each(m_plugins.begin(),m_plugins.end(), call_node_icon(nid,file) );
    return status();
}

status PluginManager::node_type(int nid, feather::node::Type& type)
{
    std::for_each(m_plugins.begin(),m_plugins.end(), call_node_type(nid,type) );
    return status();
}

void PluginManager::loaded_plugins(std::vector<PluginInfo>& list)
{
    get_name info(list);
    std::for_each(m_plugins.begin(), m_plugins.end(), info);
}

status PluginManager::get_fid_list(int nid, field::connection::Type conn, field::Fields& fields, std::vector<field::FieldBase*>& list)
{
    get_fids plugin(nid,conn,fields,list);
    std::for_each(m_plugins.begin(), m_plugins.end(), plugin);
    return status(); 
}

bool PluginManager::add_parameter_to_list(std::string cmd, int key, std::string val, parameter::ParameterList& list)
{
    bool r;

    using boost::spirit::qi::int_;
    using boost::spirit::qi::char_;
    using boost::spirit::qi::float_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::qi::parse;
    using boost::spirit::ascii::space;
    using boost::spirit::lit;


    std::string::iterator first,last;
    first = val.begin();
    last = val.end();

    // get the parameter name
    std::string param_name;
    status p(FAILED,"no parameter name found");
    std::for_each(m_plugins.begin(), m_plugins.end(), [&p,&param_name,&key,&cmd](PluginData plugin){ if( plugin.command_exist(cmd) && p.state==FAILED ){ p=plugin.parameter_name(cmd,key,param_name); } } );

    if(p.state==FAILED)
        return false;

    // see if the value is a int
    int ival=0;
    r = parse(first,last, int_, ival);
    if(r && first==last) {
        list.addIntParameter(param_name,ival);
        return true;
    }

    // see if the val is a float
    first = val.begin();
    last = val.end();
    float fval=0.0;
    r = parse(first, last, float_, fval);
    if(r && first==last) {
        list.addRealParameter(param_name,fval);
        return true;
    }

    // see if the val is a bool 
    first = val.begin();
    last = val.end();
    bool bval;
    r = phrase_parse(first, last, lit("true") | lit("false"), space);
    if(r && first==last) {
        if(val=="true")
            bval=true;
        else
            bval=false;
        list.addBoolParameter(param_name,bval);
        return true;
    }

    // see if the val is a string 
    first = val.begin();
    last = val.end();
    std::string sval;
    r = phrase_parse(first, last, ( '"' >> *(char_ - '"') >> '"' ), space, sval);
    if(r && first==last) {
        list.addStringParameter(param_name,sval);
        return true;
    }

    // see if the value is a int array
    std::vector<int> iaval;
    r = parse(first,last, *(int_), iaval);
    if(r && first==last) {
        list.addIntArrayParameter(param_name,iaval);
        return true;
    }

    // see if the value is a real array
    std::vector<double> raval;
    r = parse(first,last, *(float_), raval);
    if(r && first==last) {
        list.addRealArrayParameter(param_name,raval);
        return true;
    }

    return false;
}


// RENDER

status PluginManager::render_start(int id, render::RenderProperties& props)
{
    std::cout << "calling render start " << id << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_render_start(id,props) );
    return status();
}

status PluginManager::render_stop(int id, render::RenderProperties& props)
{
    std::cout << "calling render stop " << id << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_render_stop(id,props) );
    return status();
}

status PluginManager::render_buffer(int id, render::RenderBuffer& buffer)
{
    std::cout << "calling render buffer " << id << std::endl;
    std::for_each(m_plugins.begin(),m_plugins.end(), call_render_buffer(id,buffer) );
    return status();
}

