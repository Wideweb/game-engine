function main(app)
    app:loadModel3D_v2("box", "./assets/models/box/box.fbx");

    app:setSkybox("./assets/models/skybox-2/right.png",
                  "./assets/models/skybox-2/left.png",
                  "./assets/models/skybox-2/top-2.png",
                  "./assets/models/skybox-2/bottom.png",
                  "./assets/models/skybox-2/back.png",
                  "./assets/models/skybox-2/front.png");

    local light = app:createEntity("light");
    light:addTagComponent(Core.TagComponent("light"));
    local lightProps = Core.DirectedLight();
    lightProps.ambient = Core.vec3(0.1, 0.1, 0.1);
    lightProps.diffuse = Core.vec3(0.3, 0.3, 0.3);
    lightProps.specular = Core.vec3(1.0, 1.0, 1.0);
    lightProps.farPlane = 30;
    light:addDirectedLightComponent(Core.DirectedLightComponent(lightProps));

    local box = app:createEntity("box");
    box:addLocationComponent(Core.LocationComponent(0.0, 0.0, 0.0));
    box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
    box:addTagComponent(Core.TagComponent("platform"));
    box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
end
