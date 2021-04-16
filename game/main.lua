function main(app)
    app:loadModel3D("man", "./assets/models/box/Man1.obj",
                    "./assets/models/box/diffuse-map.png",
                    "./assets/models/box/specular-map.png",
                    "./assets/models/box/normal-map.png");


    app:loadModel3D_v2("man2", "./assets/models/man/Man3.fbx");

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
    local location = Core.LocationComponent(10.0, 2.0, 0.0);
    player:addLocationComponent(location);
    player:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("man2", 0.5);
    render.rotation.x = -1.57;
    render.rotation.z = 1.57;
    player:addRender3DComponent(render);
    player:addCollisionComponent(Core.CollisionComponent(1.0, 2.0, 1.0));
    player:addPhysicsComponent(Core.PhysicsComponent(0.01));
    player:addCameraComponent(Core.CameraComponent(-8.0, 2.0, 0.0));
    player:addBehaviourComponent(Core.BehaviourComponent("./assets/entities/player.lua"));

    local light = app:createEntity("light");
    local lightProps = Core.Light();
    lightProps.quadratic = 0;
    lightProps.linear = 0;
    light:addLocationComponent(Core.LocationComponent(15.0, 5.0, 2.0));
    light:addLight3DComponent(Core.Light3DComponent(lightProps));

    local floor = app:createEntity("floor");
    floor:addLocationComponent(Core.LocationComponent(0.0, -20.0, 0.0));
    floor:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 20.0));
    floor:addStaticCollisionComponent(Core.StaticCollisionComponent(40.0, 40.0, 40.0));

    local box = app:createEntity("box");
    box:addLocationComponent(Core.LocationComponent(11.0, 0.5, -10.0));
    box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.5));
    box:addStaticCollisionComponent(Core.StaticCollisionComponent(1.0, 1.0, 1.0));

    local cottage = app:createEntity("cottage");
    cottage:addLocationComponent(Core.LocationComponent(20.0, 0.0, 2.0));
    cottage:addStaticRender3DComponent(Core.StaticRender3DComponent("cottage", 0.5));
    cottage:addStaticCollisionComponent(Core.StaticCollisionComponent(10.0, 10.0, 10.0));
end
