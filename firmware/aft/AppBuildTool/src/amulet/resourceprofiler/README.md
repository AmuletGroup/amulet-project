Please note that there is a LARGE amount of dependence between our ResourceProfiler object and the Resource objects that it keeps track of for profiling/analysis purposes. 

Context
-------
- A context consists of a unique setting of stateful information. 
- Contexts should be entirely disjoint; any similarities should be captured in "redundant" context attributes. 

In our code, it is not uncommon to see blocks of code where the Resource Profiler is "set" to some context. 
All resources added to the profiler will be labeled with information from *that* context's attributes. When 
a context is no longer relevant, programmers should call to "clear" the context---this is nothing more than 
reseting the context to a "vanilla" state with no meaningful context.

Contexts that we currently support:
- NONE      : A "vanilla" or otherwise meaningless context (the default)
- STATE     : Represents QM code and resources that exist within (the context of) a QM State Node
- ACTION    : Represents QM code and resources that exist within (the context of) a QM Action Node
- GUARD     : Represents QM code and resources that exist within (the context of) a QM Guard Node
- OPERATION : Represents QM code and resources that exist within (the context of) a QM Operation Node
- GLOBAL    : Represents QM code and resources that exist within (the context of) a QM Class, but outside of a particular QM Node such as those listed above.

Terms
-------
- Lines of Code (LoC): the number of lines of code in a given block (e.g., entry code, exit code, transition handling code, for-loop, etc.).
