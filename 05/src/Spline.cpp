#include "Spline.h"

// template<>
// std::tuple<glm::quat*, size_t> calculateCatmullRom(const size_t& cp_num, glm::quat* const &control_points, const bool& closed)
// {
//     glm::quat* spline_points = closed ? new glm::quat [cp_num * 3 +1] : new glm::quat [cp_num * 3 - 2];

//     for (size_t i = 0; i < cp_num; i++)
//     {
//         spline_points[3*i] = control_points[i];
//     }
//     if (closed)
//     {
//         spline_points[3*cp_num] = control_points[0];
//     }
    

//     for (size_t i = 0; i < cp_num - 1; i++)
//     {
//         if (i > 0) spline_points[3*i+1] = control_points[i] * glm::inverse(control_points[i-1]) * glm::slerp(control_points[i-1], control_points[i+1], 1.0f/6.0f);
//         else
//         {
//             spline_points[3*i+1] = closed ? control_points[i] * glm::inverse(control_points[cp_num-1]) * glm::slerp(control_points[cp_num-1], control_points[i+1], 1.0f/6.0f) : control_points[i];
//             // spline_points[3*i+1] = closed ? control_points[i] * glm::slerp(control_points[cp_num-1], control_points[i+1], 1.0f/6.0f) : control_points[i];
//         }

//         if (i < cp_num - 2) spline_points[3*i+2] = control_points[i+1] * glm::inverse(control_points[i+2]) * glm::slerp(control_points[i+2], control_points[i], 1.0f/6.0f);
//         else
//         {
//             if (closed)
//             {
//                 spline_points[3*i+2] = control_points[i+1] * glm::inverse(control_points[0]) * glm::slerp(control_points[0], control_points[i], 1.0f/6.0f);
//                 spline_points[3*i+4] = control_points[i+1] * glm::inverse(control_points[i]) * glm::slerp(control_points[i], control_points[0], 1.0f/6.0f);
//                 spline_points[3*i+5] = control_points[0] * glm::inverse(control_points[1]) * glm::slerp(control_points[1], control_points[i+1], 1.0f/6.0f);
//                 // spline_points[3*i+2] = control_points[i+1] * glm::slerp(control_points[0], control_points[i], 1.0f/6.0f);
//                 // spline_points[3*i+4] = control_points[i+1] * glm::slerp(control_points[i], control_points[0], 1.0f/6.0f);
//                 // spline_points[3*i+5] = control_points[0] * glm::slerp(control_points[1], control_points[i+1], 1.0f/6.0f);
//             }
//             else spline_points[3*i+2] = control_points[i+1];
//         }
        
//     }
//     return std::tuple<glm::quat*, size_t>(spline_points, closed ? cp_num * 3 + 1 : cp_num * 3 - 2);  
// }