## Lines of Code Command for Reference

#### Used to get an idea of the project scope if someone is not familiar with feature names.

------


#### Command
cloc Astral-Runtime/ Astral-Editor/ Scripts/ Tests/ --fullpath --not-match-d='(Astral-Runtime/Renderer/Assets/(?!Shaders).*|Astral-Runtime/Debug/ImGui/ImGuiDependencies|Tests/Samples|Tests/Astral-Runtime/Asset/TestAssets|Tests/Astral-Runtime/Scenes/TestScenes)' --not-match-f='(Astral-Runtime/Debug/ImGui/imgui-config\.ini|Astral-Editor/imgui-editor-config\.ini)'

#### Output
```
383 text files.
383 unique files.
25 files ignored.

#### github.com/AlDanial/cloc v 2.06  T=0.19 s (2063.4 files/s, 217614.2 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                            157           5384           1602          18751
C/C++ Header                   195           2717           2871           6662
GLSL                            24            375            155           1144
CMake                            4             85             11            523
Python                           1             20              2             75
DOS Batch                        1              0              1              9
Bourne Shell                     1              0              1              4
-------------------------------------------------------------------------------
SUM:                           383           8581           4643          27168
-------------------------------------------------------------------------------
```