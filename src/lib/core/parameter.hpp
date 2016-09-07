/***********************************************************************
 *
 * Filename: parameter.hpp
 *
 * Description: Holds values for attributes in commands.
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

#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include "types.hpp"
#include "deps.hpp"

namespace feather
{

    namespace parameter
    {

        enum Type {
            Null,
            Bool,
            Int,
            Real,
            String,
            IntArray,
            RealArray
        };

        template <typename _T> static Type get_type_id() { return Null; };
        template <> Type get_type_id<bool>() { return Bool; };
        template <> Type get_type_id<int>() { return Int; };
        template <> Type get_type_id<double>() { return Real; };
        template <> Type get_type_id<std::string>() { return String; };
        template <> Type get_type_id<std::vector<int>>() { return IntArray; };
        template <> Type get_type_id<std::vector<double>>() { return RealArray; };

        struct ParameterBase
        {
            ParameterBase(std::string n, Type t): name(n), type(t) {};
            ~ParameterBase(){};
            std::string name;
            Type type;
        };

        template <typename _T>
            struct Parameter : public ParameterBase
        {
            Parameter(std::string n, _T v=0) : ParameterBase(n,get_type_id<_T>()){ value=v; };
            ~Parameter() {};
            _T value;
        };

        struct ParameterList
        {
            ParameterList(){};
            ~ParameterList(){ m_params.clear(); };

            void addBoolParameter(std::string name, bool val) { m_params.push_back(new Parameter<bool>(name,val)); };

            void addIntParameter(std::string name, int val) { m_params.push_back(new Parameter<int>(name,val)); };

            void addRealParameter(std::string name, double val) { m_params.push_back(new Parameter<double>(name,val)); };

            void addStringParameter(std::string name, std::string val) { 
                m_params.push_back(new Parameter<std::string>(name,val));
            };

            void addIntArrayParameter(std::string name, std::vector<int> val) { m_params.push_back(new Parameter<std::vector<int>>(name,val)); };

            void addRealArrayParameter(std::string name, std::vector<double> val) { m_params.push_back(new Parameter<std::vector<double>>(name,val)); };

            template <typename _T>
                bool getParameterValue(std::string n, _T &val) {
                    for(int i=0; i < m_params.size(); i++) {
                        if(m_params.at(i)->name==n) {
                            val=static_cast<Parameter<_T>*>(m_params.at(i))->value;
                            return true;
                        }
                    }
                    return false;
                }

            inline const uint parameterCount() { return m_params.size(); };

            inline const std::string parameterName(const uint i) { if(i < parameterCount()){ return m_params.at(i)->name; } return ""; };

            private:
            std::vector<ParameterBase*> m_params;
        };

        // get the parameter name
        template <int _Parameter,int _Command>
        struct get_name{
            static status exec(int key, std::string& name) { return get_name<_Parameter-1,_Command>::exec(key,name); };
        };

        template <int _Command> struct get_name<0,_Command> {
            static status exec(int key, std::string& name) { return status(FAILED,"no command parameter found"); };
        }; 

        // get the parameter type 
        template <int _Parameter,int _Command>
        struct get_type {
            static status exec(int k, Type& t) {
                return get_type<_Parameter-1,_Command>::exec(k,t);
            };
        };

        template <int _Command> struct get_type<0,_Command> {
            static status exec(int k, Type& t) { return status(FAILED,"no command parameter found"); };
        }; 

    } // namespace parameter

} // namespace feather

#define ADD_PARAMETER(__cmdenum,__key,__type,__name)\
namespace feather\
{\
    namespace parameter\
    {\
        template<> status get_name<__key,__cmdenum>::exec(int k, std::string& n) {\
            if(k==__key) {\
                n=__name;\
                return status();\
            } else {\
                return get_name<__key-1,__cmdenum>::exec(k,n);\
            }\
        };\
\
        template<> status get_type<__key,__cmdenum>::exec(int k, Type& t) {\
            if(k==__key) {\
                t=__type;\
                return status();\
            } else {\
                return get_type<__key-1,__cmdenum>::exec(k,t);\
            }\
        };\
\
    } /* namespace parameter */ \
} /* namespace feather*/ \

#endif
