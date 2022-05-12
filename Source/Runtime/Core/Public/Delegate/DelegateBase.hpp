#ifndef __DELEGATE_BASE_HPP__
#define __DELEGATE_BASE_HPP__

template<typename T>
class DelegateBase;

/**
 * This is the Delegate base class implementation
 *
 * @template RetValType Return value type
 * @template ParamTypes Delegate parameter types
 */
template <typename RetValType, typename... ParamTypes>
class DelegateBase<RetValType(ParamTypes...)>
{
    using CallbackType = RetValType(*)(std::shared_ptr<void>, ParamTypes...);

    public:

    // ============================== [Public Static Methods] ============================== //

        /**
         * Create a Delegate from a global method.
         *
         * @return  A DelegateBase with a global method bound to it.
         */
        template<RetValType(*funcPtr)(ParamTypes...)>
        static DelegateBase CreateGlobal    ()
        {
            return DelegateBase{ nullptr, &GlobalCaller<funcPtr> };
        }

        /**
         * Create a Delegate from a member method.
         *
         * @param   p_obj   A shared_ptr to the method owner.
         * @return  A DelegateBase with a member method bound to it.
         */
        template<typename T, RetValType(T::*funcPtr)(ParamTypes...)>
        static DelegateBase CreateMember    (std::shared_ptr<T*> const& p_obj)
        {
            return DelegateBase{ p_obj, &MemberCaller<T, funcPtr> };
        }

        /**
         * Create a Delegate from a functor.
         *
         * @param   p_t A shared_ptr containing a functor.
         * @return  A DelegateBase with a functor bound to it.
         */
        template<typename T>
        static DelegateBase CreateFunctor   (std::shared_ptr<T>         p_t)
        {
            return DelegateBase{ p_t, &FunctorCaller<T> };
        }
        
    // ============================== [Public Local Operators] ============================== //
        
        /**
         * Call the Delegate.
         */
        RetValType          operator()      (ParamTypes...          p_args)
        {
            return m_callback(m_callee, p_args...);
        }

        /**
         * Check if two Delegate are the same or not.
         *
         * @return  Are both DelegateBase the same?
         */
        bool                operator==      (DelegateBase const&    p_other)
        {
            return m_callback == p_other.m_callback;
        }

    private:
        
    // ============================== [Private Static Methods] ============================== //

        /**
         * Call a global method
         *
         * @return  The global method return value
         */
        template<RetValType(*funcPtr)(ParamTypes...)>
        static RetValType   GlobalCaller    (std::shared_ptr<void>, 
                                             ParamTypes...          p_paramTypes)
        {
            return funcPtr(p_paramTypes...);
        }

        /**
         * Call a member method
         *
         * @return  The member method return value
         */
        template<typename T, RetValType(T::*funcPtr)(ParamTypes...)>
        static RetValType   MemberCaller    (std::shared_ptr<void>  p_callee, 
                                             ParamTypes...          p_paramTypes)
        {
            return ((*(static_cast<T**>(p_callee.get())))->*funcPtr)(p_paramTypes...);
        }

        /**
         * Call a functor
         *
         * @return  The functor return value
         */
        template<typename T>
        static RetValType   FunctorCaller   (std::shared_ptr<void>  p_functor, 
                                             ParamTypes...          p_paramTypes)
        {
            return (*static_cast<T*>(p_functor.get()))(p_paramTypes...);
        }

    // ============================== [Private Local Properties] ============================== //

        /** Pointer to the object containing the method */
        std::shared_ptr<void>   m_callee    { nullptr };

        /** Callback on the Delegate method */
        CallbackType            m_callback  { nullptr };

    // ============================== [Private Constructors and Destructor] ============================== //

        /**
         * Default constructor
         */
        DelegateBase    (std::shared_ptr<void>  p_obj, 
                         CallbackType           p_funcPtr) : 
            m_callee    { p_obj }, 
            m_callback  { p_funcPtr }
        {}

}; // !class DelegateBase

#endif // !__DELEGATE_BASE_HPP__