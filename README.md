## Editor Interface

By default, the editor consist of the following tabs:
- Scene
- Scene Hierarchy
- Scene Settings
- Camera
- Content Browser
- Console
- Inspector

<img width="1000" alt="Screenshot 2022-04-29 at 14 29 08" src="https://user-images.githubusercontent.com/3997537/165936202-43c64b20-e7ce-418c-8a4d-0caccb493052.png">

### Scene
In the scene tab you can directly edit the contents of the scene: move, scale, rotate, add and remove objects. By default, the scene has directional light, camera and box.

  <img width="695" alt="Screenshot 2022-04-05 at 14 54 34" src="https://user-images.githubusercontent.com/3997537/161748346-c0ad915c-2376-463d-b516-69e2498072e1.png">
  
 **Selection.** Left-click on a scene object to select. Once it is selected you can see it's mesh and gizmo tools for edit. You can remove the selected object by backspace.
  
 **Navigation.** Use the following keys for navigation:
 - W - move forward
 - S - move back
 - A - move left
 - D - move right
 - E - move up
 - Q - move down
 - F - point camera at the selected object
 - X - point camera towards the positive x-axis
 - shift+X - point camera towards the negative x-axis
 - Y - point camera towards the positive y-axis
 - Shift+Y - point camera towards the negative y-axis
 - Z - point camera towards the positive z-axis
 - Shift+Z - point camera towards the negative z-axis
 - Space - rotate camera around the selected object

**Adding objects.** To add an object, right-click on the scene and select from the following list:
 - Cube
 - Plane
 - Empty
 - Empty 2D

### Scene Hierarchy
 All scene objects are listed in the scene hierarchy tab as a tree.

 <img width="368" alt="sh-1" src="https://user-images.githubusercontent.com/3997537/161749259-c1ca54d2-4e0c-4dcf-acb8-8f98b7914497.png">
 
 Here you can manage the parent relationship between objects:
 
 <img width="369" alt="sh-2" src="https://user-images.githubusercontent.com/3997537/161749791-568cf6a3-4a63-4ce0-a0d3-00dd4cdd844f.png">
 
 Once you have made the camera as a child object next to the box, the camera will move, rotate and scale with the box. it is possible to add additional behaviour to the object:
 
 <img width="214" alt="sh-3" src="https://user-images.githubusercontent.com/3997537/161751022-e66b732f-2ff0-4177-a4cf-48ab0ab72681.png">
 
 To add additional behavior (component), right click on the object and select from the list:

<Image>

 Once you have added the behaviour to the scene object, it's settings will appear in the inspector section.
 
 <img width="383" alt="sh-4" src="https://user-images.githubusercontent.com/3997537/161751821-c415a2d0-fefa-461e-b16b-a36b2ca2109b.png">

 ### Camera
 
 Draws the scene from the game camera's view point
 
 ### Content Browser
 
 Displays files in the ./assets folder. Can be used to add:
 - Objects (.fbx, .obj) to the scene. To add an object to the scene, simply drag it from the content browser onto the scene tab.
 - Textures (.png, .jpg, .jpeg) to materials.
 - Fonts (.ttf).
 - Scripts (.Lua).
 
 ### Scene Settings
 
 In the scene settings you can switch off/on and edit: 
 - Hdr
 - SSAO
 - Normal Mapping
 - Fog
 - Skybox
 
 For Hdr you can select tone mapping function, set exposure, gamma and threshold values, edit bloom and blur settings:
 
 <img width="332" alt="hdr" src="https://user-images.githubusercontent.com/3997537/161754845-03e1d362-c96c-4d31-a18e-2c486caeb071.png">
 <img width="255" alt="hdr-scene" src="https://user-images.githubusercontent.com/3997537/161757459-234fe2a1-55d4-43bb-8cf3-3bf549256246.png">

  When SSAO is enabled it is possible to edit it's settings:

 <img width="308" alt="ssao" src="https://user-images.githubusercontent.com/3997537/161755031-f4dc306b-b00c-49b0-aa40-e6060bae66cb.png">
 
  SSAO Off:
  
  <img width="200" alt="ssao-off" src="https://user-images.githubusercontent.com/3997537/161758409-0b78478c-ad59-4ca2-ac17-fbe392aaf032.png">
  
  SSAO On:
  
  <img width="200" alt="ssao-on" src="https://user-images.githubusercontent.com/3997537/161758488-5b5a8fa9-ade7-478b-8671-35ee54caea50.png">
 
  Normal Mapping Off:
  
  <img width="200" alt="normal-mapping-off" src="https://user-images.githubusercontent.com/3997537/161758022-3b652197-e0b9-4822-b261-9ad6edaee3d7.png">
  
  Normal Mapping On:
  
  <img width="200" alt="normal-mapping-on" src="https://user-images.githubusercontent.com/3997537/161758033-af2a06f4-5b34-481c-b1aa-8db8d1268053.png">


### Skybox
  In the skybox tab you can set the background color:

<img width="321" alt="Skybox" src="https://user-images.githubusercontent.com/3997537/161755293-b15aa8fd-76c9-435b-8a2f-141988a0f80a.png">

When CubeMap is enabled you can set the skybox textures, it's rotation and rotation speed:
  
  <img width="359" alt="skybox-2" src="https://user-images.githubusercontent.com/3997537/161755881-e11b7ae8-4ef8-4036-9caa-4fb65591028e.png">



