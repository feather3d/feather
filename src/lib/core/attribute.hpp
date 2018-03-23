/***********************************************************************
 *
 * Filename: attribute.hpp
 *
 * Description: holds a single value inside of a plugin.
 *
 * Copyright (C) 2018 Richard Layman, rlayman2000@yahoo.com 
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

#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

#include "deps.hpp"
#include "types.hpp"
#include "status.hpp"
#include "object.hpp"
#include "field.hpp"


namespace feather
{

    namespace attribute 
    {

        enum Type {
            Bool,
            Int,
            UInt,
            Real,
            String
        };

        enum Function {
            Find,
            GetType,
            GetValue,
            SetValue
        };

    } // namespace attribute

} // namespace feather


#define ADD_BOOL_ATTRIBUTE(__attribute_id, __name, __default_value)\
    namespace feather {\
        \
        static bool __name = __default_value;\
        \
        template <> struct find_attributes<__attribute_id> {\
            static bool exec(uint16_t attribute_id) {\
                if(attribute_id==__attribute_id){\
                    return true;\
                } else {\
                    return find_attributes<__attribute_id-1>::exec(attribute_id);\
                }\
            };\
        };\
        \
        /* TYPE */\
        \
        template <> struct call_attributes<__attribute_id,attribute::GetType,attribute::Type> {\
            static status exec(uint16_t attribute_id, attribute::Type& type) {\
                if(attribute_id==__attribute_id){\
                    type = attribute::Bool;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetType,attribute::Type>::exec(attribute_id,type);\
                }\
            };\
        };\
        \
        /* GET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    value = __name;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,uint32_t>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,int>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,double>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,std::string>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* SET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    __name = value;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,bool>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,bool>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,bool>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,bool>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
    } /* namespace feather */\


#define ADD_UINT_ATTRIBUTE(__attribute_id, __name, __default_value)\
    namespace feather {\
        \
        static uint32_t __name = __default_value;\
        \
        template <> struct find_attributes<__attribute_id> {\
            static bool exec(uint16_t attribute_id) {\
                if(attribute_id==__attribute_id){\
                    return true;\
                } else {\
                    return find_attributes<__attribute_id-1>::exec(attribute_id);\
                }\
            };\
        };\
        \
        /* TYPE */\
        template <> struct call_attributes<__attribute_id,attribute::GetType,attribute::Type> {\
            static status exec(uint16_t attribute_id, attribute::Type& type) {\
                if(attribute_id==__attribute_id){\
                    type = attribute::UInt;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetType,attribute::Type>::exec(attribute_id,type);\
                }\
            };\
        };\
        \
        /* GET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,bool>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    value = __name;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,int>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,double>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,std::string>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* SET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,uint32_t>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    __name = value;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,uint32_t>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,uint32_t>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,uint32_t>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
    } /* namespace feather */\


#define ADD_INT_ATTRIBUTE(__attribute_id, __name, __default_value)\
    namespace feather {\
        \
        static int __name = __default_value;\
        \
        template <> struct find_attributes<__attribute_id> {\
            static bool exec(uint16_t attribute_id) {\
                if(attribute_id==__attribute_id){\
                    return true;\
                } else {\
                    return find_attributes<__attribute_id-1>::exec(attribute_id);\
                }\
            };\
        };\
        \
        /* TYPE */\
        template <> struct call_attributes<__attribute_id,attribute::GetType,attribute::Type> {\
            static status exec(uint16_t attribute_id, attribute::Type& type) {\
                if(attribute_id==__attribute_id){\
                    type = attribute::Int;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetType,attribute::Type>::exec(attribute_id,type);\
                }\
            };\
        };\
        \
        /* GET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,bool>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,uint32_t>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    value = __name;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,double>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,std::string>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* SET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,int>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,int>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    __name = value;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,int>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,int>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
    } /* namespace feather */\


#define ADD_REAL_ATTRIBUTE(__attribute_id, __name, __default_value)\
    namespace feather {\
        \
        static double __name = __default_value;\
        \
        template <> struct find_attributes<__attribute_id> {\
            static bool exec(uint16_t attribute_id) {\
                if(attribute_id==__attribute_id){\
                    return true;\
                } else {\
                    return find_attributes<__attribute_id-1>::exec(attribute_id);\
                }\
            };\
        };\
        \
        /* TYPE */\
        template <> struct call_attributes<__attribute_id,attribute::GetType,attribute::Type> {\
            static status exec(uint16_t attribute_id, attribute::Type& type) {\
                if(attribute_id==__attribute_id){\
                    type = attribute::Real;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetType,attribute::Type>::exec(attribute_id,type);\
                }\
            };\
        };\
        \
        /* GET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,bool>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,uint32_t>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,int>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    value = __name;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,std::string>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* SET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,double>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,double>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,double>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    __name = value;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,double>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
    } /* namespace feather */\


#define ADD_STRING_ATTRIBUTE(__attribute_id, __name, __default_value)\
    namespace feather {\
        \
        static std::string __name = __default_value;\
        \
        template <> struct find_attributes<__attribute_id> {\
            static bool exec(uint16_t attribute_id) {\
                if(attribute_id==__attribute_id){\
                    return true;\
                } else {\
                    return find_attributes<__attribute_id-1>::exec(attribute_id);\
                }\
            };\
        };\
        \
        /* TYPE */\
        template <> struct call_attributes<__attribute_id,attribute::GetType,attribute::Type> {\
            static status exec(uint16_t attribute_id, attribute::Type& type) {\
                if(attribute_id==__attribute_id){\
                    type = attribute::String;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetType,attribute::Type>::exec(attribute_id,type);\
                }\
            };\
        };\
        \
        /* GET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,bool>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,uint32_t>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,int>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<std::string,double>(__name,value);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::GetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    value = __name;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::GetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* SET */\
        /* BOOL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,bool> {\
            static status exec(uint16_t attribute_id, bool& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<bool,std::string>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,bool>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* UINT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,uint32_t> {\
            static status exec(uint16_t attribute_id, uint32_t& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<uint32_t,std::string>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,uint32_t>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* INT */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,int> {\
            static status exec(uint16_t attribute_id, int& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<int,std::string>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,int>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* REAL */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,double> {\
            static status exec(uint16_t attribute_id, double& value) {\
                if(attribute_id==__attribute_id) {\
                    tools::convert<double,std::string>(value,__name);\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,double>::exec(attribute_id,value);\
                }\
            };\
        };\
        \
        /* STRING */\
        template <> struct call_attributes<__attribute_id,attribute::SetValue,std::string> {\
            static status exec(uint16_t attribute_id, std::string& value) {\
                if(attribute_id==__attribute_id) {\
                    __name = value;\
                    return status();\
                } else {\
                    return call_attributes<__attribute_id-1,attribute::SetValue,std::string>::exec(attribute_id,value);\
                }\
            };\
        };\
   } /* namespace feather */\


#endif
