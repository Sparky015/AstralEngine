

#### -- Should I register types at compile time or lazily add them when the user uses them at runtime?

Lazily adding them would save memory for unused components, but registering at compile time
would save time at the runtime because then we don't have to do an allocation on the first use
of a component. But at the same time, the components are usually first used when making a scene
in the editor. Given that we have an editor, I don't think any new typed components will be added 
into the ECS at runtime. Of course all component types will need to be defined at compile time for
the editor too, but there shouldn't be a need to introduce a new type at runtime because
it would have already been handled already with the editor and, additionally, the editor will require
a compile time definition too.

#### -- How will the ECS handle loading scenes with already defined data for the components?
 
I think we can just move the deserialized ECS into the actual ECS when the scene is loading and that
will contain all the data.



#### -- Ok so considering that, why would I want to register types at runtime?

It might be easier to iterate and add and remove new types of components easier without having to register
and unregister components. But to be honest, that's not too much to do, and you shouldn't be adding and removing
new component types that often. Another thing might be that if you can register the types during runtime,
you are paying exactly how much you are using in terms of memory. But at the same time, the user still decides
which types to register and which to not register with a macro. Anything that can be done at runtime can also 
be done at compile time and naturally compile time will be better for performance.

#### -- What kind of data structure will store the components? Will all components be packed together?


If it is runtime, then we will need to have some sort of way to associate the type to an array or vector, so we will need
a hashmap to store each component pool and some way to use each type at a key without RTTI because that is too slow.

If it is compile time, then we can simply have a tuple of component pools. Each component pool will have a vector because
I want to store all the components of each type in their own contiguous data structure and will also have an accompanying
bitset that is the same size as the number of components which is used to tell if a component is being used.

Right now, before the implementation, I am using fixed size arrays for my component pools. However, this won't work well
if I want to dynamically add and remove entities over the course of the scene. Additionally, it can lead to wasted memory 
if I preallocate the component pools with a large number of components because it is fixed and I can't change it later.
Therefore, I am going to have to use vectors for the component pools, so it can be dynamically adjusted at runtime. 

However, there should be a medium-sized initial allocation for the component pools so there isn't too many allocations 
resizing up to the starting amount of entities. I should include an option to give a hint to the ECS regarding the 
expected number of static entities, so I can not over or under reserve the amount of components in the component pool. 
But at the same time, the expected use case is to author a scene in the editor with all the entity data, save it to 
disk, and then load the scene at runtime where the ECS will be loaded with all the entity data, so the number of 
entities will already be known. But maybe depending on the game, there can be a certain amount of extra reserved space
for dynamic objects to be added and removed without incurring allocations on the first x amount of dynamic entities added.


#### -- Archetypes vs Sparse Sets

If I have enough time to add this, which one is the go-to choice?

Sparse sets are made up of a sparse set which contains entity IDs and a dense set which contains the all the data for
a component type for all entities. Sparse sets help prevent wasting memory on components that don't get used very often.
The dense set only contains components that are actually being used, which is way better for cache utilization. Additionally, 
Sparse sets help prevent wasting memory reads to check if the entity is being used or not because the sparse set only
contains entity IDs which means more IDs will fit into a cache line compared to if I were checking by just components.
Mainly, it is very helpful to efficiently iterate over only the entities that are using components when returning a 
view of the all the components of a certain type. It helps with not wasting cache lines and memory reads for entity 
slots not in use or reading component slots that also aren't in use.

Archetypes are a different way of addressing cache optimizations.

It groups components together in a contiguous structure because so systems will request two components together at the 
same time. This means its better for memory reads because the cache line can grab both component 1 and component 2 for 
a system that wants to use components 1 & 2 of each entity that has it.

Sparse sets are good with single component requests but not as good with multiple component requests
compared to the archetypes. The archetypes are already filtered to entities with the exact set of components requested
at minimum while the sparse sets are not filtered like that and have to check if all the requested component slots for
an entity are actually in use. This means that sparse sets have a much higher chance at wasting and polluting cache lines
with components that aren't actually in use while the archetypes have already vetted that each entity in the archetype
has the requested set of components at a minimum.

So archetypes are lowkey better if there aren't a lot of entities being added or removed because naturally most
systems request multiple components.

However, archetypes suffer more when there are a lot of entities being added and removed because naturally archetypes
don't handle the holes as gracefully as sparse sets because when a component or entity is added or removed, the archetypes
will either have to suffer from wasted memory in the cache line or remove the entity from the archetype which requires shifting
entities around to maintain the integrity of the archetype.

Both sparse sets and archetypes will require some sort of indirection or look up table to efficiently index into a
entities component data. This is because sparse sets contain an index to the component data in the dense set and the
archetypes will require some index into the archetype to find the location of the entity's component data because the
archetypes naturally filter the entities.

I saw that Entt had a swap and pop thing which suggests that you could do this for archetypes in that you can 
efficiently add and remove entities from an archetype. But, overall, archetypes have more of an overhead when it comes
to adding and removing entities.

I was thinking that I can have an archetype section for static entities which also doesn't 
support adding or removing entities and a sparse sets section that is used for dynamic entity addition and removing.
But at the same time, archetypes are so much more complex to implement and I don't necessarily need that yet. Especially
mixing the two would be even more complex and its just necessary right now for the effort it takes.


#### -- How should I approach optimizing the systems part of the ECS?

I could use SIMD to process the multiple components at the same time. I could also use std::ranges with parallel execution
policies, though SIMD sounds more efficient and predictable and it would be nice to learn too. 
