## Editor Interface

By default, the editor consist of the following tabs:
1. Scene
2. Scene Hierarchy
3. Scene Settings
4. Camera
5. Content Browser
6. Console
7. Inspector

<img width="1000" alt="Screenshot 2022-04-29 at 14 29 08" src="https://user-images.githubusercontent.com/3997537/165936202-43c64b20-e7ce-418c-8a4d-0caccb493052.png">

### 1. Scene
In the scene tab you can directly edit the contents of the scene: move, scale, rotate, add and remove objects. By default, the scene has directional light, camera and box.
  
  <img width="626" alt="Scene" src="https://user-images.githubusercontent.com/3997537/166137960-0a431aaf-b427-40db-b198-eae0bfde719b.png">
  
 **1.1 Selection.** Left-click on a scene object to select. Once it is selected you can see it's mesh and gizmo tools for edit. You can remove the selected object by backspace.
  
 **1.2 Navigation.** Use the following keys for navigation:
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

**1.3 Adding objects.** To add an object, right-click on the scene and select from the following list:
 - Cube
 - Plane
 - Empty
 - Empty 2D

### 2 Scene Hierarchy
 All scene objects are listed in the scene hierarchy tab as a tree.

 <img width="369" alt="Scene Hierarchy" src="https://user-images.githubusercontent.com/3997537/166555528-d0afaf33-aaaa-4a67-adce-ddd71d810c22.png">

 Here you can manage the parent relationship between objects:
 
 <img width="369" alt="sh-2" src="https://user-images.githubusercontent.com/3997537/161749791-568cf6a3-4a63-4ce0-a0d3-00dd4cdd844f.png">
 
 Once you have made the camera as a child object next to the box, the camera will move, rotate and scale with the box. 

 To add an additional behavior (component) to the object, right click on the object and select component from the list:
 
 <img width="366" alt="Add Entity Component" src="https://user-images.githubusercontent.com/3997537/166555887-3967cbf1-b1f1-40b0-90d7-e840fd7e6a7c.png">

 Once you have added the behaviour to the scene object, it's settings will appear in the inspector section.
 
 <img width="435" alt="Inspector" src="https://user-images.githubusercontent.com/3997537/166557265-dae2c2e3-ecd6-479f-9486-8984594f83c6.png">

 ### 3 Camera
 
 Draws the scene from the game camera's view point
 
 ### 4 Content Browser
 
 Displays files in the ./assets folder. Can be used to add:
 - Objects (.fbx, .obj) to the scene. To add an object to the scene, simply drag it from the content browser onto the scene tab.
 - Textures (.png, .jpg, .jpeg) to materials.
 - Fonts (.ttf).
 - Scripts (.Lua).
 
 ### 5 Scene Settings
 
 UI for Scene Settings:
 
 <img width="436" alt="Scene Settings UI" src="https://user-images.githubusercontent.com/3997537/166978347-bc567641-4c3e-4a1d-9e8b-663b6ae82834.png">

 In the scene settings you can switch off/on and edit:
 1. Gamma Correction
 2. HDR
 3. SSAO
 4. Normal Mapping
 5. Fog
 6. Skybox

 **5.1 Gamma Correction.** Allows you to apply monitor gamma inversion to the final color before displaying it on the monitor. A gamma of 2.2 is the default value and approximates the average gamma of most displays.
 
 UI for Gamma Correction:
 
 <img width="435" alt="Gamma Correction - UI" src="https://user-images.githubusercontent.com/3997537/166992870-903c5c10-6095-47d5-b782-4bc51d3c0e79.png">
 
 Scene when gamma is 1.0:

 <img width="500" alt="Screenshot 2022-05-05 at 21 33 19" src="https://user-images.githubusercontent.com/3997537/166995356-f794936c-c4ef-4230-9653-89f32018b54f.png">
 
 Scene when gamma is 2.2:
 
 <img width="500" alt="Screenshot 2022-05-05 at 21 33 03" src="https://user-images.githubusercontent.com/3997537/166995204-ae5d9acc-40cf-4cf0-8fc0-c8fb237b83cf.png">

 **5.2 HDR.** Use a wide range of brightness values to collect information about both bright and dark details of the scene.
 
 UI for HDR:
 
 <img width="434" alt="HDR - Settings" src="https://user-images.githubusercontent.com/3997537/166564867-8b0ce852-47f0-41b6-a262-4757bd9166df.png">
 
 | Property | Function |
 | :--- | :--- |
 | **Tone mapping**  | Algorithm for converting color values to fit them in the range from 0 to 1 without losing image details. |
 | **Exposure**  | Controls the balance between the display quality of dark and bright areas of the image. |
 | **Brightness Threshold**  | Brightness threshold, above which the fragment is considered bright. Bloom and Blur are applied to fragments that exceed this threshold. |
 | **Bloom Scale**  | Сontrols the degree of bloom. |
 | **Blur**  | Сontrols the degree of blur. |
 
 Scene when exposure is 1.0:
 
 <img width="500" alt="HDR - exposure 1.0" src="https://user-images.githubusercontent.com/3997537/166876630-6b5ea840-b838-4f92-a97f-46b7de42ed1f.png">

  Scene when exposure is 0.2:
  
  <img width="500" alt="HDR - exposure 0.2" src="https://user-images.githubusercontent.com/3997537/166876715-460315c8-2371-4b50-88a3-ae9556ca63c0.png">

 **5.3 SSAO.** Screen-Space Ambient Occlusion.

 UI For SSAO:

 <img width="433" alt="SSAO" src="https://user-images.githubusercontent.com/3997537/166560378-ee68ff65-3817-442e-96aa-ac53e32388f1.png">
 
