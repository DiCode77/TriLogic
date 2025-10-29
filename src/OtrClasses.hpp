//
//  OtrClasses.hpp
//  TriLogic
//
//  Created by DiCode77.
//

#ifndef OtrClasses_hpp
#define OtrClasses_hpp

#include <cstdlib>
#include <ctime>

namespace OTR {
class Rand{
    int max = 0;
public:
    Rand(int val) : max(val){};
    operator int(){
        std::srand(static_cast<int>(std::clock()));
        return std::rand() % this->max;
    }
};
}


#endif /* OtrClasses_hpp */
