function main(app)
    app:loadModel3D_v2("frog", "./assets/models/man/Man8.fbx");
    app:loadModel3D_v2("beer", "./assets/models/Duck/Duck.fbx");

    app:loadModel3D("box", "./assets/models/box/box.obj",
                    "./assets/models/box/diffuse-map.png",
                    "./assets/models/box/specular-map.png",
                    "./assets/models/box/normal-map.png");

    -- app:loadModel3D("cottage", "./assets/models/cottage/cottage.obj",
    --                 "./assets/models/cottage/cottage_diffuse.png",
    --                 "./assets/models/cottage/cottage_specular.png",
    --                 "./assets/models/cottage/cottage_normal.png");

    app:setSkybox("./assets/models/skybox-2/right.png",
                  "./assets/models/skybox-2/left.png",
                  "./assets/models/skybox-2/top.png",
                  "./assets/models/skybox-2/bottom.png",
                  "./assets/models/skybox-2/back.png",
                  "./assets/models/skybox-2/front.png");

    local player = app:createEntity("player");
    local location = Core.LocationComponent(0.0, 2.0, 0.0);
    player:addLocationComponent(location);
    player:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("frog", 0.5);
    render.rotation.y = -1.57;
    player:addRender3DComponent(render);
    player:addCollisionComponent(Core.CollisionComponent(1.0, 1.0, 1.0));
    player:addPhysicsComponent(Core.PhysicsComponent(0.01));
    player:addCameraComponent(Core.CameraComponent(-6.0, 1.5, 0.0));
    player:addBehaviourComponent(Core.BehaviourComponent("./assets/entities/player.lua"));
    player:addSkeletComponent(Core.SkeletComponent("Armature|Run"));

    app:loadTerrain("terrain", "./assets/heightmap.png", 50, 50, 5.0);
    local terrain = app:createEntity("terrain");
    terrain:addLocationComponent(Core.LocationComponent(-50.0, 0.0, -50.0));
    terrain:addStaticRender3DComponent(Core.StaticRender3DComponent("terrain", 1));
    terrain:addTerrainCollisionComponent(Core.TerrainCollisionComponent(50, 50));

    local frog = app:createEntity("frog");
    frog:addLocationComponent(Core.LocationComponent(30.0, 0.0, 10.0));
    frog:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("frog", 1.0);
    render.rotation.y = 1.57;
    frog:addRender3DComponent(render);

    for i = 3, 5 do
        local box = app:createEntity("beer" .. i);
        box:addLocationComponent(Core.LocationComponent(20.0, 4.0, i * 2));
        box:addStaticRender3DComponent(Core.StaticRender3DComponent("beer", 10));
    end

    local light = app:createEntity("light");
    local lightProps = Core.DirectedLight();
    light:addDirectedLightComponent(Core.DirectedLightComponent(lightProps));

    for j = 0, 1 do

        for i = 0, 10 do
            local box = app:createEntity("box0" .. i);
            box:addLocationComponent(Core.LocationComponent(0.0, j * -2.0, i * 2));
            box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box1" .. i);
            box:addLocationComponent(Core.LocationComponent(i * 2, j * -2.0, 20.0));
            box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box2" .. i);
            box:addLocationComponent(Core.LocationComponent(20, j * -2.01, 20.0 - i * 2));
            box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box3" .. i);
            box:addLocationComponent(Core.LocationComponent(20.0 - i * 2, j * -2.0, 0.0));
            box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

    end

    for i = 1, 3 do
        local box = app:createEntity("box4" .. i);
        box:addLocationComponent(Core.LocationComponent(10, 2 * i, 20.0));
        box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
        box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    end

    for i = 1, 5 do
        local box = app:createEntity("box5" .. i);
        box:addLocationComponent(Core.LocationComponent(14, 2 * i, 20.0));
        box:addStaticRender3DComponent(Core.StaticRender3DComponent("box", 0.9));
        box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    end
end
