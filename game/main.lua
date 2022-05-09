function main(app)
    app:loadFont("orange juice 2.0.ttf", "assets/fonts/orange-juice-2.0.ttf");

    app:load3D_v2("box", "./assets/models/box/box.fbx");

    app:setSkybox("./assets/models/skybox-2/right.png",
                  "./assets/models/skybox-2/left.png",
                  "./assets/models/skybox-2/top-2.png",
                  "./assets/models/skybox-2/bottom.png",
                  "./assets/models/skybox-2/back.png",
                  "./assets/models/skybox-2/front.png");

    local camera = app:createEntity("Camera");
    local cameraLocation = Core.LocationComponent(0.0, 3.0, 5.0);
    cameraLocation.rotation = Core.vec3(-0.5, 0.0, 0.0);
    camera:addLocationComponent(cameraLocation);
    camera:addCameraComponent(Core.CameraComponent());

    local light = app:createEntity("Directed Light");
    light:addLocationComponent(Core.LocationComponent(0.0, 3.0, -5.0));
    light:addDirectedLightComponent(Core.DirectedLightComponent(Core.DirectedLight()));

    -- for i = 1, 5 do
    --     local box = app:createEntity("Box");
    --     box:addLocationComponent(Core.LocationComponent(2.0 * i, 0.0, 0.0));
    --     box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
    --     box:addTagComponent(Core.TagComponent("platform"));
    --     box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    -- end

    local box = app:createEntity("Box");
    box:addLocationComponent(Core.LocationComponent(0.0, 0.0, 0.0));
    box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
    box:addTagComponent(Core.TagComponent("platform"));
    box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));

    local messageContainer = app:createEntity("MessageContainer");
    local messageContainerLocation = Core.Location2DComponent(0.0, 1.0);
    messageContainerLocation.ndc = true;
    messageContainer:addLocation2DComponent(messageContainerLocation);

    local message = app:createEntity("Message");
    message:addLocation2DComponent(Core.Location2DComponent(0.0, -40.0));
    message:addParentComponent(Core.ParentComponent(messageContainer:getId()));
    local messageText = Core.Text2DComponent("Hello World", "orange juice 2.0.ttf");
    messageText.color = Core.vec3(0.0, 0.0, 1.0);
    message:addText2DComponent(messageText);
end
