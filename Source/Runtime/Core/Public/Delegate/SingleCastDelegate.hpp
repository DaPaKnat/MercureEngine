#ifndef __SINGLE_CAST_DELEGATE_HPP__
#define __SINGLE_CAST_DELEGATE_HPP__

#include "DelegateBase.hpp"

/**
 * Forward declaration for the SingleCastDelegate class
 */
template <typename... ParamTypes>
class SingleCastDelegate;

/**
 * This is a SingleCastDelegate with a non-void return value
 */
template <typename RetValType, typename... ParamTypes>
class SingleCastDelegate<RetValType, ParamTypes...>
{
    public:

    // ============================== [Public Local Methods] ============================== //

        /**
         * Bind a global method to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<RetValType(*funcPtr)(ParamTypes...)>
        bool        Bind            ()
        {
            return Bind(DelegateBase<RetValType(ParamTypes...)>::template CreateGlobal<funcPtr>());
        }

        /**
         * Bind a member method to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<typename T, RetValType(T::*funcPtr)(ParamTypes...)>
        bool        Bind            (T* const                                       p_obj)
        {
            return Bind(DelegateBase<RetValType(ParamTypes...)>::template CreateMember<T, funcPtr>(std::make_shared<T*>(p_obj)));
        }

        /**
         * Bind a functor to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<typename T>
        bool        Bind            (std::shared_ptr<T> const&                      p_t)
        {
            return Bind(DelegateBase<RetValType(ParamTypes...)>::template CreateLambda(p_t));
        }

        /**
         * Update the Delegate with a new method.
         *
         * @return  Is the method bound to the Delegate?
         */
        bool        Bind            (DelegateBase<RetValType(ParamTypes...)> const& p_delegate)
        {
            if (m_delegate == nullptr)
            {
                m_delegate.reset(new DelegateBase<RetValType(ParamTypes...)>(p_delegate));

                return true;
            }
            else if (!(*m_delegate == p_delegate))
            {
                m_delegate.reset(new DelegateBase<RetValType(ParamTypes...)>(p_delegate));

                return true;
            }

            return false;
        }

        /**
         * Remove the bound method from the Delegate.
         *
         * @return  Is the method unbound from the Delegate?
         */
        bool        Unbind          ()
        {
            if (m_delegate)
            {
                m_delegate.reset();

                return true;
            }

            return false;
        }

        /**
         * If a method is bound to this Delegate, execute it. Assertion in case of nullptr.
         *
         * @return  The method return value
         */
        RetValType  Execute         (ParamTypes...                                  p_args)
        {
            assert(m_delegate);

            return (*m_delegate)(p_args...);
        }

    // ============================== [Public Local Operators] ============================== //

        /**
         * If a method is bound to this Delegate, execute it. Assertion in case of nullptr.
         *
         * @return  The method return value
         */
        RetValType  operator()  (ParamTypes...  p_args)
        {
            assert(m_delegate);
            
            return (*m_delegate)(p_args...);
        }

    private:

    // ============================== [Private Local Properties] ============================== //

        /** Reference to the bound method stored in a DelegateBase. */
        std::shared_ptr<DelegateBase<RetValType(ParamTypes...)>>    m_delegate  { nullptr };
};

/**
 * This is a SingleCastDelegate that return void
 */
template <typename... ParamTypes>
class SingleCastDelegate<void, ParamTypes...>
{
    public:

    // ============================== [Public Local Methods] ============================== //

        /**
         * Bind a global function to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<void(*funcPtr)(ParamTypes...)>
        bool    Bind            ()
        {
            return Bind(DelegateBase<void(ParamTypes...)>::template CreateGlobal<funcPtr>());
        }

        /**
         * Bind a member method to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<typename T, void(T::*funcPtr)(ParamTypes...)>
        bool    Bind            (T* const                                   p_obj)
        {
            return Bind(DelegateBase<void(ParamTypes...)>::template CreateMember<T, funcPtr>(std::make_shared<T*>(p_obj)));
        }

        /**
         * Bind a functor to the Delegate.
         *
         * @return  Is the method bound to the Delegate?
         */
        template<typename T>
        bool    Bind            (std::shared_ptr<T> const&                  p_t)
        {
            return Bind(DelegateBase<void(ParamTypes...)>::template CreateLambda(p_t));
        }

        /**
         * Update the Delegate with a new method.
         *
         * @return  Is the method bound to the Delegate?
         */
        bool    Bind            (DelegateBase<void(ParamTypes...)> const&   p_delegate)
        {
            if (m_delegate == nullptr)
            {
                m_delegate.reset(new DelegateBase<void(ParamTypes...)>(p_delegate));

                return true;
            }
            else if (!(*m_delegate == p_delegate))
            {
                m_delegate.reset(new DelegateBase<void(ParamTypes...)>(p_delegate));

                return true;
            }

            return false;
        }

        /**
         * Remove the bound method from the Delegate.
         *
         * @return  Is the method unbound from the Delegate?
         */
        bool    Unbind          ()
        {
            if (m_delegate)
            {
                m_delegate.reset();

                return true;
            }

            return false;
        }

        /**
         * If a method is bound to this Delegate, execute it. Assertion in case of nullptr.
         */
        void    Execute         (ParamTypes...                              p_args)
        {
            assert(m_delegate);

            (*m_delegate)(p_args...);

            return;
        }

        /**
         * If a method is bound to this Delegate, execute it.
         *
         * @return  Has the method been called?
         */
        bool    ExecuteIfBound  (ParamTypes...                              p_args)
        {
            if (m_delegate != nullptr)
                (*m_delegate)(p_args...);

            return m_delegate != nullptr;
        }

    // ============================== [Public Local Operators] ============================== //

        /**
         * If a method is bound to this Delegate, execute it.
         */
        void    operator()  (ParamTypes...  p_args)
        {
            if (m_delegate)
                (*m_delegate)(p_args...);
        }

    private:

    // ============================== [Private Local Properties] ============================== //

        /** Reference to the bound method stored in a DelegateBase. */
        std::shared_ptr<DelegateBase<void(ParamTypes...)>>  m_delegate  { nullptr };
};

#endif // !__SINGLE_CAST_DELEGATE_HPP__