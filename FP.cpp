/**
 * @file    FP.cpp
 * @brief   Core Utility - Templated Function Pointer Class
 * @author  sam grove
 * @version 1.0
 * @see     
 *
 * Copyright (c) 2013
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FP.h"
#include <stdint.h>

template<class retT, class argT>
FP<retT, argT>::FP()
{
    obj_callback = 0;
    c_callback = 0;
}

template<class retT, class argT>
bool FP<retT, argT>::attached()
{
    return obj_callback || c_callback;
}


template<class retT, class argT>
void FP<retT, argT>::detach()
{
    obj_callback = 0;
    c_callback = 0;
}


template<class retT, class argT>
void FP<retT, argT>::attach(retT (*function)(argT))
{
    c_callback = function;
}

template<class retT, class argT>
retT FP<retT, argT>::operator()(argT arg) const
{
    if( 0 != c_callback )
    {
        return obj_callback ? (obj_callback->*method_callback)(arg) : (*c_callback)(arg);
    }
    return (retT)0;
}

// pre-define the types for the linker
template class FP<void,char>;
template class FP<void,char*>;
template class FP<void,int8_t>;
template class FP<void,int8_t*>;
template class FP<void,uint8_t>;
template class FP<void,uint8_t*>;
template class FP<void,int16_t>;
template class FP<void,int16_t*>;
template class FP<void,uint16_t>;
template class FP<void,uint16_t*>;
template class FP<void,int32_t>;
template class FP<void,int32_t*>;
template class FP<void,uint32_t>;
template class FP<void,uint32_t*>;
template class FP<void,int64_t>;
template class FP<void,int64_t*>;
template class FP<void,uint64_t>;
template class FP<void,uint64_t*>;
template class FP<void,bool>;
template class FP<void,bool*>;
template class FP<void,float>;
template class FP<void,float*>;
template class FP<void,double>;
template class FP<void,double*>;
template class FP<void,void*>;

template class FP<int8_t,char>;
template class FP<int8_t,char*>;
template class FP<int8_t,int8_t>;
template class FP<int8_t,int8_t*>;
template class FP<int8_t,uint8_t>;
template class FP<int8_t,uint8_t*>;
template class FP<int8_t,int16_t>;
template class FP<int8_t,int16_t*>;
template class FP<int8_t,uint16_t>;
template class FP<int8_t,uint16_t*>;
template class FP<int8_t,int32_t>;
template class FP<int8_t,int32_t*>;
template class FP<int8_t,uint32_t>;
template class FP<int8_t,uint32_t*>;
template class FP<int8_t,int64_t>;
template class FP<int8_t,int64_t*>;
template class FP<int8_t,uint64_t>;
template class FP<int8_t,uint64_t*>;
template class FP<int8_t,bool>;
template class FP<int8_t,bool*>;
template class FP<int8_t,float>;
template class FP<int8_t,float*>;
template class FP<int8_t,double>;
template class FP<int8_t,double*>;

template class FP<int8_t*,char>;
template class FP<int8_t*,char*>;
template class FP<int8_t*,int8_t>;
template class FP<int8_t*,int8_t*>;
template class FP<int8_t*,uint8_t>;
template class FP<int8_t*,uint8_t*>;
template class FP<int8_t*,int16_t>;
template class FP<int8_t*,int16_t*>;
template class FP<int8_t*,uint16_t>;
template class FP<int8_t*,uint16_t*>;
template class FP<int8_t*,int32_t>;
template class FP<int8_t*,int32_t*>;
template class FP<int8_t*,uint32_t>;
template class FP<int8_t*,uint32_t*>;
template class FP<int8_t*,int64_t>;
template class FP<int8_t*,int64_t*>;
template class FP<int8_t*,uint64_t>;
template class FP<int8_t*,uint64_t*>;
template class FP<int8_t*,bool>;
template class FP<int8_t*,bool*>;
template class FP<int8_t*,float>;
template class FP<int8_t*,float*>;
template class FP<int8_t*,double>;
template class FP<int8_t*,double*>;

template class FP<uint8_t,char>;
template class FP<uint8_t,char*>;
template class FP<uint8_t,int8_t>;
template class FP<uint8_t,int8_t*>;
template class FP<uint8_t,uint8_t>;
template class FP<uint8_t,uint8_t*>;
template class FP<uint8_t,int16_t>;
template class FP<uint8_t,int16_t*>;
template class FP<uint8_t,uint16_t>;
template class FP<uint8_t,uint16_t*>;
template class FP<uint8_t,int32_t>;
template class FP<uint8_t,int32_t*>;
template class FP<uint8_t,uint32_t>;
template class FP<uint8_t,uint32_t*>;
template class FP<uint8_t,int64_t>;
template class FP<uint8_t,int64_t*>;
template class FP<uint8_t,uint64_t>;
template class FP<uint8_t,uint64_t*>;
template class FP<uint8_t,bool>;
template class FP<uint8_t,bool*>;
template class FP<uint8_t,float>;
template class FP<uint8_t,float*>;
template class FP<uint8_t,double>;
template class FP<uint8_t,double*>;

template class FP<uint8_t*,char>;
template class FP<uint8_t*,char*>;
template class FP<uint8_t*,int8_t>;
template class FP<uint8_t*,int8_t*>;
template class FP<uint8_t*,uint8_t>;
template class FP<uint8_t*,uint8_t*>;
template class FP<uint8_t*,int16_t>;
template class FP<uint8_t*,int16_t*>;
template class FP<uint8_t*,uint16_t>;
template class FP<uint8_t*,uint16_t*>;
template class FP<uint8_t*,int32_t>;
template class FP<uint8_t*,int32_t*>;
template class FP<uint8_t*,uint32_t>;
template class FP<uint8_t*,uint32_t*>;
template class FP<uint8_t*,int64_t>;
template class FP<uint8_t*,int64_t*>;
template class FP<uint8_t*,uint64_t>;
template class FP<uint8_t*,uint64_t*>;
template class FP<uint8_t*,bool>;
template class FP<uint8_t*,bool*>;
template class FP<uint8_t*,float>;
template class FP<uint8_t*,float*>;
template class FP<uint8_t*,double>;
template class FP<uint8_t*,double*>;

template class FP<int16_t,char>;
template class FP<int16_t,char*>;
template class FP<int16_t,int8_t>;
template class FP<int16_t,int8_t*>;
template class FP<int16_t,uint8_t>;
template class FP<int16_t,uint8_t*>;
template class FP<int16_t,int16_t>;
template class FP<int16_t,int16_t*>;
template class FP<int16_t,uint16_t>;
template class FP<int16_t,uint16_t*>;
template class FP<int16_t,int32_t>;
template class FP<int16_t,int32_t*>;
template class FP<int16_t,uint32_t>;
template class FP<int16_t,uint32_t*>;
template class FP<int16_t,int64_t>;
template class FP<int16_t,int64_t*>;
template class FP<int16_t,uint64_t>;
template class FP<int16_t,uint64_t*>;
template class FP<int16_t,bool>;
template class FP<int16_t,bool*>;
template class FP<int16_t,float>;
template class FP<int16_t,float*>;
template class FP<int16_t,double>;
template class FP<int16_t,double*>;

template class FP<int16_t*,char>;
template class FP<int16_t*,char*>;
template class FP<int16_t*,int8_t>;
template class FP<int16_t*,int8_t*>;
template class FP<int16_t*,uint8_t>;
template class FP<int16_t*,uint8_t*>;
template class FP<int16_t*,int16_t>;
template class FP<int16_t*,int16_t*>;
template class FP<int16_t*,uint16_t>;
template class FP<int16_t*,uint16_t*>;
template class FP<int16_t*,int32_t>;
template class FP<int16_t*,int32_t*>;
template class FP<int16_t*,uint32_t>;
template class FP<int16_t*,uint32_t*>;
template class FP<int16_t*,int64_t>;
template class FP<int16_t*,int64_t*>;
template class FP<int16_t*,uint64_t>;
template class FP<int16_t*,uint64_t*>;
template class FP<int16_t*,bool>;
template class FP<int16_t*,bool*>;
template class FP<int16_t*,float>;
template class FP<int16_t*,float*>;
template class FP<int16_t*,double>;
template class FP<int16_t*,double*>;

template class FP<uint16_t,char>;
template class FP<uint16_t,char*>;
template class FP<uint16_t,int8_t>;
template class FP<uint16_t,int8_t*>;
template class FP<uint16_t,uint8_t>;
template class FP<uint16_t,uint8_t*>;
template class FP<uint16_t,int16_t>;
template class FP<uint16_t,int16_t*>;
template class FP<uint16_t,uint16_t>;
template class FP<uint16_t,uint16_t*>;
template class FP<uint16_t,int32_t>;
template class FP<uint16_t,int32_t*>;
template class FP<uint16_t,uint32_t>;
template class FP<uint16_t,uint32_t*>;
template class FP<uint16_t,int64_t>;
template class FP<uint16_t,int64_t*>;
template class FP<uint16_t,uint64_t>;
template class FP<uint16_t,uint64_t*>;
template class FP<uint16_t,bool>;
template class FP<uint16_t,bool*>;
template class FP<uint16_t,float>;
template class FP<uint16_t,float*>;
template class FP<uint16_t,double>;
template class FP<uint16_t,double*>;

template class FP<uint16_t*,char>;
template class FP<uint16_t*,char*>;
template class FP<uint16_t*,int8_t>;
template class FP<uint16_t*,int8_t*>;
template class FP<uint16_t*,uint8_t>;
template class FP<uint16_t*,uint8_t*>;
template class FP<uint16_t*,int16_t>;
template class FP<uint16_t*,int16_t*>;
template class FP<uint16_t*,uint16_t>;
template class FP<uint16_t*,uint16_t*>;
template class FP<uint16_t*,int32_t>;
template class FP<uint16_t*,int32_t*>;
template class FP<uint16_t*,uint32_t>;
template class FP<uint16_t*,uint32_t*>;
template class FP<uint16_t*,int64_t>;
template class FP<uint16_t*,int64_t*>;
template class FP<uint16_t*,uint64_t>;
template class FP<uint16_t*,uint64_t*>;
template class FP<uint16_t*,bool>;
template class FP<uint16_t*,bool*>;
template class FP<uint16_t*,float>;
template class FP<uint16_t*,float*>;
template class FP<uint16_t*,double>;
template class FP<uint16_t*,double*>;

template class FP<int32_t,char>;
template class FP<int32_t,char*>;
template class FP<int32_t,int8_t>;
template class FP<int32_t,int8_t*>;
template class FP<int32_t,uint8_t>;
template class FP<int32_t,uint8_t*>;
template class FP<int32_t,int16_t>;
template class FP<int32_t,int16_t*>;
template class FP<int32_t,uint16_t>;
template class FP<int32_t,uint16_t*>;
template class FP<int32_t,int32_t>;
template class FP<int32_t,int32_t*>;
template class FP<int32_t,uint32_t>;
template class FP<int32_t,uint32_t*>;
template class FP<int32_t,int64_t>;
template class FP<int32_t,int64_t*>;
template class FP<int32_t,uint64_t>;
template class FP<int32_t,uint64_t*>;
template class FP<int32_t,bool>;
template class FP<int32_t,bool*>;
template class FP<int32_t,float>;
template class FP<int32_t,float*>;
template class FP<int32_t,double>;
template class FP<int32_t,double*>;

template class FP<int32_t*,char>;
template class FP<int32_t*,char*>;
template class FP<int32_t*,int8_t>;
template class FP<int32_t*,int8_t*>;
template class FP<int32_t*,uint8_t>;
template class FP<int32_t*,uint8_t*>;
template class FP<int32_t*,int16_t>;
template class FP<int32_t*,int16_t*>;
template class FP<int32_t*,uint16_t>;
template class FP<int32_t*,uint16_t*>;
template class FP<int32_t*,int32_t>;
template class FP<int32_t*,int32_t*>;
template class FP<int32_t*,uint32_t>;
template class FP<int32_t*,uint32_t*>;
template class FP<int32_t*,int64_t>;
template class FP<int32_t*,int64_t*>;
template class FP<int32_t*,uint64_t>;
template class FP<int32_t*,uint64_t*>;
template class FP<int32_t*,bool>;
template class FP<int32_t*,bool*>;
template class FP<int32_t*,float>;
template class FP<int32_t*,float*>;
template class FP<int32_t*,double>;
template class FP<int32_t*,double*>;

template class FP<uint32_t,char>;
template class FP<uint32_t,char*>;
template class FP<uint32_t,int8_t>;
template class FP<uint32_t,int8_t*>;
template class FP<uint32_t,uint8_t>;
template class FP<uint32_t,uint8_t*>;
template class FP<uint32_t,int16_t>;
template class FP<uint32_t,int16_t*>;
template class FP<uint32_t,uint16_t>;
template class FP<uint32_t,uint16_t*>;
template class FP<uint32_t,int32_t>;
template class FP<uint32_t,int32_t*>;
template class FP<uint32_t,uint32_t>;
template class FP<uint32_t,uint32_t*>;
template class FP<uint32_t,int64_t>;
template class FP<uint32_t,int64_t*>;
template class FP<uint32_t,uint64_t>;
template class FP<uint32_t,uint64_t*>;
template class FP<uint32_t,bool>;
template class FP<uint32_t,bool*>;
template class FP<uint32_t,float>;
template class FP<uint32_t,float*>;
template class FP<uint32_t,double>;
template class FP<uint32_t,double*>;

template class FP<uint32_t*,char>;
template class FP<uint32_t*,char*>;
template class FP<uint32_t*,int8_t>;
template class FP<uint32_t*,int8_t*>;
template class FP<uint32_t*,uint8_t>;
template class FP<uint32_t*,uint8_t*>;
template class FP<uint32_t*,int16_t>;
template class FP<uint32_t*,int16_t*>;
template class FP<uint32_t*,uint16_t>;
template class FP<uint32_t*,uint16_t*>;
template class FP<uint32_t*,int32_t>;
template class FP<uint32_t*,int32_t*>;
template class FP<uint32_t*,uint32_t>;
template class FP<uint32_t*,uint32_t*>;
template class FP<uint32_t*,int64_t>;
template class FP<uint32_t*,int64_t*>;
template class FP<uint32_t*,uint64_t>;
template class FP<uint32_t*,uint64_t*>;
template class FP<uint32_t*,bool>;
template class FP<uint32_t*,bool*>;
template class FP<uint32_t*,float>;
template class FP<uint32_t*,float*>;
template class FP<uint32_t*,double>;
template class FP<uint32_t*,double*>;

template class FP<int64_t,char>;
template class FP<int64_t,char*>;
template class FP<int64_t,int8_t>;
template class FP<int64_t,int8_t*>;
template class FP<int64_t,uint8_t>;
template class FP<int64_t,uint8_t*>;
template class FP<int64_t,int16_t>;
template class FP<int64_t,int16_t*>;
template class FP<int64_t,uint16_t>;
template class FP<int64_t,uint16_t*>;
template class FP<int64_t,int32_t>;
template class FP<int64_t,int32_t*>;
template class FP<int64_t,uint32_t>;
template class FP<int64_t,uint32_t*>;
template class FP<int64_t,int64_t>;
template class FP<int64_t,int64_t*>;
template class FP<int64_t,uint64_t>;
template class FP<int64_t,uint64_t*>;
template class FP<int64_t,bool>;
template class FP<int64_t,bool*>;
template class FP<int64_t,float>;
template class FP<int64_t,float*>;
template class FP<int64_t,double>;
template class FP<int64_t,double*>;

template class FP<int64_t*,char>;
template class FP<int64_t*,char*>;
template class FP<int64_t*,int8_t>;
template class FP<int64_t*,int8_t*>;
template class FP<int64_t*,uint8_t>;
template class FP<int64_t*,uint8_t*>;
template class FP<int64_t*,int16_t>;
template class FP<int64_t*,int16_t*>;
template class FP<int64_t*,uint16_t>;
template class FP<int64_t*,uint16_t*>;
template class FP<int64_t*,int32_t>;
template class FP<int64_t*,int32_t*>;
template class FP<int64_t*,uint32_t>;
template class FP<int64_t*,uint32_t*>;
template class FP<int64_t*,int64_t>;
template class FP<int64_t*,int64_t*>;
template class FP<int64_t*,uint64_t>;
template class FP<int64_t*,uint64_t*>;
template class FP<int64_t*,bool>;
template class FP<int64_t*,bool*>;
template class FP<int64_t*,float>;
template class FP<int64_t*,float*>;
template class FP<int64_t*,double>;
template class FP<int64_t*,double*>;

template class FP<uint64_t,char>;
template class FP<uint64_t,char*>;
template class FP<uint64_t,int8_t>;
template class FP<uint64_t,int8_t*>;
template class FP<uint64_t,uint8_t>;
template class FP<uint64_t,uint8_t*>;
template class FP<uint64_t,int16_t>;
template class FP<uint64_t,int16_t*>;
template class FP<uint64_t,uint16_t>;
template class FP<uint64_t,uint16_t*>;
template class FP<uint64_t,int32_t>;
template class FP<uint64_t,int32_t*>;
template class FP<uint64_t,uint32_t>;
template class FP<uint64_t,uint32_t*>;
template class FP<uint64_t,int64_t>;
template class FP<uint64_t,int64_t*>;
template class FP<uint64_t,uint64_t>;
template class FP<uint64_t,uint64_t*>;
template class FP<uint64_t,bool>;
template class FP<uint64_t,bool*>;
template class FP<uint64_t,float>;
template class FP<uint64_t,float*>;
template class FP<uint64_t,double>;
template class FP<uint64_t,double*>;

template class FP<uint64_t*,char>;
template class FP<uint64_t*,char*>;
template class FP<uint64_t*,int8_t>;
template class FP<uint64_t*,int8_t*>;
template class FP<uint64_t*,uint8_t>;
template class FP<uint64_t*,uint8_t*>;
template class FP<uint64_t*,int16_t>;
template class FP<uint64_t*,int16_t*>;
template class FP<uint64_t*,uint16_t>;
template class FP<uint64_t*,uint16_t*>;
template class FP<uint64_t*,int32_t>;
template class FP<uint64_t*,int32_t*>;
template class FP<uint64_t*,uint32_t>;
template class FP<uint64_t*,uint32_t*>;
template class FP<uint64_t*,int64_t>;
template class FP<uint64_t*,int64_t*>;
template class FP<uint64_t*,uint64_t>;
template class FP<uint64_t*,uint64_t*>;
template class FP<uint64_t*,bool>;
template class FP<uint64_t*,bool*>;
template class FP<uint64_t*,float>;
template class FP<uint64_t*,float*>;
template class FP<uint64_t*,double>;
template class FP<uint64_t*,double*>;

template class FP<float,char>;
template class FP<float,char*>;
template class FP<float,int8_t>;
template class FP<float,int8_t*>;
template class FP<float,uint8_t>;
template class FP<float,uint8_t*>;
template class FP<float,int16_t>;
template class FP<float,int16_t*>;
template class FP<float,uint16_t>;
template class FP<float,uint16_t*>;
template class FP<float,int32_t>;
template class FP<float,int32_t*>;
template class FP<float,uint32_t>;
template class FP<float,uint32_t*>;
template class FP<float,int64_t>;
template class FP<float,int64_t*>;
template class FP<float,uint64_t>;
template class FP<float,uint64_t*>;
template class FP<float,bool>;
template class FP<float,bool*>;
template class FP<float,float>;
template class FP<float,float*>;
template class FP<float,double>;
template class FP<float,double*>;

template class FP<float*,char>;
template class FP<float*,char*>;
template class FP<float*,int8_t>;
template class FP<float*,int8_t*>;
template class FP<float*,uint8_t>;
template class FP<float*,uint8_t*>;
template class FP<float*,int16_t>;
template class FP<float*,int16_t*>;
template class FP<float*,uint16_t>;
template class FP<float*,uint16_t*>;
template class FP<float*,int32_t>;
template class FP<float*,int32_t*>;
template class FP<float*,uint32_t>;
template class FP<float*,uint32_t*>;
template class FP<float*,int64_t>;
template class FP<float*,int64_t*>;
template class FP<float*,uint64_t>;
template class FP<float*,uint64_t*>;
template class FP<float*,bool>;
template class FP<float*,bool*>;
template class FP<float*,float>;
template class FP<float*,float*>;
template class FP<float*,double>;
template class FP<float*,double*>;

template class FP<double,char>;
template class FP<double,char*>;
template class FP<double,int8_t>;
template class FP<double,int8_t*>;
template class FP<double,uint8_t>;
template class FP<double,uint8_t*>;
template class FP<double,int16_t>;
template class FP<double,int16_t*>;
template class FP<double,uint16_t>;
template class FP<double,uint16_t*>;
template class FP<double,int32_t>;
template class FP<double,int32_t*>;
template class FP<double,uint32_t>;
template class FP<double,uint32_t*>;
template class FP<double,int64_t>;
template class FP<double,int64_t*>;
template class FP<double,uint64_t>;
template class FP<double,uint64_t*>;
template class FP<double,bool>;
template class FP<double,bool*>;
template class FP<double,float>;
template class FP<double,float*>;
template class FP<double,double>;
template class FP<double,double*>;

template class FP<double*,char>;
template class FP<double*,char*>;
template class FP<double*,int8_t>;
template class FP<double*,int8_t*>;
template class FP<double*,uint8_t>;
template class FP<double*,uint8_t*>;
template class FP<double*,int16_t>;
template class FP<double*,int16_t*>;
template class FP<double*,uint16_t>;
template class FP<double*,uint16_t*>;
template class FP<double*,int32_t>;
template class FP<double*,int32_t*>;
template class FP<double*,uint32_t>;
template class FP<double*,uint32_t*>;
template class FP<double*,int64_t>;
template class FP<double*,int64_t*>;
template class FP<double*,uint64_t>;
template class FP<double*,uint64_t*>;
template class FP<double*,bool>;
template class FP<double*,bool*>;
template class FP<double*,float>;
template class FP<double*,float*>;
template class FP<double*,double>;
template class FP<double*,double*>;

template class FP<char, char>;
template class FP<char, char*>;
template class FP<char, const char*>;

template class FP<char*, char>;
template class FP<char*, char*>;
template class FP<char*, const char*>;



