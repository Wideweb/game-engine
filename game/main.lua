function main(app)
    app:loadModel3D("man", "./assets/models/box/Man1.obj",
                    "./assets/models/box/diffuse-map.png",
                    "./assets/models/box/specular-map.png",
                    "./assets/models/box/normal-map.png");


    app:loadModel3D_v2("man2", "./assets/models/man/Man7.fbx");

    app:loadModel3D("box", "./assets/models/box/box.obj",
                    "./assets/models/box/diffuse-map.png",
                    "./assets/models/box/specular-map.png",
                    "./assets/models/box/normal-map.png");

    app:loadModel3D("cottage", "./assets/models/cottage/cottage.obj",
                    "./assets/models/cottage/cottage_diffuse.png",
                    "./assets/models/cottage/cottage_specular.png",
                    "./assets/models/cottage/cottage_normal.png");

    app:setSkybox("./assets/models/skybox/right.jpg",
                  "./assets/models/skybox/left.jpg",
                  "./assets/models/skybox/top.jpg",
                  "./assets/models/skybox/bottom.jpg",
                  "./assets/models/skybox/front.jpg",
                  "./assets/models/skybox/back.jpg");

    local player = app:createEntity("player");
    local location = Core.LocationComponent(0.0, 2.0, 0.0);
    player:addLocationComponent(location);
    player:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("man2", 0.6);
    render.rotation.y = -1.57;
    player:addRender3DComponent(render);
    player:addSkeletComponent(Core.SkeletComponent("Armature|Run"));
    player:addCollisionComponent(Core.CollisionComponent(1.0, 2.0, 1.0));
    player:addPhysicsComponent(Core.PhysicsComponent(0.01));
    player:addCameraComponent(Core.CameraComponent(-8.0, 1.5, 0.0));
    player:addBehaviourComponent(Core.BehaviourComponent("./assets/entities/player.lua"));

    local light = app:createEntity("light");
    local lightProps = Core.Light();
    lightProps.quadratic = 0;
    lightProps.linear = 0;
    light:addLocationComponent(Core.LocationComponent(15.0, 5.0, 2.0));
    light:addLight3DComponent(Core.Light3DComponent(lightProps));

    for i=0,20 do
        local box = app:createEntity("box" .. i);
        box:addLocationComponent(Core.LocationComponent(0.0, 0.0, i * 2));
        box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 1));
        box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    end

    local box = app:createEntity("boxn");
    box:addLocationComponent(Core.LocationComponent(0.0, 2.0, 8 * 2));
    box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 1));
    box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
end
