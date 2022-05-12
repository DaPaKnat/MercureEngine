#ifndef __MULTICAST_DELEGATE_BASE_HPP__
#define __MULTICAST_DELEGATE_BASE_HPP__

#include "DelegateBase.hpp"

/**
 * Forward declaration for the MultiCastDelegate class
 */
template <typename... ParamTypes>
class MultiCastDelegate;

/**
 * This is a MultiCastDelegate. They always returns void.
 */
template <typename... ParamTypes>
class MultiCastDelegate<void, ParamTypes...>
{
    public:

    // ============================== [Public Local Methods] ============================== //

        /**
         * Add a global method to the Delegate.
         *
         * @return  Is the method added to the Delegate?
         */
        template<void(*funcPtr)(ParamTypes...)>
        bool    Add     ()
        {
            return Add(DelegateBase<void(ParamTypes...)>::template CreateGlobal<funcPtr>());
        }

        /**
         * Add a member method to the Delegate.
         *
         * @return  Is the method added to the Delegate?
         */
        template<typename T, void(T::*funcPtr)(ParamTypes...)>
        bool    Add     (T* const                                   p_obj)
        {
            return Add(DelegateBase<void(ParamTypes...)>::template CreateMember<T, funcPtr>(std::make_shared<T*>(p_obj)));
        }

        /**
         * Add a functor to the Delegate.
         *
         * @return  Is the method added to the Delegate?
         */
        template<typename T>
        bool    Add     (std::shared_ptr<T> const&                  p_t)
        {
            return Add(DelegateBase<void(ParamTypes...)>::template CreateLambda(p_t));
        }

        /**
         * Update the Delegate with a new method.
         *
         * @return  Is the method added to the Delegate?
         */
        bool    Add     (DelegateBase<void(ParamTypes...)> const&   p_delegate)
        {
            if (std::find(m_delegates.begin(), m_delegates.end(), p_delegate) != m_delegates.end())
                return false;

            m_delegates.push_back(p_delegate);

            return true;
        }

        /**
         * Remove a global method from the Delegate.
         *
         * @return  Is the method removed from the Delegate?
         */
        template<void(*funcPtr)(ParamTypes...)>
        bool    Remove  ()
        {
            return Remove(DelegateBase<void(ParamTypes...)>::template CreateGlobal<funcPtr>());
        }

        /**
         * Remove a member method from the Delegate.
         *
         * @return  Is the method removed from the Delegate?
         */
        template<typename T, void(T::*funcPtr)(ParamTypes...)>
        bool    Remove  (T* const                                   p_obj)
        {
            return Remove(DelegateBase<void(ParamTypes...)>::template CreateMember<T, funcPtr>(std::make_shared<T*>(p_obj)));
        }

        /**
         * Remove a functor from the Delegate.
         *
         * @return  Is the method removed from the Delegate?
         */
        template<typename T>
        bool    Remove  (std::shared_ptr<T> const&                  p_t)
        {
            return Remove(DelegateBase<void(ParamTypes...)>::template CreateLambda(p_t));
        }

        /**
         * Remove a Delegate from the list.
         *
         * @return  Is the Delegate removed from the list?
         */
        bool    Remove  (DelegateBase<void(ParamTypes...)> const&   p_delegate)
        {
            auto it = std::find(m_delegates.begin(), m_delegates.end(), p_delegate);

            if (it == m_delegates.end())
                return false;

            m_delegates.erase(it);

            return true;
        }

        /**
         * Remove all Delegate from the list.
         */
        void    RemoveAll   ()
        {
            m_delegates.clear();

            return;
        }

        /**
         * Broadcast all Delegates from the list.
         */
        void    Broadcast   (ParamTypes...                          p_args)
        {
            for (auto& delegate : m_delegates)
                delegate(p_args...);

            return;
        }

    // ============================== [Public Local Operators] ============================== //

        /**
         * Broadcast all Delegates from the list.
         */
        void    operator()  (ParamTypes...  p_args)
        {
            for (auto& delegate : m_delegates)
                delegate(p_args...);

            return;
        }

    private:

    // ============================== [Private Local Properties] ============================== //

        /** Vector containing all Delegates to broadcast */
        std::vector<DelegateBase<void(ParamTypes...)>>  m_delegates;
};

#endif // !__MULTICAST_DELEGATE_BASE_HPP__