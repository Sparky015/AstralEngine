# Created by Andrew Fagan on 9/22/24.

# This script is to rename all the Ayla's in the root of header includes. For example "#include Ayla/Core/Application.h" needs to change to "#include Solas/Core/Application.h".
# All files in the codebase have includes that start with Ayla, so this script we go through and change them all to Solas.


# This must run in root directory.

import os

isInSourceDirectory = False # Checks to see if we should go deeper into the directory which we would if it is the source directory

# This function checks a given directory for "#include Ayla" and renames them to "#include Solas"
def CheckDirectory(directory):
    global isInSourceDirectory

    for item in os.listdir(directory):
        itemPath = os.path.join(directory, item)  # Get the full path of the item

        if os.path.isfile(itemPath):
            # file is actually a file. We are going to scan for "#include Ayla" now.

            if not itemPath.endswith(('.h', '.cpp')):
                continue

            with open(itemPath, 'r') as file:

                try:
                    content = file.read()
                    print(f"\n\nContents of {item}:")
                    print(content)
                    print("-" * 40)  # Separator for readability
                    
                    # Check if the content contains "#include Ayla"
                    if "#include Ayla" in content:
                        # Replace occurrences of "#include Ayla" with "#include Solas"
                        newContent = content.replace("#include Ayla", "#include Solas")

                        # Write the new content back to the file
                        with open(itemPath, 'w') as file:
                            file.write(newContent)
                        print(f"Updated: {itemPath}")
                     
                    
                except UnicodeDecodeError:
                    print(f"Skipping non-text file: {itemPath}")

                except Exception as e:
                    print(f"Error processing file {itemPath}: {e}")

        elif os.path.isdir(itemPath):
            # file is a directory. We are going to check the files in this directory now.

            # Make sure its a not a directory we don't care about 
            if item == "source":
                isInSourceDirectory = True
                CheckDirectory(itemPath)
                isInSourceDirectory = False

            elif isInSourceDirectory == True:
                CheckDirectory(itemPath)

            print("Checking directory: ", itemPath)
            
        else:
            print("Unknown item: ", itemPath)

    
# We start here with the first call to CheckDirectory and then recursively check each file in the Solas codebase.
CheckDirectory(os.path.curdir)