| Property | Function |
| :--- | :--- |
| **Kernel size**  | The number of sample points located inside a hemisphere oriented along the normal to the surface. Each sample point is used as an offset to the fragment's position in view space to obtain information about the surrounding geometry. |
| **Radius**       | Controls the radius of the hemisphere. |
| **Noise scale**  | Controls the tiling of texture that contains a set of random hemisphere rotation vectors around Z axis. |
| **Bias**         | The value is added to the original fragment depth. Allows to control how the SSAO effect looks, and also, in certain situations, removes problems with ripples in shadow areas. |

 Scene without SSAO:

 <img width="500" alt="Scene without SSAO" src="https://user-images.githubusercontent.com/3997537/166872443-c4396811-3d6c-4c29-a80d-aa3d8a1e33b2.png">
 
 Scene with SSAO:

 <img width="500" alt="Scene with SSAO" src="https://user-images.githubusercontent.com/3997537/166872482-53cf1d73-db38-405c-ac1d-989ccb1ea1bb.png">

 **5.4 Normal Mapping.** Allows you to provide the lighting algorithm with normals that are unique for each fragment.
  
 Scene without Normal Mapping:
  
 <img width="300" alt="Normal Mapping Off" src="https://user-images.githubusercontent.com/3997537/166877677-44d68545-293d-4080-8ae8-4f10ccb1d968.png">
  
  Scene with Normal Mapping:
  
  <img width="300" alt="Normal Mapping On" src="https://user-images.githubusercontent.com/3997537/166877707-924a143d-6fe7-4772-956a-c65e43686b9d.png">
  
  **5.5 Fog.** Fog is the effect of overlaying a color onto objects dependant on the distance from the camera. Can be used to hide clipping of objects when a camera’s far clip plane has been moved forward for performance.
  
  UI for Fog:
  
  <img width="430" alt="Fog - Settings" src="https://user-images.githubusercontent.com/3997537/166561298-db2b74cc-db30-4ace-a440-4f83f6db0511.png">

  | Property | Function |
  | :--- | :--- |
  | **Color**     | The fog color in RGB. |
  | **Dencity**   | Controls the dencity of the fog. |
  | **Gradient**  | Controls the rate at which objects disappear in the fog. |
  
  Scene with Fog:
  
  <img width="500" alt="Fog - Scene" src="https://user-images.githubusercontent.com/3997537/166873290-90f89899-81cd-4a73-826b-3917df33c63b.png">
  
  **5.6 Skybox.** Skybox is a wrapper around entire scene that shows what the world looks like beyond geometry. Here you can specify the background color or set six faces of a cube.

  UI for Skybox:
  
  <img width="434" alt="Skybox - background color" src="https://user-images.githubusercontent.com/3997537/166562497-f2f38df2-e725-4297-a785-d8f6b09883ae.png">
  
  | Property | Function |
  | :--- | :--- |
  | **Color**     | The background color in RGB. |

  UI for Skybox When CubeMap is enabled:
  
  <img width="430" alt="Skybox - cubemap" src="https://user-images.githubusercontent.com/3997537/166562430-34b087f0-8f1b-413e-8aec-820a82379b8d.png">
  
  | Property | Function |
  | :--- | :--- |
  | **Upper Limit**     | The boundary up to which the fog is fade. Can take values [-1; 1]. Applicable if the fog is enabled. |
  | **Lower Limit**   | The boundary after which the fog begins to fade. Can take values [-1; 1]. Applicable if the fog is enabled. |
  | **Rotation**  | The initial value of the skybox rotation around the Y-axis. |
  | **Rotation Speed**  | The speed in degrees at which the skybox should rotate around the Y-axis. |

  Scene with Skybox:
  
  <img width="364" alt="Skybox - cubemap scene" src="https://user-images.githubusercontent.com/3997537/166562324-b966a7df-0edd-48f7-8460-750f0dbf9750.png">
    
### 6. Console

Displays errors:

<img width="630" alt="Console" src="https://user-images.githubusercontent.com/3997537/166566093-abe3c6da-b1e7-4026-8707-20bf4b50a400.png">


### 7. Inspector

Components:

  1. Transform
  2. Velocity
  3. Render 3D
  4. Skelet
  5. Rigitbody
  6. Box Collision
  7. Terrain Collision
  8. Particles
  9. Behaviour (Script)
  10. 2D Text
  11. Camera
  12. Directed Light
