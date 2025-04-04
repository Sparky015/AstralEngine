#### Asset Manager Research and Planning

How will the asset handle be represented?
- I am thinking that it will be a globally unique ID (GUID) which would mean a 8 byte unsigned int. I am thinking of taking
a hash of the filepath to the asset and storing that as the GUID which will then be used to reference that asset.


How will I handle hashing collisions?
- Eventually as the number of assets increases, the possibility of a hash collision increases (but is still low). When a
collision happens, how would I handle it? I want to let the user access asset data either using a file path or using an
asset handle (which is a GUID). I was thinking about storing the asset data in a hashmap with the GUID as the key and
a pointer to the asset data as the value. But for hashmaps, the keys have to be unique, so if there is a hash collision
on the GUID, then there are duplicate keys which will cause asset data to be overwritten. So how do I ensure that the 
GUID is truly unique. I need to have a process that can turn a filepath into a GUID because the user will need to 
be able to retrieve asset data either with the filepath or the GUID. I was going to just hash the filepath and then add 
the GUID to a hashmap (key: GUID, value: filepath) to mark as used. Then, if another filepath hashes to a GUID already used, I can add 1 until I get a 
GUID that is not used. Then, if the user tries to retrieve an asset using the filepath, I can hash the filepath and then
check the hashmap to see if that GUID has the same filepath as the one the user inputted. If it's not, then keep adding one
until the file path does match. There should a very small amount of hashing collisions anyway so it should still be O(1) 
with a good hashing function.

How will I store differently typed resources? (Texture vs Mesh vs Script, etc.)
- I am thinking that I am going to have a abstracted base class (Asset) that each resource type will inherit from. Then, I
can store pointers of type Asset in the hashmap. To retrieve the data, the user will have to enter a templated type to
request and the asset handle. Then I will get the asset pointer from the hashmap using the GUID and cast it to the
templated resource type the user entered. Then, the user can access the data in whatever format the resource is stored in and
using methods specific to that resource type. This way, I can also have a method that knows how to load that type of resource
from a file and which file extensions it can take.

How will I choose which resource to create from a file path?
- I will look at the file extension of the file and decide from that which type of resource to create. For example,
if the file extension is .obj or .fbx, I will create a MeshResource, and if the file extension is .png or .jpg, then I 
create a TextureResource, and if its .cs or .py, I create a ScriptResource, etc.


What is the error handling strategy?
- I am thinking that I am going to use a C style error handling strategy by returning error codes that come from an enum.
Like I can return a RESOURCE_MANAGER_FILE_NOT_FOUND when the file can't be found or RESOURCE_MANAGER_OUT_OF_MEMORY when the
memory is close to running out, and it can't load any more assets. It could return RESOURCE_MANAGER_INVALID_ASSET_TYPE_REQUEST when
the user requests a type that differs from the resource's actual type. It could return RESOURCE_MANAGER_SUCCESS when there were
no errors.