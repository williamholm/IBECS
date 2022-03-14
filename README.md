## IBECS
IBECS is an Entity Component System that stores component data in vectors that can be accessed via entity types which are an analouge to structs in standard C/C++
## Requirements
C++ 20, has only been tested on MSVC but nothing should be specific to it.
## How to Use
add all .hpp files not in the Test folder to your project and then include EntityManager.hpp in any file that you wish to use the ECS
# Adding Components
code
explain sort options
# Adding Entity Types
code
# Using EntityManager
code
explain best case / pitfalls
##credit
idea for using sparse sets in an ECS comes from https://github.com/skypjack/entt.
