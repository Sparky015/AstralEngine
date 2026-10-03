#### Asset Manager Problem Analysis

What am I working to fix?

- I need a way to reference assets and a more efficient way of storing and loading asset data. 

Important: Don't solve problems I don't have. Trying to predict future problems and
trying to solve them just makes more problems and complicates things.

Context:

Why am I solving this problem?
- By making an asset manager, I am improving the efficiency of the memory use from loading assets. This prevents loading assets
multiple times, one time for each game object the asset is used on. This both improves the efficiency of asset loading because
I now only have to load the asset one time and then hand out references to the asset data. This also improves the memory use
because I now only need one copy of the asset in RAM instead of an arbitrary amount of copies of the same asset data
depending on how many times it is used. In conclusion, this will help me reduce memory waste and improve loading times.

Identify and Balance Constraints:

How much time to I have to make this?
- Probably about a week from planning to implementation if I was only working on this, but I have an exam this week, so
probably two weeks.

How much research do I have to do on this?

- I will do some research on asset managers and how they manage multiple different types of resources, however the scope of
this first iteration of the asset manager won't be that big because I only want to load an asset and return a handle to 
that asset for iteration 1.

How much memory can this take up?
- The amount of memory used will be dependent on the amount of assets loaded by the user. However, I expect the amount
of memory used by asset manager itself will be insignificant compared to the size of the assets being loaded in.

What is the performance budget?
- I don't know how I should budget this because the load time depends on the size of the asset but the asset manager
itself should add practically nothing on top of the loading time of assets.

What is the latency/overhead requirements?

- For loading asset data, I expect there to be some latency as loading an asset can take some time, but very little 
overhead from the asset manager itself.
- For accessing asset data, there should minimal overhead in getting the raw data from an asset handle.

Will this be handled differently in different builds (Debug vs Release vs Dist)?

- No, there will be no difference in behavior of the asset manager between debug, release, and dist builds.

What are the inputs and the outputs?

- The inputs into the asset manager will be the file path to the asset being loaded.
- The output from the asset manager will be a handle to the asset that can be used to access the raw data of the asset/resource.

Value:

Identify specific, concrete, measurable value.


How much time is this problem worth?

- No more than 2 weeks. I think the scope of what I want to do is relatively small compared to what it could be with
a fully-featured asset manager.

What is the value of solving this problem?

- The value I get from solving this problem will be reduced memory usage and faster loading times for scenes

Cost:

What is the cost of solving the problem?
- Predicted development time cost: ~2 weeks
- Maintenance cost: Minimal but each new resource will need to inherit from the Asset class and define overrides.

Platform:

How will this handle the platform targets?
- This shouldn't involve any OS specific code, however I need to monitor the amount of available RAM. That way, I can
warn the user and prevent out of memory errors.

How will this interact with shared resources (RAM, SSD)?
- The asset manager will take files from the SSD/HDD via file paths and load them into the RAM. Then the asset manager 
will hold the raw data of each asset and an asset handle that can be used to query the asset manager to get the raw asset data.

Data:

What is read and written over time? What is the access pattern?
- Initially, when a scene is being loaded in, each asset will have their data loaded into RAM from disk. This means that
there will be a big spike in data being written in as the assets in the scene loads in. After that, I don't expect there
to be any more data being written in (Unless the scene is really big, but that is out of scope for now). Now for memory
reads, I expect that most of the assets will have their data read each frame mainly due to rendering or some other system
that accesses game object data each frame. I think the access pattern will be rather random as some entities could reuse
the same asset data as well as the needs of which assets needing to be read will change each frame.

When is data accessed statistically relative to other data?

- I think all the resources of a certain type will be read at the same time because ECS systems iterate over all the components
of a certain type which will contain the same type of resources. Therefore, resources of the same type will likely be read
right after the other. In terms of just a random access read of a single asset's data, there will be one read when accessing the
key (which is the asset handle) and one read when returning the ptr to the raw asset data.

What data values are common? Outliers?

- I don't there are any obvious common data values. Each asset will only be loaded in one, which means the data in 
the asset manager will be unique and the reads of the asset data will depend on how the user is using the assets.


Possible Future Features:
1. Hot Reload Files/Assets
2. Async Asset Loading (Separate thread for loading)
3. UUIDs for asset IDs