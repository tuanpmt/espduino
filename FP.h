/**
 * @file    FP.h
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

#ifndef FP_H
#define FP_H

/** Example using the FP Class with global functions
 * @code
 *  #include "mbed.h"
 *  #include "FP.h"
 *  
 *  FP<void,bool>fp;
 *  DigitalOut myled(LED1);
 *  
 *  void handler(bool value)
 *  {
 *      myled = value;
 *      return;
 *  }
 *  
 *  int main()
 *  {
 *      fp.attach(&handler);
 *      
 *      while(1) 
 *      {
 *          fp(1);
 *          wait(0.2);
 *          fp(0);
 *          wait(0.2);
 *      }
 *  }
 * @endcode
 */
 
/** Example using the FP Class with different class member functions
 * @code
 *  #include "mbed.h"
 *  #include "FP.h"
 *  
 *  FP<void,bool>fp;
 *  DigitalOut myled(LED4);
 *  
 *  class Wrapper
 *  {
 *  public:
 *      Wrapper(){}
 *  
 *      void handler(bool value)
 *      {
 *          myled = value;
 *          return;
 *      }
 *  };
 *  
 *  int main()
 *  {
 *      Wrapper wrapped;
 *      fp.attach(&wrapped, &Wrapper::handler);
 *      
 *      while(1) 
 *      {
 *          fp(1);
 *          wait(0.2);
 *          fp(0);
 *          wait(0.2);
 *      }
 *  }
 * @endcode
 */
 
 /** Example using the FP Class with member FP and member function
 * @code
 *  #include "mbed.h"
 *  #include "FP.h"
 *  
 *  DigitalOut myled(LED2);
 *  
 *  class Wrapper
 *  {
 *  public:
 *      Wrapper()
 *      {
 *          fp.attach(this, &Wrapper::handler);
 *      }
 *  
 *      void handler(bool value)
 *      {
 *          myled = value;
 *          return;
 *      }
 *      
 *      FP<void,bool>fp;
 *  };
 *  
 *  int main()
 *  {
 *      Wrapper wrapped;
 *      
 *      while(1) 
 *      {
 *          wrapped.fp(1);
 *          wait(0.2);
 *          wrapped.fp(0);
 *          wait(0.2);
 *      }
 *  }
 * @endcode
 */

/**
 *  @class FP
 *  @brief API abstraction for a Function Pointers
 */ 
template<class retT, class argT>
class FP
{
public:
    /** Create the FP object
     */ 
    FP();

    /** Add a callback function to the class
     *  @param item - Address of the initialized class
     *  @param member - Address of the member function (dont forget the scope that the function is defined in)
     */
    template<class T>
    void attach(T *item, retT (T::*method)(argT))
    {
        obj_callback = (FPtrDummy *)(item);
        method_callback = (retT (FPtrDummy::*)(argT))(method);
        return;
    }

    /** Add a callback function to the class
     *  @param function - The address of a globally defined function
     */
    void attach(retT (*function)(argT));
    
    /** Invoke the function attached to the class
     *  @param arg - An argument that is passed into the function handler that is called
     *  @return The return from the function hanlder called by this class
     */
    retT operator()(argT arg) const;
    
    bool attached();
    
    void detach();

private:
    
    // empty type used for casting
    class FPtrDummy;
    
    FPtrDummy *obj_callback;
    
    /**
     *  @union Funciton
     *  @brief Member or global callback function
     */ 
    union
    {
        retT (*c_callback)(argT);                   /*!< Footprint for a global function */
        retT (FPtrDummy::*method_callback)(argT);   /*!< Footprint for a member function */
    };
};

#endif

