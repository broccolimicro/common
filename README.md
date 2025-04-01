# common

Common functions not provided by the C++ STL.

### Variable Mapping

The mapping class provides translation between different variable identifier spaces, which is essential when combining Boolean expressions from different contexts. It acts as a variable renaming mechanism, with bidirectional map and unmap operations for translating variable IDs. The class supports identity mapping (no translation), composition with other mappings, and reverse mapping generation. This functionality is particularly useful in hierarchical designs where different modules may have locally-defined variables that need consistent translation when integrated into a larger system.

```cpp
#include <boolean/mapping.h>
using namespace boolean;

// Create a mapping
mapping m;
m.set(0, 5);  // Map variable 0 to variable 5
m.set(1, 3);  // Map variable 1 to variable 3

// Apply mapping to a cube
cube c(0, 1);  // Variable 0 is true
c.apply(m);    // Now variable 5 is true

// Create reverse mapping
mapping reverse = m.reverse();  // Maps 5->0, 3->1
```

## License

Licensed by Cornell University under GNU GPL v3.

Written by Ned Bingham.
Copyright © 2020 Cornell University.

Haystack is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Haystack is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

A copy of the GNU General Public License may be found in COPYRIGHT.
Otherwise, see <https://www.gnu.org/licenses/>.

