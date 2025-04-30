#### Scenes Problem Analysis

What am I working to fix?

- I need to be able to store collections of assets together with their component data included (aka a scene or a level). I also need to be able to
serialize that data to disk and load that data to disk. 

Important: Don't solve problems I don't have. Trying to predict future problems and
trying to solve them just makes more problems and complicates things.

Context:

Why am I solving this problem?

- I need to solve this problem, so I can save my work to disk and load it at later times. This allows me to maintain my
progress on a project and come back to it to work on it more or load the scene to show someone. Also, games are usually 
comprised of multiple different levels or areas to go to, so separating that data into collections to be loaded when
the player gets to that area (as opposed to loading the whole games assets at once) will help with keeping
memory lower and load times smaller.

Identify and Balance Constraints:

How much time to I have to make this?

- I am giving myself about two weeks to work on this. (It is about to be finals week, or it would be less)


How much research do I have to do on this?

- I have to do some research on how open world games stream in data vs loading scenes, so I can ease the transition when 
I do need to change to streaming data in and out. Also, doing research on what a scene consists of typically would 
give me a good baseline to work from when developing a scene concept within the context of Astral Engine.

How much memory can this take up?

- The scene will roughly take the same amount or a little bit more of memory compared to the ECS memory usage. 
This is because the scene will be basically composed of an ECS for entity data and of asset handles for unloading assets 
when the scene is unloaded. The asset handles though will take a smaller amount of memory compared to an ECS because 
asset handles are only 4 bytes

What is the performance budget?

- The scene itself is just a container for data, but in terms of serializing and deserializing scenes, it should be quick.
I am going to give myself a budget 1 ms for scenes with less than 10 objects, 5 ms for scenes with less than 50 objects
and the performance budget will scale like that with adding 1 ms for every 10 objects. For how I came to that number, I just
mentally compared how long a typical frame takes to render (16.67 ms) for 60 fps and came up with that budget from how quick
I expect the scene to be serialized or deserialized to prevent stutters or latency of any kind. To be clear, most of the
budget will come from actually loading the assets from disk. Also, note that this budget is intended for release builds and
that the actual scene load time will depend on how big the assets are and how many assets need to be loaded in vs are already
loaded from previous scenes.

What is the latency/overhead requirements?

- The actual latency felt by the user should be relatively small. The performance budget was in the milliseconds, so
loading scenes should be done pretty quickly. Scene loading will block the main thread (for now), but it should be done
pretty quickly and the user will have to wait for the scene to be loaded anyways before doing anything.


Will this be handled differently in different builds (Debug vs Release vs Dist)?

- Loading and unloading scenes as well as how scenes store data won't change from different builds. The only difference
would be that any logging related to scenes will be turned off.

What are the inputs and the outputs?

- The inputs will be a scene file on disk that contains the filepaths of assets in this scene, their scene load ID, and an
ECS for the scene that comes populated with data that the user generated from the editor previously or an empty ECS if the
scene is new.

- The outputs will be that the assets will be loaded into RAM and the scene will start to be rendered as well as ECS systems
start to act on the scene data. When the user is done, the scene data will be serialized onto disk.

What would the file format be?

- For scene serialization to a file, I could do something human-readable like yaml or json while working with the editor
for development ease but when you cook the game into an executable, change the scene files to binary files for performance
because after that point, the scene file won't be changing.

Value:

Identify specific, concrete, measurable value.



How much time is this problem worth?


What is the value of solving this problem?


Cost:

What is the cost of solving the problem?


Platform:

How will this handle the platform targets?


How will this interact with shared resources (RAM, SSD)?


Data:

What is read and written over time? What is the access pattern?


When is data accessed statistically relative to other data?


What data values are common? Outliers?



Possible Future Features:
- Streaming data for open worlds

