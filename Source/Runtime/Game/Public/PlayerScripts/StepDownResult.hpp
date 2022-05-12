#ifndef __STEP_DOWN_RESUT_HPP__
#define __STEP_DOWN_RESULT_HPP__

#include "FindFloorResult.hpp"

struct StepDownResult
{
    // ==============================[Public Local Properties]============================== //

        bool            m_computedFloor = false;

        FindFloorResult m_floorResult;

    // ==============================[Public Constructor + Destructor]============================== //

        StepDownResult  () = default;

        StepDownResult  (StepDownResult const& p_copy) = default;

        StepDownResult  (StepDownResult&&      p_move) = default;

        ~StepDownResult () = default;
    
    // ==============================[Public Local Operators]============================== //

        StepDownResult& operator=   (StepDownResult const&  p_copy) = default;
        StepDownResult& operator=   (StepDownResult&&       p_move) = default;
};

#endif // !__STEP_DOWN_RESULT_HPP__